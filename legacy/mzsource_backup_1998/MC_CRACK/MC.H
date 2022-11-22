/* STAN.H */

#ifndef _STAN_H_
#define _STAN_H_

typedef struct ts_heap /* heap node */
{
 struct ts_heap far *prev; /* 4 bytes */
 struct ts_heap far *next; /* 4 bytes */
 struct ts_heap far *crnt; /* 4 bytes */
 unsigned int size, reserved;
}t_heap;

#define MZ_FP_SEG(x)             ((unsigned int)((unsigned long)(x) >> 16))
#define MZ_FP_OFF(x)             ((unsigned int)(x))
#define MZ_MK_FP(seg,off)        ((void far *)(((unsigned long)(seg) << 16)|(((unsigned long)(off)) & 0xffffLU)))
#define MZ_BIT(v,b)              (((v)&(b))==(b))

#ifndef _STAN_C_

extern const unsigned char far __num[16];
extern unsigned int heapseg;
extern t_heap far *HEAP_head;
extern t_heap far *HEAP_tail;

extern unsigned int far pascal putc(unsigned int);
extern void far * far pascal puts(void far *);
extern void far * far pascal memset(void far *, unsigned int, unsigned int);
extern void far * far pascal memcpy(void far *, void far *, unsigned int);
extern void far * far pascal memchr(void far *, unsigned int, unsigned int);
extern unsigned int far pascal memcmp(void far *, void far *, unsigned int);
extern void far pascal swap(void far *, void far *);
extern unsigned int far pascal strclen(void far *, unsigned int);
extern unsigned int far pascal strlen(void far *);
extern void far * far pascal strncpy(void far *, void far *, unsigned int);
extern void far * far pascal strcpy(void far *, void far *);
extern void far * far pascal strncat(void far *, void far *, unsigned int);
extern void far * far pascal strcat(void far *, void far *);
extern void far * far pascal strnchr(void far *, unsigned int, unsigned int);
extern void far * far pascal strchr(void far *, unsigned int);
extern unsigned int far pascal toupper(unsigned int);
extern unsigned int far pascal tolower(unsigned int);
extern unsigned int far pascal isdigit(unsigned int);
extern unsigned int far pascal isalpha(unsigned int);
extern void far * far pascal strupr(void far *);
extern void far * far pascal strlwr(void far *);
extern unsigned int far pascal tonum(unsigned int);
extern void far * far pascal itoa(void far *, unsigned int, unsigned int, unsigned int);
extern unsigned int far pascal atoi(void far *, unsigned int);
extern unsigned int far pascal peekb(unsigned int, unsigned int);
extern unsigned int far pascal peekw(unsigned int, unsigned int);
extern unsigned int far pascal pokeb(unsigned int, unsigned int, unsigned int);
extern unsigned int far pascal pokew(unsigned int, unsigned int, unsigned int);
extern unsigned int far pascal malloc_itop(unsigned int s_num);
extern void far * far pascal farmalloc(unsigned int s_size);
extern void far pascal farfree(void far *s_ptr);
extern void far * far pascal getvect(unsigned int);
extern unsigned int far pascal setvect(unsigned int, void far *);
extern void far * far pascal vsprintf(void far *, void far *, void far *);
extern void far * far cdecl printf(void far *, ...);
extern void far * far cdecl sprintf(void far *, void far *, ...);

extern void far pascal _initmain(void);

#endif _STAN_C_

#endif _STAN_H_

/* End of source */