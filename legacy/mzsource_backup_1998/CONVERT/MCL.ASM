; MCL.ASM

.286 

include            mcseg.inc

extrn              _initmain:far, pascal
extrn              _main    :far, cdecl
extrn              exit     :far, pascal
extrn              memcpy   :far, pascal

public             _heapseg, _argc, _argv, _vector, _psp, _startip, _startcs
public             SCOPY@

                   assume cs:_TEXT, ds:DGROUP, es:nothing, ss:_STACK
_TEXT              segment byte public use16 'CODE'
                   org 0
_start@:           call near ptr $ + 3 + 2
                   jmp short _getip0@
                   mov bp, sp
                   mov ax, word ptr [bp]
                   retn
_getip0@           label short
                   mov bx, DGROUP
                   mov ds, bx
                   sub ax, 3
                   mov word ptr DGROUP:_startip, ax
                   mov ax, cs
                   mov word ptr DGROUP:_startcs, ax

                   mov dx, es
                   xor ax, ax
                   mov ds, ax
                   mov ax, DGROUP
                   mov es, ax
                   xor si, si
                   mov di, offset DGROUP:_vector
                   mov cx, 256 * 4
                   cld
                   rep movsb

                   mov ax, DGROUP
                   mov ds, ax
                   mov es, ax
                   mov ax, _HEAP
                   mov word ptr DGROUP:_heapseg, ax                   
                   mov word ptr DGROUP:_psp, dx
                   call _initmain
                   call _main
                   push ax
                   call exit
SCOPY@             proc far ; void far pascal SCOPY@(void far *, void far *) , CX=LENGTH
                   push bp
                   mov bp, sp
                   push word ptr [bp + 12]
                   push word ptr [bp + 10]
                   push word ptr [bp + 8]
                   push word ptr [bp + 6]
                   push cx
                   call memcpy
                   pop bp
                   retf 4 + 4 
SCOPY@             endp
_TEXT              ends

                   assume ds:DGROUP
_DATA              segment byte public use16 'DATA'
_DATA              ends

                   assume ds:DGROUP
_BSS               segment byte public use16 'BSS'
_vector            dd 256 dup (?)
_heapseg           dw ? ; unsigned int _heapseg
_argc              dw ? ; unsigned int _argc
_argv              dd 256 dup (?) ; void far **_argv
_psp               dw ?
_startip           dw ?
_startcs           dw ?
_BSS               ends

                   assume ss:_STACK
_STACK             segment byte stack use16 'STACK'
                   db 8000h dup (?)
_STACK             ends
                   end _start@

; /* End of source */
