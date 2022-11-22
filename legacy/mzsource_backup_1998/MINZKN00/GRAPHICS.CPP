/*
  Name: SUGA O/S
  File: GRAPHICS.CPP
  Copy: Copyright(c)1998-1999 by Cho JaeHyuk
  What: Graphics routine
*/

#define FILE_GRAPHICS_CPP              "GRAPHICS.CPP"

#include <KERNEL.H>
#include <GRAPHICS.H>

void far pascal bank_cirrus(unsigned int);

unsigned char far HAN_table0_[32U] ={0x07, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0xff, 0x09, 0x0b, 0x0b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; // Á¡¬÷
unsigned char far HAN_table1_[32U] ={0xff, 0xff, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0xff, 0xff, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0xff, 0xff, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0xff, 0xff, 0x11, 0x12, 0x13, 0x14, 0x00, 0x01}; // º—¬÷
unsigned char far HAN_table2_[32U] ={0xff, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x14, 0x14}; // ¹·¬÷
unsigned char far HAN_ftable0_[21U]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x01, 0x02, 0x04, 0x04, 0x04, 0x02, 0x01, 0x03, 0x00}; // ¹·¬÷·¡ ´ô“e ‰w¶ ¤é®
unsigned char far HAN_ftable1_[21U]={0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x07, 0x07, 0x07, 0x06, 0x06, 0x07, 0x07, 0x07, 0x06, 0x06, 0x07, 0x05}; // ¹·¬÷·¡ ·¶“e ‰w¶ ¤é®
unsigned char far HAN_ltable_[21U] ={0x00, 0x02, 0x00, 0x02, 0x01, 0x02, 0x01, 0x02, 0x03, 0x00, 0x02, 0x01, 0x03, 0x03, 0x01, 0x02, 0x01, 0x03, 0x03, 0x01, 0x01}; // º—¬÷µA ˜aŸa“e ¹·¬÷ ¤é®
                                    //„a    „    „¡    „Á    „á    …A    …a    …    …¡    …Á    …á    †A    †a    †    †¡    †Á    †á    ‡A    ‡a    ‡    ‡¡

static void far pascal ___memsetd(void far *s_ptr, unsigned long s_data, unsigned int s_length)
{
           asm_push_es;
           asm_push_di;
           asm mov cx, word ptr s_length;
           asm mov eax, dword ptr s_data;
           asm les di, dword ptr s_ptr;
           asm cld;
           asm db 0xf3, 0x66, 0xab;
           asm_pop_di;
           asm_pop_es;
}

void far pascal ___memcpyd(void far *s_dest, void far *s_src, unsigned int s_length)
{
           asm_push_ds;
           asm_push_es;
           asm_push_si;
           asm_push_di;
           asm mov cx, word ptr s_length;
           asm les di, dword ptr s_dest;
           asm lds si, dword ptr s_src;
           asm cld;
           asm db 0xf3, 0x66, 0xa5;
           asm_pop_di;
           asm_pop_si;
           asm_pop_es;
           asm_pop_ds;
}

unsigned int far pascal tc_graphics::initgraph(unsigned int s_graphics)
{
           tc_graphics::base=MK_FP(0xa000, 0x0000);
           tc_graphics::bank_size=0x00010000LU;
           tc_graphics::hres=0U; tc_graphics::vres=0U;
           tc_graphics::card=0U;
           tc_graphics::mode=0U;
           tc_graphics::bank_num=0U;
           tc_graphics::process=D_PUT;
           tc_graphics::bank=bank_cirrus;
           tc_graphics::scale_x=1U;
           tc_graphics::scale_y=1U;
           tc_graphics::to_x=0U;
           tc_graphics::to_y=0U;
           tc_graphics::tty_x=0U;
           tc_graphics::tty_y=0U;
           tc_graphics::tty_color=0x0007;
           switch(s_graphics)
           {
            case D_GRAPHICS_320x200:
                 tc_graphics::hres=320U;
                 tc_graphics::vres=200U;
                 tc_graphics::mode=0x13;
                 break;
            case D_GRAPHICS_800x600:
                 tc_graphics::hres=800U;
                 tc_graphics::vres=600U;
                 tc_graphics::mode=0x5c;
                 tc_graphics::bank=bank_cirrus;
                 break;
            default: break;
           }
           tc_graphics::tty_screen=new unsigned char far [(tc_graphics::maxx>>3U)*(tc_graphics::maxy>>3U)];
           memsetb(tc_graphics::tty_screen, 0U, (tc_graphics::maxx>>3U)*(tc_graphics::maxy>>3U));
           tc_graphics::max_bank_num=(unsigned int)((((unsigned long)tc_graphics::hres)*((unsigned long)tc_graphics::vres))/((unsigned long)tc_graphics::bank_size));
           if((((unsigned long)tc_graphics::hres*(unsigned long)tc_graphics::vres)%(unsigned long)tc_graphics::bank_size))tc_graphics::max_bank_num++;
           tc_graphics::minx=0U; tc_graphics::miny=0U; tc_graphics::maxx=tc_graphics::hres-1U; tc_graphics::maxy=tc_graphics::vres-1U;
           _AX=tc_graphics::mode;
           bios_video_interrupt();
           tc_graphics::bank();
           return(True);
}

