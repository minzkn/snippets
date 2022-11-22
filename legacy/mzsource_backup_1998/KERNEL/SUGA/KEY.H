/*
  Name: SUGA O/S
  File: KEY.H
  Copy: Copyright(c)1998-1999 by Cho JaeHyuk
  What: Header of "KEY.CPP"
*/

#ifndef Use_Inline
#define Use_Inline                     "Use inline command"
#pragma inline
#endif  Use_Inline

#ifndef FILE_KEY_H
#define FILE_KEY_H                     "KEY.H"

#include <KERNEL.H>

#ifndef FILE_KEY_CPP

extern void interrupt interrupt_key(void);

extern e_boolean far *              g_key;

#endif  FILE_KEY_CPP

#endif  FILE_KEY_H

/* End of source */