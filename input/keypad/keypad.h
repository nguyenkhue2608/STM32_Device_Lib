/**
 * @file    keypad.h
 * @brief   Matrix keypad (3x4 / 4x4) scanner.
 *
 * Requires: column pins as output, row pins as input pull-up.
 *          NUMROWS / NUMCOLS are compile-time constants in this header.
 *
 * Usage:
 *   KEYPAD_Device kp;
 *   char map[NUMROWS][NUMCOLS] = { {'1','2','3'}, {'4','5','6'},
 *                                  {'7','8','9'}, {'*','0','#'} };
 *   KEYPAD_Init(&kp, map, R0_Port,R0_Pin, ...3 more rows..., C0_Port,C0_Pin, C1..., C2...);
 *   char k = KEYPAD_Readkey(&kp);   // 0 when nothing is pressed
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
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
}KEYPAD_Device;

void KEYPAD_Init(KEYPAD_Device* KEYPAD, char KEYMAP[NUMROWS][NUMCOLS],
										GPIO_TypeDef* COL1_PORT, uint32_t COL1_PIN, 
										GPIO_TypeDef* COL2_PORT, uint32_t COL2_PIN,
										GPIO_TypeDef* COL3_PORT, uint32_t COL3_PIN,
										GPIO_TypeDef* ROW1_PORT, uint32_t ROW1_PIN,
										GPIO_TypeDef* ROW2_PORT, uint32_t ROW2_PIN,
										GPIO_TypeDef* ROW3_PORT, uint32_t ROW3_PIN,
										GPIO_TypeDef* ROW4_PORT, uint32_t ROW4_PIN);
char KEYPAD_Readkey(KEYPAD_Device* KEYPAD);
#endif
