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
    /* 配置——电源管理寄存器1：依次是
    0设备不复位，0解除睡眠，0不循环，0无关位，0温度传感器使能，001陀螺仪时钟频率 */
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
    /* 配置——电源管理寄存器2：依次是
    00不需要循环唤醒频率，000000芯片的6个轴均不待机 */
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
    /* 时钟分频，这里给的0x09，是10分频，值越小，分频系数越小，则时钟越快 */
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
    /* 配置——配置寄存器：依次是
    00000不需要外部同步，110数字低通滤波器，这里110是最平滑的滤波 */
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
    /* 配置——陀螺仪配置寄存器：依次是
    000不需要自测，11选择最大满量程+-2000，000无关位*/
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
    /* 配置——加速度计配置寄存器：依次是
    000不需要自测，11选择最大满量程+-16g，000不需要高通滤波器*/
}

/// @brief 返回芯片ID号
uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

/// @brief 用于返回芯片6轴的数据，主函数的数字名最好也是AccGyro，这样就可以用MPU6050头文件的6轴宏定义了
/// @param AccGyro 指向AccGyro数组首地址，注意芯片传递的数据是有符号数int而非uint
void MPU6050_GetData(int16_t *AccGyro)
{
    uint8_t DataH = 0;
    uint8_t DataL = 0;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    AccX = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    AccY = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    AccZ = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    GyroX = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    GyroY = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    GyroZ = (DataH << 8) | DataL;

    /* 这里测出来的具体值/32765再乘以满量程就可以得出具体的加速度值或角速度值了 */
}

/* MPU6050有一个DMP（数据运动处理器），这是它自动的姿态解算器，
可以得到物体本身的姿态角，在平衡车，飞行器里面常用，做此类项目记得加上
姿态解算的代码 */
