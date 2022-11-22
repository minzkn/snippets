  /***************************************
   *         Copyright(c)1998-1998       *
   *       MC Compiler source (TEXT)     *
   *         Code by Cho JaeHyuk         *
   *          MINZ (02)822-2744          *
   ***************************************/

/* TEXT.C */

#define _TEXT_C_

#include "standard.h"

const unsigned char TEXT_line[][11]={
                               {0xb3, 0xc4, 0xda, 0xc2, 0xbf, 0xc3, 0xc5, 0xb4, 0xc0, 0xc1, 0xd9},
                               {0xba, 0xcd, 0xc9, 0xc8, 0xbb, 0xcc, 0xce, 0xb9, 0xc8, 0xca, 0xbc},
                               {0xb3, 0xcd, 0xd5, 0xd1, 0xb8, 0xc6, 0xd8, 0xb5, 0xd4, 0xcf, 0xbe},
                               {0xba, 0xc4, 0xd6, 0xd2, 0xb7, 0xc7, 0xd7, 0xb6, 0xd3, 0xd0, 0xbd}
                              };

unsigned int pascal TEXT_getc(unsigned int, unsigned int);
void far pascal TEXT_putc(unsigned int, unsigned int, unsigned int, unsigned int);
void far pascal TEXT_wputc(unsigned int, unsigned int, unsigned int);
void far pascal TEXT_attrputc(unsigned int, unsigned int, unsigned int);
void far pascal TEXT_puts(unsigned int, unsigned int, void far *, unsigned int);
void far cdecl  TEXT_printf(unsigned int, unsigned int, unsigned int, void far *, ...);
void far pascal TEXT_linespace(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void far pascal TEXT_box(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void far pascal TEXT_rec(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void far pascal TEXT_boxwindow(unsigned int, unsigned int, unsigned int, unsigned int, void far *,\
                unsigned int, unsigned int, unsigned int);
void far pascal TEXT_cursorsize(unsigned int, unsigned int);
void far pascal TEXT_cursor(unsigned int);
void far pascal TEXT_blank(unsigned int);
void far pascal TEXT_gettext(unsigned int, unsigned int, unsigned int, unsigned int, void far *);
void far pascal TEXT_puttext(unsigned int, unsigned int, unsigned int, unsigned int, void far *);
void far cdecl TEXT_mprintf(unsigned int, unsigned int, unsigned int, unsigned int, void far *, ...);
void far pascal TEXT_cputs(unsigned int, unsigned int, void far *, unsigned int);
void far cdecl TEXT_cprintf(unsigned int, unsigned int, unsigned int, void far *, ...);

unsigned int pascal TEXT_getc(unsigned int s_x, unsigned int s_y)
{
           return(peekw(0xb800, (s_y*160U)+(s_x<<1)));
}

void far pascal TEXT_putc(unsigned int s_x, unsigned int s_y, unsigned int s_char, unsigned int s_color)
{
           pokew(0xb800, (s_y*160U)+(s_x<<1), s_char|(s_color<<8));
}

void far pascal TEXT_wputc(unsigned int s_x, unsigned int s_y, unsigned int s_word)
{
           pokew(0xb800, (s_y*160U)+(s_x<<1), s_word);
}

void far pascal TEXT_attrputc(unsigned int s_x, unsigned int s_y, unsigned int s_color)
{
           pokeb(0xb800, (s_y*160U)+(s_x<<1)+1U, s_color);
}

void far pascal TEXT_puts(unsigned int s_x, unsigned int s_y, void far *s_str, unsigned int s_color)
{
           while(*((unsigned char far *)s_str))TEXT_putc(s_x++, s_y, *(((unsigned char far *)s_str)++), s_color);
}

void far cdecl TEXT_printf(unsigned int s_x, unsigned int s_y, unsigned int s_color, void far *s_fmt, ...)
{
 void far *s_str=farmalloc(256U); vsprintf(s_str, s_fmt, ...);
           TEXT_puts(s_x, s_y, s_str, s_color); farfree(s_str);
}

void far pascal TEXT_linespace(s_x1, s_x2, s_y, s_char, s_color)
unsigned int s_x1, s_x2, s_y, s_char, s_color;
{
           while(s_x1<=s_x2)TEXT_putc(s_x1++, s_y, s_char, s_color);
}

void far pascal TEXT_box(s_x1, s_y1, s_x2, s_y2, s_char, s_color)
unsigned int s_x1, s_y1, s_x2, s_y2, s_char, s_color;
{
           while(s_y1<=s_y2)TEXT_linespace(s_x1, s_x2, s_y1++, s_char, s_color);
}

void far pascal TEXT_rec(s_x1, s_y1, s_x2, s_y2, s_color, s_line)
unsigned int s_x1, s_y1, s_x2, s_y2, s_color, s_line;
{
           TEXT_linespace(s_x1+1U, s_x2-1U, s_y1, TEXT_line[s_line][1], s_color);
           TEXT_linespace(s_x1+1U, s_x2-1U, s_y2, TEXT_line[s_line][1], s_color);
           TEXT_putc(s_x1, s_y1, TEXT_line[s_line][2], s_color);
           TEXT_putc(s_x2, s_y1, TEXT_line[s_line][4], s_color);
           TEXT_putc(s_x1, s_y2, TEXT_line[s_line][8], s_color);
           TEXT_putc(s_x2, s_y2, TEXT_line[s_line][10], s_color); s_y1++;
           while(s_y1<s_y2)
           {
            TEXT_putc(s_x1, s_y1, TEXT_line[s_line][0], s_color);
            TEXT_putc(s_x2, s_y1, TEXT_line[s_line][0], s_color);
            s_y1++;
           }
}

void far pascal TEXT_boxwindow(s_x1, s_y1, s_x2, s_y2, s_title, s_color, s_tcolor, s_line)
unsigned int s_x1, s_y1, s_x2, s_y2, s_color, s_tcolor, s_line; void far *s_title;
{
           TEXT_box(s_x1, s_y1, s_x2, s_y2, ' ', s_color);
           TEXT_rec(s_x1+1U, s_y1, s_x2-1U, s_y2, s_color, s_line);
           TEXT_linespace(s_x1+2U, s_x2-2U, s_y1+1U, ' ', s_tcolor);
           TEXT_puts(s_x1+((s_x2-s_x1+1U)>>1)-(strlen(s_title)>>1), s_y1+1U, s_title, s_tcolor);
           while(s_x1<=s_x2){TEXT_attrputc(s_x1+1U, s_y2+1U, 0x08); s_x1++;}
           while(s_y1<=s_y2){TEXT_attrputc(s_x2+1U, s_y1+1U, 0x08); s_y1++;}
}

void far pascal TEXT_cursorsize(unsigned int s_start, unsigned int s_end)
{
           outx(0x03d4, 0x000a, s_start); outx(0x03d4, 0x000b, s_end);
}

void far pascal TEXT_cursor(unsigned int s_sw)
{
           if(s_sw)TEXT_cursorsize(0x0d, 0x0e);
           else TEXT_cursorsize(0x20, 0x00);
}

void far pascal TEXT_blank(unsigned int s_sw)
{
 unsigned int s_data; inportb(0x03d4+6U); s_data=inx(0x03c0, 0x10);
           if(s_sw)s_data|=0x08;
           else s_data&=0xf7;
           outportb(0x03c0, s_data); inportb(0x03d4+6U); outportb(0x03c0, 0x20);
}

void far pascal TEXT_gettext(s_x1, s_y1, s_x2, s_y2, s_buffer)
unsigned int s_x1, s_y1, s_x2, s_y2; void far *s_buffer;
{
 unsigned int s_xc;
           while(s_y1<=s_y2)
           {s_xc=s_x1;
            while(s_xc<=s_x2){*(((unsigned int far *)s_buffer)++)=TEXT_getc(s_xc++, s_y1);}
            s_y1++;}
}

void far pascal TEXT_puttext(s_x1, s_y1, s_x2, s_y2, s_buffer)
unsigned int s_x1, s_y1, s_x2, s_y2; void far *s_buffer;
{
 unsigned int s_xc;
           while(s_y1<=s_y2)
           {s_xc=s_x1;
            while(s_xc<=s_x2){TEXT_wputc(s_xc++, s_y1, *(((unsigned int far *)s_buffer)++));}
            s_y1++;}
}

void far cdecl TEXT_mprintf(unsigned int s_x1, unsigned int s_x2, unsigned int s_y, unsigned int s_color, void far *s_fmt, ...)
{
 void far *s_str=farmalloc(256U);
           vsprintf(s_str, s_fmt, ...);
           TEXT_puts(s_x1+((s_x2-s_x1+1U)>>1)-(strlen(s_str)>>1), s_y, s_str, s_color);
           farfree(s_str);
}

void far pascal TEXT_cputs(unsigned int s_x, unsigned int s_y, void far *s_str, unsigned int s_color)
{
           while(*((unsigned char far *)s_str))
           {
            if(*((unsigned char far *)s_str)=='\^')
            {
             unsigned char s_buff[20]; unsigned int s_ptr=0U;
             ((unsigned char far *)s_str)++;
             if(*((unsigned char far *)s_str)=='\^')
             {
              TEXT_putc(s_x++, s_y, *(((unsigned char far *)s_str)++), s_color);
             }
             else
             {
              while(isdigit(*((unsigned char far *)s_str)))
              {
               s_buff[s_ptr++]=*(((unsigned char far *)s_str)++);
               if(s_ptr>=19)break;
              }
              s_buff[s_ptr]='\0';
              s_color=atoi(&s_buff[0], 10U);
             }
            }
            else TEXT_putc(s_x++, s_y, *(((unsigned char far *)s_str)++), s_color);
           }
}

void far cdecl TEXT_cprintf(unsigned int s_x, unsigned int s_y, unsigned int s_color, void far *s_fmt, ...)
{
 void far *s_str=farmalloc(256U); vsprintf(s_str, s_fmt, ...);
           TEXT_cputs(s_x, s_y, s_str, s_color); farfree(s_str);
}

/* End of source */
