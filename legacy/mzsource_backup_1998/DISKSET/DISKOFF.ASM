           assume cs:_TEXT
_TEXT      segment byte public use16 'CODE'
           org 100h
start:
           mov ah, 09h
           mov dx, offset string
           int 21h
           mov dx, 01f7h
           mov al, 99h
           out dx, al
           int 20h
string     db 'Power conditions SLEEP!', 0dh, 0ah, 24h
_TEXT      ends
           end start
