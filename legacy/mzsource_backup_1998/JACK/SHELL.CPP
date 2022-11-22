/*
  JACK's SHELL.CPP
*/

#include <dir.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <stdio.h>
#include <bios.h>
#include "common.h"
#include "graph.h"
#include "autohan.h"
#include "himem.h"
#include "menu.h"
#include "timer.h"

ShellData far *s_head;
ShellData far *s_tail;
ShellData far *s_crnt;
ShellData far *s_frst;
signed char far s_shellname[MAXPATH];

void far InitShellNode(void);
void far FreeShellNode(void);
void far DrawShellNode(WindowData far *n_wd);
void far ShellScan(signed char far *n_string);
void far ShellSort(void);
signed int far ShellFile(WindowData far *n_wd);
void far ShellBar(WindowData far *n_wd, unsigned char n_xorcolor =DARKGRAY);

void far Fsplit(signed char far *n_src, signed char far *n_dest1, signed char *n_dest2)
{
 signed int n_ptr;
 signed int n_count;
           for(n_count=strlen(n_src)-1;n_count>=0;n_count--)
           {
            if(*(n_src+n_count)=='\\')
            {
             n_ptr=n_count;
             strcpy(n_dest2, n_src+n_count+1);
             break;
            }
           }
           for(n_count=0;n_count<=n_ptr;n_count++)
           {
            *(n_dest1+n_count)=*(n_src+n_count);
            *(n_dest1+n_count+1)=NULL;
           }
}

