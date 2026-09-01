/**
 * @file    st7789.c
 * @brief   ST7789 IPS TFT LCD (240x240 / 240x320), SPI - implementation.
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#include "st7789.h"


/********************************* BSP Funtions **********************************************************************/
#if defined(STM32_HAL)

static void ST7789_SPI_Transfer(ST7789_Config *ST7789, uint8_t *buff, uint16_t buff_size) {
    HAL_SPI_Transmit(ST7789->pins.SPI_Port, buff, buff_size, HAL_MAX_DELAY);
}

static void ST7789_delay_ms(uint32_t time_delay) {
    HAL_Delay(time_delay);
}

static void ST7789_Reset_On(ST7789_Config *ST7789) {
    HAL_GPIO_WritePin(ST7789->pins.RST_GPIO_Port, ST7789->pins.RST_Pin, GPIO_PIN_RESET);
}

static void ST7789_Reset_Off(ST7789_Config *ST7789) {
    HAL_GPIO_WritePin(ST7789->pins.RST_GPIO_Port, ST7789->pins.RST_Pin, GPIO_PIN_SET);
}

static void ST7789_Select(ST7789_Config *ST7789) {
    HAL_GPIO_WritePin(ST7789->pins.CS_GPIO_Port, ST7789->pins.CS_Pin, GPIO_PIN_RESET);
}

static void ST7789_Unselect(ST7789_Config *ST7789) {
    HAL_GPIO_WritePin(ST7789->pins.CS_GPIO_Port, ST7789->pins.CS_Pin, GPIO_PIN_SET);
}

static void ST7789_CmdMode(ST7789_Config *ST7789) {
    HAL_GPIO_WritePin(ST7789->pins.DC_GPIO_Port, ST7789->pins.DC_Pin, GPIO_PIN_RESET);
}

static void ST7789_DataMode(ST7789_Config *ST7789) {
    HAL_GPIO_WritePin(ST7789->pins.DC_GPIO_Port, ST7789->pins.DC_Pin, GPIO_PIN_SET);
}

#endif

/************************************ Basic control Funtions ************************************************/
void ST7789_Reset(ST7789_Config *ST7789) {
    ST7789_Reset_On(ST7789);     // Đặt chân Reset xuống mức thấp
    ST7789_delay_ms(5);          // Đợi 5ms
    ST7789_Reset_Off(ST7789);    // Đặt chân Reset lên mức cao
}

void ST7789_WriteCommand(ST7789_Config *ST7789, uint8_t cmd) {
	ST7789_CmdMode(ST7789);      // Chuyển sang chế độ lệnh
    ST7789_Select(ST7789);       // Chọn màn hình
    ST7789_SPI_Transfer(ST7789, &cmd, sizeof(cmd));  // Gửi lệnh qua SPI
    ST7789_Unselect(ST7789);     // Bỏ chọn màn hình
}

void ST7789_WriteData(ST7789_Config *ST7789, uint8_t *buff, size_t buff_size) {
    ST7789_DataMode(ST7789);     // Chuyển sang chế độ dữ liệu
    ST7789_Select(ST7789);       // Chọn màn hình
    ST7789_SPI_Transfer(ST7789, buff, buff_size);    // Gửi dữ liệu qua SPI
    ST7789_Unselect(ST7789);     // Bỏ chọn màn hình
}


void ST7789_WriteData8(ST7789_Config *ST7789, uint8_t data) {
    ST7789_DataMode(ST7789);     // Chuyển sang chế độ dữ liệu
    ST7789_Select(ST7789);       // Chọn màn hình
    ST7789_SPI_Transfer(ST7789, &data, sizeof(data)); // Gửi dữ liệu 8-bit
    ST7789_Unselect(ST7789);     // Bỏ chọn màn hình
}

void ST7789_WriteData16(ST7789_Config *ST7789, uint16_t data) {
    uint8_t data8[2];
    data8[0] = (data >> 8) & 0xFF;
    data8[1] = data & 0xFF;
    ST7789_DataMode(ST7789);     // Chuyển sang chế độ dữ liệu
    ST7789_Select(ST7789);       // Chọn màn hình
    ST7789_SPI_Transfer(ST7789, data8, sizeof(data8)); // Gửi dữ liệu 16-bit
    ST7789_Unselect(ST7789);     // Bỏ chọn màn hình
}

