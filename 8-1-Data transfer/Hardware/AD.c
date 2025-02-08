#include "AD.h"

void AD_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // 注意ADC都是APB2上的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置ADC时钟的分频器
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 由APB2时钟72Mhz 6分频即ADC时钟为12Mhz

    // 初始化GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
    /* ADC输入有专门的输入模式，可查看PPT的结构图，此时GPIO无效，引脚直接接入内部ADC */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 初始化ADC
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;                  // 选择ADC独立模式而非双ADC模式
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;              // 选择数据右对齐
    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 选择不使用外部触发，这里我们使用软件触发
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;                 // 选择单次转换模式
    ADC_InitStruct.ADC_ScanConvMode = DISABLE;                       // 选择非扫描模式
    ADC_InitStruct.ADC_NbrOfChannel = 1;                             // 因为是非扫描模式，所以只会是第一个序列的ADC通道有效
    ADC_Init(ADC1, &ADC_InitStruct);

    // 开启ADC
    ADC_Cmd(ADC1, ENABLE);

    // ADC校准（开启校准前，ADC至少上电2个ADC周期）
    ADC_ResetCalibration(ADC1); // 重置ADC1的校准寄存器
    while (ADC_GetResetCalibrationStatus(ADC1) == SET)
        ;                       // 等待ADC1的校准寄存器重置完成
    ADC_StartCalibration(ADC1); // 启动ADC1的校准
    while (ADC_GetCalibrationStatus(ADC1) == SET)
        ; // 等到ADC1校准完成
}

/// @brief 开启ADC转换并在转换完成后返回转换值
uint16_t ADC_GetValue(uint8_t ADC_Channel)
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5); // 配置ADC规则组（Regular）
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                   // 软件触发ADC1转换，连续转换只需触发一次即可
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
        ;                                // 注意这里的转换完成是1，所以不要与前面搞混了
    return ADC_GetConversionValue(ADC1); // 读取完会自动给标志位EOC置0，这里无需手动置0
}
