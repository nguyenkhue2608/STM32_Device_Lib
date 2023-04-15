/*****************************************************************************************************
@tac gia: khuenguyen
@website: khuenguyencreator.com
@youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
@huong dan su dung:
- Khoi tao timer dem moi 1us VD: fapb2 = 72 mhz. prescaler = 72 -1, ARR = 0xFFFF -1 
- Truyen timer do vao delay Init VD: DELAY_TIM_Init(&htim1)
- Su dung thu vien hal

*****************************************************************************************************/
#include "delay_timer.h"
void DELAY_TIM_Init(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Base_Start(htim);
}

void DELAY_TIM_Us(TIM_HandleTypeDef *htim, uint16_t time)
{
	__HAL_TIM_SET_COUNTER(htim,0);
	while(__HAL_TIM_GET_COUNTER(htim)<time){}
}
void DELAY_TIM_Ms(TIM_HandleTypeDef *htim, uint16_t Time)
{
	__HAL_TIM_SET_COUNTER(htim,0);
	while(Time--)
	{
		while(__HAL_TIM_GET_COUNTER(htim)<1000){}
	}
}
