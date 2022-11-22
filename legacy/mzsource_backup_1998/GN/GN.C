#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <graphics.h>
#include <stdlib.h>
#include <string.h>
#include <alloc.h>]
#include <memory.h>
#include <io.h>
#include <process.h>
#include <time.h>

#define LEFT_SPACE                     50
#define UP_SPACE                       50
#define HLINE_COLOR                    LIGHTGRAY
#define VLINE_COLOR                    LIGHTGRAY
#define VICTORY_COLOR                  YELLOW
#define UPLINE_GRID                    20
#define MAX_MAN                        9
#define TIMER_INTERRUPT                0x1c

int g_gd=DETECT, g_gm=VGAHI;
void *g_title="Line linker v1.00";
int g_victory[MAX_MAN];
int g_grid;

void my_delay(int s_delay)
{
 int s_store;
 int s_timer;
 int s_count;
           s_store=inportb(0x40);
           while(s_delay--)
           {
            for(s_count=0;s_count<17;s_count++)
            {
             do
             {
              s_timer=inportb(0x40);
             }while(s_store==s_timer);
             s_store=s_timer;
            }
           }
}

int trace(int s_number)
{
 int s_locate_x, s_locate_y;
 int s_flag=0;
 int s_color;
           s_locate_x=LEFT_SPACE+((s_number-1)*g_grid);
           s_locate_y=UP_SPACE;
           srand(inportb(0x40));
           do
           {
            do
            {
             s_color=random(15)+1;
            }while(s_color==HLINE_COLOR||s_color==VLINE_COLOR);
           }while(getpixel(s_locate_x, s_locate_y)==s_color);
           do
           {
            putpixel(s_locate_x, s_locate_y, s_color);
            if(getpixel(s_locate_x+1, s_locate_y+1)&&s_flag!=8&&s_flag!=5&&s_flag!=3)
            {
             s_locate_x++;
             s_locate_y++;
             s_flag=1;
            }
            else if(getpixel(s_locate_x-1, s_locate_y+1)&&s_flag!=6&&s_flag!=4&&s_flag!=1)
            {
             s_locate_x--;
             s_locate_y++;
             s_flag=3;
            }
            else if(getpixel(s_locate_x+1, s_locate_y)&&s_flag!=8&&s_flag!=5&&s_flag!=3)
            {
             s_locate_x++;
             s_flag=4;
            }
            else if(getpixel(s_locate_x-1, s_locate_y)&&s_flag!=6&&s_flag!=4&&s_flag!=1)
            {
             s_locate_x--;
             s_flag=5;
            }
            else if(getpixel(s_locate_x+1, s_locate_y-1)&&s_flag!=8&&s_flag!=5&&s_flag!=3)
            {
             s_locate_x++;
             s_locate_y--;
             s_flag=6;
            }
            else if(getpixel(s_locate_x-1, s_locate_y-1)&&s_flag!=6&&s_flag!=4&&s_flag!=1)
            {
             s_locate_x--;
             s_locate_y--;
             s_flag=8;
            }
            else if(s_flag!=7)
            {
             s_locate_y++;
             s_flag=2;
            }
            if(!kbhit())my_delay(300);
           }while(s_locate_y<(getmaxy()-UP_SPACE));
           while(kbhit())getch();
           return((s_locate_x-UP_SPACE)/g_grid);
}

void interrupt (*pre_interrupt_1c)(void);

void interrupt interrupt_1c(void)
{
 static int s_blue=20;
 static int s_blue_flag=0;
           pre_interrupt_1c();
           outportb(0x3c8, HLINE_COLOR);
           outportb(0x3c9, s_blue>>1);
           outportb(0x3c9, s_blue>>1);
           outportb(0x3c9, s_blue);
           if(s_blue_flag)s_blue--;
           else s_blue++;
           if(s_blue==20)s_blue_flag=0;
           if(s_blue==30)s_blue_flag=1;
}

