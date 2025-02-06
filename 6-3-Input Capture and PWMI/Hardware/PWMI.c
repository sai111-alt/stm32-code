#include "PWMI.h"

void PWMI_Init(void)
{
    // 注意要使用APB1的时钟开启函数，TIM3是APB1总线的外设
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 选择内部时钟为定时器的时钟，也可不写，因为上电默认就是内部时钟
    TIM_InternalClockConfig(TIM3);

    // 初始化时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBasicInitStructure;
    TIM_TimeBasicInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 这个分频参数与时基单元关系并不大，而与滤波器的频率相关，所以可以任意配一个分频，这里选择1分频
    TIM_TimeBasicInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 使用向上计数模式
    TIM_TimeBasicInitStructure.TIM_Period = 65536 - 1;               /* ARR自动重装值，取值0~65535，作为输入捕获的计次越大越好，防止其还没计到下个上升沿就溢出，所以这里直接取最大
                                                                      所以最大计数65535，按照测周法公式：频率=标准频率/计次，可知这里可以测量的最小频率就是1M/65535=15hz*/
    TIM_TimeBasicInitStructure.TIM_Prescaler = 72 - 1;               // PSC预分频器的分频值，取值0~65535，此值决定了测周法的标准频率，72M/（PSC+1）就是标准频率，这里是1Mhz
    TIM_TimeBasicInitStructure.TIM_RepetitionCounter = 0;            // 重复计数器的值，只有高级计数器需要，通用定时器用不到，所以直接给0即可
    TIM_TimeBaseInit(TIM3, &TIM_TimeBasicInitStructure);

    // 配置PWMI
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                // 这里使用TIM3的通道CH1进行输入捕获
    TIM_ICInitStructure.TIM_ICFilter = 0xF;                         /* 选择滤波器的参数是0xF,具体滤波器的参数可到手册的P290的1111上查看
    还是要补充一下：滤波器不是分频器，不会改变原有频率，而是滤除高频噪声，使信号更平滑，一般滤波器的采样频率都会远高于信号频率*/
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     // 选择上升沿触发
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           // 选择不分频，每次触发都有效
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 选择直连输入TI1FP1
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);
    /* 上面的TIM_PWMIConfig函数是ST公司专门做的一个PWMI封装函数，此函数会将剩下的一个通道初始化为相反的配置
    就如同你上面配置的是直连输入TI1FP1，上升沿触发，那么TIM_PWMIConfig就会将剩下的通道TI1FP2配置为
    交叉输入，下降沿触发，反之，若原来配置的是交叉输入TI1FP2，下降沿触发，那么剩下的通道TI1FP1配置为直连输入
    上升沿触发，所以上面这一行代码就相当于下面6行代码，它们效果是一样的，此函数可以快速将电路配置为PWMI模式
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; // 选择下降沿触发
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI; // 选择交叉输入TI1FP2，则不会影响直连输入的TI1FP1
    TIM_ICInit(TIM3, &TIM_ICInitStructure);*/

    // 配置触发源和从模式
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);    // 触发源选择为TI1FP1
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); // 选择从模式为RESET，使TIM3的CUT可以自动清零

    // 使能定时器
    TIM_Cmd(TIM3, ENABLE);
}

/// @brief 获取输入信号最新一个周期的频率值
uint32_t PWMI_GetFreq(void)
{
    /*测周法的公式就是标准频率/计次N，这里标准频率上面计算可知是1Mhz，
    而计次N就是CH1通道的CCR上的值，需要特别注意标准频率是1Mhz，也就是1 000 000hz
    这超过了16位整型的数值，要用32位整型才可以 */
    return 1000000 / (TIM_GetCapture1(TIM3) + 1); // 实测为1001hz，有个1的偏差，这里多除1使其变为1000hz
}

/// @brief 获取输入信号最新一个周期的占空比
uint32_t PWMI_GetDuty(void)
{
    return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3));
    /* 这里乘100是因为占空比的值本来就是0到1，而乘100使其变为百分比形式呈现，并且不能在末尾乘100，
    首先两个函数返回均是整型数，整型数/整型数还是整型数，这会造成精度的丢失，比如5/2本来是2.5，但这里
    得到的结果是整型数，所以实际是2，此时再乘100，就是200，中间用除法造成了精度的丢失，但是若是5*100=500，
    再/2就是250，最后来做整型除法就会避免精度的丢失
    其中实测为49%，有个1的偏差，这里分母多+1使其变为50%
     */
}
