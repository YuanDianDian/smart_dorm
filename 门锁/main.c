#include "usart.h"

//  u8 *p=NULL;
u8 xdata q[]="a300FF\r\n";  //�豸�� 00
u8 xdata event[14];   
u8 xdata form[7];  //�Լ����е�·�ɱ�
code unsigned char event_zw[14]={0xF3,'a','3','a','3',0xF3,6,0xF6,0xF6,0xF6,0xF6,0xF6,0xFC,0};//��������ָ��

bit zw_success;   //�����ɹ�ָ��
unsigned char count;      //0.5ms������ʶ
sbit pwm =P1^1 ;          //PWM�ź����
unsigned char jd;         //�Ƕȱ�ʶ
WORD value;
bit flag_back ;  //�Ƿ���δ�������Ϣ

void PCA_init();
#define THz (FOSC/12/4000) 

void main()
{
	flag_back = 0; 
	START:
	USART_Init(); 
	event[14]=0;  form[7]=0; 	jd=5;
	PCA_init();  //�����ʼ��
	 
 
	//strcpy(event,event_zw);   
	if (sendCommand(event_zw, "a3FF") == Success);
	else goto START;  //����
  //	delay_ms(10);                             
	while(counter1<300)  //�ȴ�1����
	{  
    if(KIND==4){ 
			if(point2>11&&Rec_Buf[point2-12]==0xf4){
					if(Rec_Buf[point2-12+3]=='a'&&Rec_Buf[point2-12+4]=='3'\
						&&(Rec_Buf[point2-12+6]==12||Rec_Buf[point2-12+Rec_Buf[point2-12+6]]==0xf6)){   //�ܵ��ܻ����͵�����ͬ��ָ��
							Rec_Buf[point2]=0;
							strcpy(form,Rec_Buf+point2-12+6);   //ȡ��·�ɱ�
							q[2]=Rec_Buf[point2-12+1];
							q[3]=Rec_Buf[point2-12+2];
						  CLR_Buf();
					  	SendString(q+2);     //���ر�ʶ  �豸��+FF
						  zw_success=1;       //�����ɹ�
						  TR0 = 0;   //ָʾ�Ʋ�����
						  LED = 1;   break;
					} 
			}	
		}  
	} 
	if(zw_success==0) goto START;  //����           

  if(flag_back!=0){
		if (sendCommand(event, "a3FF") == Success);
	   else goto START;  //����
	  flag_back = 0;
		CLR_Buf();
	}  
     while(1)
	{
		switch(KIND){
			case 2:  //�յ�����ָ��
//			if(zw_success==0) break;
				if(point2>11&&Rec_Buf[point2-12]==0xf2\
						&&(Rec_Buf[point2-12+6]==12||Rec_Buf[point2-12+Rec_Buf[point2-12+6]]==0xf6)){
						if(Rec_Buf[point2-12+3]=='a'&&Rec_Buf[point2-12+4]=='3'&&Rec_Buf[point2-12+5]==1){
              CLR_Buf();
							q[2]=Rec_Buf[point2-12+1];
							q[3]=Rec_Buf[point2-12+2];
							SendString(q+2);  //���ر�ʶ  �豸��+FF
							jd=13;
					}				
				}			
			KIND = 0;
			break;	
				
			case 4:  //�յ���������ָ��
		
			if(point2>11&&Rec_Buf[point2-12]==0xf4\
					&&(Rec_Buf[point2-12+6]==12||Rec_Buf[point2-12+Rec_Buf[point2-12+6]]==0xf6)){
					if(Rec_Buf[point2-12+3]=='a'&&Rec_Buf[point2-12+4]=='3'){      //�ܵ��ܻ����͵�����ͬ��ָ��
							Rec_Buf[point2]=0;
							strcpy(form,Rec_Buf+point2-12+6);   //ȡ��·�ɱ�
						  q[2]=Rec_Buf[point2-12+1];
							q[3]=Rec_Buf[point2-12+2];
							SendString(q+2);     //���ر�ʶ  �豸��+FF
						  zw_success=1;       //�����ɹ�
						  TR0 = 0;   //ָʾ�Ʋ�����
						  LED = 1;
					} 
			}	
		
			KIND = 0;
			break;	
			
			case 5:  //����ָ��
//		if(zw_success==0) break;
				
			KIND = 0;
			break;	
		}
		if(jd==13){ 
			delay_ms(5000);	
			jd=5;
			strcpy(event,event_zw); 
			event[5] = 1;     //���˿���
			event[6] = 0;
			strcat(event,form);  //ճ����ȥ·�ɱ�
			event[6]--;
			
			event[12] = 0xfe;
			event[0] = 0xf1;       //ת��������ָ��
			
			if (sendCommand(event, "a3FF") == Success);
	    else { 
				 flag_back=1;
				 goto START; 
			}
			CLR_Buf();
		}
	}
}
void PCA_isr() interrupt 7 using 2  //????,?????????,???using
{
	CCF0=0;
	CCAP0L=value;
	CCAP0H=value>>8;
	value += THz;
    if(count< jd)
        pwm=1;
    else
        pwm=0;
    count++;
    if(count==80) count=0;
}

void PCA_init()     //??????????P805?
{
	CCON=0;

	CL=0;
	CH=0;
	CMOD=0x00;

	value=THz;                  //??
	CCAP0L=value;
	CCAP0H=value>>8;
	value+=THz;
	CCAPM0=0x49;
	
	CR=1;
	EA=1;
}