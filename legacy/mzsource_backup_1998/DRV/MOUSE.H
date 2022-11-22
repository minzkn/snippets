/*
  Name: MINZ O/S
  File: MOUSE.H
  Copy: Copyright(c)1996-1999 by Cho JaeHyuk
  What: MINZ operating system kernel header
*/

#ifndef FILE_MOUSE_H
#define FILE_MOUSE_H "MOUSE.H"

extern void far pascal open_mouse(void);
extern void far pascal close_mouse(void);
extern void far pascal process_mouse(void);

#endif  FILE_MOUSE_H

/* End of source */
