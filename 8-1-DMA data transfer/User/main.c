#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "MyDMA.h"

uint8_t DataA[] = {0x01, 0x02, 0x03, 0x04};
uint8_t DataB[4] = {0};

int main(void)
{
	OLED_Init();
	MyDMA_Init((uint32_t)DataA, (uint32_t)DataB, 4);

	OLED_ShowString(1, 1, "DataA");
	OLED_ShowString(3, 1, "DataB");
	OLED_ShowHexNum(1, 8, (uint32_t)DataA, 8);
	OLED_ShowHexNum(3, 8, (uint32_t)DataB, 8);

	OLED_ShowHexNum(2, 1, DataA[0], 2);
	OLED_ShowHexNum(2, 4, DataA[1], 2);
	OLED_ShowHexNum(2, 7, DataA[2], 2);
	OLED_ShowHexNum(2, 10, DataA[3], 2);
	OLED_ShowHexNum(4, 1, DataB[0], 2);
	OLED_ShowHexNum(4, 4, DataB[1], 2);
	OLED_ShowHexNum(4, 7, DataB[2], 2);
	OLED_ShowHexNum(4, 10, DataB[3], 2);

	while (1)
	{
		DataA[0]++;
		DataA[1]++;
		DataA[2]++;
		DataA[3]++;

		OLED_ShowHexNum(2, 1, DataA[0], 2);
		OLED_ShowHexNum(2, 4, DataA[1], 2);
		OLED_ShowHexNum(2, 7, DataA[2], 2);
		OLED_ShowHexNum(2, 10, DataA[3], 2);
		OLED_ShowHexNum(4, 1, DataB[0], 2);
		OLED_ShowHexNum(4, 4, DataB[1], 2);
		OLED_ShowHexNum(4, 7, DataB[2], 2);
		OLED_ShowHexNum(4, 10, DataB[3], 2);

		Delay_ms(1000);

		MyDMA_Transfer();

		OLED_ShowHexNum(2, 1, DataA[0], 2);
		OLED_ShowHexNum(2, 4, DataA[1], 2);
		OLED_ShowHexNum(2, 7, DataA[2], 2);
		OLED_ShowHexNum(2, 10, DataA[3], 2);
		OLED_ShowHexNum(4, 1, DataB[0], 2);
		OLED_ShowHexNum(4, 4, DataB[1], 2);
		OLED_ShowHexNum(4, 7, DataB[2], 2);
		OLED_ShowHexNum(4, 10, DataB[3], 2);

		Delay_ms(1000);
	}
}

// const uint8_t aa = 0x66;
/* 加上const就是将其定义成了常量F，而非F变量，也就是说后期无法更改
只能读，不能写，是存在Flash区中的，一般对于查找表或者字库这种大量
无需更改的量，我们常将它们放在Flash区中以节省SRAM的空间 */
// OLED_ShowHexNum(1, 1, aa, 2);
// OLED_ShowHexNum(2, 1, (uint32_t)&aa, 8);
/* 要直接显示地址的话需要强转，因为地址都是给指针变量的，而不
能直接当作数值赋值给函数，所以需要强转，并且32位的系统地址都是32位的
所以需要强转为32位整型变量 */
// OLED_ShowHexNum(3, 1, (uint32_t)&ADC1->DR, 8);
/* 显示ADC1的DR（Data Register）数据寄存器地址 */
