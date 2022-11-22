/*
> Filename: FAT
> Testing FAT format for MS-DOS (InUse 13h interrupt)
*/

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <dir.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mem.h>
#include <alloc.h>
#include <io.h>

#define D_FIRSTDRIVE         0

#define D_LOAD               0
#define D_UPLOAD             1
#define D_DOWNLOAD           2

#define D_LEFT               0
#define D_RIGHT              1

#define D_KEYINTERRUPT       0x09

int main(void);
void FAT_lputc(int, int, int);
void FAT_putc(int, int, int, int);
int FAT_lgetc(int, int);
void FAT_puts(int, void *, int, int);
void FAT_nputs(int, int, void *, int, int);
void FAT_printf(int, int, int, void *, ...);
void FAT_hline(int, int, int, int, int);
void FAT_vline(int, int, int, int, int);
void FAT_box(int, int, int, int, int, int);
void FAT_rectangle(int, int, int, int, int, int);
void FAT_gettext(void *, int, int, int, int);
void FAT_puttext(void *, int, int, int, int);
void FAT_help(void *, ...);
void FAT_load(int);
void FAT_view(void);
void FAT_scrollup(void);
void FAT_scrolldown(void);
int FAT_absread(int, int, long, void *);
void interrupt FAT_key(void);
void FAT_initvalue(void);

static char FAT_rectable[4][11]={
      {196, 179, 218, 194, 191, 195, 197, 180, 192, 193, 217},
      {205, 186, 201, 203, 187, 204, 206, 185, 200, 202, 188},
      {205, 179, 213, 209, 184, 198, 216, 181, 212, 207, 190},
      {196, 186, 214, 210, 183, 199, 215, 182, 211, 208, 189}
     };

static char *FAT_sectorbuffer;
static int   FAT_drive, FAT_viewoffset, FAT_cursor, FAT_tab;
static long  FAT_sector;
static char *FAT_string[]={
              "Reading...",
              " :> Welcome to the disk editor~, Copyright(c)1998-1999 by Cho JaeHyuk",
              "  MINZ sector editor for MS-DOS  ",
              NULL
             };
static void interrupt (*FAT_oldkey)(void);
static char *FAT_keytable;

int main(void)
{
 char *s_prevscreen=(char *)malloc(4000);
 int s_prevx=wherex(), s_prevy=wherey();
 union REGS s_register;
           while(kbhit())getch();
           FAT_gettext(s_prevscreen, 0, 0, 79, 24);
           s_register.x.ax=0x0003;
           int86(0x10, &s_register, &s_register);
           FAT_box(0x08, 0xb0, 0, 1, 79, 23);
           FAT_hline(0x7f, ' ', 0, 79, 0);
           FAT_help(FAT_string[1]);
           FAT_puts(0x71, FAT_string[2], 23, 0);

           FAT_sectorbuffer=(char *)malloc(512*3);
           FAT_drive=D_FIRSTDRIVE;
           FAT_initvalue();
           FAT_keytable=(char *)malloc(128);
           memset(FAT_keytable, 0, 512);
           FAT_oldkey=getvect(D_KEYINTERRUPT);
           setvect(D_KEYINTERRUPT, FAT_key);
 l_key_0000:;
           if(FAT_keytable[0x01])goto l_key_0001;
           if(FAT_keytable[0x48])
           {
            FAT_keytable[0x48]=0;
            FAT_cursor-=16;
            if(FAT_cursor<0)
            {
             FAT_cursor+=16;
             FAT_scrollup();
            }
            FAT_view();
           }
           if(FAT_keytable[0x50])
           {
            FAT_keytable[0x50]=0;
            FAT_cursor+=16;
            if(FAT_cursor>=256)
            {
             FAT_cursor-=16;
             FAT_scrolldown();
            }
            FAT_view();
           }
           if(FAT_keytable[0x4b])
           {
            FAT_keytable[0x4b]=0;
            FAT_cursor--;
            if(FAT_cursor<0)
            {
             FAT_cursor+=16;
             FAT_scrollup();
            }
            FAT_view();
           }
           if(FAT_keytable[0x4d])
           {
            FAT_keytable[0x4d]=0;
            FAT_cursor++;
            if(FAT_cursor>=256)
            {
             FAT_cursor-=16;
             FAT_scrolldown();
            }
            FAT_view();
           }
           if(FAT_keytable[0x49]&&FAT_sector)
           {
            FAT_keytable[0x49]=0;
            FAT_sector--;
            FAT_load(D_UPLOAD);
            FAT_view();
           }
           if(FAT_keytable[0x51])
           {
            FAT_keytable[0x51]=0;
            FAT_sector++;
            FAT_load(D_DOWNLOAD);
            FAT_view();
           }
           if(FAT_keytable[0x47]&&FAT_sector)
           {
            FAT_keytable[0x47]=0;
            FAT_sector=0;
            FAT_viewoffset=512;
            FAT_load(D_LOAD);
            FAT_view();
           }
           if(FAT_keytable[0x0f])
           {
            FAT_keytable[0x0f]=0;
            if(FAT_tab==0)FAT_tab=1;
            else FAT_tab=0;
            FAT_view();
           }
           while(kbhit())getch();
           goto l_key_0000;
 l_key_0001:;
           setvect(D_KEYINTERRUPT, FAT_oldkey);
           free(FAT_keytable);
           free(FAT_sectorbuffer);
           s_register.x.ax=0x0003;
           int86(0x10, &s_register, &s_register);
           FAT_puttext(s_prevscreen, 0, 0, 79, 24);
           gotoxy(s_prevx, s_prevy);
           free(s_prevscreen);
           while(kbhit())getch();
	   return(1);
}

