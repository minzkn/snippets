/* FILETIME.C */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <dir.h>
#include <dos.h>
#include <alloc.h>
#include <mem.h>
#include <io.h>
#include <fcntl.h>

struct time g_time;
struct date g_date;

void main(void);
int scanning(void *);

void main(void)
{
           getdate(&g_date);
           gettime(&g_time);
           printf("Time match version1.00 by Cho JaeHyuk <MATCH-TIME %02d:%02d:%02d>\n\n", g_time.ti_hour, g_time.ti_min, g_time.ti_sec);
           scanning(NULL);
}

int scanning(void *s_path)
{
 struct ffblk *s_ffblk=malloc(sizeof(struct ffblk));
 int s_done;
           if(s_path)
           {
            char *out_string=malloc(MAXPATH);
            chdir(s_path);
            getcwd(out_string, MAXPATH);
            printf("Scanning: %s\n", out_string);
            free(out_string);
           }
           s_done=findfirst("*.*", s_ffblk, 0xffff);
           do
           {
            if(s_ffblk->ff_name[0]!='.')
            {
             if(s_ffblk->ff_attrib&FA_DIREC)
             {
              if(scanning(&s_ffblk->ff_name[0])==0)break;
              chdir("..");
             }
             else if(!(s_ffblk->ff_attrib&FA_LABEL))
             {
              struct ftime s_ftime;
              int s_handle;
              s_ftime.ft_hour =g_time.ti_hour;
              s_ftime.ft_min  =g_time.ti_min;
              s_ftime.ft_tsec =g_time.ti_sec>>1;
              s_ftime.ft_year =g_date.da_year-1980;
              s_ftime.ft_month=g_date.da_mon;
              s_ftime.ft_day  =g_date.da_day;
              s_handle=open(s_ffblk->ff_name, O_RDONLY|O_BINARY);
              setftime(s_handle, &s_ftime);
              close(s_handle);
             }
            }
            s_done=findnext(s_ffblk);
           }while(!s_done);
           free(s_ffblk);
           return(1);
}

/* End of source */