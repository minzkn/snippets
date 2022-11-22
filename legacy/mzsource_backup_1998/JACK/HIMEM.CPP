/*
  JACK's HIMEM.CPP
*/

#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <stdlib.h>
#include <dir.h>
#include "common.h"

void (far *himemCALLFunction)(void);
unsigned int himemVersion;
himemMEM n_himem;
unsigned int s_AllocTable[N_MAXHIMEMHANDLE];
unsigned int s_AllocPtr=0;

inline void far himemCALL(void){(*himemCALLFunction)();}

signed int far himemInit(void)
{
 union REGS rin;
 struct SREGS sin;
 unsigned int n_counter;
           for(n_counter=0;n_counter<N_MAXHIMEMHANDLE;n_counter++)s_AllocTable[n_counter]=0x0000;
           if(s_MD.IsSWAP==N_True){s_MD.IsHIMEM=N_False; return(N_False);}
           rin.x.ax=0x4300; int86(0x2f, &rin, &rin);
           if(rin.h.al!=0x80){s_MD.IsHIMEM=N_False; return(N_False);}
           rin.x.ax=0x4310; int86x(0x2f, &rin, &rin, &sin);
           (void far *)himemCALLFunction=(void far *)MK_FP(sin.es, rin.x.bx);
           _AX=0x0000; himemCALL(); himemVersion=(unsigned int)_AX;
           s_MD.IsHIMEM=N_True;
           return(N_True);
}

unsigned int far himemGetSize(unsigned int *n_big, unsigned int *n_total)
{
 unsigned int n_bigmem, n_totalmem;
 unsigned int n_return;
           _AH=0x08; himemCALL(); n_return=(unsigned int)_BX;
           n_totalmem=(unsigned int)_DX; n_bigmem=(unsigned int)_AX;
           *n_total=n_totalmem; *n_big=n_bigmem;
           switch((n_return&0xff)!=0)
           {
            case 0x80:
            case 0x81:
            case 0xa0:
            case 0xa1: s_MD.IsHIMEM=N_False; break;
            default: break;
           }
           if(n_totalmem<N_MINHIMEMSIZE)s_MD.IsHIMEM=N_False;
           if(n_bigmem<N_MINHIMEMSIZE)s_MD.IsHIMEM=N_False;
           return(n_return&0x00ff);
}

unsigned long far himemLock(unsigned int n_handle)
{
 unsigned int n_lock[2];
           _AH=0x0c; _DX=s_AllocTable[n_handle]; himemCALL();
           n_lock[0]=_DX; n_lock[1]=_BX;
           if(_AX!=N_True)return(N_False);
           return(((signed long)n_lock[0]<<16)|(signed long)n_lock[1]);
}

signed int far himemUnLock(unsigned int n_handle)
{
           _AH=0x0d; _DX=s_AllocTable[n_handle]; himemCALL();
           if(_AX!=N_True)return(N_False);
           return(N_True);
}

unsigned int far himemAlloc(unsigned int n_size)
{
 unsigned int n_return;
 unsigned int n_count;
           if(s_MD.IsHIMEM==N_False)
           {
            union REGS rin;
            struct SREGS sin;
            signed char far *n_filenamet=new(signed char far [30]);
            signed char far *n_filename=new(signed char far [MAXPATH]);
            for(n_count=1;n_count<N_MAXHIMEMHANDLE;n_count++)
            {
             if(s_AllocTable[n_count]==0){s_AllocPtr=n_count; break;}
            }
            sprintf(n_filenamet, "jackswap\\~jswp%03d.swp", s_AllocPtr);
            strcpy(n_filename, MakePath(n_filenamet));
            delete(n_filenamet); UnLink(n_filename);
            rin.x.ax=0x3c00; rin.x.cx=0x05; rin.x.dx=FP_OFF(n_filename);
            sin.ds=FP_SEG(n_filename); int86x(0x21, &rin, &rin, &sin);
            n_return=_AX; delete(n_filename); s_AllocTable[s_AllocPtr]=n_return;
            return(s_AllocPtr);
           }
           _AH=0x09; _DX=(unsigned int)n_size; himemCALL();
           if(_AX!=N_True)return(N_False);
           n_return=(unsigned int)_DX;
           for(n_count=1;n_count<N_MAXHIMEMHANDLE;n_count++)
           {
            if(s_AllocTable[n_count]==0)
            {
             s_AllocTable[n_count]=n_return; n_return=(unsigned int)n_count;
             break;
            }
           }
           himemLock(s_AllocTable[n_return]);
           return(n_return);
}

