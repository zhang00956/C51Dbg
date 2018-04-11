#ifndef _FONT_H
#define _FONT_H

typedef struct {
	unsigned char font_code[2];
	u8 Width;
	u8 Height;
	const char *bmBuf;
}Font_t;


extern _ROM Font_t Font16[];

#endif

