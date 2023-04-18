/*****************************************************************************************************************
Ten Thu Vien: ST7735 LIB
Chuc Nang: Dieu khien man hinh TFT LCD 1.44, 1.8 inch
Tac Gia: Khue Nguyen Creator
Huong Dan Su dung: 
- Sua lai file Platform.h khi su dung voi thu vien khac nhau  
- Dinh Nghia cac chan CS, DC, RS la cac chan ouput
- Cau Hinh SPI, Sua lai marco de dung voi SPI can dung: #define ST7735_SPI_PORT hspi1
Pinout:

******************************************************************************************************************/
#include "ST7735_SPI.h"

//*********************** Marco for HAL LIB ***************************************************///
#ifdef USE_HAL_LIB 
extern SPI_HandleTypeDef ST7735_SPI_PORT;
/****************** Low Level Functions **********************************************************************/	
void ST7735_SPI_Trans(uint8_t *buff, uint16_t buff_size)
{
	HAL_SPI_Transmit(&ST7735_SPI_PORT, buff, buff_size, HAL_MAX_DELAY);
}
void delay_ms(uint32_t time_delay)
{
	HAL_Delay(time_delay);
}
#endif
/************************************ Middle Level Functions ************************************************/
void ST7735_Reset() 
{
    ST7735_RESET_ON();
    delay_ms(5);
    ST7735_RESET_OFF();
}			
void ST7735_WriteCommand(uint8_t cmd)
{
    ST7735_CMD();
		ST7735_SPI_Trans(&cmd, sizeof(cmd));
}
void ST7735_WriteData(uint8_t* buff, size_t buff_size) 
{
    ST7735_DATA();
    ST7735_SPI_Trans(buff, buff_size);
}

void ST7735_WriteData8(uint8_t data) 
{
    ST7735_DATA();
		uint8_t data8[1];
		data8[0] = data;
    ST7735_SPI_Trans(data8, sizeof(data));
}
void ST7735_WriteData16(uint16_t data) 
{
    ST7735_DATA();
		uint8_t data8[2];
		data8[0] = data>>8 & 0xFF;
		data8[1] = data &0xFF;
    ST7735_SPI_Trans(data8, sizeof(data8));
}

void ST7735_ExecuteCommandList(const uint8_t *addr) 
{
    uint8_t numCommands, numArgs;
    uint16_t ms;
    numCommands = *addr++;
    while(numCommands--) {
        uint8_t cmd = *addr++;
        ST7735_WriteCommand(cmd);
        numArgs = *addr++;
        // If high bit set, delay follows args
        ms = numArgs & DELAY;
        numArgs &= ~DELAY;
        if(numArgs) {
            ST7735_WriteData((uint8_t*)addr, numArgs);
            addr += numArgs;
        }
        if(ms) {
            ms = *addr++;
            if(ms == 255) ms = 500;
            delay_ms(ms);
        }
    }
}
void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    // column address set
    ST7735_WriteCommand(ST7735_CASET);
    uint8_t data[] = { 0x00, x0 + ST7735_XSTART, 0x00, x1 + ST7735_XSTART };
    ST7735_WriteData(data, sizeof(data));

    // row address set
    ST7735_WriteCommand(ST7735_RASET);
    data[1] = y0 + ST7735_YSTART;
    data[3] = y1 + ST7735_YSTART;
    ST7735_WriteData(data, sizeof(data));
    // write to RAM
    ST7735_WriteCommand(ST7735_RAMWR);
}

void ST7735_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  ST7735_WriteCommand(ST7735_CASET); 
	ST7735_WriteData8(Xpos>>8); 
	ST7735_WriteData8(Xpos&0XFF);	 
	ST7735_WriteCommand(ST7735_RASET); 
	ST7735_WriteData8(Ypos>>8); 
	ST7735_WriteData8(Ypos&0XFF);
}

void ST7735_WriteRAM_Prepare(void)
{
	ST7735_WriteCommand(ST7735_RAMWR);  
}


/************************* High level Functions ****************************************************************/
void ST7735_DisplayON(void)
{
	ST7735_Select();
	ST7735_WriteCommand(ST7735_DISPON);
	delay_ms(10);
	ST7735_Unselect();
}
void ST7735_DisplayOFF(void)
{
	ST7735_Select();
	ST7735_WriteCommand(ST7735_DISPOFF);
	delay_ms(10);
	ST7735_Unselect();
}

void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
        return;
    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x+1, y+1);
    uint8_t data[] = { color >> 8, color & 0xFF };
    ST7735_WriteData(data, sizeof(data));
    ST7735_Unselect();
}

static void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;
    ST7735_SetAddressWindow(x, y, x+font.width-1, y+font.height-1);
    for(i = 0; i < font.height; i++) {
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++) {
            if((b << j) & 0x8000)  {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ST7735_WriteData(data, sizeof(data));
            } else {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ST7735_WriteData(data, sizeof(data));
            }
        }
    }
}
void ST7735_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) {
    ST7735_Select();
    while(*str) {
        if(x + font.width >= ST7735_WIDTH) {
            x = 0;
            y += font.height;
            if(y + font.height >= ST7735_HEIGHT) {
                break;
            }
            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }
        ST7735_WriteChar(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }
    ST7735_Unselect();
}
void ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // clipping
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if((x + w - 1) >= ST7735_WIDTH) w = ST7735_WIDTH - x;
    if((y + h - 1) >= ST7735_HEIGHT) h = ST7735_HEIGHT - y;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x+w-1, y+h-1);

    uint8_t data[] = { color >> 8, color & 0xFF };
    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            ST7735_WriteData(data, sizeof(data));
        }
    }
    ST7735_Unselect();
}
void ST7735_FillScreen(uint16_t color) {
    ST7735_FillRectangle(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}
void ST7735_InvertColors(bool invert) {
    ST7735_Select();
    ST7735_WriteCommand(invert ? ST7735_INVON : ST7735_INVOFF);
    ST7735_Unselect();
}

void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* data) {
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if((x + w - 1) >= ST7735_WIDTH) return;
    if((y + h - 1) >= ST7735_HEIGHT) return;
    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x+w, y+h);
		for(int i =0; i< 2*w*h; i++)
		{
			ST7735_WriteData8(data[i]);
		}
    ST7735_Unselect();
}
void drawHline(uint16_t xStart, uint16_t yStart, uint16_t Lenght, uint16_t Color) // Chieu Ngang
{
	for(int i = xStart; i < Lenght + xStart - 1; i++)
	{
		ST7735_DrawPixel(i, yStart, Color);
	}
}	
void drawVline(uint16_t xStart, uint16_t yStart, uint16_t Lenght, uint16_t Color) // Chieu Doc
{
	for(int i = yStart; i < Lenght + yStart - 1; i++)
	{
		ST7735_DrawPixel(xStart, i, Color);
	}
}
void drawRect(uint16_t xStart, uint16_t yStart, uint16_t Width, uint16_t Height, uint16_t Color)
{
	drawVline(xStart, yStart, Height , Color);
	drawVline(xStart + Width - 1, yStart, Height, Color);
	drawHline(xStart, yStart, Width , Color);
	drawHline(xStart, yStart + Height - 1, Width , Color);

}
void ST7735_Init() 
{
    ST7735_Select();
    ST7735_Reset();
    ST7735_ExecuteCommandList(init_cmds1);
    ST7735_ExecuteCommandList(init_cmds2);
    ST7735_ExecuteCommandList(init_cmds3);
    ST7735_Unselect();
}
