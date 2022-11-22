COMMENT #

#

DEF_ASM_MBS        EQU "MBS.ASM"

INCLUDE            COMMON.INC

.8086

                   ASSUME CS:CODE_MBS, DS:DATA_MBS, ES:NOTHING, SS:STACK_DEFAULT
CODE_MBS           SEGMENT
                   ORG DEF_NULL
L_MBS_Entry        LABEL SHORT
                   CLI
                   XOR DI, DI
                   MOV SS, DI
                   MOV SP, 0500h + (0200h + 0200h)
                   MOV AX, 0050h
                   MOV ES, AX                              ; ES:DI = 0050h:0000h
                   CALL NEAR PTR CODE_MBS:MBS_GetIP
MBS_GetIP          PROC NEAR
                   POP SI
                   SUB SI, (OFFSET CODE_MBS:MBS_GetIP) - (OFFSET CODE_MBS:L_MBS_Entry)
                   PUSH CS
                   POP DS
MBS_GetIP          ENDP                                    ; DS:SI = CS:IP(Entry)
                   MOV CX, 0100h                           ; 256d Words
                   PUSH ES
                   MOV AX, OFFSET CODE_MBS:L_MBS_StartUp
                   PUSH AX
                   CLD
                   REPZ MOVSW
                   RETF                                    ; Jump to ES:L_MBS_StartUp
L_MBS_StartUp      LABEL SHORT
                   STI
                   PUSH ES
                   POP DS                   
                   MOV ES, CX
                   CMP WORD PTR CODE_MBS:D_MBS_PTablesInit, 0AA55h
                   JZ SHORT CODE_MBS:L_MBS_YesPartiton
                   MOV SI, OFFSET CODE_MBS:S_MBS_Error_00
                   CALL NEAR PTR CODE_MBS:MBS_Puts
                   MAC_ShutDown
L_MBS_YesPartiton  LABEL SHORT
                   MOV SI, OFFSET CODE_MBS:S_MBS_Question
                   CALL NEAR PTR CODE_MBS:MBS_Puts
L_MBS_ReLoadPassword LABEL SHORT
                   MOV SI, OFFSET CODE_MBS:S_MBS_Password
L_MBS_LoadPassword LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT CODE_MBS:L_YesAccept
                   PUSH AX
                   XOR AX, AX
                   INT 16h
                   POP BX
                   CMP AL, BL
                   JE SHORT CODE_MBS:L_MBS_LoadPassword
                   CMP AL, 0Dh
                   JE SHORT CODE_MBS:L_MBS_NoAccept
                   JMP SHORT CODE_MBS:L_MBS_ReLoadPassword
L_MBS_NoAccept     LABEL SHORT
                   MOV SI, OFFSET CODE_MBS:S_MBS_NoAccept
                   CALL NEAR PTR CODE_MBS:MBS_Puts
                   INC BYTE PTR CODE_MBS:D_MBS_AcceptFlag
L_YesAccept        LABEL SHORT
                   MOV SI, OFFSET CODE_MBS:S_MBS_Accept
                   CALL NEAR PTR CODE_MBS:MBS_Puts
                   MOV SI, OFFSET CODE_MBS:D_MBS_PTables
                   MOV CX, 0004h
                   MOV DI, 0004h                   
L_MBS_SearchPTable LABEL SHORT
                   TEST BYTE PTR [SI + 0000h], 80h
                   JNZ SHORT CODE_MBS:L_MBS_BootPartiton
                   ADD SI, 0010h
                   LOOP SHORT CODE_MBS:L_MBS_SearchPTable
L_MBS_NotLoad      LABEL SHORT
                   MOV SI, OFFSET CODE_MBS:S_MBS_Error_01
                   CALL NEAR PTR CODE_MBS:MBS_Puts
                   INT 18h
                   MAC_ShutDown
L_MBS_ReLoad       LABEL SHORT
                   XOR AX, AX
                   INT 13h
                   OR DI, DI
                   JZ SHORT CODE_MBS:L_MBS_NotLoad
                   DEC DI
L_MBS_BootPartiton LABEL SHORT
                   PUSH ES
                   MOV BX, 7C00h                   
                   PUSH BX
                   MOV AX, 0201h
                   MOV CX, WORD PTR [SI + 0002h]
                   MOV DX, WORD PTR [SI + 0000h]
                   CMP BYTE PTR CODE_MBS:D_MBS_AcceptFlag, 00h
                   JZ SHORT CODE_MBS:L_Go
                   MOV SI, 01CEh
                   MOV CX, 0001h
                   MOV DX, 0000h                   
L_Go               LABEL SHORT
                   INT 13h
                   JC SHORT CODE_MBS:L_MBS_ReLoad
                   RETF
L_MBS_Puts_00      LABEL SHORT
                   INT 10h
MBS_Puts           PROC NEAR ; void near pascal MBS_Puts(DS:SI)
                   MOV AH, 0Eh
                   MOV BX, 0007h
                   LODSB
                   OR AL, AL
                   JNZ SHORT CODE_MBS:L_MBS_Puts_00
                   RETN
MBS_Puts           ENDP
D_MBS_AcceptFlag   DB 00h
S_MBS_Error_00     DB "Invalid partition table", 0Dh, 0Ah, 00h
S_MBS_Error_01     DB "Error loading operating system", 0Dh, 0Ah, 00h
S_MBS_Question     DB 0FEh, " System locker Version1.00B for MINZ software", 0Dh, 0Ah
                   DB "  Copyright (c) 2000 by JaeHyuk CHO", 0Dh, 0Ah
                   DB " - Type password : ", 00h
S_MBS_NoAccept     DB "User mode - ", 00h
S_MBS_Accept       DB "Accept in boot.", 0Dh, 0Ah, 0Ah, "Message : ", 00h                  
S_MBS_Password     DB "Nsens", 0Dh, 00h
                   ORG 01BEh
D_MBS_PTables      DB (10h * 04h) DUP (00h)                ; Partiton tables
D_MBS_PTablesInit  DB 55h, 0AAh                            ; Partition initial
CODE_MBS           ENDS

                   ASSUME CS:CODE_MBS, DS:DATA_MBS, ES:NOTHING, SS:STACK_DEFAULT
DATA_MBS           SEGMENT
DATA_MBS           ENDS

                   ASSUME CS:CODE_MBS, DS:BSS_MBS, ES:NOTHING, SS:STACK_DEFAULT
BSS_MBS            SEGMENT
BSS_MBS            ENDS
                   END

; End of source
