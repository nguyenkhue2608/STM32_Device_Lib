/******************************************************************************************************************
@File:  	DHT Sensor
@Author:  Khue Nguyen
@Website: khuenguyencreator.com
@Youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
Huong dan su dung:
- Su dung thu vien HAL
- Khoi tao bien DHT : DHT_Name DHT1;
- Khoi tao chan DHT:
	DHT_Init(&DHT1, DHT11, &htim4, DHT_GPIO_Port, DHT_Pin);
- Su dung cac ham phai truyen dia chi cua DHT do: 
	DHT_ReadTempHum(&DHT1);
******************************************************************************************************************/
#include "DHT.h"
//************************** Low Level Layer ********************************************************//
#include "delay_timer.h"

static void DHT_DelayInit(DHT_Name* DHT)
{
	DELAY_TIM_Init(DHT->Timer);
}
static void DHT_DelayUs(DHT_Name* DHT, uint16_t Time)
{
	DELAY_TIM_Us(DHT->Timer, Time);
}

static void DHT_SetPinOut(DHT_Name* DHT)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DHT->Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT->PORT, &GPIO_InitStruct);
}
static void DHT_SetPinIn(DHT_Name* DHT)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DHT->Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(DHT->PORT, &GPIO_InitStruct);
}
static void DHT_WritePin(DHT_Name* DHT, uint8_t Value)
{
	HAL_GPIO_WritePin(DHT->PORT, DHT->Pin, Value);
}
static uint8_t DHT_ReadPin(DHT_Name* DHT)
{
	uint8_t Value;
	Value =  HAL_GPIO_ReadPin(DHT->PORT, DHT->Pin);
	return Value;
}
//********************************* Middle level Layer ****************************************************//
static uint8_t DHT_Start(DHT_Name* DHT)
{
	uint8_t Response = 0;
	DHT_SetPinOut(DHT);  
	DHT_WritePin(DHT, 0);
	DHT_DelayUs(DHT, DHT->Type);   
	DHT_SetPinIn(DHT);    
	DHT_DelayUs(DHT, 40); 
	if (!DHT_ReadPin(DHT))
	{
		DHT_DelayUs(DHT, 40); 
		if(DHT_ReadPin(DHT))
		{
			Response = 1;   
		}
		else Response = 0;  
	}		
	while(DHT_ReadPin(DHT));

	return Response;
}
static uint8_t DHT_Read(DHT_Name* DHT)
{
	uint8_t Value = 0;
	DHT_SetPinIn(DHT);
	for(int i = 0; i<8; i++)
	{
		while(!DHT_ReadPin(DHT));
		DHT_DelayUs(DHT, 40);
		if(!DHT_ReadPin(DHT))
		{
			Value &= ~(1<<(7-i));	
		}
		else Value |= 1<<(7-i);
		while(DHT_ReadPin(DHT));
	}
	return Value;
}

//************************** High Level Layer ********************************************************//
void DHT_Init(DHT_Name* DHT, uint8_t DHT_Type, TIM_HandleTypeDef* Timer, GPIO_TypeDef* DH_PORT, uint16_t DH_Pin)
{
	if(DHT_Type == DHT11)
	{
		DHT->Type = DHT11_STARTTIME;
	}
	else if(DHT_Type == DHT22)
	{
		DHT->Type = DHT22_STARTTIME;
	}
	DHT->PORT = DH_PORT;
	DHT->Pin = DH_Pin;
	DHT->Timer = Timer;
	DHT_DelayInit(DHT);
}

uint8_t DHT_ReadTempHum(DHT_Name* DHT)
{
	uint8_t Temp1, Temp2, RH1, RH2;
	uint16_t Temp, Humi, SUM = 0;
	DHT_Start(DHT);
	RH1 = DHT_Read(DHT);
	RH2 = DHT_Read(DHT);
	Temp1 = DHT_Read(DHT);
	Temp2 = DHT_Read(DHT);
	SUM = DHT_Read(DHT);
	Temp = (Temp1<<8)|Temp2;
	Humi = (RH1<<8)|RH2;
	DHT->Temp = (float)(Temp/10.0);
	DHT->Humi = (float)(Humi/10.0);
	return SUM;
}
