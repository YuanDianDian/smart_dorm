#include "uart.h"

#define FOSC 11059200L  //����
#define BAUD 115200     //������
#define Uart1_Buf_Max 100               //�������ݻ��泤��
u8 xdata  Rec_Buf[Uart1_Buf_Max];  //�������ݻ���
u8 point1 = 0;             //練�ָ��
u8 point2 = 0;
u8 xdata KIND = 0;

void Uart_Init()	//115200@11.0592M				  			   
{
	PCON &= 0x7F;		//�����ʲ�����
	/*
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xFD;		//�趨��ʱ��ֵ
	TH1 = 0xFD;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	EA   = 1;     //��ȫ���жϿ���
	ES   = 1;     //���п��ж�
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
���ʹ�������
----------------------------*/
void SendData(unsigned char ch)
{
    SBUF = ch;                 //д���ݵ�UART���ݼĴ���
		while(TI == 0);
		TI = 0;
}

/*----------------------------
�����ַ���
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        SendData(*s++);         //���͵�ǰ�ַ�
    }
}

bit Hand(unsigned char *a)                   // ��������ʶ����
{ 
    if(strstr(Rec_Buf,a)!=NULL)
	    return 1;
	else
		return 0;
}

void CLR_Buf(void)                           // ���ڻ�������
{
	memset(Rec_Buf, 0, Uart1_Buf_Max);      //���

    point1 = 0;                    
}

void RECEIVE_DATA(void) interrupt 4 using 1         //�����ж�	  		
{    
	ES = 0;
	if (RI)
    {
      	RI = 0;                                //���RIλ
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

unsigned int sendCommand(char *Command, char *Response)  //���ͱ�׼ָ��(���˱��㷨)
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


//****************************************************
//MS��ʱ����(12M�����²���)
//****************************************************
void delay_ms(unsigned int n)
{
	unsigned int  i,j,m;
	for(m = 0 ; m < 10 ; m++)
		for(i=0;i<n;i++)
			for(j=0;j<123;j++);
}
















