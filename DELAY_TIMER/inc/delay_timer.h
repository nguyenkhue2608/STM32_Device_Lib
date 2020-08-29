/*****************************************************************************************************
@tac gia: khuenguyen
@webstie: khuenguyencreator.com

@huong dan su dung:
- Khoi tao timer dem moi 1us VD: fapb2 = 72 mhz. prescaler = 72 -1, ARR = 0xFFFF -1 
- Truyen timer do vao delay Init VD: DELAY_TIM_Init(&htim1)
- Su dung thu vien hal

*****************************************************************************************************/

#ifndef __DELAY_TIMER_H
#define __DELAY_TIMER_H
#include "stm32f1xx_hal.h"
void DELAY_TIM_Init(TIM_HandleTypeDef *htim);
void DELAY_TIM_Us(TIM_HandleTypeDef *htim, uint16_t time);
void DELAY_TIM_Ms(TIM_HandleTypeDef *htim, uint16_t Time);
#endif
