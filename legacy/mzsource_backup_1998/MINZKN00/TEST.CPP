/*
  Name: SUGA O/S
  File: TEST.CPP
  Copy: Copyright(c)1998-1999 by Cho JaeHyuk
  What: Code for Test of new code
*/

#define FILE_TEST_CPP                  "TEST.CPP"

#include "INLINE.H"

unsigned int far pascal dsp_read(void)
{
           __mov(dx, 0x220);
           __add(dx, 0x000e); /* Wait index */
           __mov(cx, 0xffff);
 l_dsp_read_00:;
           __or(cx, cx);
           __jz(short l_dsp_read_01);
           __dec(cx);
           __in(al, dx);
           __test(al, 0x80);
           __jz(short l_dsp_read_00);
           __mov(dx, 0x220);
           __add(dx, 0x000a);
           __in(al, dx);
 l_dsp_read_01:;
           __xor(ah, ah);
           return(_AX);
}

void far pascal dsp_write(unsigned int s_data)
{
           __mov(dx, 0x220);
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

unsigned int far pascal dsp_open(void)
{
 unsigned int s_irq;
           __mov(dx, 0x220);
           __add(dx, 0x06);
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
            if(inportb(0x220+0x0a)==0xaa) /* Detect index */
            {
             dsp_write(0x00d1);
             return(1);
            }
           }
           __mov(dx, 0x220);
           __add(dx, 0x000e);
           __in(al, dx);
           __mov(al, 0x20);
           __out(0x20, al);
           return(0);
}

