#include "MySPI_H.h"

void MySPI_W_SS(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启要使用的IO口上的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  // 开启要使用的SPI1上的时钟，注意SPI1也是APB2上的

    // 初始化IO口
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct); // ss用程序模拟的方式实现，即这里的PA4引脚

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;        // 配置为复用推挽输出模式，即由片上外设控制，也就是这里的SPI外设
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7; // 5引脚的复用功能是SCK，7引脚的复用功能是MOSI
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // SPI输入引脚配置为上拉输入（浮空输入也可）
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;     // 6引脚的复用功能是MISO
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 初始化SPI1外设
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                        // 配置STM32的SPI为主机模式
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   // 配置为双线全双工
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                    // 配置为8为数据帧
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                   // 配置为高位先行
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // 配置为128分频，即72MHZ/128=562.5KHZ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                           // 配置CPOL为0
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                         // 即第一个边沿移入（采样）数据，综合CPOL，为SPI模式0
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                            // NSS这里使用程序模拟实现，可以配置为软件控制
    SPI_InitStructure.SPI_CRCPolynomial = 7;                             // 配置CRC校验码的多项式，填一个默认值7即可，这里我们不用
    SPI_Init(SPI1, &SPI_InitStructure);

    // 使能SPI1外设
    SPI_Cmd(SPI1, ENABLE);

    // 初始化为默认不选择从机
    MySPI_W_SS(1);
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
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET)
        ;                             // SET就是1，这里表示要等待TXE标志位变为1，也就是TDR变为空，当TDR变为空就应该放数据了
    SPI_I2S_SendData(SPI1, ByteSend); // 将ByteSend的数据写入TDR，写入的同时会自动给TXE清0，然后TDR会自动将数据放入移位寄存器（此时TDR数据为空，则TXE会自动置1），
                                      // 然后SPI1外设会自动产生时序将移位寄存器的数据送入MOSI
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET)
        ;                             // 等待RXNE标志位变为1,也就是MISO接收到了一个字节，同时MISO与MOSI是同步的，所以MISO接收到了一个字节也代表MOSI发送完了一个字节数据
    return SPI_I2S_ReceiveData(SPI1); // 返回RDR接收到的数据，读取的同时会自动给RXNE清0
}
