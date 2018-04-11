#define SYSLOG 0

#include "inkersys.h"
#include "mbsrtu.h"

#include <absacc.h>


tMbsReg _XDATA MbsReg;

/************************************************************************/
/* #If Reg Eeprom Store                                                 */
/************************************************************************/

typedef struct{
u16  MbsIDPosEE;
u16   InputFtPosEE;
}tREG_EEPOM;

#define RegEeprom(m)	(size_t)&(((tREG_EEPOM _ROM *)REG_EEPORM)->m)

GLOBAL void RegLoadDefault(void)
{
	MbsReg.MbsId       = 1;
	MbsReg.InputFt     = 3;
}

GLOBAL void RegReadEeprom(void)
{
	ReadU16Value(RegEeprom(MbsIDPosEE),    &MbsReg.MbsId);
	ReadU16Value(RegEeprom(InputFtPosEE),  &MbsReg.InputFt);
}

GLOBAL void RegSaveEeprom(void)
{
	SaveU16Value(RegEeprom(MbsIDPosEE),	    MbsReg.MbsId);
	SaveU16Value(RegEeprom(InputFtPosEE),	MbsReg.InputFt);
}
/* #End Reg Eeprom Store */

GLOBAL void MbsRegInit(void)
{
	u8 *p,i;

	p = &MbsReg.sCoil;p++;
	for(i=0;i<(MBSREG_COIL_LEN - MBSREG_COIL_START);i++)
		{
		*p = 0;
		p++;
		}
	
	MbsReg.SetAdc0Val = 0X1234;
	MbsReg.MbsId      = 1;
	MbsReg.Adc0Fix    = 2.81718;
	MbsReg.Adc1Fix    = 3.14159;
}

GLOBAL bool MbsReg0X01(u16 addr,u16 num,u8 txd[])
{
	if((addr >= MBSREG_COIL_START) && ((addr+num) <= MBSREG_COIL_LEN))
		{
		u8  *p8,i;
		
		addr -= MBSREG_COIL_START;
		p8 = ((u8*)&MbsReg.sCoil) + addr + 1;
		
		for(i=0;i<num;i++)
			{
			if(*p8)
				txd[i/8] |=  BitTable[i%8];
			else
				txd[i/8] &= ~BitTable[i%8];
			p8++;
			}
		return TRUE;
		}
	
	return FALSE;
}

GLOBAL bool MbsReg0X05(u16 addr,u16 val)
{
	if((addr >= MBSREG_COIL_START) && (addr <= MBSREG_COIL_LEN))
		{
		u8 *p8;
		
		addr -= MBSREG_COIL_START;
		p8 = ((u8*)&MbsReg.sCoil) + addr + 1;

		if(val == 0XFF00)
			*p8 = 1;
		else
			*p8 = 0;
		
		return TRUE;
		}
	
	return FALSE;

}

GLOBAL bool MbsReg0X0F(u16 addr,u16 num,u8 rxd[])
{
	if((addr >= MBSREG_COIL_START) && ((addr+num) <= MBSREG_COIL_LEN))
		{
		u8  *p8,i;
		
		addr -= MBSREG_COIL_START;
		p8 = ((u8*)&MbsReg.sCoil) + addr + 1;

		for(i=0;i<num;i++)
			{
			if(rxd[i/8] & BitTable[i%8])
				*p8 = 1;
			else
				*p8 = 0;
			p8++;
			}
		
		return TRUE;
		}

	return FALSE;

}

GLOBAL bool MbsReg0X03(u16 addr,u16 num,u8 txd[])
{
	if((addr >= MBSREG_HOLD_S) && (addr <= MBSREG_HOLD_LEN))
		{
		u8  *p8,i;
		
		addr -= MBSREG_HOLD_S;
		p8 = ((u8*)&MbsReg.s2Hold) + addr*2 + 2;
		for(i=0;i<num*2;i++)
			{
			txd[i] = *p8;p8++;
			}
		return TRUE;
		}
	return FALSE;
}

GLOBAL bool MbsReg0X06(u16 addr,u16 val)
{
	if((addr >= MBSREG_HOLD_S) && (addr <= MBSREG_HOLD_LEN))
		{
		u8  *p8,len,result;
		
		result = WpReg(addr,(u8*)&val,&len);
		if(result == SET_OK)
			return TRUE;
		if(result == SET_ERROR)
			return FALSE;
		if(result == SET_UNKNOWU)
			{
			addr -= MBSREG_HOLD_S;
			p8 = ((u8*)&MbsReg.s2Hold) + addr*2 + 2;
			*p8 = MSB(val);p8++;
			*p8 = LSB(val);
			}
		return TRUE;
		}
	return FALSE;	
}

GLOBAL bool MbsReg0X10(u16 addr,u16 num,u8 rxd[])
{
	if((addr >= MBSREG_HOLD_S) && ((addr+num) <= MBSREG_HOLD_LEN))
		{
		u8  *p8,i,len,result;
		
		p8 = ((u8*)&MbsReg.s2Hold) + (addr - MBSREG_HOLD_S)*2 + 2;
		for(i=0;i<num*2;)
			{
			result = WpReg(addr+i/2,rxd + i,&len);
			if(result == SET_ERROR)
				return FALSE;
			if(result == SET_OK)
				p8 += 2;
			if(result == SET_UNKNOWU)
				{
				*p8 = rxd[i];p8++;
				*p8 = rxd[i+1];p8++;
				len = 2;
				}
			i += len;
			}
		return TRUE;
		}
	
	return FALSE;
}

/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/
#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

static void RegMsg(void)
{
	DPRINTF(("Coil:%bd",MBSREG_COIL_LEN - MBSREG_COIL_START));
	PrfArray((u8*)&MbsReg.sCoil + 1,MBSREG_COIL_LEN - MBSREG_COIL_START);


	DPRINTF(("Hold:%bd",MBSREG_HOLD_LEN - MBSREG_HOLD_S));
	PrfArray((u8*)&MbsReg.s2Hold + 2,(MBSREG_HOLD_LEN - MBSREG_HOLD_S)*2);
}

GLOBAL bool MbsReg_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".MbsReg\r\n"));
		return FALSE;
		}

	if(strcmp(pComDbg.Revdata,".mbsreg\r\n") == NULL)
		{
		DPRINTF(("MbsRegMsg\r\n"));
		return TRUE;
		}
		
	if(strcmp(pComDbg.Revdata,"mbsregmsg\r\n") == NULL)
		{
		RegMsg();
		return TRUE;
		}

	return FALSE;
}

#endif


