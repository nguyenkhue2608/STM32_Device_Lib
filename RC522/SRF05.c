/*****************************************************************************************************
@File: 		SRF05 Ultrasonic Module
@Author: khuenguyen
@website: khuenguyencreator.com
@youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg

@huong dan su dung:
- Cau hinh chan TRIG output no pull, Echo input no pull
- Tao mot bien luu gia tri doc duoc kieu SRF05_Typedef 
VD: SRF05_Typedef SRF05_01;
- Truyen dia chi 2 chan vao SRF05_Init
VD: SRF05_Init(&SRF05_01, ECHO_GPIO_Port, ECHO_Pin, TRIG_GPIO_Port, TRIG_Pin);
- Doc gia tri nhan duoc
VD: Value = SRF05_Read(&SRF05_01);

***Note: #include va cau hinh ham delay truyen vao
*********************** *****************************************************************************/
#include "SRF05.h"
#include "delay_timer.h"
//************************* Low Level Layer **********************************************************/
//#include "delay_timer.h"
#define TRIG_HIGH()		HAL_GPIO_WritePin(SRF05->TRIGGER_GPIOx, SRF05->TRIGGER_GPIO_Pin, GPIO_PIN_SET)
#define TRIG_LOW()		HAL_GPIO_WritePin(SRF05->TRIGGER_GPIOx, SRF05->TRIGGER_GPIO_Pin, GPIO_PIN_RESET)
#define READ_ECHO() 	HAL_GPIO_ReadPin(SRF05->ECHO_GPIOx, SRF05->ECHO_GPIO_Pin)
extern TIM_HandleTypeDef htim4;

static void SRF05_DELAY_Us(uint16_t Time)
{
	DELAY_TIM_Us(&htim4, Time); // thay the ham nay neu su dung ham delay khac
	
}

//************************* HIGH Level Layer **********************************************************/
// function:  init SRF05
// input: SRF05 target, TRIG pin, Echo Pin
// output: 0 false, 1 true
uint8_t SRF05_Init(SRF05_Device_Name* SRF05, GPIO_TypeDef* ECHO_GPIOx, uint16_t ECHO_GPIO_Pin, GPIO_TypeDef* TRIGGER_GPIOx, uint16_t TRIGGER_GPIO_Pin) {	

	SRF05->ECHO_GPIOx = ECHO_GPIOx;
	SRF05->ECHO_GPIO_Pin = ECHO_GPIO_Pin;
	SRF05->TRIGGER_GPIOx = TRIGGER_GPIOx;
	SRF05->TRIGGER_GPIO_Pin = TRIGGER_GPIO_Pin;
	TRIG_LOW();
	if (SRF05_Read(SRF05) >= 0) {
		return 1;
	}
	return 0;
}

float SRF05_Read(SRF05_Device_Name* SRF05) {
	uint32_t time, timeout;

	TRIG_LOW();
	SRF05_DELAY_Us(2);
	TRIG_HIGH();
	SRF05_DELAY_Us(10);
	TRIG_LOW();
	timeout = SRF05_TIMEOUT;
	while (!READ_ECHO()) 
	{
		if (timeout-- == 0x00) 
		{
			return -1;
		}
	}
	time = 0;
	while (READ_ECHO()) 
	{
		time++;
		SRF05_DELAY_Us(1);
	}
	SRF05->Distance =  (float)time * SRF05_NUMBER;

	return SRF05->Distance;
}
