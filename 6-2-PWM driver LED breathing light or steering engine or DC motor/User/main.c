#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "PWM_DC_Motor.h"
#include "Key.h"

uint8_t KeyNum = 0;
int8_t Speed = 0;

int main(void)
{
	OLED_Init();
	PWM_DCM_Init();
	Key_Init();

	OLED_ShowString(1, 1, "Speed:");

	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			Speed += 20;
			if (Speed > 100)
			{
				Speed = -100;
			}
		}
		Motor_SetSpeed(Speed);
		OLED_ShowSignedNum(1, 7, Speed, 3);
	}
}
// 所有文件中最后一行必须是空行，否则会报警告: last line of file ends without a newline
