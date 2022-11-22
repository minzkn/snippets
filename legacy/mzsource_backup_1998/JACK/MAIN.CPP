/*
  JACK's MAIN.CPP
*/

//#pragma inline

#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "signal.h"
#include "init.h"
#include "graph.h"
#include "himem.h"
#include "menu.h"
#include "jf.h"

MainData s_MD;
signed char far *s_WorkDirectory;
signed char far *s_MakeDirectory;
signed int s_MaxX, s_MaxY;
signed char far *s_ReadMode="rb";
signed char far *s_WriteMode="wb";
signed char far *s_Title="JACK & JACK";
signed char far *s_TitleString="JACK & JACK v1.00b";
signed int s_mx=0, s_my=0, s_mb=0;
signed int far s_Cursor1[16]={0xc000,0xa000,0x9000,0x8800,0x8400,0x8200,0x8100,0x8080,
                              0x8040,0x8020,0x83e0,0xb900,0xe980,0x0c80,0x0480,0x0780};
signed int far s_Cursor2[16]={0x0000,0x4000,0x6000,0x7000,0x7800,0x7c00,0x7e00,0x7f00,
                              0x7f80,0x7fc0,0x7c00,0x4600,0x0600,0x0300,0x0300,0x0000};
signed int far s_Cursor3[16]={0x0c00,0x1200,0x1200,0x1200,0x1200,0x13b6,0x1249,0x7249,
                              0x9249,0x9001,0x9001,0x8001,0x4002,0x4002,0x2004,0x1ff8};
signed int far s_Cursor4[16]={0x0000,0x0c00,0x0c00,0x0c00,0x0c00,0x0c00,0x0db6,0x0db6,
                              0x6db6,0x6ffe,0x6ffe,0x7ffe,0x3ffc,0x3ffc,0x1ff8,0x0000};
signed int far s_Cursor5[16]={0x0000,0x0012,0x001a,0x0016,0x0012,0x0000,0x0000,0x0000,
                              0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};
signed int s_key;
signed char far *s_scankey;
signed char far *s_asciikey;
signed char far *s_swapdir="JACKSWAP";
signed char far *s_datadir="JACKDATA";
signed char far *s_dicdir="DICTION";
signed char far *s_compressstring="CompressJACK v1.00";
WindowData s_wd;
signed char s_clock;
signed int s_saver=0;

void far MC(signed char n_flag);
void far MouseProcess(void);
void far TimeProcess(void);
signed int far MouseCheck(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2, signed int n_mb);
signed int far WindowProcess(WindowData far *n_wd);
void far UnLink(signed char far *n_name);
signed long far ComputeWindow(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2);
void far Help(signed char far *n_string);
signed int far Compress(signed char far *source, signed char far *target, signed char far *n_header);
signed int far DeCompress(signed char far *source, signed char far *target, signed char far *n_header);
void far SaveConfig(MainConfig far *n_mc);
void far LoadConfig(MainConfig far *n_mc);
void far ClockTimer(void);

void main(void){DetectOption(); Start(); PopMain(); Close();}

void far ClockTimer(void)
{
           s_clock=N_True;
}

signed long far ComputeWindow(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2)
{
 signed long n_return;
           n_return=(signed long)((n_x2-n_x1)+1)*(signed long)((n_y2-n_y1)+1);
           return(n_return);
}

void far UnLink(signed char far *n_name){_dos_setfileattr(n_name, _A_ARCH); unlink(n_name);}

void far MemoryOverFlow(void)
{
 union REGS Register;
           Register.x.ax=0x0003; int86(0x10, &Register, &Register);
           printf("Cannot allocate! - Out of memory\n"); CloseUpKey();
           Register.h.ah=0x4c; int86(0x21, &Register, &Register);
}

signed char far * far MakePath(signed char far *n_string)
{
           delete(s_MakeDirectory);
           s_MakeDirectory=new(signed char far [strlen(s_WorkDirectory)+strlen(n_string)+1]);
           strcpy(s_MakeDirectory, s_WorkDirectory); strcat(s_MakeDirectory, n_string);
           return(s_MakeDirectory);
}

