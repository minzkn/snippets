; Code by JaeHyuk.CHO

;-----------------------------------------------------------
.model small           ; memory model - small - COM
.code                  ; code segment
.386p                  ; operation 286
;-----------------------------------------------------------

exitkey equ 45h        ; NumLock
;exitkey equ 37h        ; Print Screen
;exitkey equ 46h        ; Ctrl + Break
;exitkey equ 57h        ; F11
           org 100h    ; offset
start:                 ; start offset - 100h
           push offset entry
           ret
keyp       proc interrupt
           jmp short key00    ; 00h
           keyid  dw 1a1bh    ; 02h id
           keyoff dw 6148h    ; 04h org interrupt offset
           keyseg dw 6b63h    ; 06h org interrupt segment
           codes1 dw 7265h    ; 08h Code
           codes2 dw 4449h    ; 0ah PSP Code
           string06 db 0dh, 0ah, 'Usage: NumLock key - Force exit', 0dh, 0ah, '$'
key00:
           push ax
           in al, 60h
           mov ah, al
           mov al, 20h
           out 20h, al
           cmp ah, exitkey
           jnz key01
           mov eax, cr0
           and al, 0feh
           mov cr0, eax
           mov ax, 0003h
           int 10h
           push bx
           xor bx, bx
putslb:
           mov al, cs:[string06+bx+09h]
           cmp al, '$'
           jz endputs
           mov ah, 0eh
           push bx
           mov bx, 0007h
           int 10h
           pop bx
           inc bx
           jmp short putslb
endputs:
           pop bx
           mov ax, 4c1ah
           int 21h
key01:
           pop ax
           pushf
           call dword ptr [keyoff]
           iret
           endp
entry:
           cld
           mov ah, 09h
           lea dx, string00
           int 21h
           mov si, 0080h
           lodsb
           cmp al, 01h
           jbe entryinlb
           xor ch, ch
           mov cl, al
           push offset argvlb
           ret
entryinlb: 
           jmp powerexitlb
argvlb:
           lodsb
           cmp al, '/'
           jz argvlbe
           cmp al, '-'
           jz argvlbe
           loop argvlb
argvlbe:
           lodsb
           cmp al, 'A'
           jbe argvlbe00
           cmp al, 'Z'
           jae argvlbe00
           add al, 20h
argvlbe00:
           cmp al, 'u'
           jz uninstall
           cmp al, 'r'
           jz uninstall
           cmp al, 'b'
           jz rebootlb
           cmp al, 's'
           jz firelb
           cmp al, 'e'
           jz powerexitlb
           cmp al, 'p'
           jz parklb
           cmp al, 'k'
           jz keydelaylb
           jmp entryin
firelb:           
           jmp firestart
powerexitlb:
           jmp powerexit
parklb:
           jmp parkstart
rebootlb:
           mov ax, 0ffffh
           push ax
           xor ax, ax
           push ax
           push ax
           pop es
           mov di, 0472h
           stosw
           retf
keydelaylb:
           mov ax, 0305h
           int 16h
           mov ah, 09h
           lea dx, faststring
           int 21h
           int 20h
           faststring db 0dh, 0ah, 'Setting delay of key -> FAST', 0dh, 0ah, '$'
uninstall:
           mov ax, 3509h
           int 21h
           mov ax, es:[bx+02h]
           cmp ax, word ptr cs:keyid
           jz removed
           mov ah, 09h
           lea dx, string05
           int 21h
           int 20h
removed:
           push ds
           mov dx, es:[bx+04h]
           mov ds, es:[bx+06h]
           mov ax, 2509h
           int 21h
           mov ax, es:[bx+08h]
           mov es, ax
           mov ah, 49h
           int 21h
           mov ax, es:[bx+0ah]
           mov es, ax
           mov ah, 49h
           int 21h
           pop ds
           mov ah, 09h
           lea dx, string04
           int 21h
           int 20h
entryin:
           mov ah, 09h
           lea dx, string03
           int 21h
           int 20h
powerexit:
           mov word ptr cs:codes1, cs
           mov ax, word ptr cs:[002ch]
           mov word ptr cs:codes2, ax
           mov ax, 3509h
           int 21h
           mov word ptr cs:keyoff, bx
           mov word ptr cs:keyseg, es
           mov ax, es:[bx+02h]
           push cs
           pop ds
           cmp ax, word ptr cs:keyid
           jnz entry00
           mov ah, 09h
           lea dx, string01
           int 21h
           int 20h
