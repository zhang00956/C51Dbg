
#define SYSLOG 0

#include "inkersys.h"
#include "mbsrtu.h"

static void IoPortInit(void);

GLOBAL void Stc125a60s2Init(void)
{
	IoPortInit();
	Uart0Init(115200);
}

GLOBAL void StcIap(void)/* 重启并检查是否升级 */
{
	ISP_CONTR = 0X60;
} 


GLOBAL void StcCarId(u8 *p,u8 size)
{
	u8 idata *idata_p,i;

	idata_p = 0XF1;
	for(i=0;i<size;i++)
		{
		*p = *idata_p;
		p++;
		idata_p++;
		}
}

GLOBAL void PowerDownMode(void)
{
	WAKE_CLKO = 0X40;//配置由串口进行唤醒
	PCON     |= 0X02;//进入掉电模式
}


/************************************************************************/
/*      Local Functions                                                 */
/************************************************************************/
static void IoPortInit(void)
{
/*
PnM1  PnM0	 IO模式
0	  0 	 准双向口
0	  1 	 强推免输出
1	  0 	 输入模式
1	  1 	 开漏模式
*/

	P0M1    = 0x00;
	P0M0    = 0x00;

	P1M1    = 0x00;
	P1M0    = 0x00;
	
	P2M1    = 0x00;
	P2M0    = 0x00;

	P3M1    = 0x00;
	P3M0    = 0x04;

	P4SW    = 0X70;//P4.6 P4.5 P4.4 做普通管脚使用
	P4M1	= 0x00;
	P4M0    = 0x80;
}

GLOBAL void ConfigPort(u8 port,u8 pxm1,u8 pxm0)
{
	switch(port)
		{
		case 0:
			P0M1    = pxm1;
			P0M0    = pxm0;
			break;
			
		case 1:
			P1M1    = pxm1;
			P1M0    = pxm0;
			break;
			
		case 2:
			P2M1    = pxm1;
			P2M0    = pxm0;
			break;
			
		case 3:
			P3M1    = pxm1;
			P3M0    = pxm0;
			break;
			
		case 4:
			P4M1    = pxm1;
			P4M0    = pxm0;
			break;	
		}
}

GLOBAL void SetPort(u8 port,u8 bits,bool state)
{
	switch(port)
		{
		case 0:
			state?(P0|=bits):(P0&=~bits);
			break;
		case 1:
			state?(P1|=bits):(P1&=~bits);
			break;
		case 2:
			state?(P2|=bits):(P2&=~bits);
			break;
		case 3:
			state?(P3|=bits):(P3&=~bits);
			break;
		case 4:
			state?(P4|=bits):(P4&=~bits);
			break;	
		}
}

GLOBAL u8 GetPort(u8 port)
{
	switch(port)
		{
		case 0:
			return P0;
		case 1:
			return P1;
		case 2:
			return P2;
		case 3:
			return P3;
		case 4:
			return P4;
		}
	return 0;
}

/*UART 0*/
vu8 TxdFlag = 0,RxdFlag = 0,RxdData = 0;
GLOBAL void Uart0Init(u32 Baudrate) //115200bps@22.1184MHz-TIMER1-1T
{
#ifndef KEIL_DEBUG
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON  = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
	BRT   = -(SYSTEM_OSC_FCLK/16/Baudrate);		//设定独立波特率发生器重装值
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
	ES	  = 1;
#else
	Baudrate = 0;
	SCON  = 0X50;
	TMOD |= 0X20;
	TH1   = 0XFF;
	TR1   = 1;
	//TI  = 1;
	ES    = 1;
#endif
}

GLOBAL void Uart0Send(u8 send_data)
{
	while(TxdFlag);
	SBUF    = send_data;
	TxdFlag = 1;//发送忙状态
}

