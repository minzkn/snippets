#include <stdio.h>
#include <dos.h>

#define COM1          0
#define COM2          1
#define COM3          2
#define COM4          3

#define COM1BASE      0x3F8
#define COM2BASE      0x2F8
#define COM3BASE      0x3E8
#define COM4BASE      0x2E8

#define COM13_INTERRUPT  0x0C
#define COM24_INTERRUPT  0x0B

#define MASKON13      0xEF
#define MASKON24      0xF7
#define MASKOFF13     0x10
#define MASKOFF24     0x08

#define DATABIT7      0x02
#define DATABIT8      0x03

#define STOPBIT1      0x00
#define STOPBIT2      0x04

#define PARITYNONE    0x00
#define PARITYODD     0x08
#define PARITYEVEN    0x18

#define BPS1200       1200L
#define BPS2400       2400L
#define BPS4800       4800L
#define BPS9600       9600L
#define BPS19200      19200L
#define BPS38400      38400L
#define BPS57600      57600L
#define BPS115200     115200L

#define LCR_OFFSET    3
#define MCR_OFFSET    4
#define IER_OFFSET    1
#define LSR_OFFSET    5
#define MSR_OFFSET    6

#define PICMASK       0x21
#define PICEIO        0x20

unsigned char far *VRAM=(unsigned char far *)0xb8000000;

signed char mousebuffer[3];
unsigned char lastdata[2];


void interrupt (*old_init)();
int bufpos;
int base_addr;
int port_int_no;
int imr_on;
int imr_off;

typedef struct {
    signed int x;
    signed int y;
    unsigned char *lastdata;
} MOUSELASTDATA;

MOUSELASTDATA mouselastdata;



typedef struct {
   signed int x;
   signed int y;
   signed char ix;
   signed char iy;
   signed int left;
   signed int right;
   signed int xspeed;
   signed int yspeed;
   signed int xtop;
   signed int ytop;
   signed int xbottom;
   signed int ybottom;
} MOUSEDATA;

MOUSEDATA mouse;






void printbit(unsigned long s_bit, unsigned int s_length);

void InitSerialPort(int port, unsigned char databit, unsigned char stopbit,
		     unsigned char parity, long bps)
{
    char ch, old_lcr;
    unsigned int divisor;
    unsigned char high, low;

    switch(port){
       case COM1 :
	   base_addr = COM1BASE;
	   port_int_no = COM13_INTERRUPT;
	   imr_on = MASKON13;
	   imr_off = MASKOFF13;
	   break;
       case COM2 :
	   base_addr = COM2BASE;
	   port_int_no = COM24_INTERRUPT;
	   imr_on = MASKON24;
	   imr_off = MASKOFF24;
	   break;
       case COM3 :
	   base_addr = COM3BASE;
	   port_int_no = COM13_INTERRUPT;
	   imr_on = MASKON13;
	   imr_off = MASKOFF13;
	   break;
       case COM4 :
	   base_addr = COM4BASE;
	   port_int_no = COM24_INTERRUPT;
	   imr_on = MASKON24;
	   imr_off = MASKOFF24;
	   break;
    }
    /* LCR Register set */
    ch = databit | stopbit | parity;
    outportb(base_addr + LCR_OFFSET, ch);
    /* Modem speed set */
    divisor = (unsigned int) (1843200L / (bps*16L));
    high = (unsigned char) (divisor >> 8);
    low = (unsigned char) divisor;

    /* LCR DLAB=1 */
    old_lcr = inportb(base_addr + LCR_OFFSET);
    outportb(base_addr + LCR_OFFSET, old_lcr | 0x80);

    /* DLL, DLH speed out */
    outportb(base_addr,low);
    outportb(base_addr+1,high);

    /* LCR DLAB = 0 */
    old_lcr = inportb(base_addr + LCR_OFFSET);
    outportb(base_addr + LCR_OFFSET, old_lcr & 0x7F);
}

