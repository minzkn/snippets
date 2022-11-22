#include <stdio.h>
#include <dir.h>
#include <stdlib.h>
#include <string.h>

void main(void)
{
 char s_get_string[512];
 char s_drive[MAXDRIVE], s_dir[MAXDIR], s_file[MAXFILE], s_ext[MAXEXT];
 FILE *s_fp;
           printf("Make format of MINZ source\nInput filename[Filename.xxx]: ");
           scanf("%s", s_get_string);
           strupr(&s_get_string[0]);
           fnsplit(&s_get_string[0], &s_drive[0], &s_dir[0], &s_file[0], &s_ext[0]);
           sprintf(&s_get_string[0], "%s%s%s%s", &s_drive[0], &s_dir[0], &s_file[0], ".ASM");
           s_fp=fopen(&s_get_string[0], "wt");
           if(s_fp)
           {
            fprintf(s_fp, "COMMENT \#\n Name: MINZ O/S\n File: %s\n Copy: Copyright\(c\)1996-1999 by Cho JaeHyuk\n What: MINZ operating system kernel source\n\#\n\n.286\n\nFILE_%s_%s equ \'%s\'\n\n\Include SEGMENT.INC\n\n\; End of source", &s_get_string, &s_file[0], "ASM", &s_get_string[0]);
            fclose(s_fp);
           }
           else printf("Write error[SOURCE]\n");
           sprintf(&s_get_string[0], "%s%s%s%s", &s_drive[0], &s_dir[0], &s_file[0], ".H");
           s_fp=fopen(&s_get_string[0], "wt");
           if(s_fp)
           {
            fprintf(s_fp, "\/*\n  Name: MINZ O/S\n  File: %s\n  Copy: Copyright\(c\)1996-1999 by Cho JaeHyuk\n  What: MINZ operating system kernel header\n*\/\n\n", &s_get_string);
            fprintf(s_fp, "\#ifndef FILE_%s_%s\n#define FILE_%s_%s \"%s\"\n\n\#endif  FILE_%s_%s\n\n", &s_file[0], "H", &s_file[0], "H", &s_get_string[0], &s_file[0], "H");
            fprintf(s_fp, "\/* End of source *\/");
            fclose(s_fp);
           }
           else printf("Write error[HEADER]\n");
}
