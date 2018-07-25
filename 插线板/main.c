#include "usart.h"

//  u8 *p=NULL;
u8 xdata q[]="a400FF\r\n";  //设备号 00
u8 xdata event[14];   
u8 xdata form[7];  //自己上行的路由表
code unsigned char event_zw[14]={0xF3,'a','4','a','4',0xF3,6,0xF6,0xF6,0xF6,0xF6,0xF6,0xFC,0};//组网申请指令

bit zw_success;   //组网成功指令
bit flag_back ;  //是否有未处理的信息

void main()
{
	flag_back = 0; 
	START:
	USART_Init(); 
	event[14]=0;  form[7]=0; 

 
//strcpy(event,event_zw);   
	if (sendCommand(event_zw, "a4FF") == Success);
	else goto START;  //重启
//	delay_ms(10);                             
	while(counter1<300)  //等待1分钟
	{  
    if(KIND==4){ 
			if(point2>11&&Rec_Buf[point2-12]==0xf4\
					&&(Rec_Buf[point2-12+6]==12||Rec_Buf[point2-12+Rec_Buf[point2-12+6]]==0xf6)){
					if(Rec_Buf[point2-12+3]=='a'&&Rec_Buf[point2-12+4]=='4'){   //受到总机发送的组网同意指令
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
		if (sendCommand(event, "a4FF") == Success);
	   else goto START;  //重启
	  flag_back = 0;
		CLR_Buf();
	} 

     while(1)
	{
		switch(KIND){
			case 2:  //收到下行指令
//			if(zw_success==0) break;
				if(point2>11&&Rec_Buf[point2-12]==0xf2&&Rec_Buf[point2-12+3]=='a'&&Rec_Buf[point2-12+4]=='4'\
						&&(Rec_Buf[point2-12+6]==12||Rec_Buf[point2-12+Rec_Buf[point2-12+6]]==0xf6)){
					  
						q[2]=Rec_Buf[point2-12+1];
						q[3]=Rec_Buf[point2-12+2];
						SendString(q+2);  //返回标识  设备号+FF
					
						if(Rec_Buf[point2-12+5]==1){
							pwm=0;low=1;  // 开灯
					  }	else if(Rec_Buf[point2-12+5]==2){
							pwm=1;high=1;  //关灯
					  }	
						CLR_Buf();
				}			
			KIND = 0;
			break;	
				
			case 4:  //收到组网下行指令
		
			if(point2>11&&Rec_Buf[point2-12]==0xf4\
					&&(Rec_Buf[point2-12+6]==12||Rec_Buf[point2-12+Rec_Buf[point2-12+6]]==0xf6)){
					if(Rec_Buf[point2-12+3]=='a'&&Rec_Buf[point2-12+4]=='4'){      //受到总机发送的组网同意指令
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
		if(low==1){
      low=0;
			strcpy(event,event_zw); 
			event[5] = 1;     //有人开灯
			event[6] = 0;
			strcat(event,form);  //粘贴进去路由表
			event[6]--;
			event[12]=0xFE;
			event[0]=0xF1;       //转换成上行指令
			if (sendCommand(event, "a4FF") == Success);
	    else { 
				 flag_back=1;
				 goto START; 
			}
			CLR_Buf();
		} else if(high==1){
			high=0;
			strcpy(event,event_zw); 
			event[5] = 2;     //有人关灯
			event[6] = 0;
			strcat(event,form);  //粘贴进去路由表
			event[6]--;
			event[12]=0xFE;
			event[0]=0xF1;       //转换成上行指令
			if (sendCommand(event, "a4FF") == Success);
	    else { 
				 flag_back=1;
				 goto START; 
			}
			CLR_Buf();
		} 
	}
}