void far pascal tc_graphics::closegraph(void)
{
           delete(tc_graphics::tty_screen);
           _AX=0x0003;
           bios_video_interrupt();
}

unsigned int far pascal tc_graphics::getpixel(unsigned int s_x, unsigned int s_y)
{
 union
 {
  unsigned int  word[2U];
  unsigned long dword;
 }s_offset;
           s_offset.dword=((unsigned long)s_y*(unsigned long)tc_graphics::hres)+(unsigned long)s_x;
           if(s_offset.word[D_HIGH]!=tc_graphics::bank_num)
           {
            tc_graphics::bank_num=s_offset.word[D_HIGH];
            tc_graphics::bank(tc_graphics::bank_num);
           }
           return(*(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW]));
}

void far pascal tc_graphics::putpixel(unsigned int s_color, unsigned int s_x, unsigned int s_y)
{
 union
 {
  unsigned int  word[2U];
  unsigned long dword;
 }s_offset;
           s_offset.dword=((unsigned long)s_y*(unsigned long)tc_graphics::hres)+(unsigned long)s_x;
           if(s_offset.word[D_HIGH]!=tc_graphics::bank_num)
           {
            tc_graphics::bank_num=s_offset.word[D_HIGH];
            tc_graphics::bank(tc_graphics::bank_num);
           }
           switch(tc_graphics::process)
           {
            case D_PUT:
            default:    *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])=s_color; break;
            case D_AND: *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])&=s_color; break;
            case D_XOR: *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])^=s_color; break;
            case D_OR:  *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])|=s_color; break;
           }
}

void far pascal tc_graphics::hline(unsigned int s_color, unsigned int s_x1, unsigned int s_x2, unsigned int s_y)
{
 union
 {
  unsigned int  word[2U];
  unsigned long dword;
 }s_offset;
           if(s_x1>s_x2)swap(&s_x1, &s_x2, 2U);
           s_offset.dword=((unsigned long)s_y*(unsigned long)tc_graphics::hres)+(unsigned long)s_x1;
           while(s_x1<=s_x2)
           {
            if(s_offset.word[D_HIGH]!=tc_graphics::bank_num)
            {
             tc_graphics::bank_num=s_offset.word[D_HIGH];
             tc_graphics::bank(tc_graphics::bank_num);
            }
            switch(tc_graphics::process)
            {
             case D_PUT:
             default:    *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])=s_color; break;
             case D_AND: *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])&=s_color; break;
             case D_XOR: *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])^=s_color; break;
             case D_OR:  *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])|=s_color; break;
            }
            s_offset.dword++; s_x1++;
           }
}

void far pascal tc_graphics::vline(unsigned int s_color, unsigned int s_x, unsigned int s_y1, unsigned int s_y2)
{
 union
 {
  unsigned int  word[2U];
  unsigned long dword;
 }s_offset;
           if(s_y1>s_y2)swap(&s_y1, &s_y2, 2U);
           s_offset.dword=((unsigned long)s_y1*(unsigned long)tc_graphics::hres)+(unsigned long)s_x;
           while(s_y1<=s_y2)
           {
            if(s_offset.word[D_HIGH]!=tc_graphics::bank_num)
            {
             tc_graphics::bank_num=s_offset.word[D_HIGH];
             tc_graphics::bank(tc_graphics::bank_num);
            }
            switch(tc_graphics::process)
            {
             case D_PUT:
             default:    *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])=s_color; break;
             case D_AND: *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])&=s_color; break;
             case D_XOR: *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])^=s_color; break;
             case D_OR:  *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])|=s_color; break;
            }
            s_offset.dword+=(unsigned long)tc_graphics::hres; s_y1++;
           }
}

