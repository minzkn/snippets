#include<dos.h>
#include<stdio.h>
#include<conio.h>
#include<mem.h>
#include<alloc.h>
#include<process.h>


void _MINZ_outportb( unsigned int portn, unsigned char data );
unsigned char _MINZ_inportb( unsigned int portn );

void _MINZ_DMA_SetAddress( unsigned int seg , unsigned int off , unsigned char mode );
void _MINZ_SendToFDC( unsigned char data );

unsigned char MINZ_FDC_formattrack(unsigned char t);
unsigned char MINZ_FDC_readsector( unsigned char t ,
				   unsigned char h ,
				   unsigned char s ,
				   unsigned char far *buffer );
unsigned char MINZ_FDC_writesector( unsigned char t ,
				    unsigned char h ,
				    unsigned char s ,
				    unsigned char far *buffer );

// outportb함수를 직접 만들었다.
void _MINZ_outportb( unsigned int portn , unsigned char data )
{
     asm mov dx,portn;
     asm mov al,data;
     asm out dx,al;
     return;
}

// inportb함수를 직접 만들었다.
unsigned char _MINZ_inportb( unsigned int portn )
{
     asm mov dx,portn;
     asm in al,dx;
     return(_AL);
}

// DMA를 통해 주소를 지정한다.
void _MINZ_DMA_SetAddress( unsigned int seg , unsigned int off , unsigned char mode )
{
     unsigned long offset = ((unsigned long)(seg) << 4 ) | off;
     // DMA 채널2는 디스크 콘트롤러가 사용한다.     // 주소 지정
     _MINZ_outportb(0x04,offset & 0xff);
     _MINZ_outportb(0x04,(offset >> 8) & 0xff);
     _MINZ_outportb(0x81,(offset >> 16) & 0xff);

     // 카운터 지정 ( 512 )
     _MINZ_outportb(0x05,0xff);
     _MINZ_outportb(0x05,0x01);

     _MINZ_outportb(0x0b,mode);
     _MINZ_outportb(0x0c,mode);

     _MINZ_outportb(0x0a,2);    // 채널 2번을 가능하게 한다.
     return;
}

void _MINZ_SendToFDC( unsigned char data )
{
     // 0x3f4 FDC주상태 레지스터에서
     // 준비중일때 까지 기다린다.
     while( (_MINZ_inportb(0x3f4) & 128) == 0 ); // 기다림
     // 준비중이 되었다 이제 보내자.
     _MINZ_outportb( 0x3f5 , data );
     return; // 보냈다.
}

unsigned char MINZ_FDC_readsector( unsigned char t ,
				   unsigned char h ,
				   unsigned char s ,
				   unsigned char far *buffer )
{
     _MINZ_outportb( 0x3f2 , 0x1c ); // DOR 지정
     *((unsigned char far *)(0x00400040)) = 50; // 카운트 지정

     _MINZ_SendToFDC( 0x0f ); // SEEK 명령
     _MINZ_SendToFDC( 0x00 );
     _MINZ_SendToFDC( t );

     // 주소 지정.
     _MINZ_DMA_SetAddress( FP_SEG(buffer) , FP_OFF(buffer) ,0x46);
     // 01000101
     // 싱글 모드 and 주소증가 and 메모리에 쓰기
     // chanel 2번
     _MINZ_outportb( 0x3f2 , 0x1c ); // DOR 지정
     *((unsigned char far *)(0x00400040)) = 50; // 카운트 지정

     _MINZ_SendToFDC( 0xe6 ); // 쓰기 명령
     _MINZ_SendToFDC( 0x00 ); // set drive
     _MINZ_SendToFDC( t );
     _MINZ_SendToFDC( h );
     _MINZ_SendToFDC( s );
     _MINZ_SendToFDC( 0x02 ); // 섹터당 몇 바이트
     _MINZ_SendToFDC( 0x90 ); // 트랙의 끝
     _MINZ_SendToFDC( 0x2a ); // gap 3 length
     _MINZ_SendToFDC( 0x10 ); // data length
     delay(200);

     // 명령이 run 되었음
     if( (_MINZ_inportb(0x3f5) & 0xc0) == 0 ) return 1; else return 0;
}

