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

uint8_t Key_GetNum(uint8_t *KeyFlag)
{
    if ((Key1 == 0 || Key2 == 0 || Key3 == 0 || Key4 == 0) && (*KeyFlag))
    {
        Delay_ms(10); // 消抖
        (*KeyFlag) = 0;
        if (Key1 == 0)
        {
            return 1;
        }
        else if (Key2 == 0)
        {
            return 2;
        }
        else if (Key3 == 0)
        {
            return 3;
        }
        else if (Key4 == 0)
        {
            return 4;
        }
    }
    else if (Key1 == 1 && Key2 == 1 && Key3 == 1 && Key4 == 1)
    {
        (*KeyFlag) = 1;
    }
    return 0;
}