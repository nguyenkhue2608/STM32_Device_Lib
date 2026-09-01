/**
 * @file    led7_6pin.c
 * @brief   6-pin multiplexed 7-segment LED display - implementation.
 *
 * @author  Khue Nguyen
 * @website khuenguyencreator.com
 * @youtube https://www.youtube.com/channel/UCt8cFnPOaHrQXWmVkk-lfvg
 */
#include "led7_6pin.h"

LED7_6PIN_Config ledConfig[3][8] = {
    { {CATHODE1, CATHODE2}, {CATHODE1, CATHODE3}, {CATHODE1, CATHODE3}, {CATHODE1, CATHODE3},
      {CATHODE1, CATHODE4}, {CATHODE1, CATHODE5}, {CATHODE1, CATHODE6}, {CATHODE2, CATHODE4} },

    { {CATHODE2, CATHODE1}, {CATHODE2, CATHODE2}, {CATHODE2, CATHODE4}, {CATHODE2, CATHODE5},
      {CATHODE2, CATHODE5}, {CATHODE2, CATHODE6}, {CATHODE2, CATHODE6}, {CATHODE2, CATHODE1} },

    { {CATHODE3, CATHODE2}, {CATHODE3, CATHODE2}, {CATHODE3, CATHODE3}, {CATHODE3, CATHODE3},
      {CATHODE3, CATHODE5}, {CATHODE1, CATHODE6}, {CATHODE3, CATHODE1}, {CATHODE3, CATHODE4} },
};

const uint8_t digitPatterns[10][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0},
    {1, 1, 0, 1, 1, 0, 1, 0},
    {1, 1, 1, 1, 0, 0, 1, 0},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {1, 0, 1, 1, 0, 1, 1, 0},
    {1, 0, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 0, 1, 1, 0}
};

static uint8_t currentDigit = 0;
static uint32_t tickCounter = 0;

void LED7_6PIN_Init(GPIO_TypeDef* GPIOx) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = CATHODE1 | CATHODE2 | CATHODE3 | CATHODE4 | CATHODE5 | CATHODE6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void LED7_6PIN_Scan(void) {
    HAL_GPIO_WritePin(GPIOA, CATHODE1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, CATHODE2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, CATHODE3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, ledConfig[currentDigit][0].cathode, GPIO_PIN_SET);
    for (uint8_t led = 0; led < 7; led++) {
        if (digitPatterns[0][led] == 1) {
            HAL_GPIO_WritePin(GPIOA, ledConfig[currentDigit][led].anode, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(GPIOA, ledConfig[currentDigit][led].anode, GPIO_PIN_RESET);
        }
    }
    currentDigit = (currentDigit + 1) % 3;
}

void LED7_6PIN_Scan_Handler(void) {
    tickCounter++;
    if (tickCounter >= 100) {
        LED7_6PIN_Scan();
        tickCounter = 0;
    }
}
