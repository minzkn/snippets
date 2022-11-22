/*
  JACK's MENU.CPP
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <new.h>
#include "common.h"
#include "graph.h"
#include "himem.h"
#include "init.h"
#include "autohan.h"
#include "timer.h"
#include "shell.h"
#include "com.h"
#include "diary.h"
#include "diction.h"
#include "asmproc.h"
#include "jf.h"

#define N_XORPOPCOLOR WHITE

unsigned int s_menuhandle;
unsigned int s_popmenuhandle;
signed char s_popmenux=0, s_maxpopmenux;
signed char far s_popmenuy[6]={0, };
signed char far s_maxpopmenuy[6]={0, };
signed char far *s_popmenustring[6][20]={
                     /* "  ·¡ˆõ·e"  */   {"JACK & JACK", "", "•¡¯aa—i·¡", "¹·žaÐa‹¡", 0},
                     /* " É·Ðs‹¡“w" */   {"·©‹¡¸w", "ˆa‰¦", "¹A‰¡‰ÅŸ¡", "¢‰Î‘¦…ŸA", "ˆáœÀá", "§¡—¡µ¡ ®s", "•¡¬á‰ÅŸ¡", "Ìa·©‰ÅŸ¡", 0},
                     /* " ·©¤e‹¡“w" */   {"Ìa·©Íe»³", "•¡¯a®É", "ˆe”eÐe É·¯¥", "Ìa·©´sÂ‚...", "Ìa·©Àx‹¡", "µw´á¬a¸å...", "µe‰i¯©Ð—", "‹¡«q”ib", 0},
                     /* " ¸å¶¥‰ÅŸ¡" */   {"—¡¯aÇa ÌaÇ·", "¸¦Ë·", "ÑÁ¡e¥¡Ñ¡", "Âá‹sº·...", 0},
                     /* " ÑÅ‰w¬é¸÷" */   {"Á¡‹¡¬é¸÷", "‹¡¥¥ÑÅ‰w", "´qÑ¡¬é¸÷", "¤‰w¤aŽ‹¡...", 0},
                     /* "  ¸÷  ¥¡"  */   {"•¡¶‘ i", "¹A¸bÑÅ‰w", " e—e·¡“e", "¯¡¯aÉQ ˆñ¬‚", "§¡‰·ˆ ¡A“A", 0},
                                        };
signed char far *s_popmenukey[6][20]={
                     /* "  ·¡ˆõ·e"  */   {"", "", "@S", "@X", 0},
                     /* " É·Ðs‹¡“w" */   {"", "", "", "", "", "", "", "", 0},
                     /* " ·©¤e‹¡“w" */   {"@E", "@D", "@C", "\x10\x10", "@F", "\x10\x10", "@R", "@Y", 0},
                     /* " ¸å¶¥‰ÅŸ¡" */   {"^P", "^B", "\x10\x10", "\x10\x10", 0},
                     /* " ÑÅ‰w¬é¸÷" */   {"", "^S", "", "\x10\x10", 0},
                     /* "  ¸÷  ¥¡"  */   {"F1", "", "", "", "\x10\x10", 0},
                                        };

signed char far *s_mainstring[]={"  ·¡ˆõ·e", " É·Ðs‹¡“w", " ·©¤e‹¡“w", " ¸å¶¥‰ÅŸ¡", " ÑÅ‰w¬é¸÷", "  ¸÷  ¥¡", NULL};
signed char s_popmouseflag=N_False;

void far Menu(signed char n_swich);
void far PopMenu(signed char n_swich);
void far System(void);
void far PopMouseProcess(void);
signed int far YesNo(signed char far *n_string, signed char n_default);
void far PopHelp(void);
void far BackGround(signed char n_sw, signed int n_select);
void far DeCompressData(signed char far *n_string);
signed int far RightButtonMenu(signed char far **n_string, signed int n_x =NULL, signed int n_y =NULL);
void far Check(signed char far *n_string);

