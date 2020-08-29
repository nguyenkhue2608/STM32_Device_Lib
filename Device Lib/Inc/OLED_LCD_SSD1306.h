/******************************************************************************************************************
@File:  	OLED_LCD_SSD1306 Driver
@Author:  Khue Nguyen
@Website: khuenguyencreator.com
@Youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
Huong dan su dung:
- Su dung thu vien HAL
- include "fonts.h"
- Khoi tao i2c
- Khoi tao bien LCD
SSD1306_Name SSD1306;
- Khoi tao LCD do, truyen dia chi I2C su dung:
	SSD1306_Init(&SSD1306, &hi2c1);
- Su dung cac ham phai truyen dia chi cua LCD do: 
	SSD1306_GotoXY (&SSD1306, 0,0);
  SSD1306_Puts (&SSD1306, "KHUE NGUYEN", &Font_11x18, SSD1306_COLOR_WHITE);
  SSD1306_GotoXY (&SSD1306, 10, 30);
  SSD1306_Puts (&SSD1306,"  CREATOR", &Font_11x18, SSD1306_COLOR_WHITE);
  SSD1306_UpdateScreen(&SSD1306); //display
******************************************************************************************************************/
#ifndef __OLED_LCD_SSD1306_H
#define __OLED_LCD_SSD1306_H
#include "stm32f1xx_hal.h"
#include "string.h"
#include "fonts.h"

/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))

#define SSD1306_I2C_ADDR 0x78
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH 132
#endif
/* SSD1306 LCD height in pixels */
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT 64
#endif
// SSD1306 COMMAND
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_ON 0xAF
// SSD1306 GRAPHIC CMD
#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define SSD1306_DEACTIVATE_SCROLL                    0x2E // Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F // Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range
#define SSD1306_NORMALDISPLAY       								 0xA6
#define SSD1306_INVERTDISPLAY       								 0xA7
typedef enum {
	SSD1306_COLOR_BLACK = 0x00, /*!< Black color, no pixel */
	SSD1306_COLOR_WHITE = 0x01  /*!< Pixel is set. Color depends on LCD */
} SSD1306_COLOR_t;
#define SSD1306_SEND_DATA 0x40
#define SSD1306_SEND_CMD  0x00
typedef struct
{
	uint8_t Address;
	uint8_t Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
	I2C_HandleTypeDef* I2C;
	uint8_t CurrentX;
	uint8_t CurrentY;
	uint8_t Initialized;
	uint8_t Inverted;
}SSD1306_Name;

uint8_t SSD1306_Init(SSD1306_Name* SSD1306, I2C_HandleTypeDef* I2C_In);
void SSD1306_GotoXY(SSD1306_Name* SSD1306, uint16_t x, uint16_t y);
void SSD1306_UpdateScreen(SSD1306_Name* SSD1306);

void SSD1306_ToggleInvert(SSD1306_Name* SSD1306);
void SSD1306_Fill(SSD1306_Name* SSD1306, SSD1306_COLOR_t color);
void SSD1306_Clear (SSD1306_Name* SSD1306);
void SSD1306_ON(SSD1306_Name* SSD1306);
void SSD1306_OFF(SSD1306_Name* SSD1306);
void SSD1306_ScrollRight(SSD1306_Name* SSD1306, uint8_t start_row, uint8_t end_row);
void SSD1306_ScrollLeft(SSD1306_Name* SSD1306, uint8_t start_row, uint8_t end_row);
void SSD1306_Scrolldiagright(SSD1306_Name* SSD1306, uint8_t start_row, uint8_t end_row);
void SSD1306_Scrolldiagleft(SSD1306_Name* SSD1306, uint8_t start_row, uint8_t end_row);
void SSD1306_Stopscroll(SSD1306_Name* SSD1306);
void SSD1306_InvertDisplay (SSD1306_Name* SSD1306, int Invert);

void SSD1306_DrawPixel(SSD1306_Name* SSD1306, uint16_t x, uint16_t y, SSD1306_COLOR_t color);
void SSD1306_DrawBitmap(SSD1306_Name* SSD1306, int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, SSD1306_COLOR_t color);
char SSD1306_Putc(SSD1306_Name* SSD1306, char ch, FontDef_t* Font, SSD1306_COLOR_t color);
char SSD1306_Puts(SSD1306_Name* SSD1306, char* str, FontDef_t* Font, SSD1306_COLOR_t color);
void SSD1306_DrawLine(SSD1306_Name* SSD1306, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c);
void SSD1306_DrawRectangle(SSD1306_Name* SSD1306, uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);
void SSD1306_DrawFilledRectangle(SSD1306_Name* SSD1306, uint16_t x, uint16_t y, uint16_t w, uint16_t h, SSD1306_COLOR_t c);
void SSD1306_DrawTriangle(SSD1306_Name* SSD1306, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);
void SSD1306_DrawFilledTriangle(SSD1306_Name* SSD1306, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SSD1306_COLOR_t color);
void SSD1306_DrawCircle(SSD1306_Name* SSD1306, int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c);
void SSD1306_DrawFilledCircle(SSD1306_Name* SSD1306, int16_t x0, int16_t y0, int16_t r, SSD1306_COLOR_t c);

#endif
