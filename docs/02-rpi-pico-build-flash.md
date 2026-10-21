# 02 — Raspberry Pi Pico 建置與燒錄

## 板名（board target）

| 板子 | board target |
|------|--------------|
| Pico（RP2040） | `rpi_pico` |
| Pico W | `rpi_pico/rp2040/w` |
| Pico 2（RP2350，Cortex-M33） | `rpi_pico2/rp2350a/m33` |

本教學以 `rpi_pico` 為準。

## 建置

```bash
west build -p always -b rpi_pico apps/01-blinky
```

成功後會產生 `build/zephyr/zephyr.uf2`。

## 燒錄方式一：UF2 拖放（最簡單，不需任何額外硬體）

1. 按住 Pico 上的 **BOOTSEL** 按鍵不放，插入 USB 線。
2. 電腦會出現一個名為 `RPI-RP2` 的隨身碟。
3. 把 `build/zephyr/zephyr.uf2` 拖進去（或用指令）：

```bash
cp build/zephyr/zephyr.uf2 /Volumes/RPI-RP2/
```

4. Pico 會自動重開並執行新韌體。

也可以用 west 的 UF2 runner（同樣要先進入 BOOTSEL 模式）：

```bash
west flash -r uf2
```

## 燒錄方式二：Debug Probe + OpenOCD（可除錯、不用拔插）

若有 Raspberry Pi Debug Probe（或第二塊 Pico 刷成 debugprobe）接上 SWD：

```bash
west flash -r openocd
west debug -r openocd    # GDB 除錯
```

## 看序列輸出（console）

Pico 預設 console 走 **UART0（GP0=TX, GP1=RX，115200 8N1）**，需要 USB-TTL 轉接線：

```
Pico GP0 (pin 1)  → USB-TTL RX
Pico GP1 (pin 2)  → USB-TTL TX
Pico GND (pin 3)  → USB-TTL GND
```

```bash
screen /dev/cu.usbserial-XXXX 115200    # 離開：Ctrl-A 再按 K
```

### 替代方案：USB CDC-ACM console（不需 USB-TTL 線）

建置時加上 snippet，console 會改走 Pico 自己的 USB 接口：

```bash
west build -p always -b rpi_pico -S cdc-acm-console apps/03-threads
```

燒錄後電腦會多出一個 `/dev/cu.usbmodem*` 裝置，直接 `screen` 連上即可。
純輸出類課程（03~06）建議用這個方式，最省事。