void far PopMenuRun(signed char n_x, signed char n_y)
{
 signed char far *n_helpstring;
 signed int n_casemenu;
           if(n_x==4&&n_y==3)
           {
            signed char far *n_backmenu[]={"ÐaµÁ·¡ Ð¥e", "•¡¯¡Ñ¡®· ´¡‰w",
                                           "·¥‰·¶á¬÷", "®µw", "ŒiqÑÁ¡e",
                                           NULL};
            n_casemenu=RightButtonMenu(n_backmenu, 535, 100);
            if(n_casemenu!=N_Not)
            {
             Menu(N_False);
             BackGround(1, n_casemenu);
             Menu(N_True);
             PopHelp();
            }
            return;
           }
           if(n_x==2&&n_y==3)
           {
            signed char far *n_backmenu[]={"´sÂ‚Ða‹¡", "´sÂ‚Î‰‹¡", NULL};
            n_casemenu=RightButtonMenu(n_backmenu, 345, 100);
            if(n_casemenu!=N_Not)
            {
             Menu(N_False);
             Shell("*.*");
             Menu(N_True);
             PopHelp();
            }
            return;
           }
           if(n_x==3&&n_y==2)
           {
            signed char far *n_backmenu[]={"¦‰Èa“e ÑÁ¡e", "Â‰œ÷·¡“e ¢‰", NULL};
            n_casemenu=RightButtonMenu(n_backmenu, 440, 77);
            if(n_casemenu!=N_Not)
            {
             Menu(N_False);
             switch(n_casemenu)
             {
              case 0x00: Saver(0x00); break;
              case 0x01: Saver(0x01); break;
              default: break;
             }
             Menu(N_True);
             PopHelp();
            }
            return;
           }
           if(n_x==3&&n_y==3)
           {
            signed char far *n_backmenu[]={"BIOS»¡¶‹¡", "—¡¯aÇa ˆñ¬a", NULL};
            n_casemenu=RightButtonMenu(n_backmenu, 440, 100);
            if(n_casemenu!=N_Not)
            {
             Menu(N_False);
             switch(n_casemenu)
             {
              case 0x00:
                   if(YesNo("¸÷ i¡ BIOS •A·¡ÈáŸi »¡¶¯¡‰V¯s“¡Œa?", N_No)==N_Yes)
                   {
                    Help("Warnig: Clean ROM-BIOS!");
                    sleep(2);
                    asmclrbios();
                   }
                   break;
              case 0x01:
                   if(YesNo("¸÷ i¡ —¡¯aÇaŸi ¸ñˆñ Ða¯¡‰V¯s“¡Œa?", N_No)==N_Yes);
                   break;
              default: break;
             }
             Menu(N_True);
             PopHelp();
            }
            return;
           }
           if(n_x==5&&n_y==4)
           {
            signed char far *n_backmenu[]={"Ìa·©·³b¡A“A", "µ¡É¡ aÈa", "¯aµÁÏ·", "¡A“Aˆñ¬a", "¶å•¡¶ µwµb", "Check", "Image work", NULL};
            n_casemenu=RightButtonMenu(n_backmenu, 630, 120);
            if(n_casemenu!=N_Not)
            {
             Menu(N_False);
             switch(n_casemenu)
             {
              case 0x00: Shell("*.EXE;*.COM"); break;
              case 0x01:
                   {
                    signed char n_string[51];
                    s_autogets=N_AUTOINIT;
                    s_nowgetmode=N_HANGUL;
                    MC(N_False); NowGetMode(); MC(N_True);
                    Gets(0, 560, "Ðe‹iµ¡É¡ aÈa:", n_string, 50, WHITE, BLACK);
                   }
                   break;
              case 0x04:
                   {
                    WindowData n_wd;
                    unsigned int n_handle=himemAlloc(100);
                    MC(N_False);
                    Window(0, 0, 150, 150, "¶å•¡¶", LIGHTGRAY, &n_wd, n_handle);
                    Puts(2, 30, "¡A·~", WHITE, BLACK); MC(N_True);
                    while(!kbhit()){MouseProcess(); TimeProcess(); WindowProcess(&n_wd); if(n_wd.command==WIN_CLOSE)break;}
                    MC(N_False);
                    PutImageXMS(n_wd.x1, n_wd.y1, n_wd.x1+150, n_wd.y1+150, n_handle);
                    MC(N_True);
                    himemFree(n_handle);
                   }
                   break;
              case 0x05: Check("Check message!"); break;
              case 0x06:
                   {
                    unsigned int n_temphandle=himemAlloc(512);
                    MC(N_False);
                    GetImageXMS(0, 0, 799, 599, n_temphandle);
                    drwfillbox(SET, RED, 0, 0, 799, 599);
                    Puts(0, 0, "Use himem mode [Wait]  copyright(c)1998", WHITE, N_Default);
                    sleep(1);
                    PutImageXMS(0, 0, 799, 599, n_temphandle);
                    GetImageDisk(0, 0, 799, 599, "~temp.swp", 0);
                    drwfillbox(SET, BLUE, 0, 0, 799, 599);
                    Puts(0, 0, "Use swapping mode [Wait]  copyright(c)1998", WHITE, N_Default);
                    sleep(1);
                    PutImageDisk(0, 0, 799, 599, "~temp.swp", 0);
                    UnLink(MakePath("~temp.swp"));
                    MC(N_True);
                    himemFree(n_temphandle);
                   }
                   break;
              default: break;
             }
             Menu(N_True);
             PopHelp();
            }
            return;
           }
           if(n_x==2&&n_y==5)
           {
            signed char far *n_backmenu[]={" e“w¬a¸å", "”e´á—w¢", "”e´á¸÷Ÿ¡", NULL};
            n_casemenu=RightButtonMenu(n_backmenu, 345, 144);
            if(n_casemenu!=N_Not)
            {
             Menu(N_False);
             switch(n_casemenu)
             {
              case 0x00: Diction(); break;
              case 0x01: RegisterDic(""); break;
              case 0x02: YesNo("DATAÌa·©·i ¸÷Ÿ¡Ða¯¡‰V¯s“¡˜a?", N_True); break;
              default: break;
             }
             Menu(N_True);
             PopHelp();
            }
            return;
           }
// =================================================================
           n_helpstring=new(signed char far [80]);
           sprintf(n_helpstring, "linking object: \`%s\'¡A“A· ¯©Ð—·i º…§¡º—·³“¡”a.", s_popmenustring[n_x][n_y]);
           Help(n_helpstring);
           delete(n_helpstring);
           Menu(N_False);
           switch(n_x)
           {
            case 0:
                 switch(n_y)
                 {
                  case 0: Title(); break;
                  case 1: break;
                  case 2: if(YesNo("•¡¯aa—i·¡Ÿi Ða¯¡‰V¯s“¡Œa?", N_False)==N_True)System(); break;
                  case 3: if(YesNo("Ïa¡‹aœ‘·i ¹·žaÐa¯¡‰V¯s“¡Œa?", N_False)==N_True)Close();
                       break;
                  default:
                       Help("¡A“AŸi ¥¡¯¡a¡e ´a¢Ç¡a ’‰œáº­A¶a.");
                       while(!s_mb&&!kbhit())
                       {
                        MouseProcess();
                        TimeProcess();
                       }
                       while(kbhit())getch();
                       break;
                 }
                 break;
            case 1:
                 MC(N_False);
                 drwfillbox(SET, N_SYSCOLOR, 0, 0, 639, 479);
                 MC(N_True);
                 switch(n_y)
                 {
                  case 0x00: Diary(); break;
                  default: break;
                 }
                 BackGround(2);
                 BackGround(0);
                 break;
            case 2:
                 switch(n_y)
                 {
                  case 0x00:  break;
                  case 0x02: Com();     break;
                  default: break;
                 }
                 break;
            case 3:
                 switch(n_y)
                 {
                  case 0x00:
                       {
                        signed int n_x, n_y;
                        signed char far *n_park="jackswap\\~park.swp";
                        MC(N_False);
                        GetImageDisk(120, 290, 520, 315, n_park, 0);
                        asmpark();
                        for(n_y=0;n_y<s_MaxY;n_y+=2)
                        {
                         for(n_x=0;n_x<s_MaxX;n_x+=2)drwpoint(XOR, WHITE, n_x, n_y);
                        }
                        MC(N_True);
                        Window(120, 290, 520, 315, "—¡¯aÇaˆa ÌaÇ·–A´ö¯s“¡”a. ¸å¶¥·i aº­A¶a.", LIGHTGRAY, NULL, NULL);
                        while(!kbhit()&&!s_mb)MouseProcess();
                        MC(N_False);
                        for(n_y=0;n_y<s_MaxY;n_y+=2)
                        {
                         for(n_x=0;n_x<s_MaxX;n_x+=2)drwpoint(XOR, WHITE, n_x, n_y);
                        }
                        PutImageDisk(120, 290, 520, 315, n_park, 0);
                        MC(N_True);
                        UnLink(MakePath(n_park));
                       }
                       break;
                  case 0x01:
                       if(YesNo("ÄñÏAÈáŸi ”a¯¡ ¦Ë·Ða‰V¯s“¡Œa?", N_No)==N_Yes)
                       {
                        union REGS Register;
                        void (far *n_boot)(void);
                        Help("¸q¯¡ e ‹¡”aaº­A¶a. ¸¦Ë··i ¶áÐe º…§¡º—·³“¡”a.");
                        *((unsigned int far *)0x00000472L)=0x0000;
                        (void far *)n_boot=(void far *)0xffff0000L;
                        UnLink(MakePath("jackswap\\jack04.jim"));
                        UnLink(MakePath("jackswap\\jack05.jim"));
                        UnLink(MakePath("jackswap\\jack06.jim"));
                        himemAllFree();
                        delete(s_MakeDirectory); delete(s_WorkDirectory);
                        set_new_handler(0); while(kbhit())getch();
                        CloseUpKey();
                        sleep(3);
                        Register.x.ax=0x0003; int86(0x10, &Register, &Register);
                        n_boot();
                       }
                       break;
                  case 0x02:
                       if(YesNo("JACK & JACK·i ”a¯¡ ¯¡¸bÐa¯¡‰V¯s“¡Œa?", N_No)==N_Yes);
                       break;
                  default: break;
                 }
                 break;
            case 4:
                 switch(n_y)
                 {
                  case 0x01: break;
                  default: break;
                 }
                 break;
            case 5:
                 switch(n_y)
                 {
                  case 0x00:
                  case 0x01:
                  case 0x02:
                  case 0x03: break;
                  default:
                       Shell("*.jkp");
                       break;
                 }
                 break;
            default: break;
           }
           Menu(N_True);
           PopHelp();
}

