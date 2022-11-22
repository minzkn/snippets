/* LOADER.C */

#include "standard.h"
#include "text.h"
#include "dos.h"

unsigned char far *CopyrightStr="Phoenix NoteBIOS 4.05 08CS\r\n"
                       "Copyright 1985-1995 Phoenix Technologies Ltd., All right Reserved.\r\n"
                       "CPU = Pentium 75 MHz\r\n"
                       "00000640K System RAM Passed\r\n"
                       "00007168K Extended RAM Passed\r\n"
                       "System BIOS shadowed\r\n"
                       "Video BIOS shadowed\r\n"
                       "Mouse initialized\r\n\n";

void far (far pascal *module)(void);

unsigned int handle;

void far pascal loader(void)
{
           setvideomode(0x0003);
           printf(CopyrightStr);
           handle=open("KERNEL.BIN", O_RDONLY|O_BINARY);
           if(handle)
           {
            module=farmalloc(64000U);
            read(handle, module, 64000U);
            close(handle);
            module();
            farfree(module);
           }
           else printf("Error: Found not \"KERNEL.BIN\"\r\n");
}

/* End of source */