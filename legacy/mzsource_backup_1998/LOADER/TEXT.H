  /***************************************
   *         Copyright(c)1998-1998       *
   *       MC Compiler source (TEXT)     *
   *         Code by Cho JaeHyuk         *
   *          MINZ (02)822-2744          *
   ***************************************/

/* TEXT.H */

#ifndef _TEXT_H_
#define _TEXT_H_

extern const unsigned char TEXT_line[][11];

extern void far pascal TEXT_getc(unsigned int, unsigned int);
extern void far pascal TEXT_putc(unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal TEXT_wputc(unsigned int, unsigned int, unsigned int);
extern void far pascal TEXT_attrputc(unsigned int, unsigned int, unsigned int);
extern void far pascal TEXT_puts(unsigned int, unsigned int, void far *, unsigned int);
extern void far cdecl  TEXT_printf(unsigned int, unsigned int, unsigned int, void far *, ...);
extern void far pascal TEXT_linespace(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal TEXT_box(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal TEXT_rec(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal TEXT_boxwindow(unsigned int, unsigned int, unsigned int, unsigned int, void far *,\
                       unsigned int, unsigned int, unsigned int);
extern void far pascal TEXT_cursorsize(unsigned int, unsigned int);
extern void far pascal TEXT_cursor(unsigned int);
extern void far pascal TEXT_blank(unsigned int);
extern void far pascal TEXT_gettext(unsigned int, unsigned int, unsigned int, unsigned int, void far *);
extern void far pascal TEXT_puttext(unsigned int, unsigned int, unsigned int, unsigned int, void far *);
extern void far cdecl TEXT_mprintf(unsigned int, unsigned int, unsigned int, unsigned int, void far *, ...);
extern void far pascal TEXT_cputs(unsigned int, unsigned int, void far *, unsigned int);
extern void far cdecl TEXT_cprintf(unsigned int, unsigned int, unsigned int, void far *, ...);

#endif _TEXT_H_

/* End of source */