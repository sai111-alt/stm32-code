#include "Buzzer.h"

void Buzzer_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = IO;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_SetBits(GPIOB, IO);
}

void Buzzer_ON(void)
{
    GPIO_ResetBits(GPIOB, IO);
}

void Buzzer_OFF(void)
{
    GPIO_SetBits(GPIOB, IO);
}

void Buzzer_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOB, IO) == 0)
    {
        GPIO_SetBits(GPIOB, IO);
    }
    else
    {
        GPIO_ResetBits(GPIOB, IO);
    }
}
