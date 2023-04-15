/******************************************************************************************************************
@File:  	DS18B20 Sensor
@Author:  Khue Nguyen
@Website: khuenguyencreator.com
@Youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
Huong dan su dung:
- Su dung thu vien HAL
- Khoi tao bien DS18B20 : DS18B20_Name DS1;
- Khoi tao chan DS18B20:
	DS18B20_Init(&DS1, &htim4, DS18B20_GPIO_Port, DS18B20_Pin);
- Su dung cac ham phai truyen dia chi cua DS18B20 do: 
	Temp = DS18B20_ReadTemp(&DS1);
******************************************************************************************************************/
#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f1xx_hal.h"
typedef struct
{	
	TIM_HandleTypeDef* Timer;
	uint16_t Pin;
	GPIO_TypeDef* PORT;
	float Temp;
}DS18B20_Name;
#define DS18B20_STARTLOW 18000
#define DS18B20_STARTHIGH 20

#define DS18B20_READROM 0x33
#define DS18B20_MATCHROM 0x55
#define DS18B20_SKIPROM 0xCC
#define DS18B20_SEARCHROM 0xF0
#define DS18B20_ARLAMPSEARCH 0xEC
#define DS18B20_WRITESCRAT 0x4E
#define DS18B20_READSCRAT 0xBE
#define DS18B20_COPYSCRAT 0x48
#define DS18B20_CONVERT	0x44
#define DS18B20_READPWR	0xB4

void DS18B20_Init(DS18B20_Name* DS18B20, TIM_HandleTypeDef* Timer, GPIO_TypeDef* DS_PORT, uint16_t DS_Pin);
float DS18B20_ReadTemp(DS18B20_Name* DS18B20);
#endif

