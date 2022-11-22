/*
   TwoN Group's Native source : MAIN
*/

#define N_16BIT_NATIVE

#include <bios.h>
#include <conio.h>
#include <ctype.h>
#include <dir.h>
#include <direct.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>

#ifndef N_16BIT_TABLE
#  define N_16BIT_TABLE         (0)
#  define Class                 class
#  ifdef N_16BIT_NATIVE
     typedef void                 DATA_V00;
     typedef signed char          DATA_D08;
     typedef signed int           DATA_D16;
     typedef signed long          DATA_D32;
     typedef unsigned char        DATA_U08;
     typedef unsigned int         DATA_U16;
     typedef unsigned long        DATA_U32;
     typedef float                DATA_F32;
     typedef double               DATA_D64;
#    if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
       typedef signed char *      DATA_S00;
#    else
       typedef signed char far *  DATA_S00;
#    endif
#  endif
#endif

#ifndef N_Far
#  if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#    define N_Far                far
#  else
#    define N_Far
#  endif
#endif

#ifndef N_NIL
#  if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#    define N_NIL                (0)
#  else
#    define N_NIL                (0L)
#  endif
#endif

#ifndef N_TRUE
#  define N_TRUE             (1)
#endif

#ifndef N_FALSE
#  define N_FALSE            (0)
#endif

#ifndef N_ERROR
#  define N_ERROR            (-1)
#endif

#ifndef N_SUCCESS
#  define N_SUCCESS          (1)
#endif

#ifndef N_TITLE
#  define N_TITLE            "NDLOVE"
#  define N_VER              "Ver1.00"
#  define N_COMPANY          "TwoN Group"
#endif

#define N_MAXSWAPTABLE       256

#ifndef N_COLOR_TABLE
#  define N_COLOR_TABLE
enum N_COLOR_TABLE{
                   N_BLACK=N_NIL,        /* 0x00 */
                   N_BLUE,         /* 0x01 */
                   N_GREEN,        /* 0x02 */
                   N_CYAN,         /* 0x03 */
                   N_RED,          /* 0x04 */
                   N_MAGENTA,      /* 0x05 */
                   N_BROWN,        /* 0x06 */
                   N_LIGHTGRAY,    /* 0x07 */
                   N_DARKGRAY,     /* 0x08 */
                   N_LIGHTBLUE,    /* 0x09 */
                   N_LIGHTGREEN,   /* 0x0a */
                   N_LIGHTCYAN,    /* 0x0b */
                   N_LIGHTRED,     /* 0x0c */
                   N_LIGHTMAGENTA, /* 0x0d */
                   N_YELLOW,       /* 0x0e */
                   N_WHITE         /* 0x0f */
                  };
enum N_VGA_MODE{ModeText=N_NIL, Mode320, Mode640, Mode640L, Mode800, Mode1024, Mode1280};
#endif

typedef struct{
               DATA_D08 Argc;
               DATA_S00 Argv;
              }MainStruct__;

static MainStruct__ MainStore;

class TextClass{
                public:
                DATA_V00 N_Far cdecl Putc(DATA_D08, DATA_D08, DATA_D08, DATA_U08);
                DATA_V00 N_Far cdecl Puts(DATA_D08, DATA_D08, DATA_S00, DATA_U08);
                DATA_V00 N_Far cdecl FillBox(DATA_D08, DATA_D08, DATA_D08, DATA_D08, DATA_U08, DATA_D08 =0x20);
                DATA_V00 N_Far cdecl Box(DATA_D08, DATA_D08, DATA_D08, DATA_D08, DATA_U08, DATA_U08 =1);
                DATA_V00 N_Far cdecl RecBox(DATA_D08, DATA_D08, DATA_D08, DATA_D08, DATA_U08,DATA_U08 =1);
                DATA_V00 N_Far cdecl Printf(DATA_D08, DATA_D08, DATA_U08, DATA_S00, ...);
                DATA_V00 N_Far cdecl MPuts(DATA_D08, DATA_S00, DATA_U08);
                DATA_V00 N_Far cdecl LineSpace(DATA_D08, DATA_U08);
                DATA_V00 N_Far cdecl ViewCursor(DATA_V00);
                DATA_V00 N_Far cdecl HideCursor(DATA_V00);
                DATA_V00 N_Far cdecl BlinkOFF(DATA_V00);
                DATA_V00 N_Far cdecl BlinkON(DATA_V00);
               }TextClass;

