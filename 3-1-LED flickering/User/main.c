#include "stm32f10x.h"
#include "Delay.h"

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 即使能GPIOA口的时钟（就是开启PA口的时钟）

	GPIO_InitTypeDef GPIO_InitStruct;			   // GPIO_InitTypeDef是用typedef重命名的一个结构体，就是专属于GPIO的一个初始化结构体
												   // 该结构体中包含了需要初始化GPIO用到的变量
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  // 定义GPIO的端口模式为推挽输出模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;		   // 定义要使用GPIO的哪个引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 定义该端口的速度，一般没有严格要求，就统一50MHZ即可

	GPIO_Init(GPIOA, &GPIO_InitStruct); // 第一参数选择初试化GPIO哪个口，第二参数选择已经配置好的GPIO初始化结构体指针
	// 综上，就是对GPIOA的0号引脚配置成推挽输出模式，引脚速度设置为50MHZ

	// GPIO_ResetBits(GPIOA, GPIO_Pin_0); // 设置GPIOA的0引脚为低电平，其本意是清除该引脚的数据，就是清0操作
	// GPIO_SetBits(GPIOA, GPIO_Pin_0);   // 设置GPIOA的0引脚为高电平，即置1
	// GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET); // 相当于上两个函数的综合，第三参数Bit_RESET就是清0，Bit_SET就是置1
	while (1)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
		Delay_ms(500);
	}
} // 函数最后一行必须时空行，否则会报警告: last line of file ends without a newline