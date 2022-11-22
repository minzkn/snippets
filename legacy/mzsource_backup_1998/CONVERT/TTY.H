/* TTY.H */

#ifndef _TTY_H_
#define _TTY_H_

#ifndef _TTY_C_

extern unsigned int TTY_x1, TTY_y1, TTY_x2, TTY_y2;
extern unsigned int TTY_x, TTY_y, TTY_color;

extern void far pascal TTY_init(void);
extern void far pascal TTY_setTTY(void);
extern void far pascal TTY_putc(unsigned int);
extern void far pascal TTY_puts(void far *);
extern void far cdecl TTY_printf(void far *, ...);

#endif _TTY_C_

#endif _TTY_H_

/* End of source */
