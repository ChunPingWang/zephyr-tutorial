# Zephyr RTOS 教學專案（Raspberry Pi Pico + ESP32）

以 Raspberry Pi Pico（RP2040）與 ESP32 兩塊開發板為實作平台的 Zephyr RTOS 入門教學。

> **狀態**：程式碼已撰寫完成，**尚未在實體硬體上驗證**。各課程 README 末尾附有驗證清單。

## 目錄結構

```
zephyr-rtos-tutorial/
├── docs/                        # 環境建置與燒錄說明
│   ├── 01-environment-setup.md  # Zephyr 開發環境安裝（west、SDK、blobs）
│   ├── 02-rpi-pico-build-flash.md
│   └── 03-esp32-build-flash.md
├── apps/                        # 跨板共用課程（同一份程式碼，兩塊板子都能建置）
│   ├── 01-blinky/               # LED 閃爍：GPIO 輸出、devicetree alias
│   ├── 02-button/               # 按鍵中斷：GPIO 輸入、ISR → semaphore → thread
│   ├── 03-threads/              # 多執行緒：K_THREAD_DEFINE、優先權、排程
│   ├── 04-mutex/                # 互斥鎖：共享資料保護、race condition 示範
│   ├── 05-msgq/                 # 訊息佇列：producer / consumer 模式
│   └── 06-timer-workqueue/      # 軟體計時器與工作佇列：ISR 下半部處理模式
├── pico/                        # Raspberry Pi Pico 專屬課程
│   └── die-temp/                # 讀取 RP2040 內建晶片溫度感測器（sensor API）
└── esp32/                       # ESP32 專屬課程
    └── wifi-shell/              # Wi-Fi shell：互動式掃描與連線 AP
```

### 為什麼不是「一塊板子一個資料夾」各放一份完整課程？

Zephyr 的核心設計是 **devicetree 硬體抽象**：程式碼透過 alias（如 `led0`、`sw0`）取得硬體，
板子之間的差異由各 app 內的 `boards/<board>.overlay` 檔案吸收。因此：

- **共用課程（`apps/`）**：一份 `main.c` 同時支援兩塊板子，只在需要時附上板子專屬的
  overlay。這也是 Zephyr 官方 samples 的組織方式，能直接教到「可移植性」這個最重要的概念。
- **板子專屬課程（`pico/`、`esp32/`）**：只有該板才有的硬體功能（RP2040 晶片溫度計、
  ESP32 Wi-Fi）才獨立成板子資料夾。

## 快速開始

1. 依照 [docs/01-environment-setup.md](docs/01-environment-setup.md) 安裝 Zephyr 開發環境。
2. 建置第一個範例（在 Zephyr workspace 已啟用的 shell 中）：

```bash
# Raspberry Pi Pico
west build -p always -b rpi_pico apps/01-blinky

# ESP32（舊版 Zephyr 板名為 esp32_devkitc_wroom，新版為 esp32_devkitc；
# 用 `west boards | grep esp32` 確認你的版本用哪個）
west build -p always -b esp32_devkitc_wroom/esp32/procpu apps/01-blinky
```

3. 燒錄方式見 [docs/02-rpi-pico-build-flash.md](docs/02-rpi-pico-build-flash.md) 與
   [docs/03-esp32-build-flash.md](docs/03-esp32-build-flash.md)。

## 課程列表

| 課程 | 主題 | Pico | ESP32 | 需要外部零件 |
|------|------|:----:|:-----:|--------------|
| apps/01-blinky | GPIO 輸出、devicetree alias | ✅ | ✅ | 無（ESP32 板上無 LED 時需外接） |
| apps/02-button | GPIO 中斷、ISR → thread 溝通 | ✅ | ✅ | Pico 需外接按鍵（GP14） |
| apps/03-threads | 執行緒建立、優先權、排程 | ✅ | ✅ | 無 |
| apps/04-mutex | 互斥鎖、race condition | ✅ | ✅ | 無 |
| apps/05-msgq | 訊息佇列、producer/consumer | ✅ | ✅ | 無 |
| apps/06-timer-workqueue | k_timer、workqueue | ✅ | ✅ | 無 |
| pico/die-temp | sensor API、RP2040 晶片溫度 | ✅ | — | 無 |
| esp32/wifi-shell | Wi-Fi 掃描與連線 | — | ✅ | 無 |

建議學習順序：先用任一塊板子照順序走完 `apps/01` ~ `apps/06`，再做自己板子的專屬課程。

## 版本假設

- Zephyr **v4.x**（以 v4.1/v4.2 的 API 撰寫）。
- 板名與 devicetree 節點在不同 Zephyr 版本可能微調，若建置失敗請先用
  `west boards` 確認板名，並比對 `zephyr/samples/` 內的對應官方範例。
