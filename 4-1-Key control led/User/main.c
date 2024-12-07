#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"
#include "key.h"

uint8_t KeyNum = 0;

int main(void)
{
	LED_Init();
	Key_Init();
	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			LED1_Turn();
		}
		if (KeyNum == 2)
		{
			LED2_Turn();
		}
	}
} // 所以文件中最后一行必须是空行，否则会报警告: last line of file ends without a newline
