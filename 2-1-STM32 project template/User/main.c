#include "stm32f10x.h"                  // Device header

int main(void)
{
	RCC->APB2ENR=0x00000010;
	GPIOC->CRH=0x00300000;
	GPIOC->ODR=0X00000000;
	while(1)
	{
		
	}
}//�������һ�б���ʱ���У�����ᱨ����: last line of file ends without a newline
