#include "usart.h"

#define Uart1_Buf_Max 100               //�������ݻ��泤��
u8 xdata Rec_Buf[Uart1_Buf_Max];     //�������ݻ���
//u8 xdata  Flag[]="X";   //��־λ��������
u8 point1 = 0;                          //����ָ��
bit busy;
u8 point2 = 0;
u8 xdata KIND = 0;
//  sbit LED =P5^5 ;        //�ź����
u8 counter;
WORD counter1;

void USART_Init()
{
	ACC = P_SW1;
	ACC &= ~(S1_S0|S1_S1);
	ACC |= S1_S0;   //  ����һ��Ϊ   P3.6RXD P3.7TXD
	P_SW1 = ACC;
	
    SCON=0x50;
    T2L=(65536-(FOSC/4/BAUD));
    T2H=(65536-(FOSC/4/BAUD))>>8;
    AUXR=0X14;                        //
    AUXR|=0X01;
    ES=1;
    EA=1;
	
		TMOD = 0x01;           //��ʱ�� 0  
    IE   |= 0x82;
    TH0  = 0xc4;
    TL0  = 0x00;           //  18.432m����  ,10ms
    TR0=1;  
	
	  counter1=0;
	  zw_success=0;
 } 

void Time0_Int() interrupt 1 //��ʱ��0�жϳ���
{
		TH0  = 0xc4;             //���¸�ֵ
		TL0  = 0x00;
		counter++;
		if(counter==20){      //400ms  ��һ��
			LED = ~LED;
			counter = 0;
			counter1++;
		}
}
 
void Uart() interrupt 4 using 1
{
	ES=0;
    if (RI) {
        RI=0;
				Rec_Buf[point1] = SBUF; 
		switch(Rec_Buf[point1]){         //�жϳ�������ָ������  ����point2��¼��ֹ�ڻ��������λ��    
			case 0xFE:  //�յ�����ָ��
			point2=point1;	
			KIND = 1;
			break;	
			
			case 0xFD:  //�յ�����ָ��
			point2=point1;	
			KIND = 2;
			break;	
			
			case 0xFC:  //�յ���������ָ��
			point2=point1;	
			KIND = 3;
			break;	
			
			case 0xFB:  //�յ���������ָ��
			point2=point1;	
			KIND = 4;
			break;	
			
			case 0xFA:  //����ָ��
			point2=point1;	
			KIND = 5;
			break;	
			
			case 0xFF:  //�յ�����ָ��
			jd=13;
			break;
		}			
		    point1++;               
		    if(point1>=Uart1_Buf_Max-1)          
		    {
			    point1 = 0;
		    } 
  }
     if(TI)
     {
       TI=0;
       busy=0;
     }
	ES=1;
}

void SendData(BYTE dat)
{
  while(busy);
    ACC=dat;
    busy=1;
    SBUF=ACC;
}

void SendString(char*s)
 {
     while (*s)
     {
           SendData(*s++);
  }
}
 
void CLR_Buf(void)                           // �������
{
	memset(Rec_Buf, 0, Uart1_Buf_Max);      //

    point1 = 0;                    
}

void delay_ms(unsigned int n)
{
	unsigned int  i,j,m;
	for(m = 0 ; m < 10 ; m++)
		for(i=0;i<n;i++)
			for(j=0;j<123;j++);
}

 //   void soft_reset(void)	 //����ָ�� 
 //  {
 //      ((void (code *) (void)) 0x0000) ();
 //  }


unsigned int sendCommand(char *Command, char *Response)  //���ͱ�׼ָ��(���˱��㷨)
{
	unsigned char n,Time_Cont;
 	CLR_Buf();  KIND = 0;
	for (n = 0; n <10; n++)
	{
		SendString(Command); 		//???? 
		delay_ms(100);
		Time_Cont = 0;
		while (Time_Cont < 10)
		{
			delay_ms(1);
			Time_Cont += 1;
			if (strstr(Rec_Buf, Response) != NULL)
			{
				
//				CLR_Buf();
				return Success;
			}
			
		}
		Time_Cont = 0;
	}
	for (; n <16; n++)
	{
		delay_ms(rand()%1024*50);   //ʵ���˱�
		SendString(Command); 		//����ָ�� 
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