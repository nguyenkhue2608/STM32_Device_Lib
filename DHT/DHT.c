/******************************************************************************************************************
@File:    DHT11 / DHT22 Sensor
@Author:  Khue Nguyen
@Website: khuenguyencreator.com
@Youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg

Luong doc:
 1. Host keo DATA xuong StartMs milli-giay roi tha ra.
 2. Cam bien tra loi: 80us LOW + 80us HIGH, sau do 40 bit, moi bit = 50us LOW + xung HIGH
    (26-28us = bit 0, ~70us = bit 1).
 3. Chuyen chan sang input capture, chi bat CANH XUONG (STM32F1 khong ho tro bat 2 canh).
    Do rong tu canh xuong nay den canh xuong ke tiep = chu ky mot bit:
      bit 0 ~ 77us, bit 1 ~ 120us  ->  so voi DHT_BIT_PERIOD_US.
 4. Kiem tra checksum.

Trong luc capture, ngat bi tam khoa (~5 ms) de vong poll khong bo lo canh; gia tri thoi
gian van chinh xac vi duoc phan cung chot tai CCRx.
******************************************************************************************************************/
#include "DHT.h"

/* So vong poll toi da cho mot canh (~vai us tren F1@72MHz cho moi vong) -> vai ms */
#define DHT_CAPTURE_GUARD  40000U
/* So canh xuong can bat: 1 (bat dau 80us LOW) + 1 (het response) + 40 (dau moi bit LOW)
   -> chu ky bit k = cap[k+2] - cap[k+1], k = 0..39 */
#define DHT_EDGE_COUNT     42U

//************************** Low Level Layer ********************************************************//

static void DHT_DWT_Init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;
}

static void DHT_DelayUs(uint32_t us)
{
	uint32_t start = DWT->CYCCNT;
	uint32_t ticks = us * (SystemCoreClock / 1000000U);
	while ((DWT->CYCCNT - start) < ticks) { }
}

static uint32_t DHT_TimFlag(uint32_t channel)
{
	switch (channel)
	{
		case TIM_CHANNEL_1: return TIM_FLAG_CC1;
		case TIM_CHANNEL_2: return TIM_FLAG_CC2;
		case TIM_CHANNEL_3: return TIM_FLAG_CC3;
		default:            return TIM_FLAG_CC4;
	}
}

static void DHT_SetPinOut(DHT_Name* DHT)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin   = DHT->Pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DHT->PORT, &GPIO_InitStruct);
}

static void DHT_SetPinIn(DHT_Name* DHT)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin  = DHT->Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DHT->PORT, &GPIO_InitStruct);
}

/* Cho 1 canh capture. Tra ve 0 neu qua so vong guard. */
static uint8_t DHT_WaitCapture(DHT_Name* DHT, uint32_t flag, uint32_t* value)
{
	uint32_t guard = 0;

	while (!__HAL_TIM_GET_FLAG(DHT->Timer, flag))
	{
		if (++guard > DHT_CAPTURE_GUARD) return 0;
	}
	__HAL_TIM_CLEAR_FLAG(DHT->Timer, flag);
	*value = HAL_TIM_ReadCapturedValue(DHT->Timer, DHT->Channel);
	return 1;
}

//************************** High Level Layer ********************************************************//

void DHT_Init(DHT_Name* DHT, uint8_t DHT_Type, TIM_HandleTypeDef* Timer, uint32_t Channel,
              GPIO_TypeDef* DH_PORT, uint16_t DH_Pin)
{
	DHT->Type    = DHT_Type;
	DHT->StartMs = (DHT_Type == DHT22) ? DHT22_START_MS : DHT11_START_MS;
	DHT->Timer   = Timer;
	DHT->Channel = Channel;
	DHT->PORT    = DH_PORT;
	DHT->Pin     = DH_Pin;

	DHT_DWT_Init();
	DHT_SetPinIn(DHT);
}

uint8_t DHT_ReadTempHum(DHT_Name* DHT)
{
	uint32_t cap[DHT_EDGE_COUNT];
	uint8_t  data[5] = {0};
	uint32_t flag = DHT_TimFlag(DHT->Channel);
	uint32_t i;
	uint8_t  ok = 1;

	/* 1. Xung start: keo DATA xuong StartMs ms (con IRQ de HAL_Delay chay) */
	DHT_SetPinOut(DHT);
	HAL_GPIO_WritePin(DHT->PORT, DHT->Pin, GPIO_PIN_RESET);
	HAL_Delay(DHT->StartMs);

	/* 2. Khoa IRQ, tha chan va vu trang input capture (chi canh xuong) NGAY,
	   truoc khi cam bien keo xuong (~20-40us sau khi tha). */
	__disable_irq();
	HAL_GPIO_WritePin(DHT->PORT, DHT->Pin, GPIO_PIN_SET);
	DHT_SetPinIn(DHT);
	DHT_DelayUs(2);

	__HAL_TIM_SET_CAPTUREPOLARITY(DHT->Timer, DHT->Channel, TIM_INPUTCHANNELPOLARITY_FALLING);
	if (HAL_TIM_IC_Start(DHT->Timer, DHT->Channel) != HAL_OK) { __enable_irq(); return 0; }
	__HAL_TIM_SET_COUNTER(DHT->Timer, 0);
	__HAL_TIM_CLEAR_FLAG(DHT->Timer, flag);

	for (i = 0; i < DHT_EDGE_COUNT; i++)
	{
		if (!DHT_WaitCapture(DHT, flag, &cap[i])) { ok = 0; break; }
	}

	__enable_irq();
	HAL_TIM_IC_Stop(DHT->Timer, DHT->Channel);

	if (!ok) return 0;

	/* 3. Kiem tra header: cap[1] - cap[0] ~ 160us (80us LOW + 80us HIGH) */
	if (((cap[1] - cap[0]) & 0xFFFFU) < 120U) return 0;

	/* 4. Giai ma: chu ky bit k = cap[k+2] - cap[k+1] */
	for (i = 0; i < 40; i++)
	{
		uint32_t period = (cap[i + 2] - cap[i + 1]) & 0xFFFFU;
		if (period > DHT_BIT_PERIOD_US)
			data[i / 8] |= (uint8_t)(1U << (7 - (i % 8)));
	}

	/* 5. Kiem tra checksum */
	if (((uint16_t)(data[0] + data[1] + data[2] + data[3]) & 0xFF) != data[4])
		return 0;

	/* 6. Chuyen doi theo tung dong cam bien */
	if (DHT->Type == DHT11)
	{
		DHT->Humi = (float)data[0] + (float)data[1] * 0.1f;
		DHT->Temp = (float)data[2] + (float)(data[3] & 0x0F) * 0.1f;
	}
	else /* DHT22 */
	{
		int16_t traw = (int16_t)(((data[2] & 0x7F) << 8) | data[3]);
		DHT->Humi = (float)(((uint16_t)data[0] << 8) | data[1]) / 10.0f;
		DHT->Temp = (data[2] & 0x80) ? (-(float)traw / 10.0f) : ((float)traw / 10.0f);
	}
	return 1;
}
