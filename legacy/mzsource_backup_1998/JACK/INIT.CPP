/*
  JACK's INIT.CPP
*/

#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <new.h>
#include <string.h>
#include <alloc.h>
#include <conio.h>
#include <io.h>
#include <dir.h>
#include "common.h"
#include "graph.h"
#include "himem.h"
#include "autohan.h"
#include "timer.h"
#include "asmproc.h"
#include "jf.h"

signed char far *s_SVGA_Chip[]={"UnKnown", "ACUMOS", "ATI VGA wonder", "AHEAD_A", "AHEAD_B",\
                               "CHIP STECH", "Cirrus logic", "EverEX", "Genoa", "NCR",  \
                               "OAKTECH", "Paradise", "RealTEK", "Trident", "Tseng3",   \
                               "Tseng4", "VESA complete", "Video seven", "Avance", "MXIC", "PriMus", NULL};
void interrupt (far *Key_Old_Handler)(void);
signed char far *s_ScreenSwap="JACKSWAP\\~SCR.SWP";

void far StartUpKey(void);
void far CloseUpKey(void);

void far DetectOption(void)
{
 signed int n_oc;
 signed char far *n_completestring="\nComplete data compress.\n";
 signed char far *n_errorstring="\nCann\'t compress!\n";
 signed char far *n_tempstring="~jcswp.tmp";
 signed char n_stop=N_False;
           s_MD.IsSWAP=N_False;
           for(n_oc=1;n_oc<_argc;n_oc++)
           {
            if(_argv[n_oc][0]=='-'||_argv[n_oc][0]=='/')
            {
             switch(_argv[n_oc][1])
             {
              case 's':
              case 'S': n_stop=N_True; s_MD.IsSWAP=N_True; break;
              case 'a':
              case 'A':
              case 'c':
              case 'C':
                   if(_argc<3)
                   {
                    printf("\n\aInvaid parameter!\n");
                    exit(N_False);
                   }
                   *(s_compressstring+17)='J';
                   if(_argc==3||strcmp(_argv[2], _argv[3])==NULL)
                   {
                    printf("\nCompressing file: %s", _argv[2]);
                    if(Compress(_argv[2], n_tempstring)==N_True)
                    {
                     UnLink(_argv[2]);
                     rename(n_tempstring, _argv[2]); printf(n_completestring);
                    }
                    else printf(n_errorstring);
                    UnLink(n_tempstring);
                   }
                   else
                   {
                    printf("\nCompressing file: %s to %s", _argv[2], _argv[3]);
                    if(Compress(_argv[2], _argv[3], ((_argc==6)?_argv[4]:s_compressstring))==N_True)printf(n_completestring);
                    else printf(n_errorstring);
                   }
                   exit(1);
              case 'e':
              case 'E':
              case 'x':
              case 'X':
                   if(_argc<3){printf("\n\aInvaid parameter!\n"); exit(N_False);}
                   *(s_compressstring+17)='J';
                   if(_argc==3||strcmp(_argv[2], _argv[3])==NULL)
                   {
                    printf("\nDeCompressing file: %s", _argv[2]);
                    if(DeCompress(_argv[2], n_tempstring)==N_True)
                    {
                     UnLink(_argv[2]); rename(n_tempstring, _argv[2]);
                     printf(n_completestring);
                    }
                    else printf(n_errorstring);
                    UnLink(n_tempstring);
                   }
                   else
                   {
                    printf("\nDeCompressing file: %s to %s", _argv[2], _argv[3]);
                    if(DeCompress(_argv[2], _argv[3], ((_argc==6)?_argv[4]:s_compressstring))==N_True)printf(n_completestring);
                    else printf(n_errorstring);
                   }
                   exit(1);
              default:
                   printf("\n\aInvalid option!\n");
              case '?':
              case 'h':
              case 'H':
                   printf("\nComputer programer of JACK & JACK must be developed along with the computer.\n");
                   printf("Copyright(c)JACK & JACK soft 1996-1998 by JaeHyuk.CHO  UNREGISTERED version.\n\n");
                   printf("Usage : JACK [-|/ <switch>] [-|/ <switch>] ...\n");
                   printf("Option: -? or -H   show this help message\n");
                   printf("        -A or -C   compress file         * JACK -[A|C] src [dest]\n");
                   printf("        -E or -X   decompress file       * JACK -[E|X] src [dest]\n");
                   printf("        -S         run swapping mode\n");
                   printf("        -V         view complement VGA card\n");
                   printf("        -Gn        Select VGA card       Ex(VESA) * JACK -G16\n");
                   printf("        -R         re-boot\n");
                   printf("        -Nn        Screen saver          Ex(FIRE) * JACK -C0\n");
                   printf("        -P         Disk parking\n");
                   exit(1);
                   break;
              case 'p':
              case 'P':
                   while(kbhit())getch();
                   printf("\nParking complete. turn off your computer!\n");
                   asmpark();
                   while(!kbhit());
                   while(kbhit())getch();
                   printf("Reset disk.\n");
                   exit(1);
              case 'g':
              case 'G':
                   n_stop=N_True;
                   n_oc=atoi(_argv[n_oc]+2);
                   if(n_oc!=0&&n_oc<=20)
                   {
                    setcard((VGAChipset)n_oc, 1024);
                    printf("\nSelect video card number: %d - %s\n", n_oc, s_SVGA_Chip[n_oc]);
                    printf("Press any key to continue \(Cancel: Ctrl-Break\).\n");
                    while(!kbhit()); while(kbhit())getch();
                    break;
                   }
              case 'v':
              case 'V':
                   printf("\nComplement VGA card list [1-20]\n\n"); n_oc=1;
                   while(s_SVGA_Chip[n_oc])
                   {
                    printf("-G%-2d %s\n", n_oc, s_SVGA_Chip[n_oc]);
                    n_oc++;
                   }
                   exit(1);
              case 'r':
              case 'R':
                   *((unsigned char far *)0x00000472L)=0x00;
                   printf("\n\nSending re-boot signal...[wait]\n");
                   sleep(1);
                   {
                    void (far *Boot)(void);
                    (void far *)Boot=(void far *)0xffff0000L;
                    Boot();
                   }
                   printf("\n\nFail! - error\n"); exit(1);
              case 'n':
              case 'N':
                   n_oc=atoi(_argv[n_oc]+2);
                   whichvga();
                   switch(n_oc)
                   {
                    case 0x01: Saver2(N_False); break;
                    case 0x00:
                    default:   Saver1(N_False); break;
                   }
                   restext();
                   printf("Screen saver v1.00 - by JaeHyuk.CHO [Pager:012-846-6420]\n");
                   exit(1);
             }
             if(n_stop==N_True)break;
            }
            else
            {
            }
           }
}

