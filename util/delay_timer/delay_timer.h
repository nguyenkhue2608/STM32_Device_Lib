/**
 * @file    delay_timer.h
 * @brief   Blocking microsecond / millisecond delay from a general-purpose timer.
 *
 * Requires: one timer free-running at 1 us/tick (Prescaler = TIMxCLK/1_000_000 - 1,
 *          Period = 0xFFFF). Standalone helper - no driver depends on it.
 *
 * Usage:
 *   DELAY_TIM_Init(&htim1);
 *   DELAY_TIM_Us(&htim1, 50);
 *   DELAY_TIM_Ms(&htim1, 100);
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#ifndef __DELAY_TIMER_H
#define __DELAY_TIMER_H
#include "stm32f1xx_hal.h"
void DELAY_TIM_Init(TIM_HandleTypeDef *htim);
void DELAY_TIM_Us(TIM_HandleTypeDef *htim, uint16_t time);
void DELAY_TIM_Ms(TIM_HandleTypeDef *htim, uint16_t Time);
#endif
