#define SYSLOG 0

#include "inkersys.h"
#include "crc.h"
#include "MbsRtu.h"


#define MODBUS_MAX_SIZE 64

enum{
MODBUS_DEVICE_ID,
MODBUS_CMD,
MODBUS_CMD_0X01,//Read Coils               [0X0000~0XFFFF] RW  8K
MODBUS_CMD_0X02,//Read Discrete InputS     [0X0000~0XFFFF] R   8K
MODBUS_CMD_0X03,//Read Holding Register    [0X0000~0XFFFF] RW 64K
MODBUS_CMD_0X04,//Read Input Registers     [0X0000~0XFFFF] R  64K
MODBUS_CMD_0X05,//Write Single Coil        
MODBUS_CMD_0X06,//Write Single Register    
MODBUS_CMD_0X0F,//Write MultipleCoils      
MODBUS_CMD_0X10,//Write multiple registers 
MODBUS_CRC,
};

typedef struct{
u8   cmd;
u8   state;
}tModbusCmdState;

_ROM tModbusCmdState CmdTable[] = 
{{0X01,MODBUS_CMD_0X01},
 {0X02,MODBUS_CMD_0X02},
 {0X03,MODBUS_CMD_0X03},
 {0X04,MODBUS_CMD_0X04},
 {0X05,MODBUS_CMD_0X05},
 {0X06,MODBUS_CMD_0X06},
 {0X0F,MODBUS_CMD_0X0F},
 {0X10,MODBUS_CMD_0X10},
 {0,0}};

typedef struct{
u8   CurSlaveId;	
//{
u8   Address;
u8   Function;
u8   Buf[MODBUS_MAX_SIZE];
//}
u16  Crc;

u8   BufCnt;

u8	 RecvStatus;
u8   RecvFrameFull;
u8   RecvCnt;
u8   ByteTimeOut; //字节间超时判断 50mS

u8   MbsId;
u8 	 MbsFrameEn;
u8 	 MbsCrcEn;
}tModbusPkg;

u8 _XDATA ModbusTxdBuf[MODBUS_MAX_SIZE];

tModbusPkg _XDATA ModbusPkg;

void ModbusSend(u8 *p,u8 length)
{
	u8 i;
	//PIN_485EN = 1;
	//Delay1mS(3);
	for(i=0;i<length;i++)
		{
		Uart0Send(*p);
		p++;
		}
	//Delay1mS(3);
	//PIN_485EN = 0;
}

/*//////////////////////////////////////////////////////////
Function
*/

/* 读取线圈状态 */
static void ReadCoils(void)
{
	u16 addr,num,crc;
	u8 cnt=0;
	
	addr   = ModbusPkg.Buf[cnt++];
	addr <<= 8;
	addr  |= ModbusPkg.Buf[cnt++];

	num    = ModbusPkg.Buf[cnt++];
	num  <<= 8;
	num   |= ModbusPkg.Buf[cnt++];

	if((num/8) > (MODBUS_MAX_SIZE - 6))//超出发送缓存长度
		return ;
	
	if(MbsReg0X01(addr,num,ModbusTxdBuf+3) == TRUE)
		{
		ModbusTxdBuf[0]   = ModbusPkg.MbsId;
		ModbusTxdBuf[1]   = ModbusPkg.Function;
		if(num%8)			
			ModbusTxdBuf[2]   = num/8 + 1;
		else
			ModbusTxdBuf[2]   = num/8;
		
		cnt = ModbusTxdBuf[2] + 3;
		crc = cal_crc(ModbusTxdBuf, cnt);
	
		ModbusTxdBuf[cnt++]   = LSB(crc);
		ModbusTxdBuf[cnt++]   = MSB(crc);

		ModbusSend(ModbusTxdBuf,cnt);
		}
}

/*读取单个或多个寄存器值*/
static void ReadHoldingRegisters(void)//仅当做从机时访问
{
	u16 addr,num,crc;
	u8 cnt=0;
	addr   = ModbusPkg.Buf[cnt++];
	addr <<= 8;
	addr  |= ModbusPkg.Buf[cnt++];

	num    = ModbusPkg.Buf[cnt++];
	num  <<= 8;
	num   |= ModbusPkg.Buf[cnt++];

	if((num*2) > (MODBUS_MAX_SIZE - 6))//超出发送缓存长度
		return ;
	
	if(MbsReg0X03(addr,num,ModbusTxdBuf+3) == TRUE)
		{
		cnt = 0;
		ModbusTxdBuf[cnt++]   = ModbusPkg.MbsId;
		ModbusTxdBuf[cnt++]   = ModbusPkg.Function;
		ModbusTxdBuf[cnt++]   = num*2;
		cnt += num*2;
		crc = cal_crc(ModbusTxdBuf, cnt);
	
		ModbusTxdBuf[cnt++]   = LSB(crc);
		ModbusTxdBuf[cnt++]   = MSB(crc);

		ModbusSend(ModbusTxdBuf,cnt);
		}
}

