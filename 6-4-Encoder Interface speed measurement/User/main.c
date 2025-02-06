#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Encoder.h"

int16_t Speed = 0;

int main(void)
{
	OLED_Init();
	Timer_Init();
	Encoder_Init();
	OLED_ShowString(1, 1, "Speed:");
	while (1)
	{
		OLED_ShowSignedNum(1, 7, Speed, 5);
	}
} // 所有文件中最后一行必须是空行，否则会报警告: last line of file ends without a newline

void TIM2_IRQHandler(void)
{
	// 判断是否是TIM2的中断触发的
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Speed = Encoder_Get();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 中断标志为1会进入中断，避免卡死，每次执行完中断函数需手动清除其标志位
	}
}
