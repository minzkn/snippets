/*
  JACK's JF.CPP
*/

#include <dos.h>
#include <bios.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <alloc.h>

#include "common.h"
#include "svgacc.h"
#include "graph.h"
#include "himem.h"
#include "menu.h"
#include "timer.h"

void far setrgb(signed int n_r, signed int n_g, signed int n_b)
{
	    outportb(0x3c9, n_r);
	    outportb(0x3c9, n_g);
	    outportb(0x3c9, n_b);
}

void far Saver1(signed int n_sw)
{
 signed int n_count;
 unsigned char far *n_video=(unsigned char far *)0xa0000000L;
 unsigned char far *n_ptr;
 unsigned int n_cc;
 signed int n_key=0;
 signed int n_temp;
 signed char far *n_str="Copyright(c)1998 JACK & JACK by JaeHyuk";
 signed char far *n_string;
 signed int n_xc;
 signed int n_ccount;
 union REGS Register;
     res320();
	   outportb(0x3c8, 0);
     /*for(n_count=0;n_count<64;n_count++)setrgb(0, 0, n_count);
	   for(n_count=0;n_count<64;n_count++)setrgb(0, n_count, 63);
	   for(n_count=0;n_count<64;n_count++)setrgb(n_count, 63, 63);
	   for(n_count=0;n_count<64;n_count++)setrgb(63, 63, 63);*/

     for(n_count=0;n_count<64;n_count++)setrgb(n_count, 0, 0);
	   for(n_count=0;n_count<64;n_count++)setrgb(63, n_count, 0);
     for(n_count=0;n_count<64;n_count++)setrgb(63, 63, n_count);
	   for(n_count=0;n_count<64;n_count++)setrgb(63, 63, 63);
     while(kbhit())getch();
	   while(1)
	   {
      if(s_MD.IsMouse){Register.x.ax=0x0003; int86(0x33, &Register, &Register);}
      else Register.x.bx=0;
      if(kbhit()||n_key>0||Register.x.bx)
	    {
	     n_key++;
	     while(kbhit())getch();
	    }
	    if(n_key>50)break;
      if(n_sw==N_True)
      {
       n_string=n_str;
       n_xc=3;
       n_ccount=0;
       while(*n_string)
       {
        EngPutc(n_xc, 95, *(n_string++), getpoint(n_ccount, 170), N_Default);
        n_ccount++;
        n_xc+=8;
       }
      }
	    for(n_cc=(unsigned int)41600L, n_ptr=n_video+(unsigned int)41600L;n_cc<(unsigned int)64000L;n_cc++, n_ptr++)
	    {
       n_temp=n_cc+1537;
       if(n_temp>0)n_temp=0;
	     if(*(n_ptr+320)>1)
	     {
        n_count=((*(n_ptr+319)+*(n_ptr+320)+*(n_ptr+321)+*(n_ptr))>>2)-(n_key>>2)-3;
	      if(n_count<1)n_count=0;
	      *(n_ptr-320)=*(n_video-n_temp)=*(n_ptr)=(unsigned char)n_count;
	     }
       else {*(n_video-n_temp)=*(n_ptr-1)=*(n_ptr)=0;}
	    }
	    n_ptr=n_video+(unsigned int)64000L;
      for(n_count=0;n_count<100;n_count++)*(n_ptr+random(320))=random(190)+10;
	   }
	   while(kbhit())getch();
}

void far Saver2(signed int n_sw)
{
           if(n_sw);
}

/*void far Saver2(signed int n_sw)
{
 union REGS Register;
 signed int n_count;
 signed int pl1, pl2;
 signed char far *data=(signed char far *)farmalloc(65536L);
           if(data==NULL)return;
           res320();
           for(float x=0.0; x<256.0; x++)
           {
	          for(float y=0.0; y<256.0; y++)
            {
             data[x+256*y]=83*(3+(cos(0.09*sqrt((x-128)*(x-128)+(y-128)*(y-128))))+cos(x/21.0)+cos(y/26.0));
            }
           }
           outportb(0x3c8,0);
	         for (n_count=0; n_count<128; n_count++)setrgb(0, 0, (n_count>>1)>62?62:n_count>>1);
           for (n_count=128; n_count>0; n_count--)setrgb(0, 0, (n_count>>1)>62?62:n_count>>1);
	         for (float teller=0;;teller++)
	         {
		        pl1=48+cos(teller/37)*47.0+256*(int)(48+47*(sin(teller/31)));
		        pl2=48+sin(teller/24)*47.0+256*(int)(48+47*(cos(teller/19)))-pl1;
            asm push 0xa000;
            asm pop es
   		      asm xor di,di
 		        asm push ds
		        asm lds si,data
		        asm add si,pl1
		        asm mov bx,pl2
		        asm pusha
		        asm mov cl,100
            l1: asm mov dx,160
            l2: asm lodsb
		        asm add al,[si+bx]
		        asm mov ah,al
		        asm mov es:[di+320],ax
		        asm stosw
	     	    asm dec dx
		        asm jnz l2
		        asm add si,256-160
		        asm add di,320
		        asm dec cl
		        asm jnz l1
		        asm popa
		        asm pop ds
            if(n_sw==N_True)EngPuts(3, 182, "JACK & JACK", 0, N_Default);
            if(teller>720.0)teller=0L;
            Register.x.ax=0x0003; int86(0x33, &Register, &Register);
		        if(kbhit()||Register.x.bx)break;
	         }
	         farfree(data);
}*/

void far Saver(signed int n_sw)
{
 struct time n_time;
 unsigned int n_handle;
           n_handle=himemAlloc(512);
           MC(N_False);
           StorePal();
           GetImageXMS(0, 0, s_MaxX, s_MaxY, n_handle);
           gettime(&n_time);
           switch(n_sw)
           {
            case 0x00: Saver1(N_True); break;
            case 0x01: Saver2(N_True); break;
            default:
                 if(rand()%2)Saver1(N_True);
                 else Saver2(N_True);
                 break;
           }
           res800();
           ReStorePal();
           PutImageXMS(0, 0, s_MaxX, s_MaxY, n_handle);
           himemFree(n_handle);
           MC(N_True);
}

/* End of source */