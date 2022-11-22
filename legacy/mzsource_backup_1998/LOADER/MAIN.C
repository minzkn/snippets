/* MAIN.C */

#include "standard.h"
#include "text.h"
#include "dos.h"

extern void far pascal loader(void);

unsigned int far pascal main(void);

unsigned int far pascal main(void)
{
           nosound(); while(kbhit())getch();
           chdir=_dos_chdir;
           findfirst=_dos_findfirst;
           findnext=_dos_findnext;
           getcurdir=_dos_getcurdir;
           getdisk=_dos_getdisk;
           mkdir=_dos_mkdir;
           rmdir=_dos_rmdir;
           setdisk=_dos_setdisk;
           getcwd=_dos_getcwd;
           creat=_dos_creat;
           open=_dos_open;
           close=_dos_close;
           unlink=_dos_unlink;
           read=_dos_read;
           write=_dos_write;
           lseek=_dos_lseek;
           getfileattr=_dos_getfileattr;
           setfileattr=_dos_setfileattr;

           loader();

           return(0x0000);
}

/* End of source */