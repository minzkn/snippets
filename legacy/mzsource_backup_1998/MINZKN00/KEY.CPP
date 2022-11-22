/*
  Name: SUGA O/S
  File: KEY.CPP
  Copy: Copyright(c)1998-1999 by Cho JaeHyuk
  What: Keyboard control
*/

#define FILE_KEY_CPP                   "KEY.CPP"

#include <KERNEL.H>

void interrupt interrupt_key(void);

e_boolean far *              g_key;

void interrupt interrupt_key(void)
{
 unsigned int s_key=inportb(0x0060);
           if(s_key&0x0080)g_key[s_key&0x007f]=False;
           else g_key[s_key&0x7f]=True;
           interrupt_proc[0x09]();
}

/* End of source */