unsigned char MINZ_FDC_writesector( unsigned char t ,
				    unsigned char h ,
				    unsigned char s ,
				    unsigned char far *buffer )
{
     // c5 4 0 0 1 1 2 12 ff
     // 주소 지정
     _MINZ_DMA_SetAddress( FP_SEG(buffer) , FP_OFF(buffer) ,0x4a);
     // 01001010
     /* 01 - single mode
	0  - increse address
	unused 4 bit
	10 - read mode
	10 - chanel 2       */

     _MINZ_outportb( 0x3f2 , 0x1c ); // DOR 지정
     *((unsigned char far *)(0x00400040)) = 50; // 카운트 지정

     _MINZ_SendToFDC( 0xc5 ); // command - write sector
     _MINZ_SendToFDC( 0x00 ); // drive
     _MINZ_SendToFDC( t );
     _MINZ_SendToFDC( h );
     _MINZ_SendToFDC( s );
     _MINZ_SendToFDC( 0x02 ); // bytes per sector
     _MINZ_SendToFDC( 0x90 ); // last sector in track
     _MINZ_SendToFDC( 0x2a ); // gap 3 length
     _MINZ_SendToFDC( 0x10 ); // data length
     delay(200);

     // check success?
     // 3f5(command status register) check
//     printf("%d\n",_MINZ_inportb(0x3f5) & 0xc0);
     if( (_MINZ_inportb(0x3f5) & 0xc0) == 0 ) return 1; else return 0;
}


unsigned char MINZ_FDC_formattrack(unsigned char t)
{
     _MINZ_outportb( 0x3f2 , 0x1c ); // DOR 지정
     *((unsigned char far *)(0x00400040)) = 50; // 카운트 지정

     _MINZ_outportb( 0x3f2 , 0x1c ); // DOR 지정
     *((unsigned char far *)(0x00400040)) = 50; // 카운트 지정

     _MINZ_SendToFDC( 0xed );
     _MINZ_SendToFDC( 0x00 );
     _MINZ_SendToFDC( 0xff );
     _MINZ_SendToFDC( 0x0f );
     _MINZ_SendToFDC( 0x2a ); // gap 3 length
     _MINZ_SendToFDC( 0x10 );

     if( (_MINZ_inportb(0x3f5) & 0xc0) == 0 ) return 1; else return 0;
}
unsigned char far *buffer = (unsigned char far *)farmalloc(512);

void main(void)
{
     register int c,c1;
     printf(" MINZ FDC test program - MINZ technologies (C) 1998.\n");
     printf(" load a sector...  ");
     printf("%s\n",( MINZ_FDC_readsector( 0,0,1, buffer ) ? "succeed" : "failed" ));
     system("dir a:");
     printf(" Write 'MINZ' to sector 2 in track 0...  ");
     system("dir a:");
     _MINZ_outportb( 0x3f2 , 0x1c ); // DOR 지정
     *((unsigned char far *)(0x00400040)) = 50; // 카운트 지정

     delay(200);

     _MINZ_SendToFDC( 0x07 ); // command - write sector
     _MINZ_SendToFDC( 0x00 ); // drive
     for( c=2 ; c<=8 ; c++)
     {
     delay(300);
     printf(" Write 'MINZ' to sector %d in track 0...  ",c);
     buffer[0] = 'M';
     buffer[1] = 'I';
     buffer[2] = 'N';
     buffer[3] = 'Z';
     buffer[4] = '-';
     buffer[5] = 'T';
     buffer[6] = '6';
     buffer[7] = ' ';
     buffer[8] = 'V';
     buffer[9] = 'E';
     buffer[10] = 'R';
     printf("%s\n",( MINZ_FDC_writesector( 0,0,c, buffer ) ? "succeed" : "failed" ));
     printf("\n");
     }
     farfree(buffer);
     return;
}