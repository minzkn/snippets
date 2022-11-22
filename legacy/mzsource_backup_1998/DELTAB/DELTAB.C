#include <stdio.h>

void main(int s_argc, unsigned char *s_argv[])
{
           if(s_argc==3)
           {
            FILE *s_fr, *s_fw;
            s_fr=fopen(s_argv[1], "rt");
            if(s_fr)
            {
             s_fw=fopen(s_argv[2], "wt");
             if(s_fw)
             {
              int s_data;
              while((s_data=fgetc(s_fr))!=EOF)
              {
               if(s_data=='\t')
               {
                int s_count;
                for(s_count=0;s_count<8;s_count++)fputc(' ', s_fw);
               }
               else fputc(s_data, s_fw);
              }
              fclose(s_fw);
              printf("Delete TAB character: \`%s\' -> \`%s\'\n", s_argv[1], s_argv[2]);
             }
             else printf("File open error! [WRITE]\n");
             fclose(s_fr);
            }
            else printf("File open error! [READ]\n");
           }
           else printf("Usage: DELTAB source target\n");
}