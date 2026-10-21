/*
 * 課程 02 — Button：按鍵中斷切換 LED
 *
 * 學習重點：
 *   1. GPIO 輸入與中斷（gpio_pin_interrupt_configure_dt）
 *   2. ISR（中斷服務常式）裡「只做最少的事」的原則
 *   3. 用 semaphore 把事件從 ISR 傳給執行緒處理（ISR → thread 的標準模式）
 *
 * ISR 執行時會搶占所有執行緒、且不能睡眠，所以耗時工作（印訊息、
 * 去彈跳延遲等）都應該丟回執行緒做。這裡用 k_sem 當「事件信號」。
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

static struct gpio_callback button_cb_data;

/* 計數型 semaphore：初始 0、上限 1（重複按壓不累積） */
static K_SEM_DEFINE(button_sem, 0, 1);

/* 這個函式在「中斷上下文」執行——保持越短越好 */
static void button_pressed_isr(const struct device *dev,
			       struct gpio_callback *cb, uint32_t pins)
{
	k_sem_give(&button_sem);	/* 只發信號，其他事交給 main 執行緒 */
}

int main(void)
{
	int press_count = 0;

	if (!gpio_is_ready_dt(&button) || !gpio_is_ready_dt(&led)) {
		printk("錯誤：GPIO 裝置尚未就緒\n");
		return 0;
	}

	gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	gpio_pin_configure_dt(&button, GPIO_INPUT);

	/* EDGE_TO_ACTIVE：訊號變成「作用態」的那一瞬間觸發中斷。
	 * 按鍵是 active-low（接 GND、內部上拉）還是 active-high，
	 * 由 devicetree overlay 決定，程式碼一樣不用管。 */
	gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);

	gpio_init_callback(&button_cb_data, button_pressed_isr, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);

	printk("Button 範例啟動！按下按鍵切換 LED（%s pin %d）\n",
	       button.port->name, button.pin);

	while (1) {
		/* 睡著等待 ISR 發信號——不耗任何 CPU */
		k_sem_take(&button_sem, K_FOREVER);

		gpio_pin_toggle_dt(&led);
		press_count++;
		printk("第 %d 次按壓，LED 切換\n", press_count);

		/* 簡易軟體去彈跳：處理完後睡 200ms，
		 * 並把彈跳期間 ISR 多給的信號清掉 */
		k_msleep(200);
		k_sem_reset(&button_sem);
	}
	return 0;
}
