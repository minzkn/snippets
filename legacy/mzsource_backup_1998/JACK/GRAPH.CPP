/*
  JACK's GRAPH.CPP
*/

#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#include "common.h"
#include "svgacc.h"
#include "himem.h"
#include "code.h"
#include "hjcode.h"

typedef struct{
               signed int id; signed int x1, y1, x2, y2;
               unsigned char palcolor; signed char pal[48];
              }ImageHeader;

unsigned char far *s_EngFont;
unsigned char far *s_HanFont;
signed char s_ksmode=N_False;

signed int far GetImageXMS(signed int x1, signed int y1, signed int x2, signed int y2, unsigned int n_handle);
signed int far PutImageXMS(signed int x1, signed int y1, signed int x2, signed int y2, unsigned int n_handle);
void far KS2JO(unsigned int far *n_hancode);

void far EngPutc(signed int n_x, signed int n_y, signed char n_character, unsigned char n_color, unsigned char n_bcolor)
{
 signed int n_offset;
 register signed int n_count;
 register signed int n_bitcount;
           n_offset=(signed int)(n_character)*16;
           for(n_count=0;n_count<16;n_count++)
           {
            for(n_bitcount=0;n_bitcount<8;n_bitcount++)
            {
             if((unsigned char)(*(s_EngFont+n_offset+n_count))&(1<<n_bitcount))drwpoint(SET, n_color, n_x+(7-n_bitcount), n_y+n_count);
             else if(n_bcolor!=N_Default)drwpoint(SET, n_bcolor, n_x+(7-n_bitcount), n_y+n_count);
            }
           }
}

void far EngPuts(signed int n_x, signed int n_y, signed char far *n_string, unsigned char n_color, unsigned char n_bcolor)
{
           while(*n_string){EngPutc(n_x, n_y, *(n_string++), n_color, n_bcolor); n_x+=8;}
}

void far HanPutcLow(signed int n_x, signed int n_y, unsigned char far *n_offset, unsigned char n_color, unsigned char n_bcolor)
{
 register signed int n_count;
 register signed int n_bitcount;
           for(n_count=0;n_count<16;n_count++)
           {
            for(n_bitcount=0;n_bitcount<8;n_bitcount++)
            {
             if((*(n_offset+(n_count<<1)+1))&(1<<n_bitcount))drwpoint(SET, n_color, n_x+(15-n_bitcount), n_y+n_count);
             else if(n_bcolor!=N_Default)drwpoint(SET, n_bcolor, n_x+(15-n_bitcount), n_y+n_count);
             if((*(n_offset+(n_count<<1)))&(1<<n_bitcount))drwpoint(SET, n_color, n_x+(7-n_bitcount), n_y+n_count);
             else if(n_bcolor!=N_Default)drwpoint(SET, n_bcolor, n_x+(7-n_bitcount), n_y+n_count);
            }
           }
}

void far KS2JO(unsigned int far *n_hancode)
{
 unsigned int n_c1, n_c2;
           n_c1=(*n_hancode)>>8;
           n_c2=(*n_hancode)&0x00ff;
           if(n_c2==0xa4)(*n_hancode)=((n_c2>=0xa1&&n_c2<=0xd3)?Single[n_c2-0xa1]:0);
           else if(n_c1<0xb0||n_c1>0xc8||n_c2<0xa1||n_c2>0xfe)(*n_hancode)=0;
           else (*n_hancode)=KSTBL[(n_c1-0xb0)*94+n_c2-0xa1];
}

