/**
 * @file    char_lcd_i2c.c
 * @brief   HD44780 character LCD via a PCF8574 I2C backpack - implementation.
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#include "char_lcd_i2c.h"

//************************** Low Level Function ****************************************************************//
static void CHAR_LCD_Delay(uint16_t Time)
{
	HAL_Delay(Time);
}
static void CHAR_LCD_WriteI2C(CHAR_LCD_I2C_Device* LCD, uint8_t Data, uint8_t Mode)
{
	char Data_H;
	char Data_L;
	uint8_t Data_I2C[4];
	Data_H = Data&0xF0;
	Data_L = (Data<<4)&0xF0;
	if(LCD->BACKLIGHT)
	{
		Data_H |= LCD_BACKLIGHT; 
		Data_L |= LCD_BACKLIGHT; 
	}
	if(Mode == CHAR_LCD_DATA)
	{
		Data_H |= LCD_RS;
		Data_L |= LCD_RS;
	}
	else if(Mode == CHAR_LCD_COMMAND)
	{
		Data_H &= ~LCD_RS;
		Data_L &= ~LCD_RS;
	}
	Data_I2C[0] = Data_H|LCD_EN;
	CHAR_LCD_Delay(1);
	Data_I2C[1] = Data_H;
	Data_I2C[2] = Data_L|LCD_EN;
	CHAR_LCD_Delay(1);
	Data_I2C[3] = Data_L;
	HAL_I2C_Master_Transmit(LCD->I2C, LCD->ADDRESS, (uint8_t *)Data_I2C, sizeof(Data_I2C), 1000);
}


//************************** High Level Function ****************************************************************//
void CHAR_LCD_I2C_Init(CHAR_LCD_I2C_Device* LCD, I2C_HandleTypeDef* hi2c_CHAR_LCD, uint8_t Address, uint8_t Colums, uint8_t Rows)
{
	LCD->I2C = hi2c_CHAR_LCD;
	LCD->ADDRESS = Address;
	LCD->COLUMS = Colums;
	LCD->ROWS = Rows;
	
	LCD->FUNCTIONSET = LCD_FUNCTIONSET|LCD_4BITMODE|LCD_2LINE|LCD_5x8DOTS;
	LCD->ENTRYMODE = LCD_ENTRYMODESET|LCD_ENTRYLEFT|LCD_ENTRYSHIFTDECREMENT;
	LCD->DISPLAYCTRL = LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_CURSOROFF|LCD_BLINKOFF;
	LCD->CURSORSHIFT = LCD_CURSORSHIFT|LCD_CURSORMOVE|LCD_MOVERIGHT;
	LCD->BACKLIGHT = LCD_BACKLIGHT;

	CHAR_LCD_Delay(50);
	CHAR_LCD_WriteI2C(LCD, 0x33, CHAR_LCD_COMMAND);
//	CHAR_LCD_Delay(5);
	CHAR_LCD_WriteI2C(LCD, 0x33, CHAR_LCD_COMMAND);
	CHAR_LCD_Delay(5);
	CHAR_LCD_WriteI2C(LCD, 0x32, CHAR_LCD_COMMAND);
	CHAR_LCD_Delay(5);
	CHAR_LCD_WriteI2C(LCD, 0x20, CHAR_LCD_COMMAND);
	CHAR_LCD_Delay(5);
	
	CHAR_LCD_WriteI2C(LCD, LCD->ENTRYMODE,CHAR_LCD_COMMAND);
	CHAR_LCD_WriteI2C(LCD, LCD->DISPLAYCTRL,CHAR_LCD_COMMAND);
	CHAR_LCD_WriteI2C(LCD, LCD->CURSORSHIFT,CHAR_LCD_COMMAND);
	CHAR_LCD_WriteI2C(LCD, LCD->FUNCTIONSET,CHAR_LCD_COMMAND);
	
	CHAR_LCD_WriteI2C(LCD, LCD_CLEARDISPLAY,CHAR_LCD_COMMAND);
	CHAR_LCD_WriteI2C(LCD, LCD_RETURNHOME,CHAR_LCD_COMMAND);
}
void CHAR_LCD_I2C_SetCursor(CHAR_LCD_I2C_Device* LCD, uint8_t Xpos, uint8_t Ypos)
{
	uint8_t DRAM_ADDRESS = 0x00;
	if(Xpos >= LCD->COLUMS)
	{
		Xpos = LCD->COLUMS - 1;
	}
	if(Ypos >= LCD->ROWS)
	{
		Ypos = LCD->ROWS -1;
	}
	if(Ypos == 0)
	{
		DRAM_ADDRESS = 0x00 + Xpos;
	}
	else if(Ypos == 1)
	{
		DRAM_ADDRESS = 0x40 + Xpos;
	}
	else if(Ypos == 2)
	{
		DRAM_ADDRESS = 0x14 + Xpos;
	}
	else if(Ypos == 3)
	{
		DRAM_ADDRESS = 0x54 + Xpos;
	}
	CHAR_LCD_WriteI2C(LCD, LCD_SETDDRAMADDR|DRAM_ADDRESS, CHAR_LCD_COMMAND);
}
void CHAR_LCD_I2C_WriteChar(CHAR_LCD_I2C_Device* LCD, char character)
{
	CHAR_LCD_WriteI2C(LCD, character, CHAR_LCD_DATA);
}
void CHAR_LCD_I2C_WriteString(CHAR_LCD_I2C_Device* LCD, char *String)
{
	while(*String)CHAR_LCD_I2C_WriteChar(LCD, *String++);
}
void CHAR_LCD_I2C_Clear(CHAR_LCD_I2C_Device* LCD)
{
	CHAR_LCD_WriteI2C(LCD, LCD_CLEARDISPLAY, CHAR_LCD_COMMAND);
	CHAR_LCD_Delay(5);
}
void CHAR_LCD_I2C_ReturnHome(CHAR_LCD_I2C_Device* LCD)
{
	CHAR_LCD_WriteI2C(LCD, LCD_RETURNHOME, CHAR_LCD_COMMAND);
	CHAR_LCD_Delay(5);
}
void CHAR_LCD_I2C_CursorOn(CHAR_LCD_I2C_Device* LCD)
{
	LCD->DISPLAYCTRL |= LCD_CURSORON;
	CHAR_LCD_WriteI2C(LCD, LCD->DISPLAYCTRL, CHAR_LCD_COMMAND);
}
void CHAR_LCD_I2C_CursorOff(CHAR_LCD_I2C_Device* LCD)
{
	LCD->DISPLAYCTRL &= ~LCD_CURSORON;
	CHAR_LCD_WriteI2C(LCD, LCD->DISPLAYCTRL, CHAR_LCD_COMMAND);
}
void CHAR_LCD_I2C_BlinkOn(CHAR_LCD_I2C_Device* LCD)
{
	LCD->DISPLAYCTRL |= LCD_BLINKON;
	CHAR_LCD_WriteI2C(LCD, LCD->DISPLAYCTRL, CHAR_LCD_COMMAND);
}
void CHAR_LCD_I2C_BlinkOff(CHAR_LCD_I2C_Device* LCD)
{
	LCD->DISPLAYCTRL &= ~LCD_BLINKON;
	CHAR_LCD_WriteI2C(LCD, LCD->DISPLAYCTRL, CHAR_LCD_COMMAND);
}
