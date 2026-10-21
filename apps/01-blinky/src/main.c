/*
 * 課程 01 — Blinky：讓 LED 每 500ms 閃爍一次
 *
 * 學習重點：
 *   1. devicetree alias（led0）如何讓同一份程式碼跨板使用
 *   2. gpio_dt_spec：Zephyr 操作 GPIO 的標準方式
 *   3. k_msleep()：讓目前執行緒睡眠（讓出 CPU）
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define SLEEP_TIME_MS 500

/*
 * DT_ALIAS(led0) 會在編譯期解析 devicetree 中的 "led0" alias。
 * - rpi_pico：板子定義檔已內建 led0（板載 LED，GP25）
 * - esp32_devkitc：板上沒有使用者 LED，由本課程 boards/ 下的 overlay
 *   把 led0 指到 GPIO2
 * 只要 alias 存在，這份程式碼完全不用改就能在任何板子上跑。
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

int main(void)
{
	int ret;

	/* 確認驅動已初始化完成（Zephyr 開機時會自動初始化各驅動） */
	if (!gpio_is_ready_dt(&led)) {
		printk("錯誤：LED 裝置 %s 尚未就緒\n", led.port->name);
		return 0;
	}

	/* 設為輸出，初始狀態為「點亮」（active 的意義由 devicetree 的
	 * GPIO_ACTIVE_HIGH/LOW 決定，程式碼不用管正負邏輯） */
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("錯誤：無法設定 LED 腳位（err %d）\n", ret);
		return 0;
	}

	printk("Blinky 啟動！LED 腳位：%s pin %d\n", led.port->name, led.pin);

	while (1) {
		gpio_pin_toggle_dt(&led);
		/* k_msleep() 期間本執行緒進入睡眠，CPU 可以跑其他執行緒
		 * 或進入省電的 idle 狀態——這就是 RTOS 和裸機
		 * busy-wait delay 最大的差別 */
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
