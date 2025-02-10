#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	Serial_Init();

	while (1)
	{
	}
}
