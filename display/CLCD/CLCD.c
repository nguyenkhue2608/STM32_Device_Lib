/******************************************************************************************************************
@File:  	CLCD 8BIT (Character LCD 8Bit Mode)
@Author:  Khue Nguyen
@Website: khuenguyencreator.com
@Youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg

Huong dan su dung:
- Su dung thu vien HAL
- Khoi tao bien LCD: CLCD_Name LCD1;
- Khoi tao LCD do:
Che do 8 bit
CLCD_8BIT_Init(&LCD1, 16, 2, CS_GPIO_Port, CS_Pin, EN_GPIO_Port, EN_Pin,
									D0_GPIO_Port, D0_Pin, D1_GPIO_Port, D1_Pin,
									D2_GPIO_Port, D2_Pin, D3_GPIO_Port, D3_Pin,
									D4_GPIO_Port, D4_Pin, D5_GPIO_Port, D5_Pin,
									D6_GPIO_Port, D6_Pin, D7_GPIO_Port, D7_Pin);
Che do 4 bit									
CLCD_4BIT_Init(&LCD1, 16, 2, CS_GPIO_Port, CS_Pin, EN_GPIO_Port, EN_Pin,
									D4_GPIO_Port, D4_Pin, D5_GPIO_Port, D5_Pin,
									D6_GPIO_Port, D6_Pin, D7_GPIO_Port, D7_Pin);
- Su dung cac ham truyen dia chi cua LCD do: 
CLCD__SetCursor(&LCD1, 0, 0);
CLCD_WriteString(&LCD1,"Hello anh em");	
******************************************************************************************************************/
#include "CLCD.h"


