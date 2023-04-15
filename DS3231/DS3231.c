/******************************************************************************************************************
@File:  	DS3231 RTC Module
@Author:  Khue Nguyen
@Website: khuenguyencreator.com
@Youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
Huong dan su dung:
- Su dung thu vien HAL
- Khoi tao I2C cho Ds3231
- Khoi tao bien luu DS3231
DS3231_Name DS3231;
- Khoi tao DS3231 do:
DS3231_Init(&DS3231, &hi2c1);
- Su dung cac ham phai truyen dia chi cua DS3231 do: 
DS3231_GetTime(&DS3231);
DS3231_GetDate(&DS3231);
******************************************************************************************************************/

#include "DS3231.h"
//*****************************************Low Level Layer ***********************************************//
// Thay doi thanh IT hoac DMA neu can thiet
static void I2C_WriteTime(DS3231_Name* DS3231)
{
	HAL_I2C_Mem_Write(DS3231->I2C, DS3231_ADDRESS, 0, I2C_MEMADD_SIZE_8BIT, DS3231->TxTimeBuff, 3, 1000);
}

static void I2C_ReadTime(DS3231_Name* DS3231)
{
	HAL_I2C_Mem_Read(DS3231->I2C, DS3231_ADDRESS, 0, I2C_MEMADD_SIZE_8BIT, DS3231->RxTimeBuff, 3, 1000);
}

static void I2C_WriteDate(DS3231_Name* DS3231)
{
	HAL_I2C_Mem_Write(DS3231->I2C, DS3231_ADDRESS, 3, I2C_MEMADD_SIZE_8BIT, DS3231->TxDateBuff, 4, 1000);
}

static void I2C_ReadDate(DS3231_Name* DS3231)
{
	HAL_I2C_Mem_Read(DS3231->I2C, DS3231_ADDRESS, 3, I2C_MEMADD_SIZE_8BIT, DS3231->RxDateBuff, 4, 1000);
}

static uint8_t BCD2DEC(uint8_t data)
{
	return (data>>4)*10+ (data&0x0f);
}

static uint8_t DEC2BCD(uint8_t data)
{
	return (data/10)<<4|(data%10);
}

//****************************************HIGH Level Layer *************************************************//
void DS3231_Init(DS3231_Name* DS3231, I2C_HandleTypeDef* I2C_In)
{
	DS3231->I2C = I2C_In;
}

void DS3231_SetTime(DS3231_Name* DS3231, uint8_t Hour, uint8_t Min, uint8_t Sec)
{
	DS3231->TxTimeBuff[0] = DEC2BCD(Hour);
	DS3231->TxTimeBuff[1] = DEC2BCD(Min);
	DS3231->TxTimeBuff[2] = DEC2BCD(Sec);
	I2C_WriteTime(DS3231);
}	
void DS3231_GetTime(DS3231_Name* DS3231)
{
	I2C_ReadTime(DS3231);
	DS3231->Sec = BCD2DEC(DS3231->RxTimeBuff[0]);
	DS3231->Min = BCD2DEC(DS3231->RxTimeBuff[1]);
	DS3231->Hours = BCD2DEC(DS3231->RxTimeBuff[2]);
}
void DS3231_SetDate(DS3231_Name* DS3231, uint8_t Day, uint8_t Date, uint8_t Month, uint8_t Year)
{
	DS3231->TxDateBuff[0] = DEC2BCD(Day);
	DS3231->TxDateBuff[1] = DEC2BCD(Date);
	DS3231->TxDateBuff[2] = DEC2BCD(Month);
	DS3231->TxDateBuff[3] = DEC2BCD(Year);
	I2C_WriteDate(DS3231);
}	
void DS3231_GetDate(DS3231_Name* DS3231)
{
	I2C_ReadDate(DS3231);
	DS3231->Day = BCD2DEC(DS3231->RxDateBuff[0]);
	DS3231->Date = BCD2DEC(DS3231->RxDateBuff[1]);
	DS3231->Month = BCD2DEC(DS3231->RxDateBuff[2]);
	DS3231->Year = BCD2DEC(DS3231->RxDateBuff[3]);
}
