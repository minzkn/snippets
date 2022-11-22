COMMENT #
 Name: Suga O/S
 File: C0L.ASM
 Copy: Copyright(c)1998-1999 by Cho JaeHyuk
 What: Start-up code for C++
#

.286

include            SEGMENT.INC

extrn              @_begin_main$QN14ts_boot_sector : far
extrn              _main               : far
extrn              @_exit$QUI          : far

public             _g_segment_text
public             _g_segment_DGROUP
public             _g_segment_heap
public             _g_segment_stack

public             _howcpu

public             YM_register
public             YM_enablewave
public             YM_disablewave
public             YM_method1
public             YM_method2
public             YM_method3
public             YM_method4
public             YM_method5
public             YM_method6
public             YM_selectwave
public             YM_frequency_on
public             YM_frequency_off
public             YM_iscard

                   assume cs:_TEXT, ds:DGROUP, es:nothing, ss:_STACK
_TEXT              segment
                   org ORG_ENTRY

COMMENT #
 ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
 ±±±                          * Initialize SUGA *                         ±±±
 ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#

_start_SUGA:
                   cli
                   mov ax, DGROUP
                   mov ds, ax
                   xor ax, ax
                   mov es, ax
                   mov ax, _STACK
                   mov ss, ax
                   mov sp, SIZE_STACK
                   sti

COMMENT #
 ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
 ±±±                            * Call main *                             ±±±
 ±±± 1. Clear all register                                                ±±±
 ±±± 2. Argument                                                          ±±±
 ±±±    Bootsector in memory           = SEG:OFF (2 + 2 = 4 bytes)        ±±±
 ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#
                   xor ax, ax
                   xor bx, bx
                   xor cx, cx
                   xor dx, dx
                   xor si, si
                   xor di, di
                   xor bp, bp
                   push 0000h
                   push 0500h
                   call @_begin_main$QN14ts_boot_sector
                   push 0000h
                   push 0500h
                   call _main
                   sub sp, 2 + 2

COMMENT #
 ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
 ±±±                           * Shutdown SUGA *                          ±±±
 ±±± 1. Try exit to MS-DOS                                                ±±±
 ±±± 2. Reboot (Warm[First hard-disk])                                    ±±±
 ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#
                   push ax
                   call @_exit$QUI
_TEXT              ends

        assume  cs:_TEXT
_TEXT   segment
        public  LDIV@
        public  F_LDIV@
        public  N_LDIV@
        public  LUDIV@
        public  F_LUDIV@
        public  N_LUDIV@
        public  LMOD@
        public  F_LMOD@
        public  N_LMOD@
        public  LUMOD@
        public  F_LUMOD@
        public  N_LUMOD@

N_LDIV@:
        pop     cx                      ;fix up far return
        push    cs
        push    cx
LDIV@:
F_LDIV@:
        xor     cx,cx                   ; signed divide
        jmp     short common

N_LUDIV@:
        pop     cx                      ;fix up far return
        push    cs
        push    cx
LUDIV@:
F_LUDIV@:
        mov     cx,1                    ; unsigned divide
        jmp     short common

N_LMOD@:
        pop     cx                      ;fix up far return
        push    cs
        push    cx
LMOD@:
F_LMOD@:
        mov     cx,2                    ; signed remainder
        jmp     short   common

N_LUMOD@:
        pop     cx                      ;fix up far return
        push    cs
        push    cx
LUMOD@:
F_LUMOD@:
        mov     cx,3                    ; unsigned remainder

;
;       di now contains a two bit control value.  The low order
;       bit (test mask of 1) is on if the operation is unsigned,
;       signed otherwise.  The next bit (test mask of 2) is on if
;       the operation returns the remainder, quotient otherwise.
;
common:
        push    bp
        push    si
        push    di
        mov     bp,sp                   ; set up frame
        mov     di,cx