void far HanPutc(signed int x, signed int y, union HanCode HanCode, unsigned char Color, unsigned char BackColor)
{
 signed int n_count, n_offset;
 unsigned int MiddleOffset[]={0, 0, 0, 0, 1, 2, 3, 4, 0, 0, 5, 6, 7, 8, 9, 10, 0, 0, 11, 12, 13, 14, 15, 16, 0, 0, 17, 18, 19, 20, 0, 0};
 unsigned int LastOffset[]={0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 0, 0};
 unsigned char buff[32]={0,};
           if(HanCode.Code.id==0)return;
           if(s_ksmode==N_True)KS2JO(&HanCode.WordData);
           if(HanCode.Code.First!=1)
           {
            switch(HanCode.Code.Middle)
            {
             case 0:
             case 1:
             case 2:
             case 3:
             case 4:
             case 5:
             case 6:
             case 7:
             case 10:
             case 11:
             case 12:
             case 29:
                  if(HanCode.Code.Last==1)n_offset=0;
                  else n_offset=95;
                  for(n_count=0;n_count<32;n_count++)buff[n_count]|=*(s_HanFont+n_count+((n_offset+(HanCode.Code.First-2))<<5));
                  break;
             case 13:
             case 19:
             case 27:
                  if(HanCode.Code.Last==1)n_offset=19;
                  else n_offset=114;
                  for(n_count=0;n_count<32;n_count++)buff[n_count]|=*(s_HanFont+n_count+((n_offset+(HanCode.Code.First-2))<<5));
                  break;
             case 20:
             case 26:
                  if(HanCode.Code.Last==1)n_offset=38;
                  else n_offset=114;
                  for(n_count=0;n_count<32;n_count++)buff[n_count]|=*(s_HanFont+n_count+((n_offset+(HanCode.Code.First-2))<<5));
                  break;
             case 14:
             case 15:
             case 18:
             case 28:
                  if(HanCode.Code.Last==1)n_offset=57;
                  else n_offset=133;
                  for(n_count=0;n_count<32;n_count++)buff[n_count]|=*(s_HanFont+n_count+((n_offset+(HanCode.Code.First-2))<<5));
                  break;
             case 21:
             case 22:
             case 23:
                  if(HanCode.Code.Last==1)n_offset=76;
                  else n_offset=133;
                  for(n_count=0;n_count<32;n_count++)buff[n_count]|=*(s_HanFont+n_count+((n_offset+(HanCode.Code.First-2))<<5));
                  break;
             default: // Fill code
                  break;
            }
           }
           if(HanCode.Code.Middle!=2)
           {
            switch(HanCode.Code.First)
            {
             case 0:
             case 1:
             case 2:
             case 17:
                  if(HanCode.Code.Last==1)n_offset=152;
                  else n_offset=194;
                  break;
             default:
                  if(HanCode.Code.Middle==2); // FillCode
                  if(HanCode.Code.Last==1)n_offset=173;
                  else n_offset=215;
                  break;
            }
            for(n_count=0;n_count<32;n_count++)buff[n_count]|=*(s_HanFont+n_count+((n_offset+MiddleOffset[HanCode.Code.Middle])<<5));
           }
           if(HanCode.Code.Last!=1)
           {
            switch(HanCode.Code.Middle)
            {
             case 0:
             case 1:
             case 2:
             case 3:
             case 5:
             case 14: n_offset=236; break;
             case 7:
             case 11:
             case 18:
             case 21:
             case 23:
             case 28: n_offset=263; break;
             case 4:
             case 6:
             case 10:
             case 12:
             case 15:
             case 22: n_offset=290; break;
             case 13:
             case 19:
             case 20:
             case 26:
             case 27:
             case 29: n_offset=317; break;
             default: break;
            }
            for(n_count=0;n_count<32;n_count++)buff[n_count]|=*(s_HanFont+n_count+((n_offset+LastOffset[HanCode.Code.Last])<<5));
           }
           HanPutcLow(x, y, buff, Color, BackColor);
}

void far Puts(signed int n_x, signed int n_y, signed char far *n_string, unsigned char n_color, unsigned char n_bcolor)
{
 union HanCode HanCode;
           while(*n_string)
           {
            if((*(n_string)&0x80)==0x80)
            {
             HanCode.ByteData[1]=*(n_string++); HanCode.ByteData[0]=*(n_string++);
             HanPutc(n_x, n_y, HanCode, n_color, n_bcolor); n_x+=16;
            }
            else
            {
             EngPutc(n_x, n_y, *(n_string++), n_color, n_bcolor); n_x+=8;
            }
           }
}

