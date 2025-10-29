typedef unsigned int* puint;

void Delay(unsigned int nCount);

int main(void)
{
	volatile puint RCC_APB2ENR = (puint)0x40021018;	// RCC_APB2ENR = 0x40021000 + 0x18
	volatile puint GPIOA_CRL = (puint)0x40010800;	// GPIOA_CRL = 0x40010800 + 0x00
	volatile puint GPIOA_ODR = (puint)0x4001080C;	// GPIOA_ODR = 0x40010800 + 0x0C
	
	// 使能GPIOA外设时钟
	*RCC_APB2ENR |= (1 << 2);				// Set RCC_APB2ENR[2] = 1
	
	*GPIOA_CRL &= 0xFFF0000F;				// Set GPIOA_CRL[4:19] = 0000 0000 0000 0000
	// 将PA1~PA4设置为通用推挽输出模式, 工作频率50MHz
	*GPIOA_CRL |= 0x00033330;				// Set GPIOA_CRL[4:19] = 0011 0011 0011 0011
	
	// PA1~PA4输出低电平, LED1~LED4熄灭
	*GPIOA_ODR &= ~((1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
	
	while (1)
	{
		// PA1~PA4输出高电平, LED1~LED4点亮
		*GPIOA_ODR |= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);
		Delay(0xFFFFF);
		
		// PA1~PA4输出低电平, LED1~LED4熄灭
		*GPIOA_ODR &= ~((1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
		Delay(0xFFFFF);
	}
}

void Delay(unsigned int nCount)
{
	while (nCount--);
}