int main(void)
{
 int s_line_count, s_victory_count;
 char s_str_number[48];
           srand(inportb(0x40));
           printf("%s by JaeHyuk.CHO\n", g_title);
           do
           {
            printf("#input line count[EXIT<=1, x<=%d]: ", MAX_MAN);
            scanf("%d", &s_line_count);
           }while(s_line_count>MAX_MAN);
           if(s_line_count>1)
           {
            do
            {
             printf("#input victory count[1-%d]: ", s_line_count);
             scanf("%d", &s_victory_count);
            }while(s_victory_count>s_line_count);
            registerbgidriver(EGAVGA_driver);
            initgraph(&g_gd, &g_gm, NULL);
            pre_interrupt_1c=getvect(TIMER_INTERRUPT);
            setvect(TIMER_INTERRUPT, interrupt_1c);
            while(s_line_count>1)
            {
             int s_count, s_count_00, s_count_01;
             int s_random0, s_random1;
             int s_key;
             cleardevice();
             srand(inportb(0x40));
             for(s_count=0;s_count<MAX_MAN;s_count++)g_victory[s_count]=0;
             for(s_count=0;s_count<s_victory_count;s_count++)
             {
              do
              {
               s_count_00=random(s_line_count);
              }while(g_victory[s_count_00]);
              g_victory[s_count_00]=1;
             }
             while(kbhit())getch();
             my_delay(3000);
             setcolor(VLINE_COLOR);
             g_grid=(getmaxx()-(LEFT_SPACE<<1))/(s_line_count-1);
             s_count=0;
             while(s_count<g_grid)
             {
              setcolor(BLACK);
              for(s_count_00=0, s_count_01=LEFT_SPACE;s_count_00<s_line_count;s_count_00++, s_count_01+=s_count)
              {
               line(s_count_01, UP_SPACE, s_count_01, getmaxy()-UP_SPACE);
              }
              s_count++;
              setcolor(VLINE_COLOR);
              for(s_count_00=0, s_count_01=LEFT_SPACE;s_count_00<s_line_count;s_count_00++, s_count_01+=s_count)
              {
               line(s_count_01, UP_SPACE, s_count_01, getmaxy()-UP_SPACE);
              }
              my_delay(400);
             }
             for(s_count=0;s_count<s_line_count;s_count++)
             {
              itoa(s_count+1, s_str_number, 10);
              setcolor(WHITE);
              outtextxy((s_count*g_grid)+LEFT_SPACE-4, UP_SPACE-16, s_str_number);
              if(s_count<(s_line_count-1))
              {
               setcolor(HLINE_COLOR);
               for(s_count_00=UP_SPACE;s_count_00<(getmaxy()-UP_SPACE-UPLINE_GRID);s_count_00+=UPLINE_GRID)
               {
                l_re_rendom:;
                s_random0=random(UPLINE_GRID)+s_count_00;
                for(s_count_01=0;s_count_01<5;s_count_01++)
                {
                 if(getpixel((s_count*g_grid)+LEFT_SPACE-1, s_random0+s_count_01)==HLINE_COLOR)goto l_re_rendom;
                 if(getpixel((s_count*g_grid)+LEFT_SPACE-1, s_random0-s_count_01)==HLINE_COLOR)goto l_re_rendom;
                }
                s_random1=random(UPLINE_GRID)+s_count_00;
                line((s_count*g_grid)+LEFT_SPACE, s_random0,(s_count*g_grid)+LEFT_SPACE+g_grid, s_random1);
               }
              }
              setcolor(VICTORY_COLOR);
              if(g_victory[s_count])strcpy(&s_str_number[0], "WIN");
              else strcpy(&s_str_number[0], "...");
              outtextxy((s_count*g_grid)+LEFT_SPACE-12, getmaxy()-UP_SPACE+4, s_str_number);
              my_delay(2500);
             }
             setcolor(YELLOW);
             sprintf(&s_str_number[0], "Select number [0=END, 1-%d]?", s_line_count);
             outtextxy(0, 0, &s_str_number[0]);
             l_re_trace:;
             do
             {
              while(kbhit())getch();
              s_key=getch();
              while(kbhit())getch();
             }while(s_key<'0'&&s_key>'9');
             if(s_key>='1'&&s_key<='9')
             {
              int s_victory_man;
              s_victory_man=trace(s_key-'0');
              itoa(s_key-'0', s_str_number, 10);
              if(g_victory[s_victory_man])setcolor(LIGHTBLUE);
              else setcolor(LIGHTRED);
              outtextxy((s_victory_man*g_grid)+LEFT_SPACE-4, getmaxy()-UP_SPACE+16+4, s_str_number);
              goto l_re_trace;
             }
             strcpy(&s_str_number[0], "Replay? [Y/n]");
             cleardevice();
             setcolor(YELLOW);
             outtextxy((getmaxx()>>1)-(strlen(&s_str_number[0])<<2), (getmaxy()>>1)-8, &s_str_number[0]);
             while(kbhit())getch();
             s_key=getch();
             while(kbhit())getch();
             if(s_key=='n'||s_key=='N'||s_key==0x1b)break;
            }
            setvect(TIMER_INTERRUPT, pre_interrupt_1c);
            closegraph();
           }
           printf("End of %s\n", g_title);
           return(0);
}

/* End of source */
