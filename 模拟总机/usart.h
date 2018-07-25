#ifndef _USART_H_
#define _USART_H_

#include<STC15F2K60S2.h>
#include<intrins.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char  BYTE;
typedef unsigned int   WORD;
typedef unsigned char  u8;

#define S1_S0 0x40
#define S1_S1 0x80
#define FOSC 18432000L  //晶振
#define BAUD 115200     //波特率
#define Success 1U
#define Failure 0U

extern void SendData(BYTE dat);
extern void SendString(char*s);
extern void CLR_Buf(void);
extern void delay_ms(unsigned int n);
extern void USART_Init();							//串口初始化
extern unsigned int sendCommand(char *Command, char *Response);//发送标准指令
extern void soft_reset(void);	 //重启指令

extern u8 xdata  Rec_Buf[];   //串口数据缓存

// extern u8 xdata  event[];   //数据
// extern u8 xdata  q[];
extern u8 point1;             //缓存指针
extern u8 point2;
extern u8 xdata KIND;     //收到的信息种类
extern bit busy;
extern bit zw_success;
sbit LED =P5^5 ;   //信号输出

#endif