void far pascal test(void)
{
           /*unsigned char s_buffer[2048U]; // Çiœá¯aÈá ·ª‹¡&³a‹¡ žË¥ ÑÂ·¥
           while(!bioskey(0x0001))
           {
            g_drive_head->next->cluster_rw(&s_buffer[0U], random(64000U)+2U, 0U, 4U, D_READ);
            g_text->nputs(0x0f, 2048U, &s_buffer[0U], 0, 0);
            delay(1000U);
           }*/

           /*{ // q·e ¡A¡¡Ÿ¡ ˆñ¬a £¹ µÅ¸å‰w¡ ¦…Ÿ¡ ÉA¯aËa
            t_fullname far *s_fullname;
            t_fullname far *s_name;
            g_text->tty_printf("Far core: $lu\r\n", 640000LU-farcore());
            g_text->tty_printf("Far core left: $lu\r\n", farcoreleft());
            s_name=s_fullname=g_drive_head->next->open_name("c:/project/suga/suga.exe");
            g_text->tty_printf("Far core: $lu\r\n", 640000LU-farcore());
            g_text->tty_printf("Far core left: $lu\r\n", farcoreleft());
            if(s_name)
            {
             while(s_name)
             {
              g_text->tty_printf("[$s]\r\n", &s_name->name[0U]);
              s_name=s_name->next;
             }
             g_drive_head->next->close_name(s_fullname);
            }
           }*/

           /*{ // Ìa·© TYPE ˆñ¬a
            t_root s_root;
            void far *s_filename="test0003.txt";
            t_drive far *s_drive;
            if(g_original_boot==True)
            {
             if(g_boot_sector->drive&0x80)s_drive=found_drive('C');
             else s_drive=found_drive('A');
            }
            else s_drive=found_drive('C');
            if(s_drive->found_name_in_root(&s_root, s_filename)==True&&s_drive)
            {
             unsigned long s_startcluster;
             void far *s_description;
             unsigned long s_sizecount;
             unsigned int s_length;
             s_description=farmalloc(s_drive->boot.boot.sectors_per_cluster*512U);
            while(!bioskey(0x0001))
            {
             s_sizecount=0LU;
             s_startcluster=s_root.start_cluster;
             while((s_startcluster&0x0000ffffLU)!=0x0000ffffLU)
             {
              if(s_sizecount<s_root.size)s_length=(unsigned int)(s_root.size%(s_drive->boot.boot.sectors_per_cluster*512U));
              else s_length=s_drive->boot.boot.sectors_per_cluster*512U;
              s_sizecount+=s_drive->boot.boot.sectors_per_cluster*512U;
              s_drive->cluster_rw(s_description, s_startcluster, 0U, s_drive->boot.boot.sectors_per_cluster, D_READ);
              g_text->tty_nputs(s_length, s_description);
              s_startcluster=s_drive->next_cluster(s_startcluster);
             }
             }
             farfree(s_description);
            }
            else
            {
             g_text->tty_printf("\xfe Found not $s in root\r\n", s_filename);
            }
           }*/

           /*{ // Printf drive list
            t_drive far *s_drive=g_drive_head;
            while(s_drive)
            {
             g_text->tty_printf("\xfe Drive: $c\r\n", s_drive->drive_name);
             s_drive=s_drive->next;
            }
           }*/

           /*{
            t_root_handle far *s_handle;
            unsigned int s_data;
            s_handle=___open('C', "system");
            if(s_handle)
            {
             while((s_data=___getbyte(s_handle))!=D_EOF)
             {
              g_text->tty_putc(s_data);
             }
             //g_text->tty_printf("Size = $lu\r\n", s_handle->offset);
             ___close(s_handle);
            }
            else g_text->tty_printf("Found not file\r\n");
           }*/

           /*{
            while(bioskey(0x0001))bioskey(0x0000);
            g_graphics->scale_y=2U;
            while(!bioskey(0x0001))
            {
             //g_graphics->clear(random(256U));
             //g_graphics->putpixel(random(256U), random(g_graphics->hres), random(g_graphics->vres));
             //g_graphics->hline(random(256U), g_graphics->minx, g_graphics->maxx, random(g_graphics->vres));
             //g_graphics->vline(random(256U), random(g_graphics->hres), g_graphics->miny, g_graphics->maxy);
             //g_graphics->fillbox(random(256U), 100U, 100U, 699U, 599U);
             g_graphics->ehprintf(random(256U), random(256U), random(g_graphics->hres), random(g_graphics->vres), "Ðe‹iÀáŸ¡ˆa ´é aa ¸i–I»¡ »¡Åa¥¡¸a! Under standard?");
            }
            while(bioskey(0x0001))bioskey(0x0000);
           }*/

           /*{
            unsigned int s_data, s_skip;
            t_root_handle far *s_handle;
            void far *s_wavename="suga.wav";
            s_handle=___open('C', s_wavename);
            for(s_skip=0U;s_skip<128U;s_skip++)___getbyte(s_handle);
            while(bioskey(0x0001))bioskey(0x0000);
            s_data=200U;
            while(!bioskey(0x0001))
            {
             s_data=___getbyte(s_handle);
             //s_data|=(___getbyte(s_handle)<<8U);
             //if(s_data&0x8000)nosound();
             //else sound(s_data);
             //sound(s_data&0x8000);
             sound(s_data-0x80);
             for(s_skip=0U;s_skip<400U;s_skip++)nop();
             if(s_handle->eof==True)
             {
              ___close(s_handle);
              s_handle=___open('C', s_wavename);
              for(s_skip=0U;s_skip<128U;s_skip++)___getbyte(s_handle);
              delay(100U);
              s_data=200U;
           //   break;
             }
            }
            nosound();
            ___close(s_handle);
            while(bioskey(0x0001))bioskey(0x0000);
           }*/

           /*{
            while(bioskey(0x0001))bioskey(0x0000);
            while(!bioskey(0x0001))
            {
             g_graphics->clear(random(256U));
            }
            while(bioskey(0x0001))bioskey(0x0000);
           }*/

           {
            unsigned int s_data, s_x=0U;
            dsp_open();
            while(bioskey(0x0001))bioskey(0x0000);
            while(True)
            {
             while(bioskey(0x0001))bioskey(0x0000);
             if(inportb(0x60)==0x01)break;
             dsp_write(0x20);
             s_data=dsp_read();
             g_graphics->lineto(0x0f, s_x++, g_graphics->maxy-(s_data));
             if(s_x>g_graphics->maxx)
             {
              s_x=g_graphics->minx;
              g_graphics->clear(0U);
              g_graphics->to_x=s_x;
              g_graphics->to_y=(g_graphics->maxy-s_data);
             }
            }
            while(bioskey(0x0001))bioskey(0x0000);
           }

           /*{
            unsigned int s_count;
            for(s_count=0U;s_count<120U;s_count++)
            {
             g_graphics->tty_putc(s_count);
            }
           }*/

           /*{
            while(bioskey(0x0001))bioskey(0x0000);
            while(!bioskey(0x0001))
            {
             g_graphics->line(random(256U), random(g_graphics->hres), random(g_graphics->vres), random(g_graphics->hres), random(g_graphics->vres));
             {
             // Scroll
              //g_graphics->scrollup(1U, g_graphics->minx, g_graphics->miny, g_graphics->maxx, g_graphics->maxy);
             }
            }
            while(bioskey(0x0001))bioskey(0x0000);
           }*/

           /*{
            unsigned int s_data, s_skip;
            t_root_handle far *s_handle;
            void far *s_wavename="suga.wav";
            if(YM_iscard())g_graphics->tputs(0x0f, 0xffff, "¬a¶…—a Äa—aˆa ˆñÂ‰–A´ö¯s“¡”a.", 0, 2);
            YM_register(1U, 0x20);
            YM_selectwave(0, 0);
            YM_enablewave();
            s_handle=___open('C', s_wavename);
            for(s_skip=0U;s_skip<128U;s_skip++)___getbyte(s_handle);
            while(bioskey(0x0001))bioskey(0x0000);
            s_data=200U;
            while(!bioskey(0x0001))
            {
             s_data=___getbyte(s_handle);
             if(s_data&0x80)YM_frequency_off(0, 0, 3);
             else
             {
              YM_frequency_on(0, s_data, 3);
              delay(s_data);
              YM_frequency_off(0, 0, 0);
             }
             //for(s_skip=0U;s_skip<200U;s_skip++)nop();
             if(s_handle->eof==True)
             {
              ___close(s_handle);
              s_handle=___open('C', s_wavename);
              for(s_skip=0U;s_skip<128U;s_skip++)___getbyte(s_handle);
              delay(100U);
              s_data=200U;
              break;
             }
            }
            YM_disablewave();
            ___close(s_handle);
            while(bioskey(0x0001))bioskey(0x0000);
           }*/

           /*{
            YM_register(1U, 0x20);
            YM_selectwave(0U, 0U);
            YM_enablewave();
            while(!bioskey(0x0001))
            {
             YM_frequency_on(0U, random(460U), 0U);
             delay(100U);
             YM_frequency_off(0U, 0U, 0U);
            }
            YM_disablewave();
           }*/
}

/* End of source */

/*
 extern void far pascal YM_register(unsigned int, unsigned int);
 extern void far pascal YM_enablewave(void);
 extern void far pascal YM_disablewave(void);
 extern void far pascal YM_method1(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_method2(unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_method3(unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_method4(unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_method5(unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_method6(unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_selectwave(unsigned int, unsigned int);
 extern void far pascal YM_frequency_on(unsigned int, unsigned int, unsigned int);
 extern void far pascal YM_frequency_off(unsigned int, unsigned int, unsigned int);
 extern unsigned int far pascal YM_iscard(void);
*/