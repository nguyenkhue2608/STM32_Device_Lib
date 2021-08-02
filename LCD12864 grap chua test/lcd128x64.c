#include "lcd128x64.h"
#ifndef __FONT_5X7_H
#include "font5x7.h"
#else
extern PRO_DATA uint8_t FONT5X7[95][5];
#endif

void LCD128X64_Init(uint8_t mode)
{
   #ifdef LCD128X64_RST
   LCD128X64_RST     =1;
   #endif
   
   LCD128X64_E       =0;
   LCD128X64_CS1     =0;
   LCD128X64_CS2     =0;
   LCD128X64_RS      =0;                         // Set for instruction
   
   LCD128X64_SendByte(LCD128X64_LEFT,  0xC0);    // Specify first RAM line at the top
   LCD128X64_SendByte(LCD128X64_RIGHT, 0xC0);    // of the screen
   LCD128X64_SendByte(LCD128X64_LEFT,  0x40);    // Set the column address to 0
   LCD128X64_SendByte(LCD128X64_RIGHT, 0x40);
   LCD128X64_SendByte(LCD128X64_LEFT,  0xB8);    // Set the page address to 0
   LCD128X64_SendByte(LCD128X64_RIGHT, 0xB8);
   LCD128X64_SendByte(LCD128X64_LEFT,  mode);    // Turn on or off the display
   LCD128X64_SendByte(LCD128X64_RIGHT, mode);
   LCD128X64_FillScreen(LCD128X64_OFF);                    // Clear the display
}