void interrupt port_interrupt()
{
    signed char ch;
    ch = inportb(base_addr);
    if( ch & 0x40) bufpos=0;

    mousebuffer[bufpos++]=ch;

    if(bufpos==3){
	mouse.ix=((mousebuffer[0]&0x03)<<6)+(mousebuffer[1]&63);
	mouse.iy=((mousebuffer[0]&0x0C)<<4)+(mousebuffer[2]&63);
	mouse.x+=(mouse.ix)/(mouse.xspeed);
	mouse.y+=(mouse.iy)/(mouse.yspeed);


	mouse.left=(mousebuffer[0]&32)? 1:0;
	mouse.right=(mousebuffer[0]&16)? 1:0;
	if( mouse.x < mouse.xtop ) mouse.x = mouse.xtop;
	else if( mouse.x > mouse.xbottom ) mouse.x = mouse.xbottom;
	if( mouse.y < mouse.ytop ) mouse.y = mouse.ytop;
	else if( mouse.y > mouse.ybottom ) mouse.y = mouse.ybottom;

	bufpos=0;
    }
    outportb(PICEIO,0x20);
}

void InitMouse()
{
     char old_imr;

     outportb(base_addr+MCR_OFFSET,0x0B);
     outportb(base_addr+IER_OFFSET,0x01);

     old_init = getvect(port_int_no);
     setvect(port_int_no, port_interrupt);

     old_imr = inportb(PICMASK);

     outportb(PICMASK, old_imr & imr_on);

     delay(200);

     inportb(base_addr);


}

void EndMouse()
{
    unsigned char old_imr;

    outportb(base_addr + IER_OFFSET, 0x00);

    old_imr = inportb(PICMASK);
    outportb(PICMASK, old_imr | imr_off);

    setvect(port_int_no, old_init);

}


void printbit(unsigned long s_bit, unsigned int s_length)
{
      unsigned long bit_rol;
      unsigned int i;

      bit_rol = 0x01;
      bit_rol = bit_rol << (s_length-1);

      for( i = s_length; i != 0; i--){
       printf("%d",(s_bit & bit_rol)? 1 : 0 );
       bit_rol = bit_rol >> 1;
      }
}

void printtextcusor(void)
{
   VRAM[((mouselastdata.x)+(mouselastdata.y*80)) *2] = *mouselastdata.lastdata;

   *mouselastdata.lastdata = VRAM[ (mouse.x+mouse.y*80) * 2];
   VRAM[ (mouse.x + mouse.y*80) * 2 ]=177;
   mouselastdata.x=mouse.x;
   mouselastdata.y=mouse.y;

}
void curs_off(void)  //커서를 안보이게 하는 함수
{
     union REGS reg;

     reg.h.ah=1;
     reg.h.ch=0x20;

     int86(0x10, &reg, &reg);
}

void curs_on(void)  //커서를 보이게 하는 함수
{
     union REGS reg;

     reg.h.ah=1;
     reg.h.ch=12;
     reg.h.cl=13;

     int86(0x10, &reg, &reg);

}



void main(void)
{
    unsigned char ch;


    clrscr();

    InitSerialPort(COM2, DATABIT8, STOPBIT1, PARITYNONE, 1250);

    InitMouse();

    /* Mouse setting */
    mouse.xspeed =3;
    mouse.yspeed =3;
    mouse.xtop=0;
    mouse.ytop=6;
    mouse.xbottom=79;
    mouse.ybottom=25;

    mouselastdata.lastdata=lastdata;
    /* Mouse Use Exam Start*/

    gotoxy(1,10);
printf("Mouse Use Example, You can move the mouse cursor\
 and click the mouse. When you move the mouse cursor\
 you can see how the bit's are going on. And you can see\
 the bit which comes from UART. If you want to EXIT this program\
 then click the mouse Right and Left. Or you can also EXIT by pressing\
 'ESC' key ");

    curs_off();

    while(1){

      gotoxy(1,1);
      printf("%s, ",mouse.left?"LEFT":"left");
      printf("%s",mouse.right ? "RIGHT":"right");

      if((mousebuffer[0]&0x10) && (mousebuffer[0]&0x20) ) break;

      printf("\nPosition (Vitual) = %5d:%5d",mouse.x,mouse.y);
      printf("\nBuffer Bit : [");
      printbit(mousebuffer[0],8);
      printf("] [");

      printbit(mousebuffer[1],8);
      printf("] [");
      printbit(mousebuffer[2],8);
      printf("]");
      printf("\nIncrease bit - X:");
      printbit(mouse.ix,8);
      printf(" Y:");
      printbit(mouse.iy,8);
      printtextcusor();


      if( kbhit() ){
	 ch=getch();
	 if(ch == 0x1b) break;


     }
 }
 curs_on();

 EndMouse();
}