signed char far * far Shell(signed char far *n_default, signed char far *n_title)
{
 WindowData n_wd;
 signed int n_key;
 signed char n_command;
 signed char far *n_shelldir;
 signed char far *n_shellname;
 unsigned int n_shellswap=himemAlloc((unsigned int)((ComputeWindow(100, 100, 540, 380)>>10)+30L));
           n_shelldir=new(signed char far [MAXPATH]);
           n_shellname=new(signed char far [55]);
           InitShellNode();
           DeCompressData("jack07.jim");
           n_wd.x1=100; n_wd.y1=100; n_wd.x2=540; n_wd.y2=380; n_wd.swaphandle=n_shellswap; n_wd.command=WIN_NONE;
           Help("Ìa·©·i ¬åÈ‚Ðº­A¶a."); MC(N_False);
           GetImageXMS(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, n_shellswap);
           PutImageDisk(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, "jackswap\\jack07.jim", 0);
           UnLink(MakePath("jackswap\\jack07.jim"));
           Puts(142, 105, n_title, WHITE, N_Default);
           NowGetMode(); MC(N_True); n_command=0x01; n_key=N_False;
           s_autogets=N_AUTOTAB|N_AUTOMOUSE|N_AUTOINIT;
           getcwd(n_shelldir, MAXPATH);
           strcpy(n_shellname, n_default);
           if(strlen(n_shelldir)<(50-strlen(n_shellname)-1))strcpy(s_shellname, n_shelldir);
           else strcpy(s_shellname, "");
           if(*(s_shellname+strlen(s_shellname)-1)!='\\')strcat(s_shellname, "\\");
           strcat(s_shellname, n_shellname);
           *(s_shellname+52)=NULL;
           s_ptr=strlen(s_shellname);
           ShellScan(n_shellname);
           DrawShellNode(&n_wd);
           ShellBar(&n_wd, WHITE);
           shell_loop:;
           if(kbhit())
           {
            n_key=getch();
            if(n_key==0||kbhit()){n_key=getch();if(n_key==0x0f)n_key=0x09;}
           }
           switch(n_command)
           {
            case 0x01:
                 getcwd(n_shelldir, MAXPATH);
                 if(strlen(n_shelldir)<(50-strlen(n_shellname)-1))
                 {
                  strcpy(s_shellname, n_shelldir);
                  if(*(s_shellname+strlen(s_shellname)-1)!='\\')strcat(s_shellname, "\\");
                 }
                 else strcpy(s_shellname, "");
                 strcat(s_shellname, n_shellname);
                 *(s_shellname+52)=NULL;
                 s_ptr=strlen(s_shellname);
                 n_key=Gets(n_wd.x1+11, n_wd.y1+32, s_shellname, s_shellname, 51, BLACK, CYAN);
                 Fsplit(s_shellname, n_shelldir, n_shellname);
                 FreeShellNode(); InitShellNode(); ShellScan(n_shellname);
                 DrawShellNode(&n_wd); ShellBar(&n_wd, WHITE);
                 break;
            case 0x02: n_key=ShellFile(&n_wd); break;
            default: n_key=0x1b; n_command=0; break;
           }
           re_comp_key:;
           switch(n_key)
           {
            case 0x0f:
            case 0x09:
                 if(s_scankey[0x2a]||s_scankey[0x36])
                 {
                  n_command--;
                  if(n_command<1)n_command=2;
                 }
                 else
                 {
                  n_command++;
                  if(n_command>2)n_command=1;
                 }
                 break;
            case 0x0d:
                 if(n_command==2)
                 {
                  enter_shell_loop:;
                  if(s_crnt->attrib=='D')
                  {
                   unsigned int n_max;
                   _dos_setdrive(s_crnt->name[2]-'A'+1, &n_max);
                  }
                  else if((s_crnt->attrib&FA_DIREC)==FA_DIREC)
                  {
                   if(s_crnt->name[1]==0x1b)chdir("..");
                   else chdir(s_crnt->name);
                  }
                  else
                  {
                   strcpy(s_shellname, n_shelldir);
                   if(*(s_shellname+strlen(s_shellname)-1)!='\\')strcat(s_shellname, "\\");
                   strcat(s_shellname, s_crnt->name);
                   *(s_shellname+52)=NULL;
                   goto end_shell_loop;
                  }
                  FreeShellNode();
                  InitShellNode();
                  ShellScan(n_shellname);
                  DrawShellNode(&n_wd);
                  ShellBar(&n_wd, WHITE);
                  getcwd(n_shelldir, MAXPATH);
                  if(strlen(n_shelldir)<(50-strlen(n_shellname)-1))strcpy(s_shellname, n_shelldir);
                  else strcpy(s_shellname, "");
                  if(*(s_shellname+strlen(s_shellname)-1)!='\\')strcat(s_shellname, "\\");
                  strcat(s_shellname, n_shellname);
                  *(s_shellname+52)=NULL;
                  if(s_ptr>strlen(s_shellname))s_ptr=strlen(s_shellname);
                  MC(N_False);
                  Puts(n_wd.x1+11, n_wd.y1+32, s_shellname, BLACK, CYAN);
                  drwfillbox(SET, CYAN, n_wd.x1+11+(strlen(s_shellname)<<3), n_wd.y1+32, n_wd.x1+419, n_wd.y1+47);
                  MC(N_True);
                 }
                 else
                 {
                  n_command++;
                  if(n_command>2)n_command=1;
                 }
                 break;
            case 0x00: // Mouse
                 {
                  signed char n_moveflag=N_False;
                  WindowProcess(&n_wd);
                  if(n_wd.command==WIN_CLOSE)
                  {
                   n_key=0x1b;
                   goto re_comp_key;
                  }
                  if(MouseCheck(n_wd.x1+10, n_wd.y1+30, n_wd.x1+340, n_wd.y1+50, N_LEFTBUTTON)&&(n_command!=0x01))
                  {
                   n_command=0x01;
                   n_moveflag=N_True;
                  }
                  if((MouseCheck(n_wd.x1+10, n_wd.y1+60, n_wd.x1+340, n_wd.y1+270, N_LEFTBUTTON)||MouseCheck(n_wd.x1+351, n_wd.x1+162, n_wd.x1+369, n_wd.y1+269, N_LEFTBUTTON))&&(n_command!=0x02))
                  {
                   n_command=0x02;
                   n_moveflag=N_True;
                  }
                  if(s_mb==N_RIGHTBUTTON)
                  {
                   signed char far *n_shell_right[]={"¬åÈ‚\(Enter\)", "Âá­¡\(ESC\)", NULL};
                   switch(RightButtonMenu(n_shell_right))
                   {
                    case 0x00: goto end_shell_loop;
                    case 0x01: //n_key=0x1b; goto re_comp_key;
                    case 0x02: break;
                    default: break;
                   }
                  }
                  if(n_moveflag==N_False)while(s_mb){MouseProcess();TimeProcess();}
                 }
                 break;
            case 0x1b:
                 strcpy(s_shellname, "");
                 goto end_shell_loop;
            default:
                 break;
           }
           if(n_command==0x00)goto end_shell_loop;
           while(kbhit())getch();
           goto shell_loop;
           end_shell_loop:;
           delete(n_shelldir);
           delete(n_shellname);
           FreeShellNode();
           s_autogets=N_False;
           MC(N_False);
           PutImageXMS(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, n_shellswap);
           MC(N_True);
           himemFree(n_shellswap);
           while(kbhit())getch();
           return(s_shellname);
}