void FAT_lputc(int s_attrib, int s_x, int s_y)
{
	   *((int far *)0xb8000000LU+(s_y*80)+s_x)=s_attrib;
}

void FAT_putc(int s_color, int s_char, int s_x, int s_y)
{
	   FAT_lputc(((s_color&0xff)<<8)|(s_char&0xff), s_x, s_y);
}

int FAT_lgetc(int s_x, int s_y)
{
	   return(*((int far *)0xb8000000LU+(s_y*80)+s_x));
}

void FAT_puts(int s_color, void *s_string, int s_x, int s_y)
{
	   while(*((char *)s_string))FAT_putc(s_color, *(((char *)s_string)++), s_x++, s_y);
}

void FAT_nputs(int s_color, int s_length, void *s_string, int s_x, int s_y)
{
           while(s_length--)FAT_putc(s_color, *(((char *)s_string)++), s_x++, s_y);
}

void FAT_printf(int s_color, int s_x, int s_y, void *s_fmt, ...)
{
 char *s_string=(char *)malloc(512);
           vsprintf(s_string, s_fmt, ...);
           FAT_puts(s_color, s_string, s_x, s_y);
           free(s_string);
}

void FAT_hline(int s_color, int s_char, int s_x1, int s_x2, int s_y)
{
	   while(s_x1<=s_x2)FAT_putc(s_color, s_char, s_x1++, s_y);
}

void FAT_vline(int s_color, int s_char, int s_x, int s_y1, int s_y2)
{
	   while(s_y1<=s_y2)FAT_putc(s_color, s_char, s_x, s_y1++);
}

void FAT_box(int s_color, int s_char, int s_x1, int s_y1, int s_x2, int s_y2)
{
	   while(s_y1<=s_y2)FAT_hline(s_color, s_char, s_x1, s_x2, s_y1++);
}

void FAT_rectangle(int s_color, int s_type, int s_x1, int s_y1, int s_x2, int s_y2)
{
           FAT_hline(s_color, FAT_rectable[s_type][0], s_x1+1, s_x2-1, s_y1);
           FAT_hline(s_color, FAT_rectable[s_type][0], s_x1+1, s_x2-1, s_y2);
           FAT_vline(s_color, FAT_rectable[s_type][1], s_x1, s_y1+1, s_y2-1);
           FAT_vline(s_color, FAT_rectable[s_type][1], s_x2, s_y1+1, s_y2-1);
           FAT_putc(s_color, FAT_rectable[s_type][2], s_x1, s_y1);
           FAT_putc(s_color, FAT_rectable[s_type][4], s_x2, s_y1);
           FAT_putc(s_color, FAT_rectable[s_type][8], s_x1, s_y2);
           FAT_putc(s_color, FAT_rectable[s_type][10], s_x2, s_y2);
}

void FAT_gettext(void *s_buffer, int s_x1, int s_y1, int s_x2, int s_y2)
{
 int s_cx;
	   while(s_y1<=s_y2)
	   {
	    s_cx=s_x1;
	    while(s_cx<=s_x2)
	    {
	     *(((int *)s_buffer)++)=FAT_lgetc(s_cx, s_y1);
	     s_cx++;
	    }
	    s_y1++;
	   }
}

void FAT_puttext(void *s_buffer, int s_x1, int s_y1, int s_x2, int s_y2)
{
 int s_cx;
	   while(s_y1<=s_y2)
	   {
	    s_cx=s_x1;
	    while(s_cx<=s_x2)
	    {
	     FAT_lputc(*(((int *)s_buffer)++), s_cx, s_y1);
	     s_cx++;
	    }
	    s_y1++;
	   }
}

void FAT_help(void *s_helpstring, ...)
{
 char *s_string=(char *)malloc(512);
           vsprintf(s_string, s_helpstring, ...);
           FAT_puts(0x70, s_string, 0, 24);
           FAT_hline(0x70, ' ', strlen(s_string), 79, 24);
           free(s_string);
}

