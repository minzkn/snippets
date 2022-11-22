.model large, c
.code
.286
           public asmwizard, asmclrbios, asmpark

asmwizard  proc far 
           cli
           in al, 21h
           push ax
           mov al, 11h
           out 20h, al
           mov al, 08h
           out 21h, al
           mov al, 04h
           out 21h, al
           mov al, 01h
           out 21h, al
           mov al, 0ffh
           out 21h, al
           pop ax
           out 21h, al
           sti
           retf
asmwizard  endp

asmclrbios proc far
           mov bx, 0010h
label00:
           mov al, bl
           out 70h, al
           out 0edh, al
           mov al, 0ffh
           out 71h, al
           inc bx
           cmp bx, 0080h
           jb label00
           mov ax, 0ffffh
           push ax
           xor ax, ax
           push ax
           mov es, ax
           mov di, 0472h
           stosw
           retf
asmclrbios endp

asmpark proc far
       JMP ORGENTRY
       DRIVE       DW 0080h
       PARK        DW 0000h
       FLAG        DW ?
ORGENTRY:
       MOV AX, 0800h
       XOR CX, CX
       MOV DX, 0080h
       INT 13h
       XOR DH, DH
       PUSHF
       POP FLAG
       AND FLAG, 0001h
       CMP FLAG, 0001h
       JE MESSAGEPUT
       CMP DX, 0000h
       JE MESSAGEPUT
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
       PUSHF
       POP FLAG
       AND FLAG, 0001h
       CMP FLAG, 0001h
       JE PARKCOM
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
       retf
asmpark endp
           end

/* End of source */