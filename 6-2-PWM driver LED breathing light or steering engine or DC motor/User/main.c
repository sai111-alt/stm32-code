#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"

uint16_t i = 0;

int main(void)
{
	OLED_Init();
	PWM_Init();

	while (1)
	{
		for (i = 0; i <= 100; i++)
		{
			PWM_SetCompare1(i);
			Delay_ms(10);
		}
		for (i = 0; i <= 100; i++)
		{
			PWM_SetCompare1(100 - i);
			Delay_ms(10);
		}
	}
} // 所有文件中最后一行必须是空行，否则会报警告: last line of file ends without a newline
