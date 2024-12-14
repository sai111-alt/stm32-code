#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"
#include "Key.h"
#include "LED.h"
#include "DHT22.h"

uint8_t TemHemValue[4] = {0};  // 存放温湿度值，0湿度整数，1湿度度小数，2温度整数，3温度小数
int8_t ArrayValue[4] = {0};	   // 放阈值,0即T的下阈值，1即T的上阈值，2即H的下阈值，3即H的上阈值
uint8_t KeyNum = 0;			   // 存键码
uint8_t SetFlag = 0;		   // 进入阈值设置的标志位，0完成1设置
uint8_t SetPlace = 0;		   // 设置选择，0即T的上阈值，1即T的下阈值，2即H的上阈值，3即H的下阈值
uint8_t SetPlaceFlashFlag = 0; // 闪烁标志位

void ValueShow(void)
{
	if (DHT22_Read_Data(TemHemValue))
	{
		OLED_ShowString(1, 1, "Error");
	}
	else
	{
		OLED_ShowString(1, 1, "Welcome");
	}
	Delay_ms(200); // 等待DHT22在输出完数据后还会输出50us的低电平，必须等待该电平过去，否则会出错

	// 温度小数字节的最高位是温度的符号位，0正1负
	OLED_ShowNum(2, 4, TemHemValue[2], 2);
	OLED_ShowString(2, 6, ".");
	OLED_ShowNum(2, 7, TemHemValue[3], 1);
	OLED_ShowString(2, 8, "*C");

	OLED_ShowNum(3, 4, TemHemValue[0], 2);
	OLED_ShowString(3, 6, ".");
	OLED_ShowNum(3, 7, TemHemValue[1], 1);
}

void ValueSet(void)
{
	// 阈值显示
	OLED_ShowString(1, 1, "Setting");
	OLED_ShowString(2, 1, "TL:");
	OLED_ShowString(2, 9, "HL:");
	OLED_ShowString(3, 1, "TH:");
	OLED_ShowString(3, 9, "HH:");
	OLED_ShowSignedNum(2, 4, ArrayValue[0], 2);
	OLED_ShowSignedNum(3, 4, ArrayValue[1], 2);
	OLED_ShowSignedNum(2, 12, ArrayValue[2], 2);
	if (ArrayValue[3] == 100)
	{
		OLED_ShowSignedNum(3, 12, ArrayValue[3], 3);
	}
	else
	{
		OLED_ShowSignedNum(3, 12, ArrayValue[3], 2);
	}
	if (KeyNum == 2)
	{
		SetPlace++;
		SetPlace %= 4; // 取值0~3
	}
	if (KeyNum == 3)
	{
		ArrayValue[SetPlace]++;
		// 越界判断
		if (ArrayValue[1] > 80) // 温度上阈值越界判断
		{
			ArrayValue[1] = 80;
		}
		if (ArrayValue[3] > 100) // 湿度上阈值越界判断
		{
			ArrayValue[3] = 100;
		}
		if (ArrayValue[0] >= ArrayValue[1]) // 温度下阈值++不能超过上阈值
		{
			ArrayValue[0]--;
		}
		if (ArrayValue[2] >= ArrayValue[3]) // 湿度下阈值++不能超过上阈值
		{
			ArrayValue[2]--;
		}
	}
	if (KeyNum == 4)
	{
		ArrayValue[SetPlace]--;
		if (ArrayValue[0] < -40) // 温度下阈值越界判断
		{
			ArrayValue[1] = -40;
		}
		if (ArrayValue[2] < 0) // 湿度下阈值越界判断
		{
			ArrayValue[3] = 0;
		}
		if (ArrayValue[1] <= ArrayValue[0]) // 温度上阈值--不能少于上阈值
		{
			ArrayValue[1]++;
		}
		if (ArrayValue[3] <= ArrayValue[2]) // 湿度上阈值--不能少于上阈值
		{
			ArrayValue[3]++;
		}
	}

	// 闪烁显示
	if (SetPlace == 0)
	{
		OLED_ShowString(2, 4, "   ");
	}
	else
	{
		OLED_ShowSignedNum(2, 4, ArrayValue[0], 2);
	}
	if (SetPlace == 1)
	{
		OLED_ShowString(3, 4, "   ");
	}
	else
	{
		OLED_ShowSignedNum(3, 4, ArrayValue[1], 2);
	}

	if (SetPlace == 2)
	{
		OLED_ShowString(2, 12, "   ");
	}
	else
	{
		OLED_ShowSignedNum(2, 12, ArrayValue[2], 2);
	}
	if (SetPlace == 3)
	{
		OLED_ShowString(3, 12, "   ");
	}
	else
	{
		if (ArrayValue[3] == 100)
		{
			OLED_ShowSignedNum(3, 12, ArrayValue[3], 3);
		}
		else
		{
			OLED_ShowSignedNum(3, 12, ArrayValue[3], 2);
		}
	}
}

int main(void)
{
	OLED_Init();
	W25Q64_Init();
	Key_Init();
	LED_Init();
	DHT22_Init();

	// 将W25Q64中已经存好的阈值放入，第一次存入的数据自然是空白数据0xFF
	// 因为这里是温湿度是有符号数-127~128，所以0xFF最高位是1，表示负数，应按补码计算原码为-1，所以第一次给的数据就是-1
	W25Q64_ReadData(0X000000, ArrayValue, 4);
	// DAT测量范围T为-40~80，H为0%~100%
	if (ArrayValue[1] > 80 || ArrayValue[0] < -40 || ArrayValue[3] > 100 || ArrayValue[2] < 0 ||
		ArrayValue[0] > ArrayValue[1] || ArrayValue[2] > ArrayValue[3])
	{
		ArrayValue[0] = 5;
		ArrayValue[1] = 30;
		ArrayValue[2] = 20;
		ArrayValue[3] = 80;
	}
	// OLED固定显示
	OLED_ShowString(2, 1, "T:");
	OLED_ShowString(3, 1, "H:     %");

	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			if (SetFlag == 0)
			{
				SetFlag = 1;
				SetPlace = 0;
				OLED_Clear();
			}
			else if (SetFlag == 1)
			{
				// 将设置好的数值写入存储器
				W25Q64_SectorErase(0x000000);
				W25Q64_PageProgram(0x000000, ArrayValue, 4);
				// 显示设置成功
				OLED_ShowString(4, 1, "SET OK");
				// 延时显示“SET OK”
				Delay_ms(2000);

				SetFlag = 0;
				OLED_Clear();
				OLED_ShowString(2, 1, "T:");
				OLED_ShowString(3, 1, "H:     %");
			}
		}
		switch (SetFlag)
		{
		case 0:
			ValueShow();
			break;
		case 1:
			ValueSet();
			break;
		default:
			break;
		}
	}
} // 所以文件中最后一行必须是空行，否则会报警告: last line of file ends without a newline

// 补上闪烁的定时程序
