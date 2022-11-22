/*
  Name: SUGA O/S
  File: KERNEL.H
  Copy: Copyright(c)1998-1999 by Cho JaeHyuk
  What: Header of "KERNEL.CPP"
*/

#ifndef Use_Inline
#define Use_Inline                     "Use inline command"
#pragma inline
#endif  Use_Inline

#ifndef FILE_KERNEL_H
#define FILE_KERNEL_H                  "KERNEL.H"

#define D_BIOSDISK_RESET               0x0000 // 0   Reset disk system
#define D_BIOSDISK_STATUS              0x0001 // 1   Get disk status
#define D_BIOSDISK_READ                0x0002 // 2   Read disk sectors
#define D_BIOSDISK_WRITE               0x0003 // 3   Write disk sectors
#define D_BIOSDISK_VERIFY              0x0004 // 4   Verify disk sectors
#define D_BIOSDISK_FORMAT              0x0005 // 5   Format disk track
 // 6   Format track and set bad sector flag (XT & portable)
 // 7   Format the drive starting at track (XT & portable)
 // 8   Get current drive parameters (XT & newer, see note Ø)
 // 9   Initialize 2 fixed disk base tables (XT & newer, see note Ø)
 // A   Read long sector (XT & newer, see note Ø)
 // B   Write long sector (XT & newer, see note Ø)
#define D_BIOSDISK_SEEK                0x000c // C   Seek to cylinder (XT & newer, see note Ø)
 // D   Alternate disk reset (XT & newer, see note Ø)
 // E   Read sector buffer (XT & portable only)
 // F   Write sector buffer (XT & portable only)
#define D_BIOSDISK_READY               0x0010 // 10  Test for drive ready (XT & newer, see note Ø)
#define D_BIOSDISK_RECALIBRATE         0x0011 // 11  Recalibrate drive (XT & newer, see note Ø)
 // 12  Controller ram diagnostic (XT & portable only)
 // 13  Drive diagnostic (XT & portable only)
 // 14  Controller internal diagnostic (XT & newer, see note Ø)
 // 15  Read disk type/DASD type (XT BIOS from 1/10/86 & newer)
 // 16  Disk change line status (XT BIOS from 1/10/86 & newer)
 // 17  Set dasd type for format (XT BIOS from 1/10/86 & newer)
 // 18  Set media type for format (BIOS date specific)
 // 19  Park fixed disk heads (AT & newer)
 // 1A  Format ESDI drive unit (PS/2 50+)

#define D_BYTES_PER_SECTOR             (512U)

