#include "LED.h"

uint8_t LED1_Mode = 0;
uint8_t LED2_Mode = 0;

uint16_t LED1_Count = 0;
uint16_t LED2_Count = 0;

void LED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 即使能GPIOA口的时钟（就是开启PA口的时钟）

    GPIO_InitTypeDef GPIO_InitStruct;                   // GPIO_InitTypeDef是用typedef重命名的一个结构体，就是专属于GPIO的一个初始化结构体
                                                        // 该结构体中包含了需要初始化GPIO用到的变量
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;       // 定义GPIO的端口模式为推挽输出模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2; // 定义要使用的引脚为1和2
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;      // 定义该端口的速度，一般没有严格要求，就统一50MHZ即可

    GPIO_Init(GPIOA, &GPIO_InitStruct); // 第一参数选择初试化GPIO哪个口，第二参数选择已经配置好的GPIO初始化结构体指针
    GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

void LED1_SetMode(uint8_t Mode)
{
    if (Mode != LED1_Mode)
    {
        LED1_Mode = Mode;
        LED1_Count = 0;
    }
}
void LED2_SetMode(uint8_t Mode)
{
    if (Mode != LED2_Mode)
    {
        LED2_Mode = Mode;
        LED2_Count = 0;
    }
}

void LED1_ON(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void LED1_OFF(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void LED2_ON(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

void LED2_OFF(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

void LED_Loop(void)
{
    if (LED1_Mode == 0)
    {
        LED1_OFF();
    }
    else if (LED1_Mode == 1)
    {
        LED1_ON();
    }
    else if (LED1_Mode == 2)
    {
        LED1_Count++;
        if (LED1_Count >= 999)
        {
            LED1_Count = 0;
        }
        if (LED1_Count < 500)
        {
            LED1_ON();
        }
        else
        {
            LED1_OFF();
        }
    }
    else if (LED1_Mode == 3)
    {
        LED1_Count++;
        if (LED1_Count >= 99)
        {
            LED1_Count = 0;
        }
        if (LED1_Count < 50)
        {
            LED1_ON();
        }
        else
        {
            LED1_OFF();
        }
    }
    else if (LED1_Mode == 4)
    {
        LED1_Count++;
        if (LED1_Count >= 999)
        {
            LED1_Count = 0;
        }
        if (LED1_Count < 100)
        {
            LED1_ON();
        }
        else
        {
            LED1_OFF();
        }
    }
    if (LED2_Mode == 0)
    {
        LED2_OFF();
    }
    else if (LED2_Mode == 1)
    {
        LED2_ON();
    }
    else if (LED2_Mode == 2)
    {
        LED2_Count++;
        if (LED2_Count >= 999)
        {
            LED2_Count = 0;
        }
        if (LED2_Count < 500)
        {
            LED2_ON();
        }
        else
        {
            LED2_OFF();
        }
    }
    else if (LED2_Mode == 3)
    {
        LED2_Count++;
        if (LED2_Count >= 99)
        {
            LED2_Count = 0;
        }
        if (LED2_Count < 50)
        {
            LED2_ON();
        }
        else
        {
            LED2_OFF();
        }
    }
    else if (LED2_Mode == 4)
    {
        LED2_Count++;
        if (LED2_Count >= 999)
        {
            LED2_Count = 0;
        }
        if (LED2_Count < 100)
        {
            LED2_ON();
        }
        else
        {
            LED2_OFF();
        }
    }
}
