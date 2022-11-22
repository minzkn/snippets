/*
    FLIPLAY.C
*/

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <alloc.h>
#include <math.h>

#define FLI_MAXFRAMES		4000

#define FLI_MAXSIZE         ((unsigned int)64000L)

#define FLIH_MAGIC			0xaf11	// FLIC file ID code
#define FLIF_MAGIC			0xf1fa	// FLIC frame ID code

#define FLI_COLOR			11		// DAC chunk
#define FLI_LC				12		// line compress image chunk
#define FLI_BLACK			13		// clear chunk
#define FLI_BRUN			15		// First screen image chunk
#define FLI_COPY			16		// non-compress image chunk

#define SUCCESS 			0
#define ERR_READ			1
#define ERR_WRITE			2
#define ERR_ALLOC			3
#define ERR_INV_FLIFRAME	4
#define ERR_INV_CHUNK		5
#define ERR_INV_COMP		6

#define FirstScreen             mybuf

#define DBLAC 240
#define DGRAY 242
#define BGRAY 243
#define BWHIT 245
#define BGREE 250
#define BYELL 251
#define BBLUE 252
#define DYELL 253
#define BSKY  254

#define NONE        0
#define EXIST       1

#define NORMAL_ERR  0
#define MOUSE_EXIST 1

#define PLAYING     1
#define PAUSE       0
#define ONEPLAY     0
#define REPLAY      1

#define ZOOM        0
#define UNZOOM      1



typedef unsigned char   BYTE;
typedef unsigned        WORD;
typedef unsigned long   LWORD;
typedef char *          STR;

typedef struct {
	long	Size;
	WORD   Type;
	WORD   FrameCount;
	WORD   Width;
	WORD   Height;
	WORD   BPP;
	int 	Flags;
	int 	Speed;
	long	NextHead;
	long	FramesInTable;
	int 	File;
	long	Frame1Off;
	long	Strokes;
	long	Session;
	char	Reserved[88];
	}FLIHEAD;

typedef struct {
	long	Size;
	WORD   Type;
	int 	Chunks;
	char	Pad[8];
	} FLIFRAME;

typedef struct {
	long	Size;
	WORD   Type;
	} FLICHUNK;

typedef struct {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	} DAC;


typedef struct {
	unsigned char red,green,blue;
	} TRIPLET;
TRIPLET   aacolor[256];
FLIHEAD gfliHead;
DAC 	Palette[256];
DAC 	Palette2[16];
FILE    *fp = NULL;

BYTE far mybuf[(unsigned int)64000L];
BYTE   col[16];
int    gFrames=0, FirstFlag=0; // First frame decompress flag
LWORD  ffCount=(LWORD)sizeof(FLIHEAD);
LWORD  atime=0, fcount=0;
LWORD  flispeed=0;
BYTE   active=0;

class NFLI{
	   public:
	   void SetDAC(int num, unsigned char red, unsigned char green, unsigned char blue);
	   unsigned char GetRed(int num);
	   unsigned char GetGreen(int num);
	   unsigned char GetBlue(int num);
	   void Scan_Line(unsigned x1, unsigned x2, unsigned y, char color);
	   void Write_Video(unsigned int x,unsigned int y,unsigned d_x, unsigned d_y,unsigned pitch, void far *buffer);
	   int DoPalette( WORD size );
	   int DoRLE( WORD size );
	   int DoCopy( WORD size );
	   int DoLC( WORD size );
	   int DoChunk(void);
	   int DoOneFrame(void);
	   void Set_Anima(void);
	   int Open_Anima(STR flname );
	   void Fli_play(void);
	   void Close_Anima(void);
	   int FLIPLAY(char far *filename);
	   }NFLI;

//	FLI_COLOR chunk process routine.
void NFLI::SetDAC(int num, unsigned char red, unsigned char green, unsigned char blue)
{
	aacolor[num].red = red;
	aacolor[num].green = green;
	aacolor[num].blue = blue;
	outportb(0x3c8, num);
	outportb(0x3c9, red);
	outportb(0x3c9, green);
	outportb(0x3c9, blue);
}

unsigned char NFLI::GetRed(int num)
{
	return (aacolor[num].red);
}

unsigned char NFLI::GetGreen(int num)
{
	return (aacolor[num].green);
}

unsigned char NFLI::GetBlue(int num)
{
	return (aacolor[num].blue);
}

