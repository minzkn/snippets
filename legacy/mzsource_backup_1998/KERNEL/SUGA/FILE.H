/*
  Name: SUGA O/S
  File: FILE.H
  Copy: Copyright(c)1998-1999 by Cho JaeHyuk
  What: Header of "FILE.CPP"
*/

#ifndef FILE_FILE_H
#define FILE_FILE_H

typedef struct ts_root_handle
{
 t_drive far *     drive;
 t_root            root;
 t_sector          sector;
 unsigned long     offset, offset_cluster;
 unsigned int      offset_div;
 e_boolean         eof;
}t_root_handle;

typedef struct ts_pcm_riff
{
 unsigned long file_id;
 unsigned long file_length;
 unsigned long format_type;
 unsigned long format_id;
 unsigned long format_length;
 unsigned int  format_tag;
 unsigned int  channels;
 unsigned long samples_per_sec;
 unsigned long avg_sample;
 unsigned int  block_align;
 unsigned int  bits_per_sample;
}t_pcm_riff;

#ifndef FILE_FILE_CPP

extern t_root_handle far * far pascal ___open(unsigned int, void far *);
extern void far pascal ___close(t_root_handle far *);
extern unsigned int far pascal ___getbyte(t_root_handle far *);
extern t_root_handle far * far pascal ___getdir(t_root_handle far *);

#endif  FILE_FILE_CPP

#endif  FILE_FILE_H

/* End of source */