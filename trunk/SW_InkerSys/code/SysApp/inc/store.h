#ifndef _STORE_H
#define	_STORE_H

#include "license.h"
#include <stddef.h>

/*
STC12C5A60S2 EEPROM  0X0000~0X0X03FF 1K

---------+--------+------+
         |s:0X0000| 128B | Project  Data
Sector 0 |l:0.5KB |------+
         |e:0X01FF|  64B | System   Data
         |        |------+
         |        |  16B | Modbus   Data
         |        |------+
         |        |  48B | Modbus   Data
         |        |------+         
         |        | 256B | Rev      Data         
---------+--------+------+
         |s:0X0200|      |
Sector 1 |l:0.5KB |      | RunTimeCnt
         |e:0X03FF|      |
---------+--------+------+
*/

#define EEPROMBOOTCODE   0XA00A

#define SYS_EEPROM_PAGE  0X0000

#define PRJ_EEPORM       0X0000
#define SYS_EEPROM       0X0080
#define MBS_EEPORM       0X00C0
#define REG_EEPORM       0X00D0

extern void EraseBlock(u16 addr);
extern void SaveU08Value(u16 addr,  u8    value);
extern void ReadU08Value(u16 addr,  u8   *value);
extern void SaveU16Value(u16 addr, u16    SaveValue);
extern void ReadU16Value(u16 addr, u16   *ReadValue);
extern void SaveU32Value(u16 addr, u32    SaveValue);
extern void ReadU32Value(u16 addr, u32   *ReadValue);
extern void SaveF32Value(u16 addr, float  SaveValue);
extern void ReadF32Value(u16 addr, float *ReadValue);

#endif
