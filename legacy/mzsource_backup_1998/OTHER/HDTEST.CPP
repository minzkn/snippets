#include<stdio.h>
#include<dos.h>
void main(void)
{
     outportb(0x3f6,8);
     outportb(0x1f6,0xe0);
     outportb(0x1f6,0xe0);
     outportb(0xe8,0x58);
     outportb(0xa1,0x0d);
     outportb(0x21,0xb8);
     outportb(0x1f1,0x00);
     outportb(0x1f2,0x01);
     outportb(0x1f3,0x3f);
     outportb(0x1f4,0x00);
     outportb(0x1f5,0x00);
     outportb(0x1f6,0xe0);
     outportb(0x1f7,0x21);
     outportb(0xeb,0x58); // port for delay
     unsigned int ch;
     delay(100);
     for( register int c = 0 ; c<=256 ; c++ )
     {
	ch = inpw(0x1f0);
	*( (unsigned char far *) 0xb8000000 + (2*c) ) = ( ch & 0xff );
	*( (unsigned char far *) 0xb8000000 + (2*c)+1 ) = 7;
	ch = inp(0x1f1);
	*( (unsigned char far *) 0xb8000000 + (2*c) ) = ( ch & 0xff );
	*( (unsigned char far *) 0xb8000000 + (2*c)+1 ) = 7;
     }
}