entry00:
           mov ah, 09h
           lea dx, string06
           int 21h
           lea dx, string02
           int 21h
           mov ax, 2509h
           lea dx, keyp
           int 21h
           lea dx, entry
           int 27h

           string00 db 0dh, 0ah, 'PowerEXIT v3.1 by Cho JaeHyuk [Pager:012-846-6420]  Shareware.', 0dh, 0ah
                    db 'Copyright(c)1997-1998 All right reserved.', 0dh, 0ah
                    db '$'
           string01 db 0dh, 0ah, '! Message: Already installed', 0dh, 0ah, '$'
           string02 db 0dh, 0ah, '! Message: Success installed', 0dh, 0ah, '$'
           string03 db 0dh, 0ah, 'Option) /? or /H :Show this help option', 0dh, 0ah
                    db 09h, '/E', 09h, ' :Terminate PowerEXIT  (DEFAULT)', 0dh, 0ah
                    db 09h, '/U', 09h, ' :Remove PowerEXIT (/R)', 0dh, 0ah
                    db 09h, '/P', 09h, ' :Parking', 0dh, 0ah
                    db 09h, '/S', 09h, ' :Screen saver [FIRE]', 0dh, 0ah
                    db 09h, '/K', 09h, ' :Set delay of key [FAST]', 0dh, 0ah
                    db 09h, '/B', 09h, ' :Reboot [COLD]', 0dh, 0ah
                    db 09h, '$'
           string04 db 0dh, 0ah, '! Message: Removed OK', 0dh, 0ah, '$'
           string05 db 0dh, 0ah, '! Message: Remove error!', 0dh, 0ah, '$'

;===========================================================================
; Screen saver srea --------------------------------------------------------
;===========================================================================
firestart:
           mov ax, cs
           push 0a000h
           pop es
           mov ax, 0013h
           int 10h
           mov dx, 03c8h
           xor al, al
           out dx, al
           inc dx
           xor ah, ah
           xor ch, ch
           mov cl, 64
setrgb00:
           mov al, ah
           out dx, al
           xor al, al
           out dx, al
           out dx, al
           inc ah
           loop setrgb00
           xor ah, ah
           mov cl, 64
setrgb01:
           mov al, 63
           out dx, al
           mov al, ah
           out dx, al
           xor al, al
           out dx, al
           inc ah
           loop setrgb01
           xor ah, ah
           mov cl, 64
setrgb02:
           mov al, 63
           out dx, al
           out dx, al
           mov al, ah
           out dx, al
           inc ah
           loop setrgb02
           mov cl, 64
           mov al, 63
setrgb03:
           out dx, al
           out dx, al
           out dx, al
           loop setrgb03
           cld
main00:
           push si
           push ds
           push es
           pop ds
           mov cx, 32320
           mov si, 32000
           mov di, 32000
fire00:
           mov bx, si
           add bx, 320
           xor al, al
           xlat
           cmp al, 04h
           jg refire
           inc di
           inc si
           push offset fire02
           ret
refire:
           xor dx, dx
           xor ah, ah
           mov bx, si
           add bx, 319
           xor al, al
           xlat
           mov dl, al
           inc bx
           xor al, al
           xlat
           add dx, ax
           inc bx
           xor al, al
           xlat
           add dx, ax
           lodsb
           add ax, dx
           shr ax, 1
           shr ax, 1
           dec al
           stosb
           cmp cx, 320
           jb fire02
           push di
           mov di, cx
           stosb
           pop di
fire02:
           loop fire00
           pop ds
           pop si
           mov cx, 320
           mov di, 64320
random00:
           lodsb
           or al, al
           jz random00
           rol al, cl
           stosb
           loop random00
           mov ah, 0bh
           int 21h
           or al, al
           jnz whilekey0
           jmp main00
whilekey0:
           mov ah, 0bh
           int 21h
           or al, al
           jz whilekey1
           mov ah, 07h
           int 21h
           jmp short whilekey0
whilekey1:
           mov ax, 0003h
           int 10h
           push cs
           pop ds
           mov ah, 09h
           lea dx, string00
           int 21h
           int 20h
;===========================================================================
; Parking srea --------------------------------------------------------
;===========================================================================
           COMPLETESTRING DB 0dh, 0ah, 'Turn off your computer [Parking complete]', 0Dh, 0Ah, '$'
           ERRORSTRING    DB 0dh, 0ah, 'Parking error', 0Dh, 0Ah, '$'
           RESETSTRING    DB 0dh, 0ah, 'Reset disk /Cancel parking/', 0Dh, 0Ah, '$'
           DRIVE       DW 0080h
           PARK        DW 0000h
parkstart:
       MOV AX, 0800h
       XOR CX, CX
       MOV DX, 0080h
       INT 13h
       XOR DH, DH
       jb MESSAGEPUT
       or dx, dx
       Jb MESSAGEPUT
       CMP DX, 0002h
       JLE COUNTJMP
       MOV DX, 0002h
COUNTJMP:
       XOR DH, DH
       MOV CX, DX
PARKLOOP:
       PUSH CX
       MOV AX, 01100h
       XOR CX, CX
       MOV DX, DRIVE
       INT 13h
       jb parkcom
       MOV PARK, 01h
       MOV AX, 0C00h
       XOR CX, CX
       MOV DX, DRIVE
       INT 13h
       MOV AX, 0800h
       XOR CX, CX
       MOV DX, DRIVE
       INT 13h
       MOV AX, 0C00h
       MOV DX, DRIVE
       INT 13h
PARKCOM:
       INC DRIVE
       POP CX
       LOOP PARKLOOP
MESSAGEPUT:
       MOV AH, 09h
       CMP PARK, 0FFh
       JNE OKMESSAGE
       lea dx, errorstring
       push offset intput
       ret
OKMESSAGE:
       lea dx, completestring
INTPUT:
       INT 21h
       MOV AH, 07h
       INT 21h
       MOV AH, 09h
       lea dx, resetstring
       INT 21h
EXITENTRY:
       INT 20h
;-----------------------------------------------------------
           end start
; End of source