void far ShellBar(WindowData far *n_wd, unsigned char n_xorcolor)
{
 ShellData far *n_sd;
 ShellData far *n_frst;
 signed int n_x, n_y;
           n_x=n_wd->x1+20; n_y=n_wd->y1+65;
           n_sd=s_head;
           n_frst=s_head;
           do
           {
            if(n_sd==s_crnt)break;
            n_sd=n_sd->next;
            n_y+=20;
            if(n_y>(n_wd->y1+260))
            {
             n_x+=165;
             n_y=n_wd->y1+65;
             if(n_x>(n_wd->x1+300))
             {
              n_x=n_wd->x1+20;
              n_y=n_wd->y1+65;
              n_frst=n_sd;
             }
            }
           }while(n_sd);
           if(s_frst!=n_frst)
           {
            s_frst=n_frst;
            DrawShellNode(n_wd);
           }
           MC(N_False);
           drwfillbox(XOR, n_xorcolor, n_x-5, n_y, n_x+150, n_y+16);
           MC(N_True);
}

void far ShellLeft(signed int n_grid)
{
 signed int n_count=NULL;
 ShellData far *n_temp=s_crnt;
           while(n_temp!=s_head)
           {
            if(n_count>=n_grid)break;
            n_count++;
            n_temp=n_temp->prev;
           }
           s_crnt=n_temp;
}

void far ShellRight(signed int n_grid)
{
 signed int n_count=NULL;
 ShellData far *n_temp=s_crnt;
           while(n_temp!=s_tail)
           {
            if(n_count>=n_grid)break;
            n_count++;
            n_temp=n_temp->next;
           }
           s_crnt=n_temp;
}

void far TraceShell(WindowData far *n_wd, signed int n_xd, signed int n_yd)
{
 ShellData far *n_sd;
 signed int n_x, n_y;
           n_x=n_wd->x1+20; n_y=n_wd->y1+65;
           n_sd=s_frst;
           do
           {
            if((n_x+165)>(n_xd+10)&&(n_y+20)>n_yd&&n_y<=n_yd)break;
            n_sd=n_sd->next;
            n_y+=20;
            if(n_y>(n_wd->y1+260))
            {
             n_x+=165;
             n_y=n_wd->y1+65;
             if(n_x>(n_wd->x1+300))
             {
              n_x=n_wd->x1+20;
              n_y=n_wd->y1+65;
             }
            }
           }while(n_sd!=s_tail);
           if(n_sd!=s_crnt)
           {
            ShellBar(n_wd);
            s_crnt=n_sd;
            ShellBar(n_wd);
           }
}

