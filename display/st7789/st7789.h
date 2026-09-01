/**
 * @file    st7789.h
 * @brief   ST7789 IPS TFT LCD (240x240 / 240x320), SPI.
 *
 * Requires: one SPI bus + RST / CS / DC GPIOs. Edit the STM32_HAL block for other MCU families.
 *
 * Usage:
 *   ST7789_Config lcd;
 *   ST7789_PinConfig(&lcd, &hspi1, RST_Port,RST_Pin, CS_Port,CS_Pin, DC_Port,DC_Pin);
 *   ST7789_Init(&lcd);
 *   ST7789_FillScreen(&lcd, ST7789_WHITE);
 *   ST7789_DrawChar(&lcd, 10, 10, 'A', 0xFFFF, 0x0000, Font7x10);
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#ifndef __ST7789_H
#define __ST7789_H



#define ST7789_ROTATION_DEFAULT	0
#define ST7789_ROTATION_RIGH 	1
#define ST7789_ROTATION_LEFT 	2
#define ST7789_ROTATION_DOWN 	3

#define ST7789_HEIGHT_320		320
#define ST7789_WIDTH_240 		240

// Define dành cho các board khác nhau
#define STM32_HAL 	0x00
//#define Arduino	0x01
//#define ESP32		0x02
#if defined(STM32_HAL)

#include "stm32f1xx.h"  // Thay đổi theo dòng STM32 bạn đang sử dụng
typedef struct
{
	SPI_HandleTypeDef*  SPI_Port;
	GPIO_TypeDef*  RST_GPIO_Port;
	GPIO_TypeDef*  CS_GPIO_Port;
	GPIO_TypeDef*  DC_GPIO_Port;
	uint16_t  RST_Pin;
	uint16_t  CS_Pin;
	uint16_t  DC_Pin;
} ST7789_STM32HAL_PinConfig;

typedef struct {
	uint16_t width;
	uint16_t height;
	uint16_t  xstart;
	uint16_t  ystart;
	uint8_t  rotation;
	uint8_t  colorMode;
	ST7789_STM32HAL_PinConfig pins;
} ST7789_Config;

typedef enum {
    ST7789_OK = 0,
    ST7789_ERROR_NULL_POINTER = 1,
    ST7789_ERROR_INVALID_GPIO_PORT = 2,
    ST7789_ERROR_SPI_INIT_FAILED = 3,
    ST7789_ERROR_INVALID_SPI_INSTANCE = 4
} ST7789_Status;

#elif defined(ARDUINO) || defined(ESP32)
    #include <Arduino.h>
	typedef struct
	{
	    uint8_t RST_Pin;
	    uint8_t CS_Pin;
	    uint8_t DC_Pin;
	    SPIClass* SPI_Port;
	} ST7789_Arduino_pin;
#endif



#define ST7789_CMD_DELAY 0x80
// Color definitions
#define	ST7789_BLACK   0x0000
#define	ST7789_BLUE    0x001F
#define	ST7789_RED     0xF800
#define	ST7789_GREEN   0x07E0
#define ST7789_CYAN    0x07FF
#define ST7789_MAGENTA 0xF81F
#define ST7789_YELLOW  0xFFE0
#define ST7789_WHITE   0xFFFF

// command define
#define ST7789_NOP 		0x00
#define ST7789_SWRESET 	0x01
#define ST7789_RDDID 	0x04
#define ST7789_RDDST 	0x09

#define ST7789_SLPIN 	0x10
#define ST7789_SLPOUT 	0x11
#define ST7789_PTLON 	0x12
#define ST7789_NORON 	0x13

#define ST7789_INVOFF 	0x20
#define ST7789_INVON 	0x21
#define ST7789_DISPOFF 	0x28
#define ST7789_DISPON 	0x29
#define ST7789_CASET 	0x2A
#define ST7789_RASET 	0x2B
#define ST7789_RAMWR 	0x2C
#define ST7789_RAMRD 	0x2E

#define ST7789_PTLAR 	0x30
#define ST7789_TEOFF 	0x34
#define ST7789_TEON 	0x35
#define ST7789_MADCTL 	0x36
#define ST7789_COLMOD 	0x3A

#define ST7789_RDID1 	0xDA
#define ST7789_RDID2 	0xDB
#define ST7789_RDID3 	0xDC
#define ST7789_RDID4 	0xDD

//command bit define
#define ST7789_MADCTL_MY 	0x80
#define ST7789_MADCTL_MX 	0x40
#define ST7789_MADCTL_MV 	0x20
#define ST7789_MADCTL_ML 	0x10
#define ST7789_MADCTL_RGB 	0x00

// Basic control funtions
void ST7789_Reset(ST7789_Config *ST7789);
void ST7789_WriteCommand(ST7789_Config *ST7789, uint8_t cmd);
void ST7789_WriteData(ST7789_Config *ST7789, uint8_t *buff, size_t buff_size);
void ST7789_WriteData8(ST7789_Config *ST7789, uint8_t data);
void ST7789_WriteData16(ST7789_Config *ST7789, uint16_t data);

void ST7789_SetAddressWindow(ST7789_Config *ST7789, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ST7789_SetCursor(ST7789_Config *ST7789, uint16_t Xpos, uint16_t Ypos);
void ST7789_WriteRAMPrepare(ST7789_Config *ST7789);

// Configuration and Initialization funtions
ST7789_Status ST7789_Init(ST7789_Config *ST7789);
void ST7789_PinConfig(ST7789_Config *ST7789,
                      SPI_HandleTypeDef* spi_port,
                      GPIO_TypeDef* rst_gpio_port, uint16_t rst_pin,
                      GPIO_TypeDef* cs_gpio_port, uint16_t cs_pin,
                      GPIO_TypeDef* dc_gpio_port, uint16_t dc_pin);
void ST7789_SetRotation(ST7789_Config *ST7789, uint8_t rotation);

// Basic Graphic funtions
void ST7789_FillScreen(ST7789_Config *ST7789, uint16_t color);
void ST7789_DrawPixel(ST7789_Config *ST7789, uint16_t x, uint16_t y, uint16_t color);
void ST7789_DrawImage(ST7789_Config *ST7789, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* image);

void ST7789_DrawChar(ST7789_Config *ST7789, uint16_t x, uint16_t y, char c, uint16_t textColor, uint16_t bgColor, const uint16_t *font);
#endif
