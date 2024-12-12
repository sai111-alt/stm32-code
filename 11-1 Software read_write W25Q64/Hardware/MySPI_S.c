#include "MySPI_S.h"

void MySPI_W_SS(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}

uint8_t MySPI_R_MISO(void)
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; // 输入引脚配置为推挽输出模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // 输入引脚配置为上拉输入模式
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    MySPI_W_SS(1);  // 初始化为0，不选择从机设备
    MySPI_W_SCK(0); // 使用模式0，则SCK初试化为0，备注：W25Q64支持SPI模式0和模式1
}

void MySPI_Start(void)
{
    MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
    MySPI_W_SS(1);
}

/// @brief SPI基本字节交换
/// @param ByteSend 交换字节发送的数据
/// @return 通过交换字节接受到的数据
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
    uint8_t ByteReceive = 0x00;
    uint8_t i = 0x00;

    // 0x80 >> i 掩码方式交换字节
    for (i = 0; i < 8; i++)
    {
        MySPI_W_MOSI(ByteSend & (0x80 >> i)); // 当SS置0或SCK产生下降沿后，移入数据，从机的操作不用管
        MySPI_W_SCK(1);                       // SCK产生上升沿
        if (MySPI_R_MISO() == 1)              // 主机读取MISO上的数据给ByterReceive
        {
            ByteReceive |= (0x80 >> i);
        }
        MySPI_W_SCK(0); // SCK产生下降沿
    }

    // // 按移位模型的方式交换字节，这样的好处就是省略了一个变量，ByteSend既是发送的字节数据，也是接受的字节数据
    // for (i = 0; i < 8; i++)
    // {
    //     MySPI_W_MOSI(ByteSend & 0x80);
    //     ByteSend <<= 1;
    //     MySPI_W_SCK(1);        // SCK产生上升沿
    //     if (MySPI_R_MISO == 1) // 主机读取MISO上的数据给ByterReceive
    //     {
    //         ByteSend |= 0x01;
    //     }
    //     MySPI_W_SCK(0); // SCK产生下降沿
    // }

    return ByteReceive;
}
