#ifndef __LCD_KS0108_H__
#define __LCD_KS0108_H__

#include "main.h"

#define GLCD_DATA	P0

#define GLCD_EN	P2_4
#define GLCD_RW	P2_3
#define GLCD_RS	P2_2
#define GLCD_CS2	P2_6
#define GLCD_CS1	P2_5
//#define GLCD_RST	P3_5

//instrucion code
#define GLCD_display	0x3E
#define GLCD_yaddress	0x40
#define GLCD_xaddress	0xB8
#define GLCD_statline	0xC0
#define GLCD_busy			P0_7



void GLCD_delay(void);
void GLCD_setside (char side);
void wait_GLCD(void);
void GLCD_setdisplay(unsigned char ON);
void GLCD_setyaddress(unsigned char col);
void GLCD_setxaddress(unsigned char row);
void GLCD_startline(unsigned char offset);
void GLCD_writedata(unsigned char dat);
unsigned char GLCD_readdata(void);
void GLCD_init(void);
void GLCD_gotoxy(unsigned char row, unsigned char col);
void GLCD_clear(void);
void GLCD_putchar(unsigned char row,unsigned char col, unsigned char chr);
void GLCD_puts(unsigned char row,unsigned char col, char *str);
void GLCD_putbmp(char *bmp);

#endif