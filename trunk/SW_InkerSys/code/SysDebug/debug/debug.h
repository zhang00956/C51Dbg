#ifndef _DEBUG_H
#define _DEBUG_H

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef _GLOBAL_DEBUG_ENABLE
	extern  u8 debug_printf(char *format,...);
	extern void PrfArray(u8 str[],u8 len);
	#ifdef SYSLOG
		#define DPRINTF(a) 		   {debug_printf a;}
	#else
		#define DPRINTF(a)
	#endif
#else
	#define DPRINTF(a)
	#define PrfArray(a,b)
#endif

#endif

