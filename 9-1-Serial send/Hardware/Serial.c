#include "Serial.h"

void Serial_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); /* 注意USART1是APB2的外设，其他两个USART是APB1的外设 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; /* 片上外设输出要配置位复用推挽输出模式 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    USART_InitTypeDef USART_InitStruture;
    USART_InitStruture.USART_BaudRate = 9600;                                      /* 波特率设置9600 */
    USART_InitStruture.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /* 不使用硬件流控制 */
    USART_InitStruture.USART_Mode = USART_Mode_Tx;                                 /* 选择串口发送模式 */
    USART_InitStruture.USART_Parity = USART_Parity_No;                             /* 不需要校验位 */
    USART_InitStruture.USART_StopBits = USART_StopBits_1;                          /* 停止位选择为1位 */
    USART_InitStruture.USART_WordLength = USART_WordLength_8b;                     /* 字长选择为8位 */
    USART_Init(USART1, &USART_InitStruture);

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
