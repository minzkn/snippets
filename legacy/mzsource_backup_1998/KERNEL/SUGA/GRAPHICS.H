/*
  Name: SUGA O/S
  File: GRAPHICS.H
  Copy: Copyright(c)1998-1999 by Cho JaeHyuk
  What: Header of "GRAPHICS.CPP"
*/

#ifndef Use_Inline
#define Use_Inline                     "Use inline command"
#pragma inline
#endif  Use_Inline

#ifndef FILE_GRAPHICS_H
#define FILE_GRAPHICS_H                "GRAPHICS.H"

#include <KERNEL.H>

#define D_GRAPHICS_320x200             (0U)
#define D_GRAPHICS_640x400             (1U)
#define D_GRAPHICS_640x480             (2U)
#define D_GRAPHICS_800x600             (3U)
#define D_GRAPHICS_1024x768            (4U)
#define D_GRAPHICS_1280x1024           (5U)

#define D_PUT                          (0U)
#define D_AND                          (1U)
#define D_XOR                          (2U)
#define D_OR                           (3U)

#define D_DEFAULT_COLOR                (0xffff)

typedef class tc_graphics
{
 public:
 void far *        base;
 unsigned long     bank_size;
 unsigned int      hres, vres, card, mode, bank_num, process, max_bank_num;
 unsigned int      minx, miny, maxx, maxy;
 unsigned int      scale_x, scale_y;
 unsigned int      to_x, to_y;
 unsigned int      tty_x, tty_y, tty_color;
 void far *        tty_screen;
 tc_graphics(unsigned int s_graphics = D_GRAPHICS_800x600)
 {
  tc_graphics::initgraph(s_graphics);
 }
 void far pascal (*bank)(unsigned int = 0U);
 unsigned int far pascal initgraph(unsigned int);
 void far pascal closegraph(void);
 unsigned int far pascal getpixel(unsigned int, unsigned int);
 void far pascal putpixel(unsigned int, unsigned int, unsigned int);
 void far pascal hline(unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal vline(unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal rectangle(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal fillbox(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal clear(unsigned int = 0U);
 void far pascal eputc(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal eputs(unsigned int, unsigned int, void far *, unsigned int, unsigned int);
 void far pascal bitmap16x16(unsigned int, unsigned int, void far *, unsigned int, unsigned int);
 void far pascal hputc(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal ehputs(unsigned int, unsigned int, void far *, unsigned int, unsigned int);
 void far cdecl eprintf(unsigned int, unsigned int, unsigned int, unsigned int, void far *, ...);
 void far cdecl ehprintf(unsigned int, unsigned int, unsigned int, unsigned int, void far *, ...);
 void far pascal neputs(unsigned int, unsigned int, unsigned int, void far *, unsigned int, unsigned int);
 void far pascal box(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal line(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal lineto(unsigned int, unsigned int, unsigned int);
 void far pascal video(e_boolean);
 void far pascal tputc(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal tputs(unsigned int, unsigned int, void far *, unsigned int, unsigned int);
 void far pascal linefeed(e_boolean s_sw);
 void far pascal tty_putc(unsigned int);
}t_graphics;

#ifndef FILE_GRAPHICS_CPP

extern void far pascal bank_cirrus(unsigned int);

extern unsigned char far HAN_table0_[32];
extern unsigned char far HAN_table1_[32];
extern unsigned char far HAN_table2_[32];
extern unsigned char far HAN_ftable0_[21];
extern unsigned char far HAN_ftable1_[21];
extern unsigned char far HAN_ltable_[21];

#endif  FILE_GRAPHICS_CPP

extern unsigned char far font_eng[4096];
extern unsigned char far font_han0[8][19][32];
extern unsigned char far font_han1[4][21][32];
extern unsigned char far font_han2[4][27][32];

#endif  FILE_GRAPHICS_H

/* End of source */