void ST7789_SetAddressWindow(ST7789_Config *ST7789, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // Thiết lập vùng địa chỉ cột
    uint8_t data_col[] = {
        (x0 + ST7789->xstart) >> 8, (x0 + ST7789->xstart) & 0xFF,  // XSTART
        (x1 + ST7789->xstart) >> 8, (x1 + ST7789->xstart) & 0xFF   // XEND
    };

    ST7789_WriteCommand(ST7789, ST7789_CASET);   // Lệnh thiết lập cột
    ST7789_WriteData(ST7789, data_col, sizeof(data_col));

    // Thiết lập vùng địa chỉ hàng
    uint8_t data_row[] = {
        (y0 + ST7789->ystart) >> 8, (y0 + ST7789->ystart) & 0xFF,  // YSTART
        (y1 + ST7789->ystart) >> 8, (y1 + ST7789->ystart) & 0xFF   // YEND
    };

    ST7789_WriteCommand(ST7789, ST7789_RASET);   // Lệnh thiết lập hàng
    ST7789_WriteData(ST7789, data_row, sizeof(data_row));

    // Chuẩn bị ghi vào RAM
    ST7789_WriteCommand(ST7789, ST7789_RAMWR);   // Lệnh ghi vào RAM
}

void ST7789_SetCursor(ST7789_Config *ST7789, uint16_t Xpos, uint16_t Ypos) {
    ST7789_WriteCommand(ST7789, ST7789_CASET);     // Thiết lập cột
    ST7789_WriteData8(ST7789, Xpos >> 8);          // Gửi byte cao
    ST7789_WriteData8(ST7789, Xpos & 0xFF);        // Gửi byte thấp
    ST7789_WriteCommand(ST7789, ST7789_RASET);     // Thiết lập hàng
    ST7789_WriteData8(ST7789, Ypos >> 8);          // Gửi byte cao
    ST7789_WriteData8(ST7789, Ypos & 0xFF);        // Gửi byte thấp
}

void ST7789_WriteRAMPrepare(ST7789_Config *ST7789) {
    ST7789_WriteCommand(ST7789, ST7789_RAMWR);   // Bắt đầu ghi vào RAM
}

/************************* Configuration and Initialization funtions ****************************************************************/
void ST7789_PinConfig(ST7789_Config *ST7789,
                      SPI_HandleTypeDef* spi_port,
                      GPIO_TypeDef* rst_gpio_port, uint16_t rst_pin,
                      GPIO_TypeDef* cs_gpio_port, uint16_t cs_pin,
                      GPIO_TypeDef* dc_gpio_port, uint16_t dc_pin)
{
    ST7789->pins.SPI_Port = spi_port;
    ST7789->pins.RST_GPIO_Port = rst_gpio_port;
    ST7789->pins.RST_Pin = rst_pin;
    ST7789->pins.CS_GPIO_Port = cs_gpio_port;
    ST7789->pins.CS_Pin = cs_pin;
    ST7789->pins.DC_GPIO_Port = dc_gpio_port;
    ST7789->pins.DC_Pin = dc_pin;
}