void far InitScreen(void)
{
           pageactive (1);
           pagedisplay (0, 0, 1);
           Puts(0, 0, "\x10 Loading data. . . [Page action]", WHITE, N_Default);
           pageactive (0);
           BackGround(N_False);
           drwline(SET, WHITE, 0, 480, 639, 480);
           Window(5, 485, 634, 575, "", 236, NULL, NULL);
           PushRec(9, 511, 630, 571);
           PopBox(10, 512, 629, 570, LIGHTGRAY);
           PutImageDisk(0, s_MaxY-21, 639, s_MaxY-1, "jackswap\\jack03.jim", 16);
           UnLink(MakePath("jackswap\\jack03.jim"));
           PopRec(0, s_MaxY-21, 38, s_MaxY-1);
           PopRec(39, s_MaxY-21, 77, s_MaxY-1);
           PopRec(80, s_MaxY-21, 133, s_MaxY-1);
           PopRec(136, s_MaxY-21, 549, s_MaxY-1);
           PopBox(550, s_MaxY-21, 639, s_MaxY-1, 27);
           NowGetMode();
           if(s_MD.IsHIMEM==N_True)
           {
            Puts(84, s_MaxY-17, "32§¡Ëa", WHITE, 27);
            Puts(83, s_MaxY-18, "32§¡Ëa", DARKGRAY, N_Default);
           }
           else
           {
            Puts(84, s_MaxY-18, " SWAP ", WHITE, 27);
            Puts(83, s_MaxY-19, " SWAP ", DARKGRAY, N_Default);
           }
           if(s_MaxX==800)
           {
            PutImageDisk(641, 0, s_MaxX-1, s_MaxY-1, "jackswap\\jack02.jim", 16);
            UnLink(MakePath("jackswap\\jack02.jim"));
            PopRec(641, 0, s_MaxX-1, 30);
            Puts(663, 9, "É·Ðs ­¡ÏaËa¶Á´á", WHITE, N_Default);
            Puts(662, 8, "É·Ðs ­¡ÏaËa¶Á´á", DARKGRAY, N_Default);
            PopRec(642, 1, s_MaxX-2, 29);
            PopRec(641, 32, s_MaxX-1, s_MaxY-23);
            PopRec(641, s_MaxY-21, s_MaxX-1, s_MaxY-1);
            Puts(653, s_MaxY-18, s_TitleString, WHITE, N_Default);
            Puts(652, s_MaxY-19, s_TitleString, BLACK, N_Default);
            //PushRec(644, 350, s_MaxX-4, s_MaxY-35);
            //PopRec(645, 351, s_MaxX-5, s_MaxY-36);
            /*drwfillbox(SET, DARKGRAY, 648, 354, s_MaxX-8, 370);
            Puts(680, 356, "”e Â‚ ¡A “A", BLACK, N_Default);
            Puts(679, 355, "”e Â‚ ¡A “A", WHITE, N_Default);
            PushRec(644, 207, s_MaxX-4, 347);
            PopRec(645, 208, s_MaxX-5, 346);*/
            //DeCompressData("jack09.jim");
            //PutImageDisk(645, 40, 795, 200, "jackswap\\jack09.jim", 80);
            //UnLink(MakePath("jackswap\\jack09.jim"));
            //PopRec(645, 40, 795, 200);
            //PushRec(646, 41, 794, 199);
           }
           pagedisplay (0, 0, 0);
           pageactive (0);
}

