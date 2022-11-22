COMMENT #
 NOTE: 2000.10.03 [TUE]
  - LtoA / ItoA parameter
    void far *s_dest            : 0Eh
    unsigned long s_value / unsigned int s_value : 0Ah
    unsigned int s_option0      : 08h
     Bit 0     : (-) sign = 1, Nothing = 0
     Bit 1     : (+) sign = 1, Nothing = 0                   
     Bit 2 ~ 3 : 00 = Nothing, 01 = Left align, 10 = Right align, 11 = Center
     Bit 4 ~ 6 : Comma : If 0 is nothing
     Bit 7 ~ C : Fill count : If 0 is nothing
     Bit D     : Last comma & space = 1, Nothing = 0
     Bit E     : Line feed = 1, Nothing = 0
     Bit F     : Decimal character = 1, Nothing = 0
    unsigned int s_option1      : 06h
     Bit 0 ~ 3 : (radix - 1)
     Bit 4     : Decimal character is (Lower = 0, Upper = 1)
     Bit 5     : Number is (Upper = 0, Lower = 1)
     Bit 6     : Insert sign space = 1, Nothing = 0
     Bit 7     : () = 1, Nothing = 0
     Bit 8 ~ F : Fill character
#

DEF_ASM_TO         EQU "TO.ASM"

INCLUDE            COMMON.INC

PUBLIC             ToUpper, ToLower, ToAsciiNumber, ToNum
PUBLIC             LtoA, ItoA, AtoL, AtoI

                   ASSUME CS:CODE_TO, DS:DATA_TO, ES:NOTHING, SS:STACK_DEFAULT
CODE_TO            SEGMENT
ToUpper            PROC FAR ; unsigned int far pascal ToUpper(unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h]                ; s_character
                   CALL FAR PTR CODE_IS:IsLower
                   MOV AH, BYTE PTR [BP + 06h]             ; s_character
                   XCHG AH, AL
                   OR AH, AH
                   JZ SHORT CODE_TO:L_ToUpper_00
                   XOR AH, AH
                   SUB AL, 'a'-'A'
L_ToUpper_00       LABEL SHORT                 
                   POP BP
                   RETF 2
ToUpper            ENDP
ToLower            PROC FAR ; unsigned int far pascal ToLower(unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h]                ; s_character
                   CALL FAR PTR CODE_IS:IsUpper
                   MOV AH, BYTE PTR [BP + 06h]             ; s_character
                   XCHG AH, AL
                   OR AH, AH
                   JZ SHORT CODE_TO:L_ToLower_00
                   XOR AH, AH
                   ADD AL, 'a'-'A'
L_ToLower_00       LABEL SHORT                 
                   POP BP
                   RETF 2
ToLower            ENDP
ToAsciiNumber      PROC FAR ; unsigned int far pascal ToAsciiNumber(unsigned int s_number)
                   PUSH BP
                   MOV BP, SP
                   MOV AL, '0'
                   CMP BYTE PTR [BP + 06h], 10h + ('Z' - 'A') ; s_number
                   JAE SHORT L_ToAsciiNumber_01
                   MOV AL, BYTE PTR [BP + 06h]             ; s_number
                   CMP AL, 09h
                   JBE SHORT L_ToAsciiNumber_00
                   ADD AL, 'A' - 0Ah
                   JMP SHORT L_ToAsciiNumber_01
L_ToAsciiNumber_00 LABEL SHORT
                   ADD AL, '0'
L_ToAsciiNumber_01 LABEL SHORT
                   XOR AH, AH
                   POP BP
                   RETF 2
ToAsciiNumber      ENDP
ToNum              PROC FAR ; unsigned int far pascal ToNum(unsigned int s_ascii)
                   PUSH BP
                   MOV BP, SP
                   MOV AL, BYTE PTR [BP + 06h]             ; s_ascii
                   XOR AH, AH
                   CMP AL, '0'
                   JB SHORT L_ToNum_00
                   SUB AL, '0'
                   CMP AL, 09h
                   JBE SHORT L_ToNum_01
                   PUSH WORD PTR [BP + 06h]                ; s_ascii
                   CALL FAR PTR CODE_TO:ToUpper                   
                   CMP AL, 'A'
                   JB SHORT L_ToNum_00
                   SUB AL, 'A' - 0Ah
                   CMP AL, 'Z' - 'A'
                   JBE SHORT L_ToNum_01
