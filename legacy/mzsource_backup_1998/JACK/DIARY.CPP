/*
  JACK's DIARY.CPP
*/

#include <conio.h>
#include "common.h"
#include "graph.h"
#include "menu.h"
#include "himem.h"

void far Diary(void)
{
           Help("¸aŽ ³a‰¡ ¯¼·e ·©‹¡¸w");
           while(!kbhit()){MouseProcess();TimeProcess();}
           while(kbhit())getch();
           Help("·©‹¡¸w·i ¹·žaÐs“¡”a.");
}

/* End of source */