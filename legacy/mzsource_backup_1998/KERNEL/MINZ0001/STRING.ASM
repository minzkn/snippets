COMMENT #
    
#

DEF_ASM_STRING     EQU "STRING.ASM"

INCLUDE            COMMON.INC

DEF_VSPrintF_TempValue = 0002h + 0002h + 0002h + 0002h + 000Ch
DEF_VSPrintF_Option00  = 0002h
DEF_VSPrintF_Option01  = 0004h
DEF_VSPrintF_FillCount = 0006h
DEF_VSPrintF_Long      = 0008h
DEF_VSPrintF_Buffer    = 0014h

PUBLIC             StrLenC, StrLenN, StrLen
PUBLIC             StrCpyN, StrCpy
PUBLIC             StrCatN, StrCat
PUBLIC             StrCmpN, StrCmp
PUBLIC             StrSetN, StrSet
PUBLIC             StrUpr, StrLwr
PUBLIC             StrInsert, StrInsertN, CharInsert, CharAddtion, CharAlign, GridInsert
PUBLIC             StrOptimize
PUBLIC             VSPrintF
PUBLIC             SPrintF, _SPrintF

                   ASSUME CS:CODE_STRING, DS:DATA_STRING, ES:NOTHING, SS:STACK_DEFAULT
CODE_STRING        SEGMENT
StrLenC            PROC FAR ; unsigned int far pascal StrLenC(void far *s_string, unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH SI
                   PUSH CX
                   LDS SI, DWORD PTR [BP + 08h]            ; s_string
                   XOR CX, CX
                   MOV AH, BYTE PTR [BP + 06h]             ; s_character
                   CLD
L_StrLenC_00       LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT L_StrLenC_01
                   CMP AL, AH
                   JZ SHORT L_StrLenC_01
                   INC CX
                   JMP SHORT L_StrLenC_00
L_StrLenC_01       LABEL SHORT
                   MOV AX, CX
                   POP CX
                   POP SI
                   POP DS
                   POP BP
                   RETF 4 + 2
StrLenC            ENDP
StrLenN            PROC FAR ; unsigned int far pascal StrLenN(void far *s_string)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH SI
                   PUSH CX
                   MOV CX, DEF_STRING_Max
                   LDS SI, DWORD PTR [BP + 06h]            ; s_string                   
                   CLD
L_StrLenN_00       LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT L_StrLenN_01
                   PUSH AX
                   CALL FAR PTR CODE_IS:IsHex
                   OR AL, AL
                   LOOPNZ SHORT L_StrLenN_00
L_StrLenN_01       LABEL SHORT
                   MOV AX, CX
                   NOT AX                   
                   POP CX
                   POP SI
                   POP DS
                   POP BP
                   RETF 4
StrLenN            ENDP
StrLen             PROC FAR ; unsigned int far pascal StrLen(void far *s_string)
                   PUSH BP
                   MOV BP, SP
                   PUSH ES
                   PUSH DI
                   PUSH CX
                   MOV CX, DEF_STRING_Max
                   LES DI, DWORD PTR [BP + 06h]            ; s_string
                   XOR AL, AL
                   CLD
                   REPNZ SCASB
                   MOV AX, CX
                   INC AX
                   NOT AX
                   POP CX
                   POP DI
                   POP ES
                   POP BP
                   RETF 4
StrLen             ENDP
StrCpyN            PROC FAR ; void far pascal StrCpyN(void far *s_dest, void far *s_src, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH AX
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   LDS SI, DWORD PTR [BP + 08h]            ; s_src
                   LES DI, DWORD PTR [BP + 0Ch]            ; s_dest
                   CLD
L_StrCpyN_00       LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT L_StrCpyN_01
                   STOSB
                   LOOP SHORT L_StrCpyN_00
                   XOR AL, AL  
L_StrCpyN_01       LABEL SHORT
                   STOSB
                   POP CX
                   POP AX
                   POP DI
                   POP SI
                   POP ES
                   POP DS
                   POP BP
                   RETF 4 + 4 + 2
StrCpyN            ENDP
StrCpy             PROC FAR ; void far pascal StrCpy(void far *s_dest, void far *s_src)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH AX
                   LES DI, DWORD PTR [BP + 0Ah]            ; s_dest
                   LDS SI, DWORD PTR [BP + 06h]            ; s_src
                   CLD
L_StrCpy_00        LABEL SHORT
                   LODSB
                   STOSB
                   OR AL, AL
                   JNZ SHORT L_StrCpy_00
                   POP AX
                   POP DI
                   POP SI
                   POP ES
                   POP DS
                   POP BP
                   RETF 4 + 4
StrCpy             ENDP
StrCatN            PROC FAR ; void far pascal StrCatN(void far *s_dest, void far *s_src, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   PUSH WORD PTR [BP + 0Ch + DEF_Far_Segment] ; s_dest high
                   PUSH WORD PTR [BP + 0Ch + DEF_Far_Offset] ; s_dest low
                   PUSH DEF_ASCII_EndOfString
                   PUSH DEF_STRING_Max
                   CALL FAR PTR CODE_MEMORY:MemScan
                   PUSH DX
                   PUSH AX
                   PUSH WORD PTR [BP + 08h + DEF_Far_Segment] ; s_src high
                   PUSH WORD PTR [BP + 08h + DEF_Far_Offset] ; s_src low
                   PUSH WORD PTR [BP + 06h]
                   CALL FAR PTR CODE_STRING:StrCpyN
                   POP DX
                   POP AX
                   POP BP
                   RETF 4 + 4 + 2
StrCatN            ENDP
StrCat             PROC FAR ; void far pascal StrCat(void far *s_dest, void far *s_src)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Segment] ; s_dest high
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Offset] ; s_dest low
                   PUSH DEF_ASCII_EndOfString
                   PUSH DEF_STRING_Max
                   CALL FAR PTR CODE_MEMORY:MemScan
                   PUSH DX
                   PUSH AX
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_src high
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset] ; s_src low
                   CALL FAR PTR CODE_STRING:StrCpy
                   POP DX
                   POP AX
                   POP BP
                   RETF 4 + 4 
