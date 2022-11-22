#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

void main(void)
{
 char buffer[512];
           printf("DEC2HEX v1.00 by Cho JaeHyuk   Enter = Quit\n");
           while(1)
           {
            printf("Input DEC: ");
            gets(&buffer[0]);
            if(stricmp(&buffer[0], "exit")==0||buffer[0]=='\0')break;
            printf("         : %08lx\n", atol(&buffer[0]));
           }
           printf("End of DEC2HEX\n");
}