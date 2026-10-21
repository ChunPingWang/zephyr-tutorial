# 課程 06 — Timer 與 Workqueue

週期性軟體計時器（到期回呼跑在中斷上下文）搭配 system workqueue
（耗時工作跑在執行緒上下文），完成「上半部 / 下半部」的經典中斷處理模式。

## 學習目標

- `K_TIMER_DEFINE()` + `k_timer_start()`：週期性軟體計時器
- 計時器回呼在**中斷上下文**執行的限制（不能睡眠、要快）
- `K_WORK_DEFINE()` + `k_work_submit()`：把工作交給 system workqueue
- 與課程 02（ISR → semaphore → 自建執行緒）比較：workqueue 是「不用自己養執行緒」的輕量選擇

## 建置

```bash
# Pico（console 走 USB）
west build -p always -b rpi_pico -S cdc-acm-console apps/06-timer-workqueue

# ESP32
west build -p always -b esp32_devkitc_wroom/esp32/procpu apps/06-timer-workqueue
west flash && west espressif monitor
```

## 預期結果

每 500ms 一組輸出：`[timer] 第 N 次到期...` 緊接著 `[work ] 第 N 次處理完成...`。

## 延伸練習

1. 把 work handler 裡的 `k_msleep(50)` 改成 `k_msleep(800)`（比計時器週期還長），
   觀察 work 來不及做、被合併的現象（處理次數 < 到期次數）。
2. 試著把 `k_msleep(50)` 搬進 `timer_expiry()` ——會直接 fault 或 assert，
   親身體會「中斷上下文不能睡眠」。
3. 用 `k_work_delayable` 實作「按鍵去彈跳」：中斷來了不處理，排程 20ms 後的 work 再讀腳位。

## 硬體驗證清單（待辦）

- [ ] timer 與 work 輸出成對出現，週期 500ms
- [ ] 練習 1 的合併行為符合預期
