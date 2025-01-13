#include "DHT11.h"

void DHT11_Mode(uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	if (mode)
	{
		GPIO_InitStructure.GPIO_Pin = DATA;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin = DATA;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	}
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/// @brief 主机发送开始信号
/// @param
void DHT11_Start(void)
{
	DHT11_Mode(1);				 // 设置为输出模式
	GPIO_ResetBits(GPIOB, DATA); // 拉低
	Delay_ms(18);				 // 主机拉低18ms
	GPIO_SetBits(GPIOB, DATA);	 // 拉高
	Delay_us(20);				 // 主机拉高18-30us
}

/// @brief 等待DHT11的回应
/// @param
/// @return 1:未检测到DHT11的存在 0:检测到DHT11的存在
uint8_t DHT11_Check(void)
{
	uint8_t TimeOut = 0;
	DHT11_Mode(0);												// 设置为输入模式
	while (GPIO_ReadInputDataBit(GPIOB, DATA) && TimeOut < 100) // DHT11会拉低40~80us
	{
		TimeOut++;
		Delay_us(1);
	};
	if (TimeOut >= 100)
		return 1;
	else
		TimeOut = 0;
	while (!GPIO_ReadInputDataBit(GPIOB, DATA) && TimeOut < 100) // DHT11拉低后会再次拉高40~80us
	{
		TimeOut++;
		Delay_us(1);
	};
	if (TimeOut >= 100)
		return 1;
	return 0;
}

/// @brief 读取一个位
/// @param
/// @return 返回值：1/0
uint8_t DHT11_Read_Bit(void)
{
	uint8_t TimeOut = 0;
	while (GPIO_ReadInputDataBit(GPIOB, DATA) && TimeOut < 100) // 等待变为低电平
	{
		TimeOut++;
		Delay_us(1);
	}
	TimeOut = 0;
	while (!GPIO_ReadInputDataBit(GPIOB, DATA) && TimeOut < 100) // 等待变高电平
	{
		TimeOut++;
		Delay_us(1);
	}
	Delay_us(35); // 等待35us
	if (GPIO_ReadInputDataBit(GPIOB, DATA))
		return 1;
	else
		return 0;
}

// 从DHT11读取一个字节
// 返回值：读到的数据
uint8_t DHT11_Read_Byte(void)
{
	uint8_t i, dat;
	dat = 0;
	for (i = 0; i < 8; i++)
	{
		if (DHT11_Read_Bit())
		{
			dat |= (0x80 >> i);
		}
	}
	return dat;
}

/// @brief 从DHT11读取一次数据，共40位
/// @param temp 温度 取值范围：0~50°
/// @param humi 湿度 取值范围：20%~90%
/// @return 0,正常;1,读取失败
uint8_t DHT11_Read_Data(int8_t *Array)
{
	uint8_t data[5];
	uint8_t i;
	DHT11_Start();
	if (DHT11_Check() == 0)
	{
		for (i = 0; i < 5; i++) // 读取40位数据
		{
			data[i] = DHT11_Read_Byte();
		}
		if ((data[0] + data[1] + data[2] + data[3]) == data[4])
		{
			for (i = 0; i < 5; i++) // 读取40位数据
			{
				Array[i] = data[i];
			}
		}
	}
	else
		return 1;
	return 0;
}

/// @brief 初始化DHT11的Data口同时检测DHT11的存在
void DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = DATA;					  // PG11端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	  // 开漏模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); // 初始化IO口
	GPIO_SetBits(GPIOB, DATA);			   // 拉高
}
