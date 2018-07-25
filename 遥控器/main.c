#include "usart.h"

//  u8 *p=NULL;
u8 xdata q[]="a500FF\r\n";  //设备号 a5
u8 xdata event[14];   
u8 xdata form[7];  //自己上行的路由表
code unsigned char event_zw[14]={0xF3,'a','5','a','5',0xF3,1,0xF6,0xF6,0xF6,0xF6,0xF6,0xFC,0};//组网申请指令

bit zw_success;   //组网成功指令
bit flag_back ;  //是否有未处理的信息
void Init_9600();
void Init_115200();

code u8 air_meidi[]={0x02,0x00,0x38,0x08,0x32,0};
code u8 air_close[]={0x04,0x00,0x08,0x08,0x04,0};
code u8 air_open[]={0x04,0xff,0x08,0x08,0xfb,0};
code u8 air_31[]={0x06,0x10,0x08,0x08,0x16,0};
code u8 air_17[]={0x06,0x1e,0x08,0x08,0x18,0};

void main()
{
	flag_back = 0; 
//	START:
	USART_Init(); 
	event[14]=0;  form[7]=0; 
      
                                                          /*
//strcpy(event,event_zw);   
	if (sendCommand(event_zw, "a5FF") == Success);
	else goto START;  //重启
//	delay_ms(10);                             
	while(counter1<300)  //等待1分钟
	{  
    if(KIND==4){ 
			if(point2>11&&Rec_Buf[point2-12]==0xf4&&(Rec_Buf[point2-12+6]==12||Rec_Buf[point2-12+Rec_Buf[point2-12+6]]==0xf6)){
					if(Rec_Buf[point2-12+3]=='a'&&Rec_Buf[point2-12+4]=='5'){   //受到总机发送的组网同意指令
							Rec_Buf[point2]=0;
							strcpy(form,Rec_Buf+point2-12+6);   //取出路由表
						  q[2]=Rec_Buf[point2-12+1];
							q[3]=Rec_Buf[point2-12+2];
						  CLR_Buf();
							SendString(q+2);     //返回标识  设备号+FF
						  zw_success=1;       //组网成功
						  TR0 = 0;   //指示灯不再闪
						  LED = 1;   break;
					} 
			}	
		}  
	}  
	if(zw_success==0) goto START;  //重启  
	
	 if(flag_back!=0){
		if (sendCommand(event, "a5FF") == Success);
	   else goto START;  //重启
	  flag_back = 0;
		CLR_Buf();
	}                                                          */
	 
	SendString(air_meidi); delay_ms(50); //确定空调型号  
                                                           
     while(1)
	{
		switch(KIND){
			case 2:  //收到下行指令
//			if(zw_success==0) break;
				if(point2>11&&Rec_Buf[point2-12]==0xf2&&Rec_Buf[point2-12+5]=='a'&&Rec_Buf[point2-12+5]=='5'\
					&&(Rec_Buf[point2-12+6]==12||Rec_Buf[point2-12+Rec_Buf[point2-12+6]]==0xf6)){
					  
						q[2]=Rec_Buf[point2-12+1];
						q[3]=Rec_Buf[point2-12+2];
						SendString(q+2);  //返回标识  设备号+FF
					
					  Init_9600();
					
						switch(Rec_Buf[point2-12+5]){
						case 1:   SendString(air_open);    break;
						case 2:   SendString(air_close);    break;
						case 3:   SendString(air_17);    break;
						case 4:   SendString(air_31);    break;
						case 5:   SendString(air_open);    break;
						}
						CLR_Buf();
						Init_115200();
				}			
			KIND = 0;
			break;	
				
			case 4:  //收到组网下行指令
		
			if(point2>11&&Rec_Buf[point2-12]==0xf4){
					if(Rec_Buf[point2-12+3]=='a'&&Rec_Buf[point2-12+4]=='5'\
						&&(Rec_Buf[point2-12+6]==12||Rec_Buf[point2-12+Rec_Buf[point2-12+6]]==0xf6)){      //受到总机发送的组网同意指令
							Rec_Buf[point2]=0;
							strcpy(form,Rec_Buf+point2-12+6);   //取出路由表
					  	q[2]=Rec_Buf[point2-12+1];
							q[3]=Rec_Buf[point2-12+2];
							SendString(q+2);     //返回标识  设备号+FF
						  zw_success=1;       //组网成功
						  TR0 = 0;   //指示灯不再闪
						  LED = 1;
					} 
			}	
		
			KIND = 0;
			break;				
			
			case 5:  //测试指令
//		if(zw_success==0) break;
				
			KIND = 0;
			break;	
		}
	}
}

void Init_115200()
{
	ES=0;
	ACC = P_SW1;
	ACC &= ~(S1_S0|S1_S1);
	ACC |= S1_S0;   //  串口一设为   P3.6RXD P3.7TXD
	P_SW1 = ACC;
	
	SCON=0x50;
  T2L=(65536-(FOSC/4/BAUD));
  T2H=(65536-(FOSC/4/BAUD))>>8;
  AUXR=0X14;                        //
  AUXR|=0X01;
  ES=1;
}

void Init_9600()
{
	ES=0;
	ACC = P_SW1;
	ACC &= ~(S1_S0|S1_S1);   //  串口一设为   P3.0RXD P3.1TXD
	P_SW1 = ACC;
	
	SCON=0x50;
  T2L=(65536-(FOSC/4/9600));
  T2H=(65536-(FOSC/4/9600))>>8;
  AUXR=0X14;                        //
  AUXR|=0X01;
  ES=1;
}