;
;       dividend is pushed last, therefore the first in the args
;       divisor next.
;
        mov     ax,10[bp]               ; get the first low word
        mov     dx,12[bp]               ; get the first high word
        mov     bx,14[bp]               ; get the second low word
        mov     cx,16[bp]               ; get the second high word

        or      cx,cx
        jnz     slow@ldiv               ; both high words are zero

        or      dx,dx
        jz      quick@ldiv

        or      bx,bx
        jz      quick@ldiv              ; if cx:bx == 0 force a zero divide
                                        ; we don't expect this to actually
                                        ; work

slow@ldiv:

        test    di,1                    ; signed divide?
        jnz     positive                ; no: skip
;
;               Signed division should be done.  Convert negative
;               values to positive and do an unsigned division.
;               Store the sign value in the next higher bit of
;               di (test mask of 4).  Thus when we are done, testing
;               that bit will determine the sign of the result.
;
        or      dx,dx                   ; test sign of dividend
        jns     onepos
        neg     dx
        neg     ax
        sbb     dx,0                    ; negate dividend
        or      di,0Ch
onepos:
        or      cx,cx                   ; test sign of divisor
        jns     positive
        neg     cx
        neg     bx
        sbb     cx,0                    ; negate divisor
        xor     di,4
positive:
        mov     bp,cx
        mov     cx,32                   ; shift counter
        push    di                      ; save the flags
;
;       Now the stack looks something like this:
;
;               16[bp]: divisor (high word)
;               14[bp]: divisor (low word)
;               12[bp]: dividend (high word)
;               10[bp]: dividend (low word)
;                8[bp]: return CS
;                6[bp]: return IP
;                4[bp]: previous BP
;                2[bp]: previous SI
;                 [bp]: previous DI
;               -2[bp]: control bits
;                       01 - Unsigned divide
;                       02 - Remainder wanted
;                       04 - Negative quotient
;                       08 - Negative remainder
;
        xor     di,di                   ; fake a 64 bit dividend
        xor     si,si                   ;
xloop:
        shl     ax,1                    ; shift dividend left one bit
        rcl     dx,1
        rcl     si,1
        rcl     di,1
        cmp     di,bp                   ; dividend larger?
        jb      nosub
        ja      subtract
        cmp     si,bx                   ; maybe
        jb      nosub
subtract:
        sub     si,bx
        sbb     di,bp                   ; subtract the divisor
        inc     ax                      ; build quotient
nosub:
        loop    xloop
;
;       When done with the loop the four register value look like:
;
;       |     di     |     si     |     dx     |     ax     |
;       |        remainder        |         quotient        |
;
        pop     bx                      ; get control bits
        test    bx,2                    ; remainder?
        jz      usequo
        mov     ax,si
        mov     dx,di                   ; use remainder
        shr     bx,1                    ; shift in the remainder sign bit
usequo:
        test    bx,4                    ; needs negative
        jz      finish
        neg     dx
        neg     ax
        sbb     dx,0                    ; negate
finish:
        pop     di
        pop     si
        pop     bp
        retf    8

quick@ldiv:
        div     bx                      ; unsigned divide
                                        ; DX = remainder AX = quotient
        test    di,2                    ; want remainder?
        jz      quick@quo
        xchg    ax,dx

quick@quo:

        xor     dx,dx
        jmp     short finish
                public  SCOPY@
                public  F_SCOPY@

SCOPY@:
F_SCOPY@:
                push    bp
                mov     bp,sp
                push    si
                push    di
                push    ds
                lds     si,dword ptr 6[bp]
                les     di,dword ptr 10[bp]
                cld
                shr     cx, 1
                rep     movsw
                adc     cx, cx
                rep     movsb
                pop     ds
                pop     di
                pop     si
                pop     bp
                retf    8
        public  LXMUL@
        public  F_LXMUL@

LXMUL@          PROC    FAR
F_LXMUL@:
                push    si
                xchg    si,ax           ; save lo1
                xchg    ax,dx
                test    ax,ax           ; skip mul if hi1==0
                jz      nohi1
                mul     bx              ; hi1 * lo2

nohi1:          ; if we jumped here, ax==0 so the following swap works
                jcxz    nohi2           ; skip mul if hi2==0
                xchg    cx, ax          ; result <-> hi2
                mul     si              ; lo1 * hi2
                add     ax, cx          ; ax = hi1*lo2 + hi2*lo1