/*设置单个线圈*/
static void WriteSingleCoil(void)
{
	u16 addr,val,crc;
	u8 cnt = 0;
	
	addr   = ModbusPkg.Buf[cnt++];
	addr <<= 8;
	addr  |= ModbusPkg.Buf[cnt++];

	val    = ModbusPkg.Buf[cnt++];
	val  <<= 8;
	val   |= ModbusPkg.Buf[cnt++];
	
	if(MbsReg0X05(addr,val) == TRUE)
		{
		cnt = 0;
		ModbusTxdBuf[cnt++]   = ModbusPkg.MbsId;
		ModbusTxdBuf[cnt++]   = ModbusPkg.Function;
		ModbusTxdBuf[cnt++]   = MSB(addr);
		ModbusTxdBuf[cnt++]   = LSB(addr);
		ModbusTxdBuf[cnt++]   = MSB(val);
		ModbusTxdBuf[cnt++]   = LSB(val);

		crc = cal_crc(ModbusTxdBuf, cnt);
		ModbusTxdBuf[cnt++]   = LSB(crc);
		ModbusTxdBuf[cnt++]   = MSB(crc);

		ModbusSend(ModbusTxdBuf,cnt);
		}
}

/*设置单个寄存处值*/
static void WriteSingleRegister(void)
{
	u16 addr,val,crc;
	u8 cnt = 0;
	
	addr   = ModbusPkg.Buf[cnt++];
	addr <<= 8;
	addr  |= ModbusPkg.Buf[cnt++];

	val    = ModbusPkg.Buf[cnt++];
	val  <<= 8;
	val   |= ModbusPkg.Buf[cnt++];
	
	if(MbsReg0X06(addr,val) == TRUE)
		{
		cnt = 0;
		ModbusTxdBuf[cnt++]   = ModbusPkg.MbsId;
		ModbusTxdBuf[cnt++]   = ModbusPkg.Function;
		ModbusTxdBuf[cnt++]   = MSB(addr);
		ModbusTxdBuf[cnt++]   = LSB(addr);
		ModbusTxdBuf[cnt++]   = MSB(val);
		ModbusTxdBuf[cnt++]   = LSB(val);

		crc = cal_crc(ModbusTxdBuf, cnt);
		ModbusTxdBuf[cnt++]   = LSB(crc);
		ModbusTxdBuf[cnt++]   = MSB(crc);

		ModbusSend(ModbusTxdBuf,cnt);
		}
}

/* 设置单个或多个线圈 */
static void WriteMultipleCoils(void)
{
	u16 addr,num,crc;
	u8 cnt = 0;
	addr   = ModbusPkg.Buf[cnt++];
	addr <<= 8;
	addr  |= ModbusPkg.Buf[cnt++];

	num    = ModbusPkg.Buf[cnt++];
	num  <<= 8;
	num   |= ModbusPkg.Buf[cnt++];

	cnt++;
	//size   = ModbusPkg.Buf[cnt++];

	if(MbsReg0X0F(addr,num,ModbusPkg.Buf + cnt) == TRUE)
		{
		cnt = 0;
		ModbusTxdBuf[cnt++]   = ModbusPkg.MbsId;
		ModbusTxdBuf[cnt++]   = ModbusPkg.Function;
		ModbusTxdBuf[cnt++]   = MSB(addr);
		ModbusTxdBuf[cnt++]   = LSB(addr);
		ModbusTxdBuf[cnt++]   = MSB(num);
		ModbusTxdBuf[cnt++]   = LSB(num);

		crc = cal_crc(ModbusTxdBuf, cnt);
		ModbusTxdBuf[cnt++]   = LSB(crc);
		ModbusTxdBuf[cnt++]   = MSB(crc);

		ModbusSend(ModbusTxdBuf,cnt);
		}
}

