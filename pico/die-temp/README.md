# Pico 專屬課程 — Die Temperature（RP2040 晶片溫度）

每秒讀一次 RP2040 內建的晶片溫度感測器（接在 ADC 通道 4），不需任何外接零件。

## 學習目標

- Zephyr **sensor API** 標準用法：`sensor_sample_fetch()` → `sensor_channel_get()`
- `struct sensor_value` 定點數格式（避開嵌入式最忌諱的浮點 printf）
- 用 overlay 把板子預設 disabled 的硬體（`&dietemp`、`&adc`）打開

## 建置與燒錄

```bash
# console 走 USB 最方便
west build -p always -b rpi_pico -S cdc-acm-console pico/die-temp
cp build/zephyr/zephyr.uf2 /Volumes/RPI-RP2/
```

## 預期結果

```
=== Pico 專屬課程：RP2040 晶片溫度 ===
晶片溫度：27.35 °C
晶片溫度：27.82 °C
...
```

用手指按住 RP2040 晶片，應能看到溫度緩慢上升 1~2 度。

## 已知風險（硬體驗證時注意）

- devicetree 節點 label `dietemp` 以 Zephyr v4.x 為準；若建置報
  「label not found」，請比對 `zephyr/samples/sensor/die_temp_polling/boards/`
  中 rpi_pico 的 overlay 寫法並修正本課 overlay。

## 延伸練習

1. 溫度超過 30°C 時點亮板載 LED（複習課程 01 的 GPIO）。
2. 把讀值透過課程 05 的 msgq 丟給另一個執行緒處理（整合練習）。
3. RP2040 溫度計精度不高（±數度），查 datasheet 看看轉換公式
   （27°C 時約 0.706V，每升 1°C 降 1.721mV）。

## 硬體驗證清單（待辦）

- [ ] 建置通過（確認 `dietemp` label 正確）
- [ ] 讀值合理（室溫 ±5°C），手指加熱有反應