signed int far ShellFile(WindowData far *n_wd)
{
 signed int n_key;
           ShellBar(n_wd, WHITE);
           ShellBar(n_wd);
           shell_file_loop:;
           n_key=NULL;
           MouseProcess();
           TimeProcess();
           if(s_mb==N_LEFTBUTTON)
           {
            if(MouseCheck(n_wd->x1+15, n_wd->y1+65, n_wd->x1+335, n_wd->y1+264, N_LEFTBUTTON)==N_True)
            {
             TraceShell(n_wd, s_mx, s_my);
             while(s_mb)
             {
              MouseProcess();
              TimeProcess();
              if(MouseCheck(n_wd->x1+15, n_wd->y1+65, n_wd->x1+335, n_wd->y1+264, N_LEFTBUTTON)==N_True)TraceShell(n_wd, s_mx, s_my);
             }
            }
            else if(MouseCheck(n_wd->x1+351, n_wd->y1+162, n_wd->x1+369, n_wd->y1+215, N_LEFTBUTTON)==N_True)
            {
             signed int n_timer=4;
             MC(N_False);
             PushRec(n_wd->x1+351, n_wd->y1+162, n_wd->x1+369, n_wd->y1+215);
             MC(N_True);
             StartTimer(0, ClockTimer);
             while(s_mb)
             {
              MouseProcess();
              TimeProcess();
              if(s_crnt!=s_head&&s_clock==N_True)
              {
               if(n_timer>0)
               {
                CloseTimer();
                StartTimer(n_timer, ClockTimer);
                n_timer--;
               }
               ShellBar(n_wd);
               s_crnt=s_crnt->prev;
               ShellBar(n_wd);
               s_clock=N_False;
              }
             }
             CloseTimer();
             MC(N_False);
             PopRec(n_wd->x1+351, n_wd->y1+162, n_wd->x1+369, n_wd->y1+215);
             MC(N_True);
            }
            else if(MouseCheck(n_wd->x1+351, n_wd->y1+217, n_wd->x1+369, n_wd->y1+269, N_LEFTBUTTON)==N_True)
            {
             signed int n_timer=4;
             StartTimer(0, ClockTimer);
             MC(N_False);
             PushRec(n_wd->x1+351, n_wd->y1+217, n_wd->x1+369, n_wd->y1+269);
             MC(N_True);
             while(s_mb)
             {
              MouseProcess();
              TimeProcess();
              if(s_crnt!=s_tail&&s_clock==N_True)
              {
               if(n_timer>0)
               {
                CloseTimer();
                StartTimer(n_timer, ClockTimer);
                n_timer--;
               }
               ShellBar(n_wd);
               s_crnt=s_crnt->next;
               ShellBar(n_wd);
               s_clock=N_False;
              }
             }
             CloseTimer();
             MC(N_False);
             PopRec(n_wd->x1+351, n_wd->y1+217, n_wd->x1+369, n_wd->y1+269);
             MC(N_True);
            }
            else goto end_file_loop;
           }
           if(s_mb==N_RIGHTBUTTON)goto end_file_loop;
           if(kbhit())
           {
            n_key=getch();
            if(n_key==NULL||kbhit())
            {
             n_key=getch();
             switch(n_key)
             {
              case 0x47:
                   if(s_crnt!=s_head)
                   {
                    ShellBar(n_wd);
                    s_crnt=s_head;
                    ShellBar(n_wd);
                   }
                   break;
              case 0x4f:
                   if(s_crnt!=s_tail)
                   {
                    ShellBar(n_wd);
                    s_crnt=s_tail;
                    ShellBar(n_wd);
                   }
                   break;
              case 0x49:
                   if(s_crnt!=s_head)
                   {
                    ShellBar(n_wd);
                    if(s_crnt==s_frst)ShellLeft(20);
                    else s_crnt=s_frst;
                    ShellBar(n_wd);
                   }
                   break;
              case 0x51:
                   if(s_crnt!=s_tail)
                   {
                    ShellData far *n_tempnode=s_crnt;
                    ShellBar(n_wd);
                    s_crnt=s_frst;
                    ShellRight(19);
                    if(s_crnt==n_tempnode)ShellRight(20);
                    ShellBar(n_wd);
                   }
                   break;
              case 0x4b: ShellBar(n_wd); ShellLeft(10); ShellBar(n_wd); break;
              case 0x4d: ShellBar(n_wd); ShellRight(10); ShellBar(n_wd); break;
              case 0x48:
                   if(s_crnt!=s_head)
                   {
                    ShellBar(n_wd);
                    s_crnt=s_crnt->prev;
                    ShellBar(n_wd);
                   }
                   break;
              case 0x50:
                   if(s_crnt!=s_tail)
                   {
                    ShellBar(n_wd);
                    s_crnt=s_crnt->next;
                    ShellBar(n_wd);
                   }
                   break;
              default: break;
             }
             if(n_key==0x0f)n_key=0x09;
             else n_key=NULL;
            }
            switch(n_key)
            {
             case 0x09:
             case 0x1b:
             case 0x0d: goto end_file_loop;
             default: break;
            }
            while(kbhit())getch();
           }
           goto shell_file_loop;
           end_file_loop:;
           ShellBar(n_wd);
           ShellBar(n_wd, WHITE);
           return(n_key);
}

