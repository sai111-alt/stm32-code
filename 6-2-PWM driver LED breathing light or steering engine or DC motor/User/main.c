#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "PWM_Steering_Engine.h"
#include "Key.h"

uint8_t KeyNum = 0;
float Angle = 0;

int main(void)
{
	OLED_Init();
	PWM_SE_Init();
	Key_Init();

	OLED_ShowString(1, 1, "Angle:");

	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			Angle += 30;
			if (Angle > 180)
			{
				Angle = 0;
			}
		}
		SE_SetAngle(Angle);
		OLED_ShowNum(1, 7, Angle, 3);
	}
} // 所有文件中最后一行必须是空行，否则会报警告: last line of file ends without a newline