void far pascal tc_graphics::rectangle(unsigned int s_color, unsigned int s_x1, unsigned int s_y1, unsigned int s_x2, unsigned int s_y2)
{
           tc_graphics::hline(s_color, s_x1, s_x2, s_y1);
           tc_graphics::hline(s_color, s_x1, s_x2, s_y2);
           tc_graphics::vline(s_color, s_x1, s_y1, s_y2);
           tc_graphics::vline(s_color, s_x2, s_y1, s_y2);
}

void far pascal tc_graphics::fillbox(unsigned int s_color, unsigned int s_x1, unsigned int s_y1, unsigned int s_x2, unsigned int s_y2)
{
 union
 {
  unsigned int  word[2U];
  unsigned long dword;
 }s_offset;
 unsigned int s_cx;
           while(s_y1<=s_y2)
           {
            s_offset.dword=((unsigned long)s_y1*(unsigned long)tc_graphics::hres)+(unsigned long)s_x1;
            s_cx=s_x1;
            while(s_cx<=s_x2)
            {
             if(s_offset.word[D_HIGH]!=tc_graphics::bank_num)
             {
              tc_graphics::bank_num=s_offset.word[D_HIGH];
              tc_graphics::bank(tc_graphics::bank_num);
             }
             switch(tc_graphics::process)
             {
              case D_PUT:
              default:    *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])=s_color; break;
              case D_AND: *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])&=s_color; break;
              case D_XOR: *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])^=s_color; break;
              case D_OR:  *(((unsigned char far *)tc_graphics::base)+s_offset.word[D_LOW])|=s_color; break;
             }
             s_offset.dword++; s_cx++;
            }
            s_y1++;
           }
}

void far pascal tc_graphics::clear(unsigned int s_color)
{
 unsigned int s_count;
 unsigned long s_cc;
           asm mov ax, word ptr s_color;
           asm mov ah, al;
           asm push ax;
           asm shl eax, 16;
           asm pop ax;
           asm mov dword ptr s_cc, eax;
           asm cld;
           for(s_count=0U;s_count<=tc_graphics::max_bank_num;s_count++)
           {
            tc_graphics::bank(s_count);
            ___memsetd(tc_graphics::base, s_cc, (unsigned int)(tc_graphics::bank_size>>2U));
           }
           tc_graphics::bank_num=tc_graphics::max_bank_num;
}

void far pascal tc_graphics::eputc(unsigned int s_color, unsigned int s_bcolor, unsigned int s_char, unsigned int s_x, unsigned int s_y)
{
 unsigned int s_bit, s_count_y, s_offset, s_cx;
           s_offset=s_char<<4;
           for(s_count_y=s_y;s_count_y<((tc_graphics::scale_y<<4U)+s_y);s_count_y+=tc_graphics::scale_y, s_offset++)
           {
            for(s_bit=0U, s_cx=s_x+((tc_graphics::scale_x<<3U)-tc_graphics::scale_x);s_bit<8U;s_bit++, s_cx-=tc_graphics::scale_x)
            {
             if((*(font_eng+s_offset))&(1U<<s_bit))tc_graphics::fillbox(s_color, s_cx, s_count_y, s_cx+(tc_graphics::scale_x-1U), s_count_y+(tc_graphics::scale_y-1U));
             else if(s_bcolor!=0xffff)tc_graphics::fillbox(s_bcolor, s_cx, s_count_y, s_cx+(tc_graphics::scale_x-1U), s_count_y+(tc_graphics::scale_y-1U));
            }
           }
}

void far pascal tc_graphics::eputs(unsigned int s_color, unsigned int s_bcolor, void far *s_string, unsigned int s_x, unsigned int s_y)
{
           while(*((unsigned char far *)s_string))
           {
            tc_graphics::eputc(s_color, s_bcolor, *(((unsigned char far *)s_string)++), s_x, s_y);
            s_x+=tc_graphics::scale_x<<3U;
           }
}

