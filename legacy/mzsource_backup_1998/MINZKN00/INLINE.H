/*
  Name: MINZ O/S
  File: INLINE.H
  Copy: Copyright(c)1996-1999 by Cho JaeHyuk
  What: MINZ operating system kernel header
*/

#ifndef FILE_INLINE_H
#define FILE_INLINE_H "INLINE.H"

#pragma inline

#define __asm                          asm

#define __dump(x)                      __asm db (x)
#define __dumpb(x)                     __asm db (x)
#define __dumpw(x)                     __asm dw (x)
#define __dumpd(x)                     __asm dd (x)
#define __mov(d,s)                     __asm mov (d),(s)
#define __xor(d,s)                     __asm xor (d),(s)
#define __push(x)                      __asm push (x)
#define __pop(x)                       __asm pop (x)
#define __pushf()                      __asm pushf
#define __popf()                       __asm popf
#define __out(p,d)                     __asm out (p),(d)
#define __in(d,p)                      __asm in (d),(p)
#define __int(x)                       __asm int (x)
#define __shl(d,n)                     __asm shl (d),(n)
#define __shr(d,n)                     __asm shr (d),(n)
#define __lds(r,p)                     __asm lds (r),(p)
#define __les(r,p)                     __asm les (r),(p)
#define __add(d,s)                     __asm add (d),(s)
#define __sub(d,s)                     __asm sub (d),(s)
#define __adc(d,s)                     __asm adc (d),(s)
#define __sbb(d,s)                     __asm sbb (d),(s)
#define __call(f)                      __asm call (f)
#define __cli()                        __asm cli
#define __sti()                        __asm sti
#define __jmp(x)                       __asm jmp x
#define __pusha()                      __asm pusha
#define __popa()                       __asm popa
#define __neg(x)                       __asm neg (x)
#define __mul(x)                       __asm mul (x)
#define __div(x)                       __asm div (x)
#define __loop(x)                      __asm loop x
#define __xchg(d,s)                    __asm xchg (d),(s)
#define __or(d,s)                      __asm or (d),(s)
#define __and(d,s)                     __asm and (d),(s)
#define __jc(x)                        __asm jc x
#define __jnc(x)                       __asm jnc x
#define __jz(x)                        __asm jz x
#define __jnz(x)                       __asm jnz x
#define __ja(x)                        __asm ja x
#define __jna(x)                       __asm jna x
#define __jae(x)                       __asm jae x
#define __jb(x)                        __asm jb x
#define __jnb(x)                       __asm jnb x
#define __jbe(x)                       __asm jbe x
#define __jcxz(x)                      __asm jcxz x
#define __je(x)                        __asm je x
#define __jne(x)                       __asm jne x
#define __jle(x)                       __asm jle x
#define __clc()                        __asm clc
#define __stc()                        __asm stc
#define __inc(x)                       __asm inc (x)
#define __dec(x)                       __asm dec (x)
#define __cmp(d,s)                     __asm cmp (d),(s)
#define __test(d,s)                    __asm test (d),(s)
#define __not(x)                       __asm not (x)
#define __cld()                        __asm cld
#define __lodsb()                      __asm lodsb
#define __lodsw()                      __asm lodsw
#define __lodsd()                      __asm lodsd
#define __stosb()                      __asm stosb
#define __stosw()                      __asm stosw
#define __stosd()                      __asm stosd
#define __movsb()                      __asm movsb
#define __movsw()                      __asm movsw
#define __movsd()                      __asm movsd
#define __rep()                        __asm rep

#define __farcall(s,o)                 __dumpb(0x9a);__dumpw((o));__dumpw((s))
#define __farjump(s,o)                 __dumpb(0xea);__dumpw((o));__dumpw((s))
#define __reboot()                     __farjump(0xffff, 0x0000);
#define __clear_cache()                __asm jmp short $ + 2
#define __disable()                    __cli()
#define __enable()                     __sti()
#define __notuse(x)                    if(x)

#endif  FILE_INLINE_H

/* End of source */