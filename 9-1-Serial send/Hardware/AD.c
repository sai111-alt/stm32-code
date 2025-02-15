#include "AD.h"

uint16_t AD_Value[4] = {0};
/* 初始化变量不要在AD.h头文件初始化，否则它会认为是头文件的变量，但这里其实是AD.C文件的变量，
   而通过头文件引用给主函数.c使用的 */

void AD_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); /* 注意ADC都是APB2上的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); /* 注意DMA是AHB总线的外设 */

    // 配置ADC时钟的分频器
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); /* 由APB2时钟72Mhz 6分频即ADC时钟为12Mhz */

    // 初始化GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    /* ADC输入有专门的输入模式，可查看PPT的结构图，此时GPIO无效，引脚直接接入内部ADC */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 配置ADC规则组（Regular）序列器
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5); /* 序列1对应ADC1通道0 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5); /* 序列2对应ADC1通道1 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5); /* 序列3对应ADC1通道2 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5); /* 序列4对应ADC1通道3 */

    // 初始化ADC
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                  /*  选择ADC独立模式而非双ADC模式 */
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;              /*  选择数据右对齐 */
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; /*  选择不使用外部触发，这里我们使用软件触发 */
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;                  /*  选择连续转换模式 */
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;                        /*  选择扫描模式，扫描多个序列 */
    ADC_InitStruct.ADC_NbrOfChannel = 4;                             /*  指定扫描模式只需扫描序列器前4个序列即可 */
    ADC_Init(ADC1, &ADC_InitStruct);

    // 初始化DMA
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;             /* 存放要转运数据的外设地址 */
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; /* 选择数据宽度位字节，也就是16位，对应ADC的DR位数 */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            /* 选择转运外设地址 不 自增 */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;                  /* 存放要转运数据的存储器地址 */
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         /* 选择数据宽度位字节，也就是16位 */
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     /* 选择转运存储器地址  自增 */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          /* 选择外设站点作为源头 */
    DMA_InitStructure.DMA_BufferSize = 4;                                       /* 选择转运传输的次数 */
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             /* 选择传输计数器可以自动重装 */
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                /* 选择硬件触发方式 */
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       /* 选择中等优先级，如果同时使用多个DMA通道需要注意这个参数 */
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                /* 选择DMA1的通道1 ，注意ADC1的硬件触发是定死在DMA1的通道1上的*/

    // 开启DMA1_Channel1
    DMA_Cmd(DMA1_Channel1, ENABLE);

    // 开启ADC1到DMA的通道
    ADC_DMACmd(ADC1, ENABLE);

    // 开启ADC
    ADC_Cmd(ADC1, ENABLE);

    // ADC校准（开启校准前，ADC至少上电2个ADC周期）
    ADC_ResetCalibration(ADC1); /* 重置ADC1的校准寄存器 */
    while (ADC_GetResetCalibrationStatus(ADC1) == SET)
        ;                       /* 等待ADC1的校准寄存器重置完成 */
    ADC_StartCalibration(ADC1); /* 启动ADC1的校准 */
    while (ADC_GetCalibrationStatus(ADC1) == SET)
        ; /* 等到ADC1校准完成 */

    ADC_SoftwareStartConvCmd(ADC1, ENABLE); /*  软件触发ADC1转换，连续转换只需触发一次即可 */
}
