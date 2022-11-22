/*
  JACK's GRAPH.H
*/

#include "svgacc.h"

extern unsigned char far *s_EngFont;
extern unsigned char far *s_HanFont;
extern signed char s_ksmode;

extern void far EngPutc(signed int n_x, signed int n_y, signed char n_character, unsigned char n_color, unsigned char n_bcolor);
extern void far EngPuts(signed int n_x, signed int n_y, signed char far *n_string, unsigned char n_color, unsigned char n_bcolor);
extern void far HanPutcLow(signed int n_x, signed int n_y, signed int n_offset, unsigned char n_color, unsigned char n_bcolor);
extern void far KS2JO(unsigned int far *n_hancode);
extern void far HanPutc(signed int x, signed int y, union HanCode HanCode, unsigned char Color, unsigned char BackColor);
extern void far Puts(signed int n_x, signed int n_y, signed char far *n_string, unsigned char n_color, unsigned char n_bcolor);
extern void far PopRec(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2);
extern void far PushRec(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2);
extern void far PopBox(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2, unsigned char n_color);
extern void far PushBox(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2, unsigned char n_color);
extern void far Printf(signed int n_x, signed int n_y, unsigned char n_color, unsigned char n_bcolor, signed char far *n_string, ...);
extern signed int far GetImageDisk(signed int x1, signed int y1, signed int x2, signed int y2, signed char far * filename, unsigned char palcolor =0);
extern signed int far PutImageDisk(signed int x1, signed int y1, signed int x2, signed int y2, signed char far * filename, signed int pc =0);
extern signed int far NonPutImageDisk(signed int x1, signed int y1, signed int x2, signed int y2, signed char far * filename, signed int pc, signed int n_col);
extern signed int far LoadPCX(signed int x, signed int y, signed char far *pcxname);
extern void far Window(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2, signed char far *n_string, unsigned char n_color, WindowData far *WD, unsigned int n_handle =0);
extern signed int far GetImageXMS(signed int x1, signed int y1, signed int x2, signed int y2, unsigned int n_handle);
extern signed int far PutImageXMS(signed int x1, signed int y1, signed int x2, signed int y2, unsigned int n_handle);

/* End of source */