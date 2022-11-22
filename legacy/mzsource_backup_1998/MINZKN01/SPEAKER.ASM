COMMENT #

#

DEF_ASM_SPEAKER       EQU "SPEAKER.ASM"

INCLUDE            COMMON.INC

PUBLIC             Sound, NoSound, Beep

                   ASSUME CS:CODE_SPEAKER, DS:DATA_SPEAKER, ES:NOTHING, SS:STACK_DEFAULT
CODE_SPEAKER       SEGMENT
Sound              PROC FAR ; void far pascal Sound(unsigned int s_herz)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH BX
                   PUSH DX
                   MOV BX, WORD PTR [BP + 06h]             ; s_herz
                   MOV AX, 34DDh
                   MOV DX, 0012h
                   CMP DX, BX
                   JNB SHORT L_Sound_01
                   DIV BX
                   MOV BX, AX
                   IN AL, 61h
                   TEST AL, 03h
                   JNE SHORT L_Sound_00
                   OR AL, 03h
                   OUT 61h, AL
                   MOV AL, 0B6h
                   OUT 43h, AL
L_Sound_00         LABEL SHORT
                   MOV AL, BL
                   OUT 42h, AL
                   MOV AL, BH
                   OUT 42h, AL
L_Sound_01         LABEL SHORT
                   POP DX
                   POP BX
                   POP AX
                   POP BP
                   RETF 2
Sound              ENDP
NoSound            PROC FAR ; void far pascal NoSound(void)
                   PUSH AX
                   IN AL, 61h
                   AND AL, 0FCh
                   OUT 61h, AL
                   POP AX
                   RETF
NoSound            ENDP
Beep               PROC FAR ; void far pascal Beep(unsigned int s_herz, unsigned int s_delay)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 08h]                ; s_herz
                   CALL FAR PTR CODE_SPEAKER:Sound
                   PUSH WORD PTR [BP + 06h]                ; s_delay
                   CALL FAR PTR CODE_TIME:Delay
                   CALL FAR PTR CODE_SPEAKER:NoSound
                   POP BP 
                   RETF 2 + 2
Beep               ENDP
CODE_SPEAKER       ENDS

                   ASSUME CS:CODE_SPEAKER, DS:DATA_SPEAKER, ES:NOTHING, SS:STACK_DEFAULT
DATA_SPEAKER       SEGMENT
DATA_SPEAKER       ENDS

                   ASSUME CS:CODE_SPEAKER, DS:BSS_SPEAKER, ES:NOTHING, SS:STACK_DEFAULT
BSS_SPEAKER        SEGMENT
BSS_SPEAKER        ENDS
                   END

; End of source