void NFLI::Scan_Line(unsigned x1, unsigned x2, unsigned y, char color)
{
    unsigned char current_page;
	int xwidth=x2-x1+1;
	asm push    di
	asm push    ds
	asm push    es

	asm mov ax,y
    asm mov bx,320
	asm mul bx
	asm add ax,x1
	asm adc dl,0
	asm mov bx,ax
	asm mov current_page, dl

pass:
    asm mov ax,0a000h
    asm mov es,ax
    asm mov al,color

    asm mov cx, xwidth
lp2:
    asm mov di,bx
    asm add di,xwidth
    asm jc  proc2

    asm mov cx,xwidth
    asm sub di,xwidth
    asm cld
    asm REP stosb
    asm jmp next1

proc2:
    asm sub di,xwidth
    asm mov cx,di
    asm neg cx
    asm cld
    asm REP stosb
    asm mov di,bx
    asm add di,xwidth
    asm inc dl
    asm mov current_page, dl
    asm mov cx,di
    asm xor di,di
    asm REP stosb

next1:
    asm    pop es
    asm    pop ds
    asm    pop di
}

void NFLI::Write_Video(unsigned int x,unsigned int y,unsigned d_x, unsigned d_y,unsigned pitch, void far *buffer)
{
    unsigned char current_page;
    unsigned delta;

	asm    push    si
	asm    push    di
	asm    push    ds
	asm    push    es

	asm    lds si,buffer
	asm    mov ax, 0a000h
	asm    mov es, ax
	asm    mov ax, y
	asm    mov dx, 320
	asm    mul dx
	asm    add ax,x
	asm    adc dx,0
	asm    mov di,ax
	asm    mov current_page, dl

	asm    mov ax,d_x
	asm    mov bx,pitch
	asm    sub bx,ax
	asm    mov dx, 320
	asm    sub dx,ax
	asm    mov delta, dx
	asm    cld

WV_lineloop:
	asm    mov cx,d_x
	asm    mov ax,cx
	asm    add ax,di
	asm    jnc WV_samevpage
	asm    sub cx,ax
	asm    shr cx,1
	asm    rep movsw
	asm    adc cx,cx
	asm    rep movsb
	asm    mov cx,ax
	asm    inc current_page
	asm    mov dl, current_page
	asm    jcxz    WV_line_r

WV_samevpage:
	asm    shr cx,1
	asm    rep movsw
	asm    adc cx,cx
	asm    rep movsb
WV_line_r:
	asm    add si,bx
	asm    add di,delta
	asm    jc  WV_nextvpage
	asm    dec d_y
	asm    jg  WV_lineloop
	asm    jmp WV_r

WV_nextvpage:
	asm    inc current_page
	asm    mov dl, current_page
	asm    dec d_y
	asm    jg  WV_lineloop
WV_r:
	asm    pop es
	asm    pop ds
	asm    pop di
	asm    pop si
}


int NFLI::DoPalette( WORD size )
{
	struct
	{
        BYTE   Change;
        BYTE   Change2;
        BYTE   Skip;
        BYTE   NU;
	} hd;
    WORD   idx;
	int 	i, j, u1, u2, u3, rtn, t1, t2;
	char    i1, i2, i3;

	if ( !fread( &hd, 4, 1, fp ) )
		rtn = ERR_READ;
	else {
		if ( !fread( &Palette[0], size - 4, 1, fp ) )
			rtn = ERR_READ;
		else {
			hd.Change = hd.NU;
			if ( !hd.Skip && !hd.Change )
				hd.Change = 256;
			else
				hd.Change += hd.Skip;
			if ( hd.Change >= 255 )
				hd.Change = 256;
			for ( i = hd.Skip; i < 240; i++ )
				SetDAC( i, Palette[i].red, Palette[i].green, Palette[i].blue );
			for ( i = 240 ; i < 256; i++ )
				Palette2[ i - 240 ] = Palette[ i ];

			for ( i = 240 ; i < 256; i++ ) {
				i1 = Palette2[i-240].red  ;
				i2 = Palette2[i-240].green;
				i3 = Palette2[i-240].blue ;
				u1 = GetRed(0)-i1;
				u2 = GetGreen(0)-i2;
				u3 = GetBlue(0)-i3;
				t1 = u1*u1+u2*u2+u3*u3;
				idx = 0;
				for (j = 1; j < 240; j++ ) {
					u1 = GetRed(j)-i1;
					u2 = GetGreen(j)-i2;
					u3 = GetBlue(j)-i3;
					t2 = u1*u1+u2*u2+u3*u3;
					if(t1>t2)	{
						t1 = t2;
						idx = j;
					}
				}
				col[i-240] = idx;
			}
			rtn = SUCCESS;
		}
	}
	return rtn;
}

