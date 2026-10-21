# ESP32 專屬課程 — Wi-Fi Shell（互動式掃描與連線）

啟用 Zephyr 內建的 Wi-Fi shell，用指令列掃描 AP、連上家裡的 Wi-Fi、取得 DHCP IP、ping 外網。
這是 ESP32 在 Zephyr 上最大的賣點——本教學兩塊板子中只有它有 Wi-Fi。

## 學習目標

- 體驗 Zephyr 的 **shell 子系統**（指令列、tab 補全）
- Wi-Fi 管理層（`NET_L2_WIFI_MGMT`）與網路堆疊（IPv4/DHCP）的 Kconfig 組合
- 理解 binary blobs 的角色（Espressif 閉源 Wi-Fi 韌體）

## 前置需求

```bash
west blobs fetch hal_espressif    # 只需做一次；沒做會連結失敗
```

## 建置與燒錄

```bash
west build -p always -b esp32_devkitc_wroom/esp32/procpu esp32/wifi-shell
west flash && west espressif monitor
```

## 操作流程

```
uart:~$ wifi scan
Scan requested
Num  | SSID              | Chan | RSSI | Security
1    | MyHomeAP          | 6    | -45  | WPA2-PSK
...

uart:~$ wifi connect -s "MyHomeAP" -k 1 -p "mypassword"
Connection requested
Connected

uart:~$ net iface          ← 確認拿到 DHCP 發的 IP
uart:~$ net ping 8.8.8.8   ← 測試對外連線
```

`-k` 是加密方式：`0`=開放、`1`=WPA2-PSK（最常見）、`3`=WPA3-SAE。輸入 `wifi` + Enter 可看完整子指令。

## 已知風險（硬體驗證時注意）

- prj.conf 的 Kconfig 名稱以 Zephyr v4.x 為準，若建置失敗請比對
  `zephyr/samples/net/wifi/shell` 的設定（該範例就是本課的官方完整版）。
- ESP32 經典款只支援 **2.4GHz**，掃不到 5GHz-only 的 AP 是正常的。

## 延伸練習

1. 跑官方範例 `samples/net/wifi/shell` 比較差異。
2. 下一步進階：用 `net_mgmt(NET_REQUEST_WIFI_CONNECT, ...)` 寫程式開機自動連線，
   參考 `zephyr/samples/net/wifi/` 下的其他範例。
3. 連線成功後試試 `samples/net/sockets/http_get`，體驗完整的 TCP/HTTP。

## 硬體驗證清單（待辦）

- [ ] blobs 下載後建置通過
- [ ] `wifi scan` 掃得到 2.4GHz AP
- [ ] `wifi connect` 可連上並透過 DHCP 取得 IP，`net ping 8.8.8.8` 通
