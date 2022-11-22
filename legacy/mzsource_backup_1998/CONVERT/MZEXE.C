/* Code by Cho JaeHyuk */

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include <dir.h>

#define MZ_BUFFERSIZE        512

signed char  *g_defaultexe=".EXE";
signed char  *g_defaultbin=".EXE";
signed char   g_exename[MAXPATH];
signed char   g_binname[MAXPATH];
unsigned char g_buffer[MZ_BUFFERSIZE];

void Usage(void)
{
	   printf("\nUSAGE: MZEXE <EXEC_file> <EXE_file>\n");
}

void DefaultExt(signed char *s_dest, signed char *s_src, signed char *s_defext)
{
 signed char s_drive[MAXDRIVE], s_dir[MAXDIR], s_file[MAXFILE], s_ext[MAXEXT];
	   fnsplit(s_src, s_drive, s_dir, s_file, s_ext);
	   strcpy(s_dest, s_drive);
	   strcat(s_dest, s_dir);
	   strcat(s_dest, s_file);
	   strcat(s_dest, s_defext);
}

void ConvertMZ(void)
{
 FILE          *s_fr, *s_fw;
 unsigned int  s_header[14];
 unsigned int  s_kernel[16];
 unsigned long s_code_start, s_code_size, s_head_size;
 unsigned long s_count;
 int s_data;
	   s_fr=fopen(g_exename, "rb");
	   if(s_fr==NULL)
	   {
	    printf("\nError: Can not open [READ] - \`%s\'.\n", g_exename);
	    return;
	   }
	   printf("\nMessage: Compress \`%s\' to \`%s\'.\n", g_exename, g_binname);

	   s_fw=fopen(g_binname, "wb");
	   if(s_fw==NULL)
	   {
	    printf("\nError: Can not open [WRITE] - \`%s\'.\n", g_binname);
	    fclose(s_fr);
	    return;
	   }

	   fread(s_header, 14<<1, 1, s_fr);
	   memcpy(s_kernel, s_header, 14<<1);
	   s_kernel[14]=0x494d;
	   s_kernel[15]=0x5a4e;
	   s_code_start=((unsigned long)s_header[4])<<4;
	   s_code_size =(unsigned long)(s_header[2]-1)*512LU+(s_header[1]?s_header[1]:512)-s_code_start;

	   s_head_size=((unsigned long)s_header[3]<<2)+(unsigned long)(16LU<<1);
	   if(s_head_size%16)s_kernel[4]=(unsigned int)((s_head_size+(16-(s_head_size%16)))>>4); /* Header size */
	   else s_kernel[4]=(unsigned int)((s_head_size)>>4); /* Header size */
	   s_kernel[12]=0x20U; /* Para */   /* relocate_start */
	   s_code_size+=s_head_size;
	   s_kernel[2]=(unsigned int)((s_code_size/512)+1U);
	   s_kernel[1]=(unsigned int)(s_code_size%512);
	   if(s_kernel[1]==0)s_kernel[1]=512U;

	   //fwrite(kn_table, sizeof(kn_table), 1, s_fw);
	   fwrite(s_kernel, 16<<1, 1, s_fw);
	   fseek(s_fr, s_header[12], SEEK_SET);

	   for(s_count=0LU;s_count<(unsigned long)s_kernel[3];s_count++)
	   {
	    fread(g_buffer, 4, 1, s_fr);
	    fwrite(g_buffer, 4, 1, s_fw);
	   }
	   //fseek(s_fw, ((unsigned long)s_kernel[4]<<4)+sizeof(kn_table), SEEK_SET);
	   fseek(s_fw, ((unsigned long)s_kernel[4]<<4), SEEK_SET);
	   fseek(s_fr, s_header[4]<<4, SEEK_SET);

	   while((s_data=fgetc(s_fr))!=EOF)fputc(s_data, s_fw);

	   fclose(s_fr);
	   fclose(s_fw);
	   printf("Success!\n");
}

void main(signed int s_argc, signed char *s_argv[])
{
	   printf("MZEXE (v1.00) Copyright (c) 1998 by MINZ team\nCode by Cho JaeHyuk\n");
	   if(s_argc!=2&&s_argc!=3)Usage();
           else
	   {
	    if(access(s_argv[1], 0)==0)strcpy(g_exename, s_argv[1]);
            else DefaultExt(g_exename, s_argv[1], g_defaultexe);
            if(s_argc==3)strcpy(g_binname, s_argv[2]);
            else DefaultExt(g_binname, g_exename, g_defaultbin);
            strupr(g_exename);
            strupr(g_binname);
	    if(strcmp(g_exename, g_binname)==0)printf("\nError: EXEC_file == TARGET_file\n");
            else ConvertMZ();
           }
}

/* End of source */
