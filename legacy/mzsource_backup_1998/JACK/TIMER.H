/*
  Timer.H
*/

extern signed char s_tick;
extern signed int Timer;
extern signed int MaxTimer;
extern void (far * TimerFunction)(void);
extern void interrupt (far * Old_Timer)(void);

extern void far DefaultTimer(void);
extern void interrupt TimerInterrupt(void);
extern void far StartTimer(signed int count, void (far *Function)(void));
extern void far CloseTimer(void);

/* End of source */