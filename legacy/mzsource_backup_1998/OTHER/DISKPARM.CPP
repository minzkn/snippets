/*
INT 1E - SYSTEM DATA - DISKETTE PARAMETERS
Notes:        the default parameter table (see #1129) is located at F000h:EFC7h for
	  100% compatible BIOSes
	if the table is changed, INT 13/AH=00h should be called to ensure that
	  the floppy-disk controller is appropriately reprogrammed
SeeAlso: INT 13/AH=0Fh,INT 41"HARD DISK 0",INT 4D/AH=0Ah

Format of diskette parameter table:
Offset        Size        Description        (Table 1129)
 00h        BYTE        first specify byte
		bits 7-4: step rate (Fh=2ms,Eh=4ms,Dh=6ms,etc.)
		bits 3-0: head unload time (0Fh = 240 ms)
 01h        BYTE        second specify byte
		bits 7-1: head load time (01h = 4 ms)
		bit    0: non-DMA mode (always 0)
 02h        BYTE        delay until motor turned off (in clock ticks)
 03h        BYTE        bytes per sector (00h = 128, 01h = 256, 02h = 512, 03h = 1024)
 04h        BYTE        sectors per track
 05h        BYTE        length of gap between sectors (2Ah for 5.25", 1Bh for 3.5")
 06h        BYTE        data length (ignored if bytes-per-sector field nonzero)
 07h        BYTE        gap length when formatting (50h for 5.25", 6Ch for 3.5")
 08h        BYTE        format filler byte (default F6h)
 09h        BYTE        head settle time in milliseconds
 0Ah        BYTE        motor start time in 1/8 seconds
---IBM SurePath BIOS---
 0Bh        BYTE        maximum track number
 0Ch        BYTE        data transfer rate
 0Dh        BYTE        drive type in CMOS
SeeAlso: #3078 at INT 4D/AH=09h
*/

#include<stdio.h> // Standard IO
#include<conio.h>

unsigned char *intvect1e_data
	      = (unsigned char *)( *((unsigned long *)(0x1e*4)) );;

void viewdiskparm()
{
     // address setting
     clrscr();
     printf("Int vect 1E address %p\n",intvect1e_data);
     printf("* Diskette parameters \n");
     printf("* first specify byte  - step rate        %.2xh\n",(*intvect1e_data & 224) >> 4);
     printf("                        head unload time %.2xh\n",(*intvect1e_data & 31));
     printf("* second specify byte - head load time   %.2xh\n",(*(intvect1e_data+1) & 254) >> 1);
     printf("                        %s\n",( (*(intvect1e_data+1) & 1) ? "DMA mode" : "non-DMA mode" ));
     printf("* delay until motor turned off     -     %.2xh\n",*(intvect1e_data+2));
     switch( (*(intvect1e_data+3)) )
     {
       case 0:
	 printf("* bytes per sector                 -     %.2xh\n",128);
	 break;
       case 1:
	 printf("* bytes per sector                 -     %.2xh\n",256);
	 break;
       case 2:
	 printf("* bytes per sector                 -     %.2xh\n",512);
	 break;
       case 3:
	 printf("* bytes per sector                 -     %.2xh\n",1024);
	 break;
     }
     printf("* sectors per track                -     %.2xh\n",*(intvect1e_data+4));
     printf("* length of gap between sectors    -     %.2xh\n",*(intvect1e_data+5));
     printf("* data length                      -     %.2xh\n",*(intvect1e_data+6));
     printf("* gap length when formatting       -     %.2xh\n",*(intvect1e_data+7));
     printf("* format filler byte               -     %.2xh\n",*(intvect1e_data+8));
     printf("* head settle time in milliseconds -     %.2xh\n",*(intvect1e_data+9));
     printf("* motor start time in 1/8 seconds  -     %.2xh\n",*(intvect1e_data+10));
     return;
}

void main(void)
{
     viewdiskparm();
     return;
}
