.286
.model tiny, pascal
.code
org 100h
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
_str@ db 'Warnning: Now directory not support!', 0ah, 0dh, '$'
end _start@
