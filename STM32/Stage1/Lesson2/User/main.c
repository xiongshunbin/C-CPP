#define RCC_APB2ENR	(*(volatile unsigned long *)0x40021018)
#define GPIOA_CRL 	(*(volatile unsigned long *)0x40010800)
#define GPIOA_ODR 	(*(volatile unsigned long *)0x4001080C)

void Delay(unsigned int nCount);

int main(void)
{
	// 使能GPIOA外设时钟
	RCC_APB2ENR |= (1 << 2);				// Set RCC_APB2ENR[2] = 1
	
	
	GPIOA_CRL &= 0xFFF0000F;				// Set GPIOA_CRL[4:19] = 0000 0000 0000 0000
	// 将PA1~PA4设置为通用推挽输出模式, 工作频率50MHz
	GPIOA_CRL |= 0x00033330;				// Set GPIOA_CRL[4:19] = 0011 0011 0011 0011
	
	// PA1~PA4输出低电平, LED1~LED4熄灭
	GPIOA_ODR &= ~((1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
	
	while (1)
	{
		// PA1~PA4输出高电平, LED1~LED4点亮
		GPIOA_ODR |= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);
		Delay(0xFFFFF);
		
		// PA1~PA4输出低电平, LED1~LED4熄灭
		GPIOA_ODR &= ~((1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
		Delay(0xFFFFF);
	}
}

void Delay(unsigned int nCount)
{
	while (nCount--);
}
