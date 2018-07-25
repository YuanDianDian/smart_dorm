#ifndef __LCD12864_S_H__
#define __LCD12864_S_H__

//#include "STC12C5A.h"
#include <intrins.h>
#include <STC15F2K60S2.h>

//LCD12864µÄº¯Êý
extern void WriteCommand( unsigned char Cbyte );
extern void WriteData( unsigned char Dbyte );
extern void LcmInit( void );
extern void LcmClearTXT( void );
extern void LcmClearBMP( void );
//extern void PutTemp(unsigned char row,unsigned char col);
extern void PutStr(unsigned char row,unsigned char col,unsigned char *puts);
//extern void PutNum(unsigned char row,unsigned char col,unsigned int num);


#endif