void far PopHelp(void)
{
 signed char far *n_pophelpstring[6][15]={
                     /* "  ·¡ˆõ·e"  */   {"¯¥‹¡®‰·i ÂŠÐa“e JACK & JACK!", "ÑÁ¡e¥¡Ñ¡‹¡Ÿi ¯©Ð—Ðs“¡”a.", "¸q¯¡ •¡¯a¡ aˆs“¡”a. 'EXIT'Ÿi ·³bÐa¯¡¡e •©´aµ³“¡”a.", "Ïa¡‹aœ‘·i ¹·žaÐs“¡”a.", 0},
                     /* " É·Ðs‹¡“w" */   {"Â´â‰Á w e·i ‹¡¢Ða“e ·©‹¡¸w·³“¡”a.", "´i›i¬i›i! ˆa‰¦·³“¡”a.", "¹A‰¡‰ÅŸ¡", "¢‰Î‘¦…ŸA", "ˆáœÀá", "§¡—¡µ¡ ®s", "•¡¬á‰ÅŸ¡", "Ìa·©‰ÅŸ¡", 0},
                     /* " ·©¤e‹¡“w" */   {"Ìa·©·i Íe»³Ðs“¡”a.", "•¡¯a®É", "ˆe”eÐa‰A É·¯¥µA ¸ó­¢Ði® ·¶¯s“¡”a.", "JACK & JACK· ´sÂ‚Í¡¡U·a¡ Ìa·©·i ´sÂ‚‰ÅŸ¡ Ðs“¡”a.", "»¡¸÷Ðe Ìa·©·i Àx´aº““¡”a.", "µwÐe¬a¸å·³“¡”a.", "”aŸe Ïa¡‹aœ‘·i —w¢Ðaµa ¬a¶wÐi® ·¶¯s“¡”a.", "”ib·³“¡”a.", 0},
                     /* " ¸å¶¥‰ÅŸ¡" */   {"—¡¯aÇaŸi ÌaÇ·¯¡Ç³“¡”a.", "ÄñÏAÈáŸi RESETÐs“¡”a. \(Å©—a¦Ë·\)", "Ïa¡‹aœ‘·i ¸¯¡¸bÐs“¡”a.", "·¡ ¡A“A“e •A·¡ÈáŸi ÌaŠAÐi®•¡ ·¶·a“¡ ¬a¶wµA º·Ða­A¶a.", 0},
                     /* " ÑÅ‰w¬é¸÷" */   {"¯¡¸bÐi˜ ¡yˆa»¡ ¬aÐw·i ¬é¸÷Ðs“¡”a.", "‹¡¥¥¸â·¥ ÑÅ‰w¬é¸÷·i Ðs“¡”a.", "¸a¯¥ e· ´qÑ¡¡¬á •A·¡ÈáŸi ¥¡Ñ¡Ðs“¡”a.", "¤‰w·i ¤aŽ““¡”a.", 0},
                     /* "  ¸÷  ¥¡"  */   {"•¡¶‘ i·³“¡”a.", "¹A¸bÑÅ‰wŸi ­¡ˆÐs“¡”a.", "¹A¸b¸aŸi ­¡ˆÐs“¡”a.", "¯¡¯aÉQ ˆñ¬‚Ÿi ®Ð—Ðs“¡”a.", "ˆ¤i¸a· §¡£©¡A“A Ì‚Ç¡»¡µA“e ´ô“e ¡A“A~!!!", 0},
                                        };
 signed char far *n_tempstring;
           n_tempstring=new(signed char far [80]);
           strcpy(n_tempstring, "¡A“A¬é¡w - ");
           strcat(n_tempstring, n_pophelpstring[s_popmenux][s_popmenuy[s_popmenux]]);
           Help(n_tempstring);
           delete(n_tempstring);
}

