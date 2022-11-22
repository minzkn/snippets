/*
  JACK's HIMEM.H
*/

extern void (far *himemCALL)(void);
extern signed int himemVersion;
extern unsigned int s_AllocTable[N_MAXHIMEMHANDLE];

extern signed int far himemInit(void);
extern unsigned int far himemGetSize(unsigned int far *n_big, unsigned int far *n_total);
extern unsigned int far himemAlloc(unsigned int n_size);
extern signed int far himemFree(unsigned int n_handle);
extern unsigned long far himemLock(unsigned int n_handle);
extern signed int far himemUnLock(unsigned int n_handle);
extern signed int far himemMEM2XMS(void far *n_source, unsigned int n_handle, unsigned long n_offset, unsigned long n_length);
extern signed int far himemXMS2MEM(unsigned int n_handle, void far *n_target, unsigned long n_offset, unsigned long n_length);
extern signed int far himemXMS2XMS(unsigned int n_sourcehandle, unsigned int n_targethandle, unsigned long n_offsetsource, unsigned long n_offsettarget, unsigned long n_length);
extern void far himemAllFree(void);

/* End of source */