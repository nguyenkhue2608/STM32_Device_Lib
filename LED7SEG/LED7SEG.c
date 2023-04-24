#include "LED7SEG.h"

char AnodeNumber[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0x8F,0x80,0x90,0x80}; //0 - 9,dp
char CathodeNumber[] = {0x3F,0x06,0x5B,0x40,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x7F}; //0 - 9,dp

void LED7SEG_Init(LED7SEG_Device_Name* LED7SEG, LED7TYPE Led7Type,
											GPIO_TypeDef* SEG_A,  uint16_t SEG_A_Pin,
											GPIO_TypeDef* SEG_B,  uint16_t SEG_B_Pin,
											GPIO_TypeDef* SEG_C,  uint16_t SEG_C_Pin,
											GPIO_TypeDef* SEG_D,  uint16_t SEG_D_Pin,
											GPIO_TypeDef* SEG_E,  uint16_t SEG_E_Pin,	
											GPIO_TypeDef* SEG_F,  uint16_t SEG_F_Pin,
											GPIO_TypeDef* SEG_G,  uint16_t SEG_G_Pin,	
											GPIO_TypeDef* SEG_H, 	uint16_t SEG_H_Pin)
{
	LED7SEG->Led7Type = Led7Type;
	LED7SEG->SEG_A = SEG_A;
	LED7SEG->SEG_B = SEG_B;
	LED7SEG->SEG_C = SEG_C;
	LED7SEG->SEG_D = SEG_D;
	LED7SEG->SEG_E = SEG_E;
	LED7SEG->SEG_F = SEG_F;
	LED7SEG->SEG_G = SEG_G;
	LED7SEG->SEG_G = SEG_H;
	LED7SEG->SEG_A_Pin = SEG_A_Pin;
	LED7SEG->SEG_B_Pin = SEG_B_Pin;
	LED7SEG->SEG_C_Pin = SEG_C_Pin;
	LED7SEG->SEG_D_Pin = SEG_D_Pin;
	LED7SEG->SEG_E_Pin = SEG_E_Pin;
	LED7SEG->SEG_F_Pin = SEG_F_Pin;
	LED7SEG->SEG_G_Pin = SEG_G_Pin;
	LED7SEG->SEG_H_Pin = SEG_H_Pin;
}
void LED7SEG_InitScan(LED7SEG_Num* LED7_NUM, GPIO_TypeDef* LED1, uint16_t LED1_pin, 
											GPIO_TypeDef* LED2, uint16_t LED2_pin,
											GPIO_TypeDef* LED3, uint16_t LED3_pin, 
											GPIO_TypeDef* LED4, uint16_t LED4_pin)
{
	LED7_NUM->LED1 = LED1;
	LED7_NUM->LED2 = LED2;
	LED7_NUM->LED3 = LED3;
	LED7_NUM->LED4 = LED4;
	LED7_NUM->LED1_pin = LED1_pin;
	LED7_NUM->LED2_pin = LED2_pin;
	LED7_NUM->LED3_pin = LED3_pin;
	LED7_NUM->LED4_pin = LED4_pin;
}
void LED7SEG_WriteOneNumber(LED7SEG_Device_Name* LED7SEG, uint8_t number)
{
	if(LED7SEG->Led7Type == ANODE)
	{
		HAL_GPIO_WritePin(LED7SEG->SEG_A, LED7SEG->SEG_A_Pin, AnodeNumber[number]&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_B, LED7SEG->SEG_B_Pin, AnodeNumber[number]&0x02?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_C, LED7SEG->SEG_C_Pin, AnodeNumber[number]&0x04?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_D, LED7SEG->SEG_D_Pin, AnodeNumber[number]&0x08?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_E, LED7SEG->SEG_E_Pin, AnodeNumber[number]&0x10?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_F, LED7SEG->SEG_F_Pin, AnodeNumber[number]&0x20?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_G, LED7SEG->SEG_G_Pin, AnodeNumber[number]&0x40?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_H, LED7SEG->SEG_H_Pin, AnodeNumber[number]&0x80?GPIO_PIN_SET:GPIO_PIN_RESET);
	}
	else if(LED7SEG->Led7Type == CATHODE)
	{
		HAL_GPIO_WritePin(LED7SEG->SEG_A, LED7SEG->SEG_A_Pin, CathodeNumber[number]&0x01?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_B, LED7SEG->SEG_B_Pin, CathodeNumber[number]&0x02?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_C, LED7SEG->SEG_C_Pin, CathodeNumber[number]&0x04?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_D, LED7SEG->SEG_D_Pin, CathodeNumber[number]&0x08?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_E, LED7SEG->SEG_E_Pin, CathodeNumber[number]&0x10?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_F, LED7SEG->SEG_F_Pin, CathodeNumber[number]&0x20?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_G, LED7SEG->SEG_G_Pin, CathodeNumber[number]&0x40?GPIO_PIN_SET:GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED7SEG->SEG_H, LED7SEG->SEG_H_Pin, CathodeNumber[number]&0x80?GPIO_PIN_SET:GPIO_PIN_RESET);
	}
}

LED7SEG_NumberInt LED7SEG_ConvertInt(uint32_t number) //12345
{
	LED7SEG_NumberInt numberInt;
	numberInt.TenOfThousands = number/10000;  //1
	numberInt.Thousands = (number%10000)/1000; // 2
	numberInt.Hundreds = ((number%10000)%1000)/100; //3
	numberInt.Dozens = (((number%10000)%1000)%100)/10; //4
	numberInt.Uints = (((number%10000)%1000)%100)%10; //5
	return numberInt;
}
void LED7SEG_Scan4Led(LED7SEG_Num* LED7_NUM, LED7SEG_Device_Name* LED7SEG, uint16_t number)
{
	LED7SEG_NumberInt number_convert;
	number_convert = LED7SEG_ConvertInt(number);
	HAL_GPIO_WritePin(LED7_NUM->LED1, LED7_NUM->LED1_pin, GPIO_PIN_SET);
	LED7SEG_WriteOneNumber(LED7SEG, number_convert.Thousands);
	
	HAL_GPIO_WritePin(LED7_NUM->LED1, LED7_NUM->LED1_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED7_NUM->LED2, LED7_NUM->LED2_pin, GPIO_PIN_SET);
	LED7SEG_WriteOneNumber(LED7SEG, number_convert.Hundreds);
	
	HAL_GPIO_WritePin(LED7_NUM->LED2, LED7_NUM->LED2_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED7_NUM->LED3, LED7_NUM->LED3_pin, GPIO_PIN_SET);
	LED7SEG_WriteOneNumber(LED7SEG, number_convert.Dozens);
	
	HAL_GPIO_WritePin(LED7_NUM->LED3, LED7_NUM->LED3_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED7_NUM->LED4, LED7_NUM->LED4_pin, GPIO_PIN_SET);
	LED7SEG_WriteOneNumber(LED7SEG, number_convert.Uints);
}	
