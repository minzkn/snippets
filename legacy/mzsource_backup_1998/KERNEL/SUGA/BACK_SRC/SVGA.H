/*
  Name: SUGA O/S
  File: SVGA.H
  Copy: Copyright(c)1998-1999 by Cho JaeHyuk
  What: Header of "SVGA.CPP"
*/


#ifndef FILE_SVGA_H
#define FILE_SVGA_H                    "SVGA.H"

#define D_DEFAULT_COLOR                (0xffff)

#ifndef FILE_SVGA_CPP

extern void far pascal putpixel(unsigned int, unsigned int, unsigned int);
extern void far pascal hline(unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal vline(unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal rectangle(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal box(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal clear(unsigned int);
extern void far pascal eputc(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal eputs(unsigned int, unsigned int, void far *, unsigned int, unsigned int);
extern void far pascal bitmap16x16(unsigned int, unsigned int, void far *, unsigned int, unsigned int);
extern void far pascal hputc(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal ehputs(unsigned int, unsigned int, void far *, unsigned int, unsigned int);
extern void far cdecl eprintf(unsigned int, unsigned int, unsigned int, unsigned int, void far *, ...);
extern void far cdecl ehprintf(unsigned int, unsigned int, unsigned int, unsigned int, void far *, ...);
extern void far pascal neputs(unsigned int, unsigned int, unsigned int, void far *, unsigned int, unsigned int);

extern unsigned int prev_bank_num;
extern unsigned int scale_x, scale_y;

#endif  FILE_SVGA_CPP

#endif  FILE_SVGA_H

/* End of source */