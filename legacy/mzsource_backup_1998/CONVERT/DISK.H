/* DISK.H */

#ifndef _DISK_H_
#define _DISK_H_

#include "hard.h"
#include "floppy.h"

#define DISK_NONE            0
#define DISK_EXIST           1
#define DISK_FLOPPY          2
#define DISK_HARD            3
#define DISK_IDE             4
#define DISK_SCSI            5

#define DISK_MODE            0
#define DISK_CHS             1
#define DISK_LBA             2

#define DISK_MINZ            0
#define DISK_FAT             1
#define DISK_NTFS            2

typedef struct ts_disk_func
{ /* Class I/O */
 unsigned int far pascal (far pascal *reset)(unsigned int, unsigned int);
 unsigned int far pascal (far pascal *read)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void far *);
 unsigned int far pascal (far pascal *write)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void far *);
 unsigned int far pascal (far pascal *verify)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void far *);
 unsigned int far pascal (far pascal *identify)(unsigned int, unsigned int, void far *);
}t_disk_func;

typedef struct ts_boot
{
 unsigned char          jumpcode[3];
 unsigned char          OEM[8];
 unsigned int           spb;
 unsigned char          cps;
 unsigned int           reserveds;
 unsigned char          fats;
 unsigned int           roots;
 unsigned int           sectors;
 unsigned char          media;
 unsigned int           fatps;
 unsigned int           tps;
 unsigned int           heads;
 unsigned int           hidden;
 unsigned int           adjust;
 unsigned long          sector;
 unsigned char          bootdrive;
 unsigned char          head;
 unsigned char          signature;
 unsigned long          serial;
 unsigned char          volume[11];
 unsigned char          fat_system[8];
}t_boot;

typedef struct ts_disk
{
 struct ts_disk far *next;
 t_disk_func far *   func;             /* I/O */
 unsigned int        type;             /* FLOPPY, IDE, SCSI */
 unsigned int        mode;             /* CHS, LBA */
 unsigned int        disk_num;         /* Even type */
 unsigned int        ctrl;             /* Controller num */
 unsigned int        sector_size;      /* Default: 512U */
 unsigned int        head;
 unsigned int        cylinder;
 unsigned int        sector;
}t_disk;

typedef struct ts_drive
{
 struct ts_drive far *next;
 t_disk far *         disk;
 unsigned int         drive_num;
 unsigned int         file_system;     /* MINZ, FAT, NTFS */
 unsigned long        start_lsec, end_lsec;
 unsigned int         ret;             /* 0 = Success, 0 < Error */
 t_boot               boot;            /* boot sector */
 void far *           buffer;          /* Drive buffer */
}t_drive;

typedef struct ts_partition
{
 unsigned char        boot;
 unsigned char        start_head;
 unsigned char        start_sector;
 unsigned char        start_cylinder;
 unsigned char        system;
 unsigned char        end_head;
 unsigned char        end_sector;
 unsigned char        end_cylinder;
 unsigned long        first_sector;
 unsigned long        nsectors;
}t_partition;

#ifndef _DISK_C_

extern t_disk far *DISK_disk;
extern t_drive far *DISK_drive;
extern t_drive far *DISK_crnt;
extern unsigned int DISK_drivecount;

extern void far pascal DISK_init(void);
extern void far pascal DISK_add_disk(t_disk far *);
extern void far pascal DISK_add_drive(t_drive far *);
extern void far pascal DISK_add_partition(t_disk far *, unsigned long);
extern unsigned int far pascal DISK_absread(t_disk far *, unsigned long, void far *);
extern unsigned int far pascal DISK_abswrite(t_disk far *, unsigned long, void far *);

#endif _DISK_C_

#endif _DISK_H_

/* End of source */