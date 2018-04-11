
#include "inkersys.h"


/*  EEPROM  SFR */
sfr ISP_DATA   =0xC2;
sfr ISP_ADDRH  =0xC3;
sfr ISP_ADDRL  =0xC4;
sfr ISP_CMD    =0xC5;
sfr ISP_TRIG   =0xC6;


#define RdCommand		0x01
#define PrgCommand		0x02
#define EraseCommand    0x03
#define Error           1
#define Ok              0
#define WaitTime    	0x01 // 0X03
#define PerSector		512

/* ================ 打开 ISP,IAP 功能 ================= */
static void ISP_IAP_enable(void)
{
	EA	=	0;			
	ISP_CONTR =	ISP_CONTR & 0x18;       
	ISP_CONTR =	ISP_CONTR | WaitTime;
	ISP_CONTR =	ISP_CONTR | 0x80;     
}

/* =============== 关闭 ISP,IAP 功能 ================== */
static void ISP_IAP_disable(void)
{
	ISP_CONTR	=	ISP_CONTR & 0x7f;
	ISP_TRIG	=	0x00;
	EA			=   1;		
}
/* ================ 公用的触发代码 ==================== */
static void ISPgoon(void)
{
	ISP_IAP_enable();		
	ISP_TRIG	=	0x5A;	
	ISP_TRIG	=	0xA5;		
}

/* ================== 扇区擦除 ======================== */
void EraseSector(u16 SectorAddr)
{
	unsigned int _XDATA iSectorAddr;
	iSectorAddr = (SectorAddr & 0xfe00);
	ISP_ADDRH = (unsigned char)(iSectorAddr >> 8);
	ISP_ADDRL = 0x00;

	ISP_CMD	= ISP_CMD & 0xf8;			
	ISP_CMD	= ISP_CMD | EraseCommand;	
	ISPgoon();							
	ISP_IAP_disable();
}

/* ==================== 字节读 ======================== */
u8 ReadByte(u16 ByteAddr)
{
	ISP_ADDRH = (unsigned char)(ByteAddr >> 8);	
	ISP_ADDRL = (unsigned char)(ByteAddr & 0x00ff);

	ISP_CMD   = ISP_CMD	& 0xf8;			
	ISP_CMD   = ISP_CMD	| RdCommand;

	ISPgoon();						
	ISP_IAP_disable();				

	return (ISP_DATA);			
}


/* ==================== 字节写 ======================== */
void SaveByte(u16 ByteAddr, u8 val)
{
	ISP_ADDRH =	(unsigned char)(ByteAddr >> 8); 
	ISP_ADDRL =	(unsigned char)(ByteAddr & 0x00ff);

	ISP_CMD	  = ISP_CMD & 0xf8;				
	ISP_CMD   = ISP_CMD | PrgCommand;		
	ISP_DATA  = val;		

	ISPgoon();							
	ISP_IAP_disable();				
}


