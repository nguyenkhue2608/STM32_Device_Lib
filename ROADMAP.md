# Roadmap — drivers to add

Wishlist of common maker / breakout modules, organised by the existing
categories. `✓` = already in the repo. Add a driver only when there is a real
need; keep the folder / file / API-prefix naming rule from `README.md`.

```
sensor/                     ✓ dht   ✓ ds18b20   ✓ srf05
├── bmp280 / bme280 / bme680      temperature-humidity-pressure (I2C/SPI)
├── aht10 / aht20 / sht31         temperature-humidity (I2C)
├── mpu6050 / mpu9250 / icm20948  6/9-axis IMU (I2C)
├── adxl345                       accelerometer
├── hmc5883l / qmc5883l           magnetometer / compass
├── bh1750 / tsl2561 / max44009   ambient light (lux)
├── apds9960                      gesture / colour / proximity
├── vl53l0x / vl53l1x             ToF laser distance
├── gp2y0a21                      Sharp IR distance (analog)
├── hx711                         load cell / scale (24-bit ADC)
├── mh-z19                        CO2 (UART)
├── ccs811 / sgp30                VOC / eCO2
├── mq-x                          gas sensors (analog)
├── hc-sr501 / am312              PIR motion
├── tcs34725                      colour sensor
├── ky-038 / max9814              microphone level (analog)
├── soil_moisture / water_level   analog
└── yf-s201                       water flow (pulse)

input/                      ✓ button   ✓ joystick   ✓ keypad
├── rotary_encoder               KY-040 / EC11
├── ttp223                       capacitive touch button
├── xpt2046                      resistive touch controller (for TFT)
├── ft6236 / gt911              capacitive touch panel
└── nav_switch                  5-way navigation switch

display/                    ✓ ssd1306  ✓ st7735  ✓ st7789  ✓ led7seg  ✓ led7_6pin  ✓ char_lcd  ✓ char_lcd_i2c
├── ili9341 / ili9488            SPI TFT 2.4"-3.5"
├── st7796                       IPS 320x480  (st7789 already done)
├── sh1106                       1.3" OLED
├── ssd1327 / ssd1351            grayscale / colour OLED
├── pcd8544                      Nokia 5110
├── st7920                       128x64 graphic LCD
├── max7219                      8-digit 7-seg / 8x8 matrix
├── tm1637 / tm1638             2-wire 7-seg / LED&KEY module
├── ht16k33                      LED backpack (7/14-seg, 8x8)
└── ssd1680 / il0373            e-paper 2.13" / 2.9"

led/                        (empty)
├── ws2812 / sk6812             addressable RGB (timing / SPI / PWM-DMA)
├── apa102 / sk9822             SPI RGB
├── ws2811
├── rgb_led                     3-pin PWM RGB
├── sr_74hc595                  shift-out (LED / 7-seg)
└── is31fl3731                  LED matrix driver

actuator/                   (empty)
├── servo                       50 Hz PWM  (extract from the SERVO example)
├── pca9685                     16-ch PWM / servo (I2C)
├── dc_motor                    L298N / L293D / DRV8833 / TB6612
├── stepper                     A4988 / DRV8825 / TMC2209 / ULN2003 (28BYJ-48)
├── relay                       1/2/4/8-ch relay module
└── esc                         brushless ESC (PWM)

storage/                    (empty)
├── at24cxx                     I2C EEPROM (24C02...24C256)
├── w25qxx                      SPI NOR flash (W25Q16...128)
├── sd_card                     SD card, SPI mode (+ FatFs glue)
├── fm24 / mb85rc              I2C FRAM
└── nvs                        key-value store on internal flash

comms/                      (empty)
├── nrf24l01                    2.4 GHz radio (SPI)
├── sx127x                      LoRa (Ra-02 / RFM95)
├── esp_at                      ESP-01 / ESP8266 WiFi (AT commands)
├── hc05 / hc06                 Bluetooth SPP (UART)
├── hm10                        BLE (UART)
├── sim800l                     GSM/GPRS (UART)
├── neo6m                       GPS NMEA (UART)
├── rs485                       MAX485 transceiver (DE/RE)
├── mcp2515                     CAN controller (SPI)
├── w5500 / enc28j60           Ethernet (SPI)
├── ir_nec                      IR remote receive / transmit
└── modbus_rtu                  protocol over RS485

audio/                      ✓ dfplayer
├── vs1053                      MP3/MIDI codec (SPI)
├── max98357a                   I2S class-D amplifier
├── pcm5102                     I2S DAC
├── inmp441                     I2S MEMS microphone
├── buzzer                      passive (PWM tones) / active (GPIO)
├── isd1820                     voice record / playback
└── wav_dac                     WAV playback via internal DAC + DMA

rtc/                        ✓ ds3231
├── ds1307
├── pcf8563 / bm8563
└── pcf8523 / mcp7940

rfid/                       ✓ rc522  ✓ rc522_simple
├── pn532                       NFC I2C/SPI/UART (more capable)
├── rdm6300                     125 kHz EM4100 reader (UART)
└── pn5180                      ISO 15693

power/                      (empty)
├── ina219 / ina226 / ina3221   current / power monitor (I2C)
├── max17043 / max17048        LiPo fuel gauge (I2C)
├── bq27441                    fuel gauge
├── ip5306                     power-bank IC status (I2C)
├── tp4056                     charge status (GPIO)
└── batt_adc                   battery voltage via divider

util/                       ✓ flash   ✓ delay_timer
├── ring_buffer                 FIFO / ring buffer
├── crc                         crc8/16/32 (sw + HW CRC unit)
├── printf_retarget            printf over SWO / UART
├── scheduler                  cooperative millis-based task runner
├── filter                     MA / EMA / median / 1-D Kalman
├── pid                        PID controller
├── soft_i2c / soft_spi        bit-bang buses
├── cli                        UART command parser
└── rtc_internal              on-chip RTC (backup domain)

port/                       (empty — HAL abstraction layer)
├── stm32_hal/                  F1 / F4 / G0 / G4 / L4 / H7
├── esp_idf/
├── arduino/
└── bare_metal/               register level
```

## New category to consider

```
io/     GPIO expanders — pcf8574, mcp23017 / mcp23s17, 74hc595 (out),
        74hc165 (in), pca9555
```

I/O expanders fit neither `input/` (not buttons) nor `led/`. For now they live
in `util/`; split them into `io/` once there are two or more.
