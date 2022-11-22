/*
  JACK's COM.CPP
*/

#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include "common.h"
#include "graph.h"
#include "autohan.h"
#include "himem.h"
#include "timer.h"
#include "menu.h"

#define N_COMBUFFER 0x1000

void interrupt ComI(void);

class ComClass{ /* Linking graph.cpp */
               unsigned char c_color, c_backcolor;
               signed int c_x, c_y;
               signed int c_baseaddr;
               signed int c_interrupt;
               signed int c_maskon;
               signed int c_maskoff;
               signed char far *c_buffer;
               signed int c_first, c_last;
               void interrupt (far *c_oldcom)(void);
               public:
               void far SetColor(unsigned char n_color){ComClass::c_color=n_color;}
               void far SetBackColor(unsigned char n_color){ComClass::c_backcolor=n_color;}
               unsigned char far GetColor(void){return(ComClass::c_color);}
               unsigned char far GetBackColor(void){return(ComClass::c_backcolor);}
               signed int far GetX(void){return(ComClass::c_x);}
               signed int far GetY(void){return(ComClass::c_y);}
               void far GotoXY(signed int n_x, signed int n_y){ComClass::c_x=n_x; ComClass::c_y=n_y;}
               void far InitSerialPort(signed int n_port, unsigned char n_databit, unsigned char n_stopbit, unsigned char n_parity, signed long n_baud);
               void far StartModem(void);
               void far CloseModem(void);
               void far Cominterrupt(void);
               void far SendCharacter(signed char n_char);
               void far SendString(signed char far *n_string);
               signed int far IsReadyToReceive(void);
               signed char ReceiveCharacter(void);
               void far FlushBuffer(void);
               void far SendHayesCommand(signed char far *n_command);

               void far PutChar(signed char n_char);
              }ComClass;

void far Com(void)
{
 signed int n_key;
           ComClass.InitSerialPort(3, 3, 0, 0, 19200L);
           ComClass.StartModem();
           MC(N_False);
           drwfillbox(SET, ComClass.GetBackColor(), 0, 0, 639, 456);
           PopBox(0, 457, 639, 479, LIGHTGRAY);
           PushRec(2, 459, 637, 477);
           DeCompressData("jack0a.jim");
           NonPutImageDisk(0, 30, 639, 99, "jackswap\\jack0a.jim", 96, 96);
           UnLink(MakePath("jackswap\\jack0a.jim"));
           MC(N_True);
           ComClass.SendHayesCommand("atz\x0d");
           Help("¡¡•Q·i Á¡‹¡ÑÁ Ða“e•A ¬÷‰·Ð–¯s“¡”a.");
           ComClass.FlushBuffer();
           com_loop:;
           n_key=0;
           MouseProcess();
           TimeProcess();
           if(ComClass.IsReadyToReceive())
           {
            ComClass.PutChar(ComClass.ReceiveCharacter());
           }
           if(s_scankey[0x38]&&s_scankey[0x2d])
           {
            s_scankey[0x2d]=N_False;
            if(YesNo("É·¯¥‹¡“w·i ¹·žaÐa¯¡‰V¯s“¡Œa?", N_No)==N_Yes)goto end_com;
           }
           if(kbhit())
           {
            n_key=getch();
            if(n_key==0||kbhit())
            {
             n_key=getch();
             switch(n_key)
             {
              case 0x00: break;
              default: break;
             }
             n_key=0;
            }
            switch(n_key)
            {
             case 0x00: break;
             default:
                  ComClass.SendCharacter(n_key);
                  break;
            }
            while(kbhit())getch();
           }
           goto com_loop;
           end_com:;
           ComClass.CloseModem();
           BackGround(2);
           BackGround(0);
}

void far ComClass::PutChar(signed char n_char)
{
 static signed char n_hanflag=N_False;
 static union HanCode n_hancode;
           switch(n_char)
           {
            case 0x00: break;
            case 0x0d: break;
            case 0x0a:
                 ComClass::c_x=0;
                 ComClass::c_y++;
                 if(ComClass::c_y>27)
                 {
                  MC(N_False);
                  scrollup(0, 0, 639, 456, 16, ComClass::GetBackColor());
                  MC(N_True);
                  ComClass::c_y=27;
                 }
                 break;
            default:
                 if((n_char&0x80)==0x80&&n_hanflag==N_False)
                 {
                  n_hancode.ByteData[1]=(unsigned char)n_char;
                  n_hanflag=N_True;
                 }
                 else if(n_hanflag==N_True)
                 {
                  n_hancode.ByteData[0]=(unsigned char)n_char;
                  n_hanflag=N_False;
                  if(ComClass::c_x>78)
                  {
                   ComClass::c_x=0;
                   ComClass::c_y++;
                   if(ComClass::c_y>27)
                   {
                    MC(N_False);
                    scrollup(0, 0, 639, 456, 16, ComClass::GetBackColor());
                    MC(N_True);
                    ComClass::c_y=27;
                   }
                  }
                  KS2JO(&n_hancode.WordData);
                  MC(N_False);
                  HanPutc(ComClass::c_x<<3, ComClass::c_y<<4, n_hancode, ComClass::GetColor(), ComClass::GetBackColor());
                  MC(N_True);
                  ComClass::c_x+=2;
                  if(ComClass::c_x>79)
                  {
                   ComClass::c_x=0;
                   ComClass::c_y++;
                   if(ComClass::c_y>27)
                   {
                    MC(N_False);
                    scrollup(0, 0, 639, 456, 16, ComClass::GetBackColor());
                    MC(N_True);
                    ComClass::c_y=27;
                   }
                  }
                 }
                 else
                 {
                  MC(N_False);
                  EngPutc(ComClass::c_x<<3, ComClass::c_y<<4, n_char, ComClass::GetColor(), ComClass::GetBackColor());
                  MC(N_True);
                  ComClass::c_x++;
                  if(ComClass::c_x>79)
                  {
                   ComClass::c_x=0;
                   ComClass::c_y++;
                   if(ComClass::c_y>27)
                   {
                    MC(N_False);
                    scrollup(0, 0, 639, 456, 16, ComClass::GetBackColor());
                    MC(N_True);
                    ComClass::c_y=27;
                   }
                  }
                 }
                 break;
           }
}