void far PopRec(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2)
{
           drwline(SET, 15, n_x1, n_y1, n_x2, n_y1); drwline(SET, 15, n_x1, n_y1+1, n_x1, n_y2);
           drwline(SET, 8, n_x2, n_y1+1, n_x2, n_y2); drwline(SET, 8, n_x1+1, n_y2, n_x2-1, n_y2);
}

void far PushRec(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2)
{
           drwline(SET, 8, n_x1, n_y1, n_x2, n_y1); drwline(SET, 8, n_x1, n_y1+1, n_x1, n_y2);
           drwline(SET, 15, n_x2, n_y1+1, n_x2, n_y2); drwline(SET, 15, n_x1+1, n_y2, n_x2-1, n_y2);
}

void far PopBox(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2, unsigned char n_color)
{
           drwfillbox(SET, n_color, n_x1+1, n_y1+1, n_x2-1, n_y2-1); PopRec(n_x1, n_y1, n_x2, n_y2);
}

void far PushBox(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2, unsigned char n_color)
{
           drwfillbox(SET, n_color, n_x1+1, n_y1+1, n_x2-1, n_y2-1); PushRec(n_x1, n_y1, n_x2, n_y2);
}

void far Printf(signed int n_x, signed int n_y, unsigned char n_color, unsigned char n_bcolor, signed char far *n_string, ...)
{
 signed char far *n_buffer=new(signed char far [128]);
           vsprintf(n_buffer, n_string, ...);
           Puts(n_x, n_y, n_buffer, n_color, n_bcolor); delete(n_buffer);
}

signed int far GetImageDisk(signed int x1, signed int y1, signed int x2, signed int y2, signed char far * filename, unsigned char palcolor)
{
 FILE *fw;
 signed int cx, cy, cc;
 ImageHeader ImageHeader;
 unsigned char far *n_buff;
 signed int n_buffcount;
           _dos_setfileattr(MakePath(filename), _A_ARCH); unlink(MakePath(filename));
           if((fw=fopen(MakePath(filename), "wb"))==0)return(N_False);
           n_buffcount=0; ImageHeader.id=0x39;
           ImageHeader.x1=x1; ImageHeader.y1=y1; ImageHeader.x2=x2; ImageHeader.y2=y2;
           ImageHeader.palcolor=palcolor; outportb(0x3c7, BLACK);
           for(cc=0;cc<16;cc++)
           {
            ImageHeader.pal[(cc*3)  ]=inportb(0x3c9);
            ImageHeader.pal[(cc*3)+1]=inportb(0x3c9);
            ImageHeader.pal[(cc*3)+2]=inportb(0x3c9);
           }
           fwrite(&ImageHeader, sizeof(ImageHeader), 1, fw);
           if(palcolor==N_Default)palcolor=0;
           n_buff=new(unsigned char far [N_MAXBUFFER]);
           for(cc=0;cc<5;cc++)
           {
            for(cy=y1+cc;cy<=y2;cy+=5)
            {
             for(cx=x1;cx<=x2;cx++)
             {
              *(n_buff+n_buffcount)=(unsigned char)getpoint(cx, cy)+palcolor;
              n_buffcount++;
              if(n_buffcount>=N_MAXBUFFER)
              {
               fwrite(n_buff, N_MAXBUFFER, 1, fw); n_buffcount=0;
              }
             }
             while(kbhit())getch();
            }
           }
           fwrite(n_buff, n_buffcount, 1, fw); fclose(fw);
           _dos_setfileattr(MakePath(filename), _A_ARCH|_A_RDONLY);
           delete(n_buff);
           return(N_True);
}

