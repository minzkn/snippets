/*
  Name: MINZ O/S
  File: DSP.C
  Copy: Copyright(c)1996-1999 by Cho JaeHyuk
  What: MINZ operating system kernel source
*/

/* Sound Blaster complete */

#define FILE_DSP_C "DSP.C"

#include "DSP.H"

unsigned int far pascal dsp_read(void);
void far pascal dsp_write(unsigned int);
unsigned int far pascal dsp_open(unsigned int, unsigned int, unsigned int);
unsigned int far pascal dsp_rate(unsigned int);
void far pascal dsp_alloc(void far *, unsigned int, void far *, unsigned int);
void far pascal dsp_stop(void);
unsigned int far pascal dsp_dma_count(void);
unsigned int far pascal dsp_dma_status(void);
void far pascal dsp_record(void far *, unsigned int, void far *, unsigned int);
void interrupt dsp_handler(void);

unsigned int g_dsp_address, g_dsp_irq, g_dsp_dma, g_dsp_access=0U;
void interrupt (far pascal * far pascal dsp_old_irq)(void);

struct
{
 unsigned int playing;
 unsigned int overlap;
 unsigned int empty[2U];
 unsigned int length[2U];
 unsigned long address[2U];
}g_dsp_buffer;

struct
{
 unsigned int page[4U];
 unsigned int base[4U];
 unsigned int count[4U];
}g_dsp_regs={
  {0x87, 0x83, 0x81, 0x82},
  {0U, 2U, 4U, 6U},
  {1U, 3U, 5U, 7U}
 };

unsigned int far pascal dsp_read(void)
{
           __mov(dx, word ptr g_dsp_address);
           __add(dx, 0x000e); /* Wait index */
           __mov(cx, 0xffff);
 l_dsp_read_00:;
           __or(cx, cx);
           __jz(short l_dsp_read_01);
           __dec(cx);
           __in(al, dx);
           __test(al, 0x80);
           __jz(short l_dsp_read_00);
           __mov(dx, word ptr g_dsp_address);
           __add(dx, 0x000a);
           __in(al, dx);
 l_dsp_read_01:;
           __xor(ah, ah);
           return(_AX);
}

void far pascal dsp_write(unsigned int s_data)
{
           __mov(dx, word ptr g_dsp_address);
           __add(dx, 0x000c); /* Wait index */
           __mov(cx, 0xffff);
 l_dsp_write_00:;
           __or(cx, cx);
           __jz(short l_dsp_write_01);
           __dec(cx);
           __in(al, dx);
           __test(al, 0x80);
           __jnz(short l_dsp_write_00);
           __mov(al, byte ptr s_data);
           __out(dx, al);
 l_dsp_write_01:;
}

e_boolean far pascal dsp_open(unsigned int s_address, unsigned int s_irq, unsigned int s_dma)
{
           g_dsp_address=s_address;
           g_dsp_irq=s_irq;
           g_dsp_dma=s_dma;
           __mov(dx, word ptr g_dsp_address);
           __add(dx, D_DSP_RESET);
           __mov(al, 0x01);
           __out(dx, al);
           __mov(cx, 0x0007);
 l_dsp_open_00:;
           __in(al, dx);
           __loop(short l_dsp_open_00);
           __xor(al, al);
           __out(dx, al);
           for(s_irq=0U;s_irq<100U;s_irq++)
           {
            if(inportb(g_dsp_address+0x0a)==0xaa) /* Detect index */
            {
             dsp_old_irq=getvect(g_dsp_irq+8U);
             setvect(g_dsp_irq+8U, dsp_handler);
             outportb(0x0021, inportb(0x0021)&(~(1U<<g_dsp_irq)));
             dsp_write(0x00d1);
             g_dsp_access=true;
             return(true);
            }
           }
           g_dsp_access=false;
           __mov(dx, word ptr g_dsp_address);
           __add(dx, 0x000e);
           __in(al, dx);
           __mov(al, 0x20);
           __out(0x20, al);
           outportb(0x0021, inportb(0x0021)|(1U<<g_dsp_irq));
           return(false);
}

unsigned int far pascal dsp_rate(unsigned int s_frequency)
{
           if(5000U<=s_frequency&&s_frequency<=22000U)
           {
            s_frequency=(unsigned int)(1000000LU/s_frequency);
            dsp_write(0x0040);
            dsp_write(256U-s_frequency);
            return((unsigned int)(1000000LU/s_frequency));
           }
           return(0U);
}

void far pascal dsp_alloc(void far *s_dest_ptr, unsigned int s_dest_length, void far *s_src_ptr, unsigned int s_src_length)
{
           g_dsp_buffer.playing=0U;
           g_dsp_buffer.overlap=0U;
           g_dsp_buffer.empty[0U]=0U;
           g_dsp_buffer.empty[1U]=0U;
           g_dsp_buffer.length[0U]=s_dest_length-1U;
           g_dsp_buffer.length[1U]=s_src_length-1U;
           g_dsp_buffer.address[0U]=FP_SEG(s_dest_ptr)*0x10LU+FP_OFF(s_dest_ptr);
           g_dsp_buffer.address[1U]=FP_SEG(s_src_ptr)*0x10LU+FP_OFF(s_src_ptr);
           __mov(al, byte ptr g_dsp_dma);
           __mov(ah, al);
           __add(al, 0x04);
           __out(0x0a, al);
           __mov(al, ah);
           __out(0x0c, al);
           __add(al, 0x48);
           __out(0x0b, al);
           outportb(g_dsp_regs.base[g_dsp_dma], indexbyte(g_dsp_buffer.address[g_dsp_buffer.playing], 0U));
           outportb(g_dsp_regs.base[g_dsp_dma], indexbyte(g_dsp_buffer.address[g_dsp_buffer.playing], 1U));
           outportb(g_dsp_regs.page[g_dsp_dma], indexbyte(g_dsp_buffer.address[g_dsp_buffer.playing], 2U));
           outportb(g_dsp_regs.count[g_dsp_dma], indexbyte(g_dsp_buffer.length[g_dsp_buffer.playing], 0U));
           outportb(g_dsp_regs.count[g_dsp_dma], indexbyte(g_dsp_buffer.length[g_dsp_buffer.playing], 1U));
           __mov(al, byte ptr g_dsp_dma);
           __out(0x0a, al);
           dsp_write(0x0014);
           dsp_write(indexbyte(g_dsp_buffer.length[g_dsp_buffer.playing], 0U));
           dsp_write(indexbyte(g_dsp_buffer.length[g_dsp_buffer.playing], 1U));
}

