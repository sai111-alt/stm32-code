#include "PWM_Steering_Engine.h"

void PWM_SE_Init(void)
{
    // 注意要使用APB1的时钟开启函数，TIM2是APB1总线的外设
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 选择内部时钟为定时器的时钟，也可不写，因为上电默认就是内部时钟
    TIM_InternalClockConfig(TIM2);

    // 初始化时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBasicInitStructure;
    TIM_TimeBasicInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 这个分频参数与时基单元关系并不大，而与滤波器的频率相关，所以可以任意配一个分频，这里选择1分频
    TIM_TimeBasicInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 使用向上计数模式
    TIM_TimeBasicInitStructure.TIM_Period = 20000 - 1;               // ARR自动重装值，取值0~65535
    TIM_TimeBasicInitStructure.TIM_Prescaler = 72 - 1;               // PSC预分频器的分频值，取值0~65535
    TIM_TimeBasicInitStructure.TIM_RepetitionCounter = 0;            // 重复计数器的值，只有高级计数器需要，通用定时器用不到，所以直接给0即可
    // 结合ARR，PSC的值，比如72Mhz = 72 000 000hz / 72-1+1 / 20000-1+1 = 50hz，则PWM的频率就是50hz，相应的PWM分辨率就是1/ARR+1=1%
    TIM_TimeBaseInit(TIM2, &TIM_TimeBasicInitStructure);

    // 4个输出比较通道对应GPIO不同，要用到哪一个GPIO就要用对应的输出比较通道的函数，同时也要配置相应的GPIO

    // 舵机的是用OC2Init，也就是CH2道，是复用到PA1上的，我们还可以重映射到PB3上使用，所以需要配置CH通道和对应的GPIO
    // 配置CH2通道
    TIM_OCInitTypeDef TIM_OCInitStructure;                        // 结构体的一部分参数是高级定时器才会用到的，这里只列出我们通用定时器才会使用的参数
                                                                  // 但是结构体其他参数不赋值可能出现会出现一些小bug，所以最好先对所有成员赋初始值，再对要用到的参数做具体修改
    TIM_OCStructInit(&TIM_OCInitStructure);                       // 使用此函数初始化结构体
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // 选择PWM1模式
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     // 选择REF高极性，即有效电平为高电平
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 选择比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 0;                            // 设置CCR寄存器的值，则占空比就是CCR/ARR+1，500CCR就是0.5ms，2500就是2.5ms
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);                      // 配置CH2通道则使用OC2初始化函数
    // 配置GPIO的PA1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 使用定时器控制GPIO需要用到复用推挽输出模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 使能定时器
    TIM_Cmd(TIM2, ENABLE);
}

/// @brief 用于设置舵机角度
/// @param Angle 舵机的角度0⁰~180⁰
void SE_SetAngle(float Angle)
{
    // 这里要完成舵机0⁰~180⁰到500~2500CCR的映射
    // 用(2500-500)/(180-0)=2000/180=11.11，那么也就1⁰就是11.11CCR，那么500CCR就是0⁰，500+11.11=511.11就是1⁰
    // 所以Angle*(2000/180)+500就是计算出某角度对应的CCR值，需要注意还有500CCR的偏移不要忘记
    TIM_SetCompare2(TIM2, Angle * (2000 / 180) + 500);
}
