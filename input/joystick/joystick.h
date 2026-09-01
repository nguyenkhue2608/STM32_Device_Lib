/**
 * @file    joystick.h
 * @brief   Analog 2-axis joystick with push-button.
 *
 * Requires: an ADC with two channels (X, Y) + one GPIO for the button.
 *
 * Usage:
 *   JOYSTICK_Device js;
 *   JOYSTICK_Init(&js, &hadc1, ADC_CHANNEL_0, ADC_CHANNEL_1, SW_GPIO_Port, SW_Pin);
 *   JOYSTICK_GetValue(&js);   // js.Xdirect, js.Ydirect, js.ButtonState
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "stm32f1xx_hal.h"
typedef enum
{
	No_Action = 0,
	X_Inc = 1,
	X_Dec = 2,
	Y_Inc = 3,
	Y_Dec = 4,
	isPress = 5,
	noPress = 6
}JOYSTICK_Value;

typedef struct
{
	ADC_HandleTypeDef* ADC;
	uint32_t Xchannel;
	uint32_t Ychannel;
	GPIO_TypeDef* Button_Port;
	uint16_t Button_Pin;
	uint16_t ADCvalue[2];
	JOYSTICK_Value Xdirect;
	JOYSTICK_Value	Ydirect;
	JOYSTICK_Value ButtonState;
}JOYSTICK_Device;
void JOYSTICK_Init(JOYSTICK_Device* JT, ADC_HandleTypeDef* ADC_In, uint32_t Xchannel, uint32_t Ychannel, 
										GPIO_TypeDef* Button_Port, uint16_t Button_Pin);
void JOYSTICK_GetValue(JOYSTICK_Device* JT);
JOYSTICK_Value JOYSTICK_GetButton(JOYSTICK_Device* JT);
#endif