void far pascal dsp_stop(void)
{
           __mov(dx, word ptr g_dsp_address);
           __add(dx, 0x000e);
           __in(al, dx);
           __mov(al, 0x20);
           __out(0x20, al);
           outportb(0x0021, inportb(0x0021)|(1U<<g_dsp_irq));
           setvect(g_dsp_irq+8U, dsp_old_irq);
}

unsigned int far pascal dsp_dma_count(void)
{
           __in(al, 0x03);
           __mov(ah, al);
           __in(al, 0x03);
           __xchg(ah, al);
           __cmp(ax, 0xffff)
           __jne(short l_dsp_dma_count_00;
           __mov(dx, word ptr g_dsp_address);
           __add(dx, 0x000e);
           __push(ax);
           __in(al, dx);
           __pop(ax);
 l_dsp_dma_count_00:;
           return(_AX);
}

unsigned int far pascal dsp_dma_status(void)
{
           __in(al, 0x08);
           __and(al, 0x02);
           return(_AX);
}

void far pascal dsp_record(void far *s_dest_ptr, unsigned int s_dest_length, void far *s_src_ptr, unsigned int s_src_length)
{
 /* ±±±±±±±±±±±±±±±±±±± Init value ±±±±±±±±±±±±±±±±±±±±±± */
           g_dsp_buffer.playing=0U;
           g_dsp_buffer.overlap=0U;
           g_dsp_buffer.empty[0U]=0U;
           g_dsp_buffer.empty[1U]=0U;
           g_dsp_buffer.length[0U]=s_dest_length-1U;
           g_dsp_buffer.length[1U]=s_src_length-1U;
           g_dsp_buffer.address[0U]=FP_SEG(s_dest_ptr)*0x10LU+FP_OFF(s_dest_ptr);
           g_dsp_buffer.address[1U]=FP_SEG(s_src_ptr)*0x10LU+FP_OFF(s_src_ptr);
           /*__mov(al, 0x05);
           __out(0x0a, al);
           __xor(al, al);
           __out(0x0c, al);
           __mov(al, 0x45);
           __out(0x0b, al);
           __xor(al, al);
           __out(0x02, al);
           __out(0x02, al);
           outportb(0x83, (unsigned int)(g_dsp_buffer.address[0]>>16U)&0xff);
           outportb(0x03, (unsigned char)(g_dsp_buffer.length[0U])&0xff);
           outportb(0x03, (unsigned char)(g_dsp_buffer.length[0U]>>8U)&0xff);
           __mov(al, 0x01);
           __out(0x0a, al);
           dsp_write(0x24);
           dsp_write((unsigned char)(g_dsp_buffer.length[0U])&0xff);
           dsp_write((unsigned char)(g_dsp_buffer.length[0U]>>8U)&0xff);*/
}

void interrupt dsp_handler(void)
{
           g_dsp_buffer.empty[g_dsp_buffer.playing]=1U;
           g_dsp_buffer.playing^=1U;
           if(g_dsp_buffer.empty[g_dsp_buffer.playing])g_dsp_buffer.overlap=1U;
           __mov(al, byte ptr g_dsp_dma);
           __mov(ah, al);
           __add(al, 0x04);
           __out(0x0a, al);
           __mov(al, ah);
           __out(0x0c, al);
           __add(al, 0x48);
           __out(0x0b, al);
           outportb(g_dsp_regs.base[g_dsp_dma], indexbyte(g_dsp_buffer.address[g_dsp_buffer.playing], 0U));
           outportb(g_dsp_regs.base[g_dsp_dma], indexbyte(g_dsp_buffer.address[g_dsp_buffer.playing], 1U));
           outportb(g_dsp_regs.page[g_dsp_dma], indexbyte(g_dsp_buffer.address[g_dsp_buffer.playing], 2U));
           outportb(g_dsp_regs.count[g_dsp_dma], indexbyte(g_dsp_buffer.length[g_dsp_buffer.playing], 0U));
           outportb(g_dsp_regs.count[g_dsp_dma], indexbyte(g_dsp_buffer.length[g_dsp_buffer.playing], 1U));
           __mov(al, byte ptr g_dsp_dma);
           __out(0x0a, al);
           dsp_write(0x0014);
           dsp_write(indexbyte(g_dsp_buffer.length[g_dsp_buffer.playing], 0U));
           dsp_write(indexbyte(g_dsp_buffer.length[g_dsp_buffer.playing], 1U));
           __mov(dx, word ptr g_dsp_address);
           __add(dx, 0x000e);
           __in(al, dx);
           dsp_old_irq();
           __mov(al, 0x20);
           __out(0x20, al);
}

/* End of source */