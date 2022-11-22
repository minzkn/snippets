COMMENT #
 NOTE : 2000.10.07 (SAT)
  - MZLIB : "CALC.LIB"
#

DEF_ASM_CALC       EQU "CALC.ASM"

INCLUDE            COMMON.INC

PUBLIC             Randomize, Rand, Random
PUBLIC             ToPhysical, ToLogical, ToOptimalLogical
PUBLIC             Center
PUBLIC             DivL

                   ASSUME CS:CODE_CALC, DS:DATA_CALC, ES:NOTHING, SS:STACK_DEFAULT
CODE_CALC          SEGMENT
Randomize          PROC FAR ; void far pascal Randomize(void)
                   PUSH AX
                   IN AL, 40h
                   ADD BYTE PTR CODE_CALC:D_Rand[0000h], AL
                   IN AL, 40h
                   ADC BYTE PTR CODE_CALC:D_Rand[0001h], AL
                   IN AL, 40h
                   ADC BYTE PTR CODE_CALC:D_Rand[0002h], AL
                   IN AL, 40h
                   ADC BYTE PTR CODE_CALC:D_Rand[0003h], AL
                   POP AX
                   RETF
Randomize          ENDP
Rand               PROC FAR ; unsigned int far pascal Rand(void)
                   PUSH EDX
                   MOV EAX, DEF_RANDOM_Value
                   MUL DWORD PTR CODE_CALC:D_Rand
                   INC EAX
                   MOV DWORD PTR CODE_CALC:D_Rand, EAX
                   SHR EAX, 10h
                   POP EDX
                   RETF
D_Rand             DD DEF_RANDOM_Value
Rand               ENDP
Random             PROC FAR ; unsigned int far pascal Randomize(unsigned int s_limit)
                   PUSH BP
                   MOV BP, SP
                   PUSH DX
                   CALL FAR PTR CODE_CALC:Rand
                   XOR DX, DX
                   DIV WORD PTR [BP + 06h]
                   MOV AX, DX
                   POP DX
                   POP BP
                   RETF 2
Random             ENDP
ToPhysical         PROC FAR ; unsigned long far pascal ToPhysical(void far *s_logical)
                   ; Return = (((High(s_logical)>>0x0C)|(High(s_logical)>>04h)<<0x16)+Low(s_logical)
                   PUSH BP
                   MOV BP, SP
                   MOV AX, WORD PTR [BP + 08h]             ; s_logical high
                   MOV DX, AX
                   SHL AX, 04h
                   SHR DX, 0Ch
                   ADD AX, WORD PTR [BP + 06h]             ; s_logical low
                   ADC DX, DEF_Null
                   POP BP
                   RETF 4
ToPhysical         ENDP
ToLogical          PROC FAR ; void far * far pascal ToLogical(unsigned long s_physical)
                   ; Return = (High(s_physical)<<0x0C)|(Low(s_physical)>>0x04):(Low(s_physical)&0x000f)
                   PUSH BP
                   MOV BP, SP
                   MOV AX, WORD PTR [BP + 06h]             ; s_physical low
                   MOV DX, WORD PTR [BP + 08h]             ; s_physical high
                   SHL DX, 0Ch
                   PUSH AX
                   SHR AX, 04h
                   OR DX, AX
                   POP AX
                   AND AX, 000Fh
                   POP BP
                   RETF 4
ToLogical          ENDP
ToOptimalLogical   PROC FAR ; void far * far pascal ToOptimalLogical(void far *s_logical)
                   PUSH BP
                   MOV BP, SP
                   MOV AX, WORD PTR [BP + 08h]             ; s_logical high
                   MOV DX, AX
                   SHL AX, 04h
                   SHR DX, 0Ch
                   ADD AX, WORD PTR [BP + 06h]             ; s_logical low
                   ADC DX, DEF_Null
                   SHL DX, 0Ch
                   PUSH AX
                   SHR AX, 04h
                   OR DX, AX
                   POP AX
                   AND AX, 000Fh
                   POP BP
                   RETF 4
ToOptimalLogical   ENDP
Center             PROC FAR ; unsigned int far pascal Center(unsigned int s_value0, unsigned int s_value1)
                   PUSH BP
                   MOV BP, SP
                   MOV AX, WORD PTR [BP + 06h]             ; s_value1
                   SUB AX, WORD PTR [BP + 08h]             ; s_value0
                   INC AX
                   SHR AX, 01h
                   ADD AX, WORD PTR [BP + 08h]             ; s_value0
                   POP BP
                   RETF 2 + 2
Center             ENDP
DivL               PROC FAR ; unsigned long far pascal DivL(unsigned long s_value, unsigned int s_div)
                   PUSH BP
                   MOV BP, SP
                   MOV EAX, DWORD PTR [BP + 08h]           ; s_value low
                   XOR EDX, EDX
                   XOR EBX, EBX
                   MOV BX, WORD PTR [BP + 06h]             ; s_div                   
                   DIV EBX
                   MOV BX, DX
                   MOV EDX, EAX
                   SHR EDX, 0010h
                   POP BP
                   RETF 4 + 2
DivL               ENDP
CODE_CALC          ENDS

                   ASSUME CS:CODE_CALC, DS:DATA_CALC, ES:NOTHING, SS:STACK_DEFAULT
DATA_CALC          SEGMENT
DATA_CALC          ENDS

                   ASSUME CS:CODE_CALC, DS:BSS_CALC, ES:NOTHING, SS:STACK_DEFAULT
BSS_CALC           SEGMENT
BSS_CALC           ENDS
                   END

; End of source
