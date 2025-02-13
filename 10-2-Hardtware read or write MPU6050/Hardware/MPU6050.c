#include "MPU6050.h"

void MPU6050_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT)
{
    uint32_t Timeout = 10000; /* 增加超时等待的机制，避免出问题使其一直卡死在while死循环中 */
    while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
    {
        Timeout--;
        if (Timeout == 0)
        {
            break;
        }
    }
}

/// @brief 指定寄存器地址写一个字节
/// @param RegAddress 寄存器地址
/// @param Data 要写入的一个字节数据
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    // HardwareI2C:（硬件的接收应答和发送应答都是全自动的，无需函数主动去发送接收）
    I2C_GenerateSTART(I2C2, ENABLE);                                       /* 产生I2C起始条件 */
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);                 /* 等待EV5事件产生*/
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); /* 发送从机地址MPU6050_ADDRESS，I2C_Direction_Transmitter表示给地址最后一位清零即写 */
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);   /* 等待EV6事件产生*/
    I2C_SendData(I2C2, RegAddress);                                        /* 发送要写入的寄存器地址 */
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);           /* 等待EV8事件产生*/
    I2C_SendData(I2C2, Data);                                              /* 发送要写入的一个字节数据 */
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);            /* 发送最后一个字节，要等待EV8_2事件产生*/
                                                                           /* 反之，若不是最后一个字节，应等待EV8事件产生 */
    I2C_GenerateSTOP(I2C2, ENABLE);                                        /* 产生I2C停止条件 */
}

/// @brief 指定寄存器地址读一个字节
/// @param RegAddress 寄存器地址
/// @return 返回读取的一个字节数据
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data = 0;

    // HardwareI2C:（硬件的接收应答和发送应答都是全自动的，无需函数主动去发送接收）
    I2C_GenerateSTART(I2C2, ENABLE);                                       /* 产生I2C起始条件 */
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);                 /*等待EV5事件产生*/
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter); /* 发送从机地址MPU6050_ADDRESS，I2C_Direction_Transmitter表示给地址最后一位清零即写 */
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);   /* 等待EV6事件产生*/
    I2C_SendData(I2C2, RegAddress);                                        /* 因为这里是读，所以其实是发送要读取的寄存器地址 */
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);            /* 等待EV8_2事件产生*/
    /* EV8_2事件的本质就是移位寄存器的数据寄存器都空，此时再发送起始条件最佳，因为若移位寄存器非空，再发送起始条件，可
    能会使当前发送的数据截断，虽然实际测试没有，但保险起见还是选择EV8_2事件 */
    I2C_GenerateSTART(I2C2, ENABLE);                                    /* 重复产生I2C起始条件 */
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);              /* 等待EV5事件产生*/
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver); /* 发送从机地址MPU6050_ADDRESS，I2C_Direction_Receiver表示给地址最后一位置1即读 */
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);   /* 等待EV6事件产生,注意这里是主机接收的EV6事件，前面是主机发送的EV6事件*/
    I2C_AcknowledgeConfig(I2C2, DISABLE);
    I2C_GenerateSTOP(I2C2, ENABLE);
    /* 因为只读一个字节，在EV6事件后应立刻将应答位清0即非应答，查Notes 158页可知，一般在接收一个字节后STM32主机会自动发送应答
    且没有任何事件产生，也就是说如果你不提前应答位清0，它就会自动发送应答，并自动接收下一个字节数据，所以要提前应答位清0，并且产生
    停止信号，不用担心数据丢失，因为数据已经接收了并放在了DR寄存器中，你取出即可 */
    MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED); /* 等待EV7事件产生*/
    I2C_AcknowledgeConfig(I2C2, ENABLE);                     /* 给应答位置1，使得主机默认状态下就是要给从机应答，前面特殊情况才非应答，以方便后期代码修改*/

    Data = I2C_ReceiveData(I2C2);
    return Data;
}

void MPU6050_Init(void)
{
    // HardwareI2C:
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  /* 注意：无论是I2C1还是I2C2都是APB1上的外设 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); /* 注意：GPIO是APB2上的外设 */

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD; /* 配置为复用开漏输出模式，复用即将GPIO的控制权给片上外设，开漏输出即I2C协议的硬件要求 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;                                /* 选择I2C外设的模式为I2C通信协议而非SMBus通信协议 */
    I2C_InitStructure.I2C_ClockSpeed = 50000;                                 /* 时钟频率即50Khz，选择范围0-400Khz，对于STM32，
    0-100Khz是标准速度，100-400Khz是快速，不能超过对应控制芯片的时钟频率，否则读不过来，这里MPU6050的最大为400Khz */
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;                        /*设置时钟占空比，是用于快速模式，标准速度就是1:1，
    因为我们的时钟频率是50Khz，是标准速度，所以这里的设置对应我们标准速度是无效的，可任意设置*/
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;                               /* 配置在接收数据后主机发送应答 */
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; /* 这个参数是STM32作为从机，响应几位的地址，我们用不到，可任意设置*/
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;                                 /* 这个参数是设置STM32作为从机的地址，同样用不到，但需要注意
    如果要用，这个从机地址要与上面的地址相匹配，上面选择响应7位，则这里你应该给7位的从机地址，10位同理 */
    I2C_Init(I2C2, &I2C_InitStructure);

    I2C_Cmd(I2C2, ENABLE);

    // MPU6050初始配置:
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
