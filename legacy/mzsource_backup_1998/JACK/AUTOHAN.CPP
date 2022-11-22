/*
  JACK's AUTOHAN.CPP
*/

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"
#include "graph.h"
#include "timer.h"

signed char s_nowgetmode=N_ENGLISH;
signed char s_nowinsertmode=N_True;
signed int s_hanprocess;
signed char s_getredraw;
unsigned int s_autogets=0;
signed char s_getscursorflag=N_True;
unsigned char s_getscolor;
signed int s_ptr=0;

void far NowGetMode(void);
signed int far IsHanCode(signed char far *n_str, signed int n_ptr, signed char n_flag =N_False);
signed int far GetsMouseProcess(signed int n_x, signed int n_y, signed char far *n_gets, signed int n_ptr, unsigned char n_color, unsigned char n_bcolor, signed int n_maxchar);
void far DeleteCharacter(signed char far *n_str, signed int n_ptr);
signed int far WriteCharacter(signed char far *n_str, signed int n_ptr, signed char n_character, signed int n_maxsize);
signed int far WriteHangul(signed char far *n_str, signed int n_ptr, union HanCode n_hancode, signed int n_maxsize);
signed int far Key2First(signed int n_key);
signed int far Key2Middle(signed int n_key);
signed int far Key2Last(signed int n_key);
signed int far First2Double(signed int n_comp, signed int n_key);
signed int far Middle2Double(signed int n_comp, signed int n_key);
signed int far Last2Double(signed int n_comp, signed int n_key);
signed int far Last2First(signed int n_key, signed int far *n_ret);
signed int far Double2Middle(signed int n_key);
void far GetsCursor(signed int n_x, signed int n_y, signed char far *n_gets, signed int n_ptr);

void far GetsCursorTimer(void){s_getscursorflag=N_True;}

