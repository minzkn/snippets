;============================
; MINZ technologies (C) 1998
; MINZ 개발용  - Reset FDC 
; Memory model - TINY
;============================

.MODEL TINY

PutString MACRO String
   ; 그냥 도스용에서 되게 21h를 쓰도록 하죠 -_-;
   mov AH,09h
   mov DX,offset String
   int 21h
ENDM PutString

.CODE
   ORG 100h

BeginCode:   
   PutString Message
   
   MOV DX,03F4h   ; 주 상태 레지스터 체크 
L001:   
   IN  AX,DX
   AND AX,64      ; 검사
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
