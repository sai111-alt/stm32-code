#include "Key.h"

void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = k1 | k2 | k3 | k4;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0;

    if (GPIO_ReadInputDataBit(GPIOA, k1) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOA, k1) == 0)
            ;
        Delay_ms(20);
        KeyNum = 1;
    }

    if (GPIO_ReadInputDataBit(GPIOA, k2) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOA, k2) == 0)
            ;
        Delay_ms(20);
        KeyNum = 2;
    }

    if (GPIO_ReadInputDataBit(GPIOA, k3) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOA, k3) == 0)
            ;
        Delay_ms(20);
        KeyNum = 3;
    }

    if (GPIO_ReadInputDataBit(GPIOA, k4) == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOA, k4) == 0)
            ;
        Delay_ms(20);
        KeyNum = 4;
    }

    return KeyNum;
}
