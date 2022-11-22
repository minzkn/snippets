/*
  JACK's COMMON.H
*/

#ifndef __JACK__COMMON__
#define __JACK__COMMON__ "JACK & JACK soft"

//#define N_MAKE

#define N_True               (1)
#define N_False              (0)
#define N_Error              (-1)
#define N_Not                (-1)
#define N_Yes                N_True
#define N_No                 N_False
#define N_Cancel             (2)
#define N_Default            (0xff)
#define N_NOBUTTON           (0x00)
#define N_LEFTBUTTON         (0x01)
#define N_RIGHTBUTTON        (0x02)
#define N_MIDDLEBUTTON       (N_LEFTBUTTON|N_RIGHTBUTTON)
#define N_MAXBUFFER          (30000)
/* More than 15000 may be fatal error! */
#define N_MAXCOMPRESS        (15000)
#define N_MINHIMEMSIZE 2048
#define N_MAXHIMEMHANDLE 64
#define N_HANGUL         0
#define N_ENGLISH        1
#define N_NONE           (0)
#define N_AUTOTAB        0x0001
#define N_AUTOARROW      0x0002
#define N_AUTOPAGE       0x0004
#define N_AUTOMOUSE      0x0008
#define N_AUTOINIT       0x0010
#define N_SYSCOLOR       237

enum WINCOMMAND{WIN_NONE, WIN_CLOSE, WIN_MOVE, WIN_EXEC};

typedef struct{
               unsigned char IsMouse    : 1;
               unsigned char IsHBIOS    : 1;
               unsigned char IsHIMEM    : 1;
               unsigned char IsKeyHandle: 1;
               unsigned char IsSWAP     : 1;
               unsigned char IsWin      : 1;
              }MainData;

union HanCode{
              unsigned char ByteData[2];
              unsigned int WordData;
              struct{
                     unsigned int Last    :5;
                     unsigned int Middle  :5;
                     unsigned int First   :5;
                     unsigned int id      :1;
                    }Code;
             };

typedef struct{
               signed int x1, y1, x2, y2;
               signed int bx1, by1, bx2, by2;
               signed int command, cx, cy;
               signed char far *title;
               unsigned int swaphandle;
              }WindowData;

typedef struct{
               unsigned long length;
               unsigned int sourcehandle;
               unsigned long sourceaddress;
               unsigned int targethandle;
               unsigned long targetaddress;
              }himemMEM;

typedef struct{
               signed char id;
               signed char background;
               signed char reserved[512];
              }MainConfig;

typedef struct _ShellData_{
                           signed char name[13];
                           signed char attrib;
                           //signed int date, time;
                           signed long size;
                           struct _ShellData_ far *prev;
                           struct _ShellData_ far *next;
                          }ShellData;

extern MainData s_MD;
extern signed char far *s_WorkDirectory;
extern signed char far *s_MakeDirectory;
extern signed int s_MaxX, s_MaxY;
extern signed char far *s_ReadMode;
extern signed char far *s_WriteMode;
extern signed char far *s_Title;
extern signed char far *s_TitleString;
extern signed int s_mx, s_my, s_mb;
extern signed int s_key;
extern signed char far *s_scankey;
extern signed char far *s_asciikey;
extern signed char far *s_swapdir;
extern signed char far *s_datadir;
extern signed char far *s_dicdir;
extern signed char far *s_compressstring;
extern signed char s_nowgetmode;
extern signed char s_nowinsertmode;
extern unsigned int s_autogets;
extern WindowData s_wd;
extern signed char s_clock;
extern signed int s_saver;

extern void far MemoryOverFlow(void);
extern signed char far * far MakePath(signed char far *n_string);
extern void far MC(signed char n_flag);
extern void far MouseProcess(void);
extern void far TimeProcess(void);
extern signed int far MouseCheck(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2, signed int n_mb);
extern signed int far WindowProcess(WindowData far *n_wd);
extern void far UnLink(signed char far *n_name);
extern signed long far ComputeWindow(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2);
extern void far Help(signed char far *n_string);
extern signed int far Compress(signed char far *source, signed char far *target, signed char far *n_header =s_compressstring);
extern signed int far DeCompress(signed char far *source, signed char far *target, signed char far *n_header =s_compressstring);
extern signed int far Gets(signed int n_x, signed int n_y, signed char far *n_default, signed char far *n_string, signed int n_maxchar, unsigned char n_color, unsigned char n_bcolor);
extern void far SaveConfig(MainConfig far *n_mc);
extern void far LoadConfig(MainConfig far *n_mc);
extern void far BackGround(signed char n_sw, signed int n_select =N_Default);
extern void far DeCompressData(signed char far *n_string);
extern void far ClockTimer(void);

#endif __JACK_COMMON__

/* End of source */