void far MC(signed char n_sw)
{
 static unsigned char far mcbuffer[512];
 signed int n_cx;
 signed int n_count;
 signed int n_bitcount;
 signed int n_cy, n_cc;
           if(s_MD.IsMouse==N_False){return;}
           if(n_sw)
           {
            n_cc=0;
            for(n_cy=s_my;n_cy<s_my+16;n_cy++)
            {
             for(n_cx=s_mx-4;n_cx<s_mx+16;n_cx++){mcbuffer[n_cc++]=(unsigned char)getpoint(n_cx, n_cy);}
            }
            switch(s_mb)
            {
             case 0x00:
                  for(n_count=0;n_count<16;n_count++)
                  {
                   for(n_bitcount=0;n_bitcount<16;n_bitcount++)
                   {
                    if((unsigned int)(*(s_Cursor1+n_count))&(unsigned int)(1<<n_bitcount)){drwpoint(SET, BLACK, s_mx+(15-n_bitcount), s_my+n_count);}
                    if((unsigned int)(*(s_Cursor2+n_count))&(unsigned int)(1<<n_bitcount)){drwpoint(SET, WHITE, s_mx+(15-n_bitcount), s_my+n_count);}
                   }
                  }
                  for(n_count=0;n_count<16;n_count++)
                  {
                   for(n_bitcount=0;n_bitcount<16;n_bitcount++)
                   {
                    if((unsigned int)(*(s_Cursor5+n_count))&(unsigned int)(1<<n_bitcount)){drwpoint(SET, BLACK, (s_mx-9)+(15-n_bitcount), (s_my+5)+n_count);}
                   }
                  }
                  break;
             default:
                  for(n_count=0;n_count<16;n_count++)
                  {
                   for(n_bitcount=0;n_bitcount<16;n_bitcount++)
                   {
                    if((unsigned int)(*(s_Cursor3+n_count))&(unsigned int)(1<<n_bitcount)){drwpoint(SET, DARKGRAY, (s_mx-4)+(15-n_bitcount), s_my+n_count);}
                    if((unsigned int)(*(s_Cursor4+n_count))&(unsigned int)(1<<n_bitcount)){drwpoint(SET, WHITE, (s_mx-4)+(15-n_bitcount), s_my+n_count);}
                   }
                  }
                  for(n_count=0;n_count<16;n_count++)
                  {
                   for(n_bitcount=0;n_bitcount<16;n_bitcount++)
                   {
                    if((unsigned int)(*(s_Cursor5+n_count))&(unsigned int)(1<<n_bitcount)){drwpoint(SET, BLACK, (s_mx-9)+(15-n_bitcount), (s_my+9)+n_count);}
                   }
                  }
                  break;
            }
           }
           else
           {
            for(n_cc=0, n_cy=s_my;n_cy<s_my+16;n_cy++)
            {
             for(n_cx=s_mx-4;n_cx<s_mx+16;n_cx++){drwpoint(SET, (signed int)mcbuffer[n_cc++], n_cx, n_cy);}
            }
           }
}

void far MouseProcess(void)
{
 union REGS Register;
 signed char n_flag=N_False;
           if(s_MD.IsMouse==N_False){return;}
           Register.x.ax=0x0003; int86(0x33, &Register, &Register);
           if(s_mb!=Register.x.bx)
           {
            s_popmouseflag=N_True; MC(N_False);
            s_mb=(signed int)Register.x.bx; n_flag=N_True;
            s_saver=0;
           }
           Register.x.ax=0x000b; int86(0x33, &Register, &Register);
           if(Register.x.cx!=0||Register.x.dx!=0)
           {
            s_popmouseflag=N_True;
            if(n_flag==N_False){MC(N_False);}
            s_mx+=(signed int)Register.x.cx; s_my+=(signed int)Register.x.dx;
            if(s_mx<0){s_mx=0;}
            if(s_my<0){s_my=0;}
            if(s_mx>=s_MaxX){s_mx=s_MaxX-1;}
            if(s_my>=s_MaxY){s_my=s_MaxY-1;}
            if(n_flag==N_False){MC(N_True);}
            s_saver=0;
           }
           if(n_flag==N_True){MC(N_True);}
}