StrCat             ENDP
StrCmpN            PROC FAR ; unsigned int far pascal StrCmpN(void far *s_dest, void far *s_src, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH CX
                   LES DI, DWORD PTR [BP + 0Ch]             ; s_dest
                   MOV SI, DI
                   MOV CX, DEF_STRING_Max
                   XOR AL, AL
                   CLD
                   REPNZ SCASB
                   NOT CX
                   MOV DI, SI
                   LDS SI, DWORD PTR [BP + 08h]            ; s_src
                   MOV CX, AX
                   INC CX
                   CMP CX, WORD PTR [BP + 06h]             ; s_length
                   JBE SHORT L_StrCmpN_00
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
L_StrCmpN_00       LABEL SHORT
                   REPZ CMPSB
                   MOV AL, BYTE PTR DS:[SI - 0001h]
                   XOR AH, AH
                   MOV CL, BYTE PTR ES:[DI - 0001h]
                   XOR CH, CH
                   SUB AX, CX
                   POP CX
                   POP DI
                   POP SI
                   POP ES
                   POP DS
                   POP BP
                   RETF 4 + 4 + 2
                   ; Return :
                   ;  < 0 if dest is less than src
                   ;  = 0 if dest is the same as src
                   ;  > 0 if src is greater than dest
StrCmpN            ENDP
StrCmp             PROC FAR ; unsigned int far pascal StrCmp(void far *s_dest, void far *s_src)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Segment] ; s_dest high
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Offset] ; s_dest low
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_src high
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset] ; s_src low
                   PUSH DEF_STRING_Max
                   CALL FAR PTR CODE_STRING:StrCmpN
                   POP BP
                   RETF 4 + 4 
                   ; Return :
                   ;  < 0 if dest is less than src
                   ;  = 0 if dest is the same as src
                   ;  > 0 if src is greater than dest
StrCmp             ENDP
StrSetN            PROC FAR ; void far pascal StrSetN(void far *s_string, unsigned int s_character, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Segment] ; s_string high
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Offset] ; s_string low
                   CALL FAR PTR CODE_STRING:StrLen
                   CMP AX, WORD PTR [BP + 06h]             ; s_length
                   JBE SHORT L_StrSetN_00
                   MOV AX, WORD PTR [BP + 06h]             ; s_length
L_StrSetN_00       LABEL SHORT
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Segment] ; s_string high
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Offset] ; s_string low
                   PUSH WORD PTR [BP + 08h]                ; s_character
                   PUSH AX
                   CALL FAR PTR CODE_MEMORY:MemSet
                   POP AX
                   POP BP
                   RETF 4 + 2 + 2
StrSetN            ENDP
StrSet             PROC FAR ; void far pascal StrSet(void far *s_string, unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 08h + DEF_Far_Segment] ; s_string high
                   PUSH WORD PTR [BP + 08h + DEF_Far_Offset] ; s_string low
                   PUSH WORD PTR [BP + 06h]                ; s_character
                   PUSH DEF_STRING_Max
                   CALL FAR PTR CODE_STRING:StrSetN
                   POP BP
                   RETF 4 + 2
StrSet             ENDP
StrUpr             PROC FAR ; void far pascal StrUpr(void far *s_string)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH AX
                   LDS SI, DWORD PTR [BP + 06h]            ; s_string
                   LES DI, DWORD PTR [BP + 06h]            ; s_string
                   CLD
L_StrUpr_00        LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT L_StrUpr_01
                   PUSH AX
                   CALL FAR PTR CODE_TO:ToUpper
                   STOSB
                   JMP SHORT L_StrUpr_00
L_StrUpr_01        LABEL SHORT
L_StrLwr_01        LABEL SHORT
                   POP AX
                   POP DI
                   POP SI
                   POP ES
                   POP DS
                   POP BP
                   RETF 4
StrUpr             ENDP
StrLwr             PROC FAR ; void far pascal StrLwr(void far *s_string)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH AX
                   LDS SI, DWORD PTR [BP + 06h]            ; s_string
                   PUSH DS
                   POP ES
                   MOV DI, SI
                   CLD
