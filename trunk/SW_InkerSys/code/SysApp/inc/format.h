#ifndef _FORMAT_H
#define	_FORMAT_H

extern _ROM u8 BitTable[8];

extern void DWORDToArray (unsigned long dwMsg, unsigned char *message_array);
extern unsigned long ArrayToDword (unsigned char *message_array);
extern unsigned long ConvertFloat(float value);
extern float ConvertLong(unsigned long value);
extern bool  IsChineseFont(const char* pcText);
//extern u8 Hex2Dec(u8 HexValue);
//extern u8 Dec2Hex(u8 DecValue);

#endif
