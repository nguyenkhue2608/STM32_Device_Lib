/**
 * @file    dht.h
 * @brief   DHT11 / DHT22 temperature & humidity sensor.
 *
 * Requires: one GPIO for DATA (~4.7k pull-up) + one timer channel in Input Capture mode,
 *          1 us/tick (Prescaler = TIMxCLK/1_000_000 - 1, Period = 0xFFFF).
 *
 * Usage:
 *   DHT_Device dht;
 *   DHT_Init(&dht, DHT11, &htim2, TIM_CHANNEL_1, DHT_GPIO_Port, DHT_Pin);
 *   if (DHT_ReadTempHum(&dht)) { ... dht.Temp, dht.Humi ... }
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#ifndef __DHT_H
#define __DHT_H

#include "stm32f1xx_hal.h"

#define DHT11 0x01
#define DHT22 0x02

/* Do dai xung "start" host keo xuong (ms): DHT11 >= 18 ms, DHT22 ~1..2 ms */
#define DHT11_START_MS 18
#define DHT22_START_MS 2

/* Nguong chu ky bit (falling->falling): bit 0 ~ 77us, bit 1 ~ 120us */
#define DHT_BIT_PERIOD_US 100

typedef struct
{
	uint8_t            Type;      /*!< DHT11 hoac DHT22 */
	uint16_t           StartMs;   /*!< do dai xung start, suy ra tu Type */
	TIM_HandleTypeDef* Timer;     /*!< timer input capture, 1 us / tick */
	uint32_t           Channel;   /*!< TIM_CHANNEL_x noi voi chan DATA */
	GPIO_TypeDef*      PORT;
	uint16_t           Pin;
	float              Temp;
	float              Humi;
} DHT_Device;

void    DHT_Init(DHT_Device* DHT, uint8_t DHT_Type, TIM_HandleTypeDef* Timer, uint32_t Channel,
                 GPIO_TypeDef* DH_PORT, uint16_t DH_Pin);
uint8_t DHT_ReadTempHum(DHT_Device* DHT);   /*!< 1 = OK, 0 = loi/timeout/checksum sai */

#endif