ST7789_Status ST7789_Init(ST7789_Config *ST7789)
{
	if (ST7789 == NULL) {
		return ST7789_ERROR_NULL_POINTER;
	}

	if (ST7789->pins.CS_GPIO_Port == NULL || ST7789->pins.RST_GPIO_Port == NULL || ST7789->pins.DC_GPIO_Port == NULL) {
		return ST7789_ERROR_INVALID_GPIO_PORT;
	}

	if (HAL_SPI_GetState(ST7789->pins.SPI_Port) == HAL_SPI_STATE_RESET) {
		return ST7789_ERROR_SPI_INIT_FAILED;
	}

	if (ST7789->width == 0) {
		ST7789->width = ST7789_WIDTH_240;
	}

	if (ST7789->height == 0) {
		ST7789->height = ST7789_HEIGHT_320;
	}

	ST7789_Reset_On(ST7789);
	ST7789_delay_ms(50);  // Thời gian giữ reset
	ST7789_Reset_Off(ST7789);
	ST7789_delay_ms(150); // Thời gian sau reset

	// 1. Reset màn hình
	ST7789_WriteCommand(ST7789, ST7789_SWRESET); // Gửi lệnh reset
	ST7789_delay_ms(150);                        // Đợi 150 ms

	// 2. Ra khỏi chế độ ngủ
	ST7789_WriteCommand(ST7789, ST7789_SLPOUT);  // Gửi lệnh ra khỏi chế độ ngủ
	ST7789_delay_ms(10);                         // Đợi 10 ms

	// 3. Thiết lập chế độ màu
	ST7789_WriteCommand(ST7789, ST7789_COLMOD);  // Gửi lệnh thiết lập chế độ màu
	ST7789_WriteData8(ST7789, 0x55);             // Gửi dữ liệu: 16-bit color
	ST7789_delay_ms(10);                         // Đợi 10 ms

	// 4. Thiết lập điều khiển truy cập bộ nhớ
	ST7789_WriteCommand(ST7789, ST7789_MADCTL);  // Gửi lệnh điều khiển truy cập bộ nhớ
	ST7789_WriteData8(ST7789, 0x00);             // Gửi dữ liệu: Row/col addr, bottom-top refresh

	// 5. Thiết lập địa chỉ cột
	ST7789_WriteCommand(ST7789, ST7789_CASET);   // Gửi lệnh thiết lập địa chỉ cột
	ST7789_WriteData8(ST7789, 0x00);             // XSTART = 0
	ST7789_WriteData8(ST7789, 0x00);             // XSTART = 0
	ST7789_WriteData8(ST7789, 0x00);             // XEND = 240 (0xF0)
	ST7789_WriteData8(ST7789, 0xF0);             // Gửi byte thấp cho XEND

	// 6. Thiết lập địa chỉ hàng
	ST7789_WriteCommand(ST7789, ST7789_RASET);   // Gửi lệnh thiết lập địa chỉ hàng
	ST7789_WriteData8(ST7789, 0x00);             // YSTART = 0
	ST7789_WriteData8(ST7789, 0x00);             // YSTART = 0
	ST7789_WriteData8(ST7789, 0x01);             // YEND = 320 (0x140)
	ST7789_WriteData8(ST7789, 0x40);             // Gửi byte thấp cho YEND

	// 7. Kích hoạt hiển thị
	ST7789_WriteCommand(ST7789, ST7789_INVOFF);    // Gửi lệnh hiển thị không đảo ngược màu
	ST7789_delay_ms(10);                         // Đợi 10 ms

	// 8. Bật hiển thị
	ST7789_WriteCommand(ST7789, ST7789_NORON);    // Gửi lệnh bật hiển thị
	ST7789_delay_ms(10);                         // Đợi 10 ms

	// 9. Bật màn hình
	ST7789_WriteCommand(ST7789, ST7789_DISPON);   // Gửi lệnh bật màn hình
	ST7789_delay_ms(10);                         // Đợi 10 ms

	// 10. Xoay màn hình theo hướng mặc định
    ST7789_SetRotation(ST7789, ST7789_ROTATION_DEFAULT);
    return ST7789_OK;
}

void ST7789_SetRotation(ST7789_Config *ST7789, uint8_t rotation)
{
    uint8_t madctl = 0;

    // Lưu rotation vào cấu trúc và chỉ lấy 2 bit cuối (0-3)
    ST7789->rotation = rotation % 4;

    switch (ST7789->rotation) {
    case 0:
        // Không xoay (mặc định): Top to Bottom, Left to Right, RGB
        madctl = ST7789_MADCTL_MY | ST7789_MADCTL_MX | ST7789_MADCTL_RGB;
        ST7789->xstart = 0;
        ST7789->ystart = 0;
        ST7789->width = ST7789_WIDTH_240;
        ST7789->height = ST7789_HEIGHT_320;
        break;

    case 1:
        // Xoay 90 độ: Bottom to Top, Left to Right, RGB
        madctl = ST7789_MADCTL_MV | ST7789_MADCTL_MY | ST7789_MADCTL_RGB;
        ST7789->xstart = 0;
        ST7789->ystart = 0;
        ST7789->width = ST7789_HEIGHT_320;
        ST7789->height = ST7789_WIDTH_240;
        break;

    case 2:
        // Xoay 180 độ: Bottom to Top, Right to Left, RGB
        madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB;
        ST7789->xstart = 0;
        ST7789->ystart = 0;
        ST7789->width = ST7789_WIDTH_240;
        ST7789->height = ST7789_HEIGHT_320;
        break;

    case 3:
        // Xoay 270 độ: Top to Bottom, Right to Left, RGB
        madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
        ST7789->xstart = 0;
        ST7789->ystart = 0;
        ST7789->width = ST7789_HEIGHT_320;
        ST7789->height = ST7789_WIDTH_240;
        break;
    }

    // Gửi lệnh MADCTL với giá trị đã thiết lập đến ST7789
    ST7789_WriteCommand(ST7789, ST7789_MADCTL);
    ST7789_WriteData8(ST7789, madctl);
}
/*************************  Basic Graphic funtions *********************************************************/

