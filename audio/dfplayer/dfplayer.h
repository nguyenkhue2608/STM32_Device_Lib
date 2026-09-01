/**
 * @file    dfplayer.h
 * @brief   DFPlayer Mini MP3 module, UART.
 *
 * Requires: one UART at 9600 baud.
 *
 * Usage:
 *   DFPLAYER_Device mp3;
 *   DFPLAYER_Init(&mp3, &huart1);
 *   DFPLAYER_SetVolume(&mp3, 20);
 *   DFPLAYER_PlayTrack(&mp3, 1);
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#ifndef __DFPLAYER_H
#define __DFPLAYER_H
#include "stm32f1xx_hal.h"

typedef struct
{
	UART_HandleTypeDef* DFP_UART;
	uint8_t SendBuff[10];
	uint16_t Checksum;
}DFPLAYER_Device;

#define DFP_PLAYTRACK 				0X12
#define DFP_NEXT 							0X01
#define DFP_PREV		  				0X02
#define DFP_SETVOLUME 				0X06
#define DFP_PLAY		 				 	0X0D
#define DFP_PAUSE		 				  0X0E
#define DFP_STOP					  	0X16
#define DFP_RANDOM 						0X18
#define DFP_PLAYFILEINFOLDER  0X0F

void DFPLAYER_Init(DFPLAYER_Device* MP3, UART_HandleTypeDef* UART);
void DFPLAYER_PlayTrack(DFPLAYER_Device* MP3, uint16_t num);
void DFPLAYER_Next(DFPLAYER_Device* MP3);
void DFPLAYER_Prev(DFPLAYER_Device* MP3);
void DFPLAYER_SetVolume(DFPLAYER_Device* MP3, uint16_t volume);
void DFPLAYER_Play(DFPLAYER_Device* MP3);
void DFPLAYER_Pause(DFPLAYER_Device* MP3);
void DFPLAYER_Stop(DFPLAYER_Device* MP3);
void DFPLAYER_RandomPlay(DFPLAYER_Device* MP3);
void DFPLAYER_PlayFileInFolder(DFPLAYER_Device* MP3, uint8_t folder, uint32_t num);
#endif
