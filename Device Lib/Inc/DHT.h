
#ifndef __DHT_H
#define __DHT_H

#include "stm32f1xx_hal.h"
#define DHT11_STARTTIME 18000
#define DHT22_STARTTIME 12000
#define DHT11 0x01
#define DHT22 0x02
typedef struct
{	
	uint16_t Type;
	TIM_HandleTypeDef* Timer;
	uint16_t Pin;
	GPIO_TypeDef* PORT;
	float Temp;
	float Humi;
}DHT_Name;

void DHT_Init(DHT_Name* DHT, uint8_t DHT_Type, TIM_HandleTypeDef* Timer, GPIO_TypeDef* DH_PORT, uint16_t DH_Pin);
uint8_t DHT_ReadTempHum(DHT_Name* DHT);
#endif