L_ToNum_00         LABEL SHORT
                   XOR AX, AX
L_ToNum_01         LABEL SHORT
                   POP BP
                   RETF 2
ToNum              ENDP
LtoA               PROC FAR ; void far pascal LtoA(void far *s_dest, unsigned long s_value, unsigned int s_option0, unsigned int s_option1)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSHA
                   MOV WORD PTR CODE_TO:D_LtoA_Flag, DEF_NULL
                   TEST BYTE PTR [BP + 08h], 01h ; - Sign
                   JZ SHORT L_LtoA_NoSign
                   TEST BYTE PTR [BP + 0Dh], 80h           ; s_value high bit
                   JZ SHORT L_LtoA_NoSign
                   OR BYTE PTR CODE_TO:D_LtoA_Flag, 01h
                   NOT WORD PTR [BP + 0Ch]                 ; s_value high
                   NOT WORD PTR [BP + 0Ah]                 ; s_value low
                   ADD WORD PTR [BP + 0Ah], 0001h          ; s_value low
                   ADC WORD PTR [BP + 0Ch], DEF_NULL       ; s_value high
L_LtoA_NoSign      LABEL SHORT
                   PUSH DEF_SIZE_LtoA_Buffer
                   CALL FAR PTR CODE_HEAP:FarAlloc
                   MOV WORD PTR CODE_TO:D_LtoA_Buffer[DEF_SIZE_WORD], DX
                   MOV WORD PTR CODE_TO:D_LtoA_Buffer[DEF_NULL], AX
                   MOV DS, DX
                   MOV SI, AX                   
                   XOR CX, CX
                   MOV DI, WORD PTR [BP + 06h]             ; s_option 1
                   AND DI, 000Fh
                   INC DI
                   TEST BYTE PTR [BP + 07h], 0FFh
                   JNZ SHORT L_LtoA_00
                   MOV BYTE PTR [BP + 07h], DEF_ASCII_Space
L_LtoA_00          LABEL SHORT                   
                   PUSH WORD PTR [BP + 0Ch]                ; s_value high
                   PUSH WORD PTR [BP + 0Ah]                ; s_value low
                   PUSH DI
                   CALL FAR PTR CODE_CALC:DivL
                   MOV WORD PTR [BP + 0Ch], DX             ; s_value high
                   MOV WORD PTR [BP + 0Ah], AX             ; s_value low
                   OR AX, DX
                   OR AX, BX
                   OR AX, AX
                   JZ SHORT L_LtoA_02
                   PUSH BX
                   CALL FAR PTR CODE_TO:ToAsciiNumber
                   MOV BYTE PTR DS:[SI], AL
                   INC SI
                   INC CX                   
                   JMP SHORT L_LtoA_00
L_LtoA_01          LABEL SHORT
                   MOV AL, '0'
                   STOSB
                   JMP SHORT L_LtoA_04
L_LtoA_02          LABEL SHORT
                   LES DI, DWORD PTR [BP + 0Eh]            ; s_dest
                   JCXZ SHORT L_LtoA_01                   
                   CLD
L_LtoA_03          LABEL SHORT
                   DEC SI
                   MOV AL, BYTE PTR DS:[SI]                   
                   STOSB
                   LOOP SHORT L_LtoA_03
L_LtoA_04          LABEL SHORT
                   XOR AL, AL
                   STOSB
                   PUSH WORD PTR CODE_TO:D_LtoA_Buffer[DEF_SIZE_WORD]
                   PUSH WORD PTR CODE_TO:D_LtoA_Buffer[DEF_NULL]
                   CALL FAR PTR CODE_HEAP:FarFree
                   TEST BYTE PTR [BP + 06h], 20h           ; Number is L/U
                   JZ SHORT L_LtoA_05
                   PUSH WORD PTR [BP + 10h]                ; s_string high
                   PUSH WORD PTR [BP + 0Eh]                ; s_string low
                   CALL FAR PTR CODE_STRING:StrLwr
