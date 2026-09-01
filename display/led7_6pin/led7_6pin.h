/**
 * @file    led7_6pin.h
 * @brief   6-pin multiplexed 7-segment LED display.
 *
 * Requires: 6 GPIO pins on one port. Call the scan handler periodically (e.g. from a 1 kHz timer).
 *
 * Usage:
 *   LED7_6PIN_Init(GPIOA);
 *   LED7_6PIN_DisplayNumber(1234);
 *   // in a periodic timer ISR:
 *   LED7_6PIN_Scan_Handler();
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#ifndef __LED7_6PIN_H
#define __LED7_6PIN_H

#include "stm32f1xx_hal.h"

#define CATHODE1 GPIO_PIN_1
#define CATHODE2 GPIO_PIN_2
#define CATHODE3 GPIO_PIN_3
#define CATHODE4 GPIO_PIN_4
#define CATHODE5 GPIO_PIN_5
#define CATHODE6 GPIO_PIN_6

typedef struct {
    uint16_t cathode;
    uint16_t anode;
} LED7_6PIN_Config;

extern LED7_6PIN_Config ledConfig[3][8];
extern const uint8_t digitPatterns[10][8];

void LED7_6PIN_Init(GPIO_TypeDef* GPIOx);
void LED7_6PIN_DisplayNumber(uint16_t number);
void LED7_6PIN_Scan(void);
void LED7_6PIN_Scan_Handler(void);

#endif
