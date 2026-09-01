/**
 * @file    ds3231.h
 * @brief   DS3231 real-time clock, I2C.
 *
 * Requires: one I2C bus.
 *
 * Usage:
 *   DS3231_Device rtc;
 *   DS3231_Init(&rtc, &hi2c1);
 *   DS3231_SetTime(&rtc, 12, 30, 0);
 *   DS3231_GetTime(&rtc);   // rtc.Hours, rtc.Min, rtc.Sec
 *   DS3231_GetDate(&rtc);   // rtc.Date, rtc.Month, rtc.Year, rtc.Day
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
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
}DS3231_Device;

void DS3231_Init(DS3231_Device* DS3231, I2C_HandleTypeDef* I2C_In);
void DS3231_SetTime(DS3231_Device* DS3231, uint8_t Hour, uint8_t Min, uint8_t Sec);
void DS3231_GetTime(DS3231_Device* DS3231);
void DS3231_SetDate(DS3231_Device* DS3231, uint8_t Day, uint8_t Date, uint8_t Month, uint8_t Year);
void DS3231_GetDate(DS3231_Device* DS3231);
#endif
