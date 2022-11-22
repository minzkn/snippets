/* DOS.H */

#ifndef _DOS_H_
#define _DOS_H_

extern unsigned int far pascal _dos_chdir(void far *);
extern unsigned int far pascal _dos_findfirst(void far *, void far *, unsigned int);
extern unsigned int far pascal _dos_findnext(void far *);
extern unsigned int far pascal _dos_getcurdir(unsigned int, void far *);
extern unsigned int far pascal _dos_getdisk(void);
extern unsigned int far pascal _dos_mkdir(void far *);
extern unsigned int far pascal _dos_rmdir(void far *);
extern unsigned int far pascal _dos_setdisk(unsigned int);
extern void far * far pascal _dos_getcwd(void far *, unsigned int);
extern unsigned int far pascal _dos_creat(void far *, unsigned int);
extern unsigned int far pascal _dos_open(void far *, unsigned int);
extern unsigned int far pascal _dos_close(unsigned int);
extern unsigned int far pascal _dos_unlink(void far *);
extern unsigned int far pascal _dos_read(unsigned int, void far *, unsigned int);
extern unsigned int far pascal _dos_write(unsigned int, void far *, unsigned int);
extern unsigned int far pascal _dos_lseek(unsigned int, unsigned long, unsigned int);
extern unsigned int far pascal _dos_getfileattr(void far *, unsigned int far *);
extern unsigned int far pascal _dos_setfileattr(void far *, unsigned int);
extern unsigned int far pascal _dos_getvolume(unsigned int, void far *);
extern unsigned int far pascal _dos_setvolume(unsigned int, void far *);

#endif _DOS_H_

/* End of source */
