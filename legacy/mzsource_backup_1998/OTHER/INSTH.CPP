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
#  define N_TITLE            ""
#  define N_VER              "Ver1.00"
#  define N_COMPANY          "PowerHacker Group"
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
               DATA_S00 *Argv;
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
                DATA_V00 N_Far cdecl DelayMPuts(DATA_D08, DATA_S00, DATA_U08);
                DATA_S00 N_Far cdecl GetString(DATA_D08, DATA_D08, DATA_D08, DATA_S00, DATA_U08);
               }TextClass;

typedef struct{
               DATA_D08 ID;
               DATA_D08 Directory[MAXPATH];
               DATA_D08 FileName[13];
               DATA_D32 FileSize;
               DATA_U08 Pass;
               DATA_U08 Reserved;
              }InstallHeader__;

typedef struct{
               DATA_D08 Title[20];
              }InstallMainHeader__;

#define N_STRING01    "\nSecret menu : Install maker versionSECRET for PowerHacker\n"
#define N_STRING02    "Copyright(c)1997 By PowerHacker"
#define N_STRING03    "* Power-package install utillity versionUNKNOWN *"
#define N_STRING04    "The computer must be developed along with the PowerHacker Group!"
#define N_STRING05    "* Power? Very good~ *"
#define N_STRING06    "1. All hacking? No problem!             "
#define N_STRING07    "2. Install package for MS-DOS           "
#define N_STRING08    "3. Self extract data compressing        "
#define N_STRING09    "4. Dinamic library link system          "
#define N_STRING10    "Press any key to continue."
#define N_STRING11    "                *  Get imformation box  *               "
#define N_STRING12    "Better one's working conditions."
#define N_STRING13    "Quality matter more than quantity."
#define N_STRING14    " -> Input directory (ESC:Cancel) <-"
#define N_STRING15    "C:\\POWER\\GROUP"
#define N_BUFFERSIZE  30000

DATA_D08 OffsetX;
DATA_D08 Path[MAXPATH];
DATA_D08 NowPath[MAXPATH];
DATA_D08 RunFlag=N_TRUE;

DATA_D32 N_Far cdecl Percent(DATA_D08, DATA_D32, DATA_D32);
DATA_D16 N_Far cdecl Install(DATA_V00);

