/*
 MINZ KERNEL LIBRARY
 Copyright (c) MINZ SOFTWARE

 JaeHyuk CHO   E-Mail : minzkn@dreamwiz.com
*/

#ifndef DEF_H_MZLIB
#define DEF_H_MZLIB          "MZLIB.H"

/* */
typedef struct ts_disk_item
{
 unsigned char head;
 unsigned char sector;
 unsigned char cylinder;
}t_disk_item;

typedef struct ts_partition
{
 unsigned char id;
 t_disk_item begin;
 unsigned char description;
 t_disk_item end;
 unsigned long fsector;
 unsigned long nsector;
}t_partition;

typedef struct ts_MBS
{
 unsigned char dummy[0x01be];
 t_partition partition_table[4];
 unsigned int initial;
}t_MBS;

typedef struct ts_BOOT_FAT16
{
 unsigned char jump_code[0x0003];
 unsigned char OEM_name[0x0008];
 unsigned int  bps;
 unsigned char spc;
 unsigned int  reserved_sectors;
 unsigned char FAT_on_disk;
 unsigned int  ROOT_dir_entries;
 unsigned int  total_sectors;
 unsigned char media;
 unsigned int  spFAT;
 unsigned int  spt;
 unsigned int  heads;
 unsigned int  hidden_sectors;
 unsigned int  adjust;
 unsigned long e_total_sectors;
 unsigned char drive_number;
 unsigned char reserved;
 unsigned char signature;
 unsigned long serial;
 unsigned char volume_label[0x000b];
 unsigned char FAT_string[0x0008];
}t_BOOT_FAT16;

typedef struct ts_FAT_root
{
 unsigned char fname[0x0008];
 unsigned char ext[0x0003];
 unsigned char attrib;
 unsigned char dummy_fill[0x000a];
 unsigned int  time;
 unsigned int  date;
 unsigned int  start_cluster;
 unsigned long flength;
}t_FAT_root;

typedef struct ts_Descriptor
{
 unsigned int  LimitLow;
 unsigned int  BaseLow;
 unsigned char BaseMid;
 unsigned char Access00;
 unsigned char Access01;
 unsigned int  BaseHigh;
}t_Descriptor;

