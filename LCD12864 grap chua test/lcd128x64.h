#ifndef __LCD128X64_H
#define __LCD128X64_H
#include "main.h"
#define LCD128X64_TURN_ON        0x3F
#define LCD128X64_TURN_OFF       0x3E
#define LCD128X64_ON             1
#define LCD128X64_OFF            0
#define LCD128X64_FILL_ON        0xFF
#define LCD128X64_FILL_OFF       0x00
#ifndef LCD128X64_WIDTH
#define LCD128X64_WIDTH          128
#endif

#define LCD128X64_LEFT           0
#define LCD128X64_RIGHT          1
void LCD128X64_Init(uint8_t mode);
void LCD128X64_DrawPixel(uint8_t x, uint8_t y, uint8_t color);
void LCD128X64_FillScreen(uint8_t color);
void LCD128X64_SendByte(uint8_t side, uint8_t Data);
uint8_t LCD128X64_ReadByte(uint8_t side);      
void LCD128X64_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
void LCD128X64_DrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t fill, uint8_t color);
void LCD128X64_DrawCircle(uint8_t x, uint8_t y, uint8_t radius, uint8_t fill, uint8_t color);
void LCD128X64_text57(uint8_t x, uint8_t y,char * textptr, uint8_t size, uint8_t color);
#endif
