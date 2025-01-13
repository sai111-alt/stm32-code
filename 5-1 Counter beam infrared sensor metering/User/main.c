#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"

int main(void)
{
	OLED_Init();
	OLED_ShowString(1, 1, "HelloWorld");
	while (1)
	{
		;
	}
} // 所有文件中最后一行必须是空行，否则会报警告: last line of file ends without a newline
