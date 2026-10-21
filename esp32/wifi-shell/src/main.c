/*
 * ESP32 專屬課程 — Wi-Fi Shell：互動式掃描與連線
 *
 * 這一課不寫 Wi-Fi 程式碼，而是啟用 Zephyr 內建的 Wi-Fi shell，
 * 讓你用指令列互動操作 Wi-Fi——先把「流程」玩熟，之後要寫程式
 * 自動連線時（net_mgmt API），每個 API 都對應到這裡的一條指令。
 *
 * 常用指令（在序列埠終端機輸入）：
 *   wifi scan                        掃描附近的 AP
 *   wifi connect -s "SSID" -k 1 -p "密碼"   連線（-k 1 = WPA2-PSK）
 *   wifi status                      查看連線狀態
 *   wifi disconnect                  斷線
 *   net iface                        查看網路介面與取得的 IP
 *   net ping 8.8.8.8                 測試對外連線
 */
#include <zephyr/kernel.h>

int main(void)
{
	printk("\n=== ESP32 專屬課程：Wi-Fi Shell ===\n");
	printk("試試以下指令：\n");
	printk("  wifi scan\n");
	printk("  wifi connect -s \"你的SSID\" -k 1 -p \"你的密碼\"\n");
	printk("  wifi status\n");
	printk("  net iface\n");
	printk("  net ping 8.8.8.8\n\n");
	return 0;
}