L_LtoA_05          LABEL SHORT
                   TEST BYTE PTR [BP + 08h], 70h           ; Comma
                   JZ SHORT L_LtoA_06
                   MOV AL, BYTE PTR [BP + 08h]             ; Comma
                   SHR AL, 04h
                   AND AL, 07h
                   XOR AH, AH
                   PUSH WORD PTR [BP + 10h]                ; s_string high
                   PUSH WORD PTR [BP + 0Eh]                ; s_string low
                   PUSH ','
                   PUSH AX
                   CALL FAR PTR CODE_STRING:GridInsert
L_LtoA_06          LABEL SHORT
                   MOV CX, WORD PTR [BP + 08h]             ; Align counter
                   SHR CX, 07h
                   AND CX, 003Fh                   
                   PUSH WORD PTR [BP + 10h]                ; s_string high
                   PUSH WORD PTR [BP + 0Eh]                ; s_string low
                   CALL FAR PTR CODE_STRING:StrLen
                   CMP CX, AX
                   JBE SHORT L_LtoA_09
                   SUB CX, AX
                   MOV AL, BYTE PTR [BP + 08h]             ; Align
                   AND AL, 0Ch
                   PUSH WORD PTR [BP + 10h]                ; s_string high
                   PUSH WORD PTR [BP + 0Eh]                ; s_string low
                   PUSH WORD PTR [BP + 07h]                ; s_fill
                   PUSH CX
                   CMP AL, 03h SHL 02h ; Center
                   JNE SHORT L_LtoA_07                   
                   CALL FAR PTR CODE_STRING:CharAlign
                   JMP SHORT L_LtoA_09
L_LtoA_07          LABEL SHORT
                   CMP AL, 01h SHL 02h ; Left
                   JNE SHORT L_LtoA_08                   
                   CALL FAR PTR CODE_STRING:CharAddtion 
                   JMP SHORT L_LtoA_09
L_LtoA_08          LABEL SHORT ; Right
                   CALL FAR PTR CODE_STRING:CharInsert
L_LtoA_09          LABEL SHORT
                   TEST BYTE PTR [BP + 06h], 40h ; Insert sign space
                   JZ SHORT L_LtoA_0A
                   PUSH WORD PTR [BP + 10h]                ; s_string high
                   PUSH WORD PTR [BP + 0Eh]                ; s_string low
                   PUSH DEF_ASCII_Space
                   PUSH 0001h
                   CALL FAR PTR CODE_STRING:CharInsert
L_LtoA_0A          LABEL SHORT
                   TEST BYTE PTR [BP + 08h], 01h ; - Sign
                   JZ SHORT L_LtoA_0B
                   TEST BYTE PTR CODE_TO:D_LtoA_Flag, 01h
                   JZ SHORT L_LtoA_0B
                   PUSH WORD PTR [BP + 10h]                ; s_string high
                   PUSH WORD PTR [BP + 0Eh]                ; s_string low
                   PUSH '-'
                   PUSH 0001h
                   CALL FAR PTR CODE_STRING:CharInsert
L_LtoA_0B          LABEL SHORT
                   TEST BYTE PTR [BP + 08h], 02h ; + Sign
                   JZ SHORT L_LtoA_0C
                   TEST BYTE PTR CODE_TO:D_LtoA_Flag, 01h
                   JNZ SHORT L_LtoA_0C
                   PUSH WORD PTR [BP + 10h]                ; s_string high
                   PUSH WORD PTR [BP + 0Eh]                ; s_string low
                   PUSH '+'
                   PUSH 0001h
                   CALL FAR PTR CODE_STRING:CharInsert
L_LtoA_0C          LABEL SHORT
                   TEST BYTE PTR [BP + 09h], 80h ; Decimal character
                   JZ SHORT L_LtoA_12
                   PUSH WORD PTR [BP + 10h]                ; s_string high
                   PUSH WORD PTR [BP + 0Eh]                ; s_string low
                   PUSH DEF_ASCII_EndOfString
                   PUSH DEF_STRING_Max
                   CALL FAR PTR CODE_MEMORY:MemScan
                   MOV ES, DX
                   MOV DI, AX
                   MOV AL, BYTE PTR [BP + 06h]             ; s_radix
                   AND AL, 0Fh
                   CMP AL, 01h
                   JNE SHORT L_LtoA_0D
                   MOV AL, 'b'
                   JMP SHORT L_LtoA_10
