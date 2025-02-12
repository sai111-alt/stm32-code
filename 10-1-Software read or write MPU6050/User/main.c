#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"

int main(void)
{
	OLED_Init();
	MPU6050_Init();

	while (1)
	{
	}
}
