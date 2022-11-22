/*
  Timer.CPP
*/

#include "common.h"

signed char s_tick;
signed int Timer=0;
signed int MaxTimer=0;
void (far * TimerFunction)(void);
void interrupt (far * Old_Timer)(void);

void far DefaultTimer(void)
{
           s_tick=N_True;
}

void interrupt TimerInterrupt(void)
{
           Old_Timer();
           if(Timer==(-1))return;
           Timer++;
           if(Timer>=MaxTimer){Timer=(-1); TimerFunction(); Timer=0;}
}

void far StartTimer(signed int count, void (far *Function)(void))
{
 struct Vect{signed char far *Vect;}far *Vect1c=(struct Vect far *)0x0070;
           Timer=0; MaxTimer=count; TimerFunction=Function; s_tick=N_True;
	         (signed char far *)Old_Timer=(signed char far *)Vect1c->Vect;
	         Vect1c->Vect=(signed char far *)TimerInterrupt;
}

void far CloseTimer(void)
{
 struct Vect{signed char far *Vect;}far *Vect1c=(struct Vect far *)0x0070;
           (signed char far *)Vect1c->Vect=(signed char far *)Old_Timer;
}

/* End of source */