int NFLI::DoRLE( WORD size )
{
	char far *buff;
    BYTE far *bptr;
    BYTE far *sp;
    WORD i, Count, iCount = 0;
    BYTE c;
	int Packets, rtn;

	if ( FirstFlag ) {
		fseek( fp, size, SEEK_CUR );
		rtn = SUCCESS;
	}
	else {                // case of first entrance
		if ( ( buff = ( char far *)farmalloc( size ) ) == NULL )
			rtn = ERR_ALLOC;
		else {
			if ( !fread( buff, size, 1, fp ) )
				rtn = ERR_READ;
			else {
                bptr = ( BYTE far *)buff;
				while ( iCount < FLI_MAXSIZE ) {
					Packets = *( bptr++ );      // packet count
					while ( Packets-- ) {
						Count = *( bptr++ );    // repeat count
						if ( Count > 127 ) {      // copy data
							asm NEG   BYTE PTR Count;
							for ( sp = FirstScreen + iCount, i = 0; i < Count; i++ ) {
								c = *( bptr++ );
								*( sp++ ) = ( c < 240 ) ? c : col[c-240];
							}
                        } else if ( Count ) {       // fill
							if ( ( c = *( bptr++ ) ) >= 240 )
								c = col[c-240];
							for ( sp = FirstScreen + iCount, i = 0; i < Count; i++ )
								*( sp++ ) = c;
                        } else
							goto DEND;
						iCount += Count;
					}
				}
				rtn = SUCCESS;
			}
			DEND:
			farfree( buff );
			FirstFlag = 1;
		}
	}
	Write_Video(0,0,320, 200, 320, FirstScreen);
	return rtn;
}

int NFLI::DoCopy( WORD size )
{
	char far *buff;
    BYTE far *sp;
    WORD Height, Width, Area, i;
    BYTE c;
	int rtn;

	if ( ( buff = ( char far * )farmalloc( size ) ) == NULL )
		rtn = ERR_ALLOC;
	else {
		if ( !fread( buff, size, 1, fp ) )
			rtn = ERR_READ;
		else {
			if ( size > (unsigned int)64000L )
				size = (unsigned int)64000L;
			Height = size / 320;
			Area   = Height * 320;
			Width  = size - Area;
            for ( sp = ( BYTE far * )buff, i = 0; i < size; sp++, i++ ) {
				if ( ( c = *sp ) >= 240 )
					*sp = col[ c - 240 ];
			}
			if ( Height )
				Write_Video(0,0, 320, Height, 320, buff );
			if ( Width )
				Write_Video(0,Height, Width, 1, 320, buff + Area );
			rtn = SUCCESS;
		}
		farfree( buff );
	}
	return rtn;
}

int NFLI::DoLC( WORD size )
{
	char far *buff;
    BYTE far *bptr;
    BYTE far *sp;
    WORD TotalPackets, Packets, SkipLines, SkipBytes, Limt, i, Count;
	int rtn;
    BYTE c;

	if ( ( buff = ( char far * )farmalloc( size ) ) == NULL )
		rtn = ERR_ALLOC;
	else {
		if ( !fread( buff, size, 1, fp ) )
			rtn = ERR_READ;
		else {
			SkipLines	 = *( ( unsigned far * )buff );
			TotalPackets = *( ( unsigned far * )( buff + 2 ) );
            bptr = ( BYTE far * )buff + 4;
			Limt = 200;
			while ( TotalPackets-- ) {
				Packets = *( bptr++ );
				SkipBytes = 0;
				while ( Packets-- ) {
					SkipBytes += *( bptr++ );
					Count = *( bptr++ );
					if ( Count > 127 ) {
						asm NEG   BYTE PTR Count;
						if ((c = *(bptr++))>=240) {
							c = col[c-240];
						}
						if ( SkipLines < Limt )
			    Scan_Line( SkipBytes, SkipBytes + Count - 1, SkipLines, c );
                    } else if( Count != 0 ) {
						if ( SkipLines < Limt )	{
							for (sp=bptr, i=0; i<Count; sp++, i++ )	{
								if ((c = *sp)>=240)
									*sp = col[c-240];
							}
							Write_Video( SkipBytes, SkipLines,Count, 1, 320, bptr );
						}
						bptr += Count;
                    } else {
						rtn = ERR_INV_COMP;
						goto Chunk_done;
					}
					SkipBytes += Count;
				}
				SkipLines++;
			}
			rtn = SUCCESS;
		Chunk_done:
			;
		}
		farfree( buff );
	}
	return rtn;
}

