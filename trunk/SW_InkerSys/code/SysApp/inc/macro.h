#ifndef _MACRO_H
#define _MACRO_H

typedef unsigned char  bool;
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

/* main�������жϺ�����ͬ�����ı������붨��Ϊvolatile���� */
typedef volatile unsigned char  vu8;
typedef volatile unsigned short vu16;

#ifndef NULL
#define NULL  0
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define ARRAY_SIZE(m)  (sizeof(m)/sizeof((m)[0]))

#define MSB(x)	(((x)&0xff00)>>8)
#define LSB(x)	((x)&0xff)


#define  GLOBAL
#define _GLOBAL_DEBUG_ENABLE        /*���ڴ�ӡ������Ϣ*/
#define _GLOBAL_COMDBG_ENABLE       /*���ڵ�������    */
#define _GLOBAL_VERSION_ENABLE      /*ϵͳ�汾��Ϣ    */


//MCU Define
//#define SYSTEM_OSC_FCLK 11059200uL    // Hz
//#define SYSTEM_OSC_FCLK 22118400uL    // Hz
#define SYSTEM_OSC_FCLK 24000000uL    // Hz

#if 1
	typedef unsigned char MCUTYPE;
	#define _IRAM    idata
	#define _IDATA   idata
	#define _XDATA   xdata
	#define _CODE    code
	#define _ROM     code
#else
	typedef unsigned int  MCUTYPE;
	#define _XDATA
	#define _IDATA
	#define _CODE
	#define _ROM     const
#endif


#endif

