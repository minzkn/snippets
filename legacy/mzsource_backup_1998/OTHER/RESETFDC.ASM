;============================
; MINZ technologies (C) 1998
; MINZ ���߿�  - Reset FDC 
; Memory model - TINY
;============================

.MODEL TINY

PutString MACRO String
   ; �׳� �����뿡�� �ǰ� 21h�� ������ ���� -_-;
   mov AH,09h
   mov DX,offset String
   int 21h
ENDM PutString

.CODE
   ORG 100h

BeginCode:   
   PutString Message
   
   MOV DX,03F4h   ; �� ���� �������� üũ 
L001:   
   IN  AX,DX
   AND AX,64      ; �˻�
   CMP AX,0       
   JNGE L001 
   MOV DX,3F2h
   MOV AX,0
   OUT DX,AX
   
   MOV AH,4Ch
   INT 21h   ; Call DOS
Datas:
   Message DB 10,13,' RESETDISK Version 1.0 --- MINZ Technologies (C) 1998',10,13
           DB ' Initialing FDC..........',10,13,'$'
END BeginCode