signed int far PutImageDisk(signed int x1, signed int y1, signed int x2, signed int y2, signed char far * filename, signed int pc)
{
 FILE *fr;
 signed int cx, cy, cc;
 ImageHeader ImageHeader;
 unsigned char palcolor;
 unsigned char far *n_buff;
 signed int n_buffcount;
 signed long n_size;
 signed long n_offset;
           _dos_setfileattr(MakePath(filename), _A_ARCH|_A_RDONLY);
           if((fr=fopen(MakePath(filename), "rb"))==0){drwfillbox(SET, LIGHTGRAY, x1, y1, x2, y2); return(N_False);}
           n_buff=new(unsigned char far [N_MAXBUFFER]);
           fseek(fr, 0, SEEK_END); n_size=ftell(fr);
           fseek(fr, 0, SEEK_SET); n_buffcount=N_MAXBUFFER;
           n_offset=0L; fread(&ImageHeader, sizeof(ImageHeader), 1, fr);
           palcolor=ImageHeader.palcolor+pc;
           if(palcolor!=N_Default)
           {
            outportb(0x3c8, palcolor);
            for(cc=0;cc<16;cc++)
            {
             outportb(0x3c9, ImageHeader.pal[(cc*3)  ]);
             outportb(0x3c9, ImageHeader.pal[(cc*3)+1]);
             outportb(0x3c9, ImageHeader.pal[(cc*3)+2]);
            }
           }
           for(cc=0;cc<5;cc++)
           {
            for(cy=y1+cc;cy<=y2;cy+=5)
            {
             for(cx=x1;cx<=x2;cx++)
             {
              if(n_buffcount>=N_MAXBUFFER)
              {
               if((n_size/(signed long)N_MAXBUFFER)<(n_offset/(signed long)N_MAXBUFFER))n_buffcount=(signed int)n_size%N_MAXBUFFER;
               else n_buffcount=N_MAXBUFFER;
               n_offset+=n_buffcount; fread(n_buff, n_buffcount, 1, fr); n_buffcount=0;
              }
              drwpoint(SET, *(n_buff+n_buffcount)+pc, cx, cy); n_buffcount++;
             }
             while(kbhit())getch();
            }
           }
           fclose(fr); delete(n_buff);
           return(N_True);
}

signed int far NonPutImageDisk(signed int x1, signed int y1, signed int x2, signed int y2, signed char far * filename, signed int pc, signed int n_col)
{
 FILE *fr;
 signed int cx, cy, cc, data;
 ImageHeader ImageHeader;
 unsigned char palcolor;
           _dos_setfileattr(MakePath(filename), _A_ARCH|_A_RDONLY);
           if((fr=fopen(MakePath(filename), "rb"))==0)return(N_False);
           fread(&ImageHeader, sizeof(ImageHeader), 1, fr);
           palcolor=ImageHeader.palcolor+pc;
           if(palcolor!=N_Default)
           {
            outportb(0x3c8, palcolor);
            for(cc=0;cc<16;cc++)
            {
             outportb(0x3c9, ImageHeader.pal[(cc*3)  ]);
             outportb(0x3c9, ImageHeader.pal[(cc*3)+1]);
             outportb(0x3c9, ImageHeader.pal[(cc*3)+2]);
            }
           }
           for(cc=0;cc<5;cc++)
           {
            for(cy=y1+cc;cy<=y2;cy+=5)
            {
             for(cx=x1;cx<=x2;cx++)
             {
              data=fgetc(fr);
              if(data!=n_col)drwpoint(SET, data+pc, cx, cy);
             }
             while(kbhit())getch();
            }
            //TimeProcess();
           }
           fclose(fr);
           return(N_True);
}


signed int far LoadPCX(signed int x, signed int y, signed char far *pcxname)
{
 FILE far *fr;
 signed int cc;
           if((fr=fopen(pcxname, "rb"))==0)return(N_False);
           fseek(fr, -768, SEEK_END); outportb(0x3c8, BLACK);
           for(cc=0;cc<48;cc++)
           {
            outportb(0x3c9, fgetc(fr)>>2); outportb(0x3c9, fgetc(fr)>>2);
            outportb(0x3c9, fgetc(fr)>>2);
           }
           fclose(fr); pcxput(SET, x, y, pcxname);
           return(N_True);
}

