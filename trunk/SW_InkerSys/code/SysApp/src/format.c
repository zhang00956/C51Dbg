
#include "inkersys.h"

_ROM u8 BitTable[8]       = {0X01,0X02,0X04,0X08,0X10,0X20,0X40,0X80};

void DWORDToArray (unsigned long dwMsg, unsigned char *message_array)
{
	message_array[3] = (unsigned char)(dwMsg & 0xff);	/* LSB */
	message_array[2] = (unsigned char)((dwMsg >> 8) & 0xff);
	message_array[1] = (unsigned char)((dwMsg >> 16)& 0xff);
	message_array[0] = (unsigned char)((dwMsg >> 24) & 0xff);	/* MSB */
}

unsigned long ArrayToDword (unsigned char *message_array)
{
	unsigned long dwMsg;
	dwMsg = 0;
	dwMsg |= message_array[0];
	dwMsg <<=8;
	dwMsg |= message_array[1];
	dwMsg <<=8;
	dwMsg |= message_array[2];
	dwMsg <<=8;
	dwMsg |= message_array[3];
	return dwMsg;
}

unsigned long ConvertFloat(float value)
{
	unsigned long *rtn_value;
	rtn_value = (unsigned long*)&value;
	return *rtn_value;
}


float ConvertLong(unsigned long value)
{
	float *rtn_value;
	rtn_value = (float*)&value;
	return *rtn_value;
}



bool IsChineseFont(const char* pcText)
{
	unsigned char ByteH,ByteL;
	ByteH = *pcText;
	ByteL = *(pcText+1);
	if((ByteH > 0xA0) && (ByteL > 0xA0))
		return 1;
	return 0;	
}

#if 0
static _ROM u8 Hex2DecTab[] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0};
u8 Hex2Dec(u8 HexValue)
{
	u8 RtnValue;
	RtnValue = Hex2DecTab[HexValue >> 4];
	RtnValue *= 10;
	RtnValue += Hex2DecTab[HexValue & 0xF];
	return RtnValue;
}

u8 Dec2Hex(u8 DecValue)
{
	u8 RtnValue;
	RtnValue = (DecValue/10) *16 + (DecValue % 10);
	return RtnValue;
}
#endif


/************************************************************************/
/*     Comdbg Interface                                                 */
/************************************************************************/

#ifdef _GLOBAL_COMDBG_ENABLE

#include "comdbg.h"

GLOBAL bool Format_Comdbg(void)
{
	if(strcmp(pComDbg.Revdata,"help\r\n") == NULL)
		{
		DPRINTF((".Format\r\n"));
		return FALSE;
		}
		
	if(strcmp(pComDbg.Revdata,".format\r\n") == NULL)
		{
		DPRINTF(("FtoH  <float> Float to Hex;\r\n"));
		DPRINTF(("HtoF H<32bit> Hex to Float;\r\n"));
		DPRINTF(("D16H  <u16>   Dec  u16 Hex;\r\n"));
		DPRINTF(("H16D H<u16>   Hex  u16 Dec;\r\n"));
		DPRINTF(("D32H  <u32>   Dec  u32 Hex;\r\n"));
		DPRINTF(("H32D H<u32>   Hex  u32 Dec;\r\n"));
		DPRINTF(("S2H   <str>   Str  to  Hex;\r\n"));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"ftoh") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeFloat(pComDbg.Revdata + pComDbg.Cnt,pComDbg.ParamF + 0) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;
		
		DWORDToArray(ConvertFloat(pComDbg.ParamF[0]),pComDbg.Array);

		DPRINTF(("%f => %b002X %b002X %b002X %b002X\r\n",pComDbg.ParamF[0],pComDbg.Array[0],pComDbg.Array[1],pComDbg.Array[2],pComDbg.Array[3]));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"htof") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU32(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Paramu32) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;
		pComDbg.ParamF[0] = ConvertLong(pComDbg.Paramu32[0]);

		DPRINTF(("0X%000000008lX => %f\r\n",pComDbg.Paramu32[0],pComDbg.ParamF[0]));
		return TRUE;
		}
		
	if(strcmp(pComDbg.Revdata,"d16h") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU16(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param16) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;
		DPRINTF(("%d => 0X%00004X\r\n",pComDbg.Param16[0],pComDbg.Param16[0]));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"h16d") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU16(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Param16) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		DPRINTF(("0X%00004X => %u\r\n",pComDbg.Param16[0],pComDbg.Param16[0]));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"d32h") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU32(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Paramu32) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		DPRINTF(("%lu => 0X%000000008lX\r\n",pComDbg.Paramu32[0],pComDbg.Paramu32[0]));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"h32d") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeU32(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Paramu32) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		DPRINTF(("0X%000000008lX => %lu\r\n",pComDbg.Paramu32[0],pComDbg.Paramu32[0]));
		return TRUE;
		}

	if(strcmp(pComDbg.Revdata,"s2h") == NULL)
		{
		pComDbg.Cnt = SearchChar(pComDbg.Revdata,'\0')+1;
		if(TypeString(pComDbg.Revdata + pComDbg.Cnt,pComDbg.Str) == FALSE)
			return FALSE;
		if(EndIsEnter(pComDbg.Revdata + pComDbg.Cnt -1) == FALSE)
			return FALSE;

		PrfArray(pComDbg.Str,strlen(pComDbg.Str));
		
		return TRUE;
		}
		
	return FALSE;
}

#endif

/* #End Comdbg Interface */

