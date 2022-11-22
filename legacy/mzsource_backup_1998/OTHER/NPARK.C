#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>

typedef unsigned int           word;

void main(void)
{
 word  drive=0x80;
 word  count;
 word  park_ok=NULL;
 unsigned char p=0;

 union REGS rin;
 char c1=42,c2=0;
	   clrscr();
	   printf("        *******        ***           *******        *****  *** [Advertisement] \n");
	   printf("        **    **      ** **           **   **        **  **        Ndir-II\n");
	   printf("       **     **     **   **         **     **      **  **          NPLUS\n");
	   printf("      **     **     **    **        **     **      ** **            NSUNG\n");
	   printf("     ** *****      ** *** **       ** *****       **   **          JOHWE-2\n");
	   printf("   ***           ***     ***     ***    **      ***     **          NOTICE\n");
	   printf(" *****         *****    *****  *****    ****  *****     ***        ( Park )\n\n");
	   textattr(WHITE);
	   textbackground(BLUE);
	   cprintf("\n\r                        Freeware Version Collection                            ");
	   textattr(LIGHTGRAY);
	   textbackground(BLACK);
	   cprintf("\n\r");
	   puts("\n                 NOTICE's Service Utility : Parking Utility");
	   puts("\n                   [ by JaeHyuk.CHO(Pager012-846-6420) ]");
	   puts("\nNownuri ID: kobonch , Interpia ID: NOTICE , KIS ID: NOTICE , HAN ID: NOTICE\n\n\n");
	   rin.x.ax=0x800;
	   rin.x.cx=NULL;
	   rin.x.dx=drive;
	   int86(0x13,&rin,&rin);
	   count=rin.h.dl;
	   if(rin.x.cflag!=NULL||count==NULL)return;
	   else{
		if(count>2)count=2;
		do{
		   rin.x.ax=0x1100;
		   rin.x.cx=NULL;
		   rin.x.dx=drive;
		   int86(0x13,&rin,&rin);
		   if(rin.x.cflag!=NULL)goto park__;
		   park_ok=0x80;
		   rin.x.ax=0x0c00;
		   rin.x.cx=NULL;
		   rin.x.dx=drive;
		   int86(0x13,&rin,&rin);
		   rin.x.ax=0x0800;
		   rin.x.cx=NULL;
		   rin.x.dx=drive;
		   int86(0x13,&rin,&rin);
		   rin.x.ax=0x0c00;
		   rin.x.dx=drive;
		   int86(0x13,&rin,&rin);
		   park__:
		   drive++;
		   count--;
		   }while(count);
		if(park_ok!=0x80)return;
		}
	   sound(100);
	   delay(500);
	   sound(200);
	   delay(500);
	   sound(300);
	   delay(500);
	   nosound();
	   puts("\n                  ***  Parking is completed : safety ***");
	   puts("\n                          Turn off your computer");
	   printf("\n                   Press any key to reset Hard-disk. [ ]\b");
	   while(!kbhit()){
			   rin.x.ax=0x1010;
			   rin.x.bx=LIGHTGRAY;
			   rin.h.ch=rin.h.cl=rin.h.dh=c1;
			   int86(0x10,&rin,&rin);
			   if(c2==0){
				     c1--;
				     }
			   else{
				c1++;
				}
			   if(c1<10){
				    c1=10;
				    c2=1;
				    }
			   if(c1>42){
				     c1=42;
				     c2=0;
				     }
			   delay(200);
			   switch(p){
				     case 0:
					  printf("\b\\");
					  break;
				     case 1:
					  printf("\b|");
					  break;
				     case 2:
					  printf("\b/");
					  break;
				     case 3:
					  printf("\b-");
					  break;
				     }
			   p++;
			   if(p>3)p=0;
			   }
	   while(kbhit())getch();
	   rin.x.ax=0x1010;
	   rin.x.bx=LIGHTGRAY;
	   rin.h.ch=rin.h.cl=rin.h.dh=42;
	   int86(0x10,&rin,&rin);

	   rin.h.ah=0x00;
	   rin.h.dl=0x80;
	   int86(0x13,&rin,&rin);
	   rin.h.ah=0x08;
	   rin.h.dl=0x81;
	   int86(0x13,&rin,&rin);
	   if(!rin.x.cflag){
			    rin.h.ah=0x00;
			    rin.h.dl=0x81;
			    int86(0x13,&rin,&rin);
			    }
	   puts("\n\n\n\nHard-disk reset complete...\n");
}