#ifndef D_DEFINE_0000
#define D_DEFINE_0000                  0x0000
extern "C"
{
 extern unsigned int far pascal _howcpu(void);

 extern void far pascal YM_register(unsigned int, unsigned int);
 extern void far pascal YM_enablewave(void);
 extern void far pascal YM_disablewave(void);
 extern void far pascal YM_method1(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_method2(unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_method3(unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_method4(unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_method5(unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_method6(unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_selectwave(unsigned int, unsigned int);
 extern void far pascal YM_frequency_on(unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_frequency_off(unsigned int, unsigned int, unsigned int);
 extern unsigned int far pascal YM_iscard(void);

 extern unsigned char far YM_instrument[][12U];
 extern unsigned int  far YM_frequency[12U];

 extern unsigned int                   g_segment_text;
 extern unsigned int                   g_segment_DGROUP;
 extern unsigned int                   g_segment_heap;
 extern unsigned int                   g_segment_stack;
}
#endif  D_DEFINE_0000

enum e_boolean               {False = 0U, True};
enum e_swich                 {Off = 0U, On};
enum e_return                {Error = 0U, Success, Cancel};
enum e_high                  {Low = 0U, High};

#define NULL                 (0LU)
#define NIL                  (0U)

#define D_LOW                (0U)
#define D_HIGH               (1U)

#define D_MAXDRIVE           (2U+1U)
#define D_MAXDIR             (80U+1U)
#define D_MAXFILE            (8U+1U)
#define D_MAXEXT             (3U+1U)
#define D_MAXPATH            ((D_MAXDRIVE-1U)+(D_MAXDIR-1U)+(D_MAXFILE-1U)+(D_MAXEXT-1U))

#define D_READ               (0U)
#define D_WRITE              (1U)

#define D_FAT(x)             ((512U<<3U)/((unsigned int)(x)))

#define D_EOF                0xffff

#define asm_nop              asm nop
#define asm_push_si          asm_nop
#define asm_push_di          asm_nop
#define asm_pop_si           asm_nop
#define asm_pop_di           asm_nop
#define asm_push_ds          asm push ds
#define asm_push_es          asm_nop
#define asm_pop_ds           asm pop ds
#define asm_pop_es           asm_nop

#define MK_FP(x,y)           ((void far *)((((unsigned long)(x))<<16U)|((unsigned int)y)))
#define FP_SEG(x)            ((unsigned int)(((unsigned long)(x))>>16U))
#define FP_OFF(x)            ((unsigned int)(x))
#define MK_ABSP(x)           ((unsigned long)((((unsigned long)FP_SEG((x)))<<4U)|FP_OFF(x)))

#define D_ATTRIB_RDONLY      0x01
#define D_ATTRIB_HIDDEN      0x02
#define D_ATTRIB_SYSTEM      0x04
#define D_ATTRIB_LABEL       0x08
#define D_ATTRIB_VOLUME      0x08
#define D_ATTRIB_SUBDIR      0x10
#define D_ATTRIB_ARCHIVE     0x20

#define disable()                      asm cli
#define enable()                       asm sti
#define clear_cache()                  asm jmp short $+2 /* nop() */
#define exit(x)                        _exit((x))
#define random(x)                      (rand()%(x))
#define inx(x,y)                       inportx((x),(y))
#define outx(x,y,z)                    outportx((x),(y),(z))
#define peek(x,y)                      peekb((x),(y))
#define poke(x,y,z)                    pokeb((x),(y),(z))

typedef struct ts_pcx_head
{
 unsigned char pcx_check; /* Always 0x0a */
 unsigned char version;   /* 0, 2, 3, 5 */
 unsigned char encode;    /* 1 = RLE */
 unsigned char bits_per_pixel;
 unsigned int  x1, y1, x2, y2;
 unsigned int  Hres, Vres;
 unsigned char palette[16U * 3U];
 unsigned char mode;
 unsigned char planes;
 unsigned int  palette_type;
 unsigned int  filler[58U];
}t_pcx_head;

typedef struct ts_fullname
{
 struct ts_fullname far *next;
 unsigned char name[(D_MAXFILE-1U)+(D_MAXEXT-1U)+1U+1U];
}t_fullname;

typedef struct ts_boot_sector
{
 unsigned char               jump[3U];            // 0x00
 unsigned char               OEM_name[8U];        // 0x03
 unsigned int                bytes_per_sector;    // 0x0b
 unsigned char               sectors_per_cluster; // 0x0d
 unsigned int                reserved_sectors;    // 0x0e
 unsigned char               FATs_on_disk;        // 0x10
 unsigned int                root_entries;        // 0x11
 unsigned int                total_sectors;       // 0x13
 unsigned char               media;               // 0x15
 unsigned int                sectors_per_FAT;     // 0x16
 unsigned int                sectors_per_track;   // 0x18
 unsigned int                heads;               // 0x1a
 unsigned int                hidden_sectors;      // 0x1c
 unsigned int                adjust;              // 0x1e
 union
 {
  unsigned int word[2U];
  unsigned long dword;
 }sectors;                                        // 0x20
 unsigned char               drive;               // 0x24
 unsigned char               reserved;            // 0x25
 unsigned char               signature;           // 0x26 []= 0x29
 unsigned long               serial;              // 0x27
 unsigned char               volume[11U];         // 0x2b
 unsigned char               reserved_dump[8U];   // 0x36
}t_boot_sector;

typedef union tu_identify_parameter
{
 unsigned int                configuration;
 unsigned int                logical_cylinders;
 unsigned int                reserved00;
 unsigned int                logical_heads;
 unsigned int                bytes_per_track_unformatted;
 unsigned int                bytes_per_sector_unformatted;
 unsigned int                logical_sectors_per_track;
 unsigned int                vendor_specific0[3U];
 unsigned int                serial[10];
 unsigned int                buffer_type;
 unsigned int                buffer_512segment;
 unsigned int                ECC;
 unsigned int                firmware[4U];
 unsigned int                model[20U];
 unsigned int                sector_per_interrupt_MULTIPLE;
 unsigned int                IO_possible;
 unsigned int                capabilities;
 unsigned int                reserved01;
 unsigned int                PIO_data_transfers;
 unsigned int                DMA_data_transfers;
 unsigned int                reserved02;
 unsigned int                cylinderss;
 unsigned int                heads;
 unsigned int                sectors_per_track;
 unsigned int                sectors;
 unsigned int                sectors_per_interrupt;
 unsigned int                addressable_sectors_LBA;
 unsigned int                DMA0;
 unsigned int                DMA1;
 unsigned int                reserved03;
 unsigned int                reserved04;
 unsigned int                reserved05;
 unsigned int                reserved06;
 unsigned int                reserved07;
 unsigned int                reserved08[127U-69U+1U];
 unsigned int                vendor_specific1;
 unsigned int                reserved09[255U-160U+1U];
}t_identify_parameter;

typedef struct ts_partition_table
{
 unsigned char               id;
 unsigned char               begin_head;
 unsigned char               begin_sector;
 unsigned char               begin_cylinder;
 unsigned char               system;
 unsigned char               end_head;
 unsigned char               end_sector;
 unsigned char               end_cylinder;
 unsigned long               first_position;
 unsigned long               sectors;
}t_partition_table;

typedef struct ts_partition
{
 unsigned char               dump[0x1be];
 t_partition_table           item[4U];
 unsigned int                ispartition; // 0xaa55
}t_partition;

typedef struct ts_root
{
 unsigned char               name[8U];
 unsigned char               extension[3U];
 unsigned char               attrib;
 unsigned char               reserved[10U];
 unsigned int                time;
 unsigned int                date;
 unsigned int                start_cluster;
 unsigned long               size;
}t_root;

typedef union tu_sector
{
 unsigned char               buffer[D_BYTES_PER_SECTOR];
 unsigned char               sector[D_BYTES_PER_SECTOR];
 unsigned char               bsector[D_BYTES_PER_SECTOR];
 unsigned int                wsector[D_BYTES_PER_SECTOR>>1U];
 unsigned long               dsector[D_BYTES_PER_SECTOR>>2U];
 t_boot_sector               boot;
 t_identify_parameter        identify;
 t_partition                 partition;
 t_root                      root[16U];
}t_sector;

typedef struct ts_heap
{
 struct ts_heap far         *prev;
 struct ts_heap far         *next;
 void far                   *address;
 unsigned int                length;
 unsigned int                reserved;
}t_heap;

typedef class tc_drive t_drive;

typedef class tc_text
{
 void far *video_memory;
 unsigned int hres, vres;
 unsigned int tab;
 public:
 unsigned int tty_x, tty_y, tty_color;
 unsigned int minx, miny, maxx, maxy;
 tc_text(unsigned int s_hres = 80U, unsigned int s_vres = 25U, unsigned int s_minx = 0U, unsigned int s_miny = 0U, unsigned int s_maxx = 79U, unsigned int s_maxy = 24U)
 {
  tc_text::video_memory=MK_FP(0xb800, 0x0000);
  tc_text::hres=s_hres;
  tc_text::vres=s_vres;
  tc_text::window(s_minx, s_miny, s_maxx, s_maxy);
  tc_text::gotoxy(tc_text::wherex(), tc_text::wherey());
  tc_text::tty_color=0x07;
  tc_text::tab=8U;
 }
 void far pascal window(unsigned int s_minx = 0U, unsigned int s_miny = 0U, unsigned int s_maxx = 79U, unsigned int s_maxy = 24U);
 void far pascal lputc(unsigned int, unsigned int, unsigned int);
 unsigned int far pascal lgetc(unsigned int, unsigned int);
 void far pascal putc(unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal puts(unsigned int, void far *, unsigned int, unsigned int);
 void far pascal nputs(unsigned int, unsigned int, void far *, unsigned int, unsigned int);
 void far cdecl printf(unsigned int, unsigned int, unsigned int, void far *, ...);
 void far cdecl nprintf(unsigned int, unsigned int, unsigned int, unsigned int, void far *, ...);
 void far pascal hline(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal vline(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal fillbox(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal box(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal recbox(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal clear(unsigned int = 0x0007, unsigned int = ' ');
 void far pascal gettext(void far *, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal puttext(void far *, unsigned int, unsigned int, unsigned int, unsigned int);
 void far pascal setcursor(unsigned int = 0x20, unsigned int = 0x00); // 0x0d, 0x0e
 void far pascal setblank(e_boolean);
 void far pascal setfont(void far *);
 void far pascal setcolor(unsigned int s_color = 0x07){tc_text::tty_color=s_color;}
 unsigned int far pascal wherex(void);
 unsigned int far pascal wherey(void);
 void far pascal gotoxy(unsigned int, unsigned int);
 void far pascal linefeed(e_boolean = True);
 void far pascal tty_putc(unsigned char = ' ');
 void far pascal tty_puts(void far *);
 void far pascal tty_nputs(unsigned int, void far *);
 void far cdecl tty_printf(void far *, ...);
 void far cdecl tty_nprintf(unsigned int, void far *, ...);
}t_text;

typedef class tc_disk
{
 unsigned int      cylinders, sectors_per_track, heads;
 unsigned int      recount;
 public:
 unsigned int      disk_num;
 e_boolean         access;
 class tc_disk far *prev;
 class tc_disk far *next;
 unsigned int far pascal build_drive(unsigned long s_start = 0LU);
 tc_disk(unsigned int s_disk =NIL, unsigned int s_recount =4U)
 {
  tc_disk::disk_num=s_disk;
  tc_disk::recount=s_recount;
  if(tc_disk::reset())tc_disk::access=False;
  else
  {
   tc_disk::access=(e_boolean)tc_disk::build_drive();
  }
  tc_disk::prev=tc_disk::next=NULL;
 }
 ~tc_disk()
 {
  tc_disk::reset();
 }
 void far pascal long2item(void far *, void far *, void far *, unsigned long =0LU);
 unsigned int far pascal reset(void);
 unsigned int far pascal status(void);
 unsigned int far pascal read(unsigned int, unsigned int, unsigned int, unsigned int, void far *);
 unsigned int far pascal write(unsigned int, unsigned int, unsigned int, unsigned int, void far *);
 unsigned int far pascal verify(unsigned int, unsigned int, unsigned int, unsigned int, void far *);
 unsigned int far pascal format(unsigned int, unsigned int, unsigned int, unsigned int, void far *);
 unsigned int far pascal absread(void far *, unsigned long, unsigned int =1U);
 unsigned int far pascal abswrite(void far *, unsigned long, unsigned int =1U);
 unsigned int far pascal add_drive(tc_drive far *);
 void far pascal print(void);
}t_disk;

typedef class tc_drive
{
 unsigned int                system; // FAT?, SNODE?, ???
 e_boolean                   exist, lower;
 unsigned long               fat1, fat2, root, cluster;
 unsigned long               number_of_fat1, number_of_fat2, number_of_root, number_of_cluster;
 t_sector                    nowfat1;
 unsigned long               nowfat1sector;
 public:
 unsigned long               begin, end, length;
 t_disk far *                disk;
 unsigned char               drive_name; // A, B, C, D, E, ...
 t_sector                    boot;
 struct tc_drive far *       prev;
 struct tc_drive far *       next;
 unsigned char               path[D_MAXPATH];
 unsigned int far pascal absread(void far *, unsigned long, unsigned int = 1U);
 unsigned int far pascal abswrite(void far *, unsigned long, unsigned int = 1U);
 unsigned int far pascal fat1read(void far *, unsigned long, unsigned int = 1U);
 unsigned int far pascal fat1write(void far *, unsigned long, unsigned int = 1U);
 unsigned int far pascal fat2read(void far *, unsigned long, unsigned int = 1U);
 unsigned int far pascal fat2write(void far *, unsigned long, unsigned int = 1U);
 unsigned int far pascal rootread(void far *, unsigned long, unsigned int = 1U);
 unsigned int far pascal rootwrite(void far *, unsigned long, unsigned int = 1U);
 unsigned int far pascal clusterread(void far *, unsigned long, unsigned int = 1U);
 unsigned int far pascal clusterwrite(void far *, unsigned long, unsigned int = 1U);
 e_boolean far pascal rootname_to_filename(void far *, void far *);
 e_boolean far pascal filename_to_rootname(void far *, void far *);
 unsigned int far pascal found_name_in_root(t_root far *, void far *);
 unsigned int far pascal cluster_rw(void far * = NULL, unsigned long = 0U, unsigned int = 0U, unsigned int = 1U, unsigned int = D_READ);
 t_fullname far * far pascal open_name(void far * = NULL);
 void far pascal close_name(t_fullname far * = NULL);
 unsigned long far pascal next_cluster(unsigned long = 0LU);
 tc_drive(t_disk far *s_disk, unsigned int s_system, unsigned int s_drive_name, unsigned long s_begin, unsigned long s_end, unsigned long s_length, e_boolean s_exist)
 {
  tc_drive::disk=s_disk;
  tc_drive::system=s_system;
  tc_drive::drive_name=s_drive_name;
  tc_drive::begin=s_begin;
  tc_drive::end=s_end;
  tc_drive::length=s_length;
  tc_drive::exist=s_exist;
  tc_drive::lower=False;
  tc_drive::fat1=tc_drive::fat2=tc_drive::root=tc_drive::cluster=0LU;
  tc_drive::nowfat1sector=0xffffffffLU;
  if(tc_drive::exist==True)
  {
   if(tc_drive::absread(&tc_drive::boot.sector[0U], 0LU))
   {
    tc_drive::exist=False;
   }
   else // Access boot-sector
   {
    if(tc_drive::boot.boot.total_sectors)
    {
     tc_drive::boot.boot.sectors.word[Low]=tc_drive::boot.boot.total_sectors;
     tc_drive::boot.boot.sectors.word[High]=0U;
    }
    tc_drive::fat1=tc_drive::boot.boot.reserved_sectors;
    tc_drive::fat2=((unsigned long)tc_drive::fat1)+((unsigned long)tc_drive::boot.boot.sectors_per_FAT);
    tc_drive::cluster=tc_drive::root=((((unsigned long)tc_drive::boot.boot.FATs_on_disk)*((unsigned long)tc_drive::boot.boot.sectors_per_FAT))+((unsigned long)tc_drive::fat1));
    tc_drive::cluster+=((((((unsigned long)tc_drive::boot.boot.root_entries)<<5U)+((unsigned long)tc_drive::boot.boot.bytes_per_sector))-1LU)/((unsigned long)tc_drive::boot.boot.bytes_per_sector));
    tc_drive::number_of_fat1=tc_drive::number_of_fat2=((unsigned long)tc_drive::boot.boot.sectors_per_FAT);
    tc_drive::number_of_root=((unsigned long)tc_drive::boot.boot.root_entries)>>5U;
    tc_drive::number_of_cluster=tc_drive::end-tc_drive::cluster+1LU;
   }
  }
  else
  {
   extern void far pascal memsetw(void far *, unsigned int, unsigned int);
   memsetw(&tc_drive::boot.sector[0U], 0x0000, 256U);
  }
 }
}t_drive;

#ifndef FILE_KERNEL_CPP

extern void far pascal _begin_main(t_boot_sector far *);
extern void far pascal _end_main(void);
extern void far pascal _exit(unsigned int);
extern void far pascal nop(void);
extern void interrupt inop(void);
extern unsigned int far pascal itop(unsigned int);
extern void far * far pascal farmalloc(unsigned int);
extern void far pascal farfree(void far *);
extern unsigned int far pascal inportb(unsigned int);
extern unsigned int far pascal inportw(unsigned int);
extern void far pascal outportb(unsigned int, unsigned int);
extern void far pascal outportw(unsigned int, unsigned int);
extern void far pascal insportb(void far *, unsigned int, unsigned int);
extern void far pascal insportw(void far *, unsigned int, unsigned int);
extern void far pascal outsportb(unsigned int, void far *, unsigned int);
extern void far pascal outsportw(unsigned int, void far *, unsigned int);
extern void far pascal memsetb(void far *, unsigned int, unsigned int);
extern void far pascal memsetw(void far *, unsigned int, unsigned int);
extern void far pascal memcpyb(void far *, void far *, unsigned int);
extern void far pascal memcpyw(void far *, void far *, unsigned int);
extern unsigned int far pascal memcmpb(void far *, void far *, unsigned int);
extern unsigned int far pascal strlen(void far *);
extern unsigned int far pascal strcpy(void far *, void far *);
extern unsigned int far pascal strcat(void far *, void far *);
extern unsigned int far pascal strcmp(void far *, void far *);
extern unsigned int far pascal isdigit(unsigned int);
extern unsigned int far pascal islower(unsigned int);
extern unsigned int far pascal isupper(unsigned int);
extern unsigned int far pascal isalpha(unsigned int);
extern unsigned int far pascal tolower(unsigned int);
extern unsigned int far pascal toupper(unsigned int);
extern unsigned int far pascal tonum(unsigned int);
extern void far * far pascal itoa(void far *, unsigned int, unsigned int, unsigned int);
extern void far * far pascal ltoa(void far *, unsigned long, unsigned int, unsigned int);
extern unsigned int far pascal atoi(void far *, unsigned int);
extern unsigned long far pascal atol(void far *, unsigned int);
extern void far pascal vsprintf(void far *, void far *, void far *);
extern void far cdecl sprintf(void far *, void far *, ...);
extern void far * far pascal strupr(void far *);
extern void far * far pascal strlwr(void far *);
extern void far pascal swap(void far *, void far *, unsigned int);
extern void far pascal putc(unsigned int);
extern void far pascal puts(void far *);
extern void far pascal nputs(unsigned int, void far *);
extern void far cdecl printf(void far *, ...);
extern void far cdecl nprintf(unsigned int, void far *, ...);
extern unsigned int far pascal bioskey(unsigned int);
extern unsigned int far pascal biosdisk(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void far *);
extern void far pascal delay(unsigned int);
extern void far pascal sound(unsigned int);
extern void far pascal nosound(void);
extern void far pascal beep(unsigned int);
extern unsigned int far pascal rand(void);
extern void far * far pascal howcpu(void);
extern unsigned int far pascal inportx(unsigned int, unsigned int);
extern void far pascal outportx(unsigned int, unsigned int, unsigned int);
extern unsigned int far pascal peekb(unsigned int, unsigned int);
extern unsigned int far pascal peekw(unsigned int, unsigned int);
extern void far pascal pokeb(unsigned int, unsigned int, unsigned int);
extern void far pascal pokew(unsigned int, unsigned int, unsigned int);
extern unsigned int far pascal gets(void far *, unsigned int);
extern void far pascal setrgb(unsigned int, unsigned int, unsigned int, unsigned int);
extern void far pascal getrgb(unsigned int, void far *, void far *, void far *);
extern unsigned long far pascal farcore(void);
extern unsigned int far pascal biosmemory(void);
extern unsigned long far pascal farcoreleft(void);
extern t_drive far * far pascal found_drive(unsigned int);

extern void far * operator new(unsigned int);
extern void operator delete(void far *);

extern void interrupt (**interrupt_vector)(void);
extern void interrupt (*interrupt_proc[256U])(void);
extern void interrupt (*bios_disk_interrupt)(void);
extern void interrupt (*bios_video_interrupt)(void);
extern t_heap far                  *g_heap_head;
extern t_heap far                  *g_heap_tail;
extern unsigned char far            g_numchar[36U];
extern t_disk far *                 g_disk_head;
extern t_disk far *                 g_disk_tail;
extern t_drive far *                g_drive_head;
extern t_drive far *                g_drive_tail;
extern t_text far *                 g_text;
extern unsigned int                 g_drive_name;
extern void far *                   g_cpu_name[];
extern unsigned char                g_line[4U][11U];
extern t_boot_sector far *          g_boot_sector;
extern e_boolean                    g_original_boot;

#endif  FILE_KERNEL_CPP

#endif  FILE_KERNEL_H

/* End of source */