DATA_V00 main(DATA_D08 Argc, DATA_S00 Argv)
{
 DATA_D16 Counter;
 DATA_D16 Key;
           nosound();
           MainStore.Argc=Argc;
           MainStore.Argv=Argv;
           asm MOV AX, 0x0003;
           asm INT 0x10;
           TextClass.HideCursor();
           TextClass.BlinkOFF();
           TextClass.FillBox(0, 0, 79, 24, DARKGRAY, 0xb1);
           TextClass.LineSpace(24, (LIGHTGRAY<<4));
           TextClass.MPuts(24, "NDLOVE's utillity service #NumberTen", (LIGHTGRAY<<4)+DARKGRAY);

           for(Counter=N_NIL;Counter<37;Counter++, delay(15))TextClass.RecBox(39-Counter, 1, 40+Counter, 5, (BLUE<<4)+WHITE, 2);
           TextClass.MPuts(2, "* Clear ROM-BIOS Version1.00 [ SHARE WARE VERSION ] *", (BLUE<<4)+YELLOW);
           TextClass.MPuts(3, "Copyright(c)1997 TwoNgroup  By JaeHyuk.CHO", (BLUE<<4)+LIGHTGRAY);
           TextClass.MPuts(4, "The computer must be developed along with the TwoN Group!", (BLUE<<4)+LIGHTGRAY);
           TextClass.FillBox(10, 10, 69, 14, (DARKGRAY<<4));
           TextClass.Box(11, 10, 68, 14, (DARKGRAY<<4)+WHITE, 2);
           TextClass.MPuts(11, "WARNING! : ROM-BIOS data will be lost!", (DARKGRAY<<4)+LIGHTCYAN);
           TextClass.MPuts(13, "Are you sure? (y/N)", (DARKGRAY<<4)+LIGHTCYAN);
           while(kbhit())getch();
           while(N_TRUE)
           {
            Key=N_NIL;
            if(kbhit())
            {
             Key=getch();
             if(Key==N_NIL||kbhit())Key=N_NIL;
             if(Key=='y'||Key=='Y')
             {
              asm{XOR BX, BX;
                  MOV BL, 0x10;}
                  Again_Loop:;
              asm{
                  MOV AL, BL;
                  OUT 0x70, AL;
                  OUT 0xed, AL;
                  MOV AL, 0xff;
                  OUT 0x71, AL;
                  INC BX;
                  CMP BX, 0x0080;
                  JB Again_Loop;
                  }
              while(kbhit())getch();
              TextClass.FillBox(10, 10, 69, 14, (DARKGRAY<<4));
              TextClass.Box(11, 10, 68, 14, (DARKGRAY<<4)+WHITE, 2);
              TextClass.MPuts(11, "Press any key to reboot...", (DARKGRAY<<4)+LIGHTCYAN);
              TextClass.MPuts(13, "[ TwoN Group - Better one's working conditions! ]", (DARKGRAY<<4)+LIGHTCYAN);
              while(!kbhit());
              while(kbhit())getch;
              {
               void (far *coldboot)(void);
	       disable();
	       (void far *)coldboot=(void far *)0xFFFF0000L;
	       *(unsigned int far *)0x00000472L=(unsigned int)0x0000;
	       enable();
	       coldboot();
              }
             }
             if(Key=='n'||Key=='N'||Key==0x0d||Key==0x1b)break;
            }
           }
           TextClass.BlinkON();
           TextClass.ViewCursor();
           asm MOV AX, 0x0003;
           asm INT 0x10;
           printf("ROM-BIOS data safety.\n");
}

DATA_V00 N_Far TextClass::ViewCursor(DATA_V00)
{
	   asm MOV AX,0x0100;asm MOV CX,0x0C0D;asm INT 0x10;
}

DATA_V00 N_Far TextClass::HideCursor(DATA_V00)
{
	   asm MOV AX,0x0100;asm MOV CX,0x200D;asm INT 0x10;
}

DATA_V00 N_Far TextClass::BlinkON(DATA_V00)
{
	   asm MOV AX,0x1003;asm MOV BL,0x01;asm INT 0x10;
}

