#include "Timer.h"

void Timer_Init(void)
{
    // 注意要使用APB1的时钟开启函数，TIM2是APB1总线的外设
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 选择内部时钟为定时器的时钟，也可不写，因为上电默认就是内部时钟
    // TIM_InternalClockConfig(TIM2);
    // 选择外部时钟为定时器的时钟，外部时钟要用到GPIO，所以还要先配置GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 需要注意GPIO是APB2的外设
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00); // 设置外部时钟，第二个参数选择无需分频，第三参数选择高电平或上升沿生效，第三个参数选择0x00无需滤波器

    // 初始化时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBasicInitStructure;
    TIM_TimeBasicInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 这个分频参数与时基单元关系并不大，而与滤波器的频率相关，所以可以任意配一个分频，这里选择1分频
    TIM_TimeBasicInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 使用向上计数模式
    TIM_TimeBasicInitStructure.TIM_Period = 10 - 1;                  // ARR自动重装值，取值0~65535
    TIM_TimeBasicInitStructure.TIM_Prescaler = 1 - 1;                // PSC预分频器的分频值，取值0~65535
    TIM_TimeBasicInitStructure.TIM_RepetitionCounter = 0;            // 重复计数器的值，只有高级计数器需要，通用定时器用不到，所以直接给0即可
    // 结合ARR，PSC的值，比如72Mhz = 72 000 000hz / 7200-1+1 / 10000-1+1 = 1hz，取倒数就是定时1s
    TIM_TimeBaseInit(TIM2, &TIM_TimeBasicInitStructure);

    // 使能更新中断
    TIM_ClearFlag(TIM2, TIM_FLAG_Update); // 清除更新标志位，避免初始化完就进中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // 配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 分组芯片只能用一种，所以此代码只需执行一次，若不放心，可放在主函数以避免代码冗余
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);

    // 使能定时器
    TIM_Cmd(TIM2, ENABLE);
}

/// @brief 获取当前计数器的值
uint16_t Timer_GetCounter(void)
{
    return TIM_GetCounter(TIM2);
}

/// @brief 定时器中断函数，注意中断函数名字是固定的，不能写错，到Start文件夹下的startup_stm32F10x_cl.s文件下查询
// void TIM2_IRQHandler(void)
// {
//     // 判断是否是更新中断触发的
//     if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//     {

//         TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 中断标志为1会进入中断，避免卡死，每次执行完中断函数需手动清除其标志位
//     }
// }