void far DrawShellNode(WindowData far *n_wd)
{
 signed int n_x, n_y;
 signed char Drive[MAXDRIVE], Directory[MAXPATH], FileName[MAXFILE], ExtName[MAXEXT];
 ShellData far *n_temp=s_frst;
           s_ksmode=N_True;
           MC(N_False);
           drwfillbox(SET, DARKGRAY, n_wd->x1+11, n_wd->y1+61, n_wd->x1+339, n_wd->y1+269);
           drwline(SET, WHITE, n_wd->x1+175, n_wd->y1+61, n_wd->x1+175, n_wd->y1+269);
           drwline(SET, BLACK, n_wd->x1+176, n_wd->y1+61, n_wd->x1+176, n_wd->y1+269);
           n_x=n_wd->x1+20; n_y=n_wd->y1+65;
           while(n_temp)
           {
            ExtName[0]=ExtName[1]=NULL;
            fnsplit(n_temp->name, Drive, Directory, FileName, ExtName);
            if((n_temp->attrib&FA_DIREC)==FA_DIREC)Printf(n_x, n_y, LIGHTRED, DARKGRAY, "%-8s %-3s [DIR]", FileName, ExtName+1);
            else if(n_temp->attrib=='D')Printf(n_x, n_y, LIGHTCYAN, DARKGRAY, "%-8s %-3s [DRV]", FileName, ExtName+1);
            else
            {
             if(n_temp->size>99999L)
             {
              if((n_temp->size>>10)>99999L)
              {
               if((n_temp->size>>10>>10)>99999L)Printf(n_x, n_y, WHITE, DARKGRAY, "%-8s %-3s  Very", FileName, ExtName+1);
               else Printf(n_x, n_y, WHITE, DARKGRAY, "%-8s %-3s %4luM", FileName, ExtName+1, n_temp->size>>20);
              }
              else Printf(n_x, n_y, WHITE, DARKGRAY, "%-8s %-3s %4luK", FileName, ExtName+1, n_temp->size>>10);
             }
             else Printf(n_x, n_y, WHITE, DARKGRAY, "%-8s %-3s %5lu", FileName, ExtName+1, n_temp->size);
            }
            n_y+=20;
            if(n_y>(n_wd->y1+260))
            {
             n_x+=165;
             n_y=n_wd->y1+65;
             if(n_x>(n_wd->x1+300))break;
            }
            n_temp=n_temp->next;
           }
           MC(N_True);
           s_ksmode=N_False;
}

void far InitShellNode(void)
{
           s_head=NULL;
           s_tail=NULL;
           s_crnt=NULL;
           s_frst=NULL;
}

void far FreeShellNode(void)
{
           do{
              delete(s_tail);
              s_tail=s_tail->prev;
             }while(s_tail);
}

void far ShellBuildNode(struct ffblk far *n_ffblk)
{
 ShellData far *n_temp;
           n_temp=new(ShellData far [1]);
           strcpy(n_temp->name, "            ");
           if(n_ffblk->ff_name[0]=='.')strcpy(n_temp->name, "[\x1b]");
           else strcpy(n_temp->name, n_ffblk->ff_name);
           n_temp->size=n_ffblk->ff_fsize;
           n_temp->attrib=n_ffblk->ff_attrib;
           if(s_head==NULL)
           {
            s_head=s_crnt=s_frst=n_temp;
            s_head->prev=NULL;
            s_head->next=NULL;
            s_tail=s_head;
           }
           else
           {
            s_tail->next=n_temp;
            n_temp->prev=s_tail;
            n_temp->next=NULL;
            s_tail=n_temp;
           }
}

void far ShellScan(signed char far *n_string)
{
 struct ffblk n_ffblk;
 signed int n_done;
 signed int equip;
 unsigned char diskcount;
 unsigned int drive;
 signed char far *n_tempstring;
           n_done=findfirst("*.*", &n_ffblk, FA_DIREC);
           while(!n_done)
           {
            if((n_ffblk.ff_attrib&FA_DIREC)==FA_DIREC)
            {
             if(n_ffblk.ff_name[0]=='.'&&n_ffblk.ff_name[1]!='.');
             else ShellBuildNode(&n_ffblk);
            }
            n_done=findnext(&n_ffblk);
           }
           n_tempstring=new(signed char far [strlen(n_string)]);
           while(*n_string)
           {
            n_done=0;
            while(*n_string)
            {
             if(*(n_string)=='\;'){n_string++; break;}
             *(n_tempstring+n_done++)=*(n_string++);
             *(n_tempstring+n_done)=NULL;
            }
            n_done=findfirst(n_tempstring, &n_ffblk, 0xffff);
            while(!n_done)
            {
             if((n_ffblk.ff_attrib&FA_ARCH)==FA_ARCH||(n_ffblk.ff_attrib&FA_RDONLY)==FA_RDONLY||(n_ffblk.ff_attrib&FA_HIDDEN)==FA_HIDDEN||(n_ffblk.ff_attrib&FA_SYSTEM)==FA_SYSTEM)ShellBuildNode(&n_ffblk);
             n_done=findnext(&n_ffblk);
            }
           }
           delete(n_tempstring);
           asm PUSH AX;
           asm PUSH BX;
           equip=biosequip();
           equip&=0x60;
           for(diskcount=NULL;diskcount<=(equip>>6);diskcount++)
           {
            asm MOV AH,0x44;
	          asm MOV AL,0x09;
	          asm MOV BL,diskcount;
	          asm INT 0x21;
	          drive=_DX;
            n_ffblk.ff_attrib='D';
            sprintf(n_ffblk.ff_name, "[-%c-].FDD", 0x41+diskcount);
            ShellBuildNode(&n_ffblk);
           }
           for(diskcount=3;diskcount<=23;diskcount++)
           {
            asm MOV AH,0x44;
	          asm MOV AL,0x08;
	          asm MOV BL,diskcount;
	          asm INT 0x21;
	          drive=_AX;
            if(drive!=0x0f)
            {
             drive=NULL;
             asm MOV AH,0x44;
	           asm MOV AL,0x09;
	           asm MOV BL,diskcount;
	           asm INT 0x21;
      	     drive=_DX;
             n_ffblk.ff_attrib='D';
             sprintf(n_ffblk.ff_name, "[-%c-].", 0x40+diskcount);
	           if((drive&0x1000)==0x1000)strcat(n_ffblk.ff_name, "CDR");
	           else if((drive&0x2000)==0x2000)strcat(n_ffblk.ff_name, "DR0");
	           else if((drive&0x4000)==0x4000)strcat(n_ffblk.ff_name, "SCI");
	           else if((drive&0x8000)==0x8000)strcat(n_ffblk.ff_name, "DR2");
	           else if((drive&0x0100)==0x0100)strcat(n_ffblk.ff_name, "DR3");
	           else if((drive&0x0200)==0x0200)strcat(n_ffblk.ff_name, "DR4");
	           else if((drive&0x0400)==0x0400)strcat(n_ffblk.ff_name, "DR5");
	           else if((drive&0x0800)==0x0800)strcat(n_ffblk.ff_name, "HDD");
	           else strcat(n_ffblk.ff_name, "???");
             ShellBuildNode(&n_ffblk);
            }
           }
           asm POP BX;
           asm POP AX;
           ShellSort();
}

