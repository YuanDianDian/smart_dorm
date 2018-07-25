#include "uart.h"

#define FOSC 11059200L  //晶振
#define BAUD 115200     //波特率
#define Uart1_Buf_Max 100               //串口数据缓存长度
u8 xdata  Rec_Buf[Uart1_Buf_Max];  //串口数据缓存
u8 point1 = 0;             //绶存指针
u8 point2 = 0;
u8 xdata KIND = 0;

void Uart_Init()	//115200@11.0592M				  			   
{
	PCON &= 0x7F;		//波特率不倍速
	/*
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA   = 1;     //打开全局中断控制
	ES   = 1;     //串行口中断
 */	
	
	  SCON=0x50;
    T2L=(65536-(FOSC/4/BAUD));
    T2H=(65536-(FOSC/4/BAUD))>>8;
    AUXR=0X14;                        //
    AUXR|=0X01;
    ES=1;
    EA=1;
	
}


/*----------------------------
发送串口数据
----------------------------*/
void SendData(unsigned char ch)
{
    SBUF = ch;                 //写数据到UART数据寄存器
		while(TI == 0);
		TI = 0;
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}

bit Hand(unsigned char *a)                   // 串口命令识别函数
{ 
    if(strstr(Rec_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}

void CLR_Buf(void)                           // 串口缓存清理
{
	memset(Rec_Buf, 0, Uart1_Buf_Max);      //清空

    point1 = 0;                    
}

void RECEIVE_DATA(void) interrupt 4 using 1         //串口中断	  		
{    
	ES = 0;
	if (RI)
    {
      	RI = 0;                                //清除RI位
		Rec_Buf[point1] = SBUF; 
//		if (Rec_Buf[0] == 0xd9)
//			{
//				IAP_CONTR = 0x60;
//			}
			
		point1++;               
		if(point1>=Uart1_Buf_Max)          
		{
			point1 = 0;
		}           

    }
	ES =  1;

}

unsigned int sendCommand(char *Command, char *Response)  //发送标准指令(用退避算法)
{
	unsigned char n,Time_Cont;
 	CLR_Buf();  KIND = 0;
	for (n = 0; n <10; n++)
	{
		SendString(Command); 		//???? 
		delay_ms(50);
		Time_Cont = 0;
		while (Time_Cont < 10)
		{
			delay_ms(1);
			Time_Cont += 1;
			if (strstr(Rec_Buf, Response) != NULL)
			{
				
	//			CLR_Buf();
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	for (; n <16; n++)
	{
		delay_ms(rand()%1024*50);   //实现退避
		SendString(Command); 		//发送指令 
		delay_ms(50);
		Time_Cont = 0;
		while (Time_Cont < 10)
		{
			delay_ms(1);
			Time_Cont += 1;
			if (strstr(Rec_Buf, Response) != NULL)
			{
				
	//			CLR_Buf();
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	CLR_Buf();
	return Failure;
}


//****************************************************
//MS延时函数(12M晶振下测试)
//****************************************************
void delay_ms(unsigned int n)
{
	unsigned int  i,j,m;
	for(m = 0 ; m < 10 ; m++)
		for(i=0;i<n;i++)
			for(j=0;j<123;j++);
}
