void LCD128X64_DrawPixel(uint8_t x, uint8_t y, uint8_t color)
{
   uint8_t Data;
   uint8_t side = LCD128X64_LEFT;  // Stores which chip to use on the LCD
   uint8_t Buff;
   x=63-x;
   y=127-y;
   if(x > 63)                      // Check for first or second display area
   {
      x -= 64;
      side = LCD128X64_RIGHT;
   }

   LCD128X64_RS      =     0;                             // Set for instruction
   x = x& 0x7f;
   x = x| 0x40;
   LCD128X64_SendByte(side, x);                     // Set the horizontal address
   LCD128X64_SendByte(side, (y/8 & 0xBF) | 0xB8);   // Set the vertical page address
   
   LCD128X64_RS      =1;                             // Set for Data
   LCD128X64_ReadByte(side);                         // Need two reads to get Data
   Data = LCD128X64_ReadByte(side);                  //  at new address

   if(color == LCD128X64_ON)
        {
        Buff = 1;
        Buff= Buff<<(y%8);
        Data = Data|Buff;
        }
   else
        { 
        Buff = 1;
        Buff= Buff<<(y%8);
        Data = Data & Buff;                         // or
        }

   LCD128X64_RS      =0;          // Set for instruction
   LCD128X64_SendByte(side, x);      // Set the horizontal address
   LCD128X64_RS      =1;         // Set for Data
   LCD128X64_SendByte(side, Data);   // Write the pixel Data
}
// Purpose:    Fill the LCD screen with the passed in color
// Inputs:     ON  - turn all the pixels on
//             OFF - turn all the pixels off
void LCD128X64_FillScreen(uint8_t color)
{
   uint8_t i, j;

   // Loop through the vertical pages
   for(i = 0; i < 8; ++i)
   {
      LCD128X64_RS   =0;                             // Set for instruction
      LCD128X64_SendByte(LCD128X64_LEFT, 0x40);    // Set horizontal address to 0
      LCD128X64_SendByte(LCD128X64_RIGHT, 0x40);
      LCD128X64_SendByte(LCD128X64_LEFT, i | 0x40);// Set page address
      LCD128X64_SendByte(LCD128X64_RIGHT, i | 0x40);
      LCD128X64_RS   =1;                             // Set for Data

      // Loop through the horizontal sections
      for(j = 0; j < 64; ++j)
      {
         LCD128X64_SendByte(LCD128X64_LEFT, color);  // Turn pixels on or off
         LCD128X64_SendByte(LCD128X64_RIGHT, color); // Turn pixels on or off
      }
   }
}
void LCD128X64_SendByte(uint8_t side, uint8_t Data)
{
   if(side==1)                   // Choose which side to write to
      LCD128X64_CS2  =1;
   else
      LCD128X64_CS1  =1;

   LCD128X64_RW      =0;             // Set for writing
   //LCD128X64_PORT    = Data;         // Put the Data on the port 
	 GPIO_SetState(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7,GPIO_Mode_Out_PP);
   PORTA&=(0xff00|Data);
   
   delay_us(1);
   LCD128X64_E       =1;             // Pulse the enable pin
   delay_us(2);
   LCD128X64_E       =0;

   LCD128X64_CS1     =0;             // Reset the chip select lines
   LCD128X64_CS2     =0;
   
}
uint8_t LCD128X64_ReadByte(uint8_t side)
{
   uint8_t Data=0;          // Stores the Data read from the LCD

   //LCD128X64_DDR     =0;             // Set port LCD to input   
   GPIO_SetState(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7,GPIO_Mode_IPU);
   LCD128X64_RW      =1;             // Set for reading

   if(side==1)                  // Choose which side to write to
      LCD128X64_CS2  =1;
   else
      LCD128X64_CS1  =1;

   delay_us(1);
   LCD128X64_E       =1;             // Pulse the enable pin
   delay_us(2);
   //Data         =LCD128X64_PIN;      // Get the Data from the display's output register 
	 Data=PINA&0x00ff;
   /*Data|=LCD128X64_RS0; 
   Data|=LCD128X64_RS1<<1;
   Data|=LCD128X64_RS2<<2; 
   Data|=LCD128X64_RS3<<3;
   Data|=LCD128X64_RS4<<4; 
   Data|=LCD128X64_RS5<<5;
   Data|=LCD128X64_RS6<<6; 
   Data|=LCD128X64_RS7<<7;
   */
   LCD128X64_E       =0;

   LCD128X64_CS1     =0;             // Reset the chip select lines
   LCD128X64_CS2     =0;
   //LCD128X64_DDR     =0xff;          // Set port LCD to output 
   GPIO_SetState(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7,GPIO_Mode_Out_PP);
   return Data;                 // Return the read Data
}       
void LCD128X64_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
   int          dy, dx;
   int8_t  addx=1, addy=1;
   int16_t   P, diff;

   uint8_t i=0;
   if(x1 > x2)
        {
        dx = x1-x2;
        addx = -1;
        }
        else dx = x2-x1;
   if(y1 > y2)
        {
        dy = y1-y2;
        addy = -1;
        }
        else dy = y2-y1;

   if(dx >= dy)
   {
      dy *= 2;
      P = dy - dx;
      diff = P - dx;

      for(; i<=dx; ++i)
      {
         LCD128X64_DrawPixel(x1, y1, color);

         if(P < 0)
         {
            P  += dy;
            x1 += addx;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
   else
   {
      dx *= 2;
      P = dx - dy;
      diff = P - dy;

      for(; i<=dy; ++i)
      {
         LCD128X64_DrawPixel(x1, y1, color);

         if(P < 0)
         {
            P  += dx;
            y1 += addy;
         }
         else
         {
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
}
void LCD128X64_DrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t fill, uint8_t color)

{
   if(fill==1)
   {       
      uint8_t  i, xmin, xmax, ymin, ymax;
      if(x1 < x2)                            //  Find x min and max
      {
         xmin = x1;
         xmax = x2;
      }
      else
      {
         xmin = x2;
         xmax = x1;
      }
      if(y1 < y2)                            // Find the y min and max
      {
         ymin = y1;
         ymax = y2;
      }
      else
      {
         ymin = y2;
         ymax = y1;
      }

      for(; xmin <= xmax; ++xmin)
      {
         for(i=ymin; i<=ymax; ++i)
         {
            LCD128X64_DrawPixel(xmin, i, color);
         }
      }
   }
   else
   {
      LCD128X64_DrawLine(x1, y1, x2, y1, color);      // Draw the 4 sides
      LCD128X64_DrawLine(x1, y2, x2, y2, color);
      LCD128X64_DrawLine(x1, y1, x1, y2, color);
      LCD128X64_DrawLine(x2, y1, x2, y2, color);
   }
}
void LCD128X64_DrawCircle(uint8_t x, uint8_t y, uint8_t radius, uint8_t fill, uint8_t color)
{
   int8_t  a, b, P;
   a = 0;
   b = radius;
   P = 1 - radius;
   do
   {
      if(fill==1)
      {
         LCD128X64_DrawLine(x-a, y+b, x+a, y+b, color);
         LCD128X64_DrawLine(x-a, y-b, x+a, y-b, color);
         LCD128X64_DrawLine(x-b, y+a, x+b, y+a, color);
         LCD128X64_DrawLine(x-b, y-a, x+b, y-a, color);
      }
      else
      {
         LCD128X64_DrawPixel(a+x, b+y, color);
         LCD128X64_DrawPixel(b+x, a+y, color);
         LCD128X64_DrawPixel(x-a, b+y, color);
         LCD128X64_DrawPixel(x-b, a+y, color);
         LCD128X64_DrawPixel(b+x, y-a, color);
         LCD128X64_DrawPixel(a+x, y-b, color);
         LCD128X64_DrawPixel(x-a, y-b, color);
         LCD128X64_DrawPixel(x-b, y-a, color);
      }

      if(P < 0)
         P += 3 + 2 * a++;
      else
         P += 5 + 2 * (a++ - b--);
    } while(a <= b);
}
void LCD128X64_text57(uint8_t x, uint8_t y,char * textptr, uint8_t size, uint8_t color)
{
   uint8_t j, k, l, m;                       // Loop counters
   char pixelData[5];                     // Stores character Data 
   uint8_t Buff;
   uint8_t i;

   for(; *textptr != '\0'; ++textptr, ++x)// Loop through the passed string
   {
       for(i=0;i<5;i++)
       pixelData[i] = FONT5X7[(*textptr - ' ')][i];
      // Handles newline and carriage returns
      switch(*textptr)
      {
         case '\n':
            y += 7*size + 1;
            continue;
         case '\r':
            x = 0;
            continue;
      }

      if(x+5*size >= LCD128X64_WIDTH)          // Performs character wrapping
      {
         x = 0;                           // Set x at far left position
         y += 7*size + 1;                 // Set y at next position down
      }
      for(j=0; j<5; ++j, x+=size)         // Loop through character byte Data
      {
         for(k=0; k < 7; ++k)             // Loop through the vertical pixels
         {
         Buff = 1;
         Buff = Buff<<k;
         Buff = Buff & pixelData[j];
            if(Buff>0)                    // Check if the pixel should be set
            {
               for(l=0; l < size; ++l)    // These two loops change the
               {                          // character's size
                  for(m=0; m < size; ++m)
                  {
                     LCD128X64_DrawPixel(x+m, y+k*size+l, color); // Draws the pixel
                  }
               }
            }
         }
      }
   }
}