void far Title(void)
{
 WindowData n_wd;
 unsigned int n_titleswap=himemAlloc((unsigned int)((ComputeWindow(96, 175, 543, 383)>>10)+30L));
           DeCompressData("jack05.jim");
           Help("¥¥ Ïa¡‹aœ‘·e ¸á¸bŠ¥ ¥¡Ñ¡¤óµA ·Ð¬á ¥¡Ñ¡¤h¯s“¡”a."); MC(N_False);
           Window(96, 175, 543, 383, "¸á¸bŠ¥¸a: ¹¡¸Ñb", LIGHTGRAY, &n_wd, n_titleswap);
           PutImageDisk(100, 200, 539, 379, "jackswap\\jack05.jim", 48);
           UnLink(MakePath("jackswap\\jack05"));
           while((!kbhit()))
           {
            MouseProcess(); TimeProcess(); WindowProcess(&n_wd);
            if(n_wd.command==WIN_CLOSE){break;}
            if(s_mb==N_RIGHTBUTTON){break;}
            if(MouseCheck(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, N_LEFTBUTTON)==N_True){break;}
           }
           MC(N_False);
           PutImageXMS(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, n_titleswap);
           MC(N_True); himemFree(n_titleswap);
           while(kbhit())getch();
}

void far DetectDirectory(signed char far *n_path)
{
 union REGS Register;
           if(access(MakePath(n_path), NULL)!=NULL)
           {
            mkdir(MakePath(n_path));
            if(access(MakePath(n_path), NULL)!=NULL)
            {
             delete(s_WorkDirectory);
             printf("Build not directory! - %s\n", n_path);
             Register.h.ah=0x4c; int86(0x21, &Register, &Register);
            }
           }
           printf("Detect directory - %s\n", MakePath(n_path));
}

