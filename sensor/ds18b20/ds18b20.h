/**
 * @file    ds18b20.h
 * @brief   DS18B20 1-Wire temperature sensor.
 *
 * Requires: one GPIO for DATA (~4.7k pull-up); us delays come from the DWT cycle counter
 *          (Cortex-M3/M4/M7).
 *
 * Usage:
 *   DS18B20_Device ds;
 *   DS18B20_Init(&ds, DS18B20_GPIO_Port, DS18B20_Pin);
 *   float t = DS18B20_ReadTemp(&ds);
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f1xx_hal.h"

typedef struct
{
	uint16_t      Pin;
	GPIO_TypeDef* PORT;
	float         Temp;
} DS18B20_Device;

#define DS18B20_READROM      0x33
#define DS18B20_MATCHROM     0x55
#define DS18B20_SKIPROM      0xCC
#define DS18B20_SEARCHROM    0xF0
#define DS18B20_ARLAMPSEARCH 0xEC
#define DS18B20_WRITESCRAT   0x4E
#define DS18B20_READSCRAT    0xBE
#define DS18B20_COPYSCRAT    0x48
#define DS18B20_CONVERT      0x44
#define DS18B20_READPWR      0xB4

void  DS18B20_Init(DS18B20_Device* DS18B20, GPIO_TypeDef* DS_PORT, uint16_t DS_Pin);
float DS18B20_ReadTemp(DS18B20_Device* DS18B20);

#endif