L_LtoA_0D          LABEL SHORT
                   CMP AL, 07h
                   JNE SHORT L_LtoA_0E
                   MOV AL, 'o'
                   JMP SHORT L_LtoA_10
L_LtoA_0E          LABEL SHORT
                   CMP AL, 09h
                   JNE SHORT L_LtoA_0F
                   MOV AL, 'd'
                   JMP SHORT L_LtoA_10
L_LtoA_0F          LABEL SHORT
                   CMP AL, 0Fh
                   JNE SHORT L_LtoA_12
                   MOV AL, 'h'                   
L_LtoA_10          LABEL SHORT
                   TEST BYTE PTR [BP + 06h], 10h ; Decimal character
                   JZ SHORT L_LtoA_11
                   XOR AH, AH
                   PUSH AX
                   CALL FAR PTR CODE_TO:ToUpper
L_LtoA_11          LABEL SHORT
                   STOSB
                   XOR AL, AL
                   STOSB
L_LtoA_12          LABEL SHORT
                   TEST BYTE PTR [BP + 06h], 80h ; ()
                   JZ SHORT L_LtoA_13
                   PUSH WORD PTR [BP + 10h]                ; s_string high
                   PUSH WORD PTR [BP + 0Eh]                ; s_string low
                   PUSH '('
                   PUSH 0001h
                   CALL FAR PTR CODE_STRING:CharInsert
                   PUSH WORD PTR [BP + 10h]                ; s_string high
                   PUSH WORD PTR [BP + 0Eh]                ; s_string low
                   PUSH ')'
                   PUSH 0001h
                   CALL FAR PTR CODE_STRING:CharAddtion                   
L_LtoA_13          LABEL SHORT
                   TEST BYTE PTR [BP + 09h], 060h ; Last comma/space & LineFeed
                   JZ SHORT L_LtoA_16
                   PUSH WORD PTR [BP + 10h]                ; s_string high
                   PUSH WORD PTR [BP + 0Eh]                ; s_string low
                   PUSH DEF_ASCII_EndOfString
                   PUSH DEF_STRING_Max
                   CALL FAR PTR CODE_MEMORY:MemScan
                   MOV ES, DX
                   MOV DI, AX
                   TEST BYTE PTR [BP + 09h], 020h ; Last comma/space
                   JZ SHORT L_LtoA_14
                   MOV AX, ',' OR (DEF_ASCII_Space SHL 08h)
                   STOSW
L_LtoA_14          LABEL SHORT
                   TEST BYTE PTR [BP + 09h], 040h ; Line feed
                   JZ SHORT L_LtoA_15
                   MOV AX, 0A0Dh
                   STOSW
L_LtoA_15          LABEL SHORT
                   XOR AL, AL
                   STOSB
L_LtoA_16          LABEL SHORT
                   POPA
                   POP ES
                   POP DS
                   POP BP
                   RETF 4 + 4 + 2 + 2
D_LtoA_Buffer      DW DEF_NULL, DEF_NULL
D_LtoA_Flag        DW DEF_NULL
LtoA               ENDP
ItoA               PROC FAR ; void far pascal LtoA(void far *s_dest, unsigned int s_value, unsigned int s_option0, unsigned int s_option1)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   MOV AX, WORD PTR [BP + 0Ah]
                   XOR DX, DX
                   TEST BYTE PTR [BP + 08h], 01h           ; option0 - sign
                   JZ SHORT L_ItoA_00
                   CWD
L_ItoA_00          LABEL SHORT
                   PUSH WORD PTR [BP + 0Eh]
                   PUSH WORD PTR [BP + 0Ch]
                   PUSH DX
                   PUSH AX
                   PUSH WORD PTR [BP + 08h]
                   PUSH WORD PTR [BP + 06h]
                   CALL FAR PTR CODE_TO:LtoA
                   POP DX
                   POP AX
                   POP BP
                   RETF 4 + 2 + 2 + 2
