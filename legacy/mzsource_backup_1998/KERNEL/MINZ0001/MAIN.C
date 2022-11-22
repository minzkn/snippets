/*
 Complete BORLAND C/C++ Version3.1
 Copyright (c) 2000 by MINZ SOFTWARE - JaeHyuk CHO - [minzkn@dreamwiz.com]
*/

#include "mzlib.h"

void far pascal KernelMain(void);
void far pascal Test(void);

void far pascal *S_STR_Company=DEF_Company;
void far pascal *S_STR_Name=DEF_Name;
unsigned int pascal S_STR_VersionHigh=DEF_VersionHigh;
unsigned int pascal S_STR_VersionLow=DEF_VersionLow;
void far pascal * S_STR_VersionType=DEF_VersionType;

void far pascal KernelMain(void)
{
           // Print logo message
           PrintF("\xfe $s operating system  Version$X.$02X $s  Copyright (c) $s\r\n\r\n",\
            S_STR_Name,\
            S_STR_VersionHigh, S_STR_VersionLow,\
            S_STR_VersionType,\
            S_STR_Company);
           PrintF("\xfe Minimum memory above kernel : $,LU + \xe0 Bytes.\r\n", INFO_ProgramSize);
           Test();

           // Print shutdown message
           PrintF("\r\n\xfe Shutdown $s operating system.\r\n\r\n", DEF_Name);
}

void far pascal Test(void)
{
 register unsigned int s_fixed, s_device, s_return, s_counter;
 unsigned int far *s_buffer=New(0x0200);
 void far *s_model=New(0x0200);
           for(s_fixed=0;s_fixed<2;s_fixed++)
           {
            for(s_device=0;s_device<2;s_device++)
            {
             MemClear(s_buffer, 0x0200);
             s_return=IDE_Identify(s_fixed, s_device, s_buffer);
             if(s_return==0x5050)
             {
              for(s_counter=27;s_counter<46;s_counter++)
              {
               s_buffer[s_counter]=(s_buffer[s_counter]>>0x08)|((s_buffer[s_counter]&0xff)<<0x08);
              }
              StrCpyN(s_model, &s_buffer[27], 40);
              StrOptimize(s_model, 0x20);
              PrintK("\xfe Detected IDE interface : RETURN=$04Xh, FIXED=$02Xh, DEVICE=$02Xh\r\n  - Model : \"$s\"\r\n  - CHS   : Heads=$Ud, Cylinders=$Ud, Sectors=$Ud - Real\r\n            Heads=$Ud, Cylinders=$Ud, Sectors=$Ud - Logical\r\n  - Capacity : $,LU Bytes - Real\r\n               $,LU Bytes - Calc\r\n",\
               s_return, s_fixed, s_device,\
               s_model,\
               s_buffer[55], s_buffer[54], s_buffer[56],\
               s_buffer[3], s_buffer[1], s_buffer[6],\
               (unsigned long)(((unsigned long)s_buffer[60]|((unsigned long)s_buffer[61]<<0x0010))*0x0200LU),\
               (unsigned long)s_buffer[55]*(unsigned long)s_buffer[54]*(unsigned long)s_buffer[56]*0x0200LU);
             }
            }
           }
           Delete(s_model);
           Delete(s_buffer);
}

/* End of source */