void far TimeProcess(void)
{
 static signed char n_stime=100;
 signed char n_timestring[20];
 struct time n_time;
 signed char n_hour;
           gettime(&n_time);
           if(n_stime==n_time.ti_sec){return;}
           s_saver++;
           if(s_saver>120){Saver(N_Default);s_saver=0;}
           n_stime=n_time.ti_sec;
           if(n_time.ti_hour==0)n_hour=12;
           else if(n_time.ti_hour>12)n_hour=n_time.ti_hour-12;
           else n_hour=n_time.ti_hour;
           sprintf(n_timestring, "%02d:%02d:%02d%cm", n_hour, n_time.ti_min, n_time.ti_sec, ((n_time.ti_hour<12)?'a':'p'));
           if(s_mx>534&&s_my>s_MaxY-37&&s_mx<639)MC(N_False);
           Puts(555, s_MaxY-19, n_timestring, BLACK, 27);
           if(s_mx>534&&s_my>s_MaxY-37&&s_mx<639)MC(N_True);
}

signed int far MouseCheck(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2, signed int n_mb)
{
           if(s_MD.IsMouse==N_False){return(N_False);}
           if(n_mb!=s_mb&&n_mb!=N_Default){return(N_False);}
           if(s_mx>=n_x1&&s_mx<=n_x2&&s_my>=n_y1&&s_my<=n_y2){return(N_True);}
           return(N_False);
}