void UART0IntHandler(void) interrupt 4
{
	if(RI)
		{
		RI = 0;
		RxdFlag = 1;		
		RxdData = SBUF;
		MbsRxd(RxdData);
		}

	if(TI)
		{
		TI = 0;
		TxdFlag = 0;//发送空闲状态
		}
}

/* Adc_Soc */


GLOBAL void ProB_RealTime_StcMcu(void)
{
	if(WAKE_CLKO)/*掉电唤醒清除*/
		{
		WAKE_CLKO = 0X00;
		RxdData = '0';/*SBUF 寄存器会保存掉电前的值 所以必须清除*/
		}
}

/* InPut */
GLOBAL u8 GetInPut1(void)
{
	return PIN_INPUT1;
}

GLOBAL u8 GetInPut2(void)
{
	return PIN_INPUT2;
}

/* OutPut */





/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/

#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool McuBsp_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".McuBsp\r\n"));
		return FALSE;
		}
		
	if(strcmp(pComDbg.Revdata,".mcubsp\r\n") == NULL)
		{
		DPRINTF(("注意操作P3串口脚会导致通讯失效!\r\n"));
		DPRINTF(("Iap;\r\n"));
		DPRINTF(("Reset;\r\n"));
		DPRINTF(("PowerDown;\r\n"));
		DPRINTF(("SetBardrate <u32>;\r\n"));
		DPRINTF(("CfgPort <port> <pxm1> <pxm0>;\r\n"));
		DPRINTF(("SetPort <port> <bits> <bool>;\r\n"));
		DPRINTF(("GetPort <port>;\r\n"));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"iap\r\n") == NULL)
		{
		DPRINTF(("Count Down:30S\r\n"));
		pComDbg.Param16[0] = 600;
		while((pComDbg.Param16[0]--) && (RxdFlag == 0))
			{
			ProB_RealTime_WatchDogFeed_Soc();
			Delay1mS(50);
			if(pComDbg.Param16[0]%20 == 0)
				DPRINTF(("Count Down:%002dS\r\n",pComDbg.Param16[0]/20));
			}
		StcIap();
		}
	
	if(strcmp(pComDbg.Revdata,"reset\r\n") == NULL)
		{
		DPRINTF(("Software Reset!\r\n"));
		while(1);// 等待看门狗复位
		}
	
	if(strcmp(pComDbg.Revdata,"powerdown\r\n") == NULL)
		{
		DPRINTF(("Mcu Power Down Mode!"));
		PowerDownMode();
		}
	
	if(strcmp(pComDbg.Revdata,"setbardrate ") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU32(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Paramu32) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		DPRINTF(("Uart0 Brt:%lu\r\n",pComDbg.Paramu32[0]));
		Uart0Init(pComDbg.Paramu32[0]);
		return TRUE;
		}
		
	if(strcmp(pComDbg.Revdata,"cfgport") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8) == FALSE)
			return FALSE;
		if(TypeU8(pComDbg.Revdata+pComDbg.Cnt,pComDbg.Param8+1) == FALSE)
			return FALSE;
		if(TypeU8(pComDbg.Revdata+pComDbg.Cnt,pComDbg.Param8+2) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;
		
		ConfigPort(pComDbg.Param8[0],pComDbg.Param8[1],pComDbg.Param8[2]);
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"setport") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8) == FALSE)
			return FALSE;
		if(TypeU8(pComDbg.Revdata+pComDbg.Cnt,pComDbg.Param8+1) == FALSE)
			return FALSE;
		if(TypeU8(pComDbg.Revdata+pComDbg.Cnt,pComDbg.ParamB) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;
		
		SetPort(pComDbg.Param8[0],pComDbg.Param8[1],pComDbg.ParamB[0]);
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"getport") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU8(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param8) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		DPRINTF(("P%bd == %b002X\r\n",pComDbg.Param8[0],GetPort(pComDbg.Param8[0])));
		return TRUE;
		}

	return FALSE;
}

#endif

/* #End Comdbg Interface */

