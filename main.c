#include "main.h"
#include "uart.h"
#include "LCD12864_S.h"
#include "DS18B20.h"

u8  form[7];  //�Լ����е�·�ɱ�
code unsigned char event_zw[14]={0xF3,'a','5','a','5',0xF3,1,0xF6,0xF6,0xF6,0xF6,0xF6,0xFC,0};//��������ָ��

bit zw_success;   //�����ɹ�ָ��
bit flag_back ;  //�Ƿ���δ�������Ϣ

u8  event[14]; //�豸�� 2 0x50
u8 *p=NULL;
u8 xdata q[]="a200FF";  //

#define TEMP_MAX 25
#define CO_MAX 30
#define Tip_max 40  //������������

//�������
unsigned long  Time_Cont = 0;       //��ʱ��������

unsigned char state = 0;
unsigned char state_pre1 = 0;
unsigned char state_pre2 = 0;

unsigned int Temp_Buffer = 0;
unsigned char key_num = 0;
bit flag_sw = 1;
unsigned int ADC_Buffer = 0;	

#define S1_S0 0x40
#define S1_S1 0x80

void main()
{
	u8 tip=0;
	unsigned char	i=0;
	
	ADC_CONTR = ADC_360T | ADC_ON;
	
	//AUXR1 |= ADRJ;									//ADRJ = 1;			//10bitAD�Ҷ���
	CLK_DIV |= 0x20;          //10bitAD�Ҷ���
	
	ACC = P_SW1;
	ACC &= ~(S1_S0|S1_S1);
	ACC |= S1_S0;   //  ?????   P3.6RXD P3.7TXD
	P_SW1 = ACC;
	

  Uart_Init();

	LcmInit();								//Һ����ʼ��
	LcmClearTXT();							//�����ʾ
	LcmClearBMP();
  delay_ms(100);
	PutStr(0,0,"������ʼ���С���");	

  event[3]= 0x02;            //��4λ�豸��Ϊ 0x02

	Temp_Buffer = Get_temp();
	delay_ms(1000);
	LcmClearTXT();							//�����ʾ

	while(1)
	{	
		Temp_Buffer = Get_temp();
    PutStr(0,0,"�¶ȣ�");	
	
 		if( Temp_Buffer/1000 != 0 )					//�����һλΪ0��������ʾ
 		{
 			WriteData(Temp_Buffer/1000+0X30);	   //��ʾ�¶Ȱ�λֵ
 		}
 		else
 		{
 			if(flag_temper == 1)						//�����¶ȱ�־λ��ʾ�¶�����
 			{
 				WriteData('-');
 			}
 			else
 			{
 				WriteData(' ');	
 			}	   
 		}
 			
 		WriteData(Temp_Buffer%1000/100+0X30);	   //��ʾ�¶�ʮλֵ
 		WriteData(Temp_Buffer%100/10+0X30);	   //��ʾ�¶ȸ�λֵ
 		WriteData('.');						   //��ʾС����
 		WriteData(Temp_Buffer%10+0X30);		   //��ʾ�¶�ʮ��λֵ
 		WriteData(' ');
		PutStr(0,7,"��");
                                                      //    flag_temper = 0;    Temp_Buffer=560;
		if(flag_temper == 0)
		{
			if(Temp_Buffer/10 >= TEMP_MAX)
			{
				state = 1;
				PutStr(1,0,"�¶ȳ���");				
			}
			else
			{
				state = 0;
				PutStr(1,0,"�¶Ⱥ���");
			}

			if(Temp_Buffer/10 <= TEMP_MAX - 1) 	//���ڱ���ֵ1�Ⱥ��Ͷ��Ź��ܲŸ�λ����ֹ��ͣ�ķ�����
			{
				state_pre1 = 0;
			}
		}

		if(state == 1&& state_pre1 == 0)
		{
			i=10;
			tip=0;      //���ʹ���  ����
			event[4]=Temp_Buffer/10;
			while(state_pre1==0){
			i=10;
			SendString(event);
			tip++;      //ͳ�Ʒ��ʹ���
			if(tip>=Tip_max) {state_pre1 = 1; break;}  //���� ����
		  delay_ms(50);
			while(i--)
			{
				if(strstr(Rec_Buf,"a2FF")!=NULL){      //�豸�� a2 0x50
	        CLR_Buf();
		      state_pre1 = 1;
		      break;
		    }
			}
			if(state_pre1 == 1) break;
			CLR_Buf();
			}	
		}

		
		ADC_Buffer = adc10_start(1);		// P1.1 ADC
    PutStr(2,0,"����Ũ�ȣ�");	
		ADC_Buffer = ADC_Buffer*100/1024;
		WriteData(ADC_Buffer%100/10+0X30);
		WriteData(ADC_Buffer%10+0X30);
		                                                  //     ADC_Buffer =57;
			if(ADC_Buffer >= CO_MAX)
			{
				state = 1;
				PutStr(3,0,"����Ũ�ȳ���");
			}
			else
			{
				state = 0;
				PutStr(3,0,"����Ũ�Ⱥ���");
			}

			if(ADC_Buffer <= TEMP_MAX - 10) 	//���ڱ���ֵ1�Ⱥ��Ͷ��Ź��ܲŸ�λ����ֹ��ͣ�ķ�����
			{
				state_pre2 = 0;
			}
//	/*	
			if(state == 1&& state_pre2 == 0)
		{ 
			i=10;
			tip=0;      //���ʹ���  ����
			event[4]=ADC_Buffer+100;    //����100  ����COŨ��
			while(state_pre2==0){
			i=10;
			SendString(event);
			tip++;      //ͳ�Ʒ��ʹ���
			if(tip>=Tip_max) { state_pre2 = 1; break;}  //���� ����
		  delay_ms(50);
			while(i--)
			{
				if(strstr(Rec_Buf,"a2FF")!=NULL){      //�豸�� a2 0x50
	        CLR_Buf();
		      state_pre2 = 1;
		      break;
		    }
			}
			if(state_pre2 == 1) break;
			CLR_Buf();
			}		
		}		
//  */
		p=strstr(Rec_Buf,"X");
		 if(p!=NULL){
		 	if(p-Rec_Buf<90){	
					if(*(p+8)=='Y'){
							if(*(p+7)=='1'&&*(p+5)=='0'){
										
											if(*(p+1)=='0'&&*(p+2)=='0'&&*(p+3)==2)			//����Ƿ��Ǹ��豸
										{
											      q[0]=*(p+5);
											      q[1]=*(p+6);
											      SendString(q);  //����ֵ
					                  if(*(p+4)==2)			//�����¶�
													{
																tip=0;      //���ʹ���  ����
																Temp_Buffer = Get_temp();     //��ȡ�¶�
																state_pre1 = 0;
																i=10;
																event[4]=Temp_Buffer/10;
																while(state_pre1==0){
																		i=10;
																		SendString(event);
																		tip++;      //ͳ�Ʒ��ʹ���
												            if(tip>=Tip_max) { state_pre1 = 1; break;}  //���� ����
																		delay_ms(50);
																		while(i--)
																	 {
																			if(strstr(Rec_Buf,"a2FF")!=NULL){      //�豸�� a2 0x50
																				CLR_Buf();
																				state_pre1 = 1;
																				break;
																			}
																		}
																		if(state_pre1 == 1) break;
																		CLR_Buf();
																}	
													}	else if(*(p+4)==1)	//����COŨ��		
													{
																tip=0;      //���ʹ���  ����
																ADC_Buffer = adc10_start(1);
														    ADC_Buffer = ADC_Buffer*100/1024;
																state_pre1 = 0;
																i=10;
																event[4]=ADC_Buffer+100;
																while(state_pre1==0){
																		i=10;
																		SendString(event);
																	  tip++;      //ͳ�Ʒ��ʹ���
												            if(tip>=Tip_max) { state_pre1 = 1; break;}  //���� ����
																		delay_ms(50);
																		while(i--)
																	 {
																			if(strstr(Rec_Buf,"a2FF")!=NULL){      //�豸�� a2 0x50
																				CLR_Buf();
																				state_pre1 = 1;
																				break;
																			}
																		}
																		if(state_pre1 == 1) break;
																		CLR_Buf();
																}	
													}	
										}		
								}
			 	}
			 }
		} 
	//	delay_ms(500);

	}
}


//****************************************************
//��һ��ADCת��
//****************************************************
unsigned int adc10_start(unsigned char channel)	//channel = 0~7
{
	unsigned int	adc;
	unsigned char	i;

	P1ASF = (1 << channel);			//12C5A60AD/S2ϵ��ģ������(AD)ѡ��

	ADC_RES = 0;
	ADC_RESL = 0;

	ADC_CONTR = (ADC_CONTR & 0xe0) | ADC_START | channel;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	 

//	for(i=0; i<250; i++)		//13T/loop, 40*13=520T=23.5us @ 22.1184M
	i = 250;
	do{
		if(ADC_CONTR & ADC_FLAG)
		{
			ADC_CONTR &= ~ADC_FLAG;				//�������ADC_FLAG
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			adc = 0;
			adc = (ADC_RES << 8) | ADC_RESL;	//ADRJ_enable()


			return	adc;
		}
	}while(--i);
	return	1024;
}





