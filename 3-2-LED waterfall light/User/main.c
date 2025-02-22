#include "stm32f10x.h"
#include "Delay.h"

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 即使能GPIOA口的时钟（就是开启PA口的时钟）

	GPIO_InitTypeDef GPIO_InitStruct;			   // GPIO_InitTypeDef是用typedef重命名的一个结构体，就是专属于GPIO的一个初始化结构体
												   // 该结构体中包含了需要初始化GPIO用到的变量
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  // 定义GPIO的端口模式为推挽输出模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;	   // 定义要使用全部引脚则GPIO_Pin_All，若定义要使用几个引脚，比如0，1，2，那么就GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 或上即可
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 定义该端口的速度，一般没有严格要求，就统一50MHZ即可

	GPIO_Init(GPIOA, &GPIO_InitStruct); // 第一参数选择初试化GPIO哪个口，第二参数选择已经配置好的GPIO初始化结构体指针
	// 综上，就是对GPIOA的16个引脚配置成推挽输出模式，引脚速度设置为50MHZ
	while (1)
	{
		int i = 0;
		for (i = 0; i < 8; i++)
		{
			GPIO_Write(GPIOA, ~(0x0001 << i)); // 即PA1置0，其他置1
			Delay_ms(100);
		}
	}
} // 函数最后一行必须时空行，否则会报警告: last line of file ends without a newline
