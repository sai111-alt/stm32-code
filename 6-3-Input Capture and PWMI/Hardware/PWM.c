#include "PWM.h"

void PWM_Init(void)
{
    // 注意要使用APB1的时钟开启函数，TIM2是APB1总线的外设
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 选择内部时钟为定时器的时钟，也可不写，因为上电默认就是内部时钟
    TIM_InternalClockConfig(TIM2);

    // 初始化时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBasicInitStructure;
    TIM_TimeBasicInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 这个分频参数与时基单元关系并不大，而与滤波器的频率相关，所以可以任意配一个分频，这里选择1分频
    TIM_TimeBasicInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 使用向上计数模式
    TIM_TimeBasicInitStructure.TIM_Period = 100 - 1;                 // ARR自动重装值，取值0~65535，这里固定ARR，那么就固定了PWM的分辨率，一般1%就够了，如果还需要更高，再单独更改即可
    TIM_TimeBasicInitStructure.TIM_Prescaler = 720 - 1;              // PSC预分频器的分频值，取值0~65535
    TIM_TimeBasicInitStructure.TIM_RepetitionCounter = 0;            // 重复计数器的值，只有高级计数器需要，通用定时器用不到，所以直接给0即可
    // 结合ARR，PSC的值，比如72Mhz = 72 000 000hz / 720-1+1 / 100-1+1 = 1000hz，则PWM的频率就是1000hz，相应的PWM分辨率就是1/ARR+1=1%
    TIM_TimeBaseInit(TIM2, &TIM_TimeBasicInitStructure);

    // 配置CH1通道
    TIM_OCInitTypeDef TIM_OCInitStructure;                        // 结构体的一部分参数是高级定时器才会用到的，这里只列出我们通用定时器才会使用的参数
                                                                  // 但是结构体其他参数不赋值可能出现会出现一些小bug，所以最好先对所有成员赋初始值，再对要用到的参数做具体修改
    TIM_OCStructInit(&TIM_OCInitStructure);                       // 使用此函数初始化结构体
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // 选择PWM1模式
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     // 选择REF高极性，即有效电平为高电平
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 选择比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 0;                            // 设置CCR寄存器的值，则占空比就是CCR/ARR+1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);                      // 配置CH1通道则使用OC1初始化函数
                                                                  // 需要注意TIM的OC1，2，3，4等可以同时配置，用到几个就配置几个，
                                                                  // 各自的CCR都可以分别更改，但因为都用的一个计数器做比较，所以它们的PWM波形相位是一致的

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 使用定时器控制GPIO需要用到复用推挽输出模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 使能定时器
    TIM_Cmd(TIM2, ENABLE);
}

/// @brief 用于设置CCR以更改PWM的占空比
/// @param Compare 设置CCR值
void PWM_SetCompare1(uint16_t Compare)
{
    TIM_SetCompare1(TIM2, Compare); // 更改CH1通道的CCR值就需要用到SetCompare1，同理，CH2则用SetCompare2
}

/// @brief 用于设置PSC预分频器的分频值以更改PWM的频率
/// @param Prescaler 设置分频值
void PWM_SetPrescaler(uint16_t Prescaler)
{
    TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Update);
    // TIM_PSCReloadMode_Update和Immediate的区别就是一个有预装作用，一个没有，预装相当于影子寄存器，有缓存的作用，在更新事件时统一生效
    // 而非立即生效，要等到下一个周期的PWM波形产生才生效，而Immediate就是立刻生效，会打断原来的PWM波形
}
