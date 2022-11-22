; ***********************************
; *      Operating system MINZ      *
; *            Since 1997           *
; *          By Cho JaeHyuk         *
; *       Source: MZBIN.ASM         *
; ***********************************

; Coding(write)     - 1997/../.. [xxx]
; Optimize(Rewrite) - 1998/04/08 [WED]

;------------------------------------------
; COMPILE!      - C:\> TASM.EXE MZBIN.ASM
; LINK!         - C:\> TLINK /T MZBIN.OBJ
;------------------------------------------

BUFFERSIZE EQU 512 * 9  ; Sector for floppy more faster

.386p
.model tiny

           MZGROUP group MZCODE, MZDATA
           assume cs:MZGROUP, ds:MZGROUP
MZCODE     segment para public use16 'CODE'
           org 100h
           ;org 0
StartMZ:   push cs
           pop ds
           mov ax, 3
           int 10h
           mov si, offset CopyrightStr
           call BIOSPuts ; Set to AL=0
           mov ah, 3dh
           xor cx, cx
           mov dx, offset KernelName
           int 21h
           jb LoadReturn
           mov bx, ax
           mov dx, offset BinaryArea
ReadLoop:  mov ax, 3f00h ; !
           mov cx, BUFFERSIZE
           int 21h
           or ax, ax
           jz short EndOfLoad
           add dx, BUFFERSIZE
           loop short ReadLoop
EndOfLoad: mov ah, 3eh ; Seted
           int 21h
           mov ax, cs
           cwde
           shl eax, 4
           mov bx, offset BinaryArea
           movzx ebx, bx
           add eax, ebx
           shr eax, 4
           push ax
           push 0
           xor ax, ax
           xor bx, bx
           xor cx, cx
           xor dx, dx
           xor di, di
           xor si, si 
           retf
LoadReturn:call BIOSPuts
           int 20h
BIOSPuts   proc near
           mov ah, 14
           mov bx, 7
BIOSPuts00:lodsb
           or al, al
           jz BIOSPuts01
           int 10h
           jmp short BIOSPuts00
BIOSPuts01:ret
BIOSPuts   endp
           CopyrightStr      db 'Phoenix NoteBIOS 4.05 08CS'
                             db 0dh, 0ah, 'Copyright 1985-1995 Phoenix Technologies Ltd., All right Reserved.'
                             db 0dh, 0ah, 0ah, 'CPU = Pentium 75 MHz'
                             db 0dh, 0ah, '00000640K System RAM Passed'
                             db 0dh, 0ah, '00007168K Extended RAM Passed'
                             db 0dh, 0ah, 'System BIOS shadowed'
                             db 0dh, 0ah, 'Video BIOS shadowed'
                             db 0dh, 0ah, 'Mouse initialized'
                             db 0dh, 0ah, 0ah
                             db 0
           EndStr            db 'Warnning:Found not kernel(MZKERNEL.BIN)', 0dh, 0ah, 0
           KernelName        db 'KERNEL.BIN', 0
MZCODE     ends

MZDATA     segment para public use16 'CODE'
           BinaryArea        label near
MZDATA     ends
           end StartMZ

; End of source
