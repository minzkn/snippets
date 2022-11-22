#include <stdio.h>

void main(void)
{
 int s_count;
 char s_name[13];
 FILE *s_fp;
           for(s_count=0;s_count<256;s_count++)
           {
            sprintf(&s_name[0], "FILE%04x.DAT", s_count);
            s_fp=fopen(&s_name[0], "wb");
            fprintf(s_fp, "File name: %s\r\n\x1b", &s_name[0]);
            fclose(s_fp);
           }
}