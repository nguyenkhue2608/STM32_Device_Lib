# STM32_Device_Lib

Reusable peripheral / IC drivers for STM32, written on top of the STM32 HAL.
Drivers are grouped one level deep by category (`<category>/<device>/`), each
device folder exposing `<device>.h` + `<device>.c`. No `main.c`, no `.ioc`, no
IDE project files.

Runnable example projects live in the separate **STM32_Device_Exam** repo, which
pulls this repo in as a git submodule at `lib/drivers/`.

## Status / scope

- Target: **STM32F1** only. Every driver includes `stm32f1xx_hal.h` directly and
  calls HAL functions inline — there is no HAL abstraction layer yet. Porting to
  F4/G0/ESP today means editing the driver internals. A `port/` layer
  (function-pointer IO context for I2C/SPI drivers, thin timing backend for the
  bit-bang ones) is the planned next step — see `REFACTOR_CHECKLIST.md`.
- Language: C99. No `malloc` / `free`.
- Naming: **all folder and file names lowercase**. Device folder == primary file
  basename; the API prefix is that name upper-cased (`sensor/dht/dht.c` →
  `DHT_*`, `display/char_lcd/char_lcd.c` → `CHAR_LCD_*`). Exception: `rfid/rc522/`
  exposes `MFRC522_*` (chip name, not folder name).
- Header guards: `#ifndef __<NAME>_H` (upper-cased file stem).

## Categories

Every category exists even if empty; see each folder's `README.md` for its exact
scope and the tie-break rules (e.g. current monitor → `power/` not `sensor/`;
touch controller → `input/` not `display/`; buzzer → `audio/`; LED strip →
`led/` not `display/`).

```
sensor/    input/     display/   led/       actuator/  rtc/     storage/
rfid/      comms/     audio/     power/     util/      port/
```

## Drivers

| Path | API prefix | Bus / resource | Init signature (abridged) |
|---|---|---|---|
| `sensor/dht/` | `DHT_` | GPIO + TIM input capture, 1 µs/tick | `DHT_Init(&d, DHT11, &htim, TIM_CHANNEL_x, PORT, Pin)` |
| `sensor/ds18b20/` | `DS18B20_` | GPIO (µs delay via DWT) | `DS18B20_Init(&d, PORT, Pin)` |
| `sensor/srf05/` | `SRF05_` | GPIO + TIM input capture, 1 µs/tick | `SRF05_Init(&us, &htim, TIM_CHANNEL_x, TRIG_PORT, TRIG_Pin)` |
| `input/button/` | `BUTTON_` | GPIO | `BUTTON_Init(&b, PORT, Pin)` |
| `input/joystick/` | `JOYSTICK_` | ADC | `JOYSTICK_Init(&j, &hadc, chX, chY, ...)` |
| `input/keypad/` | `KEYPAD_` | GPIO | `KEYPAD_Init(&kp, KEYMAP, row/col ports+pins)` |
| `display/ssd1306/` | `SSD1306_` | I2C | `SSD1306_Init(&oled, &hi2c)` |
| `display/st7735/` | `ST7735_` | SPI | `ST7735_Init()` (SPI handle set in `st7735.h`) |
| `display/led7seg/` | `LED7SEG_` | GPIO | `LED7SEG_Init(&seg, type, ...)` |
| `display/char_lcd/` | `CHAR_LCD_` | GPIO (4/8-bit HD44780) | `CHAR_LCD_4BIT_Init(&lcd, cols, rows, RS, EN, D4..D7)` |
| `display/char_lcd_i2c/` | `CHAR_LCD_I2C_` | I2C (HD44780 + PCF8574) | `CHAR_LCD_I2C_Init(&lcd, &hi2c, addr, cols, rows)` |
| `rtc/ds3231/` | `DS3231_` | I2C | `DS3231_Init(&rtc, &hi2c)` |
| `rfid/rc522/` | `MFRC522_` | SPI | `MFRC522_Init(&rc, &hspi, CS_PORT, CS_Pin)` |
| `audio/dfplayer/` | `DFPLAYER_` | UART | `DFPLAYER_Init(&mp3, &huart)` |
| `util/flash/` | `FLASH_` | internal flash | `FLASH_WritePage(...)`, `FLASH_ReadData(addr)` |
| `util/delay_timer/` | `DELAY_TIM_` | TIM | `DELAY_TIM_Init(&htim)` — standalone helper, no driver depends on it |

`char_lcd/` is the HD44780 character-LCD driver (a.k.a. LCD1602 / LCD2004 /
LiquidCrystal) for 16×2, 20×4, 16×4, 8×2 modules; `char_lcd_i2c/` is the same
via a PCF8574 backpack.

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
#include "../../lib/drivers/sensor/dht/dht.h"    /* Src/ layout      */
#include "../../../lib/drivers/sensor/dht/dht.h"  /* Core/Src/ layout */
```

Never copy a driver `.c` / `.h` into the project — always compile it from
`lib/drivers/<category>/<device>/`.

To pull driver updates:

```bash
cd lib/drivers && git pull origin master && cd ../..
git add lib/drivers && git commit -m "chore: bump drivers"
```
