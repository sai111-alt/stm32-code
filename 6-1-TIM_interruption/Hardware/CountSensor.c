#include "CountSensor.h"

uint16_t CountSensor = 0;

void CountSensor_Init(void)
{
    // EXTI和NVIC的时钟是默认打开的，无需开去时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // AFIO用于引脚复用功能的选择和重定义

    // 配置GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 用于配置AFIO的数据选择器,详细见讲义,这里即将EXTI14配置为GPIOB_PIN14
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);

    // 配置EXTI
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line14;                    // 配置EXTI14为外部中断线
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;                      // 开启外部中断
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;            // 使用外部中断模式，而非事件响应模式
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 选择触发方式为下降沿触发
    EXTI_Init(&EXTI_InitStruct);

    // 配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 分组芯片只能用一种，所以此代码只需执行一次，若不放心，可放在主函数以避免代码冗余
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);
}

uint16_t CountSensor_Get(void)
{
    return CountSensor;
}

/// @brief 外部中断函数，注意中断函数名字是固定的，不能写错，到Start文件夹下的startup_stm32F10x_cl.s文件下查询
void EXTI15_10_IRQHandler(void)
{
    // 判断是否是EXTI14触发的中断
    if (EXTI_GetITStatus(EXTI_Line14) == SET)
    {
        CountSensor++;
        EXTI_ClearITPendingBit(EXTI_Line14); // 中断标志为1会进入中断，避免卡死，每次执行完中断函数需手动清除其标志位
    }
}
