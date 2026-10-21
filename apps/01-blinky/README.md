# 課程 01 — Blinky（LED 閃爍）

嵌入式世界的 Hello World：讓板載 LED 每 500ms 閃一次。

## 學習目標

- 了解 devicetree **alias**（`led0`）如何做到「同一份程式碼、不同板子」
- 使用 `gpio_dt_spec` + `GPIO_DT_SPEC_GET()` 操作 GPIO
- 了解 `k_msleep()` 與裸機 delay loop 的差別（睡眠期間 CPU 讓給別人）

## 硬體

| 板子 | LED | 接線 |
|------|-----|------|
| Pico | 板載 LED（GP25） | 不需接線 |
| ESP32-DevKitC | GPIO2（由 overlay 定義） | 板上無 LED 時：GPIO2 → LED → 330Ω → GND |

## 建置與燒錄

```bash
# Raspberry Pi Pico
west build -p always -b rpi_pico apps/01-blinky
cp build/zephyr/zephyr.uf2 /Volumes/RPI-RP2/      # 先按住 BOOTSEL 插 USB

# ESP32
west build -p always -b esp32_devkitc_wroom/esp32/procpu apps/01-blinky
west flash
```

## 預期結果

LED 每 0.5 秒切換一次亮滅；序列埠（115200）會先印出一行啟動訊息。

## 延伸練習

1. 把閃爍週期改成 100ms，重新建置燒錄。
2. 在 overlay 裡把 ESP32 的 LED 改到別的腳位（例如 GPIO4），體會「改硬體不改程式碼」。
3. 看看 `build/zephyr/zephyr.dts`（最終合併的 devicetree），找到你的 `led0` 定義。

## 硬體驗證清單（待辦）

- [ ] Pico：LED 以 1Hz 閃爍（亮 0.5s / 滅 0.5s）
- [ ] ESP32：GPIO2 LED 閃爍；確認所用 Zephyr 版本的板名與 overlay 檔名相符
