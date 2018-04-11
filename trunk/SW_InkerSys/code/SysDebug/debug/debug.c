
#include "inkersys.h"

#ifdef _GLOBAL_DEBUG_ENABLE

u8 _IDATA PrintfBuf[64];
#define DbgSendByte(m) Uart0Send(m)

u8 debug_printf(char *format,...)
{
	u8 length = 0,i = 0;

	va_list arg_ptr;

	va_start (arg_ptr, format);           /* format string */
	vsprintf (PrintfBuf, format, arg_ptr);
	va_end (arg_ptr);
	length = strlen(PrintfBuf);
	for(i=0;i<length;i++)
		DbgSendByte(PrintfBuf[i]);

	return length;
}

void PrfArray(u8 str[],u8 len)
{
	u8 _IDATA i;
	debug_printf("\r\n");
	debug_printf("     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
	for(i=0; i<len; i ++)
		{
		if ( i%16 == 0 )
			debug_printf("\r\n0x%b0.2X:",i);
		debug_printf("%b0.2X ", str[i]);
		}
	debug_printf("\r\n");
}

#endif



