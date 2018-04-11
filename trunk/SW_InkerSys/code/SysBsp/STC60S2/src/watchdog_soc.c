
#include "inkersys.h"

#define WDT_FLAG        0X80
#define EN_WDT          0X20
#define WDT_CLR         0X10
#define WDT_IDLE        0X08
                              // 22.1184MHz
#define WDT_DIV_CLK_002 0X00  //   35.6mS
#define WDT_DIV_CLK_004 0X01  //   71.1mS
#define WDT_DIV_CLK_008 0X02  //  142.2mS
#define WDT_DIV_CLK_016 0X03  //  284.4mS
#define WDT_DIV_CLK_032 0X04  //  568.8mS
#define WDT_DIV_CLK_064 0X05  //  1.1377S
#define WDT_DIV_CLK_128 0X06  //  2.2755S
#define WDT_DIV_CLK_256 0X07  //  4.5544S


static vu8 _IRAM FeedCnt10mS = 0;

GLOBAL void WatchDogEn(void)
{
	WDT_CONTR = 0X3E;   //2.5S
}

GLOBAL void ProF_10mS_Wathdog_Soc(void)
{
	FeedCnt10mS++;
}

GLOBAL void ProB_RealTime_WatchDogFeed_Soc(void)
{
	if(FeedCnt10mS > 20)// 200mS Ö´ÐÐÒ»´Î
		{
		FeedCnt10mS = 0;
		WDT_CONTR  |=  WDT_CLR;
		}
}

