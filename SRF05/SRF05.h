/*****************************************************************************************************
@File:    SRF05 / HC-SR04 Ultrasonic Module
@Author:  khuenguyen
@website: khuenguyencreator.com
@youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg

@huong dan su dung:
- TRIG: cau hinh 1 chan GPIO output push-pull.
- ECHO: cau hinh 1 kenh timer o che do Input Capture (TIMx_CHy), moi tick = 1us
        (Prescaler = TIMxCLK/1_000_000 - 1, Period = 0xFFFF).
- Khoi tao:
    SRF05_Device_Name us1;
    SRF05_Init(&us1, &htim3, TIM_CHANNEL_1, TRIG_GPIO_Port, TRIG_Pin);
- Doc khoang cach (cm), tra ve -1.0f neu qua thoi gian cho (khong co vat can):
    float cm = SRF05_Read(&us1);
*****************************************************************************************************/
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
} SRF05_Device_Name;

uint8_t SRF05_Init(SRF05_Device_Name* SRF05, TIM_HandleTypeDef* Timer, uint32_t Channel,
                   GPIO_TypeDef* TRIGGER_GPIOx, uint16_t TRIGGER_GPIO_Pin);
float   SRF05_Read(SRF05_Device_Name* SRF05);

#endif
