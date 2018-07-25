#ifndef __UART_H__
#define __UART_H__

//#include "STC12C5A.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <STC15F2K60S2.h>

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

#define Success 1U
#define Failure 0U

//函数或者变量声明
void Uart_Init();
void SendData(unsigned char ch);
void SendString(char *s);
bit Hand(unsigned char *a);
void CLR_Buf(void);
void delay_ms(unsigned int n);

extern u8 xdata  Rec_Buf[];  //串口数据缓存
extern u8 point1;             //绶存指针

extern u8 point2;
extern u8 xdata KIND;     //收到的信息种类
extern u16 counter1;
extern bit busy;
extern bit zw_success;
sbit LED =P5^5 ;   //信号输出




#endif