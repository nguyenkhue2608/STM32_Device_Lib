/******************************************************************************************************************
@File:    DHT11 / DHT22 Sensor
@Author:  Khue Nguyen
@Website: khuenguyencreator.com
@Youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg

Huong dan su dung:
- Chan DATA noi voi 1 kenh timer o che do Input Capture (TIMx_CHy), moi tick = 1us
  (Prescaler = TIMxCLK/1_000_000 - 1, Period = 0xFFFF). Chan can dien tro keo len ~4.7k.
- Khoi tao:
    DHT_Name dht1;
    DHT_Init(&dht1, DHT11, &htim2, TIM_CHANNEL_1, DHT_GPIO_Port, DHT_Pin);
- Doc du lieu (tra ve 1 neu OK va checksum dung, 0 neu loi/timeout):
    if (DHT_ReadTempHum(&dht1)) { ... dht1.Temp, dht1.Humi ... }
******************************************************************************************************************/
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
} DHT_Name;

void    DHT_Init(DHT_Name* DHT, uint8_t DHT_Type, TIM_HandleTypeDef* Timer, uint32_t Channel,
                 GPIO_TypeDef* DH_PORT, uint16_t DH_Pin);
uint8_t DHT_ReadTempHum(DHT_Name* DHT);   /*!< 1 = OK, 0 = loi/timeout/checksum sai */

#endif