ItoA               ENDP
AtoL               PROC FAR ; unsigned long far pascal AtoL(void far *s_src, unsigned int s_radix)
                   PUSH BP
                   MOV BP, SP
                   SUB SP, 0004h + 0004h + 0002h
                   PUSH DS
                   PUSH SI
                   PUSH CX
                   XOR AX, AX
                   MOV BYTE PTR [BP - 0009h], AL
                   MOV WORD PTR [BP - 0008h], 0001h
                   MOV WORD PTR [BP - 0006h], AX
                   MOV WORD PTR [BP - 0004h], AX
                   MOV WORD PTR [BP - 0002h], AX
                   CMP WORD PTR [BP + 0006h], AX
                   JZ NEAR PTR L_AtoL_NoProcess
                   LDS SI, DWORD PTR [BP + 08h]            ; s_src
                   CLD
L_AtoL_Loop_SPC    LABEL SHORT
                   LODSB
                   CMP AL, DEF_ASCII_Space
                   JE SHORT L_AtoL_Loop_SPC
                   CMP AL, '-'
                   JNE SHORT L_AtoL_NoMinus
                   INC BYTE PTR [BP - 0009h]
L_AtoL_Loop_Null   LABEL SHORT
                   LODSB
L_AtoL_NoMinus     LABEL SHORT
                   OR AL, AL                               ; If AL == 0 then Jump
                   JZ SHORT L_AtoL_Return
                   CMP AL, '0'
                   JE SHORT L_AtoL_Loop_Null
                   DEC SI
                   PUSH DS
                   PUSH SI                   
                   CALL FAR PTR CODE_STRING:StrLenN
                   MOV CX, AX
                   JCXZ SHORT L_AtoL_Return
L_AtoL_Loop        LABEL SHORT
                   PUSH SI
                   ADD SI, CX
                   DEC SI
                   MOV AL, BYTE PTR DS:[SI]
                   PUSH AX
                   CALL FAR PTR CODE_TO:ToNum
                   XOR EDX, EDX
                   MOV DX, AX                   
                   MOV EAX, DWORD PTR [BP - 0008h]
                   MUL EDX
                   ADD DWORD PTR [BP - 0004h], EAX
                   XOR EDX, EDX
                   MOV DX, WORD PTR [BP + 06h]             ; s_radix
                   MOV EAX, DWORD PTR [BP - 0008h]
                   MUL EDX
                   MOV DWORD PTR [BP - 0008h], EAX
                   POP SI                   
                   LOOP SHORT L_AtoL_Loop                   
L_AtoL_Return      LABEL SHORT
                   MOV EAX, DWORD PTR [BP - 0004h]                   
                   TEST BYTE PTR [BP - 0009h], 01h
                   JZ SHORT L_AtoL_NRMinus
                   NEG EAX
L_AtoL_NRMinus     LABEL SHORT
                   MOV EDX, EAX
                   SHR EDX, 0010h
L_AtoL_NoProcess   LABEL SHORT
                   POP CX
                   POP SI
                   POP DS
                   MOV SP, BP
                   POP BP
                   RETF 4 + 2
AtoL               ENDP
AtoI               PROC FAR ; unsigned int far pascal AtoI(void far *s_src, unsigned int s_radix)
                   PUSH BP
                   MOV BP, SP
                   PUSH EDX
                   PUSH WORD PTR [BP + 08h + DEF_Far_Segment] ; s_src high
                   PUSH WORD PTR [BP + 08h + DEF_Far_Offset] ; s_src low
                   PUSH WORD PTR [BP + 06h]                ; s_radix
                   CALL FAR PTR CODE_TO:AtoL
                   POP EDX
                   POP BP
                   RETF 4 + 2
AtoI               ENDP
CODE_TO            ENDS

                   ASSUME CS:CODE_TO, DS:DATA_TO, ES:NOTHING, SS:STACK_DEFAULT
DATA_TO            SEGMENT
DATA_TO            ENDS

                   ASSUME CS:CODE_TO, DS:BSS_TO, ES:NOTHING, SS:STACK_DEFAULT
BSS_TO             SEGMENT
BSS_TO             ENDS
                   END

; End of source