signed int far WindowProcess(WindowData far *n_wd)
{
 signed int n_x1, n_y1, n_x2, n_y2;
 signed char far *n_movestring=s_Title;
           n_x1=n_wd->x1; n_x2=n_wd->x2; n_y1=n_wd->y1; n_y2=n_wd->y2;
           n_wd->command=WIN_NONE;
           if(MouseCheck(n_wd->x2-20, n_wd->y1+6, n_wd->x2-7, n_wd->y1+19, N_LEFTBUTTON)==N_True)
           {
            MC(N_False); PushRec(n_wd->x2-20, n_wd->y1+6, n_wd->x2-7, n_wd->y1+19); MC(N_True);
            while(s_mb)MouseProcess();
            MC(N_False); PopRec(n_wd->x2-20, n_wd->y1+6, n_wd->x2-7, n_wd->y1+19); MC(N_True);
            if(MouseCheck(n_wd->x2-20, n_wd->y1+6, n_wd->x2-7, n_wd->y1+19, N_NOBUTTON)==N_True){n_wd->command=WIN_CLOSE;}
           }
           else if(MouseCheck(n_wd->x1+4, n_wd->y1+3, n_wd->x2-4, n_wd->y1+22, N_LEFTBUTTON)==N_True)
           {
            unsigned int n_winhandle;
            union REGS Register;
            n_winhandle=himemAlloc((unsigned int)((ComputeWindow(n_wd->x1, n_wd->y1, n_wd->x2, n_wd->y2)>>10)+30L));
            MC(N_False);
            GetImageXMS(n_wd->x1, n_wd->y1, n_wd->x2, n_wd->y2, n_winhandle);
            drwbox(XOR, WHITE, n_wd->x1, n_wd->y1, n_wd->x2, n_wd->y2);
            drwbox(XOR, WHITE, n_wd->x1+2, n_wd->y1+2, n_wd->x2-2, n_wd->y2-2);
            drwstring(XOR, WHITE, BLACK, n_movestring, n_wd->x1+5, n_wd->y1+3);
            while(s_mb==N_LEFTBUTTON)
            {
             TimeProcess();
             Register.x.ax=0x0003; int86(0x33, &Register, &Register);
             if(s_mb!=Register.x.bx){s_mb=(signed int)Register.x.bx;}
             Register.x.ax=0x000b; int86(0x33, &Register, &Register);
             if(Register.x.cx!=0||Register.x.dx!=0)
             {
              drwbox(XOR, WHITE, n_wd->x1, n_wd->y1, n_wd->x2, n_wd->y2);
              drwbox(XOR, WHITE, n_wd->x1+2, n_wd->y1+2, n_wd->x2-2, n_wd->y2-2);
              drwstring(XOR, WHITE, BLACK, n_movestring, n_wd->x1+5, n_wd->y1+3);
              if((n_wd->x1+(signed int)Register.x.cx)<0)
              {
               signed int n_temp=n_wd->x2-n_wd->x1;
               n_wd->x1=0;
               n_wd->x2=n_wd->x1+n_temp;
              }
              else if((n_wd->x2+(signed int)Register.x.cx)>=((s_MD.IsWin==N_True)?640:s_MaxX))
              {
               signed int n_temp=n_wd->x2-n_wd->x1;
               n_wd->x2=((s_MD.IsWin==N_True)?640:s_MaxX)-1;
               n_wd->x1=n_wd->x2-n_temp;
              }
              else
              {
               s_mx+=(signed int)Register.x.cx;
               n_wd->x1+=(signed int)Register.x.cx;
               n_wd->x2+=(signed int)Register.x.cx;
              }
              if((n_wd->y1+(signed int)Register.x.dx)<0)
              {
               signed int n_temp=n_wd->y2-n_wd->y1;
               n_wd->y1=0;
               n_wd->y2=n_wd->y1+n_temp;
              }
              else if((n_wd->y2+(signed int)Register.x.dx)>=((s_MD.IsWin==N_True)?479:s_MaxY-22))
              {
               signed int n_temp=n_wd->y2-n_wd->y1;
               n_wd->y2=((s_MD.IsWin==N_True)?479:s_MaxY-22);
               n_wd->y1=n_wd->y2-n_temp;
              }
              else
              {
               s_my+=(signed int)Register.x.dx;
               n_wd->y1+=(signed int)Register.x.dx;
               n_wd->y2+=(signed int)Register.x.dx;
              }
              if(s_mx<0){s_mx=0;}
              if(s_my<0){s_my=0;}
              if(s_mx>=((s_MD.IsWin==N_True)?640:s_MaxX)){s_mx=((s_MD.IsWin==N_True)?640:s_MaxX)-1;}
              if(s_my>=((s_MD.IsWin==N_True)?480:s_MaxY)){s_my=((s_MD.IsWin==N_True)?480:s_MaxY)-1;}
              drwbox(XOR, WHITE, n_wd->x1+2, n_wd->y1+2, n_wd->x2-2, n_wd->y2-2);
              drwbox(XOR, WHITE, n_wd->x1, n_wd->y1, n_wd->x2, n_wd->y2);
              drwstring(XOR, WHITE, BLACK, n_movestring, n_wd->x1+5, n_wd->y1+3);
             }
            } // while(s_mb)
            drwbox(XOR, WHITE, n_wd->x1+2, n_wd->y1+2, n_wd->x2-2, n_wd->y2-2);
            drwbox(XOR, WHITE, n_wd->x1, n_wd->y1, n_wd->x2, n_wd->y2);
            drwstring(XOR, WHITE, BLACK, n_movestring, n_wd->x1+5, n_wd->y1+3);
            if(s_mb!=N_NOBUTTON)
            {
             n_wd->x1=n_x1; n_wd->x2=n_x2; n_wd->y1=n_y1; n_wd->y2=n_y2;
            }
            else if(n_wd->x1==n_x1&&n_wd->x2==n_x2&&n_wd->y1==n_y1&&n_wd->y2==n_y2);
            else
            {
             n_wd->command=WIN_MOVE;
             PutImageXMS(n_x1, n_y1, n_x2, n_y2, n_wd->swaphandle);
             GetImageXMS(n_wd->x1, n_wd->y1, n_wd->x2, n_wd->y2, n_wd->swaphandle);
             PutImageXMS(n_wd->x1, n_wd->y1, n_wd->x2, n_wd->y2, n_winhandle);
            }
            MC(N_True); himemFree(n_winhandle);
           }
           return(n_wd->command);
}

void far Help(signed char far *n_string)
{
           if(MouseCheck(30, 470, 600, 507, N_Default)==N_True)MC(N_False);
           Puts(45, 490, n_string, WHITE, BLUE);
           drwfillbox(SET, BLUE, 45+(strlen(n_string)<<3), 489, 600, 506);
           if(MouseCheck(30, 470, 600, 507, N_Default)==N_True)MC(N_True);
}

