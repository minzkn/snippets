; /****************************
;  *    MINZ kernel overay    *
;  * Copyright(c)1998 by MINZ *
;  *    Code by Cho JaeHyuk   *
;  ****************************/
;  ------- MZIPOVER.ASM -------

.286

                   OGROUP group _TEXT, _OVERLAY

                   assume cs:OGROUP, ds:OGROUP, es:nothing, ss:nothing
_TEXT              segment byte public use16 'CODE'
                   org 0h     
_start@            label near           
                   cli
                   call near ptr $ + 3 + 2
                   jmp short _getip0@
                   mov bp, sp
                   mov bx, word ptr [bp]
                   sub bx, 4
                   retn
_getip0@           label short
                   push cs
                   pop ds
                   lea ax, OGROUP:_run_data@[bx]
                   shr ax, 4
                   mov cx, cs
                   add ax, cx
                   add ax, word ptr OGROUP:_run_data@[bx + 8]
                   mov word ptr OGROUP:_run_seg@[bx], ax

                   mov cx, 16
                   mov ax, cs
                   mul cx
                   lea cx, OGROUP:_run_data@[bx]
                   add ax, cx
                   adc dx, 0
                   add ax, word ptr OGROUP:_run_data@[bx + 24]
                   mov word ptr OGROUP:_run_src@[bx + 0], ax
                   mov word ptr OGROUP:_run_src@[bx + 2], dx

                   mov ax, word ptr OGROUP:_run_seg@[bx]
                   add ax, word ptr OGROUP:_run_data@[bx + 22]
                   mov word ptr OGROUP:_run_jump@[bx + 3], ax
                   mov ax, word ptr OGROUP:_run_data@[bx + 20]
                   mov word ptr OGROUP:_run_jump@[bx + 1], ax
                   mov ax, word ptr OGROUP:_run_seg@[bx]
                   add ax, word ptr OGROUP:_run_data@[bx + 14]
                   mov ss, ax
                   mov sp, word ptr OGROUP:_run_data@[bx + 16]

                   mov cx, word ptr OGROUP:_run_data@[bx + 6]
                   or cx, cx
                   jz short _run_norelocate@
_run_relocate@     label short
                   push cx
                   mov cx, 16
                   mov ax, word ptr OGROUP:_run_src@[bx + 0]
                   mov dx, word ptr OGROUP:_run_src@[bx + 2]
                   mov di, ax
                   and di, 0fh
                   div cx
                   mov es, ax

                   mov ax, word ptr OGROUP:_run_seg@[bx]
                   mul cx
                   push dx
                   push ax
                   mov ax, word ptr es:[di + 2]
                   mul cx
                   add ax, word ptr es:[di]
                   adc dx, 0
                   pop di
                   pop si
                   add ax, di
                   adc dx, si
                   mov di, ax
                   and di, 0fh
                   div cx
                   mov es, ax

                   mov ax, word ptr OGROUP:_run_seg@[bx]
                   add word ptr es:[di], ax

                   add word ptr OGROUP:_run_src@[bx + 0], 4
                   adc word ptr OGROUP:_run_src@[bx + 2], 0
                   pop cx
                   loop short _run_relocate@
_run_norelocate@   label short 
                   push cs
                   pop ds
                   push cs
                   pop es
                   sti 
                   jmp short $ + 2 
_run_jump@         db 0eah      
                   db 'MINZ'
_run_seg@          db ' k'
_run_src@          db 'ernel o'
                   db 'verlay '
                   db 'by Cho JaeHyuk'
_TEXT              ends
_OVERLAY           segment para public use16 'OVERLAY'
_run_data@         label byte      
_OVERLAY           ends
                   end _start@

; End of source
