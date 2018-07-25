#ifndef __DS18B20_H__
#define __DS18B20_H__


//#include "STC12C5A.h"
#include <STC15F2K60S2.h>

//DS18B20 IO设置
// sbit DS18B20_DQ = P3^4;
sbit DS18B20_DQ = P1^0;

//函数或者变量声明
extern void DS18B20_Delay( unsigned int n );
extern void DS18B20_Write_Byte( unsigned char dat);
extern unsigned char DS18B20_Read_Byte( );
extern bit DS18B20_Init();
extern unsigned int Get_temp(void);
extern unsigned char flag_temper;


extern void Delay800us();
extern void Delay40us();
extern void Delay200us();

#endif
