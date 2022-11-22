/* TTY.C */

#define _TTY_C_

#include "standard.h"
#include "text.h"

unsigned int TTY_x1, TTY_y1, TTY_x2, TTY_y2;
unsigned int TTY_x, TTY_y, TTY_color;

void far pascal TTY_init(void);
void far pascal TTY_setTTY(void);
void far pascal TTY_putc(unsigned int);
void far pascal TTY_puts(void far *);
void far cdecl TTY_printf(void far *, ...);

void far pascal TTY_init(void)
{
           if(getvideomode()!=0x0003||wherex()>79U||wherey()>24U)setvideomode(0x0003);
           TTY_x1=0U; TTY_y1=0U; TTY_x2=79U; TTY_y2=24U; TTY_color=0x07;
           TEXT_cursor(1U); TEXT_cursorsize(0x0d, 0x0e); TEXT_blank(0U);
           TTY_x=wherex(); TTY_y=wherey();
           gotoxy(TTY_x, TTY_y);
}

void far pascal TTY_setTTY(void)
{
           if(TTY_x>TTY_x2)
           {
            TTY_x=TTY_x1;
            TTY_y++;
           }
           if(TTY_y>TTY_y2)
           {
            void far *s_buffer;
            TTY_y=TTY_y2;
            TTY_x=TTY_x1;
            s_buffer=farmalloc(((TTY_x2-TTY_x1)+1U)*((TTY_y2-TTY_y1)+1U));
            TEXT_gettext(TTY_x1, TTY_y1+1U, TTY_x2, TTY_y2, s_buffer);
            TEXT_puttext(TTY_x1, TTY_y1, TTY_x2, TTY_y2-1U, s_buffer);
            TEXT_linespace(TTY_x1, TTY_x2, TTY_y2, ' ', TTY_color);
            farfree(s_buffer);
           }
}

void far pascal TTY_putc(unsigned int s_char)
{
           TTY_setTTY();
           switch(s_char)
           {
            case '\n': TTY_y++; break;
            case '\r': TTY_x=TTY_x1; break;
            case '\t': TTY_x+=TABSIZE; break;
            case '\a': beep(100U); break;
            case '\b':
                 if(TTY_x>TTY_x1)TTY_x--;
                 else if(TTY_y>TTY_y1){TTY_x=TTY_x2; TTY_y--;}
                 break;
            case '\c': TTY_color=(TTY_color<<4)|(TTY_color>>4); break;
            default: TEXT_putc(TTY_x++, TTY_y, s_char, TTY_color); break;
           }
           TTY_setTTY();
           gotoxy(TTY_x, TTY_y);
}

void far pascal TTY_puts(void far *s_str)
{
           while(*((unsigned char far *)s_str))
           {
            if(*((unsigned char far *)s_str)=='^')
            {
             unsigned char s_cnum[5]; unsigned int s_cnumptr=0U;
             ((unsigned char far *)s_str)++;
             if(*((unsigned char far *)s_str)=='^')TTY_putc(*(((unsigned char far *)s_str)++));
             while(*((unsigned char far *)s_str)!='^')
             {
              if(s_cnumptr<3)s_cnum[s_cnumptr++]=*((unsigned char far *)s_str);
              ((unsigned char far *)s_str)++;
             }
             s_cnum[s_cnumptr]='\0';
             TTY_color=atoi(&s_cnum[0], 16);
             if(*((unsigned char far *)s_str))((unsigned char far *)s_str)++;
            }
            else TTY_putc(*(((unsigned char far *)s_str)++));
           }
}

void far cdecl TTY_printf(void far *s_fmt, ...)
{
 void far *s_str=farmalloc(256U);
           vsprintf(s_str, s_fmt, ...);
           TTY_puts(s_str);
           farfree(s_str);
}

/* End of source */
