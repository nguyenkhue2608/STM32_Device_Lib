
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
#ifndef __KEYPAD_H
#define __KEYPAD_H
#include "stm32f1xx_hal.h"
#define NUMROWS 4
#define NUMCOLS 3

typedef struct
{
	uint32_t RowPins[NUMROWS];
	uint32_t ColPins[NUMCOLS];
	GPIO_TypeDef* RowPort[NUMROWS];
	GPIO_TypeDef* ColPort[NUMCOLS];
	char MAP[NUMROWS][NUMCOLS];
	char Value;
}KEYPAD_Name;

void KEYPAD3X4_Init(KEYPAD_Name* KEYPAD, char KEYMAP[NUMROWS][NUMCOLS],
										GPIO_TypeDef* COL1_PORT, uint32_t COL1_PIN, 
										GPIO_TypeDef* COL2_PORT, uint32_t COL2_PIN,
										GPIO_TypeDef* COL3_PORT, uint32_t COL3_PIN,
										GPIO_TypeDef* ROW1_PORT, uint32_t ROW1_PIN,
										GPIO_TypeDef* ROW2_PORT, uint32_t ROW2_PIN,
										GPIO_TypeDef* ROW3_PORT, uint32_t ROW3_PIN,
										GPIO_TypeDef* ROW4_PORT, uint32_t ROW4_PIN);
char KEYPAD3X4_Readkey(KEYPAD_Name* KEYPAD);
#endif
