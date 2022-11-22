/* DISK.C */

#define _DISK_C_

#include "standard.h"
#include "disk.h"

t_disk far *DISK_disk;
t_drive far *DISK_drive;
t_drive far *DISK_crnt;
unsigned int DISK_drivecount;

void far pascal DISK_init(void);
void far pascal DISK_add_disk(t_disk far *);
void far pascal DISK_add_drive(t_drive far *);
void far pascal DISK_add_partition(t_disk far *, unsigned long);
unsigned int far pascal DISK_absread(t_disk far *, unsigned long, void far *);
unsigned int far pascal DISK_abswrite(t_disk far *, unsigned long, void far *);

void far pascal DISK_init(void)
{
           DISK_disk=MZ_NULL; DISK_drive=MZ_NULL; DISK_crnt=MZ_NULL;
           DISK_drivecount=0U; /* Floppy < 2 */
           FLOPPY_init();
           DISK_drivecount=2U; /* Hard disk >= 2 */
           IDE_init();
}

void far pascal DISK_add_disk(t_disk far *s_disk)
{
           if(DISK_disk==MZ_NULL)
           {
            s_disk->next=MZ_NULL;
            DISK_disk=s_disk;
           }
           else
           {
            t_disk far *s_node=DISK_disk;
            while(s_node->next)s_node=s_node->next;
            s_node->next=s_disk;
            s_disk->next=MZ_NULL;
           }
}

void far pascal DISK_add_drive(t_drive far *s_drive)
{
           s_drive->drive_num=DISK_drivecount++;
           if(DISK_drive==MZ_NULL)
           {
            s_drive->next=MZ_NULL;
            DISK_drive=s_drive;
           }
           else
           {
            t_drive far *s_node=DISK_drive;
            while(s_node->next)s_node=s_node->next;
            s_node->next=s_drive;
            s_drive->next=MZ_NULL;
           }
}

void far pascal DISK_add_partition(t_disk far *s_disk, unsigned long s_start)
{
 unsigned char far *s_buffer;
 t_drive far *s_node;
 t_partition s_partition[4];
 unsigned int s_pcount;
           s_buffer=(unsigned char far *)farmalloc(s_disk->sector_size);
           if(DISK_absread(s_disk, s_start, s_buffer))
           {
            memcpy(&s_partition[0], s_buffer+0x1be, sizeof(t_partition)<<2);
            for(s_pcount=0U;s_pcount<4U;s_pcount++)
            {
             switch(s_partition[s_pcount].system)
             {
              case 0x01: case 0x04: case 0x06: /* FAT */
                   s_node=(t_drive far *)farmalloc(sizeof(t_drive));
                   s_node->disk=s_disk;
                   s_node->file_system=s_partition[s_pcount].system;
                   s_node->start_lsec=s_partition[s_pcount].first_sector+s_start;
                   s_node->end_lsec=s_partition[s_pcount].nsectors+s_node->start_lsec;
                   s_node->ret=0U;
                   s_node->buffer=MZ_NULL;
                   DISK_absread(s_disk, s_node->start_lsec, &s_node->boot);
                   DISK_add_drive(s_node);
                   #ifdef DEBUG_MINZ
                    printf("Drive $c: [$u] - $lu ~ $lu\r\n", s_node->drive_num+'A', s_partition[s_pcount].system, s_node->start_lsec, s_node->end_lsec);
                   #endif DEBUG_MINZ
                   break;
              case 0x05: /* Extend FAT */
                   DISK_add_partition(s_disk, s_partition[s_pcount].first_sector+s_start);
                   break;
              default: break;
             }
            }
           }
           farfree(s_buffer);
}

unsigned int far pascal DISK_absread(t_disk far *s_disk, unsigned long s_lsector, void far *s_buffer)
{
 unsigned int s_head, s_cyl, s_sec;
           s_sec=(unsigned int)(s_lsector%s_disk->sector)+1U;
           s_lsector/=s_disk->sector;
           s_head=(unsigned int)(s_lsector%s_disk->head);
           s_lsector/=s_disk->head;
           s_cyl=(unsigned int)s_lsector;
           return(s_disk->func->read(s_disk->ctrl, s_disk->disk_num, s_head, s_cyl, s_sec, s_buffer));
}

unsigned int far pascal DISK_abswrite(t_disk far *s_disk, unsigned long s_lsector, void far *s_buffer)
{
 unsigned int s_head, s_cyl, s_sec;
           s_sec=(unsigned int)(s_lsector%s_disk->sector)+1U;
           s_lsector/=s_disk->sector;
           s_head=(unsigned int)(s_lsector%s_disk->head);
           s_lsector/=s_disk->head;
           s_cyl=(unsigned int)s_lsector;
           return(s_disk->func->write(s_disk->ctrl, s_disk->disk_num, s_head, s_cyl, s_sec, s_buffer));
}

/* End of source */