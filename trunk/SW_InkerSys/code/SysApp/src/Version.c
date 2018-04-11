/*
 * Version.c
 *
 * Copyright (C) 2012 Alsor Chen
 *
 * This file is PH2803_MAIN version api
 *
 * Change Activity:
 *
 * 2012-04-05 by Alsor Chen
 *   First create.
 *   Put version information into this file, Set this file to always builded to update the time information
 *   2012-10-22 by Alsor Chen
 *   Change the Firmware version into xx.xx.xx format.
 */
 

#define SYSLOG 0

#include "inkersys.h"

#include <stdio.h>
#include <string.h>

#ifdef _GLOBAL_VERSION_ENABLE

/************************************************************************/
/*      Constants                                                       */
/************************************************************************/
#define VersionLen 17

/************************************************************************/
/*      Type Definitions                                                */
/************************************************************************/
typedef struct
{
	unsigned char VersionValid;
	char Auther[VersionLen];
	char System[VersionLen];
	char CompanyName[VersionLen];
	char ProductName[VersionLen];
	char ProductModel[VersionLen];
	char VersionNum[VersionLen];
	char Feature[VersionLen];
}BoardVersion;


/************************************************************************/
/*      Local Prototypes                                                */
/************************************************************************/

/************************************************************************/
/*      Global Variables                                                */
/************************************************************************/
_ROM BoardVersion OwnVersion = 
{
	1,
	"Inker.Dong",
	"GPML_SYS",   //神笔马良系统 God Pen Ma Liang
	"智游电子有限公司",
	"智能管家婆",
	"ZY8888",
	"1.01",

	/*1:Debug or Release Version
	 *	D=Debug Version
	 *	R=Release Version */
#ifdef _GLOBAL_DEBUG_ENABLE
	"D"
#else
	"R"
#endif

};

/************************************************************************/
/*      Local   Variables                                               */
/************************************************************************/

static char _XDATA BuildedTime[10];// 14:00:23
static char _XDATA BuildedDate[12];// Feb 25 2017

/************************************************************************/
/*      Local Functions                                                 */
/************************************************************************/

/************************************************************************/
/*      Application Interface                                           */
/************************************************************************/
GLOBAL char* GetAuther(void)
{
	return (char*)OwnVersion.Auther;
}

GLOBAL char* GetSystem(void)
{
	return (char*)OwnVersion.System;
}

GLOBAL char* GetCompany(void)
{
	return (char*)OwnVersion.CompanyName;
}

GLOBAL char* GetProName(void)
{
	return (char*)OwnVersion.ProductName;
}

GLOBAL char* GetProModel(void)
{
	return (char*)OwnVersion.ProductModel;
}

GLOBAL char* GetVersionNum(void)
{
	return (char*)OwnVersion.VersionNum;
}

GLOBAL char* GetBoardFeature(void)
{
	return (char*)OwnVersion.Feature;
}

GLOBAL char* GetBuildedDate(void)
{
	return (char*)BuildedDate;
}

GLOBAL char* GetBuildedTime(void)
{
	return (char*)BuildedTime;
}

GLOBAL void Version(void)
{
	DPRINTF(("  Auther: %s\r\n",GetAuther()));
	DPRINTF(("  System: %s\r\n",GetSystem()));
	DPRINTF((" Company: %s\r\n",GetCompany()));
	DPRINTF((" Product: %s,%s\r\n",GetProName(),GetProModel()));
	DPRINTF((" Version: %s,%s\r\n",GetVersionNum(),GetBoardFeature()));
	DPRINTF((" Builded: %s %s\r\n",GetBuildedDate(),GetBuildedTime()));
	DPRINTF(("  Fosclk: %0.4fMHz\r\n",(float)SYSTEM_OSC_FCLK/1000000l));
}

GLOBAL void VersionInit(void)
{
	sprintf(BuildedDate,"%s",__DATE__);
	sprintf(BuildedTime,"%s",__TIME__);
}

#endif

