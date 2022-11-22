COMMENT #
 Code by JaeHyuk Cho
 Copyright (c) 1998 MINZKN
#

DEF_ASM_INFO       EQU "INFO.ASM"

INCLUDE            COMMON.INC

PUBLIC             Is_Intel80x86
PUBLIC             Is_Intel80x286
PUBLIC             Is_Intel80x386
PUBLIC             Is_Intel80x486
PUBLIC             Is_Intel80x586
PUBLIC             WhatCPU
;
PUBLIC             INFO_Entry
PUBLIC             INFO_StartUpReg
PUBLIC             INFO_IntVector
PUBLIC             INFO_ProgramSize

                   ASSUME CS:CODE_INFO, DS:DATA_INFO, ES:NOTHING, SS:STACK_DEFAULT
CODE_INFO          SEGMENT
Is_Intel80x86      PROC FAR ; unsigned int far pascal Is_Intel80x86(void)
.8086
                   PUSHF
                   PUSHF
                   POP AX
                   AND AH, 0Fh
                   PUSH AX
                   POPF
                   PUSHF
                   POP AX
                   TEST AH, 0F0h
                   JNZ SHORT L_Is_Intel80x86_00
                   XOR AX, AX
                   MAC_JumpShort <L_Is_Intel80x86_01>
L_Is_Intel80x86_00 LABEL SHORT
                   MOV AX, DEF_True
L_Is_Intel80x86_01 LABEL SHORT
                   POPF
                   RETF
DEF_CPU_Default
Is_Intel80x86      ENDP
Is_Intel80x286     PROC FAR ; unsigned int far pascal Is_Intel80x286(void)
.286
                   PUSHF
                   PUSHF
                   POP AX
                   OR AH, 0F0h
                   PUSH AX
                   POPF
                   PUSHF
                   POP AX
                   TEST AH, 0F0h
                   JZ SHORT L_Is_Intel80x286_00
                   XOR AX, AX
                   MAC_JumpShort <L_Is_Intel80x286_01>
L_Is_Intel80x286_00 LABEL SHORT
                   MOV AX, DEF_True
L_Is_Intel80x286_01 LABEL SHORT
                   POPF
                   RETF
DEF_CPU_Default
Is_Intel80x286     ENDP
Is_Intel80x386     PROC FAR ; unsigned int far pascal Is_Intel80x386(void)
.386P
                   PUSHFD
                   PUSH EBX
                   PUSHFD
                   POP EBX
                   OR EBX, 00040000h
                   PUSH EBX
                   POPFD
                   PUSHFD
                   POP EBX
                   TEST EBX, 00040000h
                   JZ SHORT L_Is_Intel80x386_00
                   XOR AX, AX
                   MAC_JumpShort <L_Is_Intel80x386_01>
L_Is_Intel80x386_00 LABEL SHORT
                   MOV AX, DEF_True
L_Is_Intel80x386_01 LABEL SHORT
                   POP EBX
                   POPFD
                   RETF
DEF_CPU_Default
Is_Intel80x386     ENDP
Is_Intel80x486     PROC FAR ; unsigned int far pascal Is_Intel80x486(void)
.486P
                   PUSHFD
                   PUSH EBX
                   PUSH ECX
                   PUSHFD
                   POP EBX
                   XOR EBX, 00200000h
                   PUSH EBX
                   POPFD
                   PUSHFD
                   POP ECX
                   CMP EBX, ECX
                   JE SHORT L_Is_Intel80x486_00
                   XOR AX, AX
                   MAC_JumpShort <L_Is_Intel80x486_01>
L_Is_Intel80x486_00 LABEL SHORT
                   MOV AX, DEF_True
L_Is_Intel80x486_01 LABEL SHORT
                   POP ECX
                   POP EBX
                   POPFD
                   RETF
DEF_CPU_Default
Is_Intel80x486     ENDP
Is_Intel80x586     PROC FAR ; unsigned int far pascal Is_Intel80x586(void)                   
.586P
                   PUSHAD
                   XOR EAX, EAX
                   CPUID
                   CMP EAX, 00000001h
                   JNE SHORT L_Is_Intel80x586_00

                   CPUID

                   POPAD
                   MOV AX, DEF_True
                   RETF
L_Is_Intel80x586_00 LABEL SHORT
                   POPAD
                   XOR AX, AX
                   RETF
DEF_CPU_Default
Is_Intel80x586     ENDP
WhatCPU            PROC FAR ; unsigned int far pascal WhatCPU(void)
                   PUSH BX
                   XOR BX, BX
                   CALL FAR PTR CODE_INFO:Is_Intel80x86 ; Return 0
                   OR AX, AX
                   JNZ SHORT L_WhatCPU_00
                   INC BX
                   CALL FAR PTR CODE_INFO:Is_Intel80x286 ; Return 1
                   OR AX, AX
                   JNZ SHORT L_WhatCPU_00
                   INC BX
                   CALL FAR PTR CODE_INFO:Is_Intel80x386 ; Return 2
                   OR AX, AX
                   JNZ SHORT L_WhatCPU_00
                   INC BX
                   CALL FAR PTR CODE_INFO:Is_Intel80x486 ; Return 3
                   OR AX, AX
                   JNZ SHORT L_WhatCPU_00
                   INC BX
                   CALL FAR PTR CODE_INFO:Is_Intel80x586 ; Return 4
                   OR AX, AX
                   JNZ SHORT L_WhatCPU_00
                   INC BX
L_WhatCPU_00       LABEL SHORT
                   MOV AX, BX
                   POP BX
                   RETF
WhatCPU            ENDP
CODE_INFO          ENDS

                   ASSUME CS:CODE_INFO, DS:DATA_INFO, ES:NOTHING, SS:STACK_DEFAULT
DATA_INFO          SEGMENT
DATA_INFO          ENDS

                   ASSUME CS:CODE_INFO, DS:BSS_INFO, ES:NOTHING, SS:STACK_DEFAULT
BSS_INFO           SEGMENT
INFO_Entry         DW ?, ?                                 ; CODE_INIT's CS:IP entry point
INFO_StartUpReg    DD ? ; 0000h : EFLAGS
                   DD ? ; 0004h : EAX
                   DD ? ; 0008h : EBX
                   DD ? ; 000Ch : ECX
                   DD ? ; 0010h : EDX
                   DD ? ; 0014h : ESI
                   DD ? ; 0018h : EDI
                   DD ? ; 001Ch : EBP
                   DD ? ; 0020h : ESP
                   DW ? ; 0024h : CS
                   DW ? ; 0026h : DS
                   DW ? ; 0028h : ES
                   DW ? ; 002Ah : SS
                   DW ? ; 002Ch : FS
                   DW ? ; 002Eh : GS
INFO_IntVector     DD DEF_INFO_IntVectors DUP (?)
INFO_ProgramSize   DD ?
BSS_INFO           ENDS
                   END

; End of source
