#include "Key.h"

// 改进：按键响应灵敏
//  键码
uint8_t Key_KeyNumber = 0;

void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = k1 | k2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/// @brief 获取当下被按下到抬起的按键码
uint8_t Key_GetNum(void)
{
    unsigned char Temp = 0;
    if (Key_KeyNumber)
    {
        Temp = Key_KeyNumber;
        Key_KeyNumber = 0;
        return Temp;
    }
    return 0;
}

/// @brief 作用是获取独立按键是否被按下
/// @return 范围为0~4，无按键按下返回值为0
uint8_t IndependentKey_Getstate()
{
    uint8_t KeyNumber = 0;

    if (Key1 == 0)
    {
        KeyNumber = 1;
    }
    if (Key2 == 0)
    {
        KeyNumber = 2;
    }

    return KeyNumber;
}

void IndependentKey_Loop(void)
{
    static unsigned char Key_Count = 0;
    static unsigned char NowState = 0;
    static unsigned char LastState = 0;

    Key_Count++;
    if (Key_Count >= 20)
    {
        Key_Count = 0;
        LastState = NowState;
        NowState = IndependentKey_Getstate();
        if (LastState != 0 && NowState == 0) // 当LastState = 1时，表示上个状态按键1被按下
        {                                    // 同时若NowState = 0时，则表示现在没有按键按下
            Key_KeyNumber = LastState;       // LastState == 1 && NowState == 0则表示按键1被按下了且按键现在抬起了
        }
    }
}
