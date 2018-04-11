#include "inkersys.h"
#include "lamp.h"
#include "beep.h"
#include "key.h"
#include "input.h"
#include "mbsrtu.h"

#define  HEARBEAT_RATE   1000       /*Hz 心率*/

#ifndef KEIL_DEBUG
/* 1T模式 */
#define HEARBEAT_FIX 70 //修正定时器进入入栈时间 系数值 3*SYSTEM_OSC_FCLK/1000000 - 2
#define TH0VAL (MSB(0XFFFF - (SYSTEM_OSC_FCLK/HEARBEAT_RATE - HEARBEAT_FIX)))
#define TL0VAL (LSB(0XFFFF - (SYSTEM_OSC_FCLK/HEARBEAT_RATE - HEARBEAT_FIX)))
#else // Keil软件仿真使用12T模式计数
/*12T模式*/
#define TH0VAL (MSB(0XFFFF - (SYSTEM_OSC_FCLK/12/HEARBEAT_RATE)))
#define TL0VAL (LSB(0XFFFF - (SYSTEM_OSC_FCLK/12/HEARBEAT_RATE)))
#endif

vu8 SysTimer10mS = 0;	/*Unit: mS*/

static vu8 Delay1mSVal = 0;//有效延时范围 1~255 mS

void SystickIntHandler(void) interrupt 1 //1MS
{
	static u8 TickPhase = 0;

	TL0 = TL0VAL;
	TH0 = TH0VAL; 

	TickPhase ++;
	if(Delay1mSVal)
		Delay1mSVal --;
		
	switch(TickPhase)
		{
		case 1:
			ProF_10mS_Lamper();
			break;
		case 2:
			ProF_10mS_Beeper();
			break;	
		case 3:
			ProF_10mS_Key();
			break;
		case 4:
			ProF_10mS_InPut();
			break;
		case 10:
			TickPhase = 0;
			SysTimer10mS ++;
			ProF_10mS_Wathdog_Soc();
			ProF_10mS_RunTimeCnt();
			ProF_10mS_MbsTimeOut();
			break;
		}
}

GLOBAL void Delay1mS(u8 ms_value)
{
	Delay1mSVal = ms_value;
	while(Delay1mSVal);
}

GLOBAL void HearBeatInit(void)
{
	AUXR |= 0x80;		  //定时器时钟1T模式
    TMOD  = TMOD|0x01;     //Timer0工作方式一16位计数
	TH0   = TH0VAL;        //定时1MS
	TL0   = TL0VAL;
	ET0   = 1;		      //定时器0允许中断
	TR0   = 1;             //开始计数
}

