#include "Hardware.h"

int8_t TemHemValue[4] = {0};   // 存放温湿度值，0湿度整数，1湿度度小数，2温度整数，3温度小数
int8_t ArrayValue[4] = {0};	   // 放阈值
uint8_t KeyNum = 0;			   // 存键码
uint8_t SetFlag = 0;		   // 进入阈值设置的标志位，0完成1设置
uint8_t SetPlace = 0;		   // 设置选择位
uint8_t SetPlaceFlashFlag = 0; // 闪烁标志位
uint8_t KeyFlag = 0;		   // 长按标志位

int main(void)
{
	HardWare_Init();

	// 将W25Q64中已经存好的阈值放入，第一次存入的数据自然是空白数据0xFF
	// 因为这里是温湿度是有符号数-127~128，所以0xFF最高位是1，表示负数，应按补码计算原码为-1，所以第一次给的数据就是-1
	W25Q64_ReadData(0X000000, ArrayValue, 4);
	W25Q64_ReadData(0X001000, TemHemValue, 4); // 设备初试化时读出一次原来的实时温湿度数据
	// DAT22测量范围T为-40~80，H为0%~100%
	if (THigh > 80 || TLow < -40 || HHigh > 100 || HLow < 0 ||
		TLow > THigh || HLow > HHigh)
	{
		TLow = 5;
		THigh = 30;
		HLow = 20;
		HHigh = 80;
	}
	// OLED固定显示
	OLED_ShowString(2, 1, "(:");
	OLED_ShowString(3, 1, "):");
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
				// 重新赋值数据
				W25Q64_ReadData(0X000000, ArrayValue, 4);
			}
			else if (SetFlag == 1)
			{
				// 将设置好的数值写入存储器
				W25Q64_SectorErase(0x000000);
				W25Q64_PageProgram(0x000000, ArrayValue, 4);
				// 显示设置成功
				OLED_ShowString(4, 1, "SET OK");
				// 延时显示“SET OK”
				Delay_ms(1500);

				SetFlag = 0;
				OLED_Clear();
				OLED_ShowString(2, 1, "(:");
				OLED_ShowString(3, 1, "):");
			}
		}
		switch (SetFlag)
		{
		case 0:
			ValueJudgeShow(TemHemValue, ArrayValue, &KeyNum);
			DataStorage(TemHemValue);
			break;
		case 1:
			ValueSet(ArrayValue, &KeyNum, &SetPlace, &SetPlaceFlashFlag, &KeyFlag);
			break;
		default:
			break;
		}
	}
} // 所以文件中最后一行必须是空行，否则会报警告: last line of file ends without a newline

// 定时程序
void TIM2_IRQHandler(void)
{
	static unsigned int T0Count1 = 0;
	static unsigned int T0Count2 = 0;
	static unsigned int T0Count3 = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		T0Count1++;
		if (T0Count1 >= 500)
		{
			T0Count1 = 0;
			SetPlaceFlashFlag = !SetPlaceFlashFlag; // 每0.5s翻转一次，对应闪烁程序
		}
		if (Key3 == 0 || Key4 == 0)
		{
			T0Count2++;
			if (T0Count2 >= 800) // 按键长按判断，若超过0.8s则判断长按
			{
				KeyFlag = 1;
			}
		}
		else
		{
			T0Count2 = 0;
			KeyFlag = 0;
		}
		T0Count3++;
		if (T0Count3 >= 20)
		{
			T0Count3 = 0;
			IndependentKey_Loop();
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
