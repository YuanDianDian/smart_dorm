#include "LCD12864_S.h"

/**************************************************************
               iO�ں궨����
***************************************************************/
//   sbit CS =P2^2;
//   sbit SID=P2^3;//r/w
//   sbit SCK=P2^4;//e
   sbit CS =P1^2;
   sbit SID=P1^3;//r/w
   sbit SCK=P1^4;//e
/*******************************************************************
              ����������
********************************************************************/	    
unsigned char code AC_TABLE[]={				           //�������
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
};
/****************************************************************
 ��������������Ȳ��������ṩ��12864word�ĵ����ϣ����12864�������
 ��ʽ��	 											  
              ����һ���ֽ�
*****************************************************************/
void SendByte(unsigned char Dbyte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		SCK = 0;
		Dbyte=Dbyte<<1;
		SID = CY;
		SCK = 1;
		SCK = 0;
	}
}
/**********************************************************
              ����һ���ֽ�
***********************************************************/

unsigned char ReceiveByte(void)
{
	unsigned char i,temp1,temp2;
	temp1=temp2=0;
	for(i=0;i<8;i++)
	{
		temp1=temp1<<1;
		SCK = 0;
		SCK = 1;                
		SCK = 0;
		if(SID) temp1++;
	}
	for(i=0;i<8;i++)
	{
		temp2=temp2<<1;
		SCK = 0;
		SCK = 1;
		SCK = 0;
		if(SID) temp2++;
	}
	return ((0xf0&temp1)+(0x0f&temp2));
}
/****************************************************************
                      ���æ״̬
******************************************************************/
void CheckBusy( void )
{
	do SendByte(0xfc);     //11111,RW(1),RS(0),0
	while(0x80&ReceiveByte());
}

/******************************************************************
           дһ���ֽڵ�ָ��
*******************************************************************/
void WriteCommand( unsigned char Cbyte )
{
	CS = 1;
	CheckBusy();
	SendByte(0xf8);          //11111,RW(0),RS(0),0
	SendByte(0xf0&Cbyte);
	SendByte(0xf0&Cbyte<<4);
	CS = 0;
}
/*************************************************************
                 дһ���ֽڵ�����
**************************************************************/
void WriteData( unsigned char Dbyte )
{
	CS = 1;
	CheckBusy();
	SendByte(0xfa);          //11111,RW(0),RS(1),0
	SendByte(0xf0&Dbyte);
	SendByte(0xf0&Dbyte<<4);
	CS = 0;
}

/******************************************************************
                         lcd��ʼ������
*******************************************************************/
void LcmInit( void )
{
     WriteCommand(0x30);
     WriteCommand(0x03);
     WriteCommand(0x0c);
     WriteCommand(0x01);
     WriteCommand(0x06);
}

/*******************************************************************************************************
                                 �趨��꺯��
********************************************************************************************************/
void Location_xy_12864(unsigned char x,unsigned char y)
{
	switch(x)
	{
		case 0:
			x=0x80;break;
		case 1:
			x=0x90;break;
		case 2:
			x=0x88;break;
		case 3:
			x=0x98;break;
		default:
			x=0x80;
	}
	y=y&0x07;
	WriteCommand(0x30);
	WriteCommand(y+x);
	WriteCommand(y+x);

}
/***********************************************************************************
                  ����ı�
************************************************************************************/
void LcmClearTXT( void )
{
	unsigned char i;
	WriteCommand(0x30);
	WriteCommand(0x80);
	for(i=0;i<64;i++)
	WriteData(0x20);
	Location_xy_12864(0,0);	    
}
/**************************************************************************************
                   ���ͼƬ
*****************************************************************************************/
void LcmClearBMP( void )
{
	unsigned char i,j;
	WriteCommand(0x34);
	WriteCommand(0x36);
	for(i=0;i<32;i++)
	{
		WriteCommand(0x80|i);
		WriteCommand(0x80);
		for(j=0;j<32;j++)
		WriteData(0);
	}
}



/****************************************************************************************
                      ��ʾ�ַ���
*****************************************************************************************/
void PutStr(unsigned char row,unsigned char col,unsigned char *puts)
{    
	WriteCommand(0x30);
	WriteCommand(AC_TABLE[8*row+col]);
	while(*puts != '\0')
	{
		if(col==8)
		{
			col=0;
			row++;
		}
		if(row==4) row=0;
		WriteCommand(AC_TABLE[8*row+col]);
		WriteData(*puts);
		puts++;
		if(*puts != '\0')
		{
			WriteData(*puts);
			puts++;
			col++;
		}  
	}
}


				 