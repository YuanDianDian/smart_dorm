#include "usart.h"

  u8 *p=NULL;
u8 xdata q[]="0000FF\r\n";  //�豸�� 00
u8 xdata event[14];   
u8 xdata form[7];  //�Լ����е�·�ɱ�
code unsigned char event_zw[14]={0xF3,'0','0','0','0',0xF3,6,0xF6,0xF6,0xF6,0xF6,0xF6,0xFC,0};//��������ָ��

bit zw_success;   //�����ɹ�ָ��

//#define Tip_max 40  //������������

u8 flag_back ;  //�Ƿ���δ�������Ϣ

void main()
{
	unsigned char abc;
	flag_back = 0; 
	START:
	USART_Init(); event[14]=0;  form[7]=0;
	
	for(abc=0;abc<100;abc++)
	{
		Rec_Buf[abc]=0xaa;
	}
 
	//strcpy(event,event_zw);   
	if (sendCommand(event_zw, "00FF") == Success);
	else goto START;  //����
//	delay_ms(10);                             
	while(counter1<300)  //�ȴ�1����
	{  
    if(KIND==4){ 
			if(point2>11&&Rec_Buf[point2-12]==0xf4&&Rec_Buf[point2-12+Rec_Buf[point2-12+6]]=='0'){
				if(2>1){  //ȷ���յ���������ָ��
					if(Rec_Buf[point2-12+3]=='0'&&Rec_Buf[point2-12+4]=='0'){   //�ܵ��ܻ����͵�����ͬ��ָ��
							Rec_Buf[point2]=0;
							strcpy(form,Rec_Buf+point2-12+6); CLR_Buf();  //ȡ��·�ɱ�
						//	form[0] -= 1;
						  strcpy(event,event_zw);  
							q[2]=event[1];
							q[3]=event[2];
							event[1]=0x30;
							event[2]=0x30;
							SendString(q+2);     //���ر�ʶ  �豸��+FF
						 
						  event[5] = 0xF6;     //�����ܻ������յ�ͬ��֪ͨ
							event[6] = 0;
						  strcat(event,form);  //ճ����ȥ·�ɱ�
							event[12]=0xFC;
						  event[1]=0xF3;       //ת������������ָ��
						//  if (sendCommand(event, "00FF") == Success);
	          //  else goto START;  //����	
	            delay_ms(10);
						  zw_success=1;       //�����ɹ�
						  TR0 = 0;   //ָʾ�Ʋ�����
						  LED = 1;   break;
					} 
				}	
			}	
		}  
	}  // zw_success=1; LED = 1;   TR0 = 0;
	if(zw_success==0) goto START;  //����           

  if(flag_back!=0){
		if (sendCommand(event, "00FF") == Success);
	   else goto START;  //����
	  flag_back = 0;
		CLR_Buf();
	}   

     while(1)
	{
		switch(KIND){
			case 1:  //�յ�����ָ��
//			if(zw_success==0) break;
				if(point2>11&&Rec_Buf[point2-12]==0xf1&&Rec_Buf[point2-12+Rec_Buf[point2-12+6]]=='0'){
					if(Rec_Buf[point2-12+6]==11||Rec_Buf[point2-12+Rec_Buf[point2-12+6]+1]==Rec_Buf[point2-12+2]){  //ȷ���յ�����ָ��
						Rec_Buf[point2+1]=0;
						strcpy(event,Rec_Buf+point2-12);
						q[2]=event[1];
						q[3]=event[2];
						event[1]=0x30;
						event[2]=0x30;
						event[6] -= 1;
						SendString(q+2);  //���ر�ʶ  �豸��+FF
						if (sendCommand(event, "00FF") == Success);
						else { flag_back=1; goto START;  //����		else soft_reset();  //����
						}
						CLR_Buf();
					//	delay_ms(10);
					}				
				}
			KIND = 0;
			break;	
			
			case 2:  //�յ�����ָ��
//			if(zw_success==0) break;
				if(point2>11&&Rec_Buf[point2-12]==0xf2&&Rec_Buf[point2-12+Rec_Buf[point2-12+6]]=='0'){
					if(Rec_Buf[point2-12+6]==7||Rec_Buf[point2-12+Rec_Buf[point2-12+6]-1]==Rec_Buf[point2-12+2]){
						Rec_Buf[point2+1]=0;
						strcpy(event,Rec_Buf+point2-12);
						q[2]=event[1];
						q[3]=event[2];
						event[1]=0x30;
						event[2]=0x30;
						event[6] += 1;
						SendString(q+2);  //���ر�ʶ  �豸��+FF
						if (sendCommand(event, "00FF") == Success);
						else {
							event[5]=0xf7;  //������
							event[6] -= 2;
							event[0] = 0xf1;
							event[12]= 0xfe;
							if (sendCommand(event, "00FF") == Success);
						  else { 
							 flag_back=1; goto START;  //����	
						  } 
						}
            CLR_Buf();						
					//	delay_ms(10);
					}				
				}			
			KIND = 0;
			break;	
			
			case 3:  //�յ���������ָ��
//			if(zw_success==0) break;
			if(point2>11&&Rec_Buf[point2-12]==0xf3&&Rec_Buf[point2-12+5]==0xF3&&Rec_Buf[point2-12+4]==Rec_Buf[point2-12+2]&&Rec_Buf[point2-12+3]==Rec_Buf[point2-12+1]){  //�յ�ĳ�豸����������
							Rec_Buf[point2+1]=0;
				      strcpy(event,Rec_Buf+point2-12);
					
							event[6] = 0;
						  strcat(event,form);  //ճ����ȥ·�ɱ�
							event[12]=0xFC;
			        event[6] -= 1;
					
							q[2]=event[1];
							q[3]=event[2];
							event[1]=0x30;
							event[2]=0x30;
							SendString(q+2);  //���ر�ʶ  �豸��+FF
						  if (sendCommand(event, "00FF") == Success);
	            else { 
							flag_back=1; goto START;  //����	
						  } 	
			        CLR_Buf();
	      //      delay_ms(10);
				} else
			if(point2>11&&Rec_Buf[point2-12]==0xf3&&Rec_Buf[point2-12+Rec_Buf[point2-12+6]]=='0'&&(Rec_Buf[point2-12+6]==11||Rec_Buf[point2-12+Rec_Buf[point2-12+6]+1]==Rec_Buf[point2-12+2])){
				  Rec_Buf[point2+1]=0;
				  strcpy(event,Rec_Buf+point2-12);
					q[2]=event[1];
					q[3]=event[2];
					event[1]=0x30;
					event[2]=0x30;
					event[6] -= 1;
				  SendString(q+2);  //���ر�ʶ  �豸��+FF
				  if (sendCommand(event, "00FF") == Success);
	        else { 
							 flag_back=1; goto START;  //����	
						  } 	
			    CLR_Buf();
	     //  delay_ms(10);
			}	
			KIND = 0;
			break;	
			
			case 4:  //�յ���������ָ��
		
			if(point2>11&&Rec_Buf[point2-12]==0xf4&&Rec_Buf[point2-12+Rec_Buf[point2-12+6]]=='0'){
				if(Rec_Buf[point2-12+6]==7||\
					Rec_Buf[point2-12+Rec_Buf[point2-12+6]-1]==Rec_Buf[point2-12+2]){  //ȷ���յ���������ָ��
					if(Rec_Buf[point2-12+3]=='0'&&Rec_Buf[point2-12+4]=='0'){      //�ܵ��ܻ����͵�����ͬ��ָ��
							Rec_Buf[point2]=0;
							strcpy(form,Rec_Buf+point2-12+6);   //ȡ��·�ɱ�
						//	form[0] -= 1;
						  strcpy(event,event_zw);  
							q[2]=event[1];
							q[3]=event[2];
							event[1]=0x30;
							event[2]=0x30;
							SendString(q+2);     //���ر�ʶ  �豸��+FF
						
						  event[5] = 0xF6;     //�����ܻ������յ�ͬ��֪ͨ
							event[6] = 0;
						  strcat(event,form);  //ճ����ȥ·�ɱ�
							event[12]=0xFC;
						  event[1]=0xF3;       //ת������������ָ��
						 // if (sendCommand(event, "00FF") == Success);
	           // else goto START;  //����	
	        //    delay_ms(10);
						  zw_success=1;       //�����ɹ�
						  TR0 = 0;   //ָʾ�Ʋ�����
						  LED = 1;
					} else{
						Rec_Buf[point2+1]=0;
						strcpy(event,Rec_Buf+point2-12);
						q[2]=event[1];
						q[3]=event[2];
						event[1]=0x30;
						event[2]=0x30;
						event[6] += 1;
						SendString(q+2);  //���ر�ʶ  �豸��+FF
						if (sendCommand(event, "00FF") == Success);
						else {
							event[5]=0xf7;  //������
							event[6] -= 2;
							event[0] = 0xf1;
							event[12]= 0xfe;
							if (sendCommand(event, "00FF") == Success);
						  else { 
							flag_back=1; goto START;  //����	
						  }  
						}	
					}
					
				}	
			}	
			
			
			
			
				
			KIND = 0;
			break;	
			
			case 5:  //����ָ��
//		if(zw_success==0) break;
				
			KIND = 0;
			break;	
		}
	}
}