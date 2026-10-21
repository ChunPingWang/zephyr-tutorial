# 課程 04 — Mutex（互斥鎖與 race condition）

兩個執行緒各對同一個計數器累加 50 萬次。有鎖時結果正確（100 萬）；
把 `USE_MUTEX` 改成 0 重新建置，就能親眼看到更新丟失。

## 學習目標

- 理解 race condition 的成因（讀-改-寫不是原子操作）
- `k_mutex_lock()` / `k_mutex_unlock()` 的用法
- 臨界區要短：鎖住的時間越長，其他執行緒等越久

## 建置

```bash
# Pico（console 走 USB）
west build -p always -b rpi_pico -S cdc-acm-console apps/04-mutex

# ESP32
west build -p always -b esp32_devkitc_wroom/esp32/procpu apps/04-mutex
west flash && west espressif monitor
```

## 預期結果

`USE_MUTEX=1`：實際總和 = 1000000，印出「正確！」。
`USE_MUTEX=0`：實際總和 < 1000000，印出「丟失更新（race condition）！」
（數字每次跑都不一樣——這正是 race condition 可怕之處：結果不可重現）。

## 延伸練習

1. 把 `k_yield()` 拿掉再跑 `USE_MUTEX=0`，race 可能變得很難重現——
   想想為什麼（提示：同優先權執行緒沒有 time-slicing 時只在讓出點切換）。
2. 在 prj.conf 加 `CONFIG_TIMESLICING=y`、`CONFIG_TIMESLICE_SIZE=1`，
   讓同優先權執行緒每 1ms 強制輪替，race 會變得穩定重現。
3. 查一下 `k_mutex` 和 `k_sem`（binary semaphore）當鎖用的差別：
   mutex 有「擁有者」概念與優先權繼承（priority inheritance），sem 沒有。

## 硬體驗證清單（待辦）

- [ ] USE_MUTEX=1 時總和正確
- [ ] USE_MUTEX=0 時可觀察到更新丟失（必要時做練習 2）
