#include<dos.h>
#include<stdio.h>

typedef unsigned char   BYTE;
typedef unsigned        WORD;
typedef unsigned long   LWORD;
typedef char *          STR;

extern gFrames;

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
	   };
