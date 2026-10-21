/*
 * 課程 06 — Timer 與 Workqueue：中斷上下文的「上/下半部」模式
 *
 * 學習重點：
 *   1. k_timer：軟體計時器，到期回呼在「中斷上下文」執行（不能睡眠！）
 *   2. k_work：把耗時工作丟給 system workqueue 執行緒處理
 *   3. timer 回呼（上半部，快）→ k_work_submit → work handler（下半部，可以慢）
 *      這正是課程 02 ISR→semaphore 模式的另一種常用寫法
 *
 * 情境模擬：每 500ms 計時器到期（想像成感測器定時取樣中斷），
 * 真正的「資料處理」交給 workqueue 做。
 */
#include <zephyr/kernel.h>

static uint32_t tick_count;

/* ---------- 下半部：在 system workqueue 執行緒裡執行 ---------- */
static void heavy_work_handler(struct k_work *work)
{
	int64_t start = k_uptime_get();

	/* 這裡在執行緒上下文，可以睡眠、可以做耗時運算 */
	k_msleep(50);	/* 模擬 50ms 的資料處理 */

	printk("[work ] 第 %u 次處理完成（在 workqueue 執行緒，耗時 %lld ms）\n",
	       tick_count, k_uptime_get() - start);
}

static K_WORK_DEFINE(heavy_work, heavy_work_handler);

/* ---------- 上半部：timer 到期回呼，在中斷上下文執行 ---------- */
static void timer_expiry(struct k_timer *timer)
{
	tick_count++;
	printk("[timer] 第 %u 次到期（中斷上下文，只做提交）\n", tick_count);

	/* 不能在這裡 k_msleep()！只把工作提交給 workqueue。
	 * 若上一次工作還沒跑完，重複 submit 會被合併成一次。 */
	k_work_submit(&heavy_work);
}

static K_TIMER_DEFINE(sample_timer, timer_expiry, NULL);

int main(void)
{
	printk("=== 課程 06：Timer + Workqueue ===\n");

	/* 啟動週期計時器：1 秒後第一次到期，之後每 500ms 一次 */
	k_timer_start(&sample_timer, K_SECONDS(1), K_MSEC(500));

	/* main 沒事做了，睡長覺。計時器和 workqueue 都由 kernel 驅動 */
	while (1) {
		k_sleep(K_FOREVER);
	}
	return 0;
}