void ST7789_FillScreen(ST7789_Config *ST7789, uint16_t color) {
    // Kiểm tra lại width và height trước khi fill
    if (ST7789->width == 0 || ST7789->height == 0) {
        return; // Kết thúc nếu width hoặc height chưa khởi tạo đúng
    }

    // Thiết lập vùng địa chỉ để điền toàn bộ màn hình
    ST7789_SetAddressWindow(ST7789, 0, 0, ST7789->width - 1, ST7789->height - 1);

    // Tạo buffer với kích thước 16-bit (2 bytes) cho màu
    uint8_t data[2];
    data[0] = (color >> 8) & 0xFF;  // Byte cao
    data[1] = color & 0xFF;         // Byte thấp

    // Ghi vào RAM
    ST7789_WriteRAMPrepare(ST7789); // Bắt đầu ghi vào RAM

    // Tổng số pixel của màn hình
    uint32_t totalPixels = ST7789->width * ST7789->height;

    // Gửi màu cho toàn bộ màn hình (tổng số pixel = width * height)
    for (uint32_t i = 0; i < totalPixels; i++) {
        ST7789_WriteData(ST7789, data, sizeof(data)); // Gửi dữ liệu màu
    }
}

void ST7789_DrawImage(ST7789_Config *ST7789, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* image) {
    // Kiểm tra kích thước hình ảnh có hợp lệ trong phạm vi màn hình không
    if ((x >= ST7789->width) || (y >= ST7789->height)) {
        return; // Kết thúc nếu tọa độ bắt đầu nằm ngoài màn hình
    }

    // Nếu hình ảnh vượt quá màn hình, giới hạn lại kích thước
    if ((x + w - 1) >= ST7789->width) {
        w = ST7789->width - x;
    }
    if ((y + h - 1) >= ST7789->height) {
        h = ST7789->height - y;
    }

    // Thiết lập vùng địa chỉ cho hình ảnh
    ST7789_SetAddressWindow(ST7789, x, y, x + w - 1, y + h - 1);

    // Bắt đầu ghi dữ liệu vào RAM
    ST7789_WriteRAMPrepare(ST7789);

    // Mỗi pixel ảnh là 16-bit (2 byte)
    uint8_t data[2];

    // Duyệt qua từng pixel trong hình ảnh
    for (uint32_t i = 0; i < w * h; i++) {
        // Lấy giá trị màu từ mảng ảnh
        uint16_t color = image[i];

        // Chia thành 2 byte (MSB và LSB) cho màu
        data[0] = (color >> 8) & 0xFF; // Byte cao
        data[1] = color & 0xFF;        // Byte thấp

        // Gửi dữ liệu màu cho từng pixel
        ST7789_WriteData(ST7789, data, sizeof(data));
    }
}

void ST7789_DrawPixel(ST7789_Config *ST7789, uint16_t x, uint16_t y, uint16_t color) {
    // Kiểm tra nếu pixel nằm ngoài giới hạn của màn hình
    if ((x >= ST7789->width) || (y >= ST7789->height)) {
        return; // Không làm gì nếu pixel ngoài phạm vi
    }
    // Thiết lập địa chỉ của pixel cần vẽ
    ST7789_SetAddressWindow(ST7789, x, y, x, y); // Thiết lập cửa sổ là chính pixel đó

    // Gửi dữ liệu màu (16-bit)
    ST7789_WriteData16(ST7789, color);
}

// Hàm vẽ một ký tự lên màn hình tại vị trí (x, y), sử dụng dữ liệu từ font được truyền vào
void ST7789_DrawChar(ST7789_Config *ST7789, uint16_t x, uint16_t y, char c, uint16_t textColor, uint16_t bgColor, const uint16_t *font) {
    if (c < 32 || c > 127) return; // Ký tự ngoài phạm vi có thể không hiển thị

    // Tính toán vị trí bitmap cho ký tự
    const uint16_t *bitmap = &font[(c - 32) * 10]; // Mỗi ký tự chiếm 10 dòng

    // Thiết lập cửa sổ vẽ cho ký tự 7x10 pixel
    ST7789_SetAddressWindow(ST7789, x, y, x + 6, y + 9); // 7 pixel width, 10 pixel height
    ST7789_WriteRAMPrepare(ST7789);

    for (int i = 0; i < 10; i++) {  // Duyệt qua từng dòng pixel của ký tự (10 dòng)
        uint16_t line = bitmap[i];  // Mỗi dòng là một hàng 16 bit

        for (int j = 0; j < 7; j++) {  // Duyệt qua từng pixel trong dòng (7 pixel mỗi dòng)
            if (line & (1 << (6 - j))) { // Kiểm tra bit từ trái sang phải
                ST7789_WriteData16(ST7789, textColor);  // Vẽ pixel màu chữ
            } else {
                ST7789_WriteData16(ST7789, bgColor);  // Vẽ pixel màu nền
            }
        }
    }
}




