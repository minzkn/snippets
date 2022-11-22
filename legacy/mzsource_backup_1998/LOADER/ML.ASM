; /***************************************
;  *         Copyright(c)1998-1998       * 
;  *        MC Compiler source (ML)      * 
;  *         Code by Cho JaeHyuk         * 
;  *          MINZ (02)822-2744          * 
;  ***************************************/

; ML.ASM

.286

include            mcseg.inc

extrn              _heapseg : word
extrn              _vector  : word

public             _num
public             nowork
public             _nointwork
public             _zerovector
public             exit
public             abort
public             putc
public             puts
public             memset
public             memcpy
public             memchr
public             memcmp
public             swap
public             strclen
public             strlen
public             strncpy
public             strcpy
public             strncat
public             strcat
public             strnchr
public             strchr
public             strcmp
public             toupper
public             tolower
public             isdigit
public             isalpha
public             strupr
public             strlwr
public             tonum
public             peekb
public             peekw
public             pokeb
public             pokew
public             getvect
public             setvect
public             inportb
public             inportw
public             outportb
public             outportw
public             rand
public             delay
public             sound
public             nosound
public             beep
public             inx
public             outx
public             kbhit
public             getch
public             coldboot
public             warmboot
public             biosreset
public             geninterrupt
public             setvideomode
public             setdelaykey
public             inx_test
public             outx_test
public             farjump
public             farcall
public             disk_reset
public             disk_read
public             disk_write
public             enable
public             disable

                   assume cs:_TEXT
_TEXT              segment byte public use16 'CODE'
nowork             proc far ; void far pascal nowork(void)
                   retf
nowork             endp
_nointwork         proc 
                   iret
_nointwork         endp
_zerovector        proc
                   push cs
                   push offset _zero00@
                   call far ptr _TEXT:puts
                   iret
_zero00@           db 0dh, 0ah, 0feh, ' Divide error #', 0dh, 0ah, 0
_zerovector        endp
exit               proc far ; void far pascal exit(unsigned int)
                   push bp
                   mov bp, sp
                   cli
                   push 0
                   push 0
                   push ds
                   push offset _vector
                   push 256 * 4
                   call far ptr _TEXT:memcpy
                   sti
                   mov al, byte ptr [bp + 6]
                   mov ah, 04ch
                   pop bp
                   int 21h
                   jmp short $
exit               endp
abort              proc far ; void far pascal abort(void)
                   push cs
                   push offset _TEXT:abort00@
                   call far ptr _TEXT:puts
                   push 0003h
                   call far ptr _TEXT:exit
abort00@           db 0dh, 0ah, 0feh, ' Abnormal program terminator #', 0
abort              endp
putc               proc far ; unsigned int far pascal putc(unsigned int)
                   push bp
                   mov bp, sp
                   mov ah, 0eh
                   mov al, byte ptr [bp + 6]
                   mov bx, 0007h
                   int 10h
                   mov ax, word ptr [bp + 6]
                   pop bp
                   retf 2
putc               endp
puts               proc far ; void far * far pascal puts(void far *)
                   push bp
                   mov bp, sp
                   push ds
                   push si
                   lds si, dword ptr [bp + 6]
                   mov ah, 0eh
                   mov bx, 0007h
                   cld
puts00@:           lodsb
                   or al, al
                   jz short puts01@
                   int 10h
                   jmp short puts00@
puts01@:           mov ax, word ptr [bp + 6]
                   mov dx, word ptr [bp + 8] 
                   pop si
                   pop ds
                   pop bp
                   retf 4
