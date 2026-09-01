/**
 * @file    srf05.h
 * @brief   SRF05 / HC-SR04 ultrasonic distance sensor.
 *
 * Requires: TRIG on a push-pull GPIO; ECHO on a timer channel in Input Capture mode,
 *          1 us/tick (Prescaler = TIMxCLK/1_000_000 - 1, Period = 0xFFFF).
 *
 * Usage:
 *   SRF05_Device us;
 *   SRF05_Init(&us, &htim3, TIM_CHANNEL_1, TRIG_GPIO_Port, TRIG_Pin);
 *   float cm = SRF05_Read(&us);   // returns -1.0f on timeout
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#ifndef __SRF05_H
#define __SRF05_H

#include "stm32f1xx_hal.h"

#define SRF05_US_PER_CM        58.0f   /* ~58 us di + ve cho moi cm (am thanh ~343 m/s) */
#define SRF05_ECHO_TIMEOUT_MS  40u     /* cho toi da 1 canh echo (~6.5 m) */

typedef struct {
	TIM_HandleTypeDef* Timer;      /*!< timer input capture, 1 us / tick */
	uint32_t           Channel;    /*!< TIM_CHANNEL_x noi voi chan ECHO */
	GPIO_TypeDef*      TRIGGER_GPIOx;
	uint16_t           TRIGGER_GPIO_Pin;
	float              Distance;   /*!< khoang cach do duoc gan nhat (cm) */
} SRF05_Device;

uint8_t SRF05_Init(SRF05_Device* SRF05, TIM_HandleTypeDef* Timer, uint32_t Channel,
                   GPIO_TypeDef* TRIGGER_GPIOx, uint16_t TRIGGER_GPIO_Pin);
float   SRF05_Read(SRF05_Device* SRF05);

#endif