void far Start(void)
{
 FILE far *fp;
 union REGS Register;
 signed int n_counter;
 signed int n_whichvga;
 signed char far *n_detect="Detect ";
           asmwizard();
           s_MD.IsWin=N_False;
           s_MD.IsKeyHandle=N_False; while(kbhit())getch();
           Register.x.ax=0x0003; int86(0x10, &Register, &Register);
           n_whichvga=whichvga();
           printf(" Initializing %s - LAST UPDATE: %s    * CPU_type: 80x%d\n\n", s_Title, __DATE__, whichcpu());
           for(n_counter=1;n_counter<160;n_counter+=2)*((unsigned char far *)0xb8000000L+n_counter)=0x1f;
           printf("Copyright(c)1996-1997 %s software by JaeHyuk.CHO - COST $100.00\n\n", s_Title);
           if(_osmajor<5){printf("Incorrect DOS version! more than 5.00!");exit(N_Error);}
           printf("%sMS-DOS version%d.%d\n", n_detect, _osmajor, _osminor);
           printf("%sconventional memory - %lubyte\n", n_detect, (signed long)farcoreleft());
           printf("%sPSP segment: %04X\(Hex\)\n", n_detect, _psp);
           //if(_8087<=0){printf("Fount not FPU!\n"); exit(1);}
           printf("%sstack size: %d\(byte\)\n", n_detect, _stklen);
           Register.x.ax=0x0000; int86(0x33, &Register, &Register);
           if(Register.x.ax==0xffff){printf("%smouse device\n", n_detect); s_MD.IsMouse=N_True;}
           else s_MD.IsMouse=N_False;
           Register.x.ax=0xfd00; int86(0x10, &Register, &Register);
           if((Register.h.al&0xfd)==0xfd){printf("%shangul bios\n", n_detect); s_MD.IsHBIOS=N_True;}
           else s_MD.IsHBIOS=N_False;
           set_new_handler(MemoryOverFlow);
           s_MakeDirectory=new(signed char far [strlen(_argv[0])+1]);
           strcpy(s_MakeDirectory, _argv[0]);
           for(n_counter=strlen(_argv[0])-1;n_counter>=0;n_counter--)
           {
            if((*(s_MakeDirectory+n_counter))=='\\'){*(s_MakeDirectory+n_counter+1)=0x00; break;}
           }
           s_WorkDirectory=new(signed char far [strlen(s_MakeDirectory)+1]);
           strcpy(s_WorkDirectory, s_MakeDirectory); delete(s_MakeDirectory);
           printf("%swork_directory - %s\n", n_detect, s_WorkDirectory);
           DetectDirectory(s_swapdir);
           DetectDirectory(s_datadir);
           DetectDirectory(s_dicdir);
           DetectDirectory("DATA");
           s_MakeDirectory=new(signed char far [16]); // Allocate paragraph unit = 16byte
           if(n_whichvga==UNKNOWN)
           {
            printf("\nUnKnown video card!!!\n"); set_new_handler(0);
            Register.h.ah=0x4c; int86(0x21, &Register, &Register);
           }
           else {printf("%sSVGA_card - %s [AUTO]\n", n_detect, s_SVGA_Chip[n_whichvga]);}
           printf("Check LLE system - OK [R0.0.1]\n");
           himemInit();
           if(s_MD.IsHIMEM==N_False){printf("%snot himem driver -> SWAPPING mode selected\n", n_detect);}
           else
           {
            unsigned int n_big, n_total;
            printf("%shimem driver - ", n_detect);
            himemGetSize(&n_big, &n_total);
            if(s_MD.IsHIMEM==N_False)
            {
             #ifndef N_MAKE
             signed int n_key;
             #endif N_MAKE
             printf("but, cann't allocate! -> SWAPPING mode seleted\n");
             printf("%17c*** Do you want to swap_mode? \(Y\/n\) ***", 0x20);
             #ifndef N_MAKE
             n_key=getch();
             if(n_key=='n'||n_key=='N')
             {
              delete(s_MakeDirectory); delete(s_WorkDirectory);
              printf("\n\nCanceled swapping mode!\n"); set_new_handler(0);
              Register.h.ah=0x4c; int86(0x21, &Register, &Register);
             }
             #endif N_MAKE
            }
            else {printf("avail size: %u KByte\n", n_total);}
           }
           printf("\nStarting %s...", s_Title);
           DeCompressData("jack00.jim");
           DeCompressData("jack01.jim");
           s_EngFont=new(unsigned char far [2048]);
           s_HanFont=new(unsigned char far [11008]);
           if((fp=fopen(MakePath("jackswap\\jack00.jim"), s_ReadMode))==0)
           {
            delete(s_EngFont); delete(s_HanFont);
            delete(s_MakeDirectory); delete(s_WorkDirectory);
            printf("\nFound not font! - JACKDATA\\JACK00.JIM\n");
            set_new_handler(0);
            Register.h.ah=0x4c; int86(0x21, &Register, &Register);
           }
           fread(s_EngFont, (size_t)2048, 1, fp); fclose(fp);
           UnLink(MakePath("jackswap\\jack00.jim"));
           if((fp=fopen(MakePath("jackswap\\jack01.jim"), s_ReadMode))==NULL)
           {
            delete(s_EngFont); delete(s_HanFont);
            delete(s_MakeDirectory); delete(s_WorkDirectory);
            printf("\nFound not font! - JACK01.FNT\n");
            set_new_handler(0);
            Register.h.ah=0x4c; int86(0x21, &Register, &Register);
           }
           fread(s_HanFont, (size_t)11008, 1, fp); fclose(fp);
           UnLink(MakePath("jackswap\\jack01.jim"));
           DeCompressData("jack02.jim");
           DeCompressData("jack03.jim");
           DeCompressData("jack04.jim");
           DeCompressData("jack06.jim");
           BackGround(2); StartUpKey();
           randomize(); UnLink(MakePath(s_ScreenSwap));
           if((fp=fopen(MakePath(s_ScreenSwap), s_WriteMode))!=NULL)
           {
            fputc(wherex(), fp); fputc(wherey(), fp);
            for(n_counter=NULL;n_counter<4094;n_counter++)fputc(*((unsigned char far *)0xb8000000L+n_counter), fp);
            fclose(fp);
           }
           res800(); s_MaxX=maxx+1; s_MaxY=maxy+1;
           if(s_MD.IsMouse==N_True){s_mx=615; s_my=490;}
           drwfillbox(SET, BLACK, 0, 0, s_MaxX-1, s_MaxY-1); InitScreen(); MC(N_True);
           {                            /*236         237        238         239*/
            unsigned char n_colortable[]={25, 40, 45, 5, 15, 20, 40, 50, 40, 1, 35, 30, NULL};
            outportb(0x3c8, 236);
            n_counter=NULL;
            while(n_colortable[n_counter])
            {
             outportb(0x3c9, n_colortable[n_counter++]);
            }
           }
           #ifndef N_MAKE
           Title();
           #endif N_MAKE
}

