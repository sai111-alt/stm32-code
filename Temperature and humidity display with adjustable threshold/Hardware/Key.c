#include "Key.h"

// 改进：按键响应灵敏
//  键码
uint8_t Key_KeyNumber = 0;

void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = k1 | k2 | k3 | k4;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// 获取当下被按下到抬起的按键码
uint8_t Key_GetNum(void)
{
    unsigned char Temp = 0;
    Temp = Key_KeyNumber;
    Key_KeyNumber = 0;
    return Temp;
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
    if (Key3 == 0)
    {
        KeyNumber = 3;
    }
    if (Key4 == 0)
    {
        KeyNumber = 4;
    }

    return KeyNumber;
}

void IndependentKey_Loop(void)
{
    static unsigned char NowState = 0;
    static unsigned char LastState = 0;
    LastState = NowState;
    NowState = IndependentKey_Getstate();
    if (LastState == 1 && NowState == 0) // 当LastState = 1时，表示上个状态按键1被按下
    {                                    // 同时若NowState = 0时，则表示现在没有按键按下
        Key_KeyNumber = 1;               // LastState == 1 && NowState == 0则表示按键1被按下了且按键现在抬起了
    }
    else if (LastState == 2 && NowState == 0)
    {
        Key_KeyNumber = 2;
    }
    else if (LastState == 3 && NowState == 0)
    {
        Key_KeyNumber = 3;
    }
    else if (LastState == 4 && NowState == 0)
    {
        Key_KeyNumber = 4;
    }
}

// 未改进：按键响应不灵敏
// #include "Key.h"

// void Key_Init(void)
// {
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

//     GPIO_InitTypeDef GPIO_InitStruct;

//     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
//     GPIO_InitStruct.GPIO_Pin = k1 | k2 | k3 | k4;
//     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

//     GPIO_Init(GPIOA, &GPIO_InitStruct);
// }

// uint8_t Key_GetNum(uint8_t *KeyFlag)
// {
//     if ((Key1 == 0 || Key2 == 0 || Key3 == 0 || Key4 == 0) && (*KeyFlag))
//     {
//         Delay_ms(10); // 消抖
//         (*KeyFlag) = 0;
//         if (Key1 == 0)
//         {
//             return 1;
//         }
//         else if (Key2 == 0)
//         {
//             return 2;
//         }
//         else if (Key3 == 0)
//         {
//             return 3;
//         }
//         else if (Key4 == 0)
//         {
//             return 4;
//         }
//     }
//     else if (Key1 == 1 && Key2 == 1 && Key3 == 1 && Key4 == 1)
//     {
//         (*KeyFlag) = 1;
//     }
//     return 0;
// }
