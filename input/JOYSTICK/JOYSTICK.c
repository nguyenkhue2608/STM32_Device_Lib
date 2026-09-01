
#include "JOYSTICK.h"
static void ADC_GetValue(JOYSTICK_Name* JT)
{
	HAL_ADC_Start_DMA(JT->ADC, (uint32_t*)JT->ADCvalue, 2);
}
static void JOYSTICK_DelayMs(uint16_t Time)
{
	HAL_Delay(Time);
}
void JOYSTICK_Init(JOYSTICK_Name* JT, ADC_HandleTypeDef* ADC_In, uint32_t Xchannel, uint32_t Ychannel, 
										GPIO_TypeDef* Button_Port, uint16_t Button_Pin)
{
	JT->ADC = ADC_In;
	JT->Xchannel = Xchannel;
	JT->Ychannel = Ychannel;
	JT->Button_Port = Button_Port;
	JT->Button_Pin = Button_Pin;
}
void JOYSTICK_GetValue(JOYSTICK_Name* JT)
{
	ADC_GetValue(JT);
	JOYSTICK_DelayMs(20);
	if((JT->ADCvalue[0] > 1548) && (JT->ADCvalue[0] < 2548))
	{
		JT->Xdirect = No_Action;
	}
	if((JT->ADCvalue[1] > 1548) && (JT->ADCvalue[1] < 2548))
	{
		JT->Ydirect = No_Action;
	}
	if(JT->ADCvalue[0] <= 1548)
	{
		JT->Xdirect = X_Dec;
	}
	if(JT->ADCvalue[1] <= 1548)
	{
		JT->Ydirect = Y_Dec;
	}
	if(JT->ADCvalue[0] >= 2548)
	{
		JT->Xdirect = X_Inc;
	}
	if(JT->ADCvalue[1] >= 2548)
	{
		JT->Ydirect = Y_Inc;
	}
}
JOYSTICK_Value JOYSTICK_GetButton(JOYSTICK_Name* JT)
{
	//JT->ButtonState = noPress;
	if(!HAL_GPIO_ReadPin(JT->Button_Port, JT->Button_Pin))
	{
		while(!HAL_GPIO_ReadPin(JT->Button_Port, JT->Button_Pin)){}
		JT->ButtonState = isPress;
	}
	return JT->ButtonState;
}
