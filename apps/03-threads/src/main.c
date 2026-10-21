/*
 * 課程 03 — Threads：建立多個執行緒
 *
 * 學習重點：
 *   1. K_THREAD_DEFINE()：編譯期靜態建立執行緒
 *   2. 優先權：數字越小優先權越高；同優先權輪流
 *   3. k_msleep() 讓出 CPU 後，排程器挑下一個「就緒」的執行緒執行
 *   4. main() 本身也是一個執行緒（priority 0、可被搶占）
 *
 * 觀察序列輸出的時間戳，體會三個執行緒「同時」在跑的感覺。
 */
#include <zephyr/kernel.h>

#define STACK_SIZE 1024

/* 執行緒 A：每 1000ms 醒來一次 */
static void thread_a_entry(void *p1, void *p2, void *p3)
{
	int count = 0;

	while (1) {
		printk("[%6lld ms] 執行緒 A：第 %d 次\n", k_uptime_get(), ++count);
		k_msleep(1000);
	}
}

/* 執行緒 B：每 700ms 醒來一次——和 A 的週期錯開，
 * 輸出交錯的樣子就是排程器在工作 */
static void thread_b_entry(void *p1, void *p2, void *p3)
{
	int count = 0;

	while (1) {
		printk("[%6lld ms] 執行緒 B：第 %d 次\n", k_uptime_get(), ++count);
		k_msleep(700);
	}
}

/*
 * K_THREAD_DEFINE(識別名, 堆疊大小, 進入函式, 三個參數,
 *                 優先權, 選項, 啟動延遲 ms)
 * 優先權 7：preemptible（正值 = 可被搶占的一般執行緒）
 */
K_THREAD_DEFINE(thread_a, STACK_SIZE, thread_a_entry, NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(thread_b, STACK_SIZE, thread_b_entry, NULL, NULL, NULL, 7, 0, 0);

int main(void)
{
	printk("=== 課程 03：多執行緒 ===\n");
	printk("main() 也是執行緒，現在共有 main、A、B 三個應用執行緒\n");

	while (1) {
		printk("[%6lld ms] main 執行緒：心跳\n", k_uptime_get());
		k_msleep(3000);
	}
	return 0;
}
