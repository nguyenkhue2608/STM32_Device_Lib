/******************************************************************************************************************
@File:    DS18B20 Sensor
@Author:  Khue Nguyen
@Website: khuenguyencreator.com
@Youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg

1-Wire: master tu tao cac khe thoi gian (reset 480us, write 60us, doc mau trong 15us).
Delay micro-giay lay tu bo dem chu ky DWT (Cortex-M3/M4), delay milli-giay dung HAL_Delay.
******************************************************************************************************************/
#include "ds18b20.h"

//************************** Low Level Layer ********************************************************//

static void DS18B20_DWT_Init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;
}

static void DS18B20_DelayUs(uint32_t us)
{
	uint32_t start = DWT->CYCCNT;
	uint32_t ticks = us * (SystemCoreClock / 1000000U);
	while ((DWT->CYCCNT - start) < ticks) { }
}

static void DS18B20_SetPinOut(DS18B20_Name* DS18B20)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin   = DS18B20->Pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DS18B20->PORT, &GPIO_InitStruct);
}

static void DS18B20_SetPinIn(DS18B20_Name* DS18B20)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin  = DS18B20->Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DS18B20->PORT, &GPIO_InitStruct);
}

static void DS18B20_WritePin(DS18B20_Name* DS18B20, uint8_t Value)
{
	HAL_GPIO_WritePin(DS18B20->PORT, DS18B20->Pin, (GPIO_PinState)Value);
}

static uint8_t DS18B20_ReadPin(DS18B20_Name* DS18B20)
{
	return HAL_GPIO_ReadPin(DS18B20->PORT, DS18B20->Pin);
}

//********************************* Middle level Layer ****************************************************//

static uint8_t DS18B20_Start(DS18B20_Name* DS18B20)
{
	uint8_t Response = 0;

	DS18B20_SetPinOut(DS18B20);
	DS18B20_WritePin(DS18B20, 0);
	DS18B20_DelayUs(480);
	DS18B20_SetPinIn(DS18B20);
	DS18B20_DelayUs(80);
	if (!DS18B20_ReadPin(DS18B20)) Response = 1;
	else                           Response = 0;
	DS18B20_DelayUs(400);           /* du 480us cho khe reset */

	return Response;
}

static void DS18B20_WriteBit(DS18B20_Name* DS18B20, uint8_t bit)
{
	if (bit)   /* Write 1: keo xuong ~2us roi tha, du khe 60us */
	{
		DS18B20_SetPinOut(DS18B20);
		DS18B20_WritePin(DS18B20, 0);
		DS18B20_DelayUs(2);
		DS18B20_SetPinIn(DS18B20);
		DS18B20_DelayUs(60);
	}
	else       /* Write 0: giu xuong ca khe 60us */
	{
		DS18B20_SetPinOut(DS18B20);
		DS18B20_WritePin(DS18B20, 0);
		DS18B20_DelayUs(60);
		DS18B20_SetPinIn(DS18B20);
		DS18B20_DelayUs(2);
	}
}

static uint8_t DS18B20_ReadBit(DS18B20_Name* DS18B20)
{
	uint8_t bit;

	DS18B20_SetPinOut(DS18B20);
	DS18B20_WritePin(DS18B20, 0);
	DS18B20_DelayUs(2);
	DS18B20_SetPinIn(DS18B20);
	DS18B20_DelayUs(10);              /* doc mau trong 15us dau khe */
	bit = DS18B20_ReadPin(DS18B20) ? 1 : 0;
	DS18B20_DelayUs(50);
	return bit;
}

static void DS18B20_Write(DS18B20_Name* DS18B20, uint8_t Data)
{
	int i;
	for (i = 0; i < 8; i++)
		DS18B20_WriteBit(DS18B20, (Data >> i) & 0x01);
}

static uint8_t DS18B20_Read(DS18B20_Name* DS18B20)
{
	uint8_t Value = 0;
	int i;
	for (i = 0; i < 8; i++)
		if (DS18B20_ReadBit(DS18B20)) Value |= (uint8_t)(1 << i);
	return Value;
}

//************************** High Level Layer ********************************************************//

void DS18B20_Init(DS18B20_Name* DS18B20, GPIO_TypeDef* DS_PORT, uint16_t DS_Pin)
{
	DS18B20->PORT = DS_PORT;
	DS18B20->Pin  = DS_Pin;
	DS18B20_DWT_Init();
}

float DS18B20_ReadTemp(DS18B20_Name* DS18B20)
{
	uint8_t  Temp1, Temp2;
	int16_t  raw;

	DS18B20_Start(DS18B20);
	DS18B20_Write(DS18B20, DS18B20_SKIPROM);
	DS18B20_Write(DS18B20, DS18B20_CONVERT);

	/* Cho chuyen doi: DS18B20 giu DATA = 0 khi ban, nha len 1 khi xong (12 bit ~ 750 ms) */
	{
		uint32_t t0 = HAL_GetTick();
		while (!DS18B20_ReadBit(DS18B20))
		{
			if ((HAL_GetTick() - t0) > 800u) break;
		}
	}

	DS18B20_Start(DS18B20);
	DS18B20_Write(DS18B20, DS18B20_SKIPROM);
	DS18B20_Write(DS18B20, DS18B20_READSCRAT);
	Temp1 = DS18B20_Read(DS18B20);
	Temp2 = DS18B20_Read(DS18B20);

	raw = (int16_t)(((uint16_t)Temp2 << 8) | Temp1);
	DS18B20->Temp = (float)raw / 16.0f;   /* LSB = 1/16 do C, co xu ly nhiet do am */
	return DS18B20->Temp;
}
