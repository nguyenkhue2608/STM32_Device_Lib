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
#ifndef __DS3231_H
#define __DS3231_H

#include "stm32f1xx_hal.h"

#define DS3231_ADDRESS 0x68<<1

typedef struct
{
	I2C_HandleTypeDef* I2C;
	uint8_t TxTimeBuff[3];
	uint8_t RxTimeBuff[3];
	uint8_t TxDateBuff[4];
	uint8_t RxDateBuff[4];
	uint8_t Hours;
	uint8_t Min;
	uint8_t Sec;
	uint8_t Date;
	uint8_t Day;
	uint8_t Month;
	uint8_t Year;
}DS3231_Name;

void DS3231_Init(DS3231_Name* DS3231, I2C_HandleTypeDef* I2C_In);
void DS3231_SetTime(DS3231_Name* DS3231, uint8_t Hour, uint8_t Min, uint8_t Sec);
void DS3231_GetTime(DS3231_Name* DS3231);
void DS3231_SetDate(DS3231_Name* DS3231, uint8_t Day, uint8_t Date, uint8_t Month, uint8_t Year);
void DS3231_GetDate(DS3231_Name* DS3231);
#endif