DATA_V00 main(DATA_D08 Argc, DATA_S00 *Argv)
{
 DATA_D16 Counter;
 DATA_D16 Return;
           MainStore.Argc=Argc;MainStore.Argv=Argv;
           if(Argc==4)
           {
            FILE *fr, *fw;
            InstallHeader__ Header;
            printf(N_STRING01);
            if(strcmp(Argv[1], "Header")==N_NIL)
            {
             if((fr=fopen(Argv[0], "rb"))==N_NIL)exit(N_ERROR);
             fseek(fr, 0, SEEK_END);
             Header.ID=0x64;
             Header.FileSize=ftell(fr);
             Header.Pass=random(256);
             fclose(fr);
             if((fw=fopen("install.pak", "wb"))==N_NIL)exit(N_ERROR);
             fwrite(&Header, sizeof(InstallHeader__), 1, fw);
             fclose(fw);
             printf("Header maker : OK   - by JaeHyuk.CHO [012-846-6420]\n");
             exit(N_SUCCESS);
            }
            else if(strcmp(Argv[1], "TwoN")==N_NIL)
            {
             DATA_D16 Data;
             DATA_U08 *Buffer;
             DATA_D32 TotalLoop;
             DATA_D32 TotalCounter;
             if((fr=fopen(Argv[3], "rb"))==N_NIL)exit(N_ERROR);
             if((fw=fopen(Argv[0], "ab"))==N_NIL){fclose(fr);exit(N_ERROR);}
             Header.ID=0x65;
             strcpy(Header.Directory, Argv[2]);
             strcpy(Header.FileName, Argv[3]);
             Header.Pass=random(255)+1;
             fseek(fr, 0, SEEK_END);
             Header.FileSize=ftell(fr);
             fseek(fr, 0, SEEK_SET);
             fwrite(&Header, sizeof(Header), 1, fw);
             printf("Write data-header : OK\n");
             //while((Data=fgetc(fr))!=EOF)fputc(Data^Header.Pass, fw);
             Buffer=new DATA_U08 N_Far [10000];
             TotalLoop=Header.FileSize/10000L;
             for(TotalCounter=N_NIL;TotalCounter<TotalLoop;TotalCounter++)
             {
              fread(Buffer, 10000, 1, fr);
              for(Data=N_NIL;Data<10000;Data++)Buffer[Data]^=Header.Pass;
              fwrite(Buffer, 10000, 1, fw);
             }
             TotalLoop=Header.FileSize%10000L;
             fread(Buffer, (size_t)TotalLoop, 1, fr);
             for(Data=N_NIL;Data<(DATA_D16)TotalLoop;Data++)Buffer[Data]^=Header.Pass;
             fwrite(Buffer, (size_t)TotalLoop, 1, fw);
             fclose(fr);
             fclose(fw);
             delete Buffer;
             printf("Data addtion      : OK\n");
             exit(N_SUCCESS);
            }
            else exit(N_ERROR);
           }
           if(Argc!=1)
           {
            printf("\nNDLOVE install pack.\nUsage: %s            .ENTER.\n", Argv[0]);
            exit(N_ERROR);
           }
           asm MOV AX, 0x0003;
           asm INT 0x10;
           TextClass.BlinkOFF();
           TextClass.HideCursor();
           TextClass.FillBox(0, 0, 79, 24, DARKGRAY, 0xb2);
           TextClass.LineSpace(24, (WHITE<<4));
           TextClass.DelayMPuts(24, N_STRING02, (WHITE<<4)+DARKGRAY);
           for(Counter=N_NIL;Counter<38;Counter++)
           {
            if(!kbhit())delay(15);
            TextClass.RecBox(39-Counter, 1, 40+Counter, 5, (BLUE<<4)+WHITE, 2);
           }
           TextClass.DelayMPuts(2, N_STRING03, (BLUE<<4)+YELLOW);
           TextClass.DelayMPuts(4, N_STRING04, (BLUE<<4)+LIGHTGRAY);
           for(Counter=N_NIL;Counter<5;Counter++)
           {
            if(!kbhit())delay(100);
            TextClass.RecBox(10, 15-Counter, 69, 16+Counter, (BLUE<<4)+WHITE, 3);
           }
           TextClass.DelayMPuts(12, N_STRING05, (BLUE<<4)+YELLOW);
           TextClass.DelayMPuts(14, N_STRING06, (BLUE<<4)+LIGHTCYAN);
           TextClass.DelayMPuts(15, N_STRING07, (BLUE<<4)+LIGHTCYAN);
           TextClass.DelayMPuts(16, N_STRING08, (BLUE<<4)+LIGHTCYAN);
           TextClass.DelayMPuts(17, N_STRING09, (BLUE<<4)+LIGHTCYAN);
           TextClass.DelayMPuts(19, N_STRING10, (BLUE<<4)+LIGHTBLUE);
           while(!kbhit());
           while(kbhit())getch();
           for(Counter=4;Counter>=0;Counter--)
           {
            if(!kbhit())delay(100);
            TextClass.FillBox(10, 14-Counter, 69, 14-Counter, DARKGRAY, 0xb2);
            TextClass.FillBox(10, 17+Counter, 69, 17+Counter, DARKGRAY, 0xb2);
            TextClass.RecBox(10, 15-Counter, 69, 16+Counter, (BLUE<<4)+WHITE, 3);
           }
           for(Counter=N_NIL;Counter<4;Counter++)
           {
            if(!kbhit())delay(100);
            TextClass.RecBox(10, 15-Counter, 69, 16+Counter, (BLUE<<4)+WHITE, 3);
           }
           TextClass.DelayMPuts(13, N_STRING11, (WHITE<<4)+BLUE);
           TextClass.DelayMPuts(15, N_STRING12, (BLUE<<4)+LIGHTGRAY);
           TextClass.DelayMPuts(16, N_STRING13, (BLUE<<4)+LIGHTGRAY);
           TextClass.DelayMPuts(17, N_STRING14, (BLUE<<4)+LIGHTCYAN);
           strcpy(Path, TextClass.GetString(12, 18, 55, N_STRING15, (BLUE<<4)+LIGHTGRAY));
           {
            DATA_U08 N_Far *VideoRam=((DATA_U08 N_Far *)0xb8000000L);
            DATA_D16 OffsetCounter;
            for(Counter=12;Counter>=8;Counter--)
            {
             if(!kbhit())delay(60);
             for(OffsetCounter=Counter*160;OffsetCounter<=(Counter+10)*160;OffsetCounter++)
             {
              *(VideoRam+OffsetCounter-160)=*(VideoRam+OffsetCounter);
             }
            }
           }
           for(Counter=N_NIL;Counter<32;Counter++)
           {
            delay(15);
            TextClass.RecBox(39-Counter, 16, 40+Counter, 22, (BLUE<<4)+WHITE, 4);
           }
           for(Counter=10;Counter<70;Counter++)
           {
            TextClass.Putc(Counter, 19, 0xb0, DARKGRAY);
            TextClass.Putc(Counter, 21, 0xb0, DARKGRAY);
           }
           TextClass.Puts(10, 18, "File  :", (BLUE<<4)+YELLOW);
           TextClass.Puts(10, 20, "Total :", (BLUE<<4)+YELLOW);
           TextClass.Puts(18, 20, "DATA", (BLUE<<4)+WHITE);
           TextClass.DelayMPuts(17, "                                                            ", (WHITE<<4)+RED);
           TextClass.DelayMPuts(17, Path, (WHITE<<4)+RED);
           Return=Install();

           for(Counter=N_NIL;Counter<MAXPATH;Counter++)NowPath[Counter]=N_NIL;
           getcwd(NowPath, MAXPATH);
           {
            DATA_U08 N_Far *VideoRam=((DATA_U08 N_Far *)0xb8000000L);
            DATA_D16 OffsetCounter;
            for(Counter=6;Counter<=13;Counter++)
            {
             if(!kbhit())delay(60);
             for(OffsetCounter=(Counter+9)*160;OffsetCounter>=Counter*160;OffsetCounter--)
             {
              *(VideoRam+OffsetCounter+160)=*(VideoRam+OffsetCounter);
             }
            }
           }
           for(Counter=21;Counter>=16;Counter--)
           {
            if(!kbhit())delay(100);
            TextClass.RecBox(10, 15, 69, Counter, (BLUE<<4)+WHITE, 3);
            TextClass.FillBox(10, Counter+1, 69, Counter+1, DARKGRAY, 0xb2);
           }
           for(Counter=16;Counter<=21;Counter++)
           {
            if(!kbhit())delay(100);
            TextClass.RecBox(10, 15, 69, Counter, (BLUE<<4)+WHITE, 3);
           }
           TextClass.DelayMPuts(16, "                    - Message box -                     ", (WHITE<<4)+BLUE);
           switch(Return)
           {
            case 256: TextClass.DelayMPuts(18, "Success! Thank-you.", (BLUE<<4)+LIGHTCYAN);break;
            case (-1): TextClass.DelayMPuts(18, "CRC & Read error : Data-base", (BLUE<<4)+LIGHTRED);break;
            case  0: TextClass.DelayMPuts(18, "Found not \'install.pak\'", (BLUE<<4)+LIGHTRED);break;
            case  1:
            case  2: TextClass.DelayMPuts(18, "CRC error : \'install.pak\'", (BLUE<<4)+LIGHTRED);break;
            case  3:
            case  4: TextClass.DelayMPuts(18, "Found not data-base", (BLUE<<4)+LIGHTRED);break;
            case  5: TextClass.DelayMPuts(18, "Allocation error : Buffer-memory", (BLUE<<4)+LIGHTRED);break;
            case  6: TextClass.DelayMPuts(18, "CRC error : Data-base", (BLUE<<4)+LIGHTRED);break;
            case  7:
            case  9:
            case 11: TextClass.DelayMPuts(18, "Write error", (BLUE<<4)+LIGHTRED);break;
            case  8:
            case 10: TextClass.DelayMPuts(18, "Read error", (BLUE<<4)+LIGHTRED);break;
            case 12: TextClass.DelayMPuts(18, "Found not drive", (BLUE<<4)+LIGHTRED);break;
            case 13: TextClass.DelayMPuts(18, "Creat error : directory", (BLUE<<4)+LIGHTRED);break;
            case 14: TextClass.DelayMPuts(18, "Cancel process...", (BLUE<<4)+LIGHTRED);break;
            default: TextClass.DelayMPuts(18, "UnKnown error!!! WARNIG!!!", (BLUE<<4)+LIGHTRED);break;
           }
           if(Return!=256)
           {
            _chdrive(NowPath[0]-0x40);
            chdir(NowPath);
           }
           TextClass.DelayMPuts(20, "> Press any key to quit <", (BLUE<<4)+LIGHTRED);
           while(!kbhit());
           while(kbhit())getch();

           TextClass.BlinkON();
           TextClass.ViewCursor();
           asm MOV AX, 0x0003;
           asm INT 0x10;
           printf("End of install. Power Group\n");
}

