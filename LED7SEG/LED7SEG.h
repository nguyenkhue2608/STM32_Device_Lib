#ifndef __LED7SEG_H
#define __LED7SEG_H

#include "stm32f1xx_hal.h"

typedef enum
{
  ANODE = 0u,
  CATHODE
} LED7TYPE;


typedef struct {  
	LED7TYPE Led7Type;
	GPIO_TypeDef* SEG_A;  
	uint16_t SEG_A_Pin;
	GPIO_TypeDef* SEG_B;  
	uint16_t SEG_B_Pin;
	GPIO_TypeDef* SEG_C;  
	uint16_t SEG_C_Pin;
	GPIO_TypeDef* SEG_D;  
	uint16_t SEG_D_Pin;
	GPIO_TypeDef* SEG_E; 
	uint16_t SEG_E_Pin;	
	GPIO_TypeDef* SEG_F;  
	uint16_t SEG_F_Pin;
	GPIO_TypeDef* SEG_G; 
	uint16_t SEG_G_Pin;	
	GPIO_TypeDef* SEG_H;  
	uint16_t SEG_H_Pin;
}LED7SEG_Device_Name;

typedef struct {  
	GPIO_TypeDef* LED1;  
	uint16_t LED1_pin;
	GPIO_TypeDef* LED2;  
	uint16_t LED2_pin;
	GPIO_TypeDef* LED3;  
	uint16_t LED3_pin;
	GPIO_TypeDef* LED4; 
	uint16_t LED4_pin;	
}LED7SEG_Num;

typedef struct {  
	uint8_t TenOfThousands;
	uint8_t Thousands;
	uint8_t Hundreds;
	uint8_t Dozens;
	uint8_t Uints;
}LED7SEG_NumberInt;

void LED7SEG_Init(LED7SEG_Device_Name* LED7SEG, LED7TYPE Led7Type,
											GPIO_TypeDef* SEG_A,  uint16_t SEG_A_Pin,
											GPIO_TypeDef* SEG_B,  uint16_t SEG_B_Pin,
											GPIO_TypeDef* SEG_C,  uint16_t SEG_C_Pin,
											GPIO_TypeDef* SEG_D,  uint16_t SEG_D_Pin,
											GPIO_TypeDef* SEG_E,  uint16_t SEG_E_Pin,	
											GPIO_TypeDef* SEG_F,  uint16_t SEG_F_Pin,
											GPIO_TypeDef* SEG_G,  uint16_t SEG_G_Pin,	
											GPIO_TypeDef* SEG_H, 	uint16_t SEG_H_Pin);
void LED7SEG_InitScan(LED7SEG_Num* LED7_NUM, GPIO_TypeDef* LED1, uint16_t LED1_pin, 
											GPIO_TypeDef* LED2, uint16_t LED2_pin,
											GPIO_TypeDef* LED3, uint16_t LED3_pin, 
											GPIO_TypeDef* LED4, uint16_t LED4_pin);
void LED7SEG_WriteOneNumber(LED7SEG_Device_Name* LED7SEG, uint8_t number);
LED7SEG_NumberInt LED7SEG_ConvertInt(uint32_t number);
void LED7SEG_Scan4Led(LED7SEG_Num* LED7_NUM, LED7SEG_Device_Name* LED7SEG, uint16_t number);
#endif
