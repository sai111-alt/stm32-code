#include "MPU6050.h"

/// @brief 指定寄存器地址写一个字节
/// @param RegAddress 寄存器地址
/// @param Data 要写入的一个字节数据
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(Data);
    MyI2C_ReceiveAck();
    MYI2C_Stop();
}

/// @brief 指定寄存器地址读一个字节
/// @param RegAddress 寄存器地址
/// @return 返回读取的一个字节数据
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data = 0;

    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress);
    MyI2C_ReceiveAck();

    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS | 0x01); /* 将最后一位改为读的标志位，读标志位为1 */
    MyI2C_ReceiveAck();
    Data = MyI2C_ReceiveByte();
    MyI2C_SendAck(1); /* 发送1非应答，表示不读了，要停止了，反之，若还要读，应给0应答，在接收最后一个字节后，给1非应答 */

    MYI2C_Stop();

    return Data;
}

void MPU6050_Init(void)
{
    MyI2C_Init();
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
    /* 配置电源管理寄存器1：依次是
    0设备不复位，0解除睡眠，0不循环，0无关位，0温度传感器使能，001陀螺仪时钟频率 */
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x01);
    /* 配置电源管理寄存器2：依次是
    0设备不复位，0解除睡眠，0不循环，0无关位，0温度传感器使能，001陀螺仪时钟频率 */
}