void FAT_load(int s_type)
{
           FAT_box(0x78, ' ', 30, 12, 49, 14);
           FAT_rectangle(0x78, 3, 31, 12, 48, 14);
           FAT_puts(0x70, FAT_string[0], 35, 13);
           while(kbhit())getch();
           switch(s_type)
           {
            case D_LOAD:
                 if(FAT_sector)FAT_absread(FAT_drive, 1, FAT_sector-1L, FAT_sectorbuffer);
                 else memset(FAT_sectorbuffer, 0, 512);
                 FAT_absread(FAT_drive, 1, FAT_sector  , FAT_sectorbuffer+512);
                 FAT_absread(FAT_drive, 1, FAT_sector+1, FAT_sectorbuffer+512+512);
                 break;
            case D_UPLOAD:
                 memcpy(FAT_sectorbuffer+512+512, FAT_sectorbuffer+512, 512);
                 memcpy(FAT_sectorbuffer+512, FAT_sectorbuffer, 512);
                 if(FAT_sector)FAT_absread(FAT_drive, 1, FAT_sector-1, FAT_sectorbuffer);
                 break;
            case D_DOWNLOAD:
                 memcpy(FAT_sectorbuffer, FAT_sectorbuffer+512, 512);
                 memcpy(FAT_sectorbuffer+512, FAT_sectorbuffer+512+512, 512);
                 FAT_absread(FAT_drive, 1, FAT_sector+1, FAT_sectorbuffer+512+512);
                 break;
           default: break;
           }
           while(kbhit())getch();
}

void FAT_view(void)
{
 int s_count, s_cx, s_cy, s_bx=2, s_by=3, s_bcolor=0x1f, s_bcolorA=0x1e, s_bcolorB=0x7f, s_temp;
 char *s_printbuffer=FAT_sectorbuffer+FAT_viewoffset;
           for(s_cx=s_bx+8, s_count=0;s_cx<(s_bx+(3<<4))+8;s_cx+=3, s_count++)FAT_printf(s_bcolorA, s_cx, s_by+1, "+%X ", s_count);
           FAT_puts(s_bcolorA, "Offset ", s_bx+1, s_by+1);
           FAT_printf(s_bcolorA, s_bx+(3<<4)+8, s_by+1, " Sector: %08lxh ", FAT_sector);
           FAT_rectangle(s_bcolor, 1, s_bx, s_by, s_bx+(3<<4)+27, 22);
           FAT_hline(s_bcolor, FAT_rectable[0][0], s_bx+1, s_bx+(3<<4)+26, s_by+2);
           for(s_cy=s_by+3, s_count=0;s_cy<=21;s_cy++)
           {
            FAT_nputs(s_bcolorB, 16, s_printbuffer, s_bx+(3<<4)+10, s_cy);
            FAT_putc(s_bcolor, FAT_rectable[0][1], s_bx+(3<<4)+8, s_cy);
            FAT_putc(s_bcolor, ' ', s_bx+(3<<4)+7+2, s_cy);
            FAT_putc(s_bcolor, ' ', s_bx+(3<<4)+7+16+3, s_cy);
            s_temp=(s_count+FAT_viewoffset)-512;
            FAT_printf(s_bcolorA, s_bx+1, s_cy, " %03X ", s_temp);
            FAT_putc(s_bcolor, FAT_rectable[0][1], s_bx+6, s_cy);
            FAT_putc(s_bcolor, ' ', s_bx+7, s_cy);
            for(s_cx=s_bx+8;s_cx<(s_bx+(3<<4))+8;s_cx+=3, s_count++)
            {
             if(s_count==FAT_cursor)
             {
              int s_xx=(s_count%16)+s_bx+(3<<4)+10, s_getc;
              s_getc=FAT_lgetc(s_xx, s_cy);
              FAT_lputc((s_getc&0x00ff)|0x0e00, s_xx, s_cy);
              FAT_printf((s_bcolor&0x0f), s_cx, s_cy, "%02X", *(s_printbuffer++)&0xff);
              FAT_putc(s_bcolor, ' ', s_cx+2, s_cy);
              if(FAT_tab==D_LEFT)gotoxy(s_cx+1, s_cy+1);
              if(FAT_tab==D_RIGHT)gotoxy(s_xx+1, s_cy+1);
             }
             else FAT_printf(s_bcolor, s_cx, s_cy, "%02X ", *(s_printbuffer++)&0xff);
            }
           }
}

void FAT_scrollup(void)
{
           if(FAT_viewoffset>=16&&!(FAT_sector==0&&FAT_viewoffset<=512))
           {
            FAT_viewoffset-=16;
            if(FAT_viewoffset<=0)
            {
             FAT_viewoffset=512;
             FAT_sector--;
             FAT_load(D_UPLOAD);
            }
           }
}

void FAT_scrolldown(void)
{
           FAT_viewoffset+=16;
           if(FAT_viewoffset>(512+512))
           {
            FAT_viewoffset=512;
            FAT_sector++;
            FAT_load(D_DOWNLOAD);
           }
}

int FAT_absread(int s_drive, int s_nsects, long s_lsect, void *s_buffer)
{
           return(absread(s_drive, s_nsects, s_lsect, s_buffer));
}

void interrupt FAT_key(void)
{
 char s_key;
           asm in al, 0x60;
           asm mov byte ptr s_key, al;
           if(s_key&0x80)FAT_keytable[s_key&0x7f]=0;
           else FAT_keytable[s_key]=1;
           FAT_oldkey();
}

void FAT_initvalue(void)
{
           FAT_sector=0L;
           FAT_viewoffset=512;
           FAT_cursor=0;
           FAT_tab=D_LEFT;
           FAT_load(D_LOAD);
           FAT_view();
}

/* End of source */