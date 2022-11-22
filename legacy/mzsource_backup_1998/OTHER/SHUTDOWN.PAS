Program Shutdown;

Begin
 asm
  mov dx, 01f7h
  mov al, 99h
  out dx, al
  db 0eah
  dw 0000h, 0ffffh
 End;
End.