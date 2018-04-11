#ifndef _VERSION_H
#define _VERSION_H

#ifdef _GLOBAL_VERSION_ENABLE

GLOBAL char* GetAuther(void);
GLOBAL char* GetSystem(void);
GLOBAL char* GetCompany(void);
GLOBAL char* GetProName(void);
GLOBAL char* GetProModel(void);
GLOBAL char* GetVersionNum(void);
GLOBAL char* GetBoardFeature(void);
GLOBAL char* GetBuildedDate(void);
GLOBAL char* GetBuildedTime(void);
GLOBAL  void Version(void);
GLOBAL  void VersionInit(void);

#endif

#endif


