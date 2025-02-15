#include "Serial.h"

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); /* 注意USART1是APB2的外设，其他两个USART是APB1的外设 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; /* 片上外设输出要配置位复用推挽输出模式 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; /* 片上外设的输入可浮空可上拉，一般建议上拉 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 初始化USART
    USART_InitTypeDef USART_InitStruture;
    USART_InitStruture.USART_BaudRate = 9600;                                      /* 波特率设置9600 */
    USART_InitStruture.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* 不使用硬件流控制 */
    USART_InitStruture.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 /* 选择串口发送与接收模式 */
    USART_InitStruture.USART_Parity = USART_Parity_No;                             /* 不需要校验位 */
    USART_InitStruture.USART_StopBits = USART_StopBits_1;                          /* 停止位选择为1位 */
    USART_InitStruture.USART_WordLength = USART_WordLength_8b;                     /* 字长选择为8位 */
    USART_Init(USART1, &USART_InitStruture);

    // 配置NVIC
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  /* 开启RXNE标志位到NVIC的输出 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); /* 分组芯片只能用一种，所以此代码只需执行一次，若不放心，可放在主函数以避免代码冗余 */
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);

    USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ; /* 等待发送数据寄存器DR为空（即TXE标志位为1，后续由硬件自动清0），以便下一次给其赋值发送 */
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i = 0;
    for (i = 0; i < Length; i++)
    {
        Serial_SendByte(Array[i]);
    }
}

void Serial_SendString(char *String)
{
    uint8_t i = 0;
    for (i = 0; String[i] != '\0'; i++)
    {
        Serial_SendByte(String[i]);
    }
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch);
    return ch;
}

void Serial_Printf(char *format, ...) /* 这是C语言可变参数的用法，有机会学习一下 */
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString(String);
}

uint8_t Serial_GetRxFlag(void)
{
    if (Serial_RxFlag == 1)
    {
        Serial_RxFlag = 0;
        return 1;
    }
    return 0;
}

uint8_t Serial_GetRxData(void)
{
    return Serial_RxData;
}

void USART1_IRQHandler(void)
{
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
    {
        Serial_RxData = USART_ReceiveData(USART1);
        Serial_RxFlag = 1;
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
