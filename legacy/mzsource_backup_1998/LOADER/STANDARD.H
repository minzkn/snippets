  /***************************************
   *         Copyright(c)1998-1998       *
   *       MC Compiler source (STANDARD) *
   *         Code by Cho JaeHyuk         *
   *          MINZ (02)822-2744          *
   ***************************************/

/* STANDARD.H */

#ifndef _STANDARD_H_
#define _STANDARD_H_

typedef struct ts_heap /* heap node */
{
 struct ts_heap far *prev; /* 4 bytes */
 struct ts_heap far *next; /* 4 bytes */
 struct ts_heap far *crnt; /* 4 bytes */
 unsigned int size, reserved;
}t_heap;

#define MZ_FP_SEG(x)         ((unsigned int)((unsigned long)(x)>>16))
#define MZ_FP_OFF(x)         ((unsigned int)(x))
#define MZ_MK_FP(seg,off)    ((void far *)(((unsigned long)(seg)<<16)|(((unsigned long)(off))&0xffffLU)))
#define MZ_BIT(v,b)          (((v)&(b))==(b))

#define MZ_NULL              0LU
#define MZ_TRUE              1
#define MZ_FALSE             0
#define MZ_SUCCESS           1
#define MZ_ERROR             0

#define SECTOR_SIZE          512

#define MAXPATH              80
#define MAXDRIVE             3
#define MAXDIR               66
#define MAXFILE              9
#define MAXNAME              MAXFILE
#define MAXEXT               5
#define MAXFILENAME          13

#define _A_NORMAL            0x00
#define _A_RDONLY            0x01
#define _A_HIDDEN            0x02
#define _A_SYSTEM            0x04
#define _A_VOLID             0x08
#define _A_SUBDIR            0x10
#define _A_ARCH              0x20

#define DOS_FA_NORMAL        0x00
#define DOS_FA_RDONLY        0x01
#define DOS_FA_HIDDEN        0x02
#define DOS_FA_SYSTEM        0x04
#define DOS_FA_LABEL         0x08
#define DOS_FA_DIREC         0x10
#define DOS_FA_ARCH          0x20

#define FA_NORMAL            DOS_FA_NORMAL
#define FA_RDONLY            DOS_FA_RDONLY
#define FA_HIDDEN            DOS_FA_HIDDEN
#define FA_SYSTEM            DOS_FA_SYSTEM
#define FA_LABEL             DOS_FA_LABEL
#define FA_DIREC             DOS_FA_DIREC
#define FA_ARCH              DOS_FA_ARCH

#define SEEK_SET             0x00
#define SEEK_CUR             0x01
#define SEEK_END             0x02

#define O_RDONLY             0x01
#define O_WRONLY             0x02
#define O_RDWR               0x04

#define O_BINARY             0x0000
#define O_TEXT               0x0000

typedef struct ts_tree
{
 struct ts_tree far *prev;      /* 4 byte */
 struct ts_tree far *next;      /* 4 byte */
 unsigned char       level;     /* 1 byte */
 unsigned int        ly;        /* 2 byte */
 unsigned char       select;     /* 1 byte */
 unsigned char       name[MAXFILENAME];  /* 13 byte */
}t_tree; /* max = 32byte (4+4+1+2+1+13 = 25)*/

typedef struct ts_dos_ffblk
{
 unsigned char ff_reserved[21];
 unsigned char ff_attrib;
 unsigned int  ff_ftime;
 unsigned int  ff_fdate;
 unsigned long ff_fsize;
 unsigned char ff_name[13];
}t_dos_ffblk;

typedef t_dos_ffblk  t_ffblk;

#ifndef _STANDARD_C_

extern void __emit__();

extern const unsigned char far pascal _num[16];
extern unsigned long far pascal _vector[256];
extern unsigned int pascal _heapseg;
extern unsigned int pascal _argc;
extern unsigned char far ** pascal _argv;
extern unsigned char far *prevdir;
extern unsigned char far *wild_all;
extern unsigned char far *rootdir;
extern t_heap far *HEAP_head;
extern t_heap far *HEAP_tail;
extern unsigned int minx, miny, maxx, maxy, maxcolor;
extern unsigned int scanning_level;

