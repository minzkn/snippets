/* HARD.C */

#define _HARD_C_

#include "standard.h"
#include "disk.h"

static unsigned int pascal IDE_ports[][9]={
                     {0x01f0, 0x01f1, 0x01f2, 0x01f3, 0x01f4, 0x01f5, 0x01f6, 0x01f7, 0x03f6},
                     {0x0170, 0x0171, 0x0172, 0x0173, 0x0174, 0x0175, 0x01f6, 0x01f7, 0x0376}
                    };

void far pascal IDE_init(void);
unsigned int far pascal IDE_ready(unsigned int);
unsigned int far pascal IDE_result(unsigned int);
unsigned int far pascal IDE_busy(unsigned int);
unsigned int far pascal IDE_reset_controller(unsigned int);
unsigned int far pascal IDE_readbyte(unsigned int, unsigned int);
void far pascal IDE_writebyte(unsigned int, unsigned int, unsigned int);
unsigned int far pascal IDE_out(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
unsigned int far pascal IDE_reset(unsigned int, unsigned int);
unsigned int far pascal IDE_read(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void far *);
unsigned int far pascal IDE_write(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void far *);
unsigned int far pascal IDE_verify(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void far *);
unsigned int far pascal IDE_identify(unsigned int, unsigned int, void far *);

t_disk_func IDE_func={IDE_reset, IDE_read, IDE_write, IDE_verify, IDE_identify};

void far pascal IDE_init(void)
{
 unsigned int s_ctrl, s_disk;
 unsigned int far *s_buffer;
 t_disk far *s_node;
           s_buffer=(unsigned int far *)farmalloc(IDE_SECTORSIZE);
           for(s_ctrl=0U;s_ctrl<2U;s_ctrl++)
           {
            if(IDE_reset_controller(s_ctrl)==1U)
            {
             for(s_disk=0U;s_disk<2U;s_disk++)
             {
              if(IDE_identify(s_ctrl, s_disk, s_buffer)==1U)
              {
               IDE_reset(s_ctrl, s_disk);
               s_node=(t_disk far *)farmalloc(sizeof(t_disk));
               s_node->func=&IDE_func;
               s_node->type=DISK_IDE;
               s_node->mode=DISK_LBA;
               s_node->ctrl=s_ctrl;
               s_node->disk_num=s_disk;
               s_node->sector_size=IDE_SECTORSIZE;
               s_node->head=*(s_buffer+0x37);
               s_node->cylinder=*(s_buffer+0x36);
               s_node->sector=*(s_buffer+0x38);
               DISK_add_disk(s_node);
               DISK_add_partition(s_node, 0LU);
               #ifdef DEBUG_MINZ
                printf("Detect IDE type: fixed$u, disk$u, Hd=$u, Cyl=$u, Sec=$u\r\n",\
                 s_ctrl, s_disk, s_node->head, s_node->cylinder, s_node->sector);
               #endif DEBUG_MINZ
              }
              else break;
             }
            }
            else break;
           }
           farfree(s_buffer);
}

unsigned int far pascal IDE_ready(unsigned int s_ctrl)
{
 unsigned int s_retries=1000U;
           while(--s_retries&&(inportb(IDE_ports[s_ctrl][IDE_PORT_STATUS])&(IDE_STAT_READY|IDE_STAT_BUSY))!=IDE_STAT_READY);
           return(s_retries);
}

unsigned int far pascal IDE_result(unsigned int s_ctrl)
{
 unsigned int s_status=inportb(IDE_ports[s_ctrl][IDE_PORT_STATUS]);
           if((s_status&(IDE_STAT_BUSY|IDE_STAT_READY|IDE_STAT_WRERR|IDE_STAT_SEEK|IDE_STAT_ERR))==(IDE_STAT_READY|IDE_STAT_SEEK))return(0U); /* OK */
           if(s_status&IDE_STAT_ERR)s_status=inportb(IDE_ports[s_ctrl][IDE_PORT_ERROR]);
           return(1U);
}

unsigned int far pascal IDE_busy(unsigned int s_ctrl)
{
 unsigned int s_count;
           for(s_count=0U;s_count<65535U;s_count++)
           {
            if(IDE_STAT_READY==(inportb(IDE_ports[s_ctrl][IDE_PORT_STATUS])&(IDE_STAT_BUSY|IDE_STAT_READY)))break;
           }
           s_count=inportb(IDE_ports[s_ctrl][IDE_PORT_STATUS])&(IDE_STAT_BUSY|IDE_STAT_READY|IDE_STAT_SEEK);
           if(s_count==(IDE_STAT_READY|IDE_STAT_SEEK))return(0U);
           #ifdef DEBUG_MINZ
            printf("Hard disk controller times out!\r\n");
           #endif DEBUG_MINZ
           return(1U);
}

unsigned int far pascal IDE_reset_controller(unsigned int s_ctrl)
{
 unsigned int s_count;
           outportb(IDE_ports[s_ctrl][IDE_PORT_CMD], 0x04);
           for(s_count=0U;s_count<1000U;s_count++)nowork();
           outportb(IDE_ports[s_ctrl][IDE_PORT_CMD], 0x00);
           for(s_count=0U;s_count<10000U&&IDE_busy(s_ctrl);s_count++);
           #ifdef DEBUG_MINZ
            if(IDE_busy(s_ctrl))printf("Hard disk controller still busy\r\n");
           #endif DEBUG_MINZ
           if((s_count=inportb(IDE_ports[s_ctrl][IDE_PORT_ERROR]))!=IDE_STAT_ERR)
           {
            #ifdef DEBUG_MINZ
             printf("Hard disk controller reset failed \(Fixed%u\): %02x\r\n", s_ctrl, s_count);
            #endif DEBUG_MINZ
            return(0U);
           }
           return(1U);
}

unsigned int far pascal IDE_readbyte(unsigned int s_ctrl, unsigned int s_port)
{
           IDE_busy(s_ctrl);
           return(inportb(s_port));
}

void far pascal IDE_writebyte(unsigned int s_ctrl, unsigned int s_port, unsigned int s_data)
{
           IDE_busy(s_ctrl);
           outportb(s_port, s_data);
}

unsigned int far pascal IDE_out(s_ctrl, s_disk, s_nsect, s_sect, s_head, s_cyl, s_cmd)
unsigned int s_ctrl, s_disk, s_nsect, s_sect, s_head, s_cyl, s_cmd;
{
 unsigned int s_port;
           if(s_disk>1||s_head>15)
           {
            #ifdef DEBUG_MINZ
             printf("Trying to write bad sector\r\n");
            #endif DEBUG_MINZ
            return(0U);
           }
           if(!IDE_ready(s_ctrl))
           {
            #ifdef DEBUG_MINZ
             printf("Hard disk controller not ready\r\n");
            #endif DEBUG_MINZ
            return(0U);
           }
           outportb(IDE_ports[s_ctrl][IDE_PORT_CMD], 0);
           s_port=IDE_ports[s_ctrl][IDE_PORT_ERROR];
           IDE_writebyte(s_ctrl, s_port++, 0x300>>2);
           IDE_writebyte(s_ctrl, s_port++, s_nsect);
           IDE_writebyte(s_ctrl, s_port++, s_sect);
           IDE_writebyte(s_ctrl, s_port++, s_cyl);
           IDE_writebyte(s_ctrl, s_port++, s_cyl>>8);
           IDE_writebyte(s_ctrl, s_port++, 0xa0|(s_disk<<4)|s_head);
           outportb(s_port, s_cmd);
           return(1U);
}

unsigned int far pascal IDE_reset(unsigned int s_ctrl, unsigned int s_disk)
{
           IDE_reset_controller(s_ctrl);
           IDE_out(s_ctrl, s_disk, 1U, 0U, 0U, 1U, IDE_CMD_CALIBRATE);
           IDE_busy(s_ctrl);
           if(IDE_result(s_ctrl))
           {
            IDE_reset_controller(s_ctrl);
            #ifdef DEBUG_MINZ
             printf("Reset failed!\r\n");
            #endif DEBUG_MINZ
            return(0U);
           }
           return(1U);
}

unsigned int far pascal IDE_read(s_ctrl, s_disk, s_head, s_cyl, s_sec, s_buffer)
unsigned int s_ctrl, s_disk, s_head, s_cyl, s_sec; void far *s_buffer;
{
           IDE_out(s_ctrl, s_disk, 1U, s_sec, s_head, s_cyl, IDE_CMD_READ);
           IDE_busy(s_ctrl);
           inportsw(IDE_ports[s_ctrl][IDE_PORT_DATA], s_buffer, IDE_SECTORSIZE>>1);
           if(IDE_result(s_ctrl))
           {
            IDE_reset_controller(s_ctrl);
            #ifdef DEBUG_MINZ
             printf("Read failed!\r\n");
            #endif DEBUG_MINZ
            return(0U);
           }
           return(1U);
}

unsigned int far pascal IDE_write(s_ctrl, s_disk, s_head, s_cyl, s_sec, s_buffer)
unsigned int s_ctrl, s_disk, s_head, s_cyl, s_sec; void far *s_buffer;
{
           IDE_out(s_ctrl, s_disk, 1U, s_sec, s_head, s_cyl, IDE_CMD_WRITE);
           IDE_busy(s_ctrl);
           outportsw(IDE_ports[s_ctrl][IDE_PORT_DATA], s_buffer, IDE_SECTORSIZE>>1);
           if(IDE_result(s_ctrl))
           {
            IDE_reset_controller(s_ctrl);
            #ifdef DEBUG_MINZ
             printf("Write failed!\r\n");
            #endif DEBUG_MINZ
            return(0U);
           }
           return(1U);
}

unsigned int far pascal IDE_verify(s_ctrl, s_disk, s_head, s_cyl, s_sec, s_buffer)
unsigned int s_ctrl, s_disk, s_head, s_cyl, s_sec; void far *s_buffer;
{
           IDE_out(s_ctrl, s_disk, 1U, s_sec, s_head, s_cyl, IDE_CMD_VERIFY);
           IDE_busy(s_ctrl);
           inportsw(IDE_ports[s_ctrl][IDE_PORT_DATA], s_buffer, IDE_SECTORSIZE>>1);
           if(IDE_result(s_ctrl))
           {
            IDE_reset_controller(s_ctrl);
            #ifdef DEBUG_MINZ
             printf("Verify failed!\r\n");
            #endif DEBUG_MINZ
            return(0U);
           }
           return(1U);
}

unsigned int far pascal IDE_identify(unsigned int s_ctrl, unsigned int s_disk, void far *s_buffer)
{
           IDE_out(s_ctrl, s_disk, 1U, 0U, 0U, 1U, IDE_CMD_IDENTIFY);
           IDE_busy(s_ctrl);
           inportsw(IDE_ports[s_ctrl][IDE_PORT_DATA], s_buffer, IDE_SECTORSIZE>>1);
           if(IDE_result(s_ctrl))
           {
            IDE_reset_controller(s_ctrl);
            #ifdef DEBUG_MINZ
             printf("Identify failed!\r\n");
            #endif DEBUG_MINZ
            return(0U);
           }
           return(1U);
}

/* End of source */