signed int RightButtonMenu(signed char far **n_string, signed int n_x, signed int n_y)
{
 WindowData n_wd;
 unsigned int n_righthandle;
 signed int n_menuy=0, n_maxmenuy;
 signed int n_key;
           n_maxmenuy=0; while(n_string[n_maxmenuy])n_maxmenuy++;
           if(n_x==NULL&&n_y==NULL){n_wd.x1=s_mx+1; n_wd.x2=s_mx+151; n_wd.y1=s_my+1; n_wd.y2=s_my+(n_maxmenuy*20);}
           else {n_wd.x1=n_x+1; n_wd.x2=n_x+151; n_wd.y1=n_y+1; n_wd.y2=n_y+(n_maxmenuy*20);}
           if(n_wd.x2>=s_MaxX){n_wd.x1=s_MaxX-151; n_wd.x2=s_MaxX-1;}
           if(n_wd.y2>=s_MaxY-22){n_wd.y1=s_MaxY-(n_maxmenuy*20)-22; n_wd.y2=s_MaxY-23;}
           n_righthandle=himemAlloc((unsigned int)((ComputeWindow(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2)>>10)+30L));
           MC(N_False);
           GetImageXMS(n_wd.x1-1, n_wd.y1-1, n_wd.x2+1, n_wd.y2+1, n_righthandle);
           drwbox(SET, BLACK, n_wd.x1-1, n_wd.y1-1, n_wd.x2+1, n_wd.y2+1);
           for(n_menuy=0;n_menuy<n_maxmenuy;n_menuy++)
           {
            PopBox(n_wd.x1, n_wd.y1+(n_menuy*20), n_wd.x2, n_wd.y1+(n_menuy*20)+19, LIGHTGRAY);
            if(n_x==NULL&&n_y==NULL)Puts(n_wd.x1+6, n_wd.y1+(n_menuy*20)+4, n_string[n_menuy], WHITE, N_Default);
            Puts(n_wd.x1+5, n_wd.y1+(n_menuy*20)+3, n_string[n_menuy], BLACK, N_Default);
           }
           n_menuy=0;
           drwfillbox(XOR, N_XORPOPCOLOR, n_wd.x1, n_wd.y1+(n_menuy*20), n_wd.x2, n_wd.y1+(n_menuy*20)+19);
           MC(N_True);
           while(s_mb){MouseProcess(); TimeProcess();}
           right_label:;
           n_key=0; MouseProcess(); TimeProcess();
           if(s_mb==N_RIGHTBUTTON){n_menuy=N_Not; goto end_right_label;}
           else if(MouseCheck(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, N_Default)==N_True)
           {
            signed char n_wr=N_False;
            if(s_mb==N_LEFTBUTTON)n_wr=N_True;
            while_right_label:;
            if(s_popmouseflag==N_True&&(MouseCheck(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, N_Default)==N_True))
            {
             signed int n_tempmenuy=(s_my-n_wd.y1)/20;
             s_popmouseflag=N_False;
             if(n_menuy!=n_tempmenuy)
             {
              MC(N_False);
              drwfillbox(XOR, N_XORPOPCOLOR, n_wd.x1, n_wd.y1+(n_menuy*20), n_wd.x2, n_wd.y1+(n_menuy*20)+19);
              n_menuy=n_tempmenuy;
              drwfillbox(XOR, N_XORPOPCOLOR, n_wd.x1, n_wd.y1+(n_menuy*20), n_wd.x2, n_wd.y1+(n_menuy*20)+19);
              MC(N_True);
             }
            }
            if(s_mb==N_LEFTBUTTON)
            {
             MouseProcess();
             goto while_right_label;
            }
            if(n_wr==N_True&&(s_mb&N_RIGHTBUTTON)!=N_RIGHTBUTTON)
            {
             if(MouseCheck(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, N_Default)==N_False)n_menuy=N_Not;
             goto end_right_label;
            }
            while(s_mb){MouseProcess(); TimeProcess();}
           }
           else if(s_mb==N_LEFTBUTTON){n_menuy=N_Not; goto end_right_label;}
           if(kbhit())
           {
            n_key=getch();
            if(n_key==0||kbhit()||n_key==0x09)
            {
             if(n_key!=0x09)n_key=getch();
             switch(n_key)
             {
              case 0x4b:
                   if(n_x>NULL&&n_y>NULL)
                   {
                    n_menuy=N_Not;
                    goto end_right_label;
                   }
              case 0x48:
              case 0x0f:
                   MC(N_False);
                   drwfillbox(XOR, N_XORPOPCOLOR, n_wd.x1, n_wd.y1+(n_menuy*20), n_wd.x2, n_wd.y1+(n_menuy*20)+19);
                   n_menuy--;
                   if(n_menuy<0)n_menuy=n_maxmenuy-1;
                   drwfillbox(XOR, N_XORPOPCOLOR, n_wd.x1, n_wd.y1+(n_menuy*20), n_wd.x2, n_wd.y1+(n_menuy*20)+19);
                   MC(N_True);
                   break;
              case 0x50:
              case 0x4d:
              case 0x09:
                   MC(N_False);
                   drwfillbox(XOR, N_XORPOPCOLOR, n_wd.x1, n_wd.y1+(n_menuy*20), n_wd.x2, n_wd.y1+(n_menuy*20)+19);
                   n_menuy++;
                   if(n_menuy>=n_maxmenuy)n_menuy=0;
                   drwfillbox(XOR, N_XORPOPCOLOR, n_wd.x1, n_wd.y1+(n_menuy*20), n_wd.x2, n_wd.y1+(n_menuy*20)+19);
                   MC(N_True);
                   break;
              case 0x47:
              case 0x49:
                   MC(N_False);
                   drwfillbox(XOR, N_XORPOPCOLOR, n_wd.x1, n_wd.y1+(n_menuy*20), n_wd.x2, n_wd.y1+(n_menuy*20)+19);
                   n_menuy=0;
                   drwfillbox(XOR, N_XORPOPCOLOR, n_wd.x1, n_wd.y1+(n_menuy*20), n_wd.x2, n_wd.y1+(n_menuy*20)+19);
                   MC(N_True);
                   break;
              case 0x4f:
              case 0x51:
                   MC(N_False);
                   drwfillbox(XOR, N_XORPOPCOLOR, n_wd.x1, n_wd.y1+(n_menuy*20), n_wd.x2, n_wd.y1+(n_menuy*20)+19);
                   n_menuy=n_maxmenuy-1;
                   drwfillbox(XOR, N_XORPOPCOLOR, n_wd.x1, n_wd.y1+(n_menuy*20), n_wd.x2, n_wd.y1+(n_menuy*20)+19);
                   MC(N_True);
                   break;
              default: break;
             }
             n_key=0;
            }
            switch(n_key)
            {
             case 0x1b: n_menuy=N_Not; goto end_right_label;
             case 0x0d: goto end_right_label;
             default: break;
            }
            while(kbhit())getch();
           }
           goto right_label;
           end_right_label:;
           while(s_mb){MouseProcess();TimeProcess();}
           MC(N_False);
           PutImageXMS(n_wd.x1-1, n_wd.y1-1, n_wd.x2+1, n_wd.y2+1, n_righthandle);
           MC(N_True); himemFree(n_righthandle);
           return(n_menuy);
}

