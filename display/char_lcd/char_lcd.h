/**
 * @file    char_lcd.h
 * @brief   HD44780 character LCD, 4-bit / 8-bit parallel (LCD1602 / LCD2004 / LiquidCrystal).
 *
 * Requires: RS + EN + 4 or 8 data GPIOs.
 *
 * Usage:
 *   CHAR_LCD_Device lcd;
 *   CHAR_LCD_4BIT_Init(&lcd, 16, 2, RS_Port,RS_Pin, EN_Port,EN_Pin,
 *                      D4_Port,D4_Pin, D5_Port,D5_Pin, D6_Port,D6_Pin, D7_Port,D7_Pin);
 *   CHAR_LCD_SetCursor(&lcd, 0, 0);
 *   CHAR_LCD_WriteString(&lcd, "Hello");
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#ifndef __CHAR_LCD_H
#define __CHAR_LCD_H

#include "stm32f1xx_hal.h" // Chuyen doi chip dang su dung

// constant
#define CHAR_LCD_COMMAND 						0x00
#define CHAR_LCD_DATA 							0x01

// commands
#define LCD_CLEARDISPLAY 				0x01
#define LCD_RETURNHOME 					0x02

#define LCD_ENTRYMODESET 				0x04
#define LCD_DISPLAYCONTROL 			0x08
#define LCD_CURSORSHIFT 				0x10
#define LCD_FUNCTIONSET 				0x20
#define LCD_SETCGRAMADDR 				0x40
#define LCD_SETDDRAMADDR	 			0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 					0x00
#define LCD_ENTRYLEFT 					0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 					0x04
#define LCD_DISPLAYOFF 					0x00
#define LCD_CURSORON 						0x02
#define LCD_CURSOROFF 					0x00
#define LCD_BLINKON 						0x01
#define LCD_BLINKOFF 						0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 				0x08
#define LCD_CURSORMOVE 					0x00
#define LCD_MOVERIGHT 					0x04
#define LCD_MOVELEFT 						0x00

// flags for function set
#define LCD_8BITMODE 						0x10
#define LCD_4BITMODE 						0x00
#define LCD_2LINE 							0x08
#define LCD_1LINE 							0x00
#define LCD_5x10DOTS 						0x04
#define LCD_5x8DOTS 						0x00
typedef struct
{
	uint8_t MODE;
	uint8_t COLUMS; 
	uint8_t ROWS; 
	GPIO_TypeDef* RS_PORT;
	uint16_t RS_PIN;
	GPIO_TypeDef* EN_PORT;
	uint16_t EN_PIN;
	GPIO_TypeDef* D0_PORT;
	uint16_t D0_PIN;
	GPIO_TypeDef* D1_PORT;
	uint16_t D1_PIN;
	GPIO_TypeDef* D2_PORT;
	uint16_t D2_PIN;
	GPIO_TypeDef* D3_PORT;
	uint16_t D3_PIN;
	GPIO_TypeDef* D4_PORT;
	uint16_t D4_PIN;
	GPIO_TypeDef* D5_PORT;
	uint16_t D5_PIN;
	GPIO_TypeDef* D6_PORT;
	uint16_t D6_PIN;
	GPIO_TypeDef* D7_PORT;
	uint16_t D7_PIN;
	uint8_t ENTRYMODE;
	uint8_t DISPLAYCTRL;
	uint8_t CURSORSHIFT;
	uint8_t FUNCTIONSET;
}CHAR_LCD_Device;


void CHAR_LCD_8BIT_Init(CHAR_LCD_Device* LCD, uint8_t Colum, uint8_t Row,
									GPIO_TypeDef* RS_PORT, uint16_t RS_PIN, GPIO_TypeDef* EN_PORT, uint16_t EN_PIN,
									GPIO_TypeDef* D0_PORT, uint16_t D0_PIN, GPIO_TypeDef* D1_PORT, uint16_t D1_PIN,
									GPIO_TypeDef* D2_PORT, uint16_t D2_PIN, GPIO_TypeDef* D3_PORT, uint16_t D3_PIN,
									GPIO_TypeDef* D4_PORT, uint16_t D4_PIN, GPIO_TypeDef* D5_PORT, uint16_t D5_PIN,
									GPIO_TypeDef* D6_PORT, uint16_t D6_PIN, GPIO_TypeDef* D7_PORT, uint16_t D7_PIN);
void CHAR_LCD_4BIT_Init(CHAR_LCD_Device* LCD, uint8_t Colum, uint8_t Row,
									GPIO_TypeDef* RS_PORT, uint16_t RS_PIN, GPIO_TypeDef* EN_PORT, uint16_t EN_PIN,
									GPIO_TypeDef* D4_PORT, uint16_t D4_PIN, GPIO_TypeDef* D5_PORT, uint16_t D5_PIN,
									GPIO_TypeDef* D6_PORT, uint16_t D6_PIN, GPIO_TypeDef* D7_PORT, uint16_t D7_PIN);
void CHAR_LCD_SetCursor(CHAR_LCD_Device* LCD, uint8_t Xpos, uint8_t YPos);
void CHAR_LCD_WriteChar(CHAR_LCD_Device* LCD, char character);
void CHAR_LCD_WriteString(CHAR_LCD_Device* LCD, char *String);
void CHAR_LCD_Clear(CHAR_LCD_Device* LCD);
void CHAR_LCD_ReturnHome(CHAR_LCD_Device* LCD);
void CHAR_LCD_CursorOn(CHAR_LCD_Device* LCD);
void CHAR_LCD_CursorOff(CHAR_LCD_Device* LCD);
void CHAR_LCD_BlinkOn(CHAR_LCD_Device* LCD);
void CHAR_LCD_BlinkOff(CHAR_LCD_Device* LCD);
#endif