void far pascal tc_graphics::bitmap16x16(unsigned int s_color, unsigned int s_bcolor, void far *s_bitmap, unsigned int s_x, unsigned int s_y)
{
 unsigned int s_bit, s_count_y, s_cx;
           for(s_count_y=s_y;s_count_y<((tc_graphics::scale_y<<4U)+s_y);s_count_y+=tc_graphics::scale_y, ((unsigned int far *)s_bitmap)++)
           {
            for(s_bit=0U, s_cx=s_x+((tc_graphics::scale_x<<4U)-tc_graphics::scale_x);s_bit<16U;s_bit++, s_cx-=tc_graphics::scale_x)
            {
             if((*((unsigned int far *)s_bitmap))&(1U<<s_bit))tc_graphics::fillbox(s_color, s_cx, s_count_y, s_cx+(tc_graphics::scale_x-1U), s_count_y+(tc_graphics::scale_y-1U));
             else if(s_bcolor!=0xffff)tc_graphics::fillbox(s_bcolor, s_cx, s_count_y, s_cx+(tc_graphics::scale_x-1U), s_count_y+(tc_graphics::scale_y-1U));
            }
           }
}

void far pascal tc_graphics::hputc(unsigned int s_color, unsigned int s_bcolor, unsigned int s_char, unsigned int s_x, unsigned int s_y)
{
 unsigned int s_c0, s_c1, s_c2, s_b0, s_b1, s_b2, s_count;
 unsigned char s_buffer[32U];
           asm mov ax, word ptr s_char; // ¤a·¡Ëa ŠaÑÅ·i Ðaµa Ðe‹iÂ‰b·i º…§¡Ðe”a.
           asm xchg ah, al;
           asm mov word ptr s_char, ax;
           s_c0=(unsigned int)HAN_table0_[(s_char>>0x0a)&0x1f]; // Á¡¬÷ ¤é®
           s_c1=(unsigned int)HAN_table1_[(s_char>>0x05)&0x1f]; // º—¬÷ ¤é®
           s_c2=(unsigned int)HAN_table2_[s_char&0x1f];         // ¹·¬÷ ¤é®
           if(s_c2==0x00ff) // ¹·¬÷·¡ ´ô“e ‰w¶
           {
            s_b0=(unsigned int)HAN_ftable0_[s_c1];   // Á¡¬÷¤é®
            if(s_c0==0x0000||s_c0==0x000f)s_b1=0U; // º—¬÷¤é® 'ˆA', 'ÄA'
            else s_b1=1U;                      // º—¬÷¤é®
            s_b2=0U;                           // ¹·¬÷¤é®
           }
           else // ¹·¬÷·¡ ·¶“e ‰w¶
           {
            s_b0=(unsigned int)HAN_ftable1_[s_c1];   // Á¡¬÷¤é®
            if(s_c0==0x0000||s_c0==0x000f)s_b1=2U; // º—¬÷¤é® 'ˆA', 'ÄA'
            else s_b1=3U;                      // º—¬÷¤é®
            s_b2=(unsigned int)HAN_ltable_[s_c1];    // ¹·¬÷¤é®
           }
           for(s_count=0U;s_count<32U;s_count++)
           {
            s_buffer[s_count]=0x00; // ‹¡¥¥ ¤áÌáŸi ÇiŸ¡´á Ða¡e¬á. . .
            if(s_c0!=0xff)s_buffer[s_count]|=font_han0[s_b0][s_c0][s_count]; // Á¡¬÷ ORµe¬e
            if(s_c1!=0xff)s_buffer[s_count]|=font_han1[s_b1][s_c1][s_count]; // º—¬÷ ORµe¬e
            if(s_c2!=0xff)s_buffer[s_count]|=font_han2[s_b2][s_c2][s_count]; // ¹·¬÷ ORµe¬e
           }
           for(s_count=0U;s_count<32U;s_count+=2)
           {
            s_c0=s_buffer[s_count];
            s_buffer[s_count]=s_buffer[s_count+1U];
            s_buffer[s_count+1U]=s_c0;
           }
           tc_graphics::bitmap16x16(s_color, s_bcolor, s_buffer, s_x, s_y);
}

