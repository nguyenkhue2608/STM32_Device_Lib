/*****************************************************************************************************
@File:		Button 
@Author:  khuenguyen
@website: khuenguyencreator.com
@youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
@huong dan su dung:
- Cau hinh cac chan vao Button la Input_pullup
- Khoi tao Bien luu gia tri Button:
	BUTTON_Name Button1;
- Truyen vao chan ten va chan button do
	BUTTON_Init(&Button1, Button_GPIO_Port, Button_Pin);
- Doc gia tri Button
	Status = Read_Button(&Button1);
*********************** *****************************************************************************/
#include "BUTTON.h"

// ************************** Low Level Layer *****************************************************//
static uint8_t BUTTON_ReadPin(BUTTON_Name* Button)
{
	return HAL_GPIO_ReadPin(Button->BUTTON_PORT, Button->BUTTON_Pin);
}
static void BUTTON_DelayMs(uint16_t Time)
{
	HAL_Delay(Time);
}

// ************************** High Level Layer *****************************************************//
void BUTTON_Init(BUTTON_Name* Button, GPIO_TypeDef* BUTTON_PORT, uint16_t BUTTON_Pin)
{
	Button->BUTTON_PORT = BUTTON_PORT;
	Button->BUTTON_Pin = BUTTON_Pin;
}

BUTTON_STATE BUTTON_Read(BUTTON_Name* Button)
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
	

