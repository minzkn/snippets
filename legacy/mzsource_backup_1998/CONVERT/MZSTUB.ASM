.286
.model tiny, pascal
.code
_start@:
mov ax, cs
mov ds, ax
mov ah, 09h
mov dx, offset _str@
int 21h
mov ax, 4c00h
int 21h
jmp short $
.data
_str@ db 'This program must be under MINZ.', 0ah, 0dh, '$'
.stack 128
end _start@
