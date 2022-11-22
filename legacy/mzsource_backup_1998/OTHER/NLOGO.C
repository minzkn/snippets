#include<dos.h>
#include<string.h>
#include<conio.h>
#include<stdlib.h>

#define INTERRUPT  0x1C

void interrupt far (*old_handler)(void);
void interrupt Interrupt(void);
void PutLogoDisplay(void);

char logomunja[80];
char munjax;
char count;

void main(char argc,char *argv[])
{
	   if(argc>1){
		      strcpy(logomunja,"[ ");
		      strcat(logomunja,argv[1]);
		      strcat(logomunja," ]");
		      }
           else strcpy(logomunja,"[ NMS soft's Logo Display R1.0.1 ]");
	   munjax=80-strlen(logomunja);
	   old_handler=getvect(INTERRUPT);
	   setvect(INTERRUPT,Interrupt);
	   keep(NULL,_SS+(_SP/16)-_psp);
}

void interrupt Interrupt(void)
{
	   old_handler();
	   PutLogoDisplay();
}

void PutLogoDisplay(void)
{
 long xcount=(long)munjax;
	   count=NULL;
	   while(count<strlen(logomunja)){
					  *((unsigned char far *)0xB8000000L+(xcount*2L)  )=logomunja[count++];
					  *((unsigned char far *)0xB8000000L+(xcount*2L)+1L)=YELLOW+(BLUE*0x10);
					  xcount++;
					  }
}
