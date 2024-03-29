/* EXE2KN.C */

#define _EXE2KN_C_

#include "standard.h"
#include "selectos.h"

unsigned char pascal g_mzover[]={
 0xfa, 0xe8, 0x02, 0x00, 0xeb, 0x09, 0x8b, 0xec, 0x8b, 0x5e, 0x00, 0x83, 0xeb, 0x04, 0xc3, 0x0e, 0x1f, 0x8d, 0x87, 0xe0, 0x00, 0xc1, 0xe8, 0x04, 0x8c, 0xc9, 0x03, 0xc1, 0x03, 0x87, 0xe8, 0x00, 0x89,
 0x87, 0xc2, 0x00, 0xb9, 0x10, 0x00, 0x8c, 0xc8, 0xf7, 0xe1, 0x8d, 0x8f, 0xe0, 0x00, 0x03, 0xc1, 0x83, 0xd2, 0x00, 0x03, 0x87, 0xf8, 0x00, 0x89, 0x87, 0xc4, 0x00, 0x89, 0x97, 0xc6, 0x00, 0x8b, 0x87,
 0xc2, 0x00, 0x03, 0x87, 0xf6, 0x00, 0x89, 0x87, 0xc0, 0x00, 0x8b, 0x87, 0xf4, 0x00, 0x89, 0x87, 0xbe, 0x00, 0x8b, 0x87, 0xc2, 0x00, 0x03, 0x87, 0xee, 0x00, 0x8e, 0xd0, 0x8b, 0xa7, 0xf0, 0x00, 0x8b,
 0x8f, 0xe6, 0x00, 0x0b, 0xc9, 0x74, 0x4c, 0x51, 0xb9, 0x10, 0x00, 0x8b, 0x87, 0xc4, 0x00, 0x8b, 0x97, 0xc6, 0x00, 0x8b, 0xf8, 0x83, 0xe7, 0x0f, 0xf7, 0xf1, 0x8e, 0xc0, 0x8b, 0x87, 0xc2, 0x00, 0xf7,
 0xe1, 0x52, 0x50, 0x26, 0x8b, 0x45, 0x02, 0xf7, 0xe1, 0x26, 0x03, 0x05, 0x83, 0xd2, 0x00, 0x5f, 0x5e, 0x03, 0xc7, 0x13, 0xd6, 0x8b, 0xf8, 0x83, 0xe7, 0x0f, 0xf7, 0xf1, 0x8e, 0xc0, 0x8b, 0x87, 0xc2,
 0x00, 0x26, 0x01, 0x05, 0x83, 0x87, 0xc4, 0x00, 0x04, 0x83, 0x97, 0xc6, 0x00, 0x00, 0x59, 0xe2, 0xb4, 0x0e, 0x1f, 0x0e, 0x07, 0xfb, 0xeb, 0x00, 0xea, 0x4d, 0x49, 0x4e, 0x5a, 0x20, 0x6b, 0x65, 0x72,
 0x6e, 0x65, 0x6c, 0x20, 0x6f, 0x76, 0x65, 0x72, 0x6c, 0x61, 0x79, 0x20, 0x62, 0x79, 0x20, 0x43, 0x68, 0x6f, 0x20, 0x4a, 0x61, 0x65, 0x48, 0x79, 0x75, 0x6b
};

unsigned int pascal g_ver[3]={0x03, 0x10, 0xe1};

void far cdecl main(void);

void far cdecl main(void)
{
           startdos(); argdos();
           printf("MINZ soft - Convert EXE to BIN(Overlay) Version $x.$02x$c  $s\r\nCopyright (c) MINZ soft Corp 1997-1998. Code by Cho JaeHyuk\(NOTICE\)\r\n\r\n", g_ver[0], g_ver[1], g_ver[2], __DATE__);
           if(_argc<2)
           {
            printf("\xfe All support binary - Relocate, IP\(0~65535\), Argument\(Keep ES\)\r\n");
            printf("\xfe Usage: EXE2KN InFile<.EXE> [OutFile<.BIN>]\r\n");
            printf(   "         InFile  = EXE, OVL  <= TINY, SMALL, MEDIUM, COMPACT, LARGE, HUGE\r\n");
            printf(   "         OutFile = BIN & COM <= Overlap\(16bit\)\r\n");
           }
           else
           {
            unsigned int s_srchandle, s_dsthandle, s_return;
            unsigned char s_srcname[MAXPATH], s_dstname[MAXPATH], s_drive[MAXDRIVE], s_dir[MAXDIR], s_file[MAXFILE], s_ext[MAXEXT];
            unsigned char s_buffer[512];
            fnsplit(_argv[1], &s_drive[0], &s_dir[0], &s_file[0], &s_ext[0]);
            strcpy(&s_srcname[0], _argv[1]);
            strupr(&s_srcname[0]);
            s_srchandle=open(_argv[1], O_BINARY|O_RDONLY);
            if(s_srchandle==0xffff)
            {
             sprintf(&s_srcname[0], "$s$s$s.EXE", &s_drive[0], &s_dir[0], &s_file[0]);
             strupr(&s_srcname[0]);
             s_srchandle=open(&s_srcname[0], O_BINARY|O_RDONLY);
            }
            if(s_srchandle!=0xffff)
            {
             if(_argc>2)
             {
              strcpy(&s_dstname[0], _argv[2]);
              fnsplit(&s_dstname[0], &s_drive[0], &s_dir[0], &s_file[0], &s_ext[0]);
             }
             else
             {
              strcpy(&s_dstname[0], _argv[1]);
              fnsplit(&s_dstname[0], &s_drive[0], &s_dir[0], &s_file[0], &s_ext[0]);
              strcpy(&s_ext[0], ".BIN");
             }
             sprintf(&s_dstname[0], "$s$s$s$s", &s_drive[0], &s_dir[0], &s_file[0], &s_ext[0]);
             strupr(&s_dstname[0]);
             s_dsthandle=creat(&s_dstname[0], FA_ARCH);
             if(s_dsthandle!=0xffff)
             {
              write(s_dsthandle, &g_mzover, sizeof(g_mzover));
              do
              {
               s_return=read(s_srchandle, &s_buffer[0], 512U);
               if(s_return)write(s_dsthandle, &s_buffer[0], s_return);
              }while(s_return&&s_return!=0xffff);
              close(s_dsthandle);
              printf("Success: Convert \`$s\' to \`$s\'\r\n", &s_srcname[0], &s_dstname[0]);
             }
             else printf("Error: Cannot write \`$s\'\r\n", &s_dstname[0]);
             close(s_srchandle);
            }
            else printf("Error: Cannot read \`$s\'\r\n", _argv[1]);
           }
}

/* End of source */
