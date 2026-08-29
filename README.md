# STM32_Device_Lib

Reusable peripheral / IC drivers for STM32, written on top of the STM32 HAL.
One folder per device, each exposing `<name>.h` + `<name>.c` (display drivers also
carry font / bitmap assets). No `main.c`, no `.ioc`, no IDE project files.

Runnable example projects live in the separate **STM32_Device_Exam** repo, which
pulls this repo in as a git submodule at `lib/drivers/`.

## Status / scope

- Target: **STM32F1** only. Every driver includes `stm32f1xx_hal.h` directly and
  calls HAL functions inline — there is no HAL abstraction layer yet. Porting to
  F4/G0/ESP today means editing the driver internals. A `port/` layer
  (function-pointer IO context for I2C/SPI drivers, thin timing backend for the
  bit-bang ones) is the planned next step — see `REFACTOR_CHECKLIST.md`.
- Language: C99. No `malloc` / `free`.
- Public API is prefixed with the device name (`DHT_*`, `SSD1306_*`, …). The
  RC522 driver is the one exception: its folder/files are `RC522/` (the common
  breakout-board name) but the API is `MFRC522_*` (the chip name).
- Header guards: `#ifndef __<NAME>_H`.

## Drivers

| Folder | Bus / resource | Init signature (abridged) |
|---|---|---|
| `Button/` | GPIO | `BUTTON_Init(&b, PORT, Pin)` |
| `CLCD/` | GPIO (4/8-bit) | `CLCD_4BIT_Init(&lcd, cols, rows, RS, EN, D4..D7)` |
| `CLCD_I2C/` | I2C | `CLCD_I2C_Init(&lcd, &hi2c, addr, cols, rows)` |
| `DFPLAYER/` | UART | `DFPLAYER_Init(&mp3, &huart)` |
| `DHT/` | GPIO + TIM (input capture, 1 µs/tick) | `DHT_Init(&d, DHT11, &htim, TIM_CHANNEL_x, PORT, Pin)` |
| `DS18B20/` | GPIO (µs delay via DWT) | `DS18B20_Init(&d, PORT, Pin)` |
| `DS3231/` | I2C | `DS3231_Init(&rtc, &hi2c)` |
| `Delay_timer/` | TIM | `DELAY_TIM_Init(&htim)` — standalone helper, no driver depends on it |
| `FLASH/` | internal flash | `FLASH_WritePage(...)`, `FLASH_ReadData(addr)` |
| `JOYSTICK/` | ADC | `JOYSTICK_Init(&j, &hadc, chX, chY, ...)` |
| `KEYPAD/` | GPIO | `KEYPAD3X4_Init(&kp, KEYMAP, row/col ports+pins)` |
| `LCD_OLED_SSD1306/` | I2C | `SSD1306_Init(&oled, &hi2c)` |
| `LCD_TFT_ST7735/` | SPI | `ST7735_Init()` (SPI handle set in `ST7735_SPI.h`) |
| `LED7SEG/` | GPIO | `LED7SEG_Init(&seg, type, ...)` |
| `RC522/` | SPI | `MFRC522_Init(&rc, &hspi, CS_PORT, CS_Pin)` |
| `SRF05/` | GPIO + TIM (input capture, 1 µs/tick) | `SRF05_Init(&us, &htim, TIM_CHANNEL_x, TRIG_PORT, TRIG_Pin)` |

### Drivers that need an input-capture timer (`DHT`, `SRF05`)

The caller configures one timer channel in **Input Capture** mode:

- Prescaler = `TIMxCLK / 1_000_000 - 1` (1 µs per tick), Period = `0xFFFF`.
- The device signal pin must be the pin of that timer channel
  (`DHT` = DATA pin, `SRF05` = ECHO pin; `SRF05` TRIG stays a plain GPIO output).
- STM32F1 cannot capture both edges, so `DHT` captures falling edges only and
  measures the falling-to-falling period of each bit.

### DWT-based microsecond delay (`DS18B20`)

`DS18B20` generates its 1-Wire slots from the Cortex-M `DWT->CYCCNT` cycle
counter — no timer handle needed. Requires a Cortex-M3/M4/M7 core (fine on
F1/F4, absent on M0/M0+) and a correct `SystemCoreClock`.

## Using it from a project

```bash
# inside the consuming repo (STM32_Device_Exam)
git submodule add https://github.com/nguyenkhue2608/STM32_Device_Lib.git lib/drivers
git submodule update --init --recursive
```

Then include the driver and add its folder to the compiler include path:

```c
#include "../../lib/drivers/DHT/DHT.h"   /* Src/ layout      */
#include "../../../lib/drivers/DHT/DHT.h" /* Core/Src/ layout */
```

Never copy a driver `.c` / `.h` into the project — always compile it from
`lib/drivers/<device>/`.

To pull driver updates:

```bash
cd lib/drivers && git pull origin master && cd ../..
git add lib/drivers && git commit -m "chore: bump drivers"
```
