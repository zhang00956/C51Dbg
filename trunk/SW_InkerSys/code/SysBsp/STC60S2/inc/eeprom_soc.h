#ifndef __EERPOM_SOC_H
#define __EEPROM_SOC_H

sfr ISP_CONTR  =0xC7;

extern void  EraseSector(u16 SectorAddr);	    // ��������
extern   u8  ReadByte(u16 ByteAddr);		    // Byte��
extern void  SaveByte(u16 ByteAddr, u8 val);	// Byteд

#endif

