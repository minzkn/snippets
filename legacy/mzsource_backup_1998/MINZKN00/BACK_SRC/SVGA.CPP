/*
  Name: SUGA O/S
  File: SVGA.CPP
  Copy: Copyright(c)1998-1999 by Cho JaeHyuk
  What: SuperVGA
*/

#define FILE_SVGA_CPP

#include <SVGA.H>
#include <KERNEL.H>

void far pascal putpixel(unsigned int, unsigned int, unsigned int);
void far pascal hline(unsigned int, unsigned int, unsigned int, unsigned int);
void far pascal vline(unsigned int, unsigned int, unsigned int, unsigned int);
void far pascal rectangle(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void far pascal box(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void far pascal clear(unsigned int);
void far pascal eputc(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void far pascal eputs(unsigned int, unsigned int, void far *, unsigned int, unsigned int);
void far pascal bitmap16x16(unsigned int, unsigned int, void far *, unsigned int, unsigned int);
void far pascal hputc(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void far pascal ehputs(unsigned int, unsigned int, void far *, unsigned int, unsigned int);
void far cdecl eprintf(unsigned int, unsigned int, unsigned int, unsigned int, void far *, ...);
void far cdecl ehprintf(unsigned int, unsigned int, unsigned int, unsigned int, void far *, ...);
void far pascal neputs(unsigned int, unsigned int, unsigned int, void far *, unsigned int, unsigned int);

unsigned int prev_bank_num=0xffff;
unsigned int scale_x=1U, scale_y=1U;
unsigned char far HAN_table0_[32U]={0x07, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0xff, 0x09, 0x0b, 0x0b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; /* Á¡¬÷ */
unsigned char far HAN_table1_[32U]={0xff, 0xff, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0xff, 0xff, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0xff, 0xff, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0xff, 0xff, 0x11, 0x12, 0x13, 0x14, 0x00, 0x01}; /* º—¬÷ */
unsigned char far HAN_table2_[32U]={0xff, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0xff, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x14, 0x14}; /* ¹·¬÷ */
unsigned char far HAN_ftable0_[21U]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x01, 0x02, 0x04, 0x04, 0x04, 0x02, 0x01, 0x03, 0x00}; // ¹·¬÷·¡ ´ô“e ‰w¶ ¤é®
unsigned char far HAN_ftable1_[21U]={0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x06, 0x07, 0x07, 0x07, 0x06, 0x06, 0x07, 0x07, 0x07, 0x06, 0x06, 0x07, 0x05}; // ¹·¬÷·¡ ·¶“e ‰w¶ ¤é®
unsigned char far HAN_ltable_[21U]={0x00, 0x02, 0x00, 0x02, 0x01, 0x02, 0x01, 0x02, 0x03, 0x00, 0x02, 0x01, 0x03, 0x03, 0x01, 0x02, 0x01, 0x03, 0x03, 0x01, 0x01}; // º—¬÷µA ˜aŸa“e ¹·¬÷ ¤é®
                                       //„a    „    „¡    „Á    „á    …A    …a    …    …¡    …Á    …á    †A    †a    †    †¡    †Á    †á    ‡A    ‡a    ‡    ‡¡

void far pascal putpixel(unsigned int s_color, unsigned int s_x, unsigned int s_y)
{
           asm mov ax, 800;
           asm mul word ptr s_y;
           asm add ax, word ptr s_x;
           asm adc dx, 0x0000;
           asm mov bx, ax
           asm cmp dx, word ptr prev_bank_num;
           asm je short l_putpixel_0000;
           asm mov word ptr prev_bank_num, dx;
           asm mov ah, dl;
           asm shl ah, 0x04;
           asm mov dx, 0x03ce;
           asm mov al, 0x09;
           asm out dx, ax;
l_putpixel_0000:;
           asm mov dl, byte ptr s_color;
           asm push ds;
           asm mov ax, 0a000h;
           asm mov ds, ax;
           asm mov byte ptr [bx], dl;
           asm pop ds;
           asm mov word ptr prev_bank_num, cx;
}

void far pascal hline(unsigned int s_color, unsigned int s_x1, unsigned int s_x2, unsigned int s_y)
{
           asm push ds;
           asm mov cx, word ptr prev_bank_num;
           asm mov ax, 0xa000;
           asm mov ds, ax;
           asm mov ax, 800;
           asm mul word ptr s_y;
           asm add ax, word ptr s_x1;
           asm adc dx, 0;
           asm mov bx, ax;
 l_hline_0000:;
           asm cmp dx, cx;
           asm je short l_hline_0001;
           asm push dx;
           asm mov cx, dx;
           asm mov ah, dl;
           asm shl ah, 0x04;
           asm mov dx, 0x03ce;
           asm mov al, 0x09;
           asm out dx, ax;
           asm pop dx;
 l_hline_0001:;
           asm mov al, byte ptr s_color;
           asm mov byte ptr [bx], al;
           asm mov ax, word ptr s_x1;
           asm cmp ax, word ptr s_x2;
           asm je short l_hline_0002;
           asm add bx, 0x0001;
           asm adc dx, 0x0000;
           asm inc word ptr s_x1;
           asm jmp short l_hline_0000;
 l_hline_0002:;
           asm pop ds;
           asm mov word ptr prev_bank_num, cx;
}

void far pascal vline(unsigned int s_color, unsigned int s_x, unsigned int s_y1, unsigned int s_y2)
{
           asm push ds;
           asm mov cx, word ptr prev_bank_num;
           asm mov ax, 0xa000;
           asm mov ds, ax;
           asm mov ax, 800;
           asm mul word ptr s_y1;
           asm add ax, word ptr s_x;
           asm adc dx, 0x0000;
           asm mov bx, ax;
 l_vline_0000:;
           asm cmp dx, cx;
           asm je short l_vline_0001;
           asm push dx;
           asm mov cx, dx;
           asm mov ah, dl;
           asm shl ah, 0x04;
           asm mov dx, 0x03ce;
           asm mov al, 0x09;
           asm out dx, ax;
           asm pop dx;
 l_vline_0001:;
           asm mov al, byte ptr s_color;
           asm mov byte ptr [bx], al;
           asm mov ax, word ptr s_y1;
           asm cmp ax, word ptr s_y2;
           asm je short l_vline_0002;
           asm add bx, 800;
           asm adc dx, 0x0000;
           asm inc word ptr s_y1;
           asm jmp short l_vline_0000;
 l_vline_0002:;
           asm pop ds;
           asm mov word ptr prev_bank_num, cx;
}

void far pascal rectangle(unsigned int s_color, unsigned int s_x1, unsigned int s_y1, unsigned int s_x2, unsigned int s_y2)
{
           hline(s_color, s_x1, s_x2, s_y1);
           vline(s_color, s_x2, s_y1+1, s_y2-1);
           vline(s_color, s_x1, s_y1+1, s_y2-1);
           hline(s_color, s_x1, s_x2, s_y2);
}

void far pascal box(unsigned int s_color, unsigned int s_x1, unsigned int s_y1, unsigned int s_x2, unsigned int s_y2)
{
           while(s_y1<=s_y2)hline(s_color, s_x1, s_x2, s_y1++);
}

void far pascal clear(unsigned int s_color)
{
           asm push es;
           asm push di;
           asm mov ax, 0xa000;
           asm mov es, ax;
           asm xor bx, bx;
           asm cld;
 l_clear_0000:;
           asm xor di, di;
           asm mov cx, 32768;
           asm mov al, byte ptr s_color;
           asm mov ah, al;
           asm rep stosw;
           asm mov ah, bl;
           asm shl ah, 4;
           asm mov al, 0x09;
           asm mov dx, 0x03ce;
           asm out dx, ax;
           asm inc bx;
           asm cmp bx, 9;
           asm jne short l_clear_0000;
           asm pop di;
           asm pop es;
           prev_bank_num=8;
}

void far pascal eputc(unsigned int s_color, unsigned int s_bcolor, unsigned int s_char, unsigned int s_x, unsigned int s_y)
{
 unsigned int s_bit, s_count_y, s_offset, s_cx;
           s_offset=s_char<<4U;
           for(s_count_y=s_y;s_count_y<((scale_y<<4U)+s_y);s_count_y+=scale_y, s_offset++)
           {
            for(s_bit=0U, s_cx=s_x+((scale_x<<3)-scale_x);s_bit<8U;s_bit++, s_cx-=scale_x)
            {
             if((*(font_eng+s_offset))&(1U<<s_bit))box(s_color, s_cx, s_count_y, s_cx+(scale_x-1U), s_count_y+(scale_y-1U));
             else if(s_bcolor!=0xffff)box(s_bcolor, s_cx, s_count_y, s_cx+(scale_x-1U), s_count_y+(scale_y-1U));
            }
           }
}

void far pascal eputs(unsigned int s_color, unsigned int s_bcolor, void far *s_string, unsigned int s_x, unsigned int s_y)
{
           while(*((unsigned char far *)s_string))
           {
            eputc(s_color, s_bcolor, *(((unsigned char far *)s_string)++), s_x, s_y);
            s_x+=scale_x<<3U;
           }
}

void far pascal bitmap16x16(unsigned int s_color, unsigned int s_bcolor, void far *s_bitmap, unsigned int s_x, unsigned int s_y)
{
 unsigned int s_bit, s_count_y, s_cx;
           for(s_count_y=s_y;s_count_y<((scale_y<<4U)+s_y);s_count_y+=scale_y, ((unsigned int far *)s_bitmap)++)
           {
            for(s_bit=0U, s_cx=s_x+((scale_x<<4)-scale_x);s_bit<16U;s_bit++, s_cx-=scale_x)
            {
             if((*((unsigned int far *)s_bitmap))&(1U<<s_bit))box(s_color, s_cx, s_count_y, s_cx+(scale_x-1U), s_count_y+(scale_y-1U));
             else if(s_bcolor!=0xffff)box(s_bcolor, s_cx, s_count_y, s_cx+(scale_x-1U), s_count_y+(scale_y-1U));
            }
           }
}

void far pascal hputc(unsigned int s_color, unsigned int s_bcolor, unsigned int s_char, unsigned int s_x, unsigned int s_y)
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
           for(s_count=0U;s_count<32U;s_count+=2U)
           {
            s_c0=s_buffer[s_count];
            s_buffer[s_count]=s_buffer[s_count+1U];
            s_buffer[s_count+1U]=s_c0;
           }
           bitmap16x16(s_color, s_bcolor, s_buffer, s_x, s_y);
}

void far pascal ehputs(unsigned int s_color, unsigned int s_bcolor, void far *s_string, unsigned int s_x, unsigned int s_y)
{
           while(*((unsigned char far *)s_string))
           {
            if(*((unsigned int far *)s_string)&0x0080)
            {
             hputc(s_color, s_bcolor, *(((unsigned int far *)s_string)++), s_x, s_y);
             s_x+=scale_x<<4U;
            }
            else
            {
             eputc(s_color, s_bcolor, *(((unsigned char far *)s_string)++), s_x, s_y);
             s_x+=scale_x<<3U;
            }
           }
}

void far cdecl eprintf(unsigned int s_color, unsigned int s_bcolor, unsigned int s_x, unsigned int s_y, void far *s_format, ...)
{
 void far *s_format_buffer=farmalloc(512U);
            vsprintf(s_format_buffer, s_format, ...);
            eputs(s_color, s_bcolor, s_format_buffer, s_x, s_y);
            farfree(s_format_buffer);
}

void far cdecl ehprintf(unsigned int s_color, unsigned int s_bcolor, unsigned int s_x, unsigned int s_y, void far *s_format, ...)
{
 void far *s_format_buffer=farmalloc(512U);
            vsprintf(s_format_buffer, s_format, ...);
            ehputs(s_color, s_bcolor, s_format_buffer, s_x, s_y);
            farfree(s_format_buffer);
}

void far pascal neputs(unsigned int s_color, unsigned int s_bcolor, unsigned int s_length, void far *s_dump, unsigned int s_x, unsigned int s_y)
{
           while(s_length--)
           {
            eputc(s_color, s_bcolor, *(((unsigned char far *)s_dump)++), s_x, s_y);
            s_x+=scale_x<<3U;
           }
}

/* End of source */