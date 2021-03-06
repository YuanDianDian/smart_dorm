#include "usart.h"

//  u8 *p=NULL;
u8 xdata q[]="0000FF\r\n";  //设备号 00
u8 xdata event[14];   
u8 xdata form[7];  //自己上行的路由表
code unsigned char event_zw[14]={0xF3,'0','0','0','0',0xF3,1,0xF6,0xF6,0xF6,0xF6,0xF6,0xFC,0};//组网申请指令

bit zw_success;   //组网成功指令

//#define Tip_max 40  //崩溃容忍上限

//u8 flag_back ;

unsigned	char i;
void main()
{
	START:
	USART_Init();  event[13]=0;  event[6]=0;
   /* 
	strcpy(event,event_zw);   
	if (sendCommand("event", "00FF") == Success);
	else soft_reset();  //重启
	delay_ms(10);
   */
//strcpy(event,event_zw); 
zw_success=1; form[0]=7; form[1]='0';
 for( i=2;i<6;i++)	 form[i]=0xf6; form[6]=0;  SendString(form);
	SendString(event_zw);
	while(1)
	{
		switch(KIND){
			case 1:  //收到上行指令
			if(zw_success==0) break;
				if(point2>11&&Rec_Buf[point2-12]==0xf1&&Rec_Buf[point2-12+Rec_Buf[point2-12+6]]=='0'){
					if(Rec_Buf[point2-12+6]==11||Rec_Buf[point2-12+Rec_Buf[point2-12+6]+1]==Rec_Buf[point2-12+2]){  //确认收到上行指令
						Rec_Buf[point2+1]=0;
						strcpy(event,Rec_Buf+point2-12);
																				SendString(event);
						q[2]=event[1];
						q[3]=event[2];
						event[1]=q[0];
						event[2]=q[1];
						event[6] -= 1;
						SendString(q+2);  //返回标识  设备号+FF
			//			if (sendCommand(event, "00FF") == Success);
			//			else goto START;  //重启
						delay_ms(10);
					}				
				}
			KIND = 0;
			break;	
			
			case 2:  //收到下行指令
			if(zw_success==0) break;
							
			KIND = 0;
			break;	
			
			case 3:  //收到组网上行指令
			if(zw_success==0) break;
			if(point2>11&&Rec_Buf[point2-12]==0xf3&&Rec_Buf[point2-12+5]==0xF3&&Rec_Buf[point2-12+4]==Rec_Buf[point2-12+2]&&Rec_Buf[point2-12+3]==Rec_Buf[point2-12+1]){  //收到某设备的组网请求
							Rec_Buf[point2+1]=0;
				      strcpy(event,Rec_Buf+point2-12);
			
							event[6] = 0;
						  strcat(event,form);  //粘贴进去路由表

							event[12]=0xFC;
					    event[0]=0xF4;
							event[12]=0xFB;
							q[2]=event[1];
							q[3]=event[2];
							event[1]=q[0];
							event[2]=q[1];
							SendString(q+2);  //返回标识  设备号+FF

						  if (sendCommand(event, "00FF") == Success);
	            else goto START;  //重启
	            delay_ms(10);
				} else
			if(point2>11&&Rec_Buf[point2-12]==0xf3&&Rec_Buf[point2-12+5]==0xF6&&Rec_Buf[point2-12+4]==Rec_Buf[point2-12+2]&&Rec_Buf[point2-12+3]==Rec_Buf[point2-12+1]){
				  
				  SendString(q+2);  //返回标识  设备号+FF
				  
			}	
			KIND = 0;
			break;	
			
			case 4:  //收到组网下行指令
			if(point2>11&&Rec_Buf[point2-12]==0xf4&&Rec_Buf[point2-12+Rec_Buf[point2-12+6]]=='0'){
				if(Rec_Buf[point2-12+6]==7||Rec_Buf[point2-12+Rec_Buf[point2-12+6]-1]==Rec_Buf[point2-12+2]){  //确认收到组网下行指令
			//	if(Rec_Buf[point2-12+6]==11||Rec_Buf[point2-12+Rec_Buf[point2-12+6]+1]==0XF6){  //已到最后一层
					if(Rec_Buf[point2-12+4]=='0'&&Rec_Buf[point2-12+5]=='0'){      //受到总机发送的组网同意指令
							Rec_Buf[point2]=0;
							strcpy(form,Rec_Buf+point2-12+6);   //取出路由表
						  strcpy(event,event_zw);  
							q[2]=event[1];
							q[3]=event[2];
							event[1]=q[0];
							event[2]=q[1];
							SendString(q+2);     //返回标识  设备号+FF
						
						  event[5] = 0xF6;     //告诉总机我已收到同意通知
							event[6] = 0;
						  strcat(event,form);  //粘贴进去路由表
							event[12]=0xFC;
						  event[1]=0xF3;       //转换成上行组网指令
						  if (sendCommand(event, "00FF") == Success);
	            else goto START;  //重启
	            delay_ms(10);
						  zw_success=1;       //组网成功
						  TR0 = 0;   //指示灯不再闪
						  LED = 1;
					} else {	  
						if(zw_success==0) break;
						Rec_Buf[point2+1]=0;
						strcpy(event,Rec_Buf-12);
						q[2]=event[1];
						q[3]=event[2];
						event[1]=q[0];
						event[2]=q[1];
						event[6] += 1;
						SendString(q+2);  //返回标识  设备号+FF
						if (sendCommand(event, "00FF") == Success);
						else goto START;  //重启
						delay_ms(10);
					}
				}
			}
			KIND = 0;
			break;	
			
			case 5:  //测试指令
			if(zw_success==0) break;
				
			KIND = 0;
			break;	
		}
	}
}