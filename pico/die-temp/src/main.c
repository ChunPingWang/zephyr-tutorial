/*
 * Pico 專屬課程 — Die Temperature：讀取 RP2040 晶片內建溫度計
 *
 * 學習重點：
 *   1. Zephyr sensor API 的標準三步驟：
 *      取得 device → sensor_sample_fetch() → sensor_channel_get()
 *   2. struct sensor_value 的定點數格式（val1 整數部 + val2 百萬分之一部），
 *      不需要浮點數 printf 支援
 *   3. devicetree alias（die-temp0）由 overlay 提供
 */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

/* overlay 裡定義的 alias：die-temp0 → RP2040 內建溫度感測器 */
static const struct device *const temp_dev = DEVICE_DT_GET(DT_ALIAS(die_temp0));

int main(void)
{
	printk("=== Pico 專屬課程：RP2040 晶片溫度 ===\n");

	if (!device_is_ready(temp_dev)) {
		printk("錯誤：溫度感測器 %s 尚未就緒\n", temp_dev->name);
		return 0;
	}

	while (1) {
		struct sensor_value val;
		int rc;

		/* 第一步：觸發一次取樣（驅動會去讀 ADC 通道 4） */
		rc = sensor_sample_fetch(temp_dev);
		if (rc != 0) {
			printk("取樣失敗（err %d）\n", rc);
			k_msleep(1000);
			continue;
		}

		/* 第二步：取出「晶片溫度」通道的值 */
		rc = sensor_channel_get(temp_dev, SENSOR_CHAN_DIE_TEMP, &val);
		if (rc != 0) {
			printk("讀值失敗（err %d）\n", rc);
			k_msleep(1000);
			continue;
		}

		/* sensor_value 是定點數：val1 = 整數部（°C），
		 * val2 = 小數部（單位：百萬分之一°C） */
		printk("晶片溫度：%d.%02d °C\n", val.val1, val.val2 / 10000);

		k_msleep(1000);
	}
	return 0;
}
