#ifndef _LCD_H
#define	_LCD_H

#include "font.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#if 0
#include "St7565r.h"
#else
#define LCD_X_MAX  128
#define LCD_Y_MAX  64

#define LCD_ORL       0X01  //或
#define LCD_NOR       0X02  //与
#define LCD_CPL       0X04  //反
#define LCD_CAP       0X08  //覆盖
#define LCD_UPDAT     0X10  //更新数据到LCD


typedef struct {
	int bmWidth;
	int bmHeight;
	const unsigned char * bmBits;
}LCDBITMAP;

typedef struct{
	u16 Posi_X;
	u16 Posi_Y;
	u16 Size;
}ItemPosi, *pItemPosi;

//#define UpdataLcd(a,b)  ( (void)0 )

#endif

#define SCREEN_X_MAX (LCD_X_MAX-1)
#define SCREEN_Y_MAX (LCD_Y_MAX-1)
#define MIN_FONT_WITH 8 //最小字体宽度

typedef struct {
	int bmWidth;
	int bmHeight;
	const unsigned char * bmBits;
}VFDBITMAP;


extern ItemPosi _XDATA Pos;

GLOBAL void LcdBufToLcd(u8 mode);
GLOBAL void LcdClearScreen(void);
GLOBAL void SetPosi(ItemPosi* pPosi,u8 iX,u8 iY,u8 iLen);
GLOBAL u8   LcdPrnText(ItemPosi Posi, u8 mode,const char *format,...);
										  
#endif
