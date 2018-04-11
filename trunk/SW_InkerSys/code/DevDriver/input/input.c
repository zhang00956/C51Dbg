
#define SYSLOG 0

#include "inkersys.h"
#include "input.h"

#if 1
/* 计数法滤波 */
#define INPUT_MAX_NUM   2 //输入接口个数
#define InPutValidTimes 5 //输入滤波级数

typedef struct{
u8	 InState;
u8	 PreState;
u8   Cnt;
u8  (*InPutFunc)(void);
}tInPut;

u8 _XDATA InPutDebug = 0; //输入屏蔽
volatile tInPut _IRAM InPut[INPUT_MAX_NUM]={
{0,0,0,GetInPut1},
{0,0,0,GetInPut2}};

GLOBAL void ProF_10mS_InPut(void)
{
	u8 i;

	if(InPutDebug)
		return ;
		
	for(i=0;i<INPUT_MAX_NUM;i++)
		{
		if(InPut[i].InPutFunc() != InPut[i].PreState)	/*状态有变化*/
			{
			InPut[i].PreState = InPut[i].InPutFunc();
			InPut[i].Cnt      = 0;
			}
		else
			{
			InPut[i].Cnt ++;
			if(InPut[i].Cnt >= InPutValidTimes)
				{
				InPut[i].Cnt     = InPutValidTimes;
				InPut[i].InState = InPut[i].PreState;
				}
			}
		}
}

GLOBAL u8 GetInput(u8 index)
{
	if(index < INPUT_MAX_NUM)
		return InPut[index].InState;
	return 0XFF;
}

#else
/* 移位法滤波 */

#define INPUT_NUMBER    8    //输入一组个数
#define INPUT_GOP_NUM   6    //输入接口个数

_ROM u8 InputFilterTab[8] = {0X01,0X03,0X07,0X0F,0X1F,0X3F,0X7F,0XFF};

typedef struct{
u8	 InputS[INPUT_NUMBER];
u8  (*InPutFunc)(void);
}tInPut;

volatile tInPut _XDATA InPut[INPUT_GOP_NUM]={
{{0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF},GetInPutX0},
{{0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF},GetInPutX1},
{{0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF},GetInPutX2},
{{0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF},GetInPutX3},
{{0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF},GetInPutX4},
{{0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF},GetInPutX5}};

 
GLOBAL void ProF_10mS_InPut(void)
{
	static u8 Cnt = 0;
	u8 i,temp;

	temp = InPut[Cnt].InPutFunc(); 
	for(i=0;i<INPUT_NUMBER;i++)
		{
		InPut[Cnt].InputS[i] <<= 1;
		if(temp&BitTable[i])
			InPut[Cnt].InputS[i] |= 0X01;
		}

	Cnt ++;
	if(Cnt >= INPUT_GOP_NUM)
		Cnt = 0;
}

GLOBAL void GetInput(void)
{
	u8 i;
	u16 tmp;

	tmp = 0X00;
	for(i=0;i<INPUT_NUMBER;i++)
		{
		tmp >>= 1;
		if((InPut[1].InputS[i]&InputFilterTab[SysReg.InputFt]) == 0)//输入有效
			tmp |= 0X8000;
		}

	for(i=0;i<INPUT_NUMBER;i++)
		{
		tmp >>= 1;
		if((InPut[0].InputS[i]&InputFilterTab[SysReg.InputFt]) == 0)//输入有效
			tmp |= 0X8000;
		}
	SysReg.X0X1 = tmp;

	tmp = 0X00;
	for(i=0;i<INPUT_NUMBER;i++)
		{
		tmp >>= 1;
		if((InPut[3].InputS[i]&InputFilterTab[SysReg.InputFt]) == 0)//输入有效
			tmp |= 0X8000;
		}

	for(i=0;i<INPUT_NUMBER;i++)
		{
		tmp >>= 1;
		if((InPut[2].InputS[i]&InputFilterTab[SysReg.InputFt]) == 0)//输入有效
			tmp |= 0X8000;
		}
	SysReg.X2X3 = tmp;

	tmp = 0X00;
	for(i=0;i<INPUT_NUMBER;i++)
		{
		tmp >>= 1;
		if((InPut[5].InputS[i]&InputFilterTab[SysReg.InputFt]) == 0)//输入有效
			tmp |= 0X8000;
		}

	for(i=0;i<INPUT_NUMBER;i++)
		{
		tmp >>= 1;
		if((InPut[4].InputS[i]&InputFilterTab[SysReg.InputFt]) == 0)//输入有效
			tmp |= 0X8000;
		}
	SysReg.X4X5 = tmp;	
}
#endif

/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/
#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

static void PrfInPutMsg(void)
{
	u8 i;

	DPRINTF(("Filter(%bd) InPutDebug(%bd)\r\n",InPutValidTimes,InPutDebug));
	DPRINTF(("Num InVal InIO\r\n"));
	for(i=0;i < INPUT_MAX_NUM;i++)
		DPRINTF(("%bd>:  (%bd)   (%bd)\r\n",i,InPut[i].InState,InPut[i].InPutFunc()));
}

GLOBAL bool InPut_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n")     == NULL)
		{
		DPRINTF((".InPut\r\n"));
		return FALSE;
		}

	if(strcmp(pComDbg.Revdata,".input\r\n")   == NULL)
		{
		DPRINTF(("InPutMsg\r\n"));
		DPRINTF(("InPutDebug <0~1>\r\n"));
		DPRINTF(("InPutval   <0~%bd> <0,1>\r\n",INPUT_MAX_NUM-1));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"inputmsg\r\n") == NULL)
		{
		PrfInPutMsg();
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"inputdebug") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		InPutDebug = pComDbg.Param8[0];
		DPRINTF(("InPutDebug <= %bd\r\n",InPutDebug));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"inputval") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8) == FALSE)
			return FALSE;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8+1) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		if(pComDbg.Param8[0] < INPUT_MAX_NUM)
			{
			InPutDebug = 1;//一旦设置输入值 自动进去输入调试模式
			InPut[pComDbg.Param8[0]].InState = pComDbg.Param8[1];
			DPRINTF(("InPut%bd <= %bd\r\n",InPut[pComDbg.Param8[0]].InState,pComDbg.Param8[1]));
			return TRUE;
			}
		}
	
	return FALSE;
}

/* #End Comdbg Interface */

#endif


