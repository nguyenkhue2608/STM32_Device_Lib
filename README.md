# STM32_Device_Lib

Reusable peripheral / IC drivers for STM32, written on top of the STM32 HAL.
Drivers are grouped one level deep by category (`<category>/<DEVICE>/`), each
device folder exposing `<DEVICE>.h` + `<DEVICE>.c`. No `main.c`, no `.ioc`, no
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
- Naming: `<category>/` lowercase, `<DEVICE>/` folder == primary file basename ==
  API prefix stem (`sensor/DHT/DHT.c` → `DHT_*`). Exceptions: `RC522/` keeps the
  board name but its API is `MFRC522_*` (chip name); `util/delay_timer/` keeps
  `DELAY_TIM_*`.
- Header guards: `#ifndef __<NAME>_H`.

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

| Path | Bus / resource | Init signature (abridged) |
|---|---|---|
| `sensor/DHT/` | GPIO + TIM (input capture, 1 µs/tick) | `DHT_Init(&d, DHT11, &htim, TIM_CHANNEL_x, PORT, Pin)` |
| `sensor/DS18B20/` | GPIO (µs delay via DWT) | `DS18B20_Init(&d, PORT, Pin)` |
| `sensor/SRF05/` | GPIO + TIM (input capture, 1 µs/tick) | `SRF05_Init(&us, &htim, TIM_CHANNEL_x, TRIG_PORT, TRIG_Pin)` |
| `input/BUTTON/` | GPIO | `BUTTON_Init(&b, PORT, Pin)` |
| `input/JOYSTICK/` | ADC | `JOYSTICK_Init(&j, &hadc, chX, chY, ...)` |
| `input/KEYPAD/` | GPIO | `KEYPAD_Init(&kp, KEYMAP, row/col ports+pins)` |
| `display/SSD1306/` | I2C | `SSD1306_Init(&oled, &hi2c)` |
| `display/ST7735/` | SPI | `ST7735_Init()` (SPI handle set in `ST7735.h`) |
| `display/LED7SEG/` | GPIO | `LED7SEG_Init(&seg, type, ...)` |
| `display/CLCD/` | GPIO (4/8-bit HD44780) | `CLCD_4BIT_Init(&lcd, cols, rows, RS, EN, D4..D7)` |
| `display/CLCD_I2C/` | I2C (HD44780 backpack) | `CLCD_I2C_Init(&lcd, &hi2c, addr, cols, rows)` |
| `rtc/DS3231/` | I2C | `DS3231_Init(&rtc, &hi2c)` |
| `rfid/RC522/` | SPI | `MFRC522_Init(&rc, &hspi, CS_PORT, CS_Pin)` |
| `audio/DFPLAYER/` | UART | `DFPLAYER_Init(&mp3, &huart)` |
| `util/FLASH/` | internal flash | `FLASH_WritePage(...)`, `FLASH_ReadData(addr)` |
| `util/delay_timer/` | TIM | `DELAY_TIM_Init(&htim)` — standalone helper, no driver depends on it |

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
#include "../../lib/drivers/sensor/DHT/DHT.h"    /* Src/ layout      */
#include "../../../lib/drivers/sensor/DHT/DHT.h"  /* Core/Src/ layout */
```

Never copy a driver `.c` / `.h` into the project — always compile it from
`lib/drivers/<category>/<DEVICE>/`.

To pull driver updates:

```bash
cd lib/drivers && git pull origin master && cd ../..
git add lib/drivers && git commit -m "chore: bump drivers"
```