void far Close(void)
{
 FILE far *fp;
 union REGS Register;
 signed int n_counter;
           UnLink(MakePath("jackswap\\jack04.jim"));
           UnLink(MakePath("jackswap\\jack05.jim"));
           UnLink(MakePath("jackswap\\jack06.jim"));
           himemAllFree(); Register.x.ax=0x0003; int86(0x10, &Register, &Register);
           if((fp=fopen(MakePath(s_ScreenSwap), s_ReadMode))!=NULL)
           {
            signed char n_tx, n_ty;
            n_tx=fgetc(fp); n_ty=fgetc(fp);
            for(n_counter=NULL;n_counter<4000;n_counter++)
            {
             *((unsigned char far *)0xb8000000L+n_counter)=(unsigned char)fgetc(fp);
            }
            fclose(fp); gotoxy(n_tx, n_ty);
           }
           printf("\n\n\nEnd of %s\n", s_Title);
           delete(s_MakeDirectory); delete(s_WorkDirectory);
           set_new_handler(0); while(kbhit())getch();
           CloseUpKey(); Register.h.ah=0x4c; int86(0x21, &Register, &Register);
}

void far CloseUpKey(void)
{
 struct Vect{signed char far *Vect;}far *Vect09=(struct Vect far *)0x0024;
           delete(s_scankey);
           delete(s_asciikey);
	         (signed char far *)Vect09->Vect=(signed char far *)Key_Old_Handler;
           s_MD.IsKeyHandle=N_False;
}

void interrupt Key_Handler(void)
{
 union REGS Register;
 unsigned char Code, Port;
	         asm CLI;
           asm PUSH AX;
           asm IN  AL, 0x60;
           asm MOV Port, AL;
           asm POP AX;
           Code=Port&127;
	         if((Port<128)&&(s_scankey[Code]==N_False))s_scankey[Code]=s_asciikey[Code]=N_True;
	         else if(Port>127)s_scankey[Code]=N_False;
	         asm PUSH AX;
	         asm MOV AL, 0x20;
	         asm OUT 0x20, AL;
	         asm POP AX;
	         if(s_scankey[0x1D]||s_scankey[0x38])
	         {
	          if(s_scankey[0x1D]&&s_scankey[0x38]&&s_scankey[0x01]&&s_scankey[0x36]&&s_scankey[0x2A])
	          {
             Register.x.ax=0x0003; int86(0x10, &Register, &Register);
	           printf("Exit process key!!! prease reboot!\n");
	           asm STI; CloseUpKey();
	           Register.h.ah=0x4c; int86(0x21, &Register, &Register);
	          }
	          asm STI;
	         }
	         else if(s_scankey[0x2A]&&s_scankey[0x37])asm STI;
	         else
	         {
	          asm STI;
            Key_Old_Handler();
	         }
           s_saver=0;
}

void far StartUpKey(void)
{
 struct Vect{signed char far *Vect;}far *Vect09=(struct Vect far *)0x0024;
 unsigned char ClearCounter;
           s_asciikey=new(signed char far [128]);
           s_scankey=new(signed char far [128]);
	         for(ClearCounter=NULL;ClearCounter<128;ClearCounter++)s_asciikey[ClearCounter]=s_scankey[ClearCounter]=NULL;
      	   (signed char far *)Key_Old_Handler=(signed char far *)Vect09->Vect;
	         Vect09->Vect=(signed char far *)Key_Handler;
           s_MD.IsKeyHandle=N_True;
}

/* End of source */