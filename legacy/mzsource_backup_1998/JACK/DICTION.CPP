/*
  JACK's DICTION.CPP
*/

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "common.h"
#include "graph.h"
#include "menu.h"
#include "himem.h"
#include "autohan.h"

#define N_DICBUFF 30

void far Diction(void);
void far RegisterDic(signed char far *n_default);
signed int far RegisterWord(signed char far *n_word, signed char far *n_wordhelp);
signed long far SearchWord(signed char far *n_word);
void far ReadWord(signed char far *n_word, signed char far *n_wordhelp);

void far ReadWord(signed char far *n_word, signed char far *n_wordhelp)
{
 FILE *fp;
 signed char n_data;
 signed char far *n_buff;
 signed char far *n_buff2;
 signed char n_flag=N_False;
           if((fp=fopen(MakePath("diction\\jack.dbs"), "rb"))==NULL)return;
           n_buff=new(signed char far [N_DICBUFF+1]);
           n_buff2=new(signed char far [41]);
           do{
              if(fread(&n_data, sizeof(n_data), 1, fp)==0){n_data=(-1);break;}
              if(fread(n_buff, (size_t)n_data, 1, fp) ==0){n_data=(-1);break;}
              if(fread(&n_data, sizeof(n_data), 1, fp)==0){n_data=(-1);break;}
              if(fread(n_buff2, (size_t)n_data, 1, fp) ==0){n_data=(-1);break;}
              if(strcmp(n_buff, n_word)==0)
              {
               n_flag=N_True;
               strcpy(n_wordhelp, n_buff2);
               break;
              }
              if(kbhit())
              {
               if(getch()==0x1b)
               {
                n_flag=N_Error;
                n_data=(-1);
                break;
               }
               while(kbhit())getch();
              }
             }while(n_data);
           delete(n_buff);
           delete(n_buff2);
           if(n_flag==N_False)strcpy(n_wordhelp, "\x10 ”e´áˆa —w¢–A´á·¶»¡ ´g¯s“¡”a.");
           else if(n_flag==N_Error)strcpy(n_wordhelp, "\x10 ”e´áˆñ¬‚·i Âá­¡Ða­v¯s“¡”a.");
           fclose(fp);
           *(n_wordhelp+38)=0;
           *(n_wordhelp+39)=0;
           return;
}

void far Diction(void)
{
 signed int n_key;
 unsigned int n_screenhandle=himemAlloc((unsigned int)((ComputeWindow(100, 200, 539, 282)>>10)+30L));
 signed char far *n_word=new(signed char far [N_DICBUFF]);
 signed char far *n_wordhelp=new(signed char far [41]);
           Help("Àx‰¡¸a Ða“e ”e´áŸi ·³bÐa­A¶a.");
           s_MD.IsWin=N_True;
           MC(N_False);
           Window(100, 200, 539, 282, " e“w¬a¸å", LIGHTGRAY, &s_wd, n_screenhandle);
           Puts(111, 231, "Àx·i”e´á:", WHITE, N_Default);
           Puts(110, 230, "Àx·i”e´á:", DARKGRAY, N_Default);
           Puts(111, 261, "”e´áÐ¬é:", WHITE, N_Default);
           Puts(110, 260, "”e´áÐ¬é:", DARKGRAY, N_Default);
           MC(N_True);
           *(n_word)=0;
           s_nowgetmode=N_ENGLISH;
           while(N_True)
           {
            s_autogets=N_AUTOINIT|N_AUTOMOUSE;
            s_ptr=strlen(n_word);
            n_key=Gets(s_wd.x1+90, s_wd.y1+30, n_word, n_word, 30, BLACK, LIGHTGRAY);
            if(n_key==0x1b)break;
            WindowProcess(&s_wd);
            if(s_wd.command==WIN_CLOSE||s_mb==N_RIGHTBUTTON)break;
            if(s_mb==N_RIGHTBUTTON)break;
            if(s_wd.command==WIN_MOVE||*(n_word)==0||*(n_word)==0x20)continue;
            if(MouseCheck(s_wd.x1+90, s_wd.y1+30, s_wd.x1+330, s_wd.y1+45, N_LEFTBUTTON)==N_False&&s_mb==N_LEFTBUTTON)continue;
            Help("”e´áŸi ˆñ¬‚º—·³“¡”a. ¸q¯¡ ‹¡”aa º¯³¯¡¶a.");
            drwfillbox(SET, LIGHTGRAY, s_wd.x1+90, s_wd.y1+60, s_wd.x1+410, s_wd.y1+75);
            Puts(s_wd.x1+90, s_wd.y1+60, "& ˆñ¬‚º— &", BLACK, LIGHTGRAY);
            ReadWord(n_word, n_wordhelp);
            MC(N_False);
            drwfillbox(SET, LIGHTGRAY, s_wd.x1+90+(strlen(n_wordhelp)<<3), s_wd.y1+60, s_wd.x1+410, s_wd.y1+75);
            Puts(s_wd.x1+90, s_wd.y1+60, n_wordhelp, BLACK, LIGHTGRAY);
            MC(N_True);
            Help(n_wordhelp);
           }
           MC(N_False);
           PutImageXMS(s_wd.x1, s_wd.y1, s_wd.x2, s_wd.y2, n_screenhandle);
           MC(N_True);
           himemFree(n_screenhandle);
           delete(n_word);
           delete(n_wordhelp);
           s_MD.IsWin=N_False;
}