signed int far Gets(signed int n_x, signed int n_y, signed char far *n_default, signed char far *n_string, signed int n_maxchar, unsigned char n_color, unsigned char n_bcolor)
{
 union HanCode n_hancode;
 signed int n_key2first, n_key2middle ,n_key2last, n_count, n_key, n_ptr;
 signed char n_tempinsertmode;
 signed char far *n_gets=new(signed char far [n_maxchar+2]);
           s_getscolor=n_bcolor^YELLOW; s_hanprocess=1;
           s_getredraw=N_False; n_hancode.WordData=0x0000;
           strcpy(n_gets, n_default); n_ptr=strlen(n_gets);
           if((s_autogets&N_AUTOMOUSE)==N_AUTOMOUSE)n_ptr=s_ptr;
           MC(N_False);
           if((s_autogets&N_AUTOINIT)==N_AUTOINIT)Puts(n_x, n_y, n_gets, BLACK, WHITE);
           else Puts(n_x, n_y, n_gets, n_color, n_bcolor);
           drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
           GetsCursor(n_x, n_y, n_gets, n_ptr); MC(N_True);
           StartTimer(15, GetsCursorTimer);
           gets_label:;
           n_key=0; MouseProcess(); TimeProcess();
           n_ptr=GetsMouseProcess(n_x, n_y, n_gets, n_ptr, n_color, n_bcolor, n_maxchar);
           if(s_getscursorflag==N_True)
           {
            MC(N_False); GetsCursor(n_x, n_y, n_gets, n_ptr);
            if(s_getscolor==(n_bcolor^YELLOW))s_getscolor=n_bcolor^DARKGRAY;
            else s_getscolor=n_bcolor^YELLOW;
            GetsCursor(n_x, n_y, n_gets, n_ptr); MC(N_True);
            s_getscursorflag=N_False;
           }
           if(MouseCheck(n_x, n_y, n_x+(n_maxchar<<3)-1, n_y+15, N_LEFTBUTTON)==N_True)
           {
            signed int n_tempptr;
            while(s_mb==N_LEFTBUTTON)
            {
             MouseProcess(); n_tempptr=(s_mx-n_x)>>3;
             if(n_tempptr>strlen(n_gets))n_tempptr=strlen(n_gets);
             if(IsHanCode(n_gets, n_tempptr, N_True)==N_True&&IsHanCode(n_gets, n_tempptr)==N_False){if(n_tempptr>0)n_tempptr--;}
             if(n_tempptr!=n_ptr)
             {
              MC(N_False); GetsCursor(n_x, n_y, n_gets, n_ptr);
              n_ptr=n_tempptr; s_hanprocess=1;
              if((s_autogets&N_AUTOINIT)==N_AUTOINIT)
              {
               drwfillbox(SET, n_bcolor, n_x, n_y, n_x+(n_maxchar<<3), n_y+15);
               Puts(n_x, n_y, n_gets, n_color, N_Default); s_autogets&=0xffef;
              }
              GetsCursor(n_x, n_y, n_gets, n_ptr); MC(N_True);
             }
            }
           }
           else if((s_autogets&N_AUTOMOUSE)==N_AUTOMOUSE&&s_mb) // ==N_LEFTBUTTON
           {
            MC(N_False); GetsCursor(n_x, n_y, n_gets, n_ptr); MC(N_True);
            if((s_autogets&N_AUTOINIT)==N_AUTOINIT)
            {
             s_autogets&=0xffef; MC(N_False);
             drwfillbox(SET, n_bcolor, n_x, n_y, n_x+(n_maxchar<<3), n_y+15);
             Puts(n_x, n_y, n_gets, n_color, N_Default);
             MC(N_True);
            }
            goto end_gets_label;
           }
           if(s_scankey[0x15]&&s_scankey[0x1d])
           {
            s_scankey[0x15]=N_False; MC(N_False); GetsCursor(n_x, n_y, n_gets, n_ptr);
            n_ptr=0; *(n_gets)=0;
            drwfillbox(SET, n_bcolor, n_x, n_y, n_x+(n_maxchar<<3), n_y+15);
            GetsCursor(n_x, n_y, n_gets, n_ptr); MC(N_True);
           }
           if(kbhit())
           {
            n_key=getch();
            MC(N_False); GetsCursor(n_x, n_y, n_gets, n_ptr); MC(N_True);
            if((s_autogets&N_AUTOINIT)==N_AUTOINIT)
            {
             MC(N_False);
             drwfillbox(SET, n_bcolor, n_x, n_y, n_x+(n_maxchar<<3), n_y+15);
             Puts(n_x, n_y, n_gets, n_color, N_Default); MC(N_True);
            }
            if(n_key==0||kbhit())
            {
             n_key=getch();
             switch(n_key)
             {
              case 0x4b:
                   if(n_ptr>0)
                   {
                    n_ptr--;
                    if(IsHanCode(n_gets, n_ptr, N_True)==N_True&&n_ptr>0)n_ptr--;
                   }
                   s_hanprocess=1;
                   break;
              case 0x4d:
                   if(n_ptr<strlen(n_gets))
                   {
                    if(IsHanCode(n_gets, n_ptr)==N_True)n_ptr++;
                    if(n_ptr<strlen(n_gets))n_ptr++;
                   }
                   s_hanprocess=1;
                   break;
              case 0x47: n_ptr=0; s_hanprocess=1; break;
              case 0x4f: n_ptr=strlen(n_gets); s_hanprocess=1; break;
              case 0x52:
                   if(s_nowinsertmode==N_True)s_nowinsertmode=N_False;
                   else s_nowinsertmode=N_True;
                   if(s_hanprocess!=1)
                   {
                    if(IsHanCode(n_gets, n_ptr)==N_True)n_ptr++;
                    if(n_ptr<strlen(n_gets))n_ptr++;
                    s_hanprocess=1;
                   }
                   MC(N_False); NowGetMode(); MC(N_True);
                   break;
              case 0x53:
                   DeleteCharacter(n_gets, n_ptr); s_hanprocess=1;
                   MC(N_False);
                   Puts(n_x+(n_ptr<<3), n_y, n_gets+n_ptr, n_color, n_bcolor);
                   drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
                   MC(N_True);
                   break;
              case 0x48:
              case 0x50:
                   if((s_autogets&N_AUTOARROW)==N_AUTOARROW)goto end_gets_label;
                   break;
              case 0x49:
              case 0x51:
                   if((s_autogets&N_AUTOPAGE)==N_AUTOPAGE)goto end_gets_label;
                   break;
              default: break;
             }
             if(n_key==0x0f)n_key=0x09;
             else n_key=0;
            }
            switch(n_key)
            {
             case 0x1b: goto end_gets_label;
             case 0x0d: goto end_gets_label;
             case 0x09:
                  if((s_autogets&N_AUTOTAB)==N_AUTOTAB)goto end_gets_label;
                  for(n_count=0;n_count<8;n_count++)
                  {
                   if(WriteCharacter(n_gets, n_ptr, 0x20, n_maxchar)==N_True)n_ptr++;
                  }
                  MC(N_False); Puts(n_x, n_y, n_gets, n_color, n_bcolor);
                  drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
                  MC(N_True); s_hanprocess=1;
                  break;
             case 0x08:
                  if(s_hanprocess>1&&((n_ptr<=(n_maxchar-2)&&s_nowinsertmode==N_False)||(/*strlen(n_gets)<=(n_maxchar-2)&&*/s_nowinsertmode==N_True)))
                  {
                   n_tempinsertmode=s_nowinsertmode; s_nowinsertmode=N_False;
                   if(s_hanprocess==4)
                   {
                    signed int n_first;
                    Last2First(n_hancode.Code.Last, &n_first);
                    n_hancode.Code.Last=n_first;
                    WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar);
                    if(n_first==1)s_hanprocess=3;
                   }
                   else if(s_hanprocess==3)
                   {
                    if(Double2Middle(n_hancode.Code.Middle)!=N_False)
                    {
                     n_hancode.Code.Middle=Double2Middle(n_hancode.Code.Middle);
                     WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar);
                    }
                    else
                    {
                     n_hancode.Code.Middle=2;
                     WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar);
                     s_hanprocess=2;
                     if(n_hancode.Code.First==1)goto delete_last_label;
                    }
                   }
                   else
                   {
                    delete_last_label:;
                    n_hancode.WordData=0x0000; DeleteCharacter(n_gets, n_ptr);
                    s_hanprocess=1;
                   }
                   s_nowinsertmode=n_tempinsertmode; MC(N_False);
                   if(s_hanprocess==1)
                   {
                    Puts(n_x+(n_ptr<<3), n_y, n_gets+n_ptr, n_color, n_bcolor);
                    drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
                   }
                   else HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                   MC(N_True);
                  }
                  else if(n_ptr>0)
                  {
                   n_ptr--;
                   if(IsHanCode(n_gets, n_ptr, N_True)==N_True&&n_ptr>0)n_ptr--;
                   DeleteCharacter(n_gets, n_ptr); MC(N_False);
                   Puts(n_x+(n_ptr<<3), n_y, n_gets+n_ptr, n_color, n_bcolor);
                   drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
                   MC(N_True); s_hanprocess=1;
                  }
                  else s_hanprocess=1;
                  break;
             default:
                  if(n_key==0x20&&(s_scankey[0x2a]||s_scankey[0x36]))
                  {
                   if(s_nowgetmode==N_True)s_nowgetmode=N_False;
                   else s_nowgetmode=N_True;
                   if(s_hanprocess!=1)
                   {
                    if(IsHanCode(n_gets, n_ptr)==N_True)n_ptr++;
                    if(n_ptr<strlen(n_gets))n_ptr++;
                    s_hanprocess=1;
                   }
                   MC(N_False); NowGetMode(); MC(N_True);
                  }
                  else if(n_key>=0x20&&n_key<=0x7e)
                  {
                   if((s_autogets&N_AUTOINIT)==N_AUTOINIT)
                   {
                    n_ptr=0; *(n_gets)=0; MC(N_False);
                    drwfillbox(SET, n_bcolor, n_x, n_y, n_x+(n_maxchar<<3), n_y+15);
                    MC(N_True);
                   }
 /* Hangul get processing area - fast search ============================== */
                   if(s_nowgetmode==N_HANGUL)
                   {
                    n_tempinsertmode=s_nowinsertmode;
                    n_key2first=Key2First(n_key);
                    n_key2middle=Key2Middle(n_key);
                    n_key2last=Key2Last(n_key);
                    if(s_hanprocess<=1&&n_key2middle!=N_False)
                    {
                     n_hancode.Code.id=1; n_hancode.Code.First=1;
                     n_hancode.Code.Middle=n_key2middle; n_hancode.Code.Last=1;
                     if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                     {
                      s_hanprocess=3; MC(N_False);
                      if(s_nowinsertmode==N_True||s_getredraw==N_True)
                      {
                       Puts(n_x+(n_ptr<<3), n_y, n_gets+n_ptr, n_color, n_bcolor);
                       drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
                       s_getredraw=N_False;
                      }
                      else HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                      MC(N_True);
                     }
                    }
                    else if(n_key2first!=N_False&&s_hanprocess<=1&&((n_ptr<=(n_maxchar-2)&&s_nowinsertmode==N_False)||(strlen(n_gets)<=(n_maxchar-2)&&s_nowinsertmode==N_True)))
                    {
                     n_hancode.Code.id=1; n_hancode.Code.First=n_key2first;
                     n_hancode.Code.Middle=2; n_hancode.Code.Last=1;
                     if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                     {
                      s_hanprocess=2; MC(N_False);
                      if(s_nowinsertmode==N_True||s_getredraw==N_True)
                      {
                       Puts(n_x+(n_ptr<<3), n_y, n_gets+n_ptr, n_color, n_bcolor);
                       drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
                       s_getredraw=N_False;
                      }
                      else HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                      MC(N_True);
                     }
                    }
                    else if(s_hanprocess==2&&n_key2middle==N_False&&n_key2first!=N_False)
                    {
                     signed int n_first2double=First2Double(n_hancode.Code.First, n_key2first);
                     if(n_first2double!=N_False)
                     {
                      n_hancode.Code.First=n_first2double;
                      s_nowinsertmode=N_False;
                      if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                      {
                       MC(N_False);
                       HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                       MC(N_True);
                      }
                      s_nowinsertmode=n_tempinsertmode;
                     }
                     else
                     {
                      if((n_ptr<=(n_maxchar-2)&&s_nowinsertmode==N_False)||(strlen(n_gets)<=(n_maxchar-2)&&s_nowinsertmode==N_True))
                      {
                       n_ptr+=2; n_hancode.Code.First=n_key2first;
                       if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                       {
                        MC(N_False);
                        if(s_nowinsertmode==N_True||s_getredraw==N_True)
                        {
                         Puts(n_x+(n_ptr<<3), n_y, n_gets+n_ptr, n_color, n_bcolor);
                         drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
                         s_getredraw=N_False;
                        }
                        else HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                        MC(N_True);
                       }
                      }
                     }
                    }
                    else if(n_key2middle!=N_False&&s_hanprocess==2)
                    {
                     n_hancode.Code.Middle=n_key2middle; s_nowinsertmode=N_False;
                     if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                     {
                      s_hanprocess=3; MC(N_False);
                      HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                      MC(N_True);
                     }
                     s_nowinsertmode=n_tempinsertmode;
                    }
                    else if(s_hanprocess==3&&n_key2last==N_False&&n_key2middle!=N_False)
                    {
                     signed int n_middle2double=Middle2Double(n_hancode.Code.Middle, n_key2middle);
                     if(n_middle2double!=N_False)
                     {
                      n_hancode.Code.Middle=n_middle2double;
                      s_nowinsertmode=N_False;
                      if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                      {
                       MC(N_False);
                       HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                       MC(N_True);
                      }
                      s_nowinsertmode=n_tempinsertmode;
                     }
                     else
                     {
                      if((n_ptr<=(n_maxchar-4)&&s_nowinsertmode==N_False)||(strlen(n_gets)<=(n_maxchar-2)&&s_nowinsertmode==N_True))
                      {
                       n_ptr+=2; n_hancode.Code.First=1;
                       n_hancode.Code.Middle=n_key2middle;
                       if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                       {
                        MC(N_False);
                        if(s_nowinsertmode==N_True||s_getredraw==N_True)
                        {
                         Puts(n_x+(n_ptr<<3), n_y, n_gets+n_ptr, n_color, n_bcolor);
                         drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
                         s_getredraw=N_False;
                        }
                        else HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                        MC(N_True);
                       }
                      }
                     }
                    }
                    else if(n_key2last!=N_False&&s_hanprocess==3)
                    {
                     n_hancode.Code.Last=n_key2last;
                     s_nowinsertmode=N_False;
                     if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                     {
                      s_hanprocess=4; MC(N_False);
                      HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                      MC(N_True);
                     }
                     s_nowinsertmode=n_tempinsertmode;
                    }
                    else if(s_hanprocess==4&&n_key2middle==N_False&&n_key2last!=N_False)
                    {
                     signed int n_last2double=Last2Double(n_hancode.Code.Last, n_key2last);
                     if(n_last2double!=N_False)
                     {
                      n_hancode.Code.Last=n_last2double; s_nowinsertmode=N_False;
                      if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                      {
                       MC(N_False);
                       HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                       MC(N_True);
                      }
                      s_nowinsertmode=n_tempinsertmode;
                     }
                     else
                     {
                      if((n_ptr<=(n_maxchar-4)&&s_nowinsertmode==N_False)||(strlen(n_gets)<=(n_maxchar-2)&&s_nowinsertmode==N_True))
                      {
                       n_ptr+=2; n_hancode.Code.id=1;
                       n_hancode.Code.First=n_key2first;
                       n_hancode.Code.Middle=2; n_hancode.Code.Last=1;
                       if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                       {
                        MC(N_False);
                        if(s_nowinsertmode==N_True||s_getredraw==N_True)
                        {
                         Puts(n_x+(n_ptr<<3), n_y, n_gets+n_ptr, n_color, n_bcolor);
                         drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
                         s_getredraw=N_False;
                        }
                        else HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                        MC(N_True); s_hanprocess=2;
                       }
                      }
                     }
                    }
                    else if(n_key2middle!=N_False&&s_hanprocess==4)
                    {
                     signed int n_last2first;
                     signed int n_first;
                     if((n_ptr<=(n_maxchar-4)&&s_nowinsertmode==N_False)||(strlen(n_gets)<=(n_maxchar-2)&&s_nowinsertmode==N_True))
                     {
                      n_last2first=Last2First(n_hancode.Code.Last, &n_first);
                      n_hancode.Code.Last=n_first;
                      s_nowinsertmode=N_False;
                      if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                      {
                       MC(N_False);
                       HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                       MC(N_True);
                      }
                      s_nowinsertmode=n_tempinsertmode;
                      n_ptr+=2; n_hancode.Code.id=1;
                      n_hancode.Code.First=n_last2first;
                      n_hancode.Code.Middle=n_key2middle;
                      n_hancode.Code.Last=1;
                      if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                      {
                       MC(N_False);
                       if(s_nowinsertmode==N_True||s_getredraw==N_True)
                       {
                        Puts(n_x+(n_ptr<<3), n_y, n_gets+n_ptr, n_color, n_bcolor);
                        drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
                        s_getredraw=N_False;
                       }
                       else HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                       MC(N_True); s_hanprocess=3;
                      }
                     }
                    }
                    else if(n_key2first!=N_False&&n_key2last==N_False&&(s_hanprocess==3||s_hanprocess==4))
                    {
                     if((n_ptr<=(n_maxchar-4)&&s_nowinsertmode==N_False)||(strlen(n_gets)<=(n_maxchar-2)&&s_nowinsertmode==N_True))
                     {
                      n_ptr+=2; n_hancode.Code.id=1;
                      n_hancode.Code.First=n_key2first;
                      n_hancode.Code.Middle=2; n_hancode.Code.Last=1;
                      if(WriteHangul(n_gets, n_ptr, n_hancode, n_maxchar)==N_True)
                      {
                       MC(N_False);
                       if(s_nowinsertmode==N_True||s_getredraw==N_True)
                       {
                        Puts(n_x+(n_ptr<<3), n_y, n_gets+n_ptr, n_color, n_bcolor);
                        drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
                        s_getredraw=N_False;
                       }
                       else HanPutc(n_x+(n_ptr<<3), n_y, n_hancode, n_color, n_bcolor);
                       MC(N_True); s_hanprocess=2;
                      }
                     }
                    }
                    else
                    {
                     if(s_hanprocess!=1)
                     {
                      if((n_ptr<=(n_maxchar-2)&&s_nowinsertmode==N_False)||(strlen(n_gets)<=(n_maxchar-1)&&s_nowinsertmode==N_True))n_ptr+=2;
                     }
                     if(WriteCharacter(n_gets, n_ptr, n_key, n_maxchar)==N_True)goto eng_label;
                    } // else
                   } // if(s_nowgetmode==N_HANGUL)
 /* ===========================End Hangul get processing area - fast search */
                   else if(WriteCharacter(n_gets, n_ptr, n_key, n_maxchar)==N_True)
                   {
                    eng_label:;
                    s_hanprocess=1; MC(N_False);
                    Puts(n_x+(n_ptr<<3), n_y, n_gets+n_ptr, n_color, n_bcolor);
                    drwfillbox(SET, n_bcolor, n_x+(strlen(n_gets)<<3), n_y, n_x+(n_maxchar<<3), n_y+15);
                    MC(N_True); n_ptr++;
                   }
                  }
                  break;
            }
            if((s_autogets&N_AUTOINIT)==N_AUTOINIT){s_autogets&=0xffef;}
            MC(N_False); GetsCursor(n_x, n_y, n_gets, n_ptr); MC(N_True);
            while(kbhit())getch();
           }
           goto gets_label;
           end_gets_label:;
           CloseTimer();
           switch(n_key)
           {
            case 0x1b: strcpy(n_string, n_default); break;
            case 0x0d:
            case 0x09:
            case 0x48:
            case 0x50:
            case 0x49:
            case 0x51: strcpy(n_string, n_gets);
            default:   break;
           }
           if((s_autogets&N_AUTOMOUSE)==N_AUTOMOUSE)
           {
            s_ptr=n_ptr;
            if(s_mb==N_LEFTBUTTON){strcpy(n_string, n_gets); n_key=0x00;}
           }
           if(s_mb==N_LEFTBUTTON)n_key=0x00;
           delete(n_gets);
           return(n_key);
}

