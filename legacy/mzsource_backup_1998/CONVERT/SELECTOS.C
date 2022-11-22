/* SELECTOS.C */

#define _SELECTOS.C_

#include "standard.h"
#include "dos.h"
#include "selectos.h"
#include "disk.h"
#include "tty.h"

void far pascal argdos(void);
void far pascal startdos(void);
void far pascal startminz(void);

void far pascal argdos(void)
{
 unsigned char far *s_arg=(unsigned char far *)MZ_MK_FP(_psp, 0x0080);
           *(s_arg+(unsigned int)(*(s_arg))+1U)='\0'; s_arg++;
           _argc=0U; _argv[_argc++]="MINZ";
           do
           {
            while(*(s_arg)==' ')*(s_arg++)='\0';
            if(*(s_arg))
            {
             if(*(s_arg)=='\"')
             {
              if(*(++s_arg))
              {
               _argv[_argc++]=s_arg;
               while(*(s_arg)&&*(s_arg)!='\"')s_arg++;
              }
             }
             else
             {
              _argv[_argc++]=s_arg;
              while(*(s_arg)&&*(s_arg)!=' ')s_arg++;
             }
             if(*(s_arg))*(s_arg++)='\0';
            }
           }while(*(s_arg));
}

void far pascal startdos(void)
{
           TTY_init();
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
}

void far pascal startminz(void)
{
           TTY_init();
           setdelaykey(0U);
           DISK_init();
           chdir=(void far *)nowork;
           findfirst=(void far *)nowork;
           findnext=(void far *)nowork;
           getcurdir=(void far *)nowork;
           getdisk=(void far *)nowork;
           mkdir=(void far *)nowork;
           rmdir=(void far *)nowork;
           setdisk=(void far *)nowork;
           getcwd=(void far *)nowork;
           creat=(void far *)nowork;
           open=(void far *)nowork;
           close=(void far *)nowork;
           unlink=(void far *)nowork;
           read=(void far *)nowork;
           write=(void far *)nowork;
           lseek=(void far *)nowork;
           getfileattr=(void far *)nowork;
           setfileattr=(void far *)nowork;
}

/* End of source */