/*设置单个或多个寄存处值*/
static void WriteMultipleRegisters(void)
{
	u16 addr,num,crc;
	u8 cnt = 0;
	addr   = ModbusPkg.Buf[cnt++];
	addr <<= 8;
	addr  |= ModbusPkg.Buf[cnt++];

	num    = ModbusPkg.Buf[cnt++];
	num  <<= 8;
	num   |= ModbusPkg.Buf[cnt++];

	cnt++;
	//size   = ModbusPkg.Buf[cnt++];

	if(MbsReg0X10(addr,num,ModbusPkg.Buf + cnt) == TRUE)
		{
		cnt = 0;
		ModbusTxdBuf[cnt++]   = ModbusPkg.MbsId;
		ModbusTxdBuf[cnt++]   = ModbusPkg.Function;
		ModbusTxdBuf[cnt++]   = MSB(addr);
		ModbusTxdBuf[cnt++]   = LSB(addr);
		ModbusTxdBuf[cnt++]   = MSB(num);
		ModbusTxdBuf[cnt++]   = LSB(num);

		crc = cal_crc(ModbusTxdBuf, cnt);
		ModbusTxdBuf[cnt++]   = LSB(crc);
		ModbusTxdBuf[cnt++]   = MSB(crc);

		ModbusSend(ModbusTxdBuf,cnt);
		}
}

static void AnalyseModbusRxdPkg(void)
{
	switch(ModbusPkg.Function)
		{
		case 0X01:
			ReadCoils();
			break;
			
		case 0X03:
			ReadHoldingRegisters();
			break;

		case 0X05:
			WriteSingleCoil();
			break;
		
		case 0X06:
			WriteSingleRegister();
			break;

		case 0X0F:
			WriteMultipleCoils();
			break;
			
		case 0X10:
			WriteMultipleRegisters();
			break;	
		}
	//DPRINTF(("Modbus Cmd:%b002X!\n",ModbusPkg.Function));
}

/************************************************************************/
/* #If Modbus Eeprom Store                                              */
/************************************************************************/
typedef struct{
u8	  MbsCrcPosEE;
u8	  MbsRxdPrfPosEE;
}tMBS_EEPOM;

#define MbsEeprom(m)	(size_t)&(((tMBS_EEPOM _ROM *)MBS_EEPORM)->m)

GLOBAL void MbsLoadDefault(void)
{
	ModbusPkg.MbsCrcEn    = 0;
	ModbusPkg.MbsFrameEn  = 0;	
}

GLOBAL void MbsReadEeprom(void)
{
	MbsLoadDefault();
	//ReadU08Value(MbsEeprom(MbsCrcPosEE),   &ModbusPkg.MbsCrcEn);
	//ReadU08Value(MbsEeprom(MbsRxdPrfPosEE),&ModbusPkg.MbsFrameEn);
}

GLOBAL void MbsSaveEeprom(void)
{
	MbsLoadDefault();
	//SaveU08Value(MbsEeprom(MbsCrcPosEE),	ModbusPkg.MbsCrcEn);
	//SaveU08Value(MbsEeprom(MbsRxdPrfPosEE), ModbusPkg.MbsFrameEn);
}
/* #End Modbus Eeprom Store */

GLOBAL void SetMbsID(u8 val)
{
	ModbusPkg.MbsId = val;
}

GLOBAL void MbsInit(void)
{
	ModbusPkg.RecvStatus    = MODBUS_DEVICE_ID;
	ModbusPkg.RecvFrameFull = 0;
	ModbusPkg.ByteTimeOut   = 0;
	ModbusPkg.RecvCnt       = 0;

	MbsRegInit();
	SetMbsID(MbsReg.MbsId);
}

GLOBAL void ProF_10mS_MbsTimeOut(void)
{
	if(ModbusPkg.ByteTimeOut)
		ModbusPkg.ByteTimeOut--;
}