signed int far Compress(signed char far *source, signed char far *target, signed char far *n_header)
{
 FILE *fr, *fw;
 unsigned int n_size;
 signed char far *n_buff;
 unsigned char far *n_buff2;
 signed long n_filesize;
 unsigned int n_count;
 unsigned int n_cc;
 signed char n_flag=N_False;
           UnLink(target);
           if((fr=fopen(source, s_ReadMode))==NULL)return(N_False);
           fseek(fr, 0, SEEK_END); n_filesize=(signed long)ftell(fr); fseek(fr, 0, SEEK_SET);
           if((fw=fopen(target, s_WriteMode))==NULL){fclose(fw); return(N_False);}
           n_buff=new(signed char far [N_MAXCOMPRESS]);
           fwrite(n_header, strlen(n_header)+1, 1, fw);
           n_count=(unsigned int)((signed long)n_filesize/(signed long)N_MAXCOMPRESS);
           for(n_cc=0;n_cc<n_count;n_cc++)
           {
            fread(n_buff, (size_t)N_MAXCOMPRESS, 1, fr);
            n_buff2=compress_buf(n_buff, (unsigned int)N_MAXCOMPRESS, &n_size);
            n_size++; fwrite(&n_size, 2, 1, fw); fwrite(&n_flag, 1, 1, fw);
            fwrite(n_buff2, n_size, 1, fw); delete(n_buff2);
           }
           n_count=(unsigned int)((signed long)n_filesize%(signed long)N_MAXCOMPRESS);
           fread(n_buff, (size_t)n_count, 1, fr);
           n_buff2=compress_buf(n_buff, (unsigned int)n_count, &n_size);
           n_size++; n_flag=N_True; fwrite(&n_size, 2, 1, fw); fwrite(&n_flag, 1, 1, fw);
           fwrite(n_buff2, n_size, 1, fw);
           delete(n_buff2); fclose(fr); fclose(fw); delete(n_buff);
           return(N_True);
}

signed int far DeCompress(signed char far *source, signed char far *target, signed char far *n_header)
{
 FILE *fr, *fw;
 unsigned int n_size;
 signed char far *n_buff;
 unsigned char far *n_buff2;
 signed char n_flag;
 signed char far *n_temp;
           UnLink(target);
           if((fr=fopen(source, s_ReadMode))==NULL)return(N_False);
           if((fw=fopen(target, s_WriteMode))==NULL){fclose(fw); return(N_False);}
           n_buff=new(signed char far [N_MAXCOMPRESS<<1]);
           n_temp=new(signed char far [strlen(n_header)+1]);
           fread(n_temp, (size_t)strlen(n_header)+1, 1, fr);
           if(strcmp(n_header, n_temp)!=NULL){delete(n_temp); return(N_False);}
           delete(n_temp);
           do{
              fread(&n_size, 2, 1, fr); fread(&n_flag, 1, 1, fr);
              fread(n_buff, n_size, 1, fr); n_buff2=expand_buf(n_buff, &n_size);
              fwrite(n_buff2, n_size, 1, fw); delete(n_buff2);
             }while(n_flag==N_False);
           fclose(fr); fclose(fw); delete(n_buff);
           return(N_True);
}

void far SaveConfig(MainConfig far *n_mc)
{
 FILE *fp;
 signed char far *n_header="JACK\'s configuration R0.0.0";
           UnLink(MakePath("jack.cfg"));
           if((fp=fopen(MakePath("jack.cfg"), s_WriteMode))==0){n_mc->id=N_False; return;}
           fwrite(n_header, strlen(n_header)+1, 1, fp);
           fwrite(n_mc, sizeof(MainConfig), 1, fp);
           n_mc->id=N_True;
           fclose(fp);
}

void far LoadConfig(MainConfig far *n_mc)
{
 FILE *fp;
 signed char far *n_header="JACK\'s configuration R0.0.0";
 signed char far *n_tempbuffer;
           if((fp=fopen(MakePath("jack.cfg"), s_ReadMode))==0)goto init_mc_label;
           n_tempbuffer=new(signed char far [strlen(n_header)+1]);
           fread(n_tempbuffer, strlen(n_header)+1, 1, fp);
           if(strcmp(n_header, n_tempbuffer)!=0)
           {
            delete(n_tempbuffer);
            fclose(fp);
            init_mc_label:;
            n_mc->id=N_False;
            n_mc->background=0;
            SaveConfig(n_mc);
            return;
           }
           delete(n_tempbuffer);
           fread(n_mc, sizeof(MainConfig), 1, fp);
           n_mc->id=N_True;
           fclose(fp);
}

/* End of source */