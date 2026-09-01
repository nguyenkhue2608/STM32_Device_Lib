/**
 * @file    button.c
 * @brief   Debounced push-button (single / double / long press) - implementation.
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#include "button.h"

// ************************** Low Level Layer *****************************************************//
static uint8_t BUTTON_ReadPin(BUTTON_Device* Button)
{
	return HAL_GPIO_ReadPin(Button->BUTTON_PORT, Button->BUTTON_Pin);
}
static void BUTTON_DelayMs(uint16_t Time)
{
	HAL_Delay(Time);
}

// ************************** High Level Layer *****************************************************//
void BUTTON_Init(BUTTON_Device* Button, GPIO_TypeDef* BUTTON_PORT, uint16_t BUTTON_Pin)
{
	Button->BUTTON_PORT = BUTTON_PORT;
	Button->BUTTON_Pin = BUTTON_Pin;
}

BUTTON_STATE BUTTON_Read(BUTTON_Device* Button)
{
	Button->State = NO_CLICK;
	while(BUTTON_ReadPin(Button) == 0)
	{
		Button->timePress++;
		Button->isPress = 1;
		BUTTON_DelayMs(1);
	}
	if(Button->isPress)
	{
		while(BUTTON_ReadPin(Button) == 1)
		{
			Button->timeDouble++;
			BUTTON_DelayMs(1);
			if(Button->timeDouble > DOUBLE_CLICK_TIME)
			{
				if(Button->timePress > DEBOUND_TIME && Button->timePress <= SINGLE_CLICK_TIME)
				{
					Button->isPress = 0;
					Button->timePress = 0;
					Button->timeDouble = 0;
					Button->State = SINGLE_CLICK;
					return Button->State;
				}
				else if(Button->timePress  > SINGLE_CLICK_TIME)
				{
					Button->isPress = 0;
					Button->timePress = 0;
					Button->timeDouble = 0;
					Button->State = LONGCLICK_1S;
					return Button->State;
				}
			}
		}
		while(BUTTON_ReadPin(Button) == 0)
		{
			Button->State = DOUBLE_CLICK;
			Button->isPress = 0;
			Button->timePress = 0;
			Button->timeDouble = 0;
			return Button->State;
		}
	}
	return NO_CLICK;
}
	

