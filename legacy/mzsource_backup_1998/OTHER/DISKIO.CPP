#include<stdlib.h>
#include<conio.h>
#include<malloc.h>
#include<stdio.h>
#include<dos.h>
#include<mem.h>
#include<process.h>

#define DEFAULT_DELAY  500
#define DEFAULT_TIMER_SET  70

unsigned char far *buffer = (unsigned char far *) malloc(512);
unsigned char far *data02 = (unsigned char far *) 0x0040003e;

char g_track = 0;

void interrupt new_int0eh(...);
void interrupt (*old_int0eh)(...);
void interrupt new_int08h(...);
void interrupt (*old_int08h)(...);

void Floopy_Seek(void);
void MainScreen(void);
void PutStatus(void);
void SetMemoryAddress( unsigned char far *);
void GetPortData(int);

unsigned char Floppy_Getdata(void)
{
   while( ((inportb(0x3f4) & 128) == 0) )
   {
       if ( kbhit() )
	  if( getch() == 27 ) break;
   }
   // 3f4의 최상위 비트(준비) 비트가 1일때 까지 대기를 함.
   return inportb(0x3f5);
}
void interrupt new_int0eh(...)
{
//   outportb(0x61,3);
   *((unsigned char far *)(0xb8000000)+(160*13)+120) = '0';
   *((unsigned char far *)(0xb8000000)+(160*13)+122) = 'e';
   *((unsigned char far *)(0xb8000000)+(160*13)+124) = 'h';
   *((unsigned char far *)(0xb8000000)+(160*13)+126) = ' ';
   *((unsigned char far *)(0xb8000000)+(160*13)+128) = 'c';
   *((unsigned char far *)(0xb8000000)+(160*13)+130) = 'a';
   *((unsigned char far *)(0xb8000000)+(160*13)+132) = 'l';
   *((unsigned char far *)(0xb8000000)+(160*13)+134) = 'l';
   *((unsigned char far *)(0xb8000000)+(160*13)+136) = '.';
   (*old_int0eh)();
}
void interrupt new_int08h(...)
{
   *((unsigned char far *)(0xb8000000)+(160*14)+120) = '0';
   *((unsigned char far *)(0xb8000000)+(160*14)+122) = '8';
   *((unsigned char far *)(0xb8000000)+(160*14)+124) = 'h';
   *((unsigned char far *)(0xb8000000)+(160*14)+126) = ' ';
   *((unsigned char far *)(0xb8000000)+(160*14)+128) = 'c';
   *((unsigned char far *)(0xb8000000)+(160*14)+130) = 'a';
   *((unsigned char far *)(0xb8000000)+(160*14)+132) = 'l';
   *((unsigned char far *)(0xb8000000)+(160*14)+134) = 'l';
   *((unsigned char far *)(0xb8000000)+(160*14)+136) = '.';
   (*old_int08h)();
}
void MainScreen(void)
{
   register int i;
   textattr( (1 << 4) | 15 );
   gotoxy(1,1);
   cprintf("   MINZ technologies 1998 (C) ");
   textattr( (1 << 4) | 14 );
   cprintf("  |  Floopy Disk Controller program  |   A Drive    ");
   for(i=1;i<=23;i++)
   {
      gotoxy(1,1+i);
      textattr( (5 << 4) | 15 );
      cprintf("                               ");
      textattr( (7 << 4) | 15 );
      cprintf("                                                 ");
   }
   gotoxy(1,22);
   textattr( (1 << 4) | 14 );
   cprintf(" 1-Motor On 2-Sense interrupt status 3-Seek 4-Read 5-Recalibrate 6-Read ID      ");
   cprintf(" 7-Read deleted data                                                            ");
   cprintf(" ESC-Exit P-Ports view M-Memory view I-Initilize at 40:3E C-Clear 0-Reset port  ");
   return;
}
void PutStatus(void)
{
   register int i,i2;
   unsigned int portnd[11] = { 0x3f0 , 0x3f1 , 0x3f2 , 0x3f3 , 0x3f4
			      , 0, 0, 0 , 0 , 0x3f6
//                            ,0x3f5 , 0x3f5 , 0x3f5 , 0x3f5 , 0x3f6
			      ,0x3f7 };
   unsigned char bitmask[8] = { 128,64,32,16,8,4,2,1 };
   unsigned char portn[11];

   portn[0] = inportb(0x3f0);
   portn[1] = inportb(0x3f1);
   portn[2] = inportb(0x3f2);
   portn[3] = inportb(0x3f3);
   portn[4] = inportb(0x3f4);
   portn[5] = inportb(0);
   portn[6] = inportb(0);
   portn[7] = inportb(0);
   portn[8] = inportb(0);
   portn[9] = inportb(0x3f6);
   portn[10] = inportb(0x3f7);

   textattr( (5 << 4) | 15 );
   gotoxy(16,2);
   cprintf("7 6 5 4 3 2 1 0");
   for( i=0 ; i<=10 ; i++ )
   {
      gotoxy(1,i+3);
      textcolor(15);
      cprintf(" Port%3x %3x - ",portnd[i],portn[i]);
      for( i2=0 ; i2<=7 ; i2++ )
      {
	 if( portn[i] & bitmask[i2] ) textcolor(14); else textcolor(8);
	 cprintf("%c ",223);
      }
   }
   textattr( (7 << 4) | 15 );
   if( portn[4] & 64 )
   {
      gotoxy( 32,13 );
      cprintf(" Transtype: controller -> system ");
   }
   else
   {
      gotoxy( 32,13 );
      cprintf(" Transtype: system -> controller ");
   }
   if( portn[4] & 16 )
      textcolor(14);
   else
      textcolor(8);
   gotoxy( 32,14 );
   cprintf(" Busy ");
   textcolor(15);
   return;
}
void DMAPorts_PutStatus(void)
{
   register int i,i2;
   unsigned int portnd[11] = { 0x04 , 0x04 , 0x05 , 0x05 , 0x08
			      , 0x09, 0x0a, 0x0b , 0x0c , 0x0d
			      , 0x0e };
   unsigned char bitmask[8] = { 128,64,32,16,8,4,2,1 };
   unsigned char portn[11];

/* portn[0] = inportb(0x04);
   portn[1] = inportb(0x04);
   portn[2] = inportb(0x05);
   portn[3] = inportb(0x05);
   portn[4] = inportb(0x08);
   portn[5] = inportb(0x09);
   portn[6] = inportb(0x0a);
   portn[7] = inportb(0x0b);
   portn[8] = inportb(0x0c);
   portn[9] = inportb(0x0d);
   portn[10] = inportb(0x0e); */

   textattr( (5 << 4) | 15 );
   gotoxy(16,2);
   cprintf("7 6 5 4 3 2 1 0");
   for( i=0 ; i<=10 ; i++ )
   {
      gotoxy(1,i+3);
      textcolor(15);
      cprintf(" Port%3x %3x - ",portnd[i],portn[i]);
      for( i2=0 ; i2<=7 ; i2++ )
      {
	 if( portn[i] & bitmask[i2] ) textcolor(14); else textcolor(8);
	 cprintf("%c ",223);
      }
   }
   textcolor(15);
   return;
}
void ViewMem( unsigned char far *buffer )
{
   unsigned char *str = "0123456789ABCDEF";
   register int i,i2,i3;
   i3 = 0;
   textattr( (7 << 4) | 15 );
   for( i=0 ; i<=9 ; i++)
   {
      gotoxy( 33,3+i );
      for( i2=0 ; i2<=15 ; i2++ )
      {
	 cprintf("%c",*(str+(*(buffer+i3) >> 4)));
	 cprintf("%c ",*(str+(*(buffer+i3++) & 15)));
      }
   }
   return;
}
void SetMemoryAddress( unsigned char far *buffer )
{
   unsigned long offset32 = ((unsigned long)(FP_SEG(buffer)) << 4) | FP_OFF(buffer);
   unsigned char a;
   unsigned char data[3];
   textattr( (7 << 4) | 15 );
   gotoxy( 33,2 );

   data[0] = offset32 & 0xff;
   data[1] = (offset32 >> 8) & 0xff;
   data[2] = (offset32 >> 16) & 0xff;

   cprintf("Seg:%5xh Off:%5xh  |  <%lx %x %x %x>\n",FP_SEG(buffer),FP_OFF(buffer),offset32,data[0],data[1],data[2]);
   ViewMem(buffer);

   outportb(0x0b,0x46);    // set DMA mode
   outportb(0x0c,0x46);

   outportb(0x04,data[0]); // send low address DMA
   outportb(0x04,data[1]); // send high address DMA
   outportb(0x81,data[2]); // send to DMA page register

   outportb(0x05,0xff);
   outportb(0x05,1);

   outportb(0x0a,0x02);    // DMA mask register.
   return;
}
void GetPortData(int count)
{
   register int i;
   unsigned char data;
   textattr( (5 << 4) | 15 );
   for( i=0 ; i<=count-1 ; i++ )
   {
      data = Floppy_Getdata();
      textattr( (5 << 4) | 15 );
      if( i==0 )
      {
	gotoxy(32,21);
	if( (data&192) == 0  )
	{
	  cprintf(" Command terminated successfully and mem view.   ");
	  ViewMem(buffer);
	  textattr( (5 << 4) | 15 );
	}
	if( (data&192) == 64  ) cprintf(" Command terminated abnormally.                  ");
	if( (data&192) == 128 ) cprintf(" Invaild command.                                ");
	if( (data&192) == 192 ) cprintf(" Terminated abnormally by change in ready signal.");
      }
      if( i==1 )
      {
	gotoxy(32,20);
	if( (data& 4 ) == 4  ) cprintf(" Sector ID not found.                            ");
	if( (data& 2 ) == 2  ) cprintf(" write protect detected during write.            ");
	if( (data& 1 ) == 1  ) cprintf(" ID address mark not found.                      ");
      }
      gotoxy(2,14+i);
      textcolor(15);
      cprintf("*3F5 :%3d(%2x)",data,data);
      gotoxy(16,14+i);
      if( data & 128 ) textcolor(14); else textcolor(8);
      cprintf("%c ",223);
      if( data & 64 ) textcolor(14); else textcolor(8);
      cprintf("%c ",223);
      if( data & 32 ) textcolor(14); else textcolor(8);
      cprintf("%c ",223);
      if( data & 16 ) textcolor(14); else textcolor(8);
      cprintf("%c ",223);
      if( data & 8 ) textcolor(14); else textcolor(8);
      cprintf("%c ",223);
      if( data & 4 ) textcolor(14); else textcolor(8);
      cprintf("%c ",223);
      if( data & 2 ) textcolor(14); else textcolor(8);
      cprintf("%c ",223);
      if( data & 1 ) textcolor(14); else textcolor(8);
      cprintf("%c ",223);
   }
   return;
}

