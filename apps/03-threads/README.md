# 課程 03 — Threads（多執行緒）

建立兩個週期不同的執行緒，加上 main 共三個執行緒交錯執行，從序列輸出觀察排程行為。

## 學習目標

- 用 `K_THREAD_DEFINE()` 靜態建立執行緒（堆疊、優先權、進入函式）
- 理解 Zephyr 優先權模型：數字越小越優先，正值 = preemptible
- 理解 `k_msleep()` 讓出 CPU 的意義
- 知道 `main()` 本身也是一個執行緒

## 硬體

不需要任何接線，只看序列輸出。Pico 建議加 `-S cdc-acm-console` 讓 console 走 USB：

```bash
# Pico（console 走 USB，免 USB-TTL 線）
west build -p always -b rpi_pico -S cdc-acm-console apps/03-threads

# ESP32
west build -p always -b esp32_devkitc_wroom/esp32/procpu apps/03-threads
west flash && west espressif monitor
```

## 預期結果

```
=== 課程 03：多執行緒 ===
main() 也是執行緒，現在共有 main、A、B 三個應用執行緒
[     0 ms] main 執行緒：心跳
[     1 ms] 執行緒 A：第 1 次
[     2 ms] 執行緒 B：第 1 次
[   702 ms] 執行緒 B：第 2 次
[  1001 ms] 執行緒 A：第 2 次
[  1402 ms] 執行緒 B：第 3 次
...
```

## 延伸練習

1. 把執行緒 A 的優先權改成 5（比 B 高），再把兩個執行緒的 `k_msleep` 都拿掉，
   觀察 B 完全餓死（starvation）——高優先權不讓出 CPU 時低優先權永遠跑不到。
2. 用 `k_thread_create()` 在 main 裡「動態」建立第三個執行緒。
3. 在 prj.conf 加 `CONFIG_SCHED_CPU_MASK=y` 前先想想：這兩塊板子 Zephyr 預設都跑單核，
   多執行緒是「分時」而不是「平行」。

## 硬體驗證清單（待辦）

- [ ] Pico / ESP32：三個執行緒的輸出依時間戳正確交錯