GLOBAL void MbsRxd(u8 rxddata)
{
	u8 i;

	if(ModbusPkg.ByteTimeOut == 0)
		ModbusPkg.RecvStatus = MODBUS_DEVICE_ID;
	
	ModbusPkg.ByteTimeOut = 5;
	//Uart0Send(rxddata);
	switch(ModbusPkg.RecvStatus)
		{
		case MODBUS_DEVICE_ID:
			if(rxddata == ModbusPkg.MbsId)
				{
				ModbusPkg.RecvStatus = MODBUS_CMD;
				ModbusPkg.Address    = rxddata;
				ModbusPkg.RecvCnt    = 0;
				}
			break;
			
		case MODBUS_CMD:
			ModbusPkg.RecvStatus  = MODBUS_DEVICE_ID;
			for(i=0;CmdTable[i].cmd;i++)
				{
				if(rxddata == CmdTable[i].cmd)
					{
					ModbusPkg.Function	 = rxddata;
					ModbusPkg.RecvCnt	 = 0;
					ModbusPkg.RecvStatus = CmdTable[i].state;
					break;
					}
				}
			break;

		case MODBUS_CMD_0X01:
		case MODBUS_CMD_0X02:	
		case MODBUS_CMD_0X03:
		case MODBUS_CMD_0X04:
		case MODBUS_CMD_0X05:
			ModbusPkg.Buf[ModbusPkg.RecvCnt] = rxddata;
			ModbusPkg.RecvCnt++;
			if(ModbusPkg.RecvCnt >= 4)
				{
				ModbusPkg.RecvStatus  = MODBUS_CRC;
				ModbusPkg.BufCnt      = ModbusPkg.RecvCnt;
				ModbusPkg.RecvCnt     = 0;
				}
			break;
			
		case MODBUS_CMD_0X06:
			ModbusPkg.Buf[ModbusPkg.RecvCnt] = rxddata;
			ModbusPkg.RecvCnt++;
			if(ModbusPkg.RecvCnt >= 4)
				{
				ModbusPkg.RecvStatus  = MODBUS_CRC;
				ModbusPkg.BufCnt      = ModbusPkg.RecvCnt;
				ModbusPkg.RecvCnt     = 0;
				}
			break;

		case MODBUS_CMD_0X0F:	
		case MODBUS_CMD_0X10:
			ModbusPkg.Buf[ModbusPkg.RecvCnt] = rxddata;
			ModbusPkg.RecvCnt++;
			if((ModbusPkg.RecvCnt > 5) && (ModbusPkg.RecvCnt > ModbusPkg.Buf[4] + 4))
				{
				ModbusPkg.RecvStatus  = MODBUS_CRC;
				ModbusPkg.BufCnt      = ModbusPkg.RecvCnt;
				ModbusPkg.RecvCnt     = 0;
				}
			if(ModbusPkg.RecvCnt > MODBUS_MAX_SIZE)//超出接收缓存
				{
				ModbusPkg.RecvStatus    = MODBUS_DEVICE_ID;
				}
			break;
			
		case MODBUS_CRC:
			ModbusPkg.Crc >>= 8;
			ModbusPkg.Crc  |= rxddata<<8;
			ModbusPkg.RecvCnt++;
			if(ModbusPkg.RecvCnt >= 2)
				{
				ModbusPkg.RecvStatus    = MODBUS_DEVICE_ID;
				ModbusPkg.RecvFrameFull = 1;
				}
			break;
		}
}

GLOBAL void DebugModbusFrame(void)
{
	DPRINTF(("\r\nModbus Rxd Data:\r\n"));
	DPRINTF(("CRC:%00004X\r\n",ModbusPkg.Crc));
	PrfArray(&ModbusPkg.Address,ModbusPkg.BufCnt+2);
}

GLOBAL void ProB_RealTime_MbsRtu(void)
{
	if(ModbusPkg.RecvFrameFull)
		{
		ModbusPkg.RecvFrameFull = 0;
		if(ModbusPkg.MbsFrameEn)
			DebugModbusFrame();
		if((ModbusPkg.MbsCrcEn != 0) || (ModbusPkg.Crc == cal_crc(&ModbusPkg.Address,ModbusPkg.BufCnt + 2)))
			{
			AnalyseModbusRxdPkg();
			}
		}
}

/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/
#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool MbsRtu_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".MbsRtu\r\n"));
		return FALSE;
		}
	
	if((strcmp(pComDbg.Revdata,".mbsrtu\r\n") == NULL))
		{
		DPRINTF(("MbsRtuMsg\r\n"));
		DPRINTF(("MbsRtuCrc   <0,1> CRC调试使能\r\n"));
		DPRINTF(("MbsRtuFrame <0,1> 打印接收数据\r\n"));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"mbsrtumsg\r\n") == NULL)
		{
		DPRINTF(("从机地址:%bd\r\n",ModbusPkg.MbsId));
		DPRINTF(("CRC校验 :%bd\r\n",ModbusPkg.MbsCrcEn));
		DPRINTF(("打印接收:%bd\r\n",ModbusPkg.MbsFrameEn));
		return TRUE;
		}
	
	if(strcmp(pComDbg.Revdata,"mbsrtucrc") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeBool(pComDbg.Revdata + pComDbg.Cnt,pComDbg.ParamB) == FALSE)
			return FALSE;

		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		ModbusPkg.MbsCrcEn = pComDbg.ParamB[0];

		DPRINTF(("CrcDebug <= %bd\r\n",pComDbg.ParamB[0]));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"mbsrtuframe") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeBool(pComDbg.Revdata + pComDbg.Cnt,pComDbg.ParamB) == FALSE)
			return FALSE;

		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		ModbusPkg.MbsFrameEn = pComDbg.ParamB[0];

		DPRINTF(("PrfMbsRxd <= %bd\r\n",pComDbg.ParamB[0]));
		return TRUE;
		}

	return FALSE;
}

#endif


