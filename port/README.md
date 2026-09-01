# port/

HAL abstraction layer. One subfolder per target (`stm32_hal/`, `esp_idf/`, ...) implementing the primitive IO a driver needs: GPIO read/write, I2C/SPI transfer, delay ms/us, UART tx. Drivers call these instead of a vendor HAL directly. Not populated yet.
