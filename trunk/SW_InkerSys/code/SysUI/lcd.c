
#define SYSLOG 0

#include "InkerSys.h"

#include "lcd.h"

ItemPosi _XDATA Pos;
char     _XDATA SysBackBuff1[LCD_X_MAX];
char     _XDATA SysBackBuff2[LCD_X_MAX];
u8       _XDATA LcdTextBuf[LCD_X_MAX/MIN_FONT_WITH + 1]={0};

#define VIR_WIN 

#ifdef VIR_WIN
/* Virtual Window  128*64

PAGE1 BYTE1 .. Byte128
      bit0  .. bit0
      :     .. :
      bit7  .. bit7

PAGE2 BYTE1 .. Byte128
      bit0  .. bit0
      :     .. :
      bit7  .. bit7      

*/

char _XDATA VWinLcd[8][128] = {0};

static void PrfBitMap(u8 val)
{
	switch(val)
		{
		case 0:
			//DPRINTF((" "));
			Uart0Send(' ');
			break;
		case 1:
			//DPRINTF(("_"));
			Uart0Send('_');
			break;
		case 2:
			//DPRINTF(("-"));
			Uart0Send('-');
			break;
		case 3:
			//DPRINTF(("="));
			Uart0Send('=');
			break;	
		}
}

static void LcdBufToCom(void)
{
	u8 _XDATA x,y,tmp8;
	
	//DPRINTF(("\r\n"));
	Uart0Send('\r');
	Uart0Send('\n');

	for(y=0;y<LCD_Y_MAX/8;y++)
		{
		for(x=0;x<LCD_X_MAX;x++)
			{
			tmp8 = VWinLcd[y][x];
			tmp8 >>= 0;
			PrfBitMap(tmp8&0X03);
			}
		//DPRINTF(("\r\n"));
		Uart0Send('\r');
		Uart0Send('\n');

		for(x=0;x<LCD_X_MAX;x++)
			{
			tmp8 = VWinLcd[y][x];
			tmp8 >>= 2;
			PrfBitMap(tmp8&0X03);
			}
		//DPRINTF(("\r\n"));
		Uart0Send('\r');
		Uart0Send('\n');

		for(x=0;x<LCD_X_MAX;x++)
			{
			tmp8 = VWinLcd[y][x];
			tmp8 >>= 4;
			PrfBitMap(tmp8&0X03);
			}
		//DPRINTF(("\r\n"));
		Uart0Send('\r');
		Uart0Send('\n');

		for(x=0;x<LCD_X_MAX;x++)
			{
			tmp8 = VWinLcd[y][x];
			tmp8 >>= 6;
			PrfBitMap(tmp8&0X03);
			}
		//DPRINTF(("\r\n"));
		Uart0Send('\r');
		Uart0Send('\n');
		}	
}

static void UptoVWinLcd(ItemPosi Posi,u8 length)
{
	u8 _XDATA i;

	for(i=0;i<length;i++)
		{
		VWinLcd[Posi.Posi_Y/8    ][Posi.Posi_X+i] = SysBackBuff1[Posi.Posi_X + i];
		VWinLcd[Posi.Posi_Y/8 + 1][Posi.Posi_X+i] = SysBackBuff2[Posi.Posi_X + i];
		}
}

#endif
const Font_t *GetFontTable(int size)
{
	switch(size)
		{
		case 16:
			return Font16;			
		default:
			return Font16;//ȱʡֵ
		}
}

static int GetFont(const Font_t *FontTable, char **pcText, u8 **Font,u8 *Width,u8 *Heigh)
{
	Font_t *font_table;
	u8 Byte0,Byte1;
	
	font_table = FontTable;
	if(IsChineseFont(*pcText))
		{
		Byte0 = **pcText;
		Byte1 = *(*pcText+1);
		*pcText += 2;
		}
	else
		{
		Byte0 = **pcText;
		Byte1 = 0;
		*pcText += 1;
		}
	
	while((unsigned short)font_table ->font_code[0])
		{
		if((font_table->font_code[0] == Byte0) && (font_table->font_code[1] == Byte1))
			{
			*Width = font_table->Width;
			*Heigh = font_table->Height;
			*Font = (u8 *)font_table->bmBuf;
			return 1;
			}
		font_table++;
		}	
	return 0;
}