void far pascal tc_graphics::ehputs(unsigned int s_color, unsigned int s_bcolor, void far *s_string, unsigned int s_x, unsigned int s_y)
{
           while(*((unsigned char far *)s_string))
           {
            if(*((unsigned int far *)s_string)&0x0080)
            {
             tc_graphics::hputc(s_color, s_bcolor, *(((unsigned int far *)s_string)++), s_x, s_y);
             s_x+=tc_graphics::scale_x<<4U;
            }
            else
            {
             tc_graphics::eputc(s_color, s_bcolor, *(((unsigned char far *)s_string)++), s_x, s_y);
             s_x+=tc_graphics::scale_x<<3U;
            }
           }
}

void far cdecl tc_graphics::eprintf(unsigned int s_color, unsigned int s_bcolor, unsigned int s_x, unsigned int s_y, void far *s_format, ...)
{
 unsigned char s_format_buffer[512U];
            vsprintf(&s_format_buffer[0U], s_format, ...);
            tc_graphics::eputs(s_color, s_bcolor, &s_format_buffer[0U], s_x, s_y);
}

void far cdecl tc_graphics::ehprintf(unsigned int s_color, unsigned int s_bcolor, unsigned int s_x, unsigned int s_y, void far *s_format, ...)
{
 unsigned char s_format_buffer[512U];
            vsprintf(&s_format_buffer[0U], s_format, ...);
            tc_graphics::ehputs(s_color, s_bcolor, &s_format_buffer[0U], s_x, s_y);
}

void far pascal tc_graphics::neputs(unsigned int s_color, unsigned int s_bcolor, unsigned int s_length, void far *s_dump, unsigned int s_x, unsigned int s_y)
{
           while(s_length--)
           {
            tc_graphics::eputc(s_color, s_bcolor, *(((unsigned char far *)s_dump)++), s_x, s_y);
            s_x+=tc_graphics::scale_x<<3U;
           }
}

void far pascal bank_cirrus(unsigned int s_bank)
{
           outportw(0x03ce, (s_bank<<12U)|9U);
}

void far pascal tc_graphics::box(unsigned int s_tl_color, unsigned int s_br_color, unsigned int s_color, unsigned int s_x1, unsigned int s_y1, unsigned int s_x2, unsigned int s_y2)
{
           tc_graphics::hline(s_tl_color, s_x1, s_x2, s_y1);
           tc_graphics::vline(s_tl_color, s_x1, s_y1+1U, s_y2);
           tc_graphics::hline(s_br_color, s_x1+1U, s_x2, s_y2);
           tc_graphics::vline(s_br_color, s_x2, s_y1+1U, s_y2-1U);
           tc_graphics::fillbox(s_color, s_x1+1U, s_y1+1U, s_x2-1U, s_y2-1U);
}

void far pascal tc_graphics::line(unsigned int s_color, unsigned int s_x1, unsigned int s_y1, unsigned int s_x2, unsigned int s_y2)
{
 signed int s_deltax, s_deltay, s_count, s_distance, s_stepx, s_stepy, s_x=0, s_y=0, s_sx, s_sy;
           s_deltax=(signed int)(s_x2-s_x1);
           s_deltay=(signed int)(s_y2-s_y1);
           s_sx=(signed int)s_x1;
           s_sy=(signed int)s_y1;
           if(s_deltax==0&&s_deltay==0)tc_graphics::putpixel(s_color, s_x1, s_y1);
           else if(s_deltax==0)tc_graphics::vline(s_color, s_x1, s_y1, s_y2);
           else if(s_deltay==0)tc_graphics::hline(s_color, s_x1, s_x2, s_y1);
           else
           {
            if(s_deltax>0)s_stepx=1;
            else s_stepx=(-1);
            if(s_deltay>0)s_stepy=1;
            else s_stepy=(-1);
            if(s_deltax<0)s_deltax*=(-1);
            if(s_deltay<0)s_deltay*=(-1);
            if(s_deltax>s_deltay)s_distance=s_deltax;
            else s_distance=s_deltay;
            for(s_count=0;s_count<=s_distance;s_count++)
            {
             tc_graphics::putpixel(s_color, s_sx, s_sy);
             s_x+=s_deltax;
             s_y+=s_deltay;
             if(s_x>=s_distance)
             {
              s_x-=s_distance;
              s_sx+=s_stepx;
             }
             if(s_y>=s_distance)
             {
              s_y-=s_distance;
              s_sy+=s_stepy;
             }
            }
           }
}

