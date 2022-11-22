/*
  Name: SUGA O/S
  File: FILE.CPP
  Copy: Copyright(c)1998-1999 by Cho JaeHyuk
  What: FILE controll
*/

#define FILE_FILE_CPP                  "FILE.CPP"

#include <KERNEL.H>
#include <FILE.H>

t_root_handle far * far pascal ___open(unsigned int, void far *);
void far pascal ___close(t_root_handle far *);
unsigned int far pascal ___getbyte(t_root_handle far *);
t_root_handle far * far pascal ___getdir(t_root_handle far *);

t_root_handle far * far pascal ___open(unsigned int s_drive_name, void far *s_filename)
{
 t_root_handle far *s_handle;
 t_drive far *s_drive;
           s_drive=found_drive(s_drive_name);
           if(s_drive==NULL)return(NULL);
           s_handle=new t_root_handle;
           if(s_drive->found_name_in_root(&s_handle->root, s_filename)==False)
           {
            delete(s_handle);
            return(NULL);
           }
           s_handle->drive=s_drive;
           s_handle->offset_cluster=0LU;
           s_handle->offset=0LU;
           s_handle->offset_cluster=s_handle->root.start_cluster;
           s_handle->offset_div=0U;
           s_handle->eof=False;
           s_handle->drive->cluster_rw(&s_handle->sector.sector[0U], s_handle->offset_cluster, s_handle->offset_div, 1U, D_READ);
           return(s_handle);
}

void far pascal ___close(t_root_handle far *s_root)
{
           if(s_root)delete(s_root);
}

unsigned int far pascal ___getbyte(t_root_handle far *s_handle)
{
 unsigned int s_byte;
           if((s_handle->offset>=s_handle->root.size&&!(s_handle->root.attrib&D_ATTRIB_SUBDIR))||s_handle->offset==0xffff)
           {
            s_handle->eof=True;
            return(D_EOF);
           }
           s_byte=s_handle->sector.sector[(unsigned int)((s_handle->offset++)%512LU)];
           if((s_handle->offset%512LU)==0LU)
           {
            s_handle->offset_div++;
            if(s_handle->offset_div>=((unsigned int)s_handle->drive->boot.boot.sectors_per_cluster))
            {
             s_handle->offset_div=0LU;
             s_handle->offset_cluster=s_handle->drive->next_cluster(s_handle->offset_cluster);
            }
            s_handle->drive->cluster_rw(&s_handle->sector.sector[0U], s_handle->offset_cluster, s_handle->offset_div, 1U, D_READ);
           }
           return(s_byte&0x00ff);
}

t_root_handle far * far pascal ___getdir(t_root_handle far *s_handle)
{
 t_root_handle far *s_return=new t_root_handle;
 unsigned char far *s_root=new unsigned char far [sizeof(t_root)];
 unsigned int s_count;
           for(s_count=0U;s_count<sizeof(t_root);s_count++)*(s_root+s_count)=___getbyte(s_handle);
           memcpyb(&s_return->root, s_root, sizeof(t_root));
           delete(s_root);
           s_return->drive=s_handle->drive;
           s_return->offset=s_return->offset_cluster=0LU;
           s_return->offset_div=0U;
           s_return->eof=False;
           s_handle->drive->cluster_rw(&s_handle->sector.sector[0U], s_handle->offset_cluster, s_handle->offset_div, 1U, D_READ);
           return(s_return);
}

/* End of source */