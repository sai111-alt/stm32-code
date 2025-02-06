#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "PWMI.h"

int main(void)
{
	OLED_Init();
	PWM_Init();
	PWMI_Init();

	OLED_ShowString(1, 1, "Freq:00000Hz");
	OLED_ShowString(2, 1, "Duty:00%");

	PWM_SetPrescaler(47185 - 1);
	/*  Freq = 72 000 000 / (PSC+1) / 100，注意这里你设置的测频下限是15hz，低于这个下限会出现很大的误差
	按照15hz来算，应该可以设置为48000-1，这样理论测出来刚好就是15hz，但是实际极限是47185-1，到了47186-1就会出现测量波动
	一个是误差，一个是测量的精度*/
	PWM_SetCompare1(80); // Duty = CCR / 100

	while (1)
	{
		OLED_ShowNum(1, 6, PWMI_GetFreq(), 5);
		OLED_ShowNum(2, 6, PWMI_GetDuty(), 2);
	}
}
// 所有文件中最后一行必须是空行，否则会报警告: last line of file ends without a newline
