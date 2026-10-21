# 03 — ESP32 建置與燒錄

## 板名（board target）

以最常見的 ESP32-DevKitC（WROOM-32 模組）為例：

```
esp32_devkitc_wroom/esp32/procpu
```

> **注意**：較新的 Zephyr 版本把板名改為 `esp32_devkitc/esp32/procpu`。
> 用 `west boards | grep esp32` 確認你的版本，下文指令請自行代換。
> `procpu` 指的是程式跑在 PRO CPU 核心上（ESP32 是雙核，Zephyr 預設用單核）。

其他家族成員：`esp32s3_devkitm`、`esp32c3_devkitm`（RISC-V 單核，無 `/procpu` 後綴）等。

## 前置需求：binary blobs

Wi-Fi / BLE 課程需要先下載 Espressif 閉源韌體（只需做一次）：

```bash
west blobs fetch hal_espressif
```

## 建置

```bash
west build -p always -b esp32_devkitc_wroom/esp32/procpu apps/01-blinky
```

## 燒錄

ESP32 DevKit 板上就有 USB-UART 橋接晶片，接上 USB 線即可：

```bash
west flash
```

west 會透過 esptool 自動偵測序列埠。若偵測失敗可手動指定：

```bash
west flash --esp-device /dev/cu.usbserial-0001
```

### 燒錄疑難排解

- **找不到序列埠**：macOS 可能需要安裝 CP210x 或 CH340 驅動（看板上的橋接晶片型號）。
- **連線逾時（Failed to connect）**：按住板上的 **BOOT** 鍵再執行 `west flash`，
  看到 `Connecting...` 之後放開。

## 看序列輸出（console）

console 走板上的 USB-UART，115200 8N1：

```bash
west espressif monitor          # 離開：Ctrl-]
# 或
screen /dev/cu.usbserial-0001 115200
```

## 已知差異

- ESP32-DevKitC 板上**沒有使用者 LED**（只有電源燈），所以 blinky 課程的 overlay
  把 `led0` 指到 **GPIO2**——許多相容板（如 NodeMCU-32S）GPIO2 上有板載藍色 LED；
  若你的板子沒有，請在 GPIO2 外接 LED + 330Ω 電阻到 GND。
- 板上的 **BOOT 按鍵接在 GPIO0**，button 課程直接拿它當輸入用，不需外接零件。
