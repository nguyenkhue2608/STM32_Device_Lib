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
#include "DS18B20.h"
//************************** Low Level Layer ********************************************************//
#include "delay_timer.h"

static void DS18B20_DelayInit(DS18B20_Name* DS18B20)
{
	DELAY_TIM_Init(DS18B20->Timer);
}
static void DS18B20_DelayUs(DS18B20_Name* DS18B20, uint16_t Time)
{
	DELAY_TIM_Us(DS18B20->Timer, Time);
}
static void DS18B20_DelayMs(DS18B20_Name* DS18B20, uint16_t Time)
{
	DELAY_TIM_Ms(DS18B20->Timer, Time);
}

static void DS18B20_SetPinOut(DS18B20_Name* DS18B20)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DS18B20->Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DS18B20->PORT, &GPIO_InitStruct);
}
static void DS18B20_SetPinIn(DS18B20_Name* DS18B20)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DS18B20->Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(DS18B20->PORT, &GPIO_InitStruct);
}
static void DS18B20_WritePin(DS18B20_Name* DS18B20, uint8_t Value)
{
	HAL_GPIO_WritePin(DS18B20->PORT, DS18B20->Pin, Value);
}
static uint8_t DS18B20_ReadPin(DS18B20_Name* DS18B20)
{
	return  HAL_GPIO_ReadPin(DS18B20->PORT, DS18B20->Pin);
}
//********************************* Middle level Layer ****************************************************//
static uint8_t DS18B20_Start(DS18B20_Name* DS18B20)
{
	uint8_t Response = 0;
	DS18B20_SetPinOut(DS18B20);  
	DS18B20_WritePin(DS18B20, 0);
	DS18B20_DelayUs(DS18B20, 480);   
	DS18B20_SetPinIn(DS18B20);    
	DS18B20_DelayUs(DS18B20, 80); 
	if (!(DS18B20_ReadPin(DS18B20))) Response = 1;   
	else Response = 0;
	DS18B20_DelayUs(DS18B20, 400); // 480 us delay totally.

	return Response;
}
static void DS18B20_Write(DS18B20_Name* DS18B20, uint8_t Data)
{
	DS18B20_SetPinOut(DS18B20);
	for(int i = 0; i<8; i++)
	{
		if((Data&(1<<i))!=0)
		{
			DS18B20_SetPinOut(DS18B20);
			DS18B20_WritePin(DS18B20, 0);
			DS18B20_DelayUs(DS18B20, 1);
			DS18B20_SetPinIn(DS18B20);
			DS18B20_DelayUs(DS18B20, 50);
		}
		else
		{
			DS18B20_SetPinOut(DS18B20);
			DS18B20_WritePin(DS18B20, 0);
			DS18B20_DelayUs(DS18B20, 50);
			DS18B20_SetPinIn(DS18B20);
		}
	}
}
static uint8_t DS18B20_Read(DS18B20_Name* DS18B20)
{
	uint8_t Value = 0;
	DS18B20_SetPinIn(DS18B20);
	for(int i = 0; i<8; i++)
	{
		DS18B20_SetPinOut(DS18B20);
		DS18B20_WritePin(DS18B20, 0);
		DS18B20_DelayUs(DS18B20, 1);
		DS18B20_SetPinIn(DS18B20);
		if(DS18B20_ReadPin(DS18B20))
		{
			Value |= 1<<i;
		}
		DS18B20_DelayUs(DS18B20, 50);
	}
	return Value;
}

//************************** High Level Layer ********************************************************//
void DS18B20_Init(DS18B20_Name* DS18B20, TIM_HandleTypeDef* Timer, GPIO_TypeDef* DS_PORT, uint16_t DS_Pin)
{
	DS18B20->PORT = DS_PORT;
	DS18B20->Pin = DS_Pin;
	DS18B20->Timer = Timer;
	DS18B20_DelayInit(DS18B20);
}

float DS18B20_ReadTemp(DS18B20_Name* DS18B20)
{
	uint8_t Temp1, Temp2;
	uint16_t Temp;
	DS18B20_Start(DS18B20);
	DS18B20_DelayMs(DS18B20, 1);
	DS18B20_Write(DS18B20, DS18B20_SKIPROM);
	DS18B20_Write(DS18B20, DS18B20_CONVERT);
	DS18B20_DelayMs(DS18B20, 200);
	DS18B20_Start(DS18B20);
	DS18B20_DelayMs(DS18B20, 1);
	DS18B20_Write(DS18B20, DS18B20_SKIPROM);
	DS18B20_Write(DS18B20, DS18B20_READSCRAT);
	Temp1 = DS18B20_Read(DS18B20);
	Temp2 = DS18B20_Read(DS18B20);
	Temp = (Temp2<<8)|Temp1;
	DS18B20->Temp = (float)(Temp/16);
	return DS18B20->Temp;
}
