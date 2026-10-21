/*
 * 課程 04 — Mutex：用互斥鎖保護共享資料
 *
 * 學習重點：
 *   1. race condition：兩個執行緒同時讀改寫同一個變數會丟失更新
 *   2. k_mutex 的 lock/unlock 用法
 *   3. 「臨界區（critical section）越短越好」的原則
 *
 * 把下面的 USE_MUTEX 改成 0 重新建置，就能親眼看到 race condition：
 * 最後的總和會小於預期值（兩個執行緒的 ++ 互相踩掉對方的結果）。
 */
#include <zephyr/kernel.h>

#define USE_MUTEX 1		/* 改成 0 可觀察 race condition */

#define STACK_SIZE 1024
#define LOOPS      500000	/* 每個執行緒累加的次數 */

static K_MUTEX_DEFINE(counter_mutex);
static volatile uint32_t shared_counter;

/* 用 semaphore 讓 main 知道兩個工作執行緒都做完了 */
static K_SEM_DEFINE(done_sem, 0, 2);

static void worker_entry(void *p1, void *p2, void *p3)
{
	const char *name = p1;

	printk("%s 開始累加 %d 次...\n", name, LOOPS);

	for (int i = 0; i < LOOPS; i++) {
#if USE_MUTEX
		k_mutex_lock(&counter_mutex, K_FOREVER);
#endif
		/* 臨界區：shared_counter++ 其實是「讀出 → 加一 → 寫回」
		 * 三個步驟。若中途被另一個執行緒插進來做同樣的事，
		 * 其中一次加一就會丟失。 */
		shared_counter++;
#if USE_MUTEX
		k_mutex_unlock(&counter_mutex);
#endif
		/* 每隔一段主動讓出 CPU，製造執行緒交錯的機會，
		 * 讓 race condition 更容易重現 */
		if ((i % 1000) == 0) {
			k_yield();
		}
	}

	printk("%s 完成\n", name);
	k_sem_give(&done_sem);
}

K_THREAD_DEFINE(worker1, STACK_SIZE, worker_entry, "worker-1", NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(worker2, STACK_SIZE, worker_entry, "worker-2", NULL, NULL, 7, 0, 0);

int main(void)
{
	printk("=== 課程 04：Mutex（USE_MUTEX=%d）===\n", USE_MUTEX);

	/* 等兩個 worker 都回報完成 */
	k_sem_take(&done_sem, K_FOREVER);
	k_sem_take(&done_sem, K_FOREVER);

	uint32_t expected = 2 * LOOPS;

	printk("預期總和：%u\n", expected);
	printk("實際總和：%u  → %s\n", shared_counter,
	       shared_counter == expected ? "正確！" : "丟失更新（race condition）！");
	return 0;
}
