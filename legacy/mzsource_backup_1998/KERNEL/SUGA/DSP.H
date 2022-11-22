/*
  Name: MINZ O/S
  File: DSP.H
  Copy: Copyright(c)1996-1999 by Cho JaeHyuk
  What: MINZ operating system kernel header
*/

#ifndef FILE_DSP_H
#define FILE_DSP_H "DSP.H"

#define D_DSP_RESET                    (0x0006)

#ifndef FILE_DSP_C

extern unsigned int far pascal dsp_read(void);
extern void far pascal dsp_write(unsigned int);
extern e_boolean far pascal dsp_open(unsigned int, unsigned int, unsigned int);
extern unsigned int far pascal dsp_rate(unsigned int);
extern void far pascal dsp_alloc(void far *, unsigned int, void far *, unsigned int);
extern void far pascal dsp_stop(void);
extern unsigned int far pascal dsp_dma_count(void);
extern unsigned int far pascal dsp_dma_status(void);
extern void far pascal dsp_record(void far *, unsigned int, void far *, unsigned int);
extern void interrupt dsp_handler(void);

extern unsigned int g_dsp_address, g_dsp_irq, g_dsp_dma, g_dsp_access;
extern void interrupt (far pascal * dsp_old_irq)(void);

#endif  FILE_DSP_C

#endif  FILE_DSP_H

/* End of source */