static void LcdClearRect(u8 iX0, u8 iY0, u8 iXLength, u8 iYLength, u8 mode)
{
	u8 i;
	
	SysBackBuff1[iX0] = 0xff;
	SysBackBuff2[iX0] = 0xff;
	for(i=1;i<iXLength;i++)
		{
		SysBackBuff1[iX0 + i] = 0X03;
		SysBackBuff2[iX0 + i] = 0XC0;
		}
	SysBackBuff1[iX0 + iXLength - 1] = 0xff;
	SysBackBuff2[iX0 + iXLength - 1] = 0xff;

	iYLength = 0;
	mode     = 0;
	iY0      = 0;
}

static void LcdDrawFont(u8 iX,u8 iY,const u8 * font,u8 width,u8 height,u8 mode)
{														    
	u8 i;
	
	if(width<8)
		width = 8;
		
	for(i=0;i<width;i++)
		{
		if(mode & LCD_CPL)
			{
			SysBackBuff1[iX+i] = ~(*(font+i));
			SysBackBuff2[iX+i] = ~(*(font+i+ width));	
			}
		if(mode & LCD_ORL)
			{
			SysBackBuff1[iX+i] |= *(font+i);
			SysBackBuff2[iX+i] |= *(font+i+ width);	
			}
		if(mode & LCD_NOR)
			{
			SysBackBuff1[iX+i] &= *(font+i);
			SysBackBuff2[iX+i] &= *(font+i+ width);	
			}
		if(mode & LCD_CAP)
			{
			SysBackBuff1[iX+i] = *(font+i);
			SysBackBuff2[iX+i] = *(font+i+ width);	
			}		
		}

	iY     = 0;
	height = 0;
}														          

GLOBAL void LcdBufToLcd(u8 mode)
{
#ifdef VIR_WIN
	if(mode)
		LcdBufToCom();
#endif
}

GLOBAL void LcdClearScreen(void)
{

#ifdef VIR_WIN
	u8 _XDATA x,y;

	for(y=0;y<8;y++)
		for(x=0;x<128;x++)
			VWinLcd[y][x] = 0;
#endif

}

GLOBAL void SetPosi(ItemPosi* pPosi,u8 iX,u8 iY,u8 iLen)
{
	pPosi->Posi_X = iX;
	pPosi->Posi_Y = iY;
	pPosi->Size = iLen;
}

GLOBAL u8 LcdPrnText(ItemPosi Posi, u8 mode,const char *format,...)
{
	u8 _XDATA Width,Height;
	Font_t *FontTable;
	u8 *msg,*Font;
	u8 _XDATA length = 0,posx;

	va_list arg_ptr;

	va_start (arg_ptr, format);           /* format string */
	vsprintf (LcdTextBuf, format, arg_ptr);

	va_end (arg_ptr);

	posx = Posi.Posi_X;
	FontTable = GetFontTable(Posi.Size);
	msg = LcdTextBuf;

	while(*msg)		
		{
		if(GetFont(FontTable,&msg,&Font,&Width,&Height))
			{
			LcdDrawFont(Posi.Posi_X,Posi.Posi_Y,Font,Width,Height,mode);		
			}
		else
			{
			LcdClearRect(Posi.Posi_X,Posi.Posi_Y,Posi.Size,Posi.Size,mode);
			Width = Posi.Size;
			}
		Posi.Posi_X += Width;
		length += Width;
		}
	
	Posi.Posi_X = posx;

#ifdef VIR_WIN
	UptoVWinLcd(Posi,length);
#endif

	return length;
}

