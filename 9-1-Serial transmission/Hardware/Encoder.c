#include "Encoder.h"

void Encoder_Init(void)
{
    // 注意要使用APB1的时钟开启函数，TIM3是APB1总线的外设
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* 关于引脚输入模式的选择原则：
    一般情况下保持与连接的外部模块默认输入的电平一致，外部是默认高电平则上拉，默认低电平则下拉
    大多数情况都是上拉，若外部模块默认输入的电平你不确定或者说外部信号输入的功率非常小，那么一般
    选择浮空输入，没有上拉下拉去影响外部信号，但缺点就是由于没有上拉下拉，容易受到噪声干扰而导致
    电平跳变*/

    /* 编码器接口就是一个带方向控制的外部时钟，不要特别去配置定时器的时钟*/

    // 初始化时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBasicInitStructure;
    TIM_TimeBasicInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 这个分频参数与时基单元关系并不大，而与滤波器的频率相关，所以可以任意配一个分频，这里选择1分频
    TIM_TimeBasicInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 使用向上计数模式
    TIM_TimeBasicInitStructure.TIM_Period = 65536 - 1;               // ARR自动重装值，取值0~65535，取满量程计数，范围最大且方便换算为负数
    TIM_TimeBasicInitStructure.TIM_Prescaler = 1 - 1;                // PSC预分频器的分频值，取值0~65535，这里选择不分频，编码器的时钟直接驱动定时器
    TIM_TimeBasicInitStructure.TIM_RepetitionCounter = 0;            // 重复计数器的值，只有高级计数器需要，通用定时器用不到，所以直接给0即可
    TIM_TimeBaseInit(TIM3, &TIM_TimeBasicInitStructure);

    // 配置输入捕获
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);          // 编码器只用得到滤波器和极性选择，其他不需要则赋初始值以免出现奇怪的bug
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 这里使用TIM3的通道CH1
    TIM_ICInitStructure.TIM_ICFilter = 0xF;          // 选择滤波器的参数是0xF,具体滤波器的参数可到手册的P290的1111上查看
    // TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    // TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    // 配置编码器接口
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);
    /* 1.这里的TIM_ICPolarity_Rising与上面的TIM_ICPolarity_Rising是一样的，属于重复配置了，重复配置则后面的配置覆盖
    前面的配置，所以可以注释掉上面，但需要注意因为重复配置会覆盖，若main又调用其他的类似模块，可能会覆盖，所以编程时要
    特别注意*/
    /* 2.选择上升沿触发，但对于编码器来说，这里并不指上升沿触发，因为编码器上升沿下降沿都要触发，这里指的是高低电平极性不
    翻转，也就是PPT 84页的均不反相，反之选择TIM_ICPolarity_Falling则表示高低电平翻转，也就是PPT 85页的TI1反相 */

    // 开启定时器
    TIM_Cmd(TIM3, ENABLE);
}

int16_t Encoder_Get(void)
{
    int16_t Temp;
    Temp = TIM_GetCounter(TIM3); // 用中间变量存储CNT的值
    TIM_SetCounter(TIM3, 0);
    /* 然后对CNT清零,根据主程序的延时函数或者定时中断，比如每1s调用一次Encoder_Get ，那么就可以得到
    每s的边沿数，这里边沿数包括A相B相的上升沿下降沿边沿总数，这就是速度*/
    return Temp;
}

/* int16_t Encoder_Get(void)
{
    return TIM_SetCounter(TIM3, 0);
} */
/* 这里的Encoder_Get函数返回的是位置，且返回值是int16型的有符号数，而CNT计数器计数是0~65535
    那么0再减就是65535，形成数据的循环，这里我们直接利用补码特性使得65535显示为-1即可
    上面的Encoder_Get函数返回的是速度*/