puts               endp
memset             proc far ; void far * far pascal memset(void far *, unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   push es
                   push di
                   les di, dword ptr [bp + 10]
                   mov cx, word ptr [bp + 6]
                   mov al, byte ptr [bp + 8]
                   cld
                   rep stosb
                   mov ax, word ptr [bp + 10]
                   mov dx, word ptr [bp + 12]
                   pop di
                   pop es
                   pop bp
                   retf 4 + 2 + 2
memset             endp
memcpy             proc far ; void far * far pascal memcpy(void far *, void far *, unsigned int)
                   push bp
                   mov bp, sp
                   push es
                   push di
                   push ds
                   push si
                   les di, dword ptr [bp + 12]
                   lds si, dword ptr [bp + 8]
                   mov cx, word ptr [bp + 6]
                   cld
                   rep movsb
                   mov ax, word ptr [bp + 12]
                   mov dx, word ptr [bp + 14]
                   pop si
                   pop ds
                   pop di
                   pop es
                   pop bp
                   retf 4 + 4 + 2
memcpy             endp
memchr             proc far ; void far * far pascal memchr(void far *, unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   push es
                   push di
                   les di, dword ptr [bp + 10]
                   mov cx, word ptr [bp + 6]
                   mov al, byte ptr [bp + 8]
                   cld
                   repnz scasb
                   jnz short memchr00@
                   mov ax, di
                   dec ax
                   mov dx, es
                   jmp short memchr01@
memchr00@:         xor ax, ax
                   xor dx, dx
memchr01@:         pop di
                   pop es
                   pop bp
                   retf 4 + 2 + 2
memchr             endp
memcmp             proc far ; unsigned int far pascal memcmp(void far *, void far *, unsigned int)
                   push bp
                   mov bp, sp
                   push es
                   push di
                   push ds
                   push si
                   les di, dword ptr [bp + 12]
                   lds si, dword ptr [bp + 8]
                   mov cx, word ptr [bp + 6]
                   cld
                   rep cmpsb
                   jne short memcmp00@
                   mov ax, 0001h
                   jmp short memcmp01@
memcmp00@:         xor ax, ax
memcmp01@:         pop si
                   pop ds
                   pop di
                   pop es
                   pop bp
                   retf 4 + 4 + 2
memcmp             endp
swap               proc far ; void far pascal swap(void far *, void far *)
                   push bp
                   mov bp, sp
                   push ds
                   lds bx, dword ptr [bp + 10]
                   mov al, byte ptr [bx]
                   lds bx, dword ptr [bp + 6]
                   mov ah, byte ptr [bx]
                   mov byte ptr [bx], al
                   lds bx, dword ptr [bp + 10]
                   mov byte ptr [bx], ah
                   pop ds
                   pop bp
                   retf 4 + 4
swap               endp
strclen            proc far ; unsigned int far pascal strclen(void far *, unsigned int)
                   push bp
                   mov bp, sp
                   push ds
                   push si
                   xor cx, cx
                   mov dl, byte ptr [bp + 6]
                   lds si, dword ptr [bp + 8]
                   cld
strclen00@:        lodsb
                   cmp al, dl
                   je short strclen01@
                   inc cx
                   jmp short strclen00@
strclen01@:        mov ax, cx
                   pop si
                   pop ds
                   pop bp
                   retf 4 + 2
strclen            endp
strlen             proc far ; unsigned int far pascal strlen(void far *)
                   push bp
                   mov bp, sp
                   push word ptr [bp + 8]
                   push word ptr [bp + 6]
                   push 0000h
                   call _TEXT:strclen
                   pop bp
                   retf 4
strlen             endp
strncpy            proc far ; void far * far pascal strncpy(void far *, void far *, unsigned int)
                   push bp
                   mov bp, sp
                   push word ptr [bp + 14]
                   push word ptr [bp + 12]
                   push word ptr [bp + 10]
                   push word ptr [bp + 8]
                   push word ptr [bp + 6]
                   call _TEXT:memcpy
                   push ds
                   lds bx, dword ptr [bp + 12]
                   add bx, word ptr [bp + 6]
                   mov byte ptr [bx], 00h
                   pop ds
                   pop bp
                   retf 4 + 4 + 2
strncpy            endp
strcpy             proc far ; void far * far pascal strcpy(void far *, void far *)
                   push bp
                   mov bp, sp
                   push word ptr [bp + 8]
                   push word ptr [bp + 6]
                   call _TEXT:strlen
                   push word ptr [bp + 12]
                   push word ptr [bp + 10]
                   push word ptr [bp + 8]
                   push word ptr [bp + 6]
                   push ax
                   call _TEXT:strncpy
                   pop bp
                   retf 4 + 4
strcpy             endp
strncat            proc far ; void far * far pascal strncat(void far *, void far *, unsigned int)
                   push bp
                   mov bp, sp
                   push word ptr [bp + 14]
                   push word ptr [bp + 12]
                   push word ptr [bp + 14]
                   push word ptr [bp + 12]
                   call _TEXT:strlen
                   add ax, word ptr [bp + 12]
                   push word ptr [bp + 14]
                   push ax
                   push word ptr [bp + 10]
                   push word ptr [bp + 8]
                   push word ptr [bp + 6]
                   call _TEXT:strncpy
                   pop ax
                   pop dx
                   pop bp
                   retf 4 + 4 + 2
strncat            endp
strcat             proc far ; void far * far pascal strcat(void far *, void far *)
                   push bp
                   mov bp, sp
                   push word ptr [bp + 8]
                   push word ptr [bp + 6]
                   call _TEXT:strlen
                   push word ptr [bp + 12]
                   push word ptr [bp + 10]
                   push word ptr [bp + 8]
                   push word ptr [bp + 6]
                   push ax
                   call _TEXT:strncat
                   pop bp
                   retf 4 + 4
strcat             endp
strnchr            proc far ; void far * far pascal strnchr(void far *, unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   push word ptr [bp + 12]
                   push word ptr [bp + 10]
                   push word ptr [bp + 8]
                   push word ptr [bp + 6]
                   call _TEXT:memchr
                   pop bp
                   retf 4 + 2 + 2
strnchr            endp
strchr             proc far ; void far * far pascal strchr(void far *, unsigned int)
                   push bp
                   mov bp, sp
                   push word ptr [bp + 10]
                   push word ptr [bp + 8]
                   call _TEXT:strlen
                   push word ptr [bp + 10]
                   push word ptr [bp + 8]
                   push word ptr [bp + 6]
                   push word ptr ax
                   call _TEXT:strnchr
                   pop bp
                   retf 4 + 2
strchr             endp
strcmp             proc far ; unsigned int far pascal strcmp(void far *, void far *)
                   push bp
                   mov bp, sp
                   push word ptr [bp + 8]
                   push word ptr [bp + 6]
                   call far ptr _TEXT:strlen
                   inc ax
                   push word ptr [bp + 12]
                   push word ptr [bp + 10]
                   push word ptr [bp + 8]
                   push word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:memcmp
                   mov sp, bp
                   pop bp
                   retf 4 + 4
strcmp             endp
toupper            proc far ; unsigned int far pascal toupper(unsigned int)
                   push bp
                   mov bp, sp
                   mov ax, word ptr [bp + 6]
                   cmp al, 'a'
                   jb short toupper00@
                   cmp al, 'z'
                   ja short toupper00@
                   sub al, 'a'-'A'
toupper00@:        pop bp
                   retf 2
toupper            endp
tolower            proc far ; unsigned int far pascal tolower(unsigned int)
                   push bp
                   mov bp, sp
                   mov ax, word ptr [bp + 6]
                   cmp al, 'A'
                   jb short tolower00@
                   cmp al, 'Z'
                   ja short tolower00@
                   add al, 'a'-'A'
tolower00@:        pop bp
                   retf 2
tolower            endp
isdigit            proc far ; unsigned int far pascal isdigit(unsigned int)
                   push bp
                   mov bp, sp
                   cmp byte ptr [bp + 6], '0'
                   jb short isdigit00@
                   cmp byte ptr [bp + 6], '9'
                   ja short isdigit00@
                   mov ax, 0001h
                   jmp short isdigit01@
isdigit00@:        xor ax, ax
isdigit01@:        pop bp
                   retf 2
isdigit            endp
isalpha            proc far ; unsigned int far pascal isalpha(unsigned int)
                   push bp
                   mov bp, sp
                   push word ptr [bp + 6]
                   call _TEXT:toupper
                   cmp al, 'A'
                   jb short isalpha00@
                   cmp al, 'Z'
                   ja short isalpha01@
                   mov ax, 0001h
                   jmp short isalpha01@
isalpha00@:        xor ax, ax
isalpha01@:        pop bp
                   retf 2
isalpha            endp
strupr             proc far ; void far * far pascal strupr(void far *)
                   push bp
                   mov bp, sp
                   push es
                   push di
                   push ds
                   push si
                   les di, dword ptr [bp + 6]
                   lds si, dword ptr [bp + 6]
                   cld
strupr00@:         lodsb
                   or al, al
                   jz strupr01@
                   push ax
                   call _TEXT:toupper
                   stosb
                   jmp short strupr00@
strupr01@:         mov ax, word ptr [bp + 6]
                   mov dx, word ptr [bp + 8]
                   pop si
                   pop ds
                   pop di
                   pop es
                   pop bp
                   retf 4
strupr             endp
strlwr             proc far ; void far * far pascal strlwr(void far *)
                   push bp
                   mov bp, sp
                   push es
                   push di
                   push ds
                   push si
                   les di, dword ptr [bp + 6]
                   lds si, dword ptr [bp + 6]
                   cld
strlwr00@:         lodsb
                   or al, al
                   jz strlwr01@
                   push ax
                   call _TEXT:tolower
                   stosb
                   jmp short strlwr00@
strlwr01@:         mov ax, word ptr [bp + 6]
                   mov dx, word ptr [bp + 8]
                   pop si
                   pop ds
                   pop di
                   pop es
                   pop bp
                   retf 4
strlwr             endp
tonum              proc far ; unsigned int far pascal tonum(unsigned int)
                   push bp
                   mov bp, sp
                   push word ptr [bp + 6]
                   call _TEXT:isdigit
                   or al, al
                   jz short tonum00@
                   mov al, byte ptr [bp + 6]
                   sub al, '0'
                   jmp short tonum01@
tonum00@:          push word ptr [bp + 6]
                   call _TEXT:tolower
                   sub al, 'a' - 0ah
tonum01@:          pop bp
                   retf 2
tonum              endp
peekb              proc far ; unsigned int far pascal peekb(unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, word ptr [bp + 8]
                   mov ds, ax
                   mov bx, word ptr [bp + 6]
                   mov al, byte ptr ds:[bx]
                   xor ah, ah
                   pop ds
                   pop bp
                   retf 2 + 2
peekb              endp
peekw              proc far ; unsigned int far pascal peekw(unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, word ptr [bp + 8]
                   mov ds, ax
                   mov bx, word ptr [bp + 6]
                   mov ax, word ptr ds:[bx]
                   pop ds
                   pop bp
                   retf 2 + 2
peekw              endp
pokeb              proc far ; unsigned int far pascal pokeb(unsigned int, unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, word ptr [bp + 10]
                   mov ds, ax
                   mov bx, word ptr [bp + 8]
                   mov al, byte ptr [bp + 6]
                   mov byte ptr ds:[bx], al
                   xor ah, ah
                   pop ds
                   pop bp
                   retf 2 + 2 + 2
pokeb              endp
pokew              proc far ; unsigned int far pascal pokew(unsigned int, unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, word ptr [bp + 10]
                   mov ds, ax
                   mov bx, word ptr [bp + 8]
                   mov ax, word ptr [bp + 6]
                   mov word ptr ds:[bx], ax
                   pop ds
                   pop bp
                   retf 2 + 2 + 2
pokew              endp
getvect            proc far ; void far * far pascal getvect(unsigned int)
                   push bp
                   mov bp, sp
                   push ds
                   xor ax, ax
                   mov ds, ax
                   mov bx, word ptr [bp + 6]
                   shl bx, 2
                   mov ax, word ptr ds:[bx]
                   mov dx, word ptr ds:[bx + 2]
                   pop ds
                   pop bp
                   retf 2
getvect            endp
setvect            proc far ; unsigned int far pascal setvect(unsigned int, void far *)
                   push bp
                   mov bp, sp
                   push ds
                   xor ax, ax
                   mov ds, ax
                   mov bx, word ptr [bp + 10]
                   shl bx, 2
                   mov ax, word ptr [bp + 6]
                   cli
                   mov word ptr ds:[bx], ax
                   mov ax, word ptr [bp + 8]
                   mov word ptr ds:[bx + 2], ax
                   sti
                   mov ax, word ptr [bp + 10]
                   pop ds
                   pop bp
                   retf 2 + 4
setvect            endp
inportb            proc far ; unsigned int far pascal inportb(unsigned int)
                   push bp
                   mov bp, sp
                   mov dx, word ptr [bp + 6]
                   in al, dx
                   xor ah, ah
                   pop bp
                   retf 2
inportb            endp
inportw            proc far ; unsigned int far pascal inportw(unsigned int)
                   push bp
                   mov bp, sp
                   mov dx, word ptr [bp + 6]
                   in ax, dx
                   pop bp
                   retf 2
inportw            endp
outportb           proc far ; unsigned int far pascal outportb(unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   mov dx, word ptr [bp + 8]
                   mov al, byte ptr [bp + 6]
                   out dx, al
                   mov ax, dx
                   pop bp
                   retf 2 + 2
outportb           endp
outportw           proc far ; unsigned int far pascal outportw(unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   mov dx, word ptr [bp + 8]
                   mov ax, word ptr [bp + 6]
                   out dx, ax
                   mov ax, dx
                   pop bp
                   retf 2 + 2
outportw           endp
rand               proc far ; unsigned int far pascal rand(void)
                   mov ax, word ptr _TEXT:rand_val@
                   mul ax
                   mov bx, ax
                   in al, 40h
                   xor ah, ah
                   add ax, bx
                   mov word ptr _TEXT:rand_val@, ax
                   retf
rand_val@          dw 0a5a5h
rand               endp
delay              proc far ; void far pascal delay(unsigned int)
                   push bp
                   mov bp, sp                   
                   mov cx, word ptr [bp + 6]
                   xor ax, ax
delay00@:          push cx
                   mov cx, 182
delay01@:          in al, 40h
                   cmp al, ah
                   je short delay01@
                   mov ah, al
                   loop short delay01@
                   pop cx
                   loop short delay00@
                   pop bp
                   retf 2
delay              endp
sound              proc far ; void far pascal sound(void)
                   mov al, 0b6h
                   out 043h, al
                   mov al, 0ffh
                   out 042h, al
                   mov al, 00fh
                   out 042h, al
                   mov al, 003h
                   out 061h, al
                   retf
sound              endp
nosound            proc far ; void far pascal nosound(void)
                   xor al, al
                   out 061h, al
                   retf
nosound            endp
beep               proc far ; void far pascal beep(unsigned int)
                   push bp
                   mov bp, sp
                   call _TEXT:sound
                   push word ptr [bp + 6]
                   call _TEXT:delay
                   call _TEXT:nosound
                   pop bp
                   retf 2
beep               endp
inx                proc far ; unsigned int far pascal inx(unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   mov dx, word ptr [bp + 8]
                   mov al, byte ptr [bp + 6]
                   out dx, al
                   inc dx
                   in al, dx
                   xor ah, ah
                   pop bp
                   retf 2 + 2
inx                endp
outx               proc far ; void far pascal outx(unsigned int, unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   mov dx, word ptr [bp + 10]
                   mov al, byte ptr [bp + 8]
                   mov ah, byte ptr [bp + 6]
                   out dx, ax
                   pop bp
                   retf 2 + 2 + 2
outx               endp
kbhit              proc far ; unsigned int far pascal kbhit(void)
                   mov ah, 01h
                   int 16h
                   jnz short kbhit00@
                   xor ax, ax
                   retf
kbhit00@:          mov ax, 0001h
                   retf
kbhit              endp
getch              proc far ; unsigned int far pascal getch(void)
                   xor ah, ah
                   int 16h
                   retf
getch              endp
coldboot           proc far ; void far pascal coldboot(void)
                   db 0eah
                   dw 0ffffh, 0000h
coldboot           endp
warmboot           proc far ; void far pascal warmboot(void)
                   int 19h
                   call _TEXT:coldboot
warmboot           endp
biosreset          proc far ; void far pascal biosreset(void)
                   int 18h
                   call _TEXT:coldboot
biosreset          endp
geninterrupt       proc far ; void far pascal geninterrupt(unsigned int)
                   push bp
                   mov bp, sp
                   push ax
                   mov al, byte ptr [bp + 6]
                   mov byte ptr _TEXT:__genint00@, al
                   pop ax
                   jmp short $ + 2 ; CACHE CLEAR
                   db 0cdh         ; INT xxh
__genint00@        db 00h                   
                   pop bp
                   retf 2
geninterrupt       endp
setvideomode       proc far ; void far pascal setvideomode(unsigned int)
                   push bp
                   mov bp, sp
                   mov ax, word ptr [bp + 6]
                   ; xor ah, ah
                   int 10h
                   pop bp
                   retf 2
setvideomode       endp
setdelaykey        proc far ;void far pascal setdelaykey(unsigned int)
                   push bp
                   mov bp, sp
                   mov ax, 0305h
                   mov bx, word ptr [bp + 6]
                   int 16h
                   pop bp
                   retf 2
setdelaykey        endp
inx_test           proc far ; unsigned int far pascal inx_test(unsigned int, unsigned int, unsigned int)
                   push word ptr [bp + 10]
                   push word ptr [bp + 8]
                   call _TEXT:inx
                   and ax, word ptr [bp + 6]
                   retf 2 + 2 + 2 
inx_test           endp
outx_test          proc far ; void far pascal outx_test(unsigned int, unsigned int, unsigned int, unsigned int)
                   mov dx, word ptr [bp + 12]
                   mov al, byte ptr [bp + 10]
                   out dx, al
                   inc dx
                   in al, dx
                   mov ah, byte ptr [bp + 8]
                   and ah, byte ptr [bp + 6]
                   or al, ah
                   out dx, al
                   retf 2 + 2 + 2 + 2
outx_test          endp
farjump            proc far ; void far pascal farjump(unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   push ax
                   mov ax, word ptr [bp + 8]
                   mov word ptr _TEXT:farjump00@ + 2, ax
                   mov ax, word ptr [bp + 6]
                   mov word ptr _TEXT:farjump00@, ax
                   pop ax
                   pop bp
                   jmp short $ + 2
                   db 0eah
farjump00@         dw 0000h, 0000h
farjump            endp
farcall            proc far ; void far pascal farcall(unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   push ax
                   mov ax, word ptr [bp + 8]
                   mov word ptr _TEXT:farcall00@ + 2, ax
                   mov ax, word ptr [bp + 6]
                   mov word ptr _TEXT:farcall00@, ax
                   pop ax
                   jmp short $ + 2
                   db 09ah
farcall00@         dw 0000h, 0000h
                   pop bp
                   retf
farcall            endp
disk_reset         proc far ; unsigned int far pascal disk_reset(unsigned int)
                   push bp
                   mov bp, sp
                   xor ax, ax
                   mov dl, byte ptr [bp + 6]
                   int 13h
                   jnc disk_reset0@
                   mov al, ah
                   xor ah, ah
                   jmp short disk_reset1@
disk_reset0@:      xor ax, ax
disk_reset1@:      pop bp
                   retf 2
disk_reset         endp
disk_read          proc far ; unsigned int far pascal disk_read(unsigned int, unsigned int, unsigned int, unsigned int, void far *)
                   push bp
                   mov bp, sp
                   mov cx, word ptr [bp + 14]
                   shr ch, 6
                   or ch, byte ptr [bp + 10]
                   xchg ch, cl
                   mov dh, byte ptr [bp + 12]
                   mov dl, byte ptr [bp + 16]
                   push es
                   les bx, dword ptr [bp + 6]
                   mov ax, 0201h
                   int 13h
                   jnc disk_read0@
                   mov al, ah
                   xor ah, ah
                   jmp short disk_read1@
disk_read0@:       xor ax, ax
disk_read1@:       pop es
                   pop bp
                   retf 2 + 2 + 2 + 2 + 4
disk_read          endp
disk_write         proc far ; unsigned int far pascal disk_write(unsigned int, unsigned int, unsigned int, unsigned int, void far *)
                   push bp
                   mov bp, sp
                   mov cx, word ptr [bp + 14]
                   shr ch, 6
                   or ch, byte ptr [bp + 10]
                   xchg ch, cl
                   mov dh, byte ptr [bp + 12]
                   mov dl, byte ptr [bp + 16]
                   push es
                   les bx, dword ptr [bp + 6]
                   mov ax, 0301h
                   int 13h
                   jnc disk_write1@
                   mov al, ah
                   xor ah, ah
                   jmp short disk_write1@
disk_write0@:      xor ax, ax
disk_write1@:      pop es
                   pop bp
                   retf 2 + 2 + 2 + 2 + 4
disk_write         endp
enable             proc far ; void far pascal enable(void)
                   sti
                   retf
enable             endp
disable            proc far ; void far pascal disable(void)
                   cli
                   retf
disable            endp
_TEXT              ends

                   assume ds:DGROUP
_CONST             segment byte public use16 'CONST'
_num               db '0123456789ABCDEF'
_CONST             ends

                   assume ds:DGROUP
_DATA              segment byte public use16 'DATA'
_DATA              ends
                   end

; End of source 
