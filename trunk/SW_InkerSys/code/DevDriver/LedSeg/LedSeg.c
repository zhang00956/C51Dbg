
#define SYSLOG 0

#include "inkersys.h"
#include "LedSeg.h"

/*
1 自检断码 全亮 全灭
************************
*  --   --   --   --   *
* |  | |  |o|  | |  |  *
*  --   --   --   --   *
* |  | |  |o|  | |  |  *
*  -- o -- o -- o -- o *
************************

2 显示版本号 V1.02
************************
*            --   --   *
* |  |    | |  |    |  *
*                 --   *
* |  |    | |  | |     *
*  --      o --   --   *
************************

3 显示重要设置参数

4 运行参数

*/


#define LED_SEG_NUM 4 //数码管位数
u8 _XDATA LedSegBuf[LED_SEG_NUM];

typedef struct {
	char Ascii;
	u8   SegVal;
}tLedSeg;

_ROM tLedSeg LedSegTab[] = {
{' ',0XFF},
{'x',0X00},	

{'0',0X88},
{'1',0XBB},
{'2',0XC1},
{'3',0X91},
{'4',0XB2},
{'5',0X94},
{'6',0X84},
{'7',0XB9},
{'8',0X80},
{'9',0X90},
{'-',0XF7},
{'C',0XCC},
{'p',0XE0},

{'V',0X8A},
{'I',0XEE},
{'A',0XA0},
{'d',0X83},
{'c',0XC7},

{'Z',0XC1},
{'e',0XC4},
{'r',0XE6},
{'o',0X87},

{'F',0XE4},// FULL
{'U',0X8A},
{'L',0XCE},

{0,0}};

/************************************************************************/
/*      Local Functions                                                 */
/************************************************************************/
static void sBufToDis(void)
{   

}

static u8 sScanLedSegTab(u8 dat)
{
	u8 i;
	for(i=0;LedSegTab[i].Ascii;i++)
		{
		if(dat == LedSegTab[i].Ascii)
			return LedSegTab[i].SegVal;
		}
	return 0;
}

/************************************************************************/
/*      Application Interface                                           */
/************************************************************************/
u8 _XDATA LedSegPrf[LED_SEG_NUM+2];// LedSegNum + . + \0
GLOBAL void LedSegDis(u8 Up, char *format,...)
{
	u8 i,n = 0;

	va_list arg_ptr;
	va_start (arg_ptr, format);           /* format string */
	vsprintf (LedSegPrf, format, arg_ptr);   //使用时要注意内存溢出问题
	va_end (arg_ptr);

	for(i=0;i<LED_SEG_NUM+1;i++)
		{
		if(LedSegPrf[i] == '.')
			LedSegBuf[n-1] = LedSegBuf[n-1]&0X7F;//将小数点添加到前面一位数码管缓存中
		else
			LedSegBuf[n++] = sScanLedSegTab(LedSegPrf[i]);
		if(n >= LED_SEG_NUM)
			break;
		}
	
	if(Up)
		sBufToDis();
}

GLOBAL void LedSegInit(void)
{
	LedSegDis(1,"xxxx");
	Delay1mS(20);
	LedSegDis(1,"    ");
	Delay1mS(20);
	LedSegDis(1,"V%s",GetVersionNum());
	Delay1mS(30);
}

GLOBAL void ProB_100mS_LedSeg(void)
{

}

/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/
#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool LedSeg_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".LedSeg\r\n"));
		return FALSE;
		}

	if(strcmp(pComDbg.Revdata,".ledseg\r\n") == NULL)
		{
		DPRINTF(("LedSegMsg\r\n"));
		DPRINTF(("LedSeg <0:%bd> <Seg>\r\n",LED_SEG_NUM-1));
		DPRINTF(("LedSegF <float>\r\n"));
		DPRINTF(("LedSegStr <str>\r\n"));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"ledsegmsg\r\n") == NULL)
		{
		u8 i;
		
		DPRINTF(("LedSegBuf:"));
		for(i=0;i<LED_SEG_NUM;i++)
			DPRINTF(("[%b002X] ",LedSegBuf[i]));
		DPRINTF(("\r\n"));

		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"ledseg") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8 + 0) == FALSE)
			return FALSE;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8 + 1) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		if(pComDbg.Param8[0] < (LED_SEG_NUM-1))
			{
			LedSegBuf[pComDbg.Param8[0]] = pComDbg.Param8[1];
			return TRUE;
			}
		}

	if(strcmp(pComDbg.Revdata,"ledsegf") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeFloat(pComDbg.Revdata + pComDbg.Cnt,&pComDbg.ParamF[0]) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		LedSegDis(1,"%0005.1f",pComDbg.ParamF[0]);
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"ledsegstr") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeString(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Str) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		LedSegDis(1,"%s",pComDbg.Str);
		return TRUE;
		}

	return FALSE;
}

#endif