void far RegisterDic(signed char far *n_default)
{
 signed int n_key;
 unsigned int n_screenhandle=himemAlloc((unsigned int)((ComputeWindow(100, 150, 539, 330)>>10)+30L));
 signed char far *n_word=new(signed char far [N_DICBUFF+1]);
 signed char far *n_wordhelp=new(signed char far [41]);
           Re_Register:;
           Help("”e´áŸi —w¢Ðº­A¶a. \(Âá­¡:ESC\)");
           MC(N_False);
           Window(100, 150, 539, 330, "¯¥‹A”e´á —w¢", LIGHTGRAY, &s_wd, n_screenhandle);
           Puts(111, 181, "—w¢”e´á:", WHITE, N_Default);
           Puts(110, 180, "—w¢”e´á:", DARKGRAY, N_Default);
           Puts(111, 211, "”e´áÐ¬é:", WHITE, N_Default);
           Puts(110, 210, "”e´áÐ¬é:", DARKGRAY, N_Default);
           PushRec(110, 245, 529, 246);
           Puts(110, 260, "1. ”e´á“e ÂA‰¡ 30 byteŒa»¡ ‹¡¢ˆa“wÐs“¡”a.", BLACK, N_Default);
           Puts(110, 276, "2. ”e´áÐ¬é·e ÂA‰¡ 40 byteŒa»¡ ‹¡¢ˆa“wÐs“¡”a.", BLACK, N_Default);
           Puts(110, 292, "3. —w¢”e´á· ¹AÐeˆ•®“e ¢¹AÐe·³“¡”a.", BLACK, N_Default);
           Puts(110, 308, "4. µw´á”e´á £¹ ”aŸe ”e´á•¡ —w¢·¡ ˆa“wÐs“¡”a.", BLACK, N_Default);
           MC(N_True);
           strcpy(n_word, n_default);
           strcpy(n_wordhelp, "");
           s_autogets=N_AUTOMOUSE|N_AUTOINIT|N_AUTOARROW|N_AUTOTAB;
           while(N_True)
           {
            s_ptr=strlen(n_word);
            s_nowgetmode=N_ENGLISH;
            MC(N_False); NowGetMode(); MC(N_True);
            again_loop1:;
            n_key=Gets(s_wd.x1+90, s_wd.y1+30, n_word, n_word, N_DICBUFF, BLACK, LIGHTGRAY);
            if(n_key==0x1b)break;
            WindowProcess(&s_wd);
            if(s_wd.command==WIN_CLOSE||s_mb==N_RIGHTBUTTON)break;
            if(s_wd.command==WIN_MOVE)
            {
             s_wd.command=WIN_NONE;
             goto again_loop1;
            }
            if(MouseCheck(s_wd.x1+90, s_wd.y1+60, s_wd.x1+410, s_wd.y1+75, N_LEFTBUTTON)==N_False&&s_mb==N_LEFTBUTTON)goto again_loop1;
            s_ptr=strlen(n_wordhelp);
            s_nowgetmode=N_HANGUL;
            MC(N_False); NowGetMode(); MC(N_True);
            again_loop2:;
            n_key=Gets(s_wd.x1+90, s_wd.y1+60, n_wordhelp, n_wordhelp, 40, BLACK, LIGHTGRAY);
            if(n_key==0x1b)break;
            if(n_key==0x0d)break;
            WindowProcess(&s_wd);
            if(s_wd.command==WIN_CLOSE||s_mb==N_RIGHTBUTTON)break;
            if(s_wd.command==WIN_MOVE)
            {
             s_wd.command=WIN_NONE;
             goto again_loop2;
            }
            if(MouseCheck(s_wd.x1+90, s_wd.y1+30, s_wd.x1+330, s_wd.y1+45, N_LEFTBUTTON)==N_False&&s_mb==N_LEFTBUTTON)goto again_loop2;
           }
           MC(N_False);
           PutImageXMS(s_wd.x1, s_wd.y1, s_wd.x2, s_wd.y2, n_screenhandle);
           MC(N_True);
           if(n_key==0x0d)
           {
            if(*(n_word)==0)
            {
             Check("”e´áˆa ·³b–A»¡ ´g´v¯s“¡”a.");
            }
            else if(YesNo("”e´áŸi —w¢Ða¯¡‰V¯s“¡Œa?", N_Yes)==N_Yes)RegisterWord(n_word, n_wordhelp);
            if(YesNo("‰­¢Ð¬á ”e´áŸi —w¢Ða¯¡‰V¯s“¡Œa?", N_Yes)==N_Yes)
            {
             strcpy(n_default, "");
             goto Re_Register;
            }
           }
           himemFree(n_screenhandle);
           delete(n_word);
           delete(n_wordhelp);
}

signed int far RegisterWord(signed char far *n_word, signed char far *n_wordhelp)
{
 FILE *fp;
 signed char n_data;
           if(SearchWord(n_word)!=(-1L))
           {
            if(YesNo("·¡£¡ —w¢–A´á·¶¯s“¡”a. ¸—w¢ÐiŒa¶a?", N_No)==N_No)return(N_False);
           }
           if((fp=fopen(MakePath("diction\\jack.dbs"), "ab"))==NULL)return(N_False);
           n_data=strlen(n_word)+1;
           fwrite(&n_data, sizeof(n_data), 1, fp);
           fwrite(n_word, n_data, 1, fp);
           n_data=strlen(n_wordhelp)+1;
           fwrite(&n_data, sizeof(n_data), 1, fp);
           fwrite(n_wordhelp, n_data, 1, fp);
           fclose(fp);
           return(N_True);
}

signed long far SearchWord(signed char far *n_word)
{
 FILE *fp;
 signed char n_data;
 signed long n_return=0L;
 signed char far *n_buff;
           Help("”e´áˆa ·¡£¡ ¹¥¸Ða“e»¡ ˆñ¬‚º—·³“¡”a.");
           if((fp=fopen(MakePath("diction\\jack.dbs"), "rb"))==NULL)return((-1L));
           n_buff=new(signed char far [N_DICBUFF+1]);
           do{
              if(fread(&n_data, sizeof(n_data), 1, fp)==0){n_data=(-1);break;}
              if(fread(n_buff, (size_t)n_data, 1, fp) ==0){n_data=(-1);break;}
              if(strcmp(n_buff, n_word)==0)break;
              if(fread(&n_data, sizeof(n_data), 1, fp)==0){n_data=(-1);break;}
              if(fseek(fp, n_data, SEEK_CUR)!=0){n_data=(-1);break;}
              n_return++;
             }while(n_data);
           delete(n_buff);
           fclose(fp);
           if(n_data==(-1))return((-1L));
           return(n_return);
}

/* End of source */