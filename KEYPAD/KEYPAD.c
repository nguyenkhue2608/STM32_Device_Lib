/******************************************************************************************************************
@File:  	Key Pad Module 3x4 4x4
@Author:  Khue Nguyen
@Website: khuenguyencreator.com
@Youtube: https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
Huong dan su dung:
- Su dung thu vien HAL
- Khoi tao cac chan Col Ouput, Cac chan Row là Input_Pull_UP
- Khoi tao bien luu Keypad va keymap
KEYPAD_Name KeyPad;
char KEYMAP[NUMROWS][NUMCOLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
- Khoi tao Keypad do:
	KEYPAD3X4_Init(&KeyPad, KEYMAP, GPIOA, GPIO_PIN_4, GPIOA, GPIO_PIN_5, GPIOA, GPIO_PIN_6, 
																	GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1,
																	GPIOA, GPIO_PIN_2, GPIOA, GPIO_PIN_3);
- Su dung cac ham phai truyen dia chi cua Keypad do: 
	Key = KEYPAD3X4_Readkey(&KeyPad);
- Pinmap Keypad 3x4: COL1 COL2 COL3 ROW1 ROW2 ROW3 ROW4
											3		  1		5		 2		7		 6		4
******************************************************************************************************************/
#include "KEYPAD.h"
static void KEYPAD_Delay(uint16_t Time)
{
	HAL_Delay(Time);
}
void KEYPAD3X4_Init(KEYPAD_Name* KEYPAD, char KEYMAP[NUMROWS][NUMCOLS],
										GPIO_TypeDef* COL1_PORT, uint32_t COL1_PIN, 
										GPIO_TypeDef* COL2_PORT, uint32_t COL2_PIN,
										GPIO_TypeDef* COL3_PORT, uint32_t COL3_PIN,
										GPIO_TypeDef* ROW1_PORT, uint32_t ROW1_PIN,
										GPIO_TypeDef* ROW2_PORT, uint32_t ROW2_PIN,
										GPIO_TypeDef* ROW3_PORT, uint32_t ROW3_PIN,
										GPIO_TypeDef* ROW4_PORT, uint32_t ROW4_PIN)
{
	KEYPAD->ColPort[0] = COL1_PORT; //Copy gia tri vao keypad
	KEYPAD->ColPort[1] = COL2_PORT;
	KEYPAD->ColPort[2] = COL3_PORT;
	KEYPAD->ColPins[0] = COL1_PIN;
	KEYPAD->ColPins[1] = COL2_PIN;
	KEYPAD->ColPins[2] = COL3_PIN;
	
	KEYPAD->RowPort[0] = ROW1_PORT;
	KEYPAD->RowPort[1] = ROW2_PORT;
	KEYPAD->RowPort[2] = ROW3_PORT;
	KEYPAD->RowPort[3] = ROW4_PORT;
	KEYPAD->RowPins[0] = ROW1_PIN;
	KEYPAD->RowPins[1] = ROW2_PIN;
	KEYPAD->RowPins[2] = ROW3_PIN;
	KEYPAD->RowPins[3] = ROW4_PIN;
	
	for(int colum = 0; colum < NUMCOLS; colum++)
	{
		for(int row = 0; row < NUMROWS; row++)
		{
			KEYPAD->MAP[row][colum] = KEYMAP[row][colum];
		}
	}
	
	HAL_GPIO_WritePin(KEYPAD->ColPort[0],KEYPAD->ColPins[0],GPIO_PIN_SET);
	HAL_GPIO_WritePin(KEYPAD->ColPort[1],KEYPAD->ColPins[1],GPIO_PIN_SET);
	HAL_GPIO_WritePin(KEYPAD->ColPort[2],KEYPAD->ColPins[2],GPIO_PIN_SET);
}
char KEYPAD3X4_Readkey(KEYPAD_Name* KEYPAD) // Scan Colums
{
	KEYPAD->Value = 0;
	for(int colum = 0; colum < NUMCOLS; colum++)
	{
		HAL_GPIO_WritePin(KEYPAD->ColPort[colum],KEYPAD->ColPins[colum],GPIO_PIN_RESET);
		for(int row = 0; row < NUMROWS; row++)
		{
			if(HAL_GPIO_ReadPin(KEYPAD->RowPort[row],KEYPAD->RowPins[row]) == 0)
			{
				KEYPAD_Delay(50);// debound
				while(HAL_GPIO_ReadPin(KEYPAD->RowPort[row],KEYPAD->RowPins[row])==0){}
				KEYPAD->Value = KEYPAD->MAP[row][colum];
					
				return KEYPAD->Value;
			}
		}
		HAL_GPIO_WritePin(KEYPAD->ColPort[colum],KEYPAD->ColPins[colum],GPIO_PIN_SET);
	}
	
	return 0;
}

void KEYPAD3x4_Config(KEYPAD_Name* KEYPAD, char KEYMAP_Config[NUMROWS][NUMCOLS])
{
	for(int colum = 0; colum < NUMCOLS; colum++)
	{
		for(int row = 0; row < NUMROWS; row++)
		{
			KEYPAD->MAP[row][colum] = KEYMAP_Config[row][colum];
		}
	}
}