extern unsigned int far pascal (far pascal *chdir)(void far *);
extern unsigned int far pascal (far pascal *findfirst)(void far *, void far *, unsigned int);
extern unsigned int far pascal (far pascal *findnext)(void far *);
extern unsigned int far pascal (far pascal *getcurdir)(unsigned int, void far *);
extern unsigned int far pascal (far pascal *getdisk)(void);
extern unsigned int far pascal (far pascal *mkdir)(void far *);
extern unsigned int far pascal (far pascal *rmdir)(void far *);
extern unsigned int far pascal (far pascal *setdisk)(unsigned int);
extern void far * far pascal (far pascal *getcwd)(void far *, unsigned int);
extern unsigned int far pascal (far pascal *creat)(void far *, unsigned int);
extern unsigned int far pascal (far pascal *open)(void far *, unsigned int);
extern unsigned int far pascal (far pascal *close)(unsigned int);
extern unsigned int far pascal (far pascal *unlink)(void far *);
extern unsigned int far pascal (far pascal *read)(unsigned int, void far *, unsigned int);
extern unsigned int far pascal (far pascal *write)(unsigned int, void far *, unsigned int);
extern unsigned int far pascal (far pascal *lseek)(unsigned int, unsigned long, unsigned int);
extern unsigned int far pascal (far pascal *getfileattr)(void far *, unsigned int far *);
extern unsigned int far pascal (far pascal *setfileattr)(void far *, unsigned int);
extern unsigned int far pascal (far pascal *scanning_func)(void far *);

extern void far pascal nowork(void);
extern void interrupt nointwork(void);
extern void interrupt zerovector(void);
extern void far pascal exit(unsigned int);
extern void far pascal abort(void);
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
extern unsigned int far pascal strcmp(void far *, void far *);
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
extern void far * far pascal farmalloc(unsigned int s_size);
extern void far pascal farfree(void far *s_ptr);
extern void far * far pascal getvect(unsigned int);
extern unsigned int far pascal setvect(unsigned int, void far *);
extern void far * far pascal vsprintf(void far *, void far *, void far *);
extern void far * far cdecl printf(void far *, ...);
extern void far * far cdecl sprintf(void far *, void far *, ...);
extern void far * far pascal insert(void far *, unsigned int);
extern void far * far pascal delete(void far *);
extern unsigned int far pascal inportb(unsigned int);
extern unsigned int far pascal inportw(unsigned int);
extern unsigned int far pascal outportb(unsigned int, unsigned int);
extern unsigned int far pascal outportw(unsigned int, unsigned int);
extern unsigned int far pascal rand(void);
extern void far pascal delay(unsigned int);
extern void far pascal sound(void);
extern void far pascal nosound(void);
extern void far pascal beep(unsigned int);
extern unsigned int far pascal inx(unsigned int, unsigned int);
extern void far pascal outx(unsigned int, unsigned int, unsigned int);
extern unsigned int far pascal kbhit(void);
extern unsigned int far pascal getch(void);
extern void far pascal coldboot(void);
extern void far pascal warmboot(void);
extern void far pascal biosreset(void);
extern void far pascal geninterrupt(unsigned int);
extern void far pascal setvideomode(unsigned int);
extern void far pascal setdelaykey(unsigned int);
extern unsigned int far pascal inx_test(unsigned int, unsigned int, unsigned int);
extern void far pascal outx_test(unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal farjump(unsigned int, unsigned int);
extern void far pascal farcall(unsigned int, unsigned int);
extern unsigned int far pascal disk_reset(unsigned int);
extern unsigned int far pascal disk_read(unsigned int, unsigned int, unsigned int, unsigned int, void far *);
extern unsigned int far pascal disk_write(unsigned int, unsigned int, unsigned int, unsigned int, void far *);
extern void far pascal gotoxy(unsigned int, unsigned int);
extern void far pascal enable(void);
extern void far pascal disable(void);
extern void far pascal fnsplit(void far *, void far *, void far *, void far *, void far *);
extern void far pascal fnmerge(void far *, void far *, void far *, void far *, void far *);
extern unsigned int far pascal scanning(void far *);

extern void far pascal _initmain(void);

#endif _STANDARD_C_

#endif _STANDARD_H_

/* End of source */