void far ShellSwapNode(ShellData far *srcnode, ShellData far *destnode)
{
 ShellData TempNode;
           memmove(&TempNode, srcnode, 18);
           memmove(srcnode, destnode, 18);
           memmove(destnode, &TempNode, 18);
}

void far ShellSort(void)
{
 ShellData far *StepNode1;
 ShellData far *StepNode2;
 signed char Drive[MAXDRIVE], Directory[MAXPATH], FileName[MAXFILE], ExtName1[MAXEXT], ExtName2[MAXEXT];
 signed int ExtFlag;
           if(s_head==s_tail)return;
           if(s_head->name[0]=='['&&s_head->name[1]==0x1b)StepNode1=s_head->next;
           else StepNode1=s_head;
           while(StepNode1)
           {
            if((StepNode1->attrib&FA_DIREC)!=FA_DIREC)break;
            StepNode2=StepNode1->next;
            while(StepNode2)
            {
             if((StepNode2->attrib&FA_DIREC)!=FA_DIREC)break;
             strcpy(ExtName1, "    "); strcpy(ExtName2, "    ");
             fnsplit(StepNode1->name, Drive, Directory, FileName, ExtName1);
             fnsplit(StepNode2->name, Drive, Directory, FileName, ExtName2);
             ExtFlag=memcmp(ExtName1+1, ExtName2+1, 3);
             if(ExtFlag>NULL)ShellSwapNode(StepNode1, StepNode2);
             else
             {
              if(ExtFlag==NULL&&memcmp(StepNode1->name, StepNode2->name, 8)>NULL)ShellSwapNode(StepNode1, StepNode2);
             }
             StepNode2=StepNode2->next;
            }
            StepNode1=StepNode1->next;
           }
           while(StepNode1)
           {
            if(StepNode1->attrib=='D')break;
            StepNode2=StepNode1->next;
            while(StepNode2)
            {
             if(StepNode2->attrib=='D')break;
             strcpy(ExtName1, "    "); strcpy(ExtName2, "    ");
             fnsplit(StepNode1->name, Drive, Directory, FileName, ExtName1);
             fnsplit(StepNode2->name, Drive, Directory, FileName, ExtName2);
             ExtFlag=memcmp(ExtName1+1, ExtName2+1, 3);
             if(ExtFlag>NULL)ShellSwapNode(StepNode1, StepNode2);
             else
             {
              if(ExtFlag==NULL&&memcmp(StepNode1->name, StepNode2->name, 8)>NULL)ShellSwapNode(StepNode1, StepNode2);
             }
             StepNode2=StepNode2->next;
            }
            StepNode1=StepNode1->next;
           }
}

/* End of source */