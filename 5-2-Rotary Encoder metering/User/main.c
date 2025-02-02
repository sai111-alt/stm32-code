#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"

int main(void)
{
	OLED_Init();
	CountSensor_Init();

	OLED_ShowString(1, 1, "Count:");
	while (1)
	{
		OLED_ShowNum(2, 1, CountSensor_Get(), 5);
	}
} // 所有文件中最后一行必须是空行，否则会报警告: last line of file ends without a newline
