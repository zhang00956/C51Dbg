
#include "inkersys.h"

void EraseBlock(u16 addr)
{
	EraseSector(addr);
}

void SaveU08Value(u16 addr,u8 value)
{
	SaveByte(addr,value);
}


void ReadU08Value(u16 addr,u8 * value)
{
	*value = ReadByte(addr);
}

void SaveU16Value(u16 addr, u16 SaveValue)
{
	SaveByte(addr,MSB(SaveValue));
	SaveByte(addr + 1,LSB(SaveValue));
}

void ReadU16Value(u16 addr, u16 *ReadValue)
{
	*ReadValue   = ReadByte(addr);
	*ReadValue <<= 8;
	*ReadValue  |= ReadByte(addr+1);
}

void SaveU32Value(u16 addr, u32 SaveValue)
{
	unsigned char buf[4],i;
	DWORDToArray(SaveValue,buf);
	for(i=0;i<4;i++)
		{
		SaveU08Value(addr + i,buf[i]);
		}
}

void ReadU32Value(u16 addr, u32 *ReadValue)
{
	unsigned char buf[4],i;
	for(i=0;i<4;i++)
		{
		ReadU08Value(addr + i,buf+i);
		}
	*ReadValue = ArrayToDword(buf);
}

void SaveF32Value(u16 addr, float SaveValue)
{
	unsigned char buf[4],i;
	DWORDToArray(ConvertFloat(SaveValue),buf);
	for(i=0;i<4;i++)
		{
		SaveU08Value(addr + i,buf[i]);
		}
}

void ReadF32Value(u16 addr, float *ReadValue)
{
	unsigned char buf[4],i;
	for(i=0;i<4;i++)
		{
		ReadU08Value(addr+i,buf+i);
		}
	*ReadValue = ConvertLong(ArrayToDword(buf));
}

