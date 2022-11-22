/*
  Name: SUGA O/S
  File: SUGA.CPP
  Copy: Copyright(c)1998-1999 by Cho JaeHyuk
  What: Main code of SUGA
*/

#define FILE_SUGA_CPP                  "SUGA.CPP"

#include <SUGA.H>
#include <KERNEL.H>
#include <FILE.H>
#include <KEY.H>
#include <GRAPHICS.H>

unsigned int far cdecl main(void);
void far pascal start_screen(void);

t_graphics far *             g_graphics;

#include <test.cpp>

unsigned int far cdecl main(void)
{
 unsigned char far *s_scr=new unsigned char [8000U];
           g_text->gettext(s_scr, g_text->minx, g_text->miny, g_text->maxx, g_text->maxy);
           g_graphics=new t_graphics(D_GRAPHICS_800x600);
           start_screen();
           test();
           while(True)
           {
            if(g_key[0x01]==True)
            {
             while(g_key[0x01]==True)while(bioskey(0x0001))bioskey(0x0000);
             break;
            }
            while(bioskey(0x0001))bioskey(0x0000);
           }
           g_graphics->closegraph();
           delete(g_graphics);
           g_text->puttext(s_scr, g_text->minx, g_text->miny, g_text->maxx, g_text->maxy);
           g_text->gotoxy(g_text->tty_x, g_text->tty_y);
           delete(s_scr);
           return(Success);
}

void far pascal start_screen(void)
{
 void far *s_title="·q¬÷·¥¯¢ ¶…µwÁA¹A \`­A¹·”µ×\' [ÐeŠ‚Ìe]";
           g_graphics->clear(0x00);
           g_graphics->fillbox(0x08, g_graphics->minx, g_graphics->miny, g_graphics->maxx, g_graphics->miny+21U);
           g_graphics->hline(0x07, g_graphics->minx, g_graphics->maxx, g_graphics->miny+22U);
           g_graphics->ehputs(0x0f, D_DEFAULT_COLOR, s_title, g_graphics->minx+5U, g_graphics->miny+3U);
           test();
}

/* End of source */