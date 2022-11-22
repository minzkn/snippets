; Code by Cho JaeHyuk

.286
.model tiny
.code
           org 100h
StartMZSYS:cld
           mov si, 80h
           lodsb
           cbw
           mov cx, ax
           mov ax, cs
           mov es, ax
Re_Space:  lodsb
           dec cx
           cmp al, 20h
           jz short Re_Space
           dec si
           inc cx

           lea di, Argument
           repz movsb
           mov ax, 2400h
           stosw

           mov ax, cs
           mov ds, ax
           mov es, ax               ; Init seg

           lea di, BootSys
           mov cx, 512 / 2
           xor ax, ax
           stosw                    ; Clear BootSys

           mov ah, 09h
           lea dx, CopyRight
           int 21h                  ; Print right

           lea bx, Argument
           cmp byte ptr [bx+1], ':'
           jnz short ErrorLabel
           cmp byte ptr [bx+2], 00h
           jnz short ErrorLabel


           cmp byte ptr [bx], 'a'
           jl short UP_char
           sub byte ptr [bx], 20h
UP_char:   mov al, [bx]
           sub al, 'A'
           mov DiskHead, 0
           cmp al, 2
           jl short NOT_hard
           add al, 80h-2h
           mov DiskHead, 1
NOT_hard:  mov Drive, al
           
           mov ax, 3d00h            ; open
           xor bx, bx
           xor cx, cx
           lea dx, SystemF
           int 21h
           jc short ErrorLabel
           mov bx, ax
           mov ax, 3f00h            ; read
           mov cx, 512
           lea dx, BootSys
           int 21h
           jc short ErrorLabel
           mov ax, 3e00h            ; close
           int 21h
           jc short ErrorLabel

           mov cx, 16
ReCountRD: push cx
           mov ax, 0201h
           lea bx, DiskSys
           mov cx, 0001h
           mov dh, DiskHead
           mov dl, Drive
           int 13h
           jnc NoErrorRD
           xor ax, ax
           mov dl, Drive
           int 13h
           pop cx
           loop short ReCountRD
NoErrorRD: pop cx       

           jmp short JumpErr
ErrorLabel:mov ah, 09h
           lea dx, Usage
           int 21h
           mov al, 0ffh
           jmp near ptr ExitLabel   ; Short label error
JumpErr:   

; Modifier sector ----------------------------------------
           lea di, BootSys
           add di, 512 - 2
           mov ax, 0aa55h  
           stosw                    ; End of FAT system

           lea si, DiskSys + 3 + 8  ; ~~~
           lea di, BootSys + 3 + 8  ; NOT COPY OEM
           mov cx, (3eh + 9) - (3 + 8)        ; Info grid
           repz movsb               ; Copy disk-info
; --------------------------------------------------------

           mov cx, 16
ReCountRS: push cx
           xor ax, ax
           mov dl, Drive
           int 13h
           jnc short NoErrorRS
           pop cx
           loop short ReCountRS
NoErrorRS: pop cx           
           
           mov cx, 16
ReCountWS: push cx
           push cs
           pop ds
           mov ax, 0301h            ; write sector
           mov cx, 0001h
           mov dh, DiskHead
           mov dl, Drive
           push cs
           pop es
           lea bx, BootSys
           int 13h
           jnc NoErrorWS
           xor ax, ax
           mov dl, Drive
           int 13h
           pop cx
           loop short ReCountWS
           jmp short ErrorLabel
NoErrorWS: pop cx

           
           mov ah, 09h
           lea dx, Success
           int 21h        

           mov al, Drive
           cmp al, 80h
           jb short NOT_hard_disk
           sub al, 80h-2h
NOT_hard_disk:
           add Trans, al
           add SystemKD, al
           mov ah, 09h
           lea dx, Trans
           int 21h

; Kernel copy ---------------------------------------------------
           mov ax, 4100h
           mov cx, 0200h
           lea dx, SystemKD
           int 21h

           mov ax, 3d00h
           xor cx, cx
           lea dx, SystemK
           int 21h               ; open "rb"
           jb short CopyError
           mov Source, ax

           mov ax, 3c00h
           mov cx, 0006h         ; System attr
           lea dx, SystemKD
           int 21h               ; open "wb"
           jb short CopyError    
           mov Target, ax        

Re_load:
           mov ax, 3f00h
           mov bx, Source
           mov cx, 512
           lea dx, DiskSys
           int 21h               ; read
           jb short CopyError

           push ax

           mov cx, ax
           mov ax, 4000h
           mov bx, Target
           lea dx, DiskSys
           int 21h               ; write
           jb short CopyError

           pop ax
           cmp ax, 512
           jz short Re_load

           mov ax, 3e00h
           mov bx, Source
           int 21h
           jb short CopyError
           mov ax, 3e00h
           mov bx, Target
           int 21h               ; close
           jb short CopyError

           jmp short NoCopyError
CopyError: mov ah, 09h
           lea dx, CopyStr00
           int 21h
           jmp near ptr ExitLabel
           CopyStr00 db 'ERROR: System file not copy!', 0dh, 0ah, '$'
NoCopyError:
; ---------------------------------------------------------------

           xor al, al
ExitLabel: mov ah, 4ch
           int 21h

           CopyRight db 'Copies MINZ system v1.01  Copyright(c)1998 by JaeHyuk', 0dh, 0ah, '$'
           Usage     db 'Usage: MZSYS.COM <drive:>', 0dh, 0ah, '$'
           Success   db 0dh, 0ah, 'System transferred to ', '$'
           Trans     db 'A:', 0dh, 0ah, '$'
           SystemF   db 'MZBOOT.BIN', 00h
           SystemKD  db 'A:\KERNEL.SYS', 00h
           SystemK   db 'KERNEL.BIN', 00h
           Source    dw ?
           Target    dw ?
           Drive     db ?
           Argument  dw 128 dup (?)
           BootSys   db 512 dup (?)
           DiskSys   db 512 dup (?)
           DiskHead  db ?
           end StartMZSYS

; Back-up code ====================================================
           
           shr al, 4
           add al, '0'
           mov ah, 0eh
           mov bx, 7
           int 10h
           mov al, Drive
           and al, 0fh
           add al, '0'
           mov ah, 0eh
           int 10h

; End of source
