COMMENT #

#

DEF_ASM_INIT       EQU "INIT.ASM"

INCLUDE            COMMON.INC

PUBLIC             HaltDos, ShutDown, HaltSystem
PUBLIC             ResetVideo

                   ASSUME CS:CODE_INIT, DS:DATA_INIT, ES:NOTHING, SS:STACK_DEFAULT
CODE_INIT          SEGMENT
                   ORG DEF_NULL
L_ENTRY_CODE_INIT  LABEL FAR
.8086 ; Complete Intel 80x86 CPU
                   ; Clear code cache
                   MAC_ClearCache

                   ; Initialize stack
                   CLI                   
                   MOV SP, STACK_DEFAULT
                   MOV SS, SP
                   MOV SP, DEF_SIZE_STACK_DEFAULT
                   PUSH BSS_INFO
                   PUSH OFFSET BSS_INFO:INFO_IntVector
                   PUSH DEF_Null
                   PUSH DEF_Null
                   PUSH 0004h * DEF_INFO_IntVectors
                   CALL FAR PTR CODE_MEMORY:MemCpy
                   STI

                   ; Check CPU & V-Mode
                   CALL FAR PTR CODE_INFO:WhatCPU
                   CMP AX, 0003h ; if x < 80x386 than
                   ; JAE SHORT L_AcceptCPU
                   JMP SHORT L_AcceptCPU
                   PUSH CODE_INIT
                   PUSH OFFSET CODE_INIT:S_ERROR_AcceptCPU
                   CALL FAR PTR CODE_TEXT:Puts
                   CALL FAR PTR CODE_INIT:HaltSystem                   
L_AcceptCPU        LABEL SHORT

                   ; Code is original kernel
DEF_CPU_Default
                   PUSHF
                   CLI
                   MOVZX ESP, SP                           ; Expend ESP register
                   POPF

                   ; Store / Clear register
                   ASSUME DS:BSS_INFO
                   PUSH DS
                   PUSH DS
                   PUSH BSS_INFO
                   POP DS
                   PUSHFD
                   POP DWORD PTR BSS_INFO:INFO_StartUpReg[0000h]
                   MOV DWORD PTR BSS_INFO:INFO_StartUpReg[0004h], EAX
                   MOV DWORD PTR BSS_INFO:INFO_StartUpReg[0008h], EBX
                   MOV DWORD PTR BSS_INFO:INFO_StartUpReg[000Ch], ECX
                   MOV DWORD PTR BSS_INFO:INFO_StartUpReg[0010h], EDX
                   MOV DWORD PTR BSS_INFO:INFO_StartUpReg[0014h], ESI
                   MOV DWORD PTR BSS_INFO:INFO_StartUpReg[0018h], EDI
                   MOV DWORD PTR BSS_INFO:INFO_StartUpReg[001Ch], EBP
                   MOV DWORD PTR BSS_INFO:INFO_StartUpReg[0020h], DEF_SIZE_STACK_DEFAULT ; ESP
                   MOV WORD PTR BSS_INFO:INFO_StartUpReg[0024h], CS
                   POP WORD PTR BSS_INFO:INFO_StartUpReg[0026h]
                   MOV WORD PTR BSS_INFO:INFO_StartUpReg[0028h], ES
                   MOV WORD PTR BSS_INFO:INFO_StartUpReg[002Ah], SS
                   MOV WORD PTR BSS_INFO:INFO_StartUpReg[002Ch], FS
                   MOV WORD PTR BSS_INFO:INFO_StartUpReg[002Eh], GS
                   POP DS
                   XOR EAX, EAX
                   XOR EBX, EBX
                   XOR ECX, ECX
                   XOR EDX, EDX
                   XOR ESI, ESI
                   XOR EDI, EDI
                   XOR EBP, EBP
                   MOV DS, AX
                   MOV ES, AX
                   MOV FS, AX
                   MOV GS, AX
                   ASSUME DS:DATA_INFO

                   ; Get information to fill DATA_INFO segment
                   PUSH AX
                   CALL FAR PTR CODE_TIME:OpenTimer ; Check clock tick for debugging
                   CALL NEAR PTR CODE_INIT:GetIP
GetIP              PROC NEAR
                   POP AX
                   SUB AX, (OFFSET CODE_INIT:GetIP) - (OFFSET CODE_INIT:L_ENTRY_CODE_INIT)
                   ASSUME DS:BSS_INFO
                   PUSH AX
                   PUSH CS                   
                   MOV AX, BSS_INFO
                   MOV DS, AX
                   POP WORD PTR BSS_INFO:INFO_Entry[DEF_Far_Segment]
                   POP WORD PTR BSS_INFO:INFO_Entry[DEF_Far_Offset]                   
GetIP              ENDP
                   CALL FAR PTR CODE_TIME:CloseTimer ; Check clock tick for debugging
                   ; Check clock tick - If TIMER_ClockTick over then Kill!
                   ASSUME DS:BSS_TIME
                   MOV AX, BSS_TIME
                   MOV DS, AX
                   IFDEF DEF_Lock_Debugging ; -------------------------------
                    MOV AX, WORD PTR BSS_TIME:TIMER_ClockTick[DEF_DWord_High]
                    OR AX, AX
                    JNZ SHORT L_NowDebugging
                    MOV AX, WORD PTR BSS_TIME:TIMER_ClockTick[DEF_DWord_Low]
                    CMP AX, DEF_AcceptTime_Debugging
                    JA SHORT L_NowDebugging
                   ENDIF ; --------------------------------------------------                                      
                   MAC_JumpShort <L_NotDebugging>