signed int far Double2Middle(signed int n_key)
{
           switch(n_key)
           {
            case 14:
            case 15:
            case 18: return(13);
            case 21:
            case 22:
            case 23: return(20);
            case 28: return(27);
            default: break;
           }
           return(N_False);
}

void far GetsCursor(signed int n_x, signed int n_y, signed char far *n_gets, signed int n_ptr)
{
           if(s_nowinsertmode==N_True)drwfillbox(XOR, s_getscolor, n_x+(n_ptr<<3), n_y, n_x+(n_ptr<<3)+((IsHanCode(n_gets, n_ptr)==N_True)?15:7), n_y+15);
           else drwbox(XOR, s_getscolor, n_x+(n_ptr<<3), n_y, n_x+(n_ptr<<3)+((IsHanCode(n_gets, n_ptr)==N_True)?15:7), n_y+15);
}

signed int far Last2First(signed int n_key, signed int far *n_ret)
{
 signed int n_last2first[]={0, 0, 2, 3, 11, 4, 14, 20, 5, 7, 2, 8, 9, 11, 18, 19, 20, 8, 0, 9, 11, 11, 12, 13, 14, 16, 17, 18, 19, 20, 0, 0};
 signed int n_last[]={1, 1, 1, 1, 2, 1, 5, 5, 1, 1, 9, 9, 9, 9, 9, 9, 9, 1, 1, 1, 19, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
           *(n_ret)=n_last[n_key]; return(n_last2first[n_key]);
}

signed int far Key2First(signed int n_key)
{
 signed int n_shift;
           if(s_scankey[0x2a]||s_scankey[0x36])n_shift=N_True;
           else n_shift=N_False;
           n_key=toupper(n_key);
           switch(n_key)
           {
            case 'R': return((n_shift==N_True)?3:2);   // 'ˆA', 'ŒA'
            case 'S': return(4);                       // 'A'
            case 'E': return((n_shift==N_True)?6:5);   // '”A', '˜A'
            case 'F': return(7);                       // 'œA'
            case 'A': return(8);                       // ' A'
            case 'Q': return((n_shift==N_True)?10:9);  // '¤A', '¨A'
            case 'T': return((n_shift==N_True)?12:11); // '¬A', '°A'
            case 'D': return(13);                      // '´A'
            case 'W': return((n_shift==N_True)?15:14); // '¸A', '¼A'
            case 'C': return(16);                      // 'ÀA'
            case 'Z': return(17);                      // 'ÄA'
            case 'X': return(18);                      // 'ÈA'
            case 'V': return(19);                      // 'ÌA'
            case 'G': return(20);                      // 'ÐA'
            default: break;
           }
           return(N_False);
}

signed int far Key2Middle(signed int n_key)
{
 signed int n_shift;
           if(s_scankey[0x2a]||s_scankey[0x36])n_shift=N_True;
           else n_shift=N_False;
           n_key=toupper(n_key);
           switch(n_key)
           {
            case 'K': return(3);
            case 'O': return((n_shift==N_True)?6:4);
            case 'I': return(5);
            case 'J': return(7);
            case 'P': return((n_shift==N_True)?12:10);
            case 'U': return(11);
            case 'H': return(13);
            case 'Y': return(19);
            case 'N': return(20);
            case 'B': return(26);
            case 'M': return(27);
            case 'L': return(29);
            default: break;
           }
           return(N_False);
}

signed int far Key2Last(signed int n_key)
{
 signed int n_shift;
           if(s_scankey[0x2a]||s_scankey[0x36])n_shift=N_True;
           else n_shift=N_False;
           n_key=toupper(n_key);
           switch(n_key)
           {
            case 'R': return((n_shift==N_True)?3:2);
            case 'S': if(n_shift==N_False)return(5);
                 break;
            case 'E': if(n_shift==N_False)return(8);
                 break;
            case 'F': if(n_shift==N_False)return(9);
                 break;
            case 'A': if(n_shift==N_False)return(17);
                 break;
            case 'Q': if(n_shift==N_False)return(19);
                 break;
            case 'T': return((n_shift==N_True)?22:21);
            case 'D': if(n_shift==N_False)return(23);
                 break;
            case 'W': if(n_shift==N_False)return(24);
                 break;
            case 'C': if(n_shift==N_False)return(25);
                 break;
            case 'Z': if(n_shift==N_False)return(26);
                 break;
            case 'X': if(n_shift==N_False)return(27);
                 break;
            case 'V': if(n_shift==N_False)return(28);
                 break;
            case 'G': if(n_shift==N_False)return(29);
                 break;
            default: break;
           }
           return(N_False);
}

signed int far WriteHangul(signed char far *n_str, signed int n_ptr, union HanCode n_hancode, signed int n_maxsize)
{
           if(s_nowinsertmode==N_True)
           {
            if(WriteCharacter(n_str, n_ptr, (signed char)n_hancode.ByteData[0], n_maxsize)==N_False)return(N_False);
            if(WriteCharacter(n_str, n_ptr, (signed char)n_hancode.ByteData[1], n_maxsize)==N_False)
            {
             DeleteCharacter(n_str, n_ptr); return(N_False);
            }
           }
           else
           {
            signed char n_tempchar=*(n_str+n_ptr);
            if(IsHanCode(n_str, n_ptr+1)==N_True){WriteCharacter(n_str, n_ptr+2, 0x20, n_maxsize); s_getredraw=N_True;}
            if(WriteCharacter(n_str, n_ptr, (signed char)n_hancode.ByteData[1], n_maxsize)==N_False)return(N_False);
            if(WriteCharacter(n_str, n_ptr+1, (signed char)n_hancode.ByteData[0], n_maxsize)==N_False)
            {
             WriteCharacter(n_str, n_ptr, n_tempchar, n_maxsize); return(N_False);
            }
           }
           return(N_True);
}

signed int far WriteCharacter(signed char far *n_str, signed int n_ptr, signed char n_character, signed int n_maxsize)
{
           if(s_nowinsertmode==N_True)
           {
            signed int n_count;
            if(strlen(n_str)>=n_maxsize)return(N_False);
            for(n_count=strlen(n_str);n_count>=n_ptr;n_count--){*(n_str+n_count+1)=*(n_str+n_count);}
            *(n_str+n_ptr)=n_character;
           }
           else
           {
            if(n_ptr>=n_maxsize)return(N_False);
            if(n_ptr>=strlen(n_str))*(n_str+n_ptr+1)=0;
            if(IsHanCode(n_str, n_ptr)==N_True){*(n_str+n_ptr+1)=0x20;}
            *(n_str+n_ptr)=n_character;
           }
           *(n_str+n_maxsize)=0;
           return(N_True);
}

void far DeleteCharacter(signed char far *n_str, signed int n_ptr)
{
           if(IsHanCode(n_str, n_ptr)==N_True)
           {
            signed int n_count=n_ptr;
            while(*(n_str+n_count)){*(n_str+n_count)=*(n_str+n_count+1); n_count++;}
           }
           while(*(n_str+n_ptr)){*(n_str+n_ptr)=*(n_str+n_ptr+1); n_ptr++;}
}

signed int far GetsMouseProcess(signed int n_x, signed int n_y, signed char far *n_gets, signed int n_ptr, unsigned char n_color, unsigned char n_bcolor, signed int n_maxchar)
{
           if(MouseCheck(0, s_MaxY-21, 38, s_MaxY-1, N_LEFTBUTTON)==N_True)
           {
            MC(N_False); PushRec(0, s_MaxY-21, 38, s_MaxY-1); MC(N_True);
            while(s_mb){MouseProcess(); TimeProcess();}
            MC(N_False); PopRec(0, s_MaxY-21, 38, s_MaxY-1); MC(N_True);
            if(MouseCheck(0, s_MaxY-21, 38, s_MaxY-1, N_NOBUTTON)==N_True)
            {
             MC(N_False); GetsCursor(n_x, n_y, n_gets, n_ptr);
             if(s_nowgetmode==N_True)s_nowgetmode=N_False;
             else s_nowgetmode=N_True;
             if(s_hanprocess!=1)
             {
              if(IsHanCode(n_gets, n_ptr)==N_True)n_ptr++;
              if(n_ptr<strlen(n_gets))n_ptr++;
              s_hanprocess=1;
             }
             if((s_autogets&N_AUTOINIT)==N_AUTOINIT)
             {
              drwfillbox(SET, n_bcolor, n_x, n_y, n_x+(n_maxchar<<3), n_y+15);
              Puts(n_x, n_y, n_gets, n_color, N_Default); s_autogets&=0xffef;
             }
             NowGetMode(); GetsCursor(n_x, n_y, n_gets, n_ptr); MC(N_True);
            }
           }
           if(MouseCheck(39, s_MaxY-21, 77, s_MaxY-1, N_LEFTBUTTON)==N_True)
           {
            MC(N_False); PushRec(39, s_MaxY-21, 77, s_MaxY-1); MC(N_True);
            while(s_mb){MouseProcess(); TimeProcess();}
            MC(N_False); PopRec(39, s_MaxY-21, 77, s_MaxY-1); MC(N_True);
            if(MouseCheck(39, s_MaxY-21, 77, s_MaxY-1, N_NOBUTTON)==N_True)
            {
             MC(N_False); GetsCursor(n_x, n_y, n_gets, n_ptr);
             if(s_nowinsertmode==N_True)s_nowinsertmode=N_False;
             else s_nowinsertmode=N_True;
             if(s_hanprocess!=1)
             {
              if(IsHanCode(n_gets, n_ptr)==N_True)n_ptr++;
              if(n_ptr<strlen(n_gets))n_ptr++;
              s_hanprocess=1;
             }
             if((s_autogets&N_AUTOINIT)==N_AUTOINIT)
             {
              drwfillbox(SET, n_bcolor, n_x, n_y, n_x+(n_maxchar<<3), n_y+15);
              Puts(n_x, n_y, n_gets, n_color, N_Default);
              s_autogets&=0xffef;
             }
             NowGetMode(); GetsCursor(n_x, n_y, n_gets, n_ptr); MC(N_True);
            }
           }
           return(n_ptr);
}

void far NowGetMode(void)
{
           if(s_nowgetmode==N_HANGUL)
           {
            Puts(4, s_MaxY-17, "Ðe‹i", WHITE, 27);
            Puts(3, s_MaxY-18, "Ðe‹i", BLACK, N_Default);
           }
           else
           {
            Puts(4, s_MaxY-17, "µw¢…", WHITE, 27);
            Puts(3, s_MaxY-18, "µw¢…", BLACK, N_Default);
           }
           if(s_nowinsertmode==N_True)
           {
            Puts(43, s_MaxY-17, "¬s·³", WHITE, 27);
            Puts(42, s_MaxY-18, "¬s·³", BLACK, N_Default);
           }
           else
           {
            Puts(43, s_MaxY-17, "®¸÷", WHITE, 27);
            Puts(42, s_MaxY-18, "®¸÷", BLACK, N_Default);
           }
}

signed int far IsHanCode(signed char far *n_str, signed int n_ptr, signed char n_flag)
{
 signed int n_count=0;
           while(*(n_str+n_count))
           {
            if((*(n_str+n_count)&0x80)==0x80)
            {
             if(n_count==n_ptr){return(N_True);}
             n_count++;
             if(n_count==n_ptr&&n_flag==N_True){return(N_True);}
            }
            n_count++;
           }
           return(N_False);
}

signed int far First2Double(signed int n_comp, signed int n_key)
{
 signed int n_shift;
           if(s_scankey[0x2a]||s_scankey[0x36])n_shift=N_True;
           else n_shift=N_False;
           if(n_shift==N_True)return(N_False);
           n_key=toupper(n_key);
           if(n_comp==2&&n_key==2)return(3);
           if(n_comp==5&&n_key==5)return(6);
           if(n_comp==9&&n_key==9)return(10);
           if(n_comp==11&&n_key==11)return(12);
           if(n_comp==14&&n_key==14)return(15);
           return(N_False);
}

signed int far Middle2Double(signed int n_comp, signed int n_key)
{
 signed int n_shift;
           if(s_scankey[0x2a]||s_scankey[0x36])n_shift=N_True;
           else n_shift=N_False;
           if(n_shift==N_True)return(N_False);
           n_key=toupper(n_key);
           if(n_comp==13&&n_key==3)return(14);
           if(n_comp==13&&n_key==4)return(15);
           if(n_comp==13&&n_key==29)return(18);
           if(n_comp==20&&n_key==7)return(21);
           if(n_comp==20&&n_key==10)return(22);
           if(n_comp==20&&n_key==29)return(23);
           if(n_comp==27&&n_key==29)return(28);
           return(N_False);
}

signed int far Last2Double(signed int n_comp, signed int n_key)
{
 signed int n_shift;
           if(s_scankey[0x2a]||s_scankey[0x36])n_shift=N_True;
           else n_shift=N_False;
           if(n_shift==N_True)return(N_False);
           n_key=toupper(n_key);
           if(n_comp==2&&n_key==21)return(4);
           if(n_comp==5&&n_key==24)return(6);
           if(n_comp==5&&n_key==29)return(7);
           if(n_comp==9&&n_key==2)return(10);
           if(n_comp==9&&n_key==17)return(11);
           if(n_comp==9&&n_key==19)return(12);
           if(n_comp==9&&n_key==21)return(13);
           if(n_comp==9&&n_key==27)return(14);
           if(n_comp==9&&n_key==28)return(15);
           if(n_comp==9&&n_key==29)return(16);
           if(n_comp==19&&n_key==21)return(20);
           return(N_False);
}

/* End of source */