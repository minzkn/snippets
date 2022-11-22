; PowerJACK utility v1.0
; 300Byte
; Copyright(c)1997 by JaeHyuk.CHO 

.MODEL SMALL
.CODE
       ORG 100h
STARTENTRY:
       JMP ORGENTRY
       COMPLETESTRING DB 'Turn off your computer [Parking complete]', 0Dh, 0Ah, '$'
       ERRORSTRING    DB 'Error: Parking abort', 0Dh, 0Ah, '$'
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
       MOV AH, 09h
       CMP PARK, 0FFh
       JNE OKMESSAGE
       MOV DX, OFFSET ERRORSTRING
       JMP INTPUT
OKMESSAGE:
       MOV DX, OFFSET COMPLETESTRING
INTPUT:
       INT 21h
       MOV AH, 07h
       INT 21h
EXITENTRY:                
       INT 20h
       END STARTENTRY
