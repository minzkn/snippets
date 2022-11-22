#include <stdio.h>
#include <dos.h>

void main(void)
{
 FILE *fp;
 int i;
           fp=fopen("RGB.DAT", "wb");
           outportb(0x03c7, 0);
           for(i=0;i<48;i++)
           {
            fputc(inportb(0x03c9), fp);
           }
           fclose(fp);
}