L_NowDebugging     LABEL SHORT                   
                   PUSH CODE_INIT
                   PUSH OFFSET CODE_INIT:S_ERROR_NowDebug
                   CALL FAR PTR CODE_TEXT:Puts
                   CALL FAR PTR CODE_INIT:HaltSystem                   
L_NotDebugging     LABEL SHORT
                   ; Calc program size
                   ASSUME DS:BSS_INFO
                   MOV AX, BSS_INFO
                   MOV DS, AX
                   MOV AX, HEAP_DEFAULT
                   SUB AX, CODE_ALIGN
                   MOV DX, 0010h
                   MUL DX
                   MOV WORD PTR BSS_INFO:INFO_ProgramSize[DEF_DWord_Low], AX
                   MOV WORD PTR BSS_INFO:INFO_ProgramSize[DEF_DWord_High], DX
                   ; ReAssume segment
                   ASSUME DS:DATA_INIT
                   PUSH DATA_INIT
                   POP DS
                   POP AX

                   ; Starting MINZ KERNEL ===================================                   

                   ; Setup text-mode
                   PUSH CODE_TEXT
                   PUSH OFFSET CODE_TEXT:Putc_01
                   CALL FAR PTR CODE_TEXT:SetPutc
                   PUSH DEF_False
                   CALL FAR PTR CODE_TEXT:Blink
                   
                   MOV AX, 0003h
                   INT DEF_INT_Video
                   ; Call void far pascal KernelMain(void)
                   MOV AX, DGROUP
                   MOV DS, AX
                   MOV ES, AX
                   XOR EAX, EAX
                   MOV FS, EAX
                   MOV GS, EAX
                   XOR EBX, EBX
                   XOR ECX, ECX
                   XOR EDX, EDX
                   XOR ESI, ESI
                   XOR EDI, EDI
                   XOR EBP, EBP
                   CALL FAR PTR KernelMain

                   ; End of MINZ KERNEL =====================================

                   CMP SP, DEF_SIZE_STACK_DEFAULT
                   JE SHORT L_OK_STACK
                   PUSH CODE_INIT
                   PUSH OFFSET CODE_INIT:S_ERROR_AvailStack
                   CALL FAR PTR CODE_TEXT:Puts
L_OK_STACK         LABEL SHORT
                   ; Halt system - End of run to shutdown
                   MAC_CallFar <CODE_INIT>, <OFFSET CODE_INIT:HaltSystem>
HaltSystem         PROC FAR ; void far pascal HaltSystem(void)
                   PUSH DEF_True
                   CALL FAR PTR CODE_TEXT:Blink
                   PUSHF
                   CLI
                   PUSH DEF_Null
                   PUSH DEF_Null
                   PUSH BSS_INFO
                   PUSH OFFSET BSS_INFO:INFO_IntVector
                   PUSH 0004h * DEF_INFO_IntVectors
                   CALL FAR PTR CODE_MEMORY:MemCpy
                   POPF
                   MAC_ClearCache
HaltDos            PROC FAR ; void far pascal HaltDos(AL)
                   MOV AH, 4Ch ; DOS Call - Return to dos
                   INT DEF_INT_Dos
HaltDos            ENDP
ShutDown           PROC FAR ; void far pascal ShutDown(void)
                   MAC_JumpShort <ShutDown>
ShutDown           ENDP
HaltSystem         ENDP
ResetVideo         PROC FAR ; void far pascal ResetVideo(void)
                   PUSHF                   
                   PUSHAD
                   PUSH DS
                   PUSH ES
                   MOV AX, 0C000h
                   MOV DS, AX
                   CMP BYTE PTR DS:[DEF_Word_Low], 55h
                   JNE SHORT L_ResetVideo_00
                   CMP BYTE PTR DS:[DEF_Word_High], 0AAh
                   JNE SHORT L_ResetVideo_00
                   MAC_CallFar <0C000h>, <0003h>
L_ResetVideo_00    LABEL SHORT
                   POP ES
                   POP DS
                   POPAD
                   POPF
                   RETF
ResetVideo         ENDP
CODE_INIT          ENDS

                   ASSUME CS:CODE_INIT, DS:DATA_INIT, ES:NOTHING, SS:STACK_DEFAULT
DATA_INIT          SEGMENT
S_ERROR_AvailStack DB "Error : Stack(SS:SP) was left."
                   DB DEF_ASCII_CarrigeReturn, DEF_ASCII_LineFeed
                   DB DEF_ASCII_EndOfString
S_ERROR_NowDebug   DB "Error : Fuck, Bad boy! Do not accept on debugging(Cracking)."
                   DB DEF_ASCII_CarrigeReturn, DEF_ASCII_LineFeed
                   DB DEF_ASCII_EndOfString
S_ERROR_AcceptCPU  DB "Error : Sorry! This system must be run under intel 32bit-CPU."
                   DB DEF_ASCII_CarrigeReturn, DEF_ASCII_LineFeed
                   DB DEF_ASCII_EndOfString
DATA_INIT          ENDS

                   ASSUME CS:CODE_INIT, DS:BSS_INIT, ES:NOTHING, SS:STACK_DEFAULT
BSS_INIT           SEGMENT
BSS_INIT           ENDS

                   ASSUME CS:CODE_INIT, DS:DATA_INIT, ES:NOTHING, SS:STACK_DEFAULT
STACK_DEFAULT      SEGMENT
                   DB DEF_SIZE_STACK_DEFAULT DUP (?)
STACK_DEFAULT      ENDS
                   END L_ENTRY_CODE_INIT

; End of source
