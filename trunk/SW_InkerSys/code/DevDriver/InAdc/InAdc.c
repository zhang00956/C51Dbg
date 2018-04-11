
#define SYSLOG 0

#include "inkersys.h"
#include "InAdc.h"


#define INADC_CH_NUM  2
#define INADC_BUF_NUM 5

// 各通道采集缓存数据个数
static u8 _XDATA AdcBuf[INADC_CH_NUM][INADC_BUF_NUM]={
{65,45,28,34,75},
{14,36,11,45,26}};

static u8 _XDATA CalcBuf[INADC_BUF_NUM];

static u8 FtAdcVal(u8 Buf[])
{
	u8 i,j,tmp8;

	for(i=0;i<INADC_BUF_NUM;i++)	
		CalcBuf[i] = Buf[i];
	
	for(i=0;i<INADC_BUF_NUM-1-i;i++)
		for(j=0;j<INADC_BUF_NUM-1-i;j++)
			{
			if(CalcBuf[j]>CalcBuf[j+1])
				{
				tmp8=CalcBuf[j];
				CalcBuf[j]=CalcBuf[j+1];
				CalcBuf[j+1]=tmp8;
				}
			}
		
	return CalcBuf[2];// 排列采集个数为5个 所以取中间2的数据
}

GLOBAL void ProB_1S_GetAdc(void)
{
	static u8 _XDATA Cnt = 0;
#ifndef KEIL_DEBUG	
	u8 i;

	for(i=0;i<INADC_CH_NUM;i++)
		AdcBuf[i][Cnt] = ReadAdcCh(i);
#endif
	Cnt ++;
	if(Cnt >= INADC_BUF_NUM)
		Cnt = 0;
}

GLOBAL u8 ReadAdcChVal(u8 Ch)// Ch<0..7
{
	if(Ch < INADC_CH_NUM)
		return FtAdcVal(AdcBuf[Ch]);

	return 0;
}


/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/
#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool InAdc_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".InAdc\r\n"));
		return FALSE;
		}

	if(strcmp(pComDbg.Revdata,".inadc\r\n") == NULL)
		{
		DPRINTF(("InAdcmsg\r\n"));
		return TRUE;
		}
		
	if(strcmp(pComDbg.Revdata,"inadcmsg\r\n") == NULL)
		{
		u8 i;
		
		DPRINTF(("AdcChNum   :%bd\r\n",INADC_CH_NUM));
		DPRINTF(("AdcChBufNum:%bd\r\n",INADC_BUF_NUM));
		for(i=0;i<INADC_CH_NUM;i++)
			DPRINTF(("AdcCh%bd:%bd\r\n",i,ReadAdcChVal(i)));
		return TRUE;
		}

	return FALSE;
}

#endif