nohi2:
                xchg    ax,si
                mul     bx              ; lo1 * lo2
                add     dx,si           ; hi order result += partials
                pop     si
                ret
LXMUL@          ENDP
        public  LXLSH@
        public  F_LXLSH@
        public  N_LXLSH@

N_LXLSH@:
        pop     bx                      ;fix up for far return
        push    cs
        push    bx
LXLSH@:
F_LXLSH@:
        cmp     cl,16
        jae     lsh@small
        mov     bx,ax                   ; save the low bits
        shl     ax,cl                   ; now shift each half
        shl     dx,cl
;
;                       We now have a hole in DX where the upper bits of
;                       AX should have been shifted.  So we must take our
;                       copy of AX and do a reverse shift to get the proper
;                       bits to be or'ed into DX.
;
        neg     cl
        add     cl,16
        shr     bx,cl
        or      dx,bx
        retf
lsh@small:
        sub     cl,16                   ; for shifts more than 15, do this
                                        ; short sequence.
        xchg    ax,dx
        xor     ax,ax                   ; We have now done a shift by 16.
        shl     dx,cl                   ; Now shift the remainder.
        retf
        public  LXURSH@
        public  F_LXURSH@
        public  N_LXURSH@

N_LXURSH@:
        pop     bx                      ;fix up far return
        push    cs
        push    bx
LXURSH@:
F_LXURSH@:
        cmp     cl,16
        jae     lsh@small_
        mov     bx,dx                   ; save the high bits
        shr     ax,cl                   ; now shift each half
        shr     dx,cl
;
;                       We now have a hole in AX where the lower bits of
;                       DX should have been shifted.  So we must take our
;                       copy of DX and do a reverse shift to get the proper
;                       bits to be or'ed into AX.
;
        neg     cl
        add     cl,16
        shl     bx,cl
        or      ax,bx
        retf
lsh@small_:
        sub     cl,16                   ; for shifts more than 15, do this
                                        ; short sequence.
        xchg    ax,dx
        xor     dx,dx                   ; We have now done a shift by 16.
        shr     ax,cl                   ; Now shift the remainder.
        retf
_TEXT   ends

_TEXT      segment
_howcpu    proc far
check_8086:
.186
           pushf                           ;save FLAGS
           pop     bx                      ;store FLAGS in BX
           mov     ax, 0fffh               ;clear bits 12-15
           and     ax, bx                  ;  in FLAGS
           push    ax                      ;store new FLAGS calue on stack
           popf                            ;replace current FLAGS value
           pushf                           ;set new flags
           pop     ax                      ;store new flags in AX
           and     ax, 0f000h              ;if bits 12-15 are set, then CPU
           cmp     ax, 0f000h              ;  is an 8086/8088
           je      cpu_type_8086

check_80286:
.286
           or      bx, 0f000h              ;try to set bits 12-15
           push    bx
           popf
           pushf
           pop     ax
           and     ax, 0f000h              ; if bits 12-15 are cleared,
                                           ;       CPU=Intel 286
           jz      cpu_type_286            ; if CPU is intel 286, check
                                           ; for Intel 287 math coprocessor

check_intel386:
.386
           pushfd
           pop     eax                     ;get original EFLAGS
           mov     ecx, eax                ; save original EFLAGS
           xor     eax, 40000h             ;flip AC bit in EFLAGS
           push    eax                     ; save for EFLAGS
           popfd                           ; copy to EFLAGS
           pushfd                          ; push EFLAGS
           pop     eax                     ; get new EFLAGS value
           xor     eax, ecx                ; can't toggle AC bit, CPU=Intel386
           je      cpu_type_386            ; if CPU is Intel386, now check
                                           ; for an Intel 287 or Intel387 MCP