signed int far himemFree(unsigned int n_handle)
{
           if(s_MD.IsHIMEM==N_False)
           {
            union REGS rin;
            signed char far *n_filenamet=new(signed char far [30]);
            signed char far *n_filename=new(signed char far [MAXPATH]);
            sprintf(n_filenamet, "jackswap\\~jswp%03d.swp", n_handle);
            strcpy(n_filename, MakePath(n_filenamet)); delete(n_filenamet);
            rin.x.ax=0x3e00; rin.x.bx=s_AllocTable[n_handle];
            int86(0x21, &rin, &rin); s_AllocTable[n_handle]=0;
            UnLink(n_filename); delete(n_filename); return(N_True);
           }
           himemUnLock(s_AllocTable[n_handle]);
           _AH=0x0a; _DX=s_AllocTable[n_handle]; himemCALL();
           s_AllocTable[n_handle]=0;
           if(_AX!=N_True)return(N_False);
           return(N_True);
}

signed int far himemMEM2XMS(void far *n_source, unsigned int n_handle, unsigned long n_offset, unsigned long n_length)
{
           if(s_MD.IsHIMEM==N_False)
           {
            union REGS rin;
            struct SREGS sin;
            rin.x.ax=0x4200; rin.x.bx=s_AllocTable[n_handle];
            rin.x.cx=(unsigned int)((n_offset>>16)&0xffff);
            rin.x.dx=(unsigned int)(n_offset&0xffff);
            int86(0x21, &rin, &rin); rin.x.ax=0x4000;
            rin.x.bx=s_AllocTable[n_handle]; rin.x.cx=(unsigned int)n_length;
            rin.x.dx=FP_OFF(n_source); sin.ds=FP_SEG(n_source);
            int86x(0x21, &rin, &rin, &sin); return(N_True);
           }
           asm push si;
           asm push ds;
           n_himem.length=n_length; n_himem.sourcehandle=0;
           n_himem.sourceaddress=(unsigned long)n_source;
           n_himem.targethandle=s_AllocTable[n_handle];
           n_himem.targetaddress=(unsigned long)n_offset;
           _SI=(unsigned int)FP_OFF(&n_himem);
           _DS=(unsigned int)FP_SEG(&n_himem);
           _AH=0x0b; himemCALL();
           asm pop ds;
           asm pop si;
           if(_AX!=N_True)return(N_False);
           return(N_True);
}

signed int far himemXMS2MEM(unsigned int n_handle, void far *n_target, unsigned long n_offset, unsigned long n_length)
{
           if(s_MD.IsHIMEM==N_False)
           {
            union REGS rin;
            struct SREGS sin;
            rin.x.ax=0x4200; rin.x.bx=s_AllocTable[n_handle];
            rin.x.cx=(unsigned int)((n_offset>>16)&0xffff);
            rin.x.dx=(unsigned int)(n_offset&0xffff);
            int86(0x21, &rin, &rin); rin.x.ax=0x3f00;
            rin.x.bx=s_AllocTable[n_handle];
            rin.x.cx=(unsigned int)n_length;
            rin.x.dx=FP_OFF(n_target); sin.ds=FP_SEG(n_target);
            int86x(0x21, &rin, &rin, &sin);
            return(N_True);
           }
           asm push si;
           asm push ds;
           n_himem.length=n_length;
           n_himem.sourcehandle=s_AllocTable[n_handle];
           n_himem.sourceaddress=(unsigned long)n_offset;
           n_himem.targethandle=0;
           n_himem.targetaddress=(unsigned long)n_target;
           _SI=(unsigned int)FP_OFF(&n_himem);
           _DS=(unsigned int)FP_SEG(&n_himem);
           _AH=0x0b; himemCALL();
           asm pop ds;
           asm pop si;
           if(_AX!=N_True)return(N_False);
           return(N_True);
}

signed int far himemXMS2XMS(unsigned int n_sourcehandle, unsigned int n_targethandle, unsigned long n_offsetsource, unsigned long n_offsettarget, unsigned long n_length)
{
           if(s_MD.IsHIMEM==N_False)return(N_False);
           asm push si;
           asm push ds;
           n_himem.length=n_length;
           n_himem.sourcehandle=s_AllocTable[n_sourcehandle];
           n_himem.sourceaddress=(unsigned long)n_offsetsource;
           n_himem.targethandle=s_AllocTable[n_targethandle];
           n_himem.targetaddress=(unsigned long)n_offsettarget;
           _SI=(unsigned int)FP_OFF(&n_himem); _DS=(unsigned int)FP_SEG(&n_himem);
           _AH=0x0b; himemCALL();
           asm pop ds;
           asm pop si;
           if(_AX!=N_True)return(N_False);
           return(N_True);
}

void far himemAllFree(void)
{
 signed int n_count;
           for(n_count=1;n_count<N_MAXHIMEMHANDLE;n_count++)
           {
            if(s_AllocTable[n_count]!=0){himemFree(s_AllocTable[n_count]);}
           }
}

/* End of source */