void far ComClass::FlushBuffer(void)
{
           while(ComClass::c_first!=ComClass::c_last)
           {
            if(ComClass::c_first==N_COMBUFFER)ComClass::c_first=0;
            ComClass::PutChar(*(ComClass::c_buffer+ComClass::c_first++));
           }
}

void far ComClass::SendHayesCommand(signed char far *n_command)
{
           ComClass::SendString(n_command);
           while(!ComClass::IsReadyToReceive());
           ComClass::FlushBuffer();
}

void far ComClass::SendCharacter(signed char n_char)
{
 signed char n_comp;
           do{
              n_comp=inport(ComClass::c_baseaddr+5)&0x20;
             }while(!n_comp);
           outportb(ComClass::c_baseaddr, n_char);
}

void far ComClass::SendString(signed char far *n_string)
{
           while(*n_string)SendCharacter(*(n_string++));
}

signed int far ComClass::IsReadyToReceive(void)
{
           if(ComClass::c_first==ComClass::c_last)return(N_False);
           return(N_True);
}

signed char far ComClass::ReceiveCharacter(void)
{
           if(ComClass::c_first==N_COMBUFFER)ComClass::c_first=0;
           return(*(ComClass::c_buffer+ComClass::c_first++));
}

void interrupt ComI(void)
{
           ComClass.Cominterrupt();
}

void far ComClass::StartModem(void)
{
 struct vect{signed char far *vect;} far *n_vect=(struct vect far *)(ComClass::c_interrupt<<2);
           outportb(ComClass::c_baseaddr+4, 0x0b);
           outportb(ComClass::c_baseaddr+1, 0x01);
           ComClass::c_first=ComClass::c_last=0;
           ComClass::c_buffer=new(signed char far [N_COMBUFFER]);
           ComClass::c_x=0; ComClass::c_y=10; ComClass::c_color=WHITE; ComClass::c_backcolor=BLUE;
           (void far *)(ComClass::c_oldcom)=(void far *)(n_vect->vect);
           (void far *)(n_vect->vect)=(void far *)(ComI);
           outportb(ComClass::c_baseaddr+6, inportb(ComClass::c_baseaddr+6)|0x30);
           outportb(0x21, inportb(0x21)&ComClass::c_maskon);
           delay(200);
           inportb(ComClass::c_baseaddr);
           inportb(ComClass::c_baseaddr);
}

void far ComClass::CloseModem(void)
{
 struct vect{signed char far *vect;} far *n_vect=(struct vect far *)(ComClass::c_interrupt<<2);
           outportb(ComClass::c_baseaddr+1, 0x00);
           outportb(0x21, inportb(0x21)|ComClass::c_maskoff);
           (void far *)n_vect->vect=(void far *)ComClass::c_oldcom;
           delete(ComClass::c_buffer);
}

void ComClass::Cominterrupt(void)
{
 signed char n_char;
           if(ComClass::c_last==N_COMBUFFER)ComClass::c_last=0;
           n_char=(signed char)inportb(ComClass::c_baseaddr);
           if(n_char)*(ComClass::c_buffer+ComClass::c_last++)=n_char;
           outportb(0x20, 0x20);
}

void far ComClass::InitSerialPort(signed int n_port, unsigned char n_databit, unsigned char n_stopbit, unsigned char n_parity, signed long n_baud)
{
 unsigned int n_div;
           switch(n_port)
           {
            case 0x00: // COM1
                 ComClass::c_baseaddr=0x3f8; ComClass::c_interrupt=0x0c;
                 ComClass::c_maskon=0xef; ComClass::c_maskoff=0x10;
                 break;
            case 0x01: // COM2
                 ComClass::c_baseaddr=0x2f8; ComClass::c_interrupt=0x0b;
                 ComClass::c_maskon=0xf7; ComClass::c_maskoff=0x08;
                 break;
            case 0x02: // COM3
                 ComClass::c_baseaddr=0x3e8; ComClass::c_interrupt=0x0c;
                 ComClass::c_maskon=0xef; ComClass::c_maskoff=0x10;
                 break;
            case 0x03: // COM4
                 ComClass::c_baseaddr=0x2e8; ComClass::c_interrupt=0x0b;
                 ComClass::c_maskon=0xf7; ComClass::c_maskoff=0x08;
                 break;
            default: break;
           }
           outportb(c_baseaddr+3, n_databit|n_stopbit|n_parity);
           n_div=(unsigned int)(1843200L/(n_baud<<4L));
           outportb(ComClass::c_baseaddr+3, inportb(ComClass::c_baseaddr+3)|0x80);
           outportb(ComClass::c_baseaddr, (unsigned char)n_div);
           outportb(ComClass::c_baseaddr+1, (unsigned char)(n_div>>8));
           outportb(ComClass::c_baseaddr+3, inportb(ComClass::c_baseaddr+3)&0x7f);
}

/* End of source */
