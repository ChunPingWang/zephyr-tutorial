# 課程 02 — Button（按鍵中斷切換 LED）

按一下按鍵，LED 切換一次亮滅。重點不是按鍵本身，而是 **ISR → 執行緒** 的標準溝通模式。

## 學習目標

- 設定 GPIO 輸入中斷（`gpio_pin_interrupt_configure_dt`、`gpio_add_callback`）
- 理解「ISR 只做最少的事」：ISR 裡只 `k_sem_give()`，其餘交給執行緒
- 用 `k_sem_take(K_FOREVER)` 讓執行緒零耗電等待事件
- 簡易軟體去彈跳（debounce）

## 硬體

| 板子 | 按鍵 | LED |
|------|------|-----|
| Pico | **需外接**：GP14（pin 19）─ 按鍵 ─ GND（pin 18） | 板載 GP25 |
| ESP32-DevKitC | 板上 **BOOT 鍵**（GPIO0），免接線 | GPIO2（同課程 01） |

## 建置與燒錄

```bash
# Pico
west build -p always -b rpi_pico apps/02-button

# ESP32
west build -p always -b esp32_devkitc_wroom/esp32/procpu apps/02-button
west flash
```

## 預期結果

每按一次按鍵，LED 切換一次，序列埠印出「第 N 次按壓，LED 切換」。

## 延伸練習

1. 把 `k_sem_reset()` 和 `k_msleep(200)` 註解掉，觀察按鍵彈跳造成一次按壓觸發多次。
2. 改用 `GPIO_INT_EDGE_BOTH`，分別偵測「按下」與「放開」（提示：用 `gpio_pin_get_dt()` 讀目前狀態）。
3. 思考：為什麼不直接在 ISR 裡 `printk` + `k_msleep`？（後者會直接當掉——ISR 不能睡眠）

## 硬體驗證清單（待辦）

- [ ] Pico：外接按鍵於 GP14，按壓可切換板載 LED
- [ ] ESP32：BOOT 鍵可切換 GPIO2 LED；確認 GPIO0 當輸入不影響開機（GPIO0 是 strapping pin，開機後當輸入是安全的）