check_Intel486:
.486
           pushfd
           pushfd                          ;push original EFLAGS
           pop     eax                     ; get original EFLAGS in eax
           mov     ecx, eax                ;save original EFLAGS in ecx
           or      eax, 00200000h          ; flip ID bit in EFLAGS
           push    eax                     ;save for EFLAGS
           popfd                           ;copy to EFLAGS
           pushfd                          ;push EFLAGS
           pop     eax                     ;get new EFLAGS value
           popfd
           xor     eax, ecx
           je      cpu_type_486            ;if ID bit cannot be changed,
                                           ;CPU=Intel486 without CPUID
                                           ;instruction functionality

           jmp short cpu_type_586

cpu_type_8086:
           mov ax, 8086h
           jmp short end_get_cpuid
cpu_type_286:
           mov ax, 0286h
           jmp short end_get_cpuid
cpu_type_386:
           mov ax, 0386h
           jmp short end_get_cpuid
cpu_type_486:
           mov ax, 0486h
           jmp short end_get_cpuid
cpu_type_586:
           mov ax, 0586h
           jmp short end_get_cpuid
end_get_cpuid:
.386p
           retf
_howcpu    endp
_TEXT      ends

.286

                   assume cs:_TEXT
_TEXT              segment
YM_register        proc far ; void far pascal YM_register(unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   mov dx, 0388h
                   mov al, byte ptr [bp + 8]
                   out dx, al
                   mov cx, 0006h
YM_reg00@:         in al, dx
                   loop short YM_reg00@
                   inc dx
                   mov al, [bp + 6]
                   out dx, al
                   dec dx
                   mov cx, 35
YM_reg01@:         in al, dx
                   loop short YM_reg01@
                   pop bp
                   retf 2 + 2
YM_register        endp
YM_enablewave      proc far ; void far pascal YM_enablewave(void)
                   push 0001h
                   push 0020h
                   call far ptr _TEXT:YM_register
                   retf
YM_enablewave      endp
YM_disablewave     proc far ; void far pascal YM_disablewave(void)
                   push 0001h
                   push 0000h
                   call far ptr _TEXT:YM_register
                   retf
YM_disablewave     endp
YM_method1         proc far ; void far pascal YM_method1(word, word, word, word, word, word)
                   push bp
                   mov bp, sp
                   mov ax, 0020h
                   add ax, word ptr [bp + 16]
                   push ax
                   mov dx, word ptr [bp + 14]
                   shl dx, 7
                   mov ax, word ptr [bp + 12]
                   shl ax, 6
                   or dx, ax
                   mov ax, word ptr [bp + 10]
                   shl ax, 5
                   or dx, ax
                   mov ax, word ptr [bp + 8]
                   shl ax, 4
                   or ax, dx
                   or ax, word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2 + 2 + 2 + 2
YM_method1         endp
YM_method2         proc far ; void far pascal YM_method2(word, word, word)
                   push bp
                   mov bp, sp
                   mov ax, 0040h
                   add ax, word ptr [bp + 10]
                   push ax
                   mov ax, word ptr [bp + 8]
                   shl ax, 6
                   or ax, word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_method2         endp
YM_method3         proc far ; void far pascal YM_method3(word, word, word)
                   push bp
                   mov bp, sp
                   mov ax, 0060h
                   add ax, word ptr [bp + 10]
                   push ax
                   mov ax, word ptr [bp + 8]
                   shl ax, 4
                   or ax, word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_method3         endp
YM_method4         proc far ; void far pascal YM_method4(word, word, word)
                   push bp
                   mov bp, sp
                   mov ax, 0080h
                   add ax, word ptr [bp + 10]
                   push ax
                   mov ax, word ptr [bp + 8]
                   shl ax, 4
                   or ax, word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_method4         endp
YM_method5         proc far ; void far pascal YM_method5(word, word, word)
                   push bp
                   mov bp, sp
                   push 00bdh
                   mov dx, word ptr [bp + 10]
                   shl dx, 7
                   mov ax, word ptr [bp + 8]
                   shl ax, 6
                   or dx, ax
                   mov ax, word ptr [bp + 6]
                   shl ax, 5
                   or dx, ax
                   mov ax, word ptr [bp + 6]
                   and ax, 001fh
                   or ax, dx
                   push ax                   
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_method5         endp
YM_method6         proc far ; void far pascal YM_method6(word, word, word)
                   push bp
                   mov bp, sp
                   mov ax, 00c0h
                   add ax, word ptr [bp + 10]
                   push ax
                   mov ax, word ptr [bp + 8]
                   shl ax, 1
                   or ax, word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_method6         endp
YM_selectwave      proc far ; void far pascal YM_selectwave(unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   mov ax, 00e0h
                   add ax, word ptr [bp + 8]
                   push ax
                   push word ptr [bp + 6]
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2
YM_selectwave      endp
YM_frequency_on    proc far ; void far pascal YM_frquency_on(unsigned int, unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   mov ax, 00a0h
                   add ax, word ptr [bp + 10]
                   push ax
                   push word ptr [bp + 8]
                   call far ptr _TEXT:YM_register
                   mov ax, 00b0h
                   add ax, word ptr [bp + 10]
                   push ax
                   mov ax, word ptr [bp + 8]
                   shr ax, 8
                   or ax, word ptr [bp + 6]
                   or ax, 0032h
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_frequency_on    endp
YM_frequency_off   proc far ; void far pascal YM_frquency_off(unsigned int, unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   mov ax, 00a0h
                   add ax, word ptr [bp + 10]
                   push ax
                   push word ptr [bp + 8]
                   call far ptr _TEXT:YM_register
                   mov ax, 00b0h
                   add ax, word ptr [bp + 10]
                   push ax
                   mov ax, word ptr [bp + 8]
                   shr ax, 8
                   or ax, word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_frequency_off   endp
YM_iscard          proc far ; unsigned int far pascal YM_iscard(void)
                   push 0004h
                   push 0060h
                   call far ptr _TEXT:YM_register
                   push 0004h
                   push 0080h
                   call far ptr _TEXT:YM_register
                   mov dx, 0388h
                   in al, dx
                   push ax ; ---------- _time[0]
                   push 0002h
                   push 00ffh
                   call far ptr _TEXT:YM_register
                   push 0004h
                   push 0021h
                   call far ptr _TEXT:YM_register
                   mov dx, 0388h
                   mov cx, 201
YM_iscard00@:      in al, dx
                   loop short YM_iscard00@
                   push ax ; ---------- _time[1]
                   push 0004h
                   push 0060h
                   call far ptr _TEXT:YM_register
                   push 0004h
                   push 0080h
                   call far ptr _TEXT:YM_register
                   pop dx ; ----------- _time[1]
                   pop ax ; ----------- _time[0]
                   and dx, 00e0h
                   and ax, 00e0h
                   or ax, ax
                   jnz YM_iscard01@
                   cmp dx, 00c0h
                   jne YM_iscard01@
                   mov ax, 0001h
                   retf
YM_iscard01@:      xor ax, ax
                   retf
YM_iscard          endp
_TEXT              ends

                   assume ds:_DATA
_DATA              segment
YM_instrument      db  0,  0,  0,  0,  1,  1, 15, 15,  1,  5,  3,  0 ; unsigned char YM_instrument[6U][12U]
                   db  0,  0,  1,  0,  0,  0, 18, 13,  1, 14,  3,  0
                   db  0,  0,  1,  1,  1,  0, 15, 11,  7,  7,  0,  0
                   db  0,  0,  0,  1,  1,  1, 20,  3,  2,  2,  2,  0
                   db  0,  0,  1,  1,  2,  1, 22,  3,  2,  2,  2,  0
                   db  0,  0,  1,  1,  1,  0, 28, 15,  2,  2,  2,  0
YM_frequency       dw 345, 365, 387, 410, 435, 460, 488, 517, 547, 580, 615, 651 ; unsigned int YM_frequency[12U]
_DATA              ends

_DATA              segment
_g_segment_text    dw _TEXT
_g_segment_DGROUP  dw DGROUP
_g_segment_heap    dw _HEAP
_g_segment_stack   dw _STACK
_DATA              ends

_STACK             segment
                   db SIZE_STACK dup (?)
_STACK             ends

                   end _start_SUGA

; End of source