DATA_D16 N_Far Install(DATA_V00)
{
 FILE *fr, *fw;
 DATA_D08 OffsetX01=10;
 DATA_D08 OffsetX02=10;
 InstallHeader__ Header;
 InstallHeader__ CHeader;
 DATA_U08 *Buffer;
 DATA_D32 Counter;
 DATA_D32 TotalLoop;
 DATA_D32 TotalSize;
 DATA_D32 FileSize;
 DATA_D32 DataSize;
 DATA_D08 MakePath_[MAXPATH];
 DATA_D16 UnLockCounter;
           if(RunFlag==N_FALSE)return (14);
           if((fr=fopen(MainStore.Argv[0], "rb"))==N_NIL)return N_ERROR;
           fseek(fr, 0, SEEK_END);
           DataSize=ftell(fr);
           fclose(fr);
           if((fr=fopen("install.pak", "rb"))==N_NIL)
           {
            if((fr=fopen(MainStore.Argv[0], "rb"))==N_NIL)return (0);
            fseek(fr, N_NIL-(sizeof(InstallHeader__)), SEEK_END);
            if(fread(&CHeader, sizeof(InstallHeader__), 1, fr)==N_NIL){fclose(fr);return (0);}
            fclose(fr);
            if(CHeader.ID!=0x64)return (0);
           }
           else
           {
            if(fread(&CHeader, sizeof(InstallHeader__), 1, fr)==N_NIL){fclose(fr);return (0);}
            fclose(fr);
           }
           if(CHeader.ID!=0x64&&CHeader.ID!=0x65)return (2);

           TotalLoop=N_NIL;
           if(Path[strlen(Path)-1]!='\\'&&Path[0]!=N_NIL)strcat(Path, "\\");
           if(Path[1]==':')
           {
            if(_chdrive(Path[0]-0x40)==(-1))return 12;
            Counter=3; chdir("\\");
           }
           else if(Path[0]=='\\')
           {
            Counter=1; chdir("\\");
           }
           else Counter=N_NIL;
           for(;Counter<(DATA_D32)strlen(Path);Counter++)
           {
            if(Path[(DATA_D16)Counter]=='\\')
            {
             MakePath_[(DATA_D16)TotalLoop]=N_NIL;
             TotalLoop=N_NIL;
             mkdir(MakePath_);
             if(chdir(MakePath_)==(-1))return 13;
            }
            else
            {
             MakePath_[(DATA_D16)TotalLoop]=Path[(DATA_D16)Counter];
             TotalLoop++;
            }
           }

           DataSize-=CHeader.FileSize;
           if((fr=fopen(MainStore.Argv[0], "rb"))==N_NIL)return (3);
           if(fseek(fr, (DATA_D32)CHeader.FileSize, SEEK_SET)!=N_NIL)return (4);
           Buffer=new DATA_U08 N_Far [N_BUFFERSIZE];
           if(Buffer==N_NIL){fclose(fr);return (5);}
           TotalSize=N_NIL;
           while(N_TRUE)
           {
            if(fread(&Header, sizeof(InstallHeader__), 1, fr)==N_NIL)break;
            if(Header.ID!=0x65)
            {
             if(Header.ID==0x64)break;
             fclose(fr);delete Buffer;return (6);
            }
            TotalLoop=Header.FileSize/(DATA_D32)N_BUFFERSIZE;
            if(Header.Directory[0]=='.')strcpy(Header.Directory, ".");
            else mkdir(Header.Directory);
            strcat(Header.Directory, "\\");
            strcat(Header.Directory, Header.FileName);
            TextClass.Printf(18, 18, (BLUE<<4)+WHITE, "%-12s", Header.FileName);
            if((fw=fopen(Header.Directory, "wb"))==N_NIL){fclose(fr);delete Buffer;return (7);}
            FileSize=N_NIL;
            for(Counter=10;Counter<70;Counter++)TextClass.Putc(Counter, 19, 0xb0, DARKGRAY);
            OffsetX01=10;
            for(Counter=N_NIL;Counter<TotalLoop;Counter++)
            {
             if(fread(Buffer, N_BUFFERSIZE, 1, fr)==N_NIL){fclose(fr);fclose(fw);delete Buffer;return (8);}
             for(UnLockCounter=N_NIL;UnLockCounter<N_BUFFERSIZE;UnLockCounter++)Buffer[UnLockCounter]^=Header.Pass;
             if(fwrite(Buffer, N_BUFFERSIZE, 1, fw)==N_NIL){fclose(fr);fclose(fw);delete Buffer;return (9);}
             TotalSize+=N_BUFFERSIZE;
             FileSize+=N_BUFFERSIZE;
             OffsetX=OffsetX01;
             OffsetX01=Percent(19, FileSize, Header.FileSize);
             OffsetX=OffsetX02;
             OffsetX02=Percent(21, TotalSize, DataSize);
             TextClass.Printf(18, 20, (BLUE<<4)+WHITE, "%3lu\%", (DATA_U32)((DATA_F32)(TotalSize*100L)/(DATA_F32)DataSize));
            }
            TotalLoop=Header.FileSize%(DATA_D32)N_BUFFERSIZE;
            if(fread(Buffer, (size_t)TotalLoop, 1, fr)==N_NIL){fclose(fr);fclose(fw);delete Buffer;return (10);}
            for(UnLockCounter=N_NIL;UnLockCounter<(DATA_D16)TotalLoop;UnLockCounter++)Buffer[UnLockCounter]^=Header.Pass;
            if(fwrite(Buffer, (size_t)TotalLoop, 1, fw)==N_NIL){fclose(fr);fclose(fw);delete Buffer;return (11);}
            TotalSize+=TotalLoop;
            OffsetX=OffsetX01;
            OffsetX01=Percent(19, Header.FileSize, Header.FileSize);
            OffsetX=OffsetX02;
            OffsetX02=Percent(21, TotalSize, DataSize);
            fclose(fw);
           }
           OffsetX=10;OffsetX01=Percent(19, Header.FileSize, Header.FileSize);
           OffsetX=10;OffsetX02=Percent(21, DataSize, DataSize);
           TextClass.Puts(18, 20, "100\%", (BLUE<<4)+WHITE);
           fclose(fr);
           delete Buffer;
           return 256;
}