DATA_V00 N_Far TextClass::BlinkOFF(DATA_V00)
{
	   asm MOV AX,0x1003;asm MOV BL,0x00;asm INT 0x10;
}


DATA_V00 N_Far TextClass::LineSpace(DATA_D08 y, DATA_U08 Color)
{
 DATA_D08 CounterX;for(CounterX=N_NIL;CounterX<80;CounterX++)TextClass::Putc(CounterX, y, 0x20, Color);
}

DATA_V00 N_Far TextClass::MPuts(DATA_D08 y, DATA_S00 String, DATA_U08 Color)
{
           TextClass::Puts(40-(strlen(String)/2), y, String, Color);
}

DATA_V00 N_Far TextClass::Printf(DATA_D08 x, DATA_D08 y, DATA_U08 Color, DATA_S00 StringBuffer, ...)
{
 DATA_S00 String=new DATA_D08 N_Far [80];vsprintf(String, StringBuffer, ...);TextClass::Puts(x, y, String, Color);delete String;
}

DATA_V00 N_Far TextClass::RecBox(DATA_D08 x1, DATA_D08 y1, DATA_D08 x2, DATA_D08 y2, DATA_U08 Color, DATA_U08 Line)
{
           TextClass::Box(x1++, y1++, x2--, y2--, Color, Line);TextClass::FillBox(x1, y1, x2, y2, Color);
}

DATA_V00 N_Far TextClass::Box(DATA_D08 x1, DATA_D08 y1, DATA_D08 x2, DATA_D08 y2, DATA_U08 Color, DATA_U08 Line)
{
 DATA_U08 Char[6];DATA_D08 Counter;
           switch(Line)
           {
            case 0: Char[0]=0x20;Char[1]=0x20;Char[2]=0x20;Char[3]=0x20;Char[4]=0x20;Char[5]=0x20;break;
            case 1: Char[0]=218;Char[1]=191;Char[2]=192;Char[3]=217;Char[4]=179;Char[5]=196;break;
            case 2: Char[0]=201;Char[1]=187;Char[2]=200;Char[3]=188;Char[4]=186;Char[5]=205;break;
            case 3: Char[0]=213;Char[1]=184;Char[2]=212;Char[3]=190;Char[4]=179;Char[5]=205;break;
            case 4: Char[0]=214;Char[1]=183;Char[2]=211;Char[3]=189;Char[4]=186;Char[5]=196;break;
            default: Char[0]=Char[1]=Char[2]=Char[3]=Char[4]=Char[5]=N_NIL; break;
           }
           TextClass.Putc(x1, y1, (DATA_D08)Char[0], Color);TextClass.Putc(x2, y1, (DATA_D08)Char[1], Color);TextClass.Putc(x1, y2, (DATA_D08)Char[2], Color);TextClass.Putc(x2, y2, (DATA_D08)Char[3], Color);
           for(Counter=x1+1;Counter<x2;Counter++){TextClass::Putc(Counter, y1, (DATA_D08)Char[5], Color);TextClass::Putc(Counter, y2, (DATA_D08)Char[5], Color);}
           for(Counter=y1+1;Counter<y2;Counter++){TextClass::Putc(x1, Counter, (DATA_D08)Char[4], Color);TextClass::Putc(x2, Counter, (DATA_D08)Char[4], Color);}
}

DATA_V00 N_Far TextClass::FillBox(DATA_D08 x1, DATA_D08 y1, DATA_D08 x2, DATA_D08 y2, DATA_U08 Color, DATA_D08 Char)
{
 DATA_D08 CounterX;for(;y1<=y2;y1++){for(CounterX=x1;CounterX<=x2;CounterX++)TextClass::Putc(CounterX, y1, Char, Color);}
}

DATA_V00 N_Far TextClass::Putc(DATA_D08 x, DATA_D08 y, DATA_D08 Char, DATA_U08 Color)
{
 DATA_U08 N_Far *VideoRam=((DATA_U08 N_Far *)0xb8000000L+(y*160)+(x<<1));*(VideoRam++)=(DATA_U08)Char;*(VideoRam  )=Color;
}

DATA_V00 N_Far TextClass::Puts(DATA_D08 x, DATA_D08 y, DATA_S00 String, DATA_U08 Color)
{
           while(*String)TextClass::Putc(x++, y, *(String++), Color);
}

/* End of Native-code */
/* End. */