void Floppy_Senddata( unsigned char data )
{
   while( ((inportb(0x3f4) & 128) == 0) )
   {
       if ( kbhit() )
	  if( getch() == 27 ) break;
   }
   // 3f4의 최상위 비트(준비) 비트가 1일때 까지 대기를 함.
   outportb(0x3f5,data);
}
char Floppy_Senddata2( unsigned char data )
{
   while( ((inportb(0x3f4) & 128) == 0) )
   {
       if ( kbhit() )
	  if( getch() == 27 ) break;
   }
   // 3f4의 최상위 비트(준비) 비트가 1일때 까지 대기를 함.
   if( (inportb(0x3f4) & 64) == 1 ) return 0;
   outportb(0x3f5,data);
   return 1;
}
void Floppy_MotorOn(void)
{
   outportb(0x3f2 , 0x1c ); // DMA enable , FDC reset
   delay(200);
   *( (unsigned char *) 0x00400040 ) = DEFAULT_TIMER_SET;
}
void Floppy_SenseInterruptStatus(void)
{
   Floppy_Senddata( 0x08 );
   PutStatus();
   GetPortData(2);
}
void Floppy_Seek(void)
{
   PutStatus();
   Floppy_Senddata( 0x0f );
   Floppy_Senddata( 0x00 );
   Floppy_Senddata( g_track );
}
void Floppy_Read(void)
{
   Floppy_Senddata( 0xe6 );
   PutStatus();
   Floppy_Senddata( 0x00 );
   Floppy_Senddata( g_track );
   Floppy_Senddata( 0x00 );
   Floppy_Senddata( 0x06 );
   Floppy_Senddata( 0x02 );
   Floppy_Senddata( 0x90 );
   Floppy_Senddata( 0x36 );
   Floppy_Senddata( 0x10 );
   delay(200);
   GetPortData(7);
}
void Floppy_Write(void)
{
   Floppy_Senddata( 0xc5 );
   PutStatus();
   Floppy_Senddata( 0x00 );
   Floppy_Senddata( g_track);
   Floppy_Senddata( 0x00 );
   Floppy_Senddata( 0x02 );
   Floppy_Senddata( 0x02 );
   Floppy_Senddata( 0x90 );
   Floppy_Senddata( 0x36 );
   Floppy_Senddata( 0x10 );
   delay(200);
   GetPortData(7);
}
void Floppy_Diag(void)
{
   Floppy_Senddata( 0x02 );
   PutStatus();
   Floppy_Senddata( 0x00 );
   Floppy_Senddata( g_track );
   Floppy_Senddata( 0x00 );
   Floppy_Senddata( 0x02 );
   Floppy_Senddata( 0x02 );
   Floppy_Senddata( 0x90 );
   Floppy_Senddata( 0x36 );
   Floppy_Senddata( 0x10 );
   delay(200);
   GetPortData(7);
}
void Floppy_DeletedDataRead(void)
{
   Floppy_Senddata( 0xc9 );
   PutStatus();
   Floppy_Senddata( 0x00 );
   Floppy_Senddata( g_track );
   Floppy_Senddata( 0x00 );
   Floppy_Senddata( 0x02 );
   Floppy_Senddata( 0x02 );
   Floppy_Senddata( 0x90 );
   Floppy_Senddata( 0x36 );
   Floppy_Senddata( 0x10 );
   delay(200);
   GetPortData(7);
}
void Floppy_Recalibrate(void)
{
   Floppy_Senddata( 0x07 );
   Floppy_Senddata( 0x00 );
}
void Floppy_ReadID(void)
{
   Floppy_Senddata( 0x4a );
   Floppy_Senddata( 0x00 );
   delay(200);
   GetPortData(7);
}
void main(void)
{
   unsigned char ch;
   register int count;
   _fmemset( buffer , 0 , 512 ); // clear buffer
   old_int0eh = getvect(0x0e);
   setvect( 0x0e , new_int0eh );
/* old_int0eh = getvect(0x08);
   setvect( 0x08 , new_int08h ); */
   clrscr();     // clear screen
   MainScreen(); // view main screen
   PutStatus();  // view status
   SetMemoryAddress(buffer);
   *data02 = 0;
   for(;;)
   {
      gotoxy(2,21);
      cprintf("data at 40:3E - %2x Track - %3d",*data02,g_track);
//      gotoxy(2,20);
//      cprintf("                %2d",inportb(0x0f));
      PutStatus();
      ViewMem(buffer);
      gotoxy(32,15);
      cprintf(" Motor off count: %3d",*((unsigned char *)0x00400040));
      textattr( (5 << 4) | 15 );
      *((unsigned char far *)(0xb8000000)+(160*13)+120) = ' ';
      *((unsigned char far *)(0xb8000000)+(160*13)+122) = ' ';
      *((unsigned char far *)(0xb8000000)+(160*13)+124) = ' ';
      *((unsigned char far *)(0xb8000000)+(160*13)+126) = ' ';
      *((unsigned char far *)(0xb8000000)+(160*13)+128) = ' ';
      *((unsigned char far *)(0xb8000000)+(160*13)+130) = ' ';
      *((unsigned char far *)(0xb8000000)+(160*13)+132) = ' ';
      *((unsigned char far *)(0xb8000000)+(160*13)+134) = ' ';
      *((unsigned char far *)(0xb8000000)+(160*13)+136) = ' ';
      if( kbhit() )
      {
	switch( ch = getch() )
	{
	   case '+':
	     g_track++;
	     break;
	   case '-':
	     g_track--;
	     if( g_track < 0 ) g_track = 0;
	     break;
	   case '1':
	     Floppy_MotorOn();
	     break;
	   case '2':
	     Floppy_SenseInterruptStatus();
	     break;
	   case '3':
	     Floppy_Seek();
	     break;
	   case '4':
	     outportb(0x0a,0x02);    // DMA mask register.
	     outportb(0x0b,0x46);    // DMA mask register.
				     // 010X1110
	     Floppy_Read();
	     ViewMem(buffer);
	     break;
	   case '5':
	     Floppy_Recalibrate();
	     break;
	   case '6':
	     Floppy_ReadID();
	     ViewMem(buffer);
	     break;
	   case '7':
	     Floppy_DeletedDataRead();
	     ViewMem(buffer);
	     break;
	   case '8':
	     Floppy_Diag();
	     ViewMem(buffer);
	     break;
	   case '9':
	     Floppy_MotorOn();
	     Floppy_Recalibrate();
	     Floppy_SenseInterruptStatus();
	     ViewMem(buffer);
	     break;
	   case '0':
	     outportb( 0x3f2 , 00 );
	     break;
	   case 'P','p':
	     GetPortData(7);
	     break;
	   case 'M','m':
	     ViewMem(buffer);
	     break;
	   case 'O','o':
	     DMAPorts_PutStatus();
	     getch();
	     break;
	   case 'I','i':
	     *data02 = 0;
	     break;
	   case 'D','d':
	     system("dir A: > TMP.2");
	     system("D:");
	     for(count=1;count<=20;count++)
	     {
		gotoxy(1,1+count);
		textattr( (5 << 4) | 15 );
		cprintf("                               ");
	     }
	     PutStatus();
	     break;
	   case 'C','c':
	     for( count=0 ; count<=6 ; count++ )
	     {
		gotoxy(1,count+14);
		cprintf("                              ");
	     }
	     break;
	   case ':':
	     Floppy_MotorOn();
	     Floppy_Senddata( 0x03 );
	     Floppy_Senddata( 0xdf );
	     Floppy_Senddata( 0x02 );
	     break;
	   case '<':
	     asm mov ax,0201h;
	     asm les bx,buffer;
	     asm mov cx,0001h;
	     asm mov dx,0000h;
	     asm int 13h;
	     ViewMem(buffer);
	     break;
	   case '>':
	     _fmemset( buffer , 0 , 512 ); // clear buffer
	     ViewMem(buffer);
	     break;
	   case '.':
	     while(!((inport(0x3f4) & 64) == 0));
	     PutStatus();
	     break;
	   case 27:
	     outportb(0x61,0);
	     free(buffer);
  //    setvect( 0x08 , old_int08h );
	     setvect( 0x0e , old_int0eh );
	     textattr(7);
	     clrscr();
	     return;
	}
	PutStatus();
      }
   }
   free(buffer);
   return;
}