//************************ Low Level Function *****************************************//
static void CLCD_Delay(uint16_t Time)
{
	HAL_Delay(Time);
	
}
static void CLCD_Write8(CLCD_Name* LCD, uint8_t Data, uint8_t Mode)
{
	if(Mode == CLCD_COMMAND)
	{
		HAL_GPIO_WritePin(LCD->RS_PORT, LCD->RS_PIN, GPIO_PIN_RESET);// RS = 0, write cmd
	}
	else if(Mode == CLCD_DATA)
	{
		HAL_GPIO_WritePin(LCD->RS_PORT, LCD->RS_PIN, GPIO_PIN_SET);// RS = 1, write data
	}
	HAL_GPIO_WritePin(LCD->D0_PORT, LCD->D0_PIN, Data&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D1_PORT, LCD->D1_PIN, Data>>1&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D2_PORT, LCD->D2_PIN, Data>>2&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D3_PORT, LCD->D3_PIN, Data>>3&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D4_PORT, LCD->D4_PIN, Data>>4&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D5_PORT, LCD->D5_PIN, Data>>5&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D6_PORT, LCD->D6_PIN, Data>>6&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D7_PORT, LCD->D7_PIN, Data>>7&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(LCD->EN_PORT, LCD->EN_PIN, GPIO_PIN_RESET);
	CLCD_Delay(1);
	HAL_GPIO_WritePin(LCD->EN_PORT, LCD->EN_PIN, GPIO_PIN_SET);
	CLCD_Delay(1);
	HAL_GPIO_WritePin(LCD->EN_PORT, LCD->EN_PIN, GPIO_PIN_RESET);
	CLCD_Delay(1);
}
static void CLCD_Write4(CLCD_Name* LCD, uint8_t Data, uint8_t Mode)
{
	uint8_t Data_H = Data >>4;
	uint8_t Data_L = Data;
	if(Mode == CLCD_COMMAND)
	{
		HAL_GPIO_WritePin(LCD->RS_PORT, LCD->RS_PIN, GPIO_PIN_RESET);// RS = 0, write cmd
	}
	else if(Mode == CLCD_DATA)
	{
		HAL_GPIO_WritePin(LCD->RS_PORT, LCD->RS_PIN, GPIO_PIN_SET);// RS = 1, write data
	}
	HAL_GPIO_WritePin(LCD->D4_PORT, LCD->D4_PIN, Data_H&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D5_PORT, LCD->D5_PIN, Data_H>>1&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D6_PORT, LCD->D6_PIN, Data_H>>2&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D7_PORT, LCD->D7_PIN, Data_H>>3&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(LCD->EN_PORT, LCD->EN_PIN, GPIO_PIN_RESET);
	CLCD_Delay(1);
	HAL_GPIO_WritePin(LCD->EN_PORT, LCD->EN_PIN, GPIO_PIN_SET);
	CLCD_Delay(1);
	HAL_GPIO_WritePin(LCD->EN_PORT, LCD->EN_PIN, GPIO_PIN_RESET);
	CLCD_Delay(1);
	
	HAL_GPIO_WritePin(LCD->D4_PORT, LCD->D4_PIN, Data_L&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D5_PORT, LCD->D5_PIN, Data_L>>1&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D6_PORT, LCD->D6_PIN, Data_L>>2&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD->D7_PORT, LCD->D7_PIN, Data_L>>3&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(LCD->EN_PORT, LCD->EN_PIN, GPIO_PIN_RESET);
	CLCD_Delay(1);
	HAL_GPIO_WritePin(LCD->EN_PORT, LCD->EN_PIN, GPIO_PIN_SET);
	CLCD_Delay(1);
	HAL_GPIO_WritePin(LCD->EN_PORT, LCD->EN_PIN, GPIO_PIN_RESET);
	CLCD_Delay(1);
}
//************************ High Level Function *****************************************//
void CLCD_8BIT_Init(CLCD_Name* LCD, uint8_t Colum, uint8_t Row,
									GPIO_TypeDef* RS_PORT, uint16_t RS_PIN, GPIO_TypeDef* EN_PORT, uint16_t EN_PIN,
									GPIO_TypeDef* D0_PORT, uint16_t D0_PIN, GPIO_TypeDef* D1_PORT, uint16_t D1_PIN,
									GPIO_TypeDef* D2_PORT, uint16_t D2_PIN, GPIO_TypeDef* D3_PORT, uint16_t D3_PIN,
									GPIO_TypeDef* D4_PORT, uint16_t D4_PIN, GPIO_TypeDef* D5_PORT, uint16_t D5_PIN,
									GPIO_TypeDef* D6_PORT, uint16_t D6_PIN, GPIO_TypeDef* D7_PORT, uint16_t D7_PIN)
{
	LCD->MODE = LCD_8BITMODE;
	LCD->COLUMS = Colum;
	LCD->ROWS = Row;
	LCD->RS_PORT = RS_PORT;
	LCD->RS_PIN = RS_PIN;
	LCD->EN_PORT = EN_PORT;
	LCD->EN_PIN = EN_PIN;
	LCD->D0_PORT = D0_PORT;
	LCD->D0_PIN = D0_PIN;
	LCD->D1_PORT = D1_PORT;
	LCD->D1_PIN = D1_PIN;
	LCD->D2_PORT = D2_PORT;
	LCD->D2_PIN = D2_PIN;
	LCD->D3_PORT = D3_PORT;
	LCD->D3_PIN = D3_PIN;
	LCD->D4_PORT = D4_PORT;
	LCD->D4_PIN = D4_PIN;
	LCD->D5_PORT = D5_PORT;
	LCD->D5_PIN = D5_PIN;
	LCD->D6_PORT = D6_PORT;
	LCD->D6_PIN = D6_PIN;
	LCD->D7_PORT = D7_PORT;
	LCD->D7_PIN = D7_PIN;
	CLCD_Delay(50);
	LCD->FUNCTIONSET = LCD_FUNCTIONSET|LCD_8BITMODE|LCD_2LINE|LCD_5x8DOTS;
	LCD->ENTRYMODE = LCD_ENTRYMODESET|LCD_ENTRYLEFT|LCD_ENTRYSHIFTDECREMENT;
	LCD->DISPLAYCTRL = LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_CURSOROFF|LCD_BLINKOFF;
	LCD->CURSORSHIFT = LCD_CURSORSHIFT|LCD_CURSORMOVE|LCD_MOVERIGHT;
	
	CLCD_Write8(LCD, LCD->ENTRYMODE,CLCD_COMMAND);
	CLCD_Write8(LCD, LCD->DISPLAYCTRL,CLCD_COMMAND);
	CLCD_Write8(LCD, LCD->CURSORSHIFT,CLCD_COMMAND);
	CLCD_Write8(LCD, LCD->FUNCTIONSET,CLCD_COMMAND);
	
	CLCD_Write8(LCD, LCD_CLEARDISPLAY,CLCD_COMMAND);
	CLCD_Write8(LCD, LCD_RETURNHOME,CLCD_COMMAND);
}

void CLCD_4BIT_Init(CLCD_Name* LCD, uint8_t Colum, uint8_t Row,
									GPIO_TypeDef* RS_PORT, uint16_t RS_PIN, GPIO_TypeDef* EN_PORT, uint16_t EN_PIN,
									GPIO_TypeDef* D4_PORT, uint16_t D4_PIN, GPIO_TypeDef* D5_PORT, uint16_t D5_PIN,
									GPIO_TypeDef* D6_PORT, uint16_t D6_PIN, GPIO_TypeDef* D7_PORT, uint16_t D7_PIN)
{
	LCD->MODE = LCD_4BITMODE;
	LCD->COLUMS = Colum;
	LCD->ROWS = Row;
	LCD->RS_PORT = RS_PORT;
	LCD->RS_PIN = RS_PIN;
	LCD->EN_PORT = EN_PORT;
	LCD->EN_PIN = EN_PIN;
	LCD->D4_PORT = D4_PORT;
	LCD->D4_PIN = D4_PIN;
	LCD->D5_PORT = D5_PORT;
	LCD->D5_PIN = D5_PIN;
	LCD->D6_PORT = D6_PORT;
	LCD->D6_PIN = D6_PIN;
	LCD->D7_PORT = D7_PORT;
	LCD->D7_PIN = D7_PIN;
	LCD->FUNCTIONSET = LCD_FUNCTIONSET|LCD_4BITMODE|LCD_2LINE|LCD_5x8DOTS;
	LCD->ENTRYMODE = LCD_ENTRYMODESET|LCD_ENTRYLEFT|LCD_ENTRYSHIFTDECREMENT;
	LCD->DISPLAYCTRL = LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_CURSOROFF|LCD_BLINKOFF;
	LCD->CURSORSHIFT = LCD_CURSORSHIFT|LCD_CURSORMOVE|LCD_MOVERIGHT;
	
	CLCD_Delay(50);
	CLCD_Write4(LCD, 0x33, CLCD_COMMAND);
	CLCD_Delay(5);
	CLCD_Write4(LCD, 0x33, CLCD_COMMAND);
	CLCD_Delay(5);
	CLCD_Write4(LCD, 0x32, CLCD_COMMAND);
	CLCD_Delay(5);
	
	CLCD_Write4(LCD, LCD->ENTRYMODE,CLCD_COMMAND);
	CLCD_Write4(LCD, LCD->DISPLAYCTRL,CLCD_COMMAND);
	CLCD_Write4(LCD, LCD->CURSORSHIFT,CLCD_COMMAND);
	CLCD_Write4(LCD, LCD->FUNCTIONSET,CLCD_COMMAND);
	
	CLCD_Write4(LCD, LCD_CLEARDISPLAY,CLCD_COMMAND);
	CLCD_Write4(LCD, LCD_RETURNHOME,CLCD_COMMAND);
}
void CLCD_SetCursor(CLCD_Name* LCD, uint8_t Xpos, uint8_t Ypos)
{
	
	uint8_t DRAM_ADDRESS = 0x00;
	uint8_t DRAM_OFFSET[4] = {0x00, 0x40, 0x14, 0x54};
	if(Xpos >= LCD->COLUMS)
	{
		Xpos = LCD->COLUMS - 1;
	}
	if(Ypos >= LCD->ROWS)
	{
		Ypos = LCD->ROWS -1;
	}
	DRAM_ADDRESS = DRAM_OFFSET[Ypos] + Xpos;
	if(LCD->MODE == LCD_8BITMODE)
	{
		CLCD_Write8(LCD, LCD_SETDDRAMADDR|DRAM_ADDRESS, CLCD_COMMAND);
	}
	else if(LCD->MODE == LCD_4BITMODE)
	{
		CLCD_Write4(LCD, LCD_SETDDRAMADDR|DRAM_ADDRESS, CLCD_COMMAND);
	}
}
void CLCD_WriteChar(CLCD_Name* LCD, char character)
{
	if(LCD->MODE == LCD_8BITMODE)
	{
		CLCD_Write8(LCD, character, CLCD_DATA);
	}
	else if(LCD->MODE == LCD_4BITMODE)
	{
		CLCD_Write4(LCD, character, CLCD_DATA);
	}
}
void CLCD_WriteString(CLCD_Name* LCD, char *String)
{
	while(*String)CLCD_WriteChar(LCD, *String++);
}
void CLCD_Clear(CLCD_Name* LCD)
{
	if(LCD->MODE == LCD_8BITMODE)
	{
		CLCD_Write8(LCD, LCD_CLEARDISPLAY, CLCD_COMMAND);
	}
	else if(LCD->MODE == LCD_4BITMODE)
	{
		CLCD_Write4(LCD, LCD_CLEARDISPLAY, CLCD_COMMAND);
	}
	CLCD_Delay(5);
}
void CLCD_ReturnHome(CLCD_Name* LCD)
{
	if(LCD->MODE == LCD_8BITMODE)
	{
		CLCD_Write8(LCD, LCD_RETURNHOME, CLCD_COMMAND);
	}
	else if(LCD->MODE == LCD_4BITMODE)
	{
		CLCD_Write4(LCD, LCD_RETURNHOME, CLCD_COMMAND);
	}
	CLCD_Delay(5);
}
void CLCD_CursorOn(CLCD_Name* LCD)
{
	LCD->DISPLAYCTRL |= LCD_CURSORON;
	if(LCD->MODE == LCD_8BITMODE)
	{
		CLCD_Write8(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
	}
	else if(LCD->MODE == LCD_4BITMODE)
	{
		CLCD_Write4(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
	}
}
void CLCD_CursorOff(CLCD_Name* LCD)
{
	LCD->DISPLAYCTRL &= ~LCD_CURSORON;
	if(LCD->MODE == LCD_8BITMODE)
	{
		CLCD_Write8(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
	}
	else if(LCD->MODE == LCD_4BITMODE)
	{
		CLCD_Write4(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
	}
}
void CLCD_BlinkOn(CLCD_Name* LCD)
{
	LCD->DISPLAYCTRL |= LCD_BLINKON;
	if(LCD->MODE == LCD_8BITMODE)
	{
		CLCD_Write8(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
	}
	else if(LCD->MODE == LCD_4BITMODE)
	{
		CLCD_Write4(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
	}
}
void CLCD_BlinkOff(CLCD_Name* LCD)
{
	LCD->DISPLAYCTRL &= ~LCD_BLINKON;
	if(LCD->MODE == LCD_8BITMODE)
	{
		CLCD_Write8(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
	}
	else if(LCD->MODE == LCD_4BITMODE)
	{
		CLCD_Write4(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
	}
	
}
