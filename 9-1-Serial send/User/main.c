#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	Serial_Init();

	// Serial_SendByte('A');

	// Serial_SendString("HelloWorld!\r\n");

	// Serial_SendNumber(12345, 5);

	// printf("Num=%d\r\n", 666);

	// char String[100];
	// sprintf(String, "Num=%d\r\n", 666);
	// Serial_SendString(String);

	Serial_Printf("你好世界");

	while (1)
	{
	}
}
