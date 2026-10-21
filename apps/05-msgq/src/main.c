/*
 * 課程 05 — Message Queue：producer / consumer 模式
 *
 * 學習重點：
 *   1. k_msgq：固定大小訊息的 FIFO 佇列，執行緒安全
 *   2. producer / consumer 解耦：產生資料和處理資料的速度可以不同，
 *      佇列當緩衝
 *   3. 佇列滿 / 空時的阻塞行為與逾時處理
 *
 * 情境模擬：一個「感測器執行緒」每 100ms 產生一筆讀值（producer），
 * 一個「處理執行緒」每 300ms 才處理一筆（consumer，故意比較慢），
 * 觀察佇列被塞滿後 producer 的 drop 行為。
 */
#include <zephyr/kernel.h>

#define STACK_SIZE 1024

/* 訊息內容：一筆模擬的感測器讀值 */
struct sensor_msg {
	uint32_t seq;		/* 序號 */
	int32_t value;		/* 模擬讀值 */
	int64_t timestamp;	/* 產生時間（ms） */
};

/* 佇列：每格 sizeof(struct sensor_msg)，共 8 格，4-byte 對齊 */
K_MSGQ_DEFINE(sensor_q, sizeof(struct sensor_msg), 8, 4);

/* producer：每 100ms 產生一筆資料 */
static void producer_entry(void *p1, void *p2, void *p3)
{
	uint32_t seq = 0;
	uint32_t dropped = 0;

	while (1) {
		struct sensor_msg msg = {
			.seq = seq++,
			/* 模擬讀值：用序號做一個三角波 */
			.value = 100 + (int32_t)(seq % 50),
			.timestamp = k_uptime_get(),
		};

		/* K_NO_WAIT：佇列滿了就立刻返回錯誤，不等待。
		 * 這是感測器資料常見策略——舊資料沒人收就丟掉，
		 * 絕不讓感測迴圈被下游卡住。 */
		if (k_msgq_put(&sensor_q, &msg, K_NO_WAIT) != 0) {
			dropped++;
			printk("  [producer] 佇列已滿，丟棄 seq=%u（累計丟 %u 筆）\n",
			       msg.seq, dropped);
		}

		k_msleep(100);
	}
}

/* consumer：每 300ms 才處理一筆（故意比 producer 慢） */
static void consumer_entry(void *p1, void *p2, void *p3)
{
	struct sensor_msg msg;

	while (1) {
		/* K_FOREVER：佇列空了就睡著等，有資料立刻醒來 */
		k_msgq_get(&sensor_q, &msg, K_FOREVER);

		printk("[consumer] seq=%u value=%d 延遲=%lld ms（佇列還有 %u 筆）\n",
		       msg.seq, msg.value,
		       k_uptime_get() - msg.timestamp,
		       k_msgq_num_used_get(&sensor_q));

		k_msleep(300);	/* 模擬處理耗時 */
	}
}

K_THREAD_DEFINE(producer, STACK_SIZE, producer_entry, NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(consumer, STACK_SIZE, consumer_entry, NULL, NULL, NULL, 7, 0, 0);

int main(void)
{
	printk("=== 課程 05：Message Queue（producer 100ms / consumer 300ms）===\n");
	printk("觀察：佇列先逐漸累積，滿 8 筆後 producer 開始丟資料\n");
	return 0;
}
