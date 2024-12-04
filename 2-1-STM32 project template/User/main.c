#include "stm32f10x.h" // Device header

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_initStructure;
	GPIO_initStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_initStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_initStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_13); // 将Pin_13置为高电平
	// GPIO_ResetBits(GPIOC, GPIO_Pin_13); // 将Pin_13置为低电平
	while (1)
	{
		;
	}
} // 函数最后一行必须时空行，否则会报警告: last line of file ends without a newline
