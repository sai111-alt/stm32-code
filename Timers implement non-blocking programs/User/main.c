#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "Timer.h"

uint8_t KeyNum = 0;
uint8_t LED1Mode = 0;
uint8_t LED2Mode = 0;

uint16_t i = 0;

int main(void)
{
	OLED_Init();
	Key_Init();
	LED_Init();
	Timer_Init();

	OLED_ShowString(1, 1, "i:");
	OLED_ShowString(2, 1, "LED1Mode:");
	OLED_ShowString(3, 1, "LED2Mode:");

	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			LED1Mode++;
			LED1Mode %= 5;
			LED1_SetMode(LED1Mode);
		}
		if (KeyNum == 2)
		{
			LED2Mode++;
			LED2Mode %= 5;
			LED2_SetMode(LED2Mode);
		}
		OLED_ShowNum(1, 3, i++, 5);
		OLED_ShowNum(2, 10, LED1Mode, 1);
		OLED_ShowNum(3, 10, LED2Mode, 1);
	}
} // 所有文件中最后一行必须是空行，否则会报警告: last line of file ends without a newline

void TIM2_IRQHandler(void)
{
	// 判断是否是更新中断触发的
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		IndependentKey_Loop();
		LED_Loop();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 中断标志为1会进入中断，避免卡死，每次执行完中断函数需手动清除其标志位
	}
}