L_StrLwr_00        LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT L_StrLwr_01
                   PUSH AX
                   CALL FAR PTR CODE_TO:ToLower
                   STOSB
                   JMP SHORT L_StrLwr_00
StrLwr             ENDP
StrInsert          PROC FAR ; void far pascal StrInsert(void far *s_string, void far *s_istring)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   PUSH WORD PTR [BP + 0Ch]                ; s_string high
                   PUSH WORD PTR [BP + 0Ah]                ; s_string low
                   CALL FAR PTR CODE_STRING:StrLen
                   INC AX
                   PUSH AX
                   CALL FAR PTR CODE_HEAP:FarAlloc
                   PUSH DX
                   PUSH AX
                   PUSH WORD PTR [BP + 0Ch]                ; s_string high
                   PUSH WORD PTR [BP + 0Ah]                ; s_string low
                   PUSH DX
                   PUSH AX
                   PUSH DX
                   PUSH AX
                   PUSH WORD PTR [BP + 0Ch]                ; s_string high
                   PUSH WORD PTR [BP + 0Ah]                ; s_string low
                   CALL FAR PTR CODE_STRING:StrCpy
                   PUSH WORD PTR [BP + 0Ch]                ; s_string high
                   PUSH WORD PTR [BP + 0Ah]                ; s_string low
                   PUSH WORD PTR [BP + 08h]                ; s_istring high
                   PUSH WORD PTR [BP + 06h]                ; s_istring low
                   CALL FAR PTR CODE_STRING:StrCpy
                   CALL FAR PTR CODE_STRING:StrCat
                   CALL FAR PTR CODE_HEAP:FarFree
                   POP DX
                   POP AX
                   POP BP
                   RETF 4 + 4
