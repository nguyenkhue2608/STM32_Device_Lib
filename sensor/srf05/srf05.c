/**
 * @file    srf05.c
 * @brief   SRF05 / HC-SR04 ultrasonic distance sensor - implementation.
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#include "srf05.h"

//************************* Low Level Layer *********************************************************/

/* Delay us bang DWT cycle counter (Cortex-M3/M4), chi dung cho xung trigger */
static void SRF05_DWT_Init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;
}

static void SRF05_DelayUs(uint32_t us)
{
	uint32_t start = DWT->CYCCNT;
	uint32_t ticks = us * (SystemCoreClock / 1000000U);
	while ((DWT->CYCCNT - start) < ticks) { }
}

static uint32_t SRF05_TimFlag(uint32_t channel)
{
	switch (channel)
	{
		case TIM_CHANNEL_1: return TIM_FLAG_CC1;
		case TIM_CHANNEL_2: return TIM_FLAG_CC2;
		case TIM_CHANNEL_3: return TIM_FLAG_CC3;
		default:            return TIM_FLAG_CC4;
	}
}

static uint8_t SRF05_WaitCapture(SRF05_Device* SRF05, uint32_t* value)
{
	uint32_t flag  = SRF05_TimFlag(SRF05->Channel);
	uint32_t start = HAL_GetTick();

	while (!__HAL_TIM_GET_FLAG(SRF05->Timer, flag))
	{
		if ((HAL_GetTick() - start) > SRF05_ECHO_TIMEOUT_MS) return 0;
	}
	__HAL_TIM_CLEAR_FLAG(SRF05->Timer, flag);
	*value = HAL_TIM_ReadCapturedValue(SRF05->Timer, SRF05->Channel);
	return 1;
}

//************************* High Level Layer *******************************************************/

uint8_t SRF05_Init(SRF05_Device* SRF05, TIM_HandleTypeDef* Timer, uint32_t Channel,
                   GPIO_TypeDef* TRIGGER_GPIOx, uint16_t TRIGGER_GPIO_Pin)
{
	SRF05->Timer            = Timer;
	SRF05->Channel          = Channel;
	SRF05->TRIGGER_GPIOx    = TRIGGER_GPIOx;
	SRF05->TRIGGER_GPIO_Pin = TRIGGER_GPIO_Pin;
	SRF05->Distance         = -1.0f;

	SRF05_DWT_Init();
	HAL_GPIO_WritePin(TRIGGER_GPIOx, TRIGGER_GPIO_Pin, GPIO_PIN_RESET);

	return (SRF05_Read(SRF05) >= 0.0f) ? 1 : 0;
}

float SRF05_Read(SRF05_Device* SRF05)
{
	uint32_t t_rise, t_fall, width;

	/* Xung trigger: LOW 2us -> HIGH 10us -> LOW */
	HAL_GPIO_WritePin(SRF05->TRIGGER_GPIOx, SRF05->TRIGGER_GPIO_Pin, GPIO_PIN_RESET);
	SRF05_DelayUs(2);
	HAL_GPIO_WritePin(SRF05->TRIGGER_GPIOx, SRF05->TRIGGER_GPIO_Pin, GPIO_PIN_SET);
	SRF05_DelayUs(10);
	HAL_GPIO_WritePin(SRF05->TRIGGER_GPIOx, SRF05->TRIGGER_GPIO_Pin, GPIO_PIN_RESET);

	/* Canh len cua ECHO */
	__HAL_TIM_SET_CAPTUREPOLARITY(SRF05->Timer, SRF05->Channel, TIM_INPUTCHANNELPOLARITY_RISING);
	if (HAL_TIM_IC_Start(SRF05->Timer, SRF05->Channel) != HAL_OK)
	{
		SRF05->Distance = -1.0f;
		return -1.0f;
	}
	__HAL_TIM_CLEAR_FLAG(SRF05->Timer, SRF05_TimFlag(SRF05->Channel));

	if (!SRF05_WaitCapture(SRF05, &t_rise))
	{
		HAL_TIM_IC_Stop(SRF05->Timer, SRF05->Channel);
		SRF05->Distance = -1.0f;
		return -1.0f;
	}

	/* Canh xuong cua ECHO (khong clear flag: doc CCRx o tren da xoa CCxIF) */
	__HAL_TIM_SET_CAPTUREPOLARITY(SRF05->Timer, SRF05->Channel, TIM_INPUTCHANNELPOLARITY_FALLING);

	if (!SRF05_WaitCapture(SRF05, &t_fall))
	{
		HAL_TIM_IC_Stop(SRF05->Timer, SRF05->Channel);
		SRF05->Distance = -1.0f;
		return -1.0f;
	}

	HAL_TIM_IC_Stop(SRF05->Timer, SRF05->Channel);

	width = (t_fall - t_rise) & 0xFFFFU;   /* mask 16 bit: xu ly 1 lan tran counter */
	SRF05->Distance = (float)width / SRF05_US_PER_CM;
	return SRF05->Distance;
}
