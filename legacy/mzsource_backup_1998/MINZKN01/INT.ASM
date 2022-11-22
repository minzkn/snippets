COMMENT #
 NOTE : 2000.10.07 (SAT)
  - MZLIB : "INT.LIB"
#

DEF_ASM_INT        EQU "INT.ASM"

INCLUDE            COMMON.INC

PUBLIC             NopInterrupt
PUBLIC             GetVect, SetVect

                   ASSUME CS:CODE_INT, DS:DATA_INT, ES:NOTHING, SS:STACK_DEFAULT
CODE_INT           SEGMENT
NopInterrupt       PROC FAR ; void interrupt NopInterrupt(void)
                   IRET
NopInterrupt       ENDP
GetVect            PROC FAR ; void far * far pascal GetVect(unsigned int s_intnum)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH BX
                   XOR AX, AX
                   MOV DS, AX
                   MOV BX, WORD PTR [BP + 06h]             ; s_intnum
                   SHL BX, 02h
                   PUSHF
                   CLI
                   MOV AX, WORD PTR DS:[BX + DEF_Far_Offset]
                   MOV DX, WORD PTR DS:[BX + DEF_Far_Segment]
                   POPF
                   POP BX
                   POP DS
                   POP BP
                   RETF 2
GetVect            ENDP
SetVect            PROC FAR ; void far pascal SetVect(void far *s_handle, unsigned int s_intnum)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH BX
                   XOR BX, BX
                   MOV DS, BX
                   MOV BX, WORD PTR [BP + 06h]             ; s_intnum
                   SHL BX, 02h
                   PUSHF
                   CLI
                   PUSH WORD PTR [BP + 08h + DEF_Far_Offset]
                   POP WORD PTR DS:[BX + DEF_Far_Offset]
                   PUSH WORD PTR [BP + 08h + DEF_Far_Segment]
                   POP WORD PTR DS:[BX + DEF_Far_Segment]
                   POPF
                   POP BX
                   POP DS
                   POP BP
                   RETF 4 + 2
SetVect            ENDP
CODE_INT           ENDS

                   ASSUME CS:CODE_INT, DS:DATA_INT, ES:NOTHING, SS:STACK_DEFAULT
DATA_INT           SEGMENT
DATA_INT           ENDS

                   ASSUME CS:CODE_INT, DS:BSS_INT, ES:NOTHING, SS:STACK_DEFAULT
BSS_INT            SEGMENT
BSS_INT            ENDS
                   END

; End of source