extern         unsigned int far pascal Is_Intel80x86(void);
extern         unsigned int far pascal Is_Intel80x286(void);
extern         unsigned int far pascal Is_Intel80x386(void);
extern         unsigned int far pascal Is_Intel80x486(void);
extern         unsigned int far pascal Is_Intel80x586(void);
extern         unsigned int far pascal WhatCPU(void);
extern         void far pascal *INFO_Entry;
extern         void far pascal *INFO_StartUpReg;
extern         void far pascal *INFO_IntVector;
extern         unsigned long pascal INFO_ProgramSize;
extern         void far pascal HaltDos(void);
extern         void far pascal ShutDown(void);
extern         void far pascal HaltSystem(void);
extern         void far pascal ResetVideo(void);
extern         void far pascal Randomize(void);
extern         unsigned int far pascal Rand(void);
extern         unsigned int far pascal Random(unsigned int);
extern         unsigned long far pascal ToPhysical(void far *);
extern         void far * far pascal ToLogical(unsigned long);
extern         void far * far pascal ToOptimalLogical(void far *);
extern         unsigned int far pascal Center(unsigned int);
extern         unsigned long far pascal DivL(unsigned long, unsigned int);
extern         unsigned int far pascal Peek(void far *);
extern         unsigned int far pascal PeekB(void far *);
extern         unsigned int far pascal PeekW(void far *);
extern         void far pascal Poke(void far *, unsigned int);
extern         void far pascal PokeB(void far *, unsigned int);
extern         void far pascal PokeW(void far *, unsigned int);
extern         void far pascal Swap(void far *, void far *, unsigned int);
extern         void far pascal SwapB(void far *, void far *, unsigned int);
extern         void far pascal SwapW(void far *, void far *, unsigned int);
extern         void far pascal MemSet(void far *, unsigned int, unsigned int);
extern         void far pascal MemSetB(void far *, unsigned int, unsigned int);
extern         void far pascal MemSetW(void far *, unsigned int, unsigned int);
extern         void far pascal MemClear(void far *, unsigned int);
extern         void far pascal MemCpy(void far *, void far *, unsigned int);
extern         void far pascal MemCpyB(void far *, void far *, unsigned int);
extern         void far pascal MemCpyW(void far *, void far *, unsigned int);
extern         void far * far pascal MemScan(void far *, unsigned int, unsigned int);
extern         void far * far pascal MemScanB(void far *, unsigned int, unsigned int);
extern         void far * far pascal MemScanW(void far *, unsigned int, unsigned int);
extern         unsigned int far pascal MemCmp(void far *, void far *, unsigned int);
extern         unsigned int far pascal MemCmpB(void far *, void far *, unsigned int);
extern         unsigned int far pascal MemCmpW(void far *, void far *, unsigned int);
extern         unsigned int far pascal WhatBit(unsigned int, unsigned int);
extern         void far pascal OpenBit(void far *);
extern         unsigned int far pascal CloseBit(void);
extern         unsigned int far pascal ReadBit(void);
extern         void far pascal WriteBitSet(void);
extern         void far pascal WriteBitClear(void);
extern         void far pascal Putc_00(unsigned int);
extern         void far pascal Putc_01(unsigned int);
extern         void far pascal Putc(unsigned int);
extern         void far pascal SetPutc(void far *);
extern         void far pascal Puts_00(void far *);
extern         void far pascal Putn_00(void far *, unsigned int);
extern         void far pascal Puts(void far *);
extern         void far pascal Putn(void far *, unsigned int);
extern         void far pascal SetPuts(void far *);
extern         void far pascal SetPutn(void far *);
extern         void far pascal PutXY_00(unsigned int, unsigned int, unsigned int);
extern         void far pascal PutXY(unsigned int, unsigned int, unsigned int);
extern         void far pascal SetPutXY(void far *);
extern         unsigned int far pascal GetXY_00(unsigned int, unsigned int);
extern         unsigned int far pascal GetXY(unsigned int, unsigned int);
extern         void far pascal SetGetXY(void far *);
extern         void far pascal PutLineX(unsigned int, unsigned int, unsigned int, unsigned int);
extern         void far pascal PutLineY(unsigned int, unsigned int, unsigned int, unsigned int);
extern         void far pascal Box(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern         void far pascal Rectangle(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern         void far pascal Window(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern         void far pascal ClearScreen(unsigned int);
extern         void far pascal SetWindow(unsigned int, unsigned int, unsigned int, unsigned int);
extern         void far pascal GetText(void far *, unsigned int, unsigned int, unsigned int, unsigned int);
extern         void far pascal PutText(void far *, unsigned int, unsigned int, unsigned int, unsigned int);
extern         unsigned int far pascal WhereX_00(void);
extern         unsigned int far pascal WhereY_00(void);
extern         unsigned int far pascal WhereX(void);
extern         unsigned int far pascal WhereY(void);
extern         void far pascal SetWhereX(void far *);
extern         void far pascal SetWhereY(void far *);
extern         void far pascal MoveCursor_00(unsigned int, unsigned int);
extern         void far pascal MoveCursor(unsigned int, unsigned int);
extern         void far pascal CursorX(unsigned int);
extern         void far pascal CursorY(unsigned int);
extern         void far pascal CursorXY(unsigned int, unsigned int);
extern         void far pascal SetMoveCursor(void far *);
extern         void far pascal Blink_00(unsigned int);
extern         void far pascal Blink(unsigned int);
extern         void far pascal SetBlink(void far *);
extern         void far pascal PutsXY(unsigned int, void far *, unsigned int, unsigned int);
extern         void far pascal PutnXY(unsigned int, void far *, unsigned int, unsigned int, unsigned int);
extern         void far pascal PutmXY(unsigned int, void far *, unsigned int, unsigned int, unsigned int);
extern         void far cdecl PrintF(void far *, ...);
extern         void far cdecl PrintK(void far *, ...);
extern         void far pascal PutDump(void far *, unsigned long, unsigned int);
extern         void far pascal *TEXT_VideoMemory;
extern         unsigned int far pascal TEXT_Page, TEXT_Color, TEXT_Character, TEXT_Res[0x0002], TEXT_Window[4], TEXT_TabLength, TEXT_LineTable[0x0004][0x0010];
extern         void far pascal ResetTimer(void);
extern         void far pascal Delay(unsigned int);
extern         void far pascal TimerDummy(void);
extern         void far pascal OpenTimer(void);
extern         void far pascal CloseTimer(void);
extern         void far pascal SetTimerRate(unsigned int);
extern         void far pascal SetTimerCallBack(void far *);
extern         void interrupt TimerHandler(void);
extern         unsigned long GetClockTick(void);
extern         unsigned int far pascal TIMER_IRQ_Enabled, TIMER_IRQ_Active, TIMER_Speed, TIMER_Count;
extern         void far pascal *TIMER_OldVector;
extern         void far pascal *TIMER_IRQ_CallBack;
extern         unsigned long far pascal TIMER_ClockTick;
extern         void far pascal Sound(unsigned int);
extern         void far pascal NoSound(void);
extern         void far pascal Beep(unsigned int, unsigned int);
extern         void interrupt NopInterrupt(void);
extern         void far * far pascal GetVect(unsigned int);
extern         void far pascal SetVect(void far *, unsigned int);
extern         unsigned int far pascal StrLenC(void far *, unsigned int);
extern         unsigned int far pascal StrLenN(void far *, unsigned int);
extern         unsigned int far pascal StrLen(void far *);
extern         void far pascal StrCpyN(void far *, void far *, unsigned int);
extern         void far pascal StrCpy(void far *, void far *);
extern         void far * far pascal StrCatN(void far *, void far *, unsigned int);
extern         void far * far pascal StrCat(void far *, void far *);
extern         unsigned int far pascal StrCmpN(void far *, void far *, unsigned int);
extern         unsigned int far pascal StrCmp(void far *, void far *);
extern         void far pascal StrSetN(void far *, unsigned int, unsigned int);
extern         void far pascal StrSet(void far *, unsigned int);
extern         void far pascal StrUpr(void far *);
extern         void far pascal StrLwr(void far *);
extern         void far pascal StrOptimize(void far *, unsigned int);
extern         void far pascal VSPrintF(void far *, void far *, void far *);
extern         void far cdecl SPrintF(void far *, void far *, ...);
extern         unsigned int far pascal InPort(unsigned int);
extern         unsigned int far pascal InPortB(unsigned int);
extern         unsigned int far pascal InPortW(unsigned int);
extern         void far pascal OutPort(unsigned int, unsigned int);
extern         void far pascal OutPortB(unsigned int, unsigned int);
extern         void far pascal OutPortW(unsigned int, unsigned int);
extern         unsigned int far pascal InPortx(unsigned int, unsigned int);
extern         unsigned int far pascal InPortxB(unsigned int, unsigned int);
extern         unsigned int far pascal InPortxW(unsigned int, unsigned int);
extern         void far pascal OutPortx(unsigned int, unsigned int, unsigned int);
extern         void far pascal OutPortxB(unsigned int, unsigned int, unsigned int);
extern         void far pascal OutPortxW(unsigned int, unsigned int, unsigned int);
extern         void far pascal InPorts(void far *, unsigned int, unsigned int);
extern         void far pascal InPortsB(void far *, unsigned int, unsigned int);
extern         void far pascal InPortsW(void far *, unsigned int, unsigned int);
extern         void far pascal OutPorts(void far *, unsigned int, unsigned int);
extern         void far pascal OutPortsB(void far *, unsigned int, unsigned int);
extern         void far pascal OutPortsW(void far *, unsigned int, unsigned int);
extern         void far * far pascal New_00(unsigned int);
extern         void far * far pascal New_00_00(unsigned int);
extern         void far * far pascal New(unsigned int);
extern         void far * far pascal FarAlloc(unsigned int);
extern         void far pascal SetNew(void far *);
extern         void far pascal SetFarAlloc(void far *);
extern         void far pascal Delete_00(void far *);
extern         void far pascal Delete(void far *);
extern         void far pascal FarFree(void far *);
extern         void far pascal SetDelete(void far *);
extern         void far pascal SetDeleteFarFree(void far *);
extern         unsigned int far pascal IsUpper(unsigned int);
extern         unsigned int far pascal IsLower(unsigned int);
extern         unsigned int far pascal IsSpace(unsigned int);
extern         unsigned int far pascal IsDigit(unsigned int);
extern         unsigned int far pascal IsHex(unsigned int);
extern         unsigned int far pascal IsControl(unsigned int);
extern         unsigned int far pascal IsNest(unsigned int);
extern         unsigned char far pascal T_IS[0x0100 + 0x0001];
extern         unsigned int far pascal ToUpper(unsigned int);
extern         unsigned int far pascal ToLower(unsigned int);
extern         unsigned int far pascal ToAsciiNumber(unsigned int);
extern         unsigned int far pascal ToNum(unsigned int);
extern         void far pascal LtoA(void far *, unsigned long, unsigned int, unsigned int);
extern         void far pascal ItoA(void far *, unsigned int, unsigned int, unsigned int);
extern         unsigned long far pascal AtoL(void far *, unsigned int);
extern         unsigned int far pascal AtoI(void far *, unsigned int);
extern         unsigned char far pascal FONT_English[0x0800];
extern         unsigned int far pascal HUF_Encode(void far *, void far *, unsigned int);
extern         unsigned int far pascal HUF_Decode(void far *, unsigned int);
extern         unsigned int far pascal Encode(void far *, void far *, unsigned int);
extern         unsigned int far pascal Decode(void far *, unsigned int);
extern         void far pascal SetCode(void far *, void far *);
extern         unsigned int far pascal DOS_chdir(void far *);
extern         unsigned int far pascal DOS_findfirst(void far *, void far *, unsigned int);
extern         unsigned int far pascal DOS_findnext(void far *);
extern         unsigned int far pascal DOS_getcurdir(unsigned int, void far *);
extern         void far * far pascal DOS_getcwd(void far *, unsigned int);
extern         unsigned int far pascal DOS_getdisk(void);
extern         unsigned int far pascal DOS_mkdir(void far *);
extern         unsigned int far pascal DOS_rmdir(void far *);
extern         unsigned int far pascal DOS_setdisk(unsigned int);
extern         unsigned int far pascal DOS_creat(void far *, unsigned int);
extern         unsigned int far pascal DOS_open(void far *, unsigned int);
extern         unsigned int far pascal DOS_close(unsigned int);
extern         unsigned int far pascal DOS_unlink(void far *);
extern         unsigned int far pascal DOS_read(unsigned int, void far *, unsigned int);
extern         unsigned int far pascal DOS_write(unsigned int, void far *, unsigned int);
extern         unsigned int far pascal DOS_lseek(unsigned int, unsigned long, unsigned int);
extern         unsigned int far pascal DOS_getfileattr(void far *, unsigned int far *);
extern         unsigned int far pascal DOS_setfileattr(void far *, unsigned int);
extern         unsigned int far pascal IDE_Identify(unsigned int s_fixed, unsigned int s_device, void far *s_buffer);
extern         unsigned int far pascal IDE_Read(unsigned int s_fixed, unsigned int s_device, unsigned int s_head, unsigned int s_cylinder, unsigned int s_sector, void far *s_buffer);
extern         unsigned int far pascal IDE_Write(unsigned int s_fixed, unsigned int s_device, unsigned int s_head, unsigned int s_cylinder, unsigned int s_sector, void far *s_buffer);
extern         unsigned int far pascal IDE_Seek(unsigned int s_fixed, unsigned int s_device, unsigned int s_head, unsigned int s_cylinder, unsigned int s_sector);
extern         void far pascal IDE_ResetController(void);
extern         unsigned int far pascal IDE_BasePort[];

/* */
#define        DEF_Null                0x0000
#define        DEF_Nil                 DEF_Null

#ifndef        DEF_C_MAIN "MAIN.C"
 #define        DEF_Company             "MINZ SOFTWARE"
 #define        DEF_Name                "MINZ"
 #define        DEF_VersionHigh         0x0001
 #define        DEF_VersionLow          0x0000
 #define        DEF_VersionType         "Beta-test "                               
 extern void far pascal *S_STR_Company;
 extern void far pascal *S_STR_Name;
 extern unsigned int pascal S_STR_VersionHigh;
 extern unsigned int pascal S_STR_VersionLow;
 extern void far pascal * S_STR_VersionType;
#endif

#endif

/* End of source */


