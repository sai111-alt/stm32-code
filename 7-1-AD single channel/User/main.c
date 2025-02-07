#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue = 0;
float Voltage = 0;

int main(void)
{
	OLED_Init();
	AD_Init();

	OLED_ShowString(1, 1, "ADValue:");
	OLED_ShowString(2, 1, "Volatge:0.00V");
	while (1)
	{
		ADValue = ADC_GetValue();
		Voltage = (float)ADValue / 4095 * 3.3;
		/* 因为ADValue是整数，就是整数/整数=整数，会自动舍弃小数，所以需要先对
		ADValue强转为float，则浮点数/整数=浮点数，不会舍弃小数 */

		OLED_ShowNum(1, 9, ADValue, 4);
		OLED_ShowNum(2, 9, Voltage, 1);
		OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100) % 100, 2);
		/* 浮点数不能取余，需先强转为整数才行 */
		Delay_ms(100);
	}
}
