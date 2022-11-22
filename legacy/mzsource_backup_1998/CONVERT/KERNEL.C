/* KERNEL.C */

#define _KERNEL_C_

#include "standard.h"
#include "text.h"
#include "floppy.h"
#include "ym3812.h"
#include "selectos.h"
#include "disk.h"
#include "tty.h"

unsigned int pascal g_ver[3]={0x00, 0x96, 0xe1};

unsigned int far cdecl main(void);

unsigned int far cdecl main(void)
{
           nosound(); while(kbhit())getch();
           printf("\xfe Starting MINZ v$x.$x$c\r\n\r\n", g_ver[0], g_ver[1], g_ver[2]);
           startminz();

           {
            unsigned char s_buffer[512];
            DISK_absread(DISK_drive->disk, DISK_drive->start_lsec, &s_buffer[0]);
            TEXT_nputs(0U, 0U, &s_buffer[0], 512U, 0x1f);
           }
           {
            t_drive far *s_drive=DISK_drive;
            printf("-------------------------------------------------------------\r\n");
            while(s_drive)
            {
             printf("Drive : $lu, $lu\r\n", s_drive->start_lsec, s_drive->end_lsec);
             s_drive=s_drive->next;
            }
           }
           return(0x0000);
}

/* End of source */