void far Window(signed int n_x1, signed int n_y1, signed int n_x2, signed int n_y2, signed char far *n_string, unsigned char n_color, WindowData far *WD, unsigned int n_handle)
{
           if(WD)
           {
            WD->command=WIN_NONE; WD->x1=n_x1; WD->y1=n_y1; WD->x2=n_x2; WD->y2=n_y2;
            WD->bx1=n_x2-20; WD->by1=n_y1+6; WD->bx2=n_x2-7; WD->by2=n_y1+19;
            WD->title=n_string; WD->swaphandle=n_handle;
            GetImageXMS(WD->x1, WD->y1, WD->x2, WD->y2, WD->swaphandle);
           }
           drwbox(SET, BLACK, n_x1, n_y1, n_x2, n_y2);
           PopBox(n_x1+1, n_y1+1, n_x2-1, n_y2-1, n_color);
           drwfillbox(SET, BLUE, n_x1+4, n_y1+3, n_x2-4, n_y1+22);
           NonPutImageDisk(n_x1+5, n_y1-3, n_x1+36, n_y1+28, "jackswap\\jack04.jim", 32, 32);
           Puts(n_x1+42, n_y1+5, n_string, WHITE, N_Default);
           if(WD)
           {
            PopBox(n_x2-20, n_y1+6, n_x2-7, n_y1+19, LIGHTGRAY);
            drwline(SET, BLACK, n_x2-17, n_y1+9, n_x2-10, n_y1+16);
            drwline(SET, BLACK, n_x2-10, n_y1+9, n_x2-17, n_y1+16);
            drwline(SET, BLACK, n_x2-16, n_y1+9, n_x2-11, n_y1+16);
            drwline(SET, BLACK, n_x2-11, n_y1+9, n_x2-16, n_y1+16);
           }
}

signed int far GetImageXMS(signed int x1, signed int y1, signed int x2, signed int y2, unsigned int n_handle)
{
 unsigned long n_offset;
 signed int n_boffset;
 signed int cx, cy, cc;
 unsigned char far *n_buffer=new(unsigned char far [N_MAXBUFFER]);
           n_offset=0L; n_boffset=0;
           for(cc=0;cc<5;cc++)
           {
            for(cy=y1+cc;cy<=y2;cy+=5)
            {
             for(cx=x1;cx<=x2;cx++)
             {
              *(n_buffer+n_boffset)=(unsigned char)getpoint(cx, cy);
              n_boffset++;
              if(n_boffset>=N_MAXBUFFER)
              {
               himemMEM2XMS(n_buffer, n_handle, n_offset, (unsigned long)N_MAXBUFFER);
               n_offset+=(unsigned long)N_MAXBUFFER; n_boffset=0;
              }
             }
            }
           }
           himemMEM2XMS(n_buffer, n_handle, n_offset, (unsigned long)N_MAXBUFFER);
           delete(n_buffer);
           return(N_True);
}

signed int far PutImageXMS(signed int x1, signed int y1, signed int x2, signed int y2, unsigned int n_handle)
{
 unsigned long n_offset;
 signed int n_boffset;
 signed int cx, cy, cc;
 unsigned char far *n_buffer=new(unsigned char far [N_MAXBUFFER]);
           himemXMS2MEM(n_handle, n_buffer, 0L, (signed long)N_MAXBUFFER);
           n_offset=(unsigned long)N_MAXBUFFER; n_boffset=0;
           for(cc=0;cc<5;cc++)
           {
            for(cy=y1+cc;cy<=y2;cy+=5)
            {
             for(cx=x1;cx<=x2;cx++)
             {
              drwpoint(SET, *(n_buffer+n_boffset), cx, cy); n_boffset++;
              if(n_boffset>=N_MAXBUFFER)
              {
               himemXMS2MEM(n_handle, n_buffer, n_offset, (unsigned long)N_MAXBUFFER);
               n_offset+=(unsigned long)N_MAXBUFFER; n_boffset=0;
              }
             }
            }
           }
           delete(n_buffer);
           return(N_True);
}

/* End of source */