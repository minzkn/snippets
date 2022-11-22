; /***************************************
;  *         Copyright(c)1998-1998       * 
;  *        MC Compiler source (MCL)     *
;  *         Code by Cho JaeHyuk         *
;  *          MINZ (02)822-2744          *
;  ***************************************/

; MCL.ASM

.286 

include            mcseg.inc

extrn              _initmain:far, pascal
extrn              main     :far, pascal
extrn              exit     :far, pascal
extrn              memcpy   :far, pascal

public             _heapseg, _argc, _argv, _vector
public             SCOPY@

                   assume cs:_TEXT, ds:DGROUP, es:nothing, ss:_STACK
_TEXT              segment byte public use16 'CODE'
                   org 0
_start@:           xor ax, ax
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
                   call _initmain
                   call main
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
_argv              dd ? ; void far **_argv
_BSS               ends

                   assume ss:_STACK
_STACK             segment byte stack use16 'STACK'
                   db 8000h dup (?)
_STACK             ends
                   end _start@

; /* End of source */
