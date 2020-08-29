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
#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f1xx_hal.h"

#define DEBOUND_TIME 50
#define SINGLE_CLICK_TIME 1000
#define DOUBLE_CLICK_TIME 300

typedef enum
{
	NO_CLICK 		 = 0x00,
  SINGLE_CLICK = 0x01,
	DOUBLE_CLICK = 0x02,
	LONGCLICK_1S = 0x03
}BUTTON_STATE;

typedef struct {
	GPIO_TypeDef* BUTTON_PORT;
	uint16_t BUTTON_Pin;
	BUTTON_STATE State;
	uint8_t  isPress;
	uint16_t timePress;
	uint16_t timeDouble;
}BUTTON_Name;

void BUTTON_Init(BUTTON_Name* Button, GPIO_TypeDef* BUTTON_PORT, uint16_t BUTTON_Pin);
BUTTON_STATE BUTTON_Read(BUTTON_Name* Button);

#endif

