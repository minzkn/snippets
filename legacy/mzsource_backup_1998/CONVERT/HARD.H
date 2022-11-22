/* HARD.H */

#ifndef _HARD_H_
#define _HARD_H_

#define IDE_SECTORSIZE       512

#define IDE_STAT_ERR         0x01
#define IDE_STAT_INDEX       0x02
#define IDE_STAT_ECC         0x04
#define IDE_STAT_DRQ         0x08
#define IDE_STAT_SEEK        0x10
#define IDE_STAT_WRERR       0x20
#define IDE_STAT_READY       0x40
#define IDE_STAT_BUSY        0x80

#define IDE_PORT_DATA        0
#define IDE_PORT_ERROR       1
#define IDE_PORT_NSECTOR     2
#define IDE_PORT_SECTOR      3
#define IDE_PORT_LCYL        4
#define IDE_PORT_HCYL        5
#define IDE_PORT_CURRENT     6
#define IDE_PORT_STATUS      7
#define IDE_PORT_CMD         8
#define IDE_PORT_PRECOMP     IDE_PORT_ERROR
#define IDE_PORT_COMMAND     IDE_PORT_STATUS

#define IDE_CMD_RESTORE      0x10
#define IDE_CMD_CALIBRATE    0x10
#define IDE_CMD_READ         0x20
#define IDE_CMD_WRITE        0x30
#define IDE_CMD_VERIFY       0x40
#define IDE_CMD_FORMAT       0x50
#define IDE_CMD_INIT         0x60
#define IDE_CMD_SEEK         0x70
#define IDE_CMD_DIAGNOSE     0x90
#define IDE_CMD_SPECIFY      0x91
#define IDE_CMD_IDENTIFY     0xec

#ifndef _HARD_C_

extern void far pascal IDE_init(void);
extern unsigned int far pascal IDE_ready(unsigned int);
extern unsigned int far pascal IDE_result(unsigned int);
extern unsigned int far pascal IDE_busy(unsigned int);
extern unsigned int far pascal IDE_reset_controller(unsigned int);
extern unsigned int far pascal IDE_readbyte(unsigned int, unsigned int);
extern void far pascal IDE_writebyte(unsigned int, unsigned int, unsigned int);
extern unsigned int far pascal IDE_out(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern unsigned int far pascal IDE_reset(unsigned int, unsigned int);
extern unsigned int far pascal IDE_read(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void far *);
extern unsigned int far pascal IDE_write(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void far *);
extern unsigned int far pascal IDE_verify(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, void far *);
extern unsigned int far pascal IDE_identify(unsigned int, unsigned int, void far *);

#endif _HARD_C_

#endif _HARD_H_

/* End of source */