void far DeCompressData(signed char far *n_string)
{
 signed char far *n_filename=new(signed char far [30]);
 signed char far *n_detect=new(signed char far [MAXPATH]);
           sprintf(n_filename, "jackdata\\%s", n_string);
           strcpy(n_detect, MakePath(n_filename));
           sprintf(n_filename, "jackswap\\%s", n_string);
           DeCompress(n_detect, MakePath(n_filename));
           delete(n_filename); delete(n_detect);
}

void far BackGround(signed char n_sw, signed int n_select)
{
 MainConfig n_mc;
 signed char far *n_filename=new(signed char far [30]);
           LoadConfig(&n_mc);
           switch(n_sw)
           {
            case 0x01:
                 Help("¸q¯¡ e ‹¡”aa º¯³¯¡¶a. Ñe¸ ¤‰w·i ¤aŽ‹¡ ¶áÐe ¸b´óº—·³“¡”a.");
                 if(n_select==N_Default)
                 {
                  n_mc.background++;
                  if(n_mc.background>3)n_mc.background=0;
                 }
                 else n_mc.background=n_select;
                 SaveConfig(&n_mc);
                 if(n_mc.background!=4)
                 {
                  sprintf(n_filename, "jbgd%02d.jim", n_mc.background);
                  DeCompressData(n_filename);
                 }
            case 0x00:
                 if(n_mc.background==4)
                 {
                  signed int n_x, n_y;
                  MC(N_False);
                  drwfillbox(SET, 0, 0, 0, 639, 479);
                  for(n_y=0;n_y<480;n_y+=2)
                  {
                   for(n_x=0;n_x<640;n_x+=2)drwpoint(SET, 239, n_x, n_y);
                  }
                  Puts(8, 0, "JACK & JACK", 238, N_Default);
                  MC(N_True);
                  break;
                 }
                 sprintf(n_filename, "jackswap\\jbgd%02d.jim", n_mc.background);
                 MC(N_False); PutImageDisk(0, 0, 639, 479, n_filename, 64); MC(N_True);
                 UnLink(MakePath(n_filename));
                 break;
            case 0x02:
                 sprintf(n_filename, "jbgd%02d.jim", n_mc.background);
                 DeCompressData(n_filename);
                 break;
            default: break;
           }
           delete(n_filename);
}

void far YesNo00(WindowData far *n_wd)
{
           MC(N_False);
           drwfillbox(XOR, 11,  n_wd->x1+17, n_wd->y1+57, n_wd->x1+163, n_wd->y1+93);
           drwfillbox(XOR, 11,  n_wd->x1+20, n_wd->y1+60, n_wd->x1+160, n_wd->y1+90);
           drwfillbox(XOR, 11, n_wd->x1+177, n_wd->y1+57, n_wd->x1+323, n_wd->y1+93);
           drwfillbox(XOR, 11, n_wd->x1+180, n_wd->y1+60, n_wd->x1+320, n_wd->y1+90);
           MC(N_True);
}

signed int far YesNo(signed char far *n_string, signed char n_default)
{
 WindowData n_wd;
 signed char Return=n_default;
 signed int n_key;
 signed int n_count;
 signed char far *n_yesstring="µ(Accept)";
 signed char far *n_nostring="´a“¡µ¡(Reject)";
 unsigned int n_yesnohandle=himemAlloc((unsigned int)((ComputeWindow(150, 200, 490, 300)>>10)+30L));
           DeCompressData("jack08.jim");
           n_wd.x1=150;n_wd.y1=200;n_wd.x2=490;n_wd.y2=300;n_wd.swaphandle=n_yesnohandle;n_wd.command=WIN_NONE;
           Help(n_string); MC(N_False);
           GetImageXMS(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, n_yesnohandle);
           PutImageDisk(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, "jackswap\\jack08.jim", 0);
           UnLink(MakePath("jackswap\\jack08.jim"));
           Puts(n_wd.x1+42, n_wd.y1+5, "»©¢… ?", WHITE, N_Default);
           PopRec(n_wd.x1+20, n_wd.y1+60, n_wd.x1+160, n_wd.y1+90);
           PopRec(n_wd.x1+180, n_wd.y1+60, n_wd.x1+320, n_wd.y1+90);
           PopRec(n_wd.x1+21, n_wd.y1+61, n_wd.x1+159, n_wd.y1+89);
           PopRec(n_wd.x1+181, n_wd.y1+61, n_wd.x1+319, n_wd.y1+89);
           Puts(n_wd.x1+55, n_wd.y1+68, n_yesstring, WHITE, N_Default);
           Puts(n_wd.x1+196, n_wd.y1+68, n_nostring, WHITE, N_Default);
           Puts(n_wd.x1+54, n_wd.y1+67, n_yesstring, BLACK, N_Default);
           Puts(n_wd.x1+195, n_wd.y1+67, n_nostring, BLACK, N_Default);
           n_count=n_wd.x1+(((n_wd.x2-n_wd.x1)>>1)-((strlen(n_string)>>1)<<3));
           Puts(n_count+1, n_wd.y1+31, n_string, WHITE, N_Default);
           Puts(n_count, n_wd.y1+30, n_string, BLACK, N_Default);
           drwline(SET, BLACK, n_count, n_wd.y1+48, n_count+(strlen(n_string)<<3), n_wd.y1+48);
           drwline(SET, BLACK, n_count, n_wd.y1+50, n_count+(strlen(n_string)<<3), n_wd.y1+50);
           if(Return==N_Yes)
           {
            drwfillbox(XOR, 11, n_wd.x1+17, n_wd.y1+57, n_wd.x1+163, n_wd.y1+93);
            drwfillbox(XOR, 11, n_wd.x1+20, n_wd.y1+60, n_wd.x1+160, n_wd.y1+90);
           }
           else
           {
            drwfillbox(XOR, 11, n_wd.x1+177, n_wd.y1+57, n_wd.x1+323, n_wd.y1+93);
            drwfillbox(XOR, 11, n_wd.x1+180, n_wd.y1+60, n_wd.x1+320, n_wd.y1+90);
           }
           MC(N_True);
           yesno_loop:;
           n_key=0; MouseProcess(); TimeProcess(); WindowProcess(&n_wd);
           if(n_wd.command==WIN_CLOSE)
           {
            Return=N_Cancel;
            goto end_yesno_loop;
           }
           if(s_mb==N_RIGHTBUTTON)
           {
            signed char far *n_yesno_right[]={n_yesstring, n_nostring, "Âá­¡\(Cancel\)", NULL};
            switch(RightButtonMenu(n_yesno_right))
            {
             case 0x00: Return=N_Yes;    goto end_yesno_loop;
             case 0x01: Return=N_No;     goto end_yesno_loop;
             case 0x02: Return=N_Cancel; goto end_yesno_loop;
             default: break;
            }
           }
           if(MouseCheck(n_wd.x1+20, n_wd.y1+60, n_wd.x1+160, n_wd.y1+90, N_LEFTBUTTON)==N_True)
           {
            if(Return==N_No)
            {
             YesNo00(&n_wd);
             Return=N_Yes;
            }
            MC(N_False);
            PushRec(n_wd.x1+20, n_wd.y1+60, n_wd.x1+160, n_wd.y1+90); MC(N_True);
            while(s_mb==N_LEFTBUTTON){MouseProcess();TimeProcess();}
            MC(N_False); PopRec(n_wd.x1+20, n_wd.y1+60, n_wd.x1+160, n_wd.y1+90); MC(N_True);
            if(MouseCheck(n_wd.x1+20, n_wd.y1+60, n_wd.x1+160, n_wd.y1+90, N_NOBUTTON)==N_True)goto end_yesno_loop;
           }
           if(MouseCheck(n_wd.x1+180, n_wd.y1+60, n_wd.x1+320, n_wd.y1+90, N_LEFTBUTTON)==N_True)
           {
            if(Return==N_Yes)
            {
             YesNo00(&n_wd);
             Return=N_No;
            }
            MC(N_False);
            PushRec(n_wd.x1+180, n_wd.y1+60, n_wd.x1+320, n_wd.y1+90); MC(N_True);
            while(s_mb==N_LEFTBUTTON){MouseProcess();TimeProcess();}
            MC(N_False); PopRec(n_wd.x1+180, n_wd.y1+60, n_wd.x1+320, n_wd.y1+90); MC(N_True);
            if(MouseCheck(n_wd.x1+180, n_wd.y1+60, n_wd.x1+320, n_wd.y1+90, N_NOBUTTON)==N_True)goto end_yesno_loop;
           }
           if(kbhit())
           {
            n_key=getch();
            if(n_key==0||kbhit())
            {
             n_key=getch();
             switch(n_key)
             {
              case 0x4b: if(Return==N_No){YesNo00(&n_wd); Return=N_Yes;}
                   break;
              case 0x4d: if(Return==N_Yes){YesNo00(&n_wd); Return=N_No;}
                   break;
              default: break;
             }
             if(n_key==0x0f)n_key=0x09;
             else n_key=0;
            }
            switch(n_key)
            {
             case 0x09:
                   YesNo00(&n_wd);
                   if(Return==N_Yes)Return=N_No;
                   else Return=N_Yes;
                   break;
             case 0x1b: Return=N_Cancel; goto end_yesno_loop;
             case 'y':
             case 'Y':
             case 'a':
             case 'A': Return=N_Yes; goto end_yesno_loop;
             case 'n':
             case 'N':
             case 'r':
             case 'R': Return=N_No; goto end_yesno_loop;
             case 0x20:
             case 0x0d: goto end_yesno_loop;
             default: break;
            }
            while(kbhit())getch();
           }
           goto yesno_loop;
           end_yesno_loop:;
           MC(N_False);
           PutImageXMS(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, n_yesnohandle);
           MC(N_True); himemFree(n_yesnohandle);
           return(Return);
}

void far StorePal(void)
{
 FILE *fp;
 signed int n_count;
           if((fp=fopen(MakePath("jackswap\\~pal.swp"), s_WriteMode))==0)return;
           outportb(0x3c7, 0);
           for(n_count=0;n_count<768;n_count++)fputc(inportb(0x3c9), fp);
           fclose(fp);
}

void far ReStorePal(void)
{
 FILE *fp;
 signed int n_count;
           if((fp=fopen(MakePath("jackswap\\~pal.swp"), s_ReadMode))==0)return;
           outportb(0x3c8, 0);
           for(n_count=0;n_count<768;n_count++)outportb(0x3c9, fgetc(fp));
           fclose(fp);
           UnLink(MakePath("jackswap\\~pal.swp"));
}

void far System(void)
{
 union REGS Register;
 signed char far *n_comspec=getenv("COMSPEC");
 unsigned int n_systemhandle;
           Help("\`EXIT\'Ÿi ·³bÐa¯¡¡e ”a¯¡ •©´aµ©® ·¶¯s“¡”a.");
           n_systemhandle=himemAlloc(500);
           MC(N_False); GetImageXMS(0, 0, s_MaxX-1, s_MaxY-1, n_systemhandle); StorePal();
           Register.x.ax=0x0003; int86(0x10, &Register, &Register);
           printf("JACK & JACK software - Copyright(c)1997-1998\n");
           printf("Type \'EXIT\' to return to %s\n\n", s_Title);
           printf("Detect comspec - %s", n_comspec);
           CloseUpKey(); system(n_comspec); StartUpKey(); res800();
           ReStorePal(); PutImageXMS(0, 0, s_MaxX-1, s_MaxY-1, n_systemhandle);
           MC(N_True); himemFree(n_systemhandle);
}

void far PopMain(void)
{
           PopHelp();
           Menu(N_True);
           l_pop_label:;
           s_key=0; MouseProcess(); TimeProcess(); PopMouseProcess();
           if(s_scankey[0x38]&&s_scankey[0x2d])PopMenuRun(0, 3);
           if(s_scankey[0x38]&&s_scankey[0x1f])PopMenuRun(0, 2);
           if(s_scankey[0x38]&&s_scankey[0x10])goto l_end_pop_label;
           if(kbhit())
           {
            s_popmouseflag=N_False; s_key=getch();
            if(s_key==0||kbhit())
            {
             s_key=getch();
             switch(s_key)
             {
              case 0x47:
              case 0x49:
                   MC(N_False);
                   drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
                   s_popmenuy[s_popmenux]=0;
                   drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
                   MC(N_True); PopHelp();
                   break;
              case 0x4f:
              case 0x51:
                   MC(N_False);
                   drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
                   s_popmenuy[s_popmenux]=s_maxpopmenuy[s_popmenux]-1;
                   drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
                   MC(N_True); PopHelp();
                   break;
              case 0x48:
                   MC(N_False);
                   drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
                   s_popmenuy[s_popmenux]--;
                   if(s_popmenuy[s_popmenux]<0)s_popmenuy[s_popmenux]=s_maxpopmenuy[s_popmenux]-1;
                   drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
                   MC(N_True); PopHelp();
                   break;
              case 0x50:
                   MC(N_False);
                   drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
                   s_popmenuy[s_popmenux]++;
                   if(s_popmenuy[s_popmenux]>=s_maxpopmenuy[s_popmenux])s_popmenuy[s_popmenux]=0;
                   drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
                   MC(N_True); PopHelp();
                   break;
              case 0x4b:
                   PopMenu(N_False); s_popmenux--;
                   if(s_popmenux<0)s_popmenux=s_maxpopmenux-1;
                   PopMenu(N_True); PopHelp();
                   break;
              case 0x4d:
                   PopMenu(N_False); s_popmenux++;
                   if(s_popmenux>=s_maxpopmenux)s_popmenux=0;
                   PopMenu(N_True); PopHelp();
                   break;
              default: break;
             }
             if(s_key==0x0f)s_key=0x09;
             else s_key=0;
            }
            switch(s_key)
            {
             case 0x09:
                  if(s_scankey[0x2a]||s_scankey[0x36])
                  {
                   MC(N_False);
                   drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
                   s_popmenuy[s_popmenux]--;
                   if(s_popmenuy[s_popmenux]<0)s_popmenuy[s_popmenux]=s_maxpopmenuy[s_popmenux]-1;
                   drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
                   MC(N_True); PopHelp();
                   break;
                  }
                  MC(N_False);
                  drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
                  s_popmenuy[s_popmenux]++;
                  if(s_popmenuy[s_popmenux]>=s_maxpopmenuy[s_popmenux])s_popmenuy[s_popmenux]=0;
                  drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
                  MC(N_True); PopHelp();
                  break;
             case 0x20:
             case 0x0d:
                  PopMenuRun(s_popmenux, s_popmenuy[s_popmenux]);
                  break;
             default:   break;
            }
            while(kbhit())getch();
           }
           goto l_pop_label;
           l_end_pop_label:;
           Menu(N_False);
}

void far PopMouseProcess(void)
{
 signed int n_count, n_cc;
 signed char far *n_rightmenu[]={"•¡¶‘ i", "¡A“A®‘‹±", "•¡¯aa—i·¡", "Ïa¡‹aœ‘ ¹·ža", NULL};
           if(s_MD.IsMouse==N_False)return;
           if(s_popmouseflag==N_False)return;
           if(s_mb==N_RIGHTBUTTON)
           {
            switch(RightButtonMenu(n_rightmenu))
            {
             case 0x00: PopMenuRun(5, 0); break;
             case 0x01: PopMenuRun(0, 4); break;
             case 0x02: PopMenuRun(0, 2); break;
             case 0x03: PopMenuRun(0, 3); break;
             default: break;
            }
           }
           for(n_count=5, n_cc=0;n_count<639, n_cc<s_maxpopmenux;n_count+=95, n_cc++)
           {
            if(MouseCheck(n_count, 2, n_count+90, 23, N_Default)==N_True&&s_popmenux!=n_cc)
            {
             PopMenu(N_False); s_popmenux=n_cc; PopMenu(N_True); PopHelp(); break;
            }
           }
           for(n_count=0;n_count<s_maxpopmenuy[s_popmenux];n_count++)
           {
            if(MouseCheck(((signed int)(s_popmenux)*95)+8, (n_count*22)+33, ((signed int)(s_popmenux)*95)+152, (n_count*22)+54, N_Default)==N_True)//&&s_popmenuy[s_popmenux]!=n_count)
            {
             if(s_popmenuy[s_popmenux]!=n_count)
             {
              MC(N_False);
              drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
              s_popmenuy[s_popmenux]=n_count;
              drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
              MC(N_True); PopHelp();
              break;
             }
             else
             {
              if(s_mb==N_LEFTBUTTON)
              {
               while(s_mb==N_LEFTBUTTON){MouseProcess(); TimeProcess();}
               if(MouseCheck(((signed int)(s_popmenux)*95)+8, (n_count*22)+33, ((signed int)(s_popmenux)*95)+152, (n_count*22)+54, N_NOBUTTON)==N_True){PopMenuRun(s_popmenux, s_popmenuy[s_popmenux]);}
              }
             }
             break;
            }
           }
           s_popmouseflag=N_False;
}

void far PopMenu(signed char n_swich)
{
 signed int n_count;
           if(n_swich==N_True)
           {
            MC(N_False);
            PushRec((s_popmenux*95)+6, 3, (s_popmenux*95)+94, 22);
            Puts((s_popmenux*95)+10, 5, s_mainstring[s_popmenux], BLACK, N_Default);
            GetImageXMS(((signed int)(s_popmenux)*95)+5, 30, ((signed int)(s_popmenux)*95)+155, (s_maxpopmenuy[s_popmenux]*22)+35, s_popmenuhandle);
            drwbox(SET, BLACK, ((signed int)(s_popmenux)*95)+5, 30, ((signed int)(s_popmenux)*95)+155, (s_maxpopmenuy[s_popmenux]*22)+35);
            PopRec(((signed int)(s_popmenux)*95)+6, 31, ((signed int)(s_popmenux)*95)+154, (s_maxpopmenuy[s_popmenux]*22)+34);
            PushRec(((signed int)(s_popmenux)*95)+7, 32, ((signed int)(s_popmenux)*95)+153, (s_maxpopmenuy[s_popmenux]*22)+33);
            for(n_count=0;n_count<s_maxpopmenuy[s_popmenux];n_count++)
            {
             PopBox(((signed int)(s_popmenux)*95)+8, (n_count*22)+33, ((signed int)(s_popmenux)*95)+152, (n_count*22)+54, LIGHTGRAY);
             Puts(((signed int)(s_popmenux)*95)+12, (n_count*22)+37, s_popmenustring[s_popmenux][n_count], BLACK, N_Default);
             Puts(((signed int)(s_popmenux)*95)+131, (n_count*22)+38, s_popmenukey[s_popmenux][n_count], WHITE, N_Default);
             Puts(((signed int)(s_popmenux)*95)+130, (n_count*22)+37, s_popmenukey[s_popmenux][n_count], DARKGRAY, N_Default);
            }
            drwfillbox(XOR, N_XORPOPCOLOR, ((signed int)(s_popmenux)*95)+8, (s_popmenuy[s_popmenux]*22)+33, ((signed int)(s_popmenux)*95)+152, (s_popmenuy[s_popmenux]*22)+54);
            MC(N_True);
           }
           else
           {
            MC(N_False);
            PutImageXMS(((signed int)(s_popmenux)*95)+5, 30, ((signed int)(s_popmenux)*95)+155, (s_maxpopmenuy[s_popmenux]*22)+35, s_popmenuhandle);
            Puts((s_popmenux*95)+10, 5, s_mainstring[s_popmenux], DARKGRAY, N_Default);
            PopRec((s_popmenux*95)+6, 3, (s_popmenux*95)+94, 22);
            MC(N_True);
           }
}

