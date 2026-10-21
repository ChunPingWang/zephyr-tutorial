# 01 — Zephyr 開發環境建置（macOS）

以下步驟以 macOS 為主（本專案開發環境），Linux 使用者可參考
[Zephyr 官方 Getting Started](https://docs.zephyrproject.org/latest/develop/getting_started/index.html)。

## 1. 安裝系統相依套件

```bash
brew install cmake ninja gperf python3 ccache qemu dtc libmagic wget openocd
```

## 2. 建立 Zephyr workspace

建議把 Zephyr 原始碼放在獨立的 workspace（例如 `~/zephyrproject`），
本教學專案的 app 都是「freestanding application」，可以放在 workspace 之外。

```bash
# 建立 Python 虛擬環境並安裝 west
python3 -m venv ~/zephyrproject/.venv
source ~/zephyrproject/.venv/bin/activate
pip install west

# 下載 Zephyr 與所有模組（第一次會花不少時間）
west init ~/zephyrproject
cd ~/zephyrproject
west update
west zephyr-export
west packages pip --install   # 舊版 Zephyr 用：pip install -r zephyr/scripts/requirements.txt
```

## 3. 安裝 Zephyr SDK（交叉編譯工具鏈）

```bash
cd ~/zephyrproject/zephyr
west sdk install
```

SDK 包含 ARM（Pico 用）、Xtensa（ESP32 用）、RISC-V（ESP32-C 系列用）等工具鏈。

## 4. ESP32 專用：下載 Espressif binary blobs

ESP32 的 Wi-Fi / BLE 功能依賴 Espressif 的閉源韌體，必須額外下載：

```bash
cd ~/zephyrproject
west blobs fetch hal_espressif
```

> 不做這一步的話，純 GPIO 範例仍可建置，但 Wi-Fi / BLE 相關課程（esp32/wifi-shell）會失敗。

## 5. 每次開發前的環境啟用

```bash
source ~/zephyrproject/.venv/bin/activate
```

之後就可以在本教學專案目錄下直接用 `west build` 建置各課程
（freestanding app 會透過 `west zephyr-export` 註冊的 CMake package 找到 Zephyr）。

## 6. 驗證安裝

```bash
cd /path/to/zephyr-rtos-tutorial
west build -p always -b rpi_pico apps/01-blinky
```

能產出 `build/zephyr/zephyr.uf2` 就代表環境建置成功。
