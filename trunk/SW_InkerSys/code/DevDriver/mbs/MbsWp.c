#define SYSLOG 0

#include "inkersys.h"
#include "mbsrtu.h"

enum{
REG_W2R_R,
REG_W2R_RW,	
};

typedef struct{
	u8 PcSetVal[4];
	u8 Index;
	u8 Result;
}tMbsWp;

tMbsWp _XDATA MbsWpPkg;

typedef void (*MenuProc)(void);

struct tMbsRegWp{
u8       Index;	   /* Æ«ÒÆ±äÖ·   */
u8       Byten;    /* Êý¾Ý³¤¶È   */
u8       W2R;      /* ¶ÁÐ´ÊôÐÔ   */	
u16      Addr;     /* ¼Ä´æÆ÷µØÖ· */
MenuProc pFunc;    /* Ö´ÐÐº¯Êý   */
};


/* Write Protectc Param */
static void WpMbsID(void)
{
	u16 *pValue = &MbsReg.MbsId,tmp;

	tmp = MbsWpPkg.PcSetVal[1];
	if((tmp > 0) && (tmp < 248))
		{
		MbsWpPkg.Result   = TRUE;
		if(*pValue != tmp)
			{
			*pValue   = tmp;
			 SetMbsID(*pValue);
			 SysParam.EepromFlag = 1;
			}
		}
}

static void WpInputFt(void)
{
	u16 *pValue = &MbsReg.InputFt,tmp;

	tmp = MbsWpPkg.PcSetVal[1];
	if(tmp < 8)
		{
		MbsWpPkg.Result   = TRUE;
		if(*pValue != tmp)
			{
			*pValue   = tmp;
			 SysParam.EepromFlag = 1;
			}
		}
}

_ROM struct tMbsRegWp RegWpTable[] = 
{
/* System State */
{0,2,REG_W2R_RW ,MBSREG_HOLD_MBSID    ,WpMbsID   					 },
{0,2,REG_W2R_RW ,MBSREG_HOLD_INFT     ,WpInputFt				     },
{0,0,0,0}
};

GLOBAL u8 WpReg(u16 Addr,u8 RxdBuf[],u8 *len)
{
	u8 i,n;

	for(i=0;RegWpTable[i].Byten;i++)
		{
		if(Addr == RegWpTable[i].Addr)
			{
			*len = RegWpTable[i].Byten;
			if(RegWpTable[i].W2R == REG_W2R_R)//Ö»¶Á¼Ä´æÆ÷
				return SET_ERROR;
			
			MbsWpPkg.Index  = RegWpTable[i].Index;
			MbsWpPkg.Result = FALSE;
			for(n=0;n<RegWpTable[i].Byten;n++)
				MbsWpPkg.PcSetVal[n] = RxdBuf[n];
			
			RegWpTable[i].pFunc();
			if(MbsWpPkg.Result == FALSE)
				return SET_ERROR;
			else
				return SET_OK;
			}
		}
	return SET_UNKNOWU;
}