DATA_D32 N_Far Percent(DATA_D08 y, DATA_D32 Offset, DATA_D32 Total)
{
 DATA_D32 PointerX;
 DATA_D16 CounterX;
           PointerX=(DATA_D32)((DATA_F32)(Offset*59L)/(DATA_F32)Total)+10L;
           for(CounterX=OffsetX;CounterX<=PointerX;CounterX++)TextClass.Putc(CounterX, y, 0x20, (YELLOW<<4));
           return PointerX;
}




/* === Org code  ================================================ */

DATA_S00 N_Far TextClass::GetString(DATA_D08 x, DATA_D08 y, DATA_D08 Length, DATA_S00 DefaultString, DATA_U08 Color)
{
 DATA_D16 Counter;
 DATA_D08 Return[256];
 DATA_D08 Pointer;
 DATA_D16 Key;
 DATA_D08 Flag=N_FALSE;
           for(Counter=(DATA_D16)x;Counter<=(DATA_D16)(x+Length);Counter++)TextClass::Putc(Counter, y, 0x20, Color&0x0f);
           for(Counter=N_NIL;Counter<256;Counter++)Return[Counter]=N_NIL;
           strcpy(Return, DefaultString);
           TextClass::Puts(x, y, Return, BLACK+(WHITE<<4));
           Pointer=strlen(Return);
           gotoxy(x+strlen(Return)+1, y+1);
           TextClass::ViewCursor();
           while(kbhit())getch();
           while(N_TRUE)
           {
            Key=N_NIL;
            if(kbhit())
            {
             Key=getch();
             if(Key==N_NIL||kbhit()){Key=N_NIL;while(kbhit())getch();}
             switch(Key)
             {
              case 0x1b: strcpy(Return, DefaultString); RunFlag=N_FALSE; break;
              case 0x0d: break;
              case 0x00:
              case 0x20: sound(1000);delay(100);nosound(); break;
              case 0x07:
                   if(Flag==N_FALSE)
                   {
                    Pointer=N_NIL;
                    for(Counter=N_NIL;Counter<256;Counter++)Return[Counter]=N_NIL;
                    for(Counter=(DATA_D16)x;Counter<=(DATA_D16)(x+Length);Counter++)TextClass::Putc(Counter, y, 0x20, Color&0x0f);
                    gotoxy(x+1, y+1);
                   }
                   if(Pointer<Length)
                   {
                    Return[Pointer]=(DATA_D08)0x20;
                    TextClass.Putc(x+Pointer++, y, (DATA_D08)Key, Color&0x0f);
                    gotoxy(x+Pointer+1, y+1);
                   }
                   break;
              case 0x08:
                   if(Pointer>N_NIL)
                   {
                    Return[--Pointer]=N_NIL;
                    TextClass.Putc(x+Pointer, y, 0x20, Color&0x0f);
                    gotoxy(x+Pointer+1, y+1);
                   }
                   break;
              default:
                   if(Flag==N_FALSE)
                   {
                    Pointer=N_NIL;
                    for(Counter=N_NIL;Counter<256;Counter++)Return[Counter]=N_NIL;
                    for(Counter=(DATA_D16)x;Counter<=(DATA_D16)(x+Length);Counter++)TextClass::Putc(Counter, y, 0x20, Color&0x0f);
                    gotoxy(x+1, y+1);
                   }
                   if(Pointer<Length)
                   {
                    Return[Pointer]=toupper((DATA_D08)Key);
                    TextClass.Putc(x+Pointer, y, toupper((DATA_D08)Key), Color&0x0f);
                    Pointer++;
                    gotoxy(x+Pointer+1, y+1);
                   }
                   break;
             }
             if(Flag==N_FALSE)
             {
              Flag=N_TRUE;
              for(Counter=(DATA_D16)x;Counter<=(DATA_D16)(x+Length);Counter++)TextClass::Putc(Counter, y, 0x20, Color&0x0f);
              gotoxy(x+Pointer+1, y+1);
              TextClass.Puts(x, y, Return, Color&0x0f);
             }
             if(Key==0x1b||Key==0x0d)break;
             while(kbhit())getch();
            }
           }
           TextClass::HideCursor();
           return Return;
}

DATA_V00 N_Far TextClass::DelayMPuts(DATA_D08 y, DATA_S00 String, DATA_U08 Color)
{
 DATA_D16 Counter;
 DATA_D08 String1[100];
            for(Counter=N_NIL;Counter<strlen(String);Counter++)
            {
             String1[Counter]=String[Counter];
             String1[Counter+1]=N_NIL;
             if(!kbhit())delay(8);
             TextClass.MPuts(y, String1, Color);
            }
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