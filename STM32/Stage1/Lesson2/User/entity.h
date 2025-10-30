#ifndef __ENTITY_H__
#define __ENTITY_H__

typedef struct
{
	unsigned int CR;
	unsigned int CFGR;
	unsigned int CIR;
	unsigned int APB2RSTR;
	unsigned int APB1RSTR;
	unsigned int AHBENR;
	unsigned int APB2ENR;
	unsigned int APB1ENR;
	unsigned int BDCR;
	unsigned int CSR;
	unsigned int AHBRSTR;
	unsigned int CFGR2;
}RCC_Typedef;

typedef struct
{
	unsigned int CRL;
	unsigned int CRH;
	unsigned int IDR;
	unsigned int ODR;
	unsigned int BSRR;
	unsigned int BRR;
	unsigned int LCKR;
}GPIOA_Typedef;

#define RCC ((RCC_Typedef*)0x40021000)
#define GPIOA ((GPIOA_Typedef*)0x40010800)

#endif