void far Menu(signed char n_swich)
{
 signed int n_count, n_cc;
           if(n_swich==N_True)
           {
            s_menuhandle=himemAlloc(30); s_popmenuhandle=himemAlloc(90); MC(N_False);
            GetImageXMS(0, 0, 639, 26, s_menuhandle);
            drwfillbox(SET, BLUE, 0, 0, 639, 25);
            NonPutImageDisk(571, -2, 602, 29, "jackswap\\jack04.jim", 32, 32);
            Puts(602, 7, "&", YELLOW, N_Default);
            NonPutImageDisk(609, -2, 640, 29, "jackswap\\jack04.jim", 32, 32);
            drwline(SET, WHITE, 0, 26, 639, 26);
            for(n_count=5, n_cc=0, s_maxpopmenux=0;n_count<639;n_count+=95, n_cc++)
            {
             if(s_mainstring[n_cc]==0)break;
             PutImageDisk(n_count+2, 4, n_count+88, 21, "jackswap\\jack06.jim", 16);
             drwbox(SET, BLACK, n_count, 2, n_count+90, 23);
             PopRec(n_count+1, 3, n_count+89, 22);
             Puts(n_count+6, 6, s_mainstring[n_cc], WHITE, N_Default);
             Puts(n_count+5, 5, s_mainstring[n_cc], DARKGRAY, N_Default);
             s_maxpopmenuy[n_cc]=0;
             while(s_popmenustring[n_cc][s_maxpopmenuy[n_cc]]){s_maxpopmenuy[n_cc]++;}
             s_maxpopmenux++;
            }
            MC(N_True); PopMenu(N_True);
           }
           else
           {
            PopMenu(N_False); MC(N_False); PutImageXMS(0, 0, 639, 26, s_menuhandle);
            MC(N_True); himemFree(s_popmenuhandle); himemFree(s_menuhandle);
           }
}

void far Check(signed char far *n_string)
{
 WindowData n_wd;
 signed int n_count;
 signed char far *n_okstring="ÑÂ  ·¥ \(ENTER\)";
 unsigned int n_checkhandle=himemAlloc((unsigned int)((ComputeWindow(150, 200, 490, 300)>>10)+30L));
           DeCompressData("jack08.jim");
           n_wd.x1=150;n_wd.y1=200;n_wd.x2=490;n_wd.y2=300;n_wd.swaphandle=n_checkhandle;n_wd.command=WIN_NONE;
           Help(n_string); MC(N_False);
           GetImageXMS(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, n_checkhandle);
           PutImageDisk(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, "jackswap\\jack08.jim", 0);
           UnLink(MakePath("jackswap\\jack08.jim"));
           n_count=n_wd.x1+(((n_wd.x2-n_wd.x1)>>1)-((strlen(n_string)>>1)<<3));
           Puts(n_wd.x1+42, n_wd.y1+5, "ÑÂ·¥·i Ðº­A¶a.", WHITE, N_Default);
           Puts(n_count+1, n_wd.y1+31, n_string, WHITE, N_Default);
           Puts(n_count, n_wd.y1+30, n_string, BLACK, N_Default);
           drwline(SET, BLACK, n_count, n_wd.y1+48, n_count+(strlen(n_string)<<3), n_wd.y1+48);
           drwline(SET, BLACK, n_count, n_wd.y1+50, n_count+(strlen(n_string)<<3), n_wd.y1+50);
           PopRec(n_wd.x1+100, n_wd.y1+60, n_wd.x1+240, n_wd.y1+90);
           PopRec(n_wd.x1+101, n_wd.y1+61, n_wd.x1+239, n_wd.y1+89);
           Puts(n_wd.x1+116, n_wd.y1+69, n_okstring, WHITE, N_Default);
           Puts(n_wd.x1+115, n_wd.y1+68, n_okstring, BLACK, N_Default);
           MC(N_False);
           while(N_True)
           {
            MouseProcess();
            TimeProcess();
            WindowProcess(&n_wd);
            if(n_wd.command==WIN_CLOSE)break;
            if(MouseCheck(n_wd.x1+100, n_wd.y1+60, n_wd.x1+240, n_wd.y1+90, N_LEFTBUTTON)==N_True)
            {
             MC(N_False);
             PushRec(n_wd.x1+100, n_wd.y1+60, n_wd.x1+240, n_wd.y1+90);
             MC(N_True);
             while(s_mb){MouseProcess(); TimeProcess();}
             MC(N_False);
             PopRec(n_wd.x1+100, n_wd.y1+60, n_wd.x1+240, n_wd.y1+90);
             MC(N_True);
             if(MouseCheck(n_wd.x1+100, n_wd.y1+60, n_wd.x1+240, n_wd.y1+90, N_NOBUTTON)==N_True)break;
            }
            if(kbhit())
            {
             n_count=getch();
             while(kbhit())getch();
             if(n_count==0x1b||n_count==0x0d||n_count==0x20)break;
            }
           }
           while(kbhit())getch();
           MC(N_False);
           PutImageXMS(n_wd.x1, n_wd.y1, n_wd.x2, n_wd.y2, n_checkhandle);
           MC(N_True);
}

/* End of source */