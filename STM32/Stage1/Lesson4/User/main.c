#include <stm32f10x.h>

void Delay(unsigned int nCount);

int main(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	// 使能GPIOA外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 初始化GPIOA, 将PA1~PA4设置为通用推挽输出模式, 工作频率50MHz
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA1~PA4输出低电平, LED1~LED4熄灭
	GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_RESET);
	
	while (1)
	{
		// PA1~PA4输出高电平, LED1~LED4点亮
		GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_SET);
		Delay(0xFFFFF);
		
		// PA1~PA4输出低电平, LED1~LED4熄灭
		GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_RESET);
		Delay(0xFFFFF);
	}
}

void Delay(unsigned int nCount)
{
	while (nCount--);
}