StrInsert          ENDP
StrInsertN         PROC FAR ; void far pascal StrInsertN(void far *s_string, void far *s_istring, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   PUSH WORD PTR [BP + 0Eh]                ; s_string high
                   PUSH WORD PTR [BP + 0Ch]                ; s_string low
                   CALL FAR PTR CODE_STRING:StrLen
                   INC AX
                   PUSH AX
                   CALL FAR PTR CODE_HEAP:FarAlloc
                   PUSH DX
                   PUSH AX
                   PUSH WORD PTR [BP + 0Eh]                ; s_string high
                   PUSH WORD PTR [BP + 0Ch]                ; s_string low
                   PUSH DX
                   PUSH AX
                   PUSH DX
                   PUSH AX
                   PUSH WORD PTR [BP + 0Eh]                ; s_string high
                   PUSH WORD PTR [BP + 0Ch]                ; s_string low
                   CALL FAR PTR CODE_STRING:StrCpy
                   PUSH WORD PTR [BP + 0Eh]                ; s_string high
                   PUSH WORD PTR [BP + 0Ch]                ; s_string low
                   PUSH WORD PTR [BP + 0Ah]                ; s_istring high
                   PUSH WORD PTR [BP + 08h]                ; s_istring low
                   PUSH WORD PTR [BP + 06h]                ; s_length
                   CALL FAR PTR CODE_STRING:StrCpyN
                   CALL FAR PTR CODE_STRING:StrCat
                   CALL FAR PTR CODE_HEAP:FarFree
                   POP DX
                   POP AX
                   POP BP
                   RETF 4 + 4 + 2
StrInsertN         ENDP
CharInsert         PROC FAR ; void far pascal CharInsert(void far *s_string, unsigned int s_byte, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH CX
                   PUSH DX
                   PUSH WORD PTR [BP + 0Ch]                ; s_string high
                   PUSH WORD PTR [BP + 0Ah]                ; s_string low
                   CALL FAR PTR CODE_STRING:StrLen
                   INC AX
                   PUSH AX
                   CALL FAR PTR CODE_HEAP:FarAlloc
                   PUSH DX
                   PUSH AX
                   PUSH DX
                   PUSH AX
                   PUSH WORD PTR [BP + 0Ch]                ; s_string high
                   PUSH WORD PTR [BP + 0Ah]                ; s_string low
                   CALL FAR PTR CODE_STRING:StrCpy
                   PUSH WORD PTR [BP + 0Ch]                ; s_string high
                   PUSH WORD PTR [BP + 0Ah]                ; s_string low
                   PUSH WORD PTR [BP + 08h]                ; s_byte
                   PUSH WORD PTR [BP + 06h]                ; s_length
                   CALL FAR PTR CODE_MEMORY:MemSet
                   PUSH WORD PTR [BP + 0Ch]                ; s_string high
                   MOV CX, WORD PTR [BP + 0Ah]             ; s_string low
                   ADD CX, WORD PTR [BP + 06h]             ; s_length
                   PUSH CX
                   PUSH DX
                   PUSH AX
                   CALL FAR PTR CODE_STRING:StrCpy
                   CALL FAR PTR CODE_HEAP:FarFree
                   POP DX
                   POP CX
                   POP AX
                   POP BP
                   RETF 4 + 2 + 2
CharInsert         ENDP
CharAddtion        PROC FAR ; void far pascal CharAddtion(void far *s_string, unsigned int s_byte, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH ES
                   PUSH DI
                   PUSH AX
                   PUSH CX
                   PUSH DX
                   PUSH WORD PTR [BP + 0Ch]                ; s_string high
                   PUSH WORD PTR [BP + 0Ah]                ; s_string low
                   PUSH DEF_ASCII_EndOfString
                   PUSH DEF_STRING_Max
                   CALL FAR PTR CODE_MEMORY:MemScan
                   MOV ES, DX
                   MOV DI, AX
                   MOV AL, BYTE PTR [BP + 08h]             ; s_byte
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   JCXZ SHORT L_CharAddtion_00
                   CLD
                   REPZ STOSB
                   XOR AL, AL
                   STOSB
L_CharAddtion_00   LABEL SHORT
                   POP DX
                   POP CX
                   POP AX
                   POP DI
                   POP ES
                   POP BP
                   RETF 4 + 2 + 2
CharAddtion        ENDP
CharAlign          PROC FAR ; void far pascal CharAlign(void far *s_string, unsigned int s_byte, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX                   
                   MOV AX, WORD PTR [BP + 06h]             ; s_length
                   SHR AX, 01h
                   PUSH WORD PTR [BP + 0Ch]                ; s_string high
                   PUSH WORD PTR [BP + 0Ah]                ; s_string low
                   PUSH WORD PTR [BP + 08h]                ; s_byte
                   PUSH AX
                   CALL FAR PTR CODE_STRING:CharInsert
                   SUB WORD PTR [BP + 06h], AX             ; s_length
                   PUSH WORD PTR [BP + 0Ch]                ; s_string high
                   PUSH WORD PTR [BP + 0Ah]                ; s_string low
                   PUSH WORD PTR [BP + 08h]                ; s_byte
                   PUSH WORD PTR [BP + 06h]                ; s_length
                   CALL FAR PTR CODE_STRING:CharAddtion
                   POP AX
                   POP BP
                   RETF 4 + 2 + 2
CharAlign          ENDP
GridInsert         PROC FAR ; void far pascal GridInsert(void far *s_string, unsigned int s_byte, unsigned int s_grid)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSHA
                   LES DI, DWORD PTR [BP + 0Ah]            ; s_string
                   PUSH ES
                   PUSH DI
                   CALL FAR PTR CODE_STRING:StrLen                                      
                   CMP AX, WORD PTR [BP + 06h]             ; s_grid
                   JBE SHORT L_GridInsert_03                   
                   MOV CX, AX
                   INC AX
                   PUSH AX
                   CALL FAR PTR CODE_HEAP:FarAlloc
                   PUSH DX
                   PUSH AX
                   MOV DS, DX
                   MOV SI, AX
                   PUSH DX
                   PUSH AX
                   PUSH ES
                   PUSH DI
                   CALL FAR PTR CODE_STRING:StrCpy
                   CLD
                   JMP SHORT L_GridInsert_01
L_GridInsert_00    LABEL SHORT
                   MOV AX, CX
                   XOR DX, DX
                   DIV WORD PTR [BP + 06h]
                   OR DX, DX
                   JNZ SHORT L_GridInsert_01
                   MOV AL, BYTE PTR [BP + 08h]             ; s_byte
                   STOSB
L_GridInsert_01    LABEL SHORT
                   MOVSB
                   LOOP SHORT L_GridInsert_00
                   XOR AL, AL
                   STOSB
L_GridInsert_02    LABEL SHORT
                   CALL FAR PTR CODE_HEAP:FarFree
L_GridInsert_03    LABEL SHORT
                   POPA
                   POP ES
                   POP DS
                   POP BP
                   RETF 4 + 2 + 2
GridInsert         ENDP
StrOptimize        PROC ; void far pascal StrOptimize(void far *s_string, unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH AX
                   LDS SI, DWORD PTR [BP + 08h]
                   CLD
L_StrOptimize_00   LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT L_StrOptimize_04
                   CMP AL, BYTE PTR [BP + 06h + DEF_Word_Low] ; s_character
                   JE SHORT L_StrOptimize_00                   
                   DEC SI
                   PUSH DS
                   PUSH SI
L_StrOptimize_01   LABEL SHORT
                   LODSB
                   OR AL, AL
                   JNZ SHORT L_StrOptimize_01
                   DEC SI
L_StrOptimize_02   LABEL SHORT
                   DEC SI
                   MOV AL, BYTE PTR DS:[SI]                   
                   CMP AL, BYTE PTR [BP + 06h + DEF_Word_Low] ; s_character
                   JE SHORT L_StrOptimize_02
                   MOV BYTE PTR DS:[SI + 0001h], DEF_ASCII_EndOfString
                   POP SI
                   POP DS
                   LES DI, DWORD PTR [BP + 08h]            ; s_string
L_StrOptimize_03   LABEL SHORT
                   LODSB
                   STOSB
                   OR AL, AL
                   JNZ SHORT L_StrOptimize_03
L_StrOptimize_04   LABEL SHORT
                   POP AX
                   POP DI
                   POP SI
                   POP ES
                   POP DS
                   POP BP
                   RETF 4 + 2
StrOptimize        ENDP
VSPrintF_Type      PROC NEAR ; AH near pascal VSPrintF_Type(AL)
                   ; {$}{+*-}{<_>}{,}{0 }{fillcount}{l}{Typr} Null
                   ;  1   2    3   4   5      6      7    8    0
                   XOR AH, AH
                   OR AL, AL
                   JZ SHORT L_VSPrintF_Type_Return
                   CMP AL, '+'
                   JE SHORT L_VSPrintF_Type_00
                   CMP AL, '*'
                   JE SHORT L_VSPrintF_Type_00
                   CMP AL, '-'
                   JNE SHORT L_VSPrintF_Type_01
L_VSPrintF_Type_00 LABEL SHORT
                   MOV AH, 02h
                   JMP SHORT L_VSPrintF_Type_Return
L_VSPrintF_Type_01 LABEL SHORT
                   CMP AL, '<'
                   JE SHORT L_VSPrintF_Type_02
                   CMP AL, '_'
                   JE SHORT L_VSPrintF_Type_02
                   CMP AL, '>'
                   JNE SHORT L_VSPrintF_Type_03
L_VSPrintF_Type_02 LABEL SHORT
                   MOV AH, 03h
                   JMP SHORT L_VSPrintF_Type_Return
L_VSPrintF_Type_03 LABEL SHORT
                   CMP AL, ','
                   JNE SHORT L_VSPrintF_Type_04
                   MOV AH, 04h
                   JMP SHORT L_VSPrintF_Type_Return
L_VSPrintF_Type_04 LABEL SHORT
                   CMP AL, ' '
                   JE SHORT L_VSPrintF_Type_05
                   CMP AL, '0'
                   JNE SHORT L_VSPrintF_Type_06
L_VSPrintF_Type_05 LABEL SHORT
                   MOV AH, 05h
                   JMP SHORT L_VSPrintF_Type_Return
L_VSPrintF_Type_06 LABEL SHORT
                   CMP AL, 'l'
                   JE SHORT L_VSPrintF_Type_07
                   CMP AL, 'L'
                   JNE SHORT L_VSPrintF_Type_08
L_VSPrintF_Type_07 LABEL SHORT
                   MOV AH, 07h
                   JMP SHORT L_VSPrintF_Type_Return
L_VSPrintF_Type_08 LABEL SHORT
                   PUSH BX
                   PUSH AX
                   PUSH AX
                   CALL FAR PTR CODE_IS:IsDigit
                   OR AL, AL
                   JZ SHORT L_VSPrintF_Type_09
                   MOV AH, 06h
                   JMP SHORT L_VSPrintF_Type_0A
L_VSPrintF_Type_09 LABEL SHORT
                   MOV AH, 08h
L_VSPrintF_Type_0A LABEL SHORT
                   POP BX
                   MOV AL, BL
                   POP BX                   
L_VSPrintF_Type_Return LABEL SHORT
                   RETN
VSPrintF_Type      ENDP
VSPrintF_Format    PROC NEAR ; void near pascal VSPrintF(void)
                   PUSH BP
                   MOV BP, SP                   
                   SUB SP, DEF_VSPrintF_TempValue                   
                   CMP AL, DEF_VSPrintF_Format_Character
                   JNE SHORT L_VSPrintF_Format_00
                   STOSB
                   JMP NEAR PTR L_VSPrintF_Format_Return
L_VSPrintF_Format_00 LABEL SHORT
                   MOV WORD PTR [BP - DEF_VSPrintF_Option00], DEF_Null
                   MOV WORD PTR [BP - DEF_VSPrintF_Option01], DEF_Null
                   MOV WORD PTR [BP - DEF_VSPrintF_FillCount], DEF_Null
                   MOV WORD PTR [BP - DEF_VSPrintF_Long], DEF_Null
L_VSPrintF_Format_01 LABEL SHORT
                   CALL NEAR PTR CODE_STRING:VSPrintF_Type
                   OR AH, AH
                   JNZ SHORT L_VSPrintF_Format_NReturn
                   JMP NEAR PTR L_VSPrintF_Format_Return
L_VSPrintF_Format_NReturn LABEL SHORT
                   CMP AH, 08h 
                   JNE SHORT L_VSPrintF_Format_N0C                   
                   JMP NEAR PTR L_VSPrintF_Format_0C
L_VSPrintF_Format_N0C LABEL SHORT
                   CMP AH, 02h ; {+*-}
                   JNE SHORT L_VSPrintF_Format_04
                    CMP AL, '-'
                    JNE SHORT L_VSPrintF_Format_02
                     OR BYTE PTR [BP - DEF_VSPrintF_Option00], 01h
                     JMP NEAR PTR L_VSPrintF_Format_0B
                    L_VSPrintF_Format_02 LABEL SHORT
                    CMP AL, '+'
                    JNE SHORT L_VSPrintF_Format_03
                     OR BYTE PTR [BP - DEF_VSPrintF_Option00], 02h
                     JMP NEAR PTR L_VSPrintF_Format_0B
                    L_VSPrintF_Format_03 LABEL SHORT
                    CMP AL, '*'
                    JNE SHORT L_VSPrintF_Format_04
                     OR BYTE PTR [BP - DEF_VSPrintF_Option00], 03h
                     JMP NEAR PTR L_VSPrintF_Format_0B
L_VSPrintF_Format_04 LABEL SHORT
                   CMP AH, 03h ; {<_>}
                   JNE SHORT L_VSPrintF_Format_07
                    CMP AL, '<'
                    JNE SHORT L_VSPrintF_Format_05
                     OR BYTE PTR [BP - DEF_VSPrintF_Option00], 01h SHL 02h                     
                     JMP NEAR PTR L_VSPrintF_Format_0B
                    L_VSPrintF_Format_05 LABEL SHORT
                    CMP AL, '>'
                    JNE SHORT L_VSPrintF_Format_06
                     OR BYTE PTR [BP - DEF_VSPrintF_Option00], 02h SHL 02h
                     JMP NEAR PTR L_VSPrintF_Format_0B
                    L_VSPrintF_Format_06 LABEL SHORT
                    CMP AL, '_'
                    JNE SHORT L_VSPrintF_Format_07
                     OR BYTE PTR [BP - DEF_VSPrintF_Option00], 03h SHL 02h
                     JMP NEAR PTR L_VSPrintF_Format_0B
L_VSPrintF_Format_07 LABEL SHORT
                   CMP AH, 04h ; {,}
                   JNE SHORT L_VSPrintF_Format_08
                    OR BYTE PTR [BP - DEF_VSPrintF_Option00], 03h SHL 04h
                    JMP NEAR PTR L_VSPrintF_Format_0B
L_VSPrintF_Format_08 LABEL SHORT
                   CMP AH, 05h ; {0}
                   JNE SHORT L_VSPrintF_Format_09
                    MOV BYTE PTR [BP - DEF_VSPrintF_Option01 + 01h], '0'                    
                    JMP NEAR PTR L_VSPrintF_Format_0B
L_VSPrintF_Format_09 LABEL SHORT
                   CMP AH, 06h ; {fillcount}
                   JNE SHORT L_VSPrintF_Format_0A
                   ; Fill count
                    PUSH ES
                    PUSH DI                    
                    PUSH BX
                    PUSH DX
                    PUSH SS
                    POP ES 
                    LEA DI, BYTE PTR [BP - DEF_VSPrintF_Buffer]
                    PUSH ES
                    PUSH DI
                    STOSB
                   L_VSPrintF_Format_09_00 LABEL SHORT
                    LODSB                    
                    OR AL, AL
                    JZ SHORT L_VSPrintF_Format_09_01
                    MOV BX, AX
                    PUSH AX
                    CALL FAR PTR CODE_IS:IsDigit
                    XCHG BX, AX
                    OR BL, BL
                    JZ SHORT L_VSPrintF_Format_09_01
                    STOSB
                    JMP SHORT L_VSPrintF_Format_09_00
                   L_VSPrintF_Format_09_01 LABEL SHORT
                    MOV BX, AX
                    XOR AL, AL
                    STOSB
                    PUSH 10d ; radix
                    CALL FAR PTR CODE_TO:AtoI                    
                    MOV WORD PTR [BP - DEF_VSPrintF_FillCount], AX                  
                    AND AX, 003Fh
                    SHL AX, 07h                    
                    OR WORD PTR [BP - DEF_VSPrintF_Option00], AX
                    MOV AX, BX
                    POP DX
                    POP BX                    
                    POP DI
                    POP ES
                    JMP NEAR PTR L_VSPrintF_Format_01
L_VSPrintF_Format_0A LABEL SHORT
                   CMP AH, 07h ; {l}
                   JNE SHORT L_VSPrintF_Format_0B
                    OR WORD PTR [BP - DEF_VSPrintF_Long], 0001h
L_VSPrintF_Format_0B LABEL SHORT
                   LODSB
                   JMP NEAR PTR L_VSPrintF_Format_01
L_VSPrintF_Format_0C LABEL SHORT                   
                   PUSH AX
                   PUSH AX
                   PUSH AX
                   CALL FAR PTR CODE_IS:IsLower
                   OR AL, AL
                   JZ SHORT L_VSPrintF_Format_0D
                    OR BYTE PTR [BP - DEF_VSPrintF_Option01], 20h
L_VSPrintF_Format_0D LABEL SHORT
                   POP AX
                   CMP AL, 'c'
                   JNE SHORT L_VSPrintF_Format_0E
L_VSPrintF_Format_c LABEL SHORT
                   CALL NEAR PTR CODE_STRING:VSPrintF_c
                   JMP NEAR PTR L_VSPrintF_Format_FF
L_VSPrintF_Format_0E LABEL SHORT
                   CMP AL, 'x'
                   JNE SHORT L_VSPrintF_Format_11
L_VSPrintF_Format_x LABEL SHORT
                    OR BYTE PTR [BP - DEF_VSPrintF_Option01], 0Fh
                   JMP SHORT L_VSPrintF_Format_n
L_VSPrintF_Format_b LABEL SHORT
                    OR BYTE PTR [BP - DEF_VSPrintF_Option01], 01h
                   JMP SHORT L_VSPrintF_Format_n
L_VSPrintF_Format_o LABEL SHORT
                    OR BYTE PTR [BP - DEF_VSPrintF_Option01], 07h
                   JMP SHORT L_VSPrintF_Format_n
L_VSPrintF_Format_u LABEL SHORT
                    OR BYTE PTR [BP - DEF_VSPrintF_Option01], 09h
L_VSPrintF_Format_n LABEL SHORT
                   PUSH WORD PTR [BP - DEF_VSPrintF_Option00]
                   PUSH WORD PTR [BP - DEF_VSPrintF_Option01]
                   TEST BYTE PTR [BP - DEF_VSPrintF_Long], 01h
                   JZ SHORT L_VSPrintF_Format_0F
                   CALL NEAR PTR CODE_STRING:VSPrintF_l
                   JMP SHORT L_VSPrintF_Format_10
L_VSPrintF_Format_0F LABEL SHORT
                   CALL NEAR PTR CODE_STRING:VSPrintF_u
L_VSPrintF_Format_10 LABEL SHORT
                   JMP SHORT L_VSPrintF_Format_FF
L_VSPrintF_Format_11 LABEL SHORT
                   CMP AL, 's'
                   JNE SHORT L_VSPrintF_Format_12
L_VSPrintF_Format_s LABEL SHORT
                   PUSH WORD PTR [BP - DEF_VSPrintF_Option01]
                   PUSH WORD PTR [BP - DEF_VSPrintF_FillCount]
                   CALL NEAR PTR CODE_STRING:VSPrintF_s
L_VSPrintF_Format_12 LABEL SHORT
                   CMP AL, 'C'
                   JE SHORT L_VSPrintF_Format_c
                   CMP AL, 'a'
                   JE SHORT L_VSPrintF_Format_c
                   CMP AL, 'A'
                   JE SHORT L_VSPrintF_Format_c
                   CMP AL, 'X'
                   JE SHORT L_VSPrintF_Format_x
                   CMP AL, 'u'
                   JE SHORT L_VSPrintF_Format_u
                   CMP AL, 'U'
                   JE SHORT L_VSPrintF_Format_u
                   CMP AL, 'd'
                   JE SHORT L_VSPrintF_Format_u
                   CMP AL, 'D'
                   JE SHORT L_VSPrintF_Format_u
                   CMP AL, 'i'
                   JE SHORT L_VSPrintF_Format_u
                   CMP AL, 'I'
                   JE SHORT L_VSPrintF_Format_u
                   CMP AL, 'n'
                   JE SHORT L_VSPrintF_Format_u
                   CMP AL, 'N'
                   JE SHORT L_VSPrintF_Format_u
                   CMP AL, 'b'
                   JE SHORT L_VSPrintF_Format_b
                   CMP AL, 'B'
                   JE SHORT L_VSPrintF_Format_b
                   CMP AL, 'o'
                   JE SHORT L_VSPrintF_Format_o
                   CMP AL, 'O'
                   JE SHORT L_VSPrintF_Format_o
                   CMP AL, 'S'
                   JE SHORT L_VSPrintF_Format_s
L_VSPrintF_Format_FF LABEL SHORT
                   POP AX
L_VSPrintF_Format_Return LABEL SHORT
                   MOV SP, BP
                   POP BP
                   RETN
VSPrintF_Format    ENDP
VSPrintF_s         PROC NEAR ; void near pascal VSPrintF_s(unsigned int s_option0, unsigned int s_fillcount)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH SI
                   PUSH AX
                   LDS SI, DWORD PTR CODE_STRING:D_VSPrintF_varg
                   ADD WORD PTR CODE_STRING:D_VSPrintF_varg[DEF_Far_Offset], DEF_SIZE_Far
                   LDS SI, DWORD PTR DS:[SI]
L_VSPrintF_s_00    LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT L_VSPrintF_s_01
                   STOSB
                   JMP SHORT L_VSPrintF_s_00
L_VSPrintF_s_01    LABEL SHORT
                   POP AX
                   POP SI
                   POP DS
                   POP BP
                   RETN 2 + 2
VSPrintF_s         ENDP
VSPrintF_c         PROC NEAR ; void near pascal VSPrintF_c(void)
                   PUSH DS
                   PUSH SI
                   LDS SI, DWORD PTR CODE_STRING:D_VSPrintF_varg
                   ADD WORD PTR CODE_STRING:D_VSPrintF_varg[DEF_Far_Offset], DEF_SIZE_Word
                   LODSW
                   STOSB
                   POP SI
                   POP DS
                   RETN
VSPrintF_c         ENDP    
VSPrintF_u         PROC NEAR ; void near pascal VSPrintF_u(unsigned int s_option0, unsigned int s_option1)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH SI
                   PUSH 0020h
                   CALL FAR PTR CODE_HEAP:New
                   PUSH DX
                   PUSH AX
                   PUSH DX
                   PUSH AX
                   PUSH DX
                   PUSH AX
                   LDS SI, DWORD PTR CODE_STRING:D_VSPrintF_varg
                   ADD WORD PTR CODE_STRING:D_VSPrintF_varg[DEF_Far_Offset], DEF_SIZE_Word
                   PUSH WORD PTR DS:[SI]
                   PUSH WORD PTR [BP + 06h]                ; s_option0                   
                   PUSH WORD PTR [BP + 04h]                ; s_option1
                   CALL FAR PTR CODE_TO:ItoA
                   POP SI
                   POP DS
L_VSPrintF_u_00    LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT L_VSPrintF_u_01
                   STOSB
                   JMP SHORT L_VSPrintF_u_00
L_VSPrintF_u_01    LABEL SHORT
                   CALL FAR PTR CODE_HEAP:Delete
                   POP SI
                   POP DS
                   POP BP
                   RETN 2 + 2
VSPrintF_u         ENDP
VSPrintF_l         PROC NEAR ; void near pascal VSPrintF_l(unsigned int s_option0, unsigned int s_option1)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH SI
                   PUSH 0040h
                   CALL FAR PTR CODE_HEAP:New
                   PUSH DX
                   PUSH AX
                   PUSH DX
                   PUSH AX
                   PUSH DX
                   PUSH AX
                   LDS SI, DWORD PTR CODE_STRING:D_VSPrintF_varg
                   ADD WORD PTR CODE_STRING:D_VSPrintF_varg[DEF_Far_Offset], DEF_SIZE_DWord
                   PUSH WORD PTR DS:[SI + DEF_DWord_High]
                   PUSH WORD PTR DS:[SI + DEF_DWord_Low]
                   PUSH WORD PTR [BP + 06h]                ; s_option0                   
                   PUSH WORD PTR [BP + 04h]                ; s_option1
                   CALL FAR PTR CODE_TO:LtoA
                   POP SI
                   POP DS
L_VSPrintF_l_00    LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT L_VSPrintF_l_01
                   STOSB
                   JMP SHORT L_VSPrintF_l_00
L_VSPrintF_l_01    LABEL SHORT
                   CALL FAR PTR CODE_HEAP:Delete
                   POP SI
                   POP DS
                   POP BP
                   RETN 2 + 2
VSPrintF_l         ENDP
VSPrintF           PROC FAR ; void far pascal VSPrintF(void far *s_dest, void far *s_format, void far *s_varg)
                   PUSH BP
                   MOV BP, SP                   
                   PUSH DS
                   PUSH ES
                   PUSHA
                   LES DI, DWORD PTR [BP + 0Eh]            ; s_dest
                   LDS SI, DWORD PTR [BP + 0Ah]            ; s_format
                   CLD
                   MOV AX, WORD PTR [BP + 06h + DEF_Far_Segment] ; s_varg high
                   MOV WORD PTR CODE_STRING:D_VSPrintF_varg[DEF_Far_Segment], AX
                   MOV AX, WORD PTR [BP + 06h + DEF_Far_Offset] ; s_varg high
                   MOV WORD PTR CODE_STRING:D_VSPrintF_varg[DEF_Far_Offset], AX
L_VSPrintF_00      LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT L_VSPrintF_03
                   CMP AL, DEF_VSPrintF_Format_Character
                   JNE SHORT L_VSPrintF_01
                   ; ======== V-Argument =========
                   LODSB
                   OR AL, AL
                   JZ SHORT L_VSPrintF_03
                   CALL NEAR PTR CODE_STRING:VSPrintF_Format
                   OR AL, AL
                   JZ SHORT L_VSPrintF_03
                   ; ======== V-Argument =========
                   JMP SHORT L_VSPrintF_02
L_VSPrintF_01      LABEL SHORT
                   STOSB
L_VSPrintF_02      LABEL SHORT
                   JMP SHORT L_VSPrintF_00
L_VSPrintF_03      LABEL SHORT
                   XOR AL, AL
                   STOSB
                   POPA
                   POP ES
                   POP DS                   
                   POP BP
                   RETF 4 + 4 + 4
D_VSPrintF_varg    DW DEF_Null, DEF_Null
VSPrintF           ENDP
_SPrintF           PROC FAR ; void far cdecl _SPrintF(void far *s_dest, void far *s_format, ...)
SPrintF            PROC FAR ; void far cdecl SPrintF(void far *s_dest, void far *s_format, ...)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_dest high
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_dest high
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Segment] ; s_format high
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Offset]  ; s_format low
                   PUSH SS
                   LEA AX, WORD PTR [BP + 0Eh]             ; ...
                   PUSH AX
                   CALL FAR PTR CODE_STRING:VSPrintf
                   POP AX
                   POP BP
                   RETF
SPrintF            ENDP
_SPrintF           ENDP
CODE_STRING        ENDS

                   ASSUME CS:CODE_STRING, DS:DATA_STRING, ES:NOTHING, SS:STACK_DEFAULT
DATA_STRING        SEGMENT
DATA_STRING        ENDS

                   ASSUME CS:CODE_STRING, DS:BSS_STRING, ES:NOTHING, SS:STACK_DEFAULT
BSS_STRING         SEGMENT
BSS_STRING         ENDS
                   END

; End of source