void far pascal tc_graphics::lineto(unsigned int s_color, unsigned int s_x, unsigned int s_y)
{
           tc_graphics::line(s_color, tc_graphics::to_x, tc_graphics::to_y, s_x, s_y);
           tc_graphics::to_x=s_x;
           tc_graphics::to_y=s_y;
}

void far pascal tc_graphics::video(e_boolean s_sw)
{
           if(s_sw==True)outportx(0x03c4, 0x01, inportx(0x03c4, 0x01)&0xdf);
           else outportx(0x03c4, 0x01, inportx(0x03c4, 0x01)|0x20);
}

void far pascal tc_graphics::tputc(unsigned int s_color, unsigned int s_bcolor, unsigned int s_char, unsigned int s_x, unsigned int s_y)
{
 unsigned int s_bit, s_count_y, s_offset, s_cx;
           s_x<<=3U; s_y<<=4U;
           s_offset=s_char<<4;
           for(s_count_y=s_y;s_count_y<((tc_graphics::scale_y<<4U)+s_y);s_count_y+=tc_graphics::scale_y, s_offset++)
           {
            for(s_bit=0U, s_cx=s_x+((tc_graphics::scale_x<<3U)-tc_graphics::scale_x);s_bit<8U;s_bit++, s_cx-=tc_graphics::scale_x)
            {
             if((*(font_eng+s_offset))&(1U<<s_bit))tc_graphics::fillbox(s_color, s_cx, s_count_y, s_cx+(tc_graphics::scale_x-1U), s_count_y+(tc_graphics::scale_y-1U));
             else if(s_bcolor!=0xffff)tc_graphics::fillbox(s_bcolor, s_cx, s_count_y, s_cx+(tc_graphics::scale_x-1U), s_count_y+(tc_graphics::scale_y-1U));
            }
           }
}

void far pascal tc_graphics::tputs(unsigned int s_color, unsigned int s_bcolor, void far *s_string, unsigned int s_x, unsigned int s_y)
{
           s_x<<=3U; s_y<<=4U;
           while(*((unsigned char far *)s_string))
           {
            if(*((unsigned int far *)s_string)&0x0080)
            {
             tc_graphics::hputc(s_color, s_bcolor, *(((unsigned int far *)s_string)++), s_x, s_y);
             s_x+=tc_graphics::scale_x<<4U;
            }
            else
            {
             tc_graphics::eputc(s_color, s_bcolor, *(((unsigned char far *)s_string)++), s_x, s_y);
             s_x+=tc_graphics::scale_x<<3U;
            }
           }
}

void far pascal tc_graphics::linefeed(e_boolean s_sw)
{
           tc_graphics::tty_y++;
           if(tc_graphics::tty_y>=(tc_graphics::maxy>>4U))
           {
            /*unsigned char far *s_store=new unsigned char far [8000U];
            tc_graphics::tty_y=tc_graphics::maxy;
            tc_graphics::gettext(s_store, tc_graphics::minx, tc_graphics::miny+1U, tc_graphics::maxx, tc_graphics::maxy);
            tc_graphics::puttext(s_store, tc_graphics::minx, tc_graphics::miny, tc_graphics::maxx, tc_graphics::maxy-1U);
            tc_graphics::hline(tc_graphics::tty_color, ' ', tc_graphics::minx, tc_graphics::maxx, tc_graphics::maxy);
            delete(s_store);*/
           }
           if(s_sw==True)tc_graphics::tty_x=0U;
}

void far pascal tc_graphics::tty_putc(unsigned int s_char)
{
           switch(s_char)
           {
            case 0x0d: tc_graphics::tty_x=0U; break;
            case 0x0a: tc_graphics::linefeed(False); break;
            case 0x08:
                 if(tc_graphics::tty_x)tc_graphics::tty_x--;
                 else if(tc_graphics::tty_y)
                 {
                  tc_graphics::tty_x=tc_graphics::maxx>>3U;
                  tc_graphics::tty_y--;
                 }
                 else beep(10U);
                 break;
            default:
                 tc_graphics::tputc((tc_graphics::tty_color&0x000f), ((tc_graphics::tty_color>>4)&0x000f), s_char, tc_graphics::tty_x++, tc_graphics::tty_y);
                 if(tc_graphics::tty_x>(tc_graphics::maxx>>3U))tc_graphics::linefeed(True);
                 break;
           }
}

/* End of source */