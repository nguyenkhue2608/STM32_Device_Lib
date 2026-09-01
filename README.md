# STM32_Device_Lib

Reusable peripheral / IC drivers for STM32, built on the STM32 HAL. Source only —
no `main.c`, no `.ioc`, no IDE project files. Runnable examples live in the
separate **STM32_Device_Exam** repo, which consumes this repo as a git submodule
at `lib/drivers/`.

- Target: STM32F1 (drivers include `stm32f1xx_hal.h` directly).
- Language: C99, no dynamic allocation.

## Layout

Drivers are grouped one level deep, **all names lowercase**:
`<category>/<device>/<device>.{c,h}`.

| Category | Devices |
|---|---|
| `sensor/` | `dht` `ds18b20` `srf05` |
| `input/` | `button` `joystick` `keypad` |
| `display/` | `ssd1306` `st7735` `led7seg` `char_lcd` `char_lcd_i2c` |
| `rtc/` | `ds3231` |
| `rfid/` | `rc522` |
| `audio/` | `dfplayer` |
| `util/` | `flash` `delay_timer` |
| `led/` `actuator/` `storage/` `comms/` `power/` `port/` | *(empty — reserved)* |

Every category folder has a `README.md` describing its scope and where
borderline devices go. `char_lcd` / `char_lcd_i2c` are the HD44780 character-LCD
driver (a.k.a. LCD1602 / LCD2004 / LiquidCrystal), parallel and PCF8574-I2C.

## Naming

Device folder == primary file basename. The public API prefix is that name
upper-cased: `sensor/dht/dht.c` → `DHT_Init()`, `display/char_lcd/` →
`CHAR_LCD_*`. Exception: `rfid/rc522/` exposes `MFRC522_*` (chip name).
Header guards: `#ifndef __<NAME>_H`.

Each driver exposes an `*_Init()` plus read/write functions. Usage, required
peripherals and wiring are documented at the top of every `<device>.h`.

## Using it from a project

Add the submodule once:

```bash
git submodule add https://github.com/nguyenkhue2608/STM32_Device_Lib.git lib/drivers
git submodule update --init --recursive
```

Include the driver directly from the submodule (never copy it into the project):

```c
#include "../../lib/drivers/sensor/dht/dht.h"
```

In the IDE project, compile `lib/drivers/<category>/<device>/<device>.c` and add
that folder to the include path.

Update the drivers later with:

```bash
cd lib/drivers && git pull origin master && cd ../..
git add lib/drivers && git commit -m "chore: bump drivers"
```
