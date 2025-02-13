#include "MyI2C.h"
#include "Delay.h"

void MyI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
    Delay_us(5);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
    Delay_us(5);
}

uint8_t MYI2C_R_SDA(void)
{
    uint8_t BitValue = 0;
    BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    Delay_us(5);
    return BitValue;
}

void MyI2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD; /* I2C的SCL和SDA硬件电路都要初始化为开漏输出模式 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11); /* SCL和SDA置高电平以便起始条件产生 */
}

void MyI2C_Start(void)
{
    MyI2C_W_SDA(1); /* 先释放SDA是为了兼容重复起始条件，重复起始条件之前的1位是读写位，可能是0，可能是1 */
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

void MYI2C_Stop(void)
{
    MyI2C_W_SDA(0); /* 先释放SDA是因为前一位不确定，为了确保可以产生停止条件的SDA上升沿，所以要先置0 */
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

/// @brief 用于发送一个字节数据，若是开始条件后的地址字节，0表示写，1表示读
/// @param Byte 要发送的一个字节数据
void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i = 0;
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }
}

uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i = 0x0;
    uint8_t Byte = 0x0;
    MyI2C_W_SDA(1); /* 主机释放SDA，将SDA控制权交给从机 */
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);
        if (MYI2C_R_SDA() == 1)
        {
            Byte |= (0x80 >> i);
        }
        MyI2C_W_SCL(0);
    }
    return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

/// @brief 用于接收从机应答
/// @return 返回冲击应答值，0应答，1非应答
uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit = 0x0;
    MyI2C_W_SDA(1); /* 主机释放SDA，将SDA控制权交给从机 */
    MyI2C_W_SCL(1);
    AckBit = MYI2C_R_SDA();
    MyI2C_W_SCL(0);
    return AckBit;
}
