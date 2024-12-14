#include "LED.h"

void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 即使能GPIOB口的时钟（就是开启PA口的时钟）

    GPIO_InitTypeDef GPIO_InitStruct;              // GPIO_InitTypeDef是用typedef重命名的一个结构体，就是专属于GPIO的一个初始化结构体
                                                   // 该结构体中包含了需要初始化GPIO用到的变量
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  // 定义GPIO的端口模式为推挽输出模式
    GPIO_InitStruct.GPIO_Pin = L1 | L2;            // 定义要使用的引脚为1和2
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 定义该端口的速度，一般没有严格要求，就统一50MHZ即可

    GPIO_Init(GPIOB, &GPIO_InitStruct); // 第一参数选择初试化GPIO哪个口，第二参数选择已经配置好的GPIO初始化结构体指针
    GPIO_SetBits(GPIOB, L1 | L2);
}

void LED1_ON(void)
{
    GPIO_ResetBits(GPIOB, L1);
}

void LED1_OFF(void)
{
    GPIO_SetBits(GPIOB, L1);
}

void LED1_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOB, L1) == 0)
    {
        GPIO_SetBits(GPIOB, L1);
    }
    else
    {
        GPIO_ResetBits(GPIOB, L1);
    }
}

void LED2_ON(void)
{
    GPIO_ResetBits(GPIOB, L2);
}

void LED2_OFF(void)
{
    GPIO_SetBits(GPIOB, L2);
}

void LED2_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOB, L2) == 0)
    {
        GPIO_SetBits(GPIOB, L2);
    }
    else
    {
        GPIO_ResetBits(GPIOB, L2);
    }
}