int NFLI::DoChunk(void)
{
	FLICHUNK fliChunk;
	int 	 rtn;

    if ( !fread( ( STR )&fliChunk, sizeof( FLICHUNK ), 1, fp ) )
		rtn = ERR_READ;
	else {
		switch( fliChunk.Type ) {
            case FLI_COLOR:
		rtn = DoPalette( (unsigned int)fliChunk.Size - sizeof( FLICHUNK ) );
                break;
            case FLI_LC:
                rtn = DoLC( (unsigned int)fliChunk.Size - sizeof( FLICHUNK ) );
                break;
	    case FLI_BLACK:
		{
		//unsigned int c;
		//for(c=0;c<64000;c++)*((unsigned char far *)0xA0000000L+c)=0;
		}
                rtn = SUCCESS;
                break;
            case FLI_BRUN:
		rtn = DoRLE( (unsigned int)fliChunk.Size - sizeof( FLICHUNK ) );
                break;
            case FLI_COPY:
                rtn = DoCopy( (unsigned int)fliChunk.Size - sizeof( FLICHUNK ) );
                break;
            default:
                rtn = ERR_INV_CHUNK;
        }
	}
	return rtn;
}

int NFLI::DoOneFrame(void)
{
 FLIFRAME fliFrame;
 int 	  i, rtn;
	if(!fread((STR)&fliFrame,sizeof(FLIFRAME),1,fp))rtn=ERR_READ;
	else{
	     if(fliFrame.Type!=FLIF_MAGIC)rtn = ERR_INV_FLIFRAME;
	     else{
		  for(i=0;i<fliFrame.Chunks;i++){
						 rtn=DoChunk();
						 if(rtn!=SUCCESS)break;
						 }
		  ffCount+=fliFrame.Size;
		  fseek(fp,ffCount,SEEK_SET);
		  rtn=SUCCESS;
		  }
	     }
	return rtn;
}

void NFLI::Set_Anima(void)
{
	gFrames=0;
	atime=*((volatile unsigned long far *)0x0000046cL);
	ffCount=(LWORD)sizeof(FLIHEAD);
	fseek(fp,(long)sizeof(FLIHEAD),SEEK_SET);
}

int NFLI::Open_Anima(STR flname )
{
 int rtn;

	if((fp=fopen(flname,"rb"))==NULL)rtn=1;
	else if(!fread((STR)&gfliHead,sizeof(FLIHEAD),1,fp))rtn=2;
	else if(gfliHead.Type!=FLIH_MAGIC)rtn=3;
	else rtn=0;
	if(rtn){
		if(rtn>1)fclose(fp);
		fp=NULL;
		active=PAUSE;
		}
	else{
	     active=PLAYING;
	     FirstFlag=0;
	     Set_Anima();
	     memset(&Palette2[0],0,96);
	     }
	flispeed=gfliHead.Speed/6;
	return rtn;
}

void NFLI::Fli_play(void)
{
	if(active){
		   if(flispeed<(*((volatile unsigned long far *)0x0000046cL)-atime)){
					      gFrames++;
					      if((DoOneFrame()!=SUCCESS)||(gFrames>=gfliHead.FrameCount)){
								     Set_Anima();
								     }
					      atime=*((volatile unsigned long far *)0x0000046cL);
					      }
		   }

}

void NFLI::Close_Anima(void)
{
	if(fp){
	       fclose( fp );
	       fp = NULL;
	       }
	active=0;
}

int NFLI::FLIPLAY(char far *filename)
{
 	   _AX=0x0013;
	   asm INT 10H;
	   if(Open_Anima(filename)!=NULL)return (-1);
	   while(!kbhit()){
			   Fli_play();
         delay(50);
			   }
	   if(getch()==0)getch();
	   Close_Anima();
	   _AX=0x0003;
	   asm INT 10H;
	   gFrames=0;
	   FirstFlag=0;
	   ffCount=(LWORD)sizeof(FLIHEAD);
	   atime=0;
	   fcount=0;
	   flispeed=0;
	   active=0;
	   fp=0;
	   return (1);
}


void main(void)
{
 NFLI.FLIPLAY("nslogo.dat");
}