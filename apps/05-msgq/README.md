# 課程 05 — Message Queue（producer / consumer）

感測器執行緒每 100ms 產一筆資料，處理執行緒每 300ms 收一筆，
用 `k_msgq` 當中間緩衝，觀察佇列累積、塞滿、丟棄的完整過程。

## 學習目標

- `K_MSGQ_DEFINE()` 定義固定格式的訊息佇列
- producer / consumer 解耦：兩端速度不同時佇列當緩衝
- 逾時策略：`K_NO_WAIT`（滿了就丟）vs `K_FOREVER`（空了就睡著等）
- `k_msgq_num_used_get()` 觀察佇列水位

## 建置

```bash
# Pico（console 走 USB）
west build -p always -b rpi_pico -S cdc-acm-console apps/05-msgq

# ESP32
west build -p always -b esp32_devkitc_wroom/esp32/procpu apps/05-msgq
west flash && west espressif monitor
```

## 預期結果

開頭 consumer 很快收到資料、佇列水位逐漸上升；約 3~4 秒後佇列滿 8 筆，
producer 開始印出「佇列已滿，丟棄 seq=...」，之後穩定維持「每收 1 筆、丟 2 筆」的節奏
（producer 速度是 consumer 的 3 倍）。

## 延伸練習

1. 把 producer 的 `K_NO_WAIT` 改成 `K_FOREVER`，行為變成「背壓（backpressure）」：
   producer 被迫慢下來配合 consumer，不再丟資料——兩種策略各適合什麼場景？
2. 把佇列加大到 32 格，觀察只是「延後」塞滿，速度不匹配的問題本質沒變。
3. 比較 `k_msgq`（定長複製）與 `k_fifo`（傳指標、變長）的使用時機。

## 硬體驗證清單（待辦）

- [ ] 佇列滿後 producer 開始丟資料，consumer 每收一筆印出佇列水位
