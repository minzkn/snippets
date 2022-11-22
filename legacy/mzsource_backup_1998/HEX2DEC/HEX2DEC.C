#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

void main(void)
{
 unsigned long num;
           printf("HEX2DEC v1.00 by Cho JaeHyuk   \'0\' = Quit\n");
           while(1)
           {
            printf("Input HEX: ");
            num=0LU;
            scanf("%lx", &num);
            while(kbhit())getch();
            if(num==0LU)break;
            printf("         : %lu\n", num);
           }
           printf("End of HEX2DEC\n");
}