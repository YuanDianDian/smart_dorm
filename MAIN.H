#ifndef __MAIN_H__
#define __MAIN_H__

//#include "STC12C5A.h"
#include <STC15F2K60S2.h>

#include <intrins.h>

#define ADC_OFF()	ADC_CONTR = 0
#define ADC_ON		(1 << 7)
#define ADC_90T		(3 << 5)
#define ADC_180T	(2 << 5)
#define ADC_360T	(1 << 5)
#define ADC_540T	0

#define ADC_CH0		0
#define ADC_CH1		1
#define ADC_CH2		2
#define ADC_CH3		3
#define ADC_CH4		4
#define ADC_CH5		5
#define ADC_CH6		6
#define ADC_CH7		7

#define ADC_FLAG    0x10        //ADC转换完成标志
#define ADC_START   0x08        //ADC开始转换控制位



//函数或者变量声明

extern unsigned int sendCommand(char *Command, char *Response, unsigned long Timeout, unsigned char Retry);


extern unsigned int adc10_start(unsigned char channel);	//channel = 0~7


#endif