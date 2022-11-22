COMMENT #

#

DEF_ASM_MEMORY     EQU "MEMORY.ASM"

INCLUDE            COMMON.INC

PUBLIC             Peek, PeekB, PeekW
PUBLIC             Poke, PokeB, PokeW
PUBLIC             Swap, SwapB, SwapW
PUBLIC             MemSet, MemSetB, MemSetW, MemClear
PUBLIC             MemCpy, MemCpyB, MemCpyW
PUBLIC             MemScan, MemScanB, MemScanW
PUBLIC             MemCmp, MemCmpB, MemCmpW
PUBLIC             WhatBit
PUBLIC             OpenBit, CloseBit, ReadBit, WriteBitSet, WriteBitClear

                   ASSUME CS:CODE_MEMORY, DS:DATA_MEMORY, ES:NOTHING, SS:STACK_DEFAULT
CODE_MEMORY        SEGMENT
Peek               PROC FAR ; unsigned int far pascal Peek(void far *s_address)
PeekB              PROC FAR ; unsigned int far pascal PeekB(void far *s_address)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH BX
                   LDS BX, DWORD PTR [BP + 06h]            ; s_address
                   MOV AL, BYTE PTR DS:[BX]
                   XOR AH, AH
L_PeekW_00         LABEL SHORT
                   POP BX
                   POP DS
                   POP BP
                   RETF 4
PeekB              ENDP
PeekW              PROC FAR ; unsigned int far pascal PeekW(void far *s_address)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH BX
                   LDS BX, DWORD PTR [BP + 06h]            ; s_address
                   MOV AX, WORD PTR DS:[BX]
                   MAC_JumpShort L_PeekW_00
PeekW              ENDP
Peek               ENDP
Poke               PROC FAR ; void far pascal Poke(void far *s_address, unsigned int s_byte)
PokeB              PROC FAR ; void far pascal PokeB(void far *s_address, unsigned int s_byte)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH AX
                   PUSH BX
                   LDS BX, DWORD PTR [BP + 08h]            ; s_address
                   MOV AL, BYTE PTR [BP + 06h + DEF_Word_Low] ; s_byte
                   MOV BYTE PTR DS:[BX], AL
L_PokeW_00         LABEL SHORT
                   POP BX
                   POP AX
                   POP DS
                   POP BP
                   RETF 4 + 2
PokeB              ENDP
PokeW              PROC FAR ; void far pascal PokeW(void far *s_address, unsigned int s_word)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH AX
                   PUSH BX
                   LDS BX, DWORD PTR [BP + 08h]            ; s_address
                   MOV AX, WORD PTR [BP + 06h]             ; s_word
                   MOV WORD PTR DS:[BX], AX
                   MAC_JumpShort <L_PokeW_00>
PokeW              ENDP
Poke               ENDP
Swap               PROC FAR ; void far pascal Swap(void far *s_dest, void far *s_src, unsigned int s_length)
SwapB              PROC FAR ; void far pascal SwapB(void far *s_dest, void far *s_src, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH AX
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   JCXZ SHORT L_Swap_01
                   LDS SI, DWORD PTR [BP + 08h]            ; s_src
                   LES DI, DWORD PTR [BP + 0Ch]            ; s_dest                   
L_Swap_00          LABEL SHORT
                   MOV AL, BYTE PTR DS:[SI]
                   MOV AH, BYTE PTR ES:[DI]
                   MOV BYTE PTR DS:[SI], AH
                   MOV BYTE PTR DS:[DI], AL
                   INC SI
                   INC DI
                   LOOP SHORT L_Swap_00
L_Swap_01          LABEL SHORT
L_SwapW_00         LABEL SHORT
                   POP CX
                   POP AX
                   POP DI
                   POP SI
                   POP ES
                   POP DS
                   POP BP
                   RETF 4 + 4 + 2
SwapB              ENDP
SwapW              PROC FAR ; void far pascal SwapW(void far *s_dest, void far *s_src, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH AX
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   JCXZ SHORT L_SwapW_00
                   LDS SI, DWORD PTR [BP + 08h]            ; s_src
                   LES DI, DWORD PTR [BP + 0Ch]            ; s_dest                   
L_SwapW_01         LABEL SHORT
                   PUSH WORD PTR DS:[SI]
                   PUSH WORD PTR ES:[DI]
                   POP WORD PTR DS:[SI]
                   POP WORD PTR ES:[DI]
                   ADD SI, 0002h
                   ADD DI, 0002h
                   LOOP SHORT L_SwapW_01
                   MAC_JumpShort <L_SwapW_00>
SwapW              ENDP
Swap               ENDP
MemSet             PROC FAR ; void far pascal MemSet(void far *s_dest, unsigned int s_byte, unsigned int s_length)
MemSetB            PROC FAR ; void far pascal MemSetB(void far *s_dest, unsigned int s_byte, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH ES
                   PUSH DI
                   PUSH AX
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   JCXZ SHORT L_MemSet_01
                   LES DI, DWORD PTR [BP + 0Ah]            ; s_dest
                   MOV AL, BYTE PTR [BP + 08h + DEF_Word_Low] ; s_byte
                   CLD
                   SHR CX, 01h
                   JNC SHORT L_MemSet_00
                   STOSB
L_MemSet_00        LABEL SHORT
                   JCXZ SHORT L_MemSet_01
                   MOV AH, AL
L_MemSetW_00       LABEL SHORT   
                   REPZ STOSW
L_MemSet_01        LABEL SHORT
L_MemSetW_01       LABEL SHORT   
                   POP CX
                   POP AX
                   POP DI
                   POP ES
                   POP BP
                   RETF 4 + 2 + 2
MemSetB            ENDP
MemSetW            PROC FAR ; void far pascal MemSetW(void far *s_dest, unsigned int s_word, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH ES
                   PUSH DI
                   PUSH AX
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   JCXZ SHORT L_MemSetW_01
                   LES DI, DWORD PTR [BP + 0Ah]            ; s_dest
                   MOV AX, WORD PTR [BP + 08h]             ; s_word
                   CLD
                   MAC_JumpShort <L_MemSetW_00>
MemSetW            ENDP
MemClear           PROC FAR ; void far pascal MemClear(void far *s_dest, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 0Ah]                ; s_dest high
                   PUSH WORD PTR [BP + 08h]                ; s_dest low
                   PUSH DEF_NULL
                   PUSH WORD PTR [BP + 06h]                ; s_length
                   CALL FAR PTR CODE_MEMORY:MemSet
                   POP BP
                   RETF 4 + 2
MemClear           ENDP
MemSet             ENDP
MemCpy             PROC FAR ; void far pascal MemCpy(void far *s_dest, void far *s_src, unsigned int s_length)
MemCpyB            PROC FAR ; void far pascal MemCpyB(void far *s_dest, void far *s_src, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   JCXZ SHORT L_MemCpy_01
                   LDS SI, DWORD PTR [BP + 08h]            ; s_src
                   LES DI, DWORD PTR [BP + 0Ch]            ; s_dest
                   CLD
                   SHR CX, 01h
                   JNC SHORT L_MemCpy_00
                   MOVSB
L_MemCpy_00        LABEL SHORT
L_MemCpyW_00       LABEL SHORT
                   REPZ MOVSW
L_MemCpy_01        LABEL SHORT
L_MemCpyW_01       LABEL SHORT
                   POP CX
                   POP DI
                   POP SI
                   POP ES
                   POP DS
                   POP BP
                   RETF 4 + 4 + 2
MemCpyB            ENDP
MemCpyW            PROC FAR ; void far pascal MemCpyW(void far *s_dest, void far *s_src, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   JCXZ SHORT L_MemCpyW_01
                   LDS SI, DWORD PTR [BP + 08h]            ; s_src
                   LES DI, DWORD PTR [BP + 0Ch]            ; s_dest
                   CLD
                   MAC_JumpShort <L_MemCpyW_00>
MemCpyW            ENDP
MemCpy             ENDP
MemScan            PROC FAR ; void far * far pascal MemScan(void far *s_dest, unsigned int s_byte, unsigned int s_length)
MemScanB           PROC FAR ; void far * far pascal MemScanB(void far *s_dest, unsigned int s_byte, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH ES
                   PUSH DI
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   JCXZ SHORT L_MemScan_01
                   MOV AL, BYTE PTR [BP + 08h + DEF_Word_Low] ; s_byte
                   LES DI, DWORD PTR [BP + 0Ah]            ; s_dest
                   CLD
                   REPNZ SCASB
L_MemScanW_00      LABEL SHORT
                   MOV DX, ES
                   MOV AX, DI
                   DEC AX
L_MemScan_00       LABEL SHORT
                   POP CX
                   POP DI
                   POP ES
                   POP BP
                   RETF 4 + 2 + 2
L_MemScan_01       LABEL SHORT
L_MemScanW_01      LABEL SHORT
                   XOR AX, AX
                   XOR DX, DX
                   MAC_JumpShort <L_MemScan_00>
MemScanB           ENDP
MemScanW           PROC FAR ; void far * far pascal MemScanW(void far *s_dest, unsigned int s_word, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH ES
                   PUSH DI
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   JCXZ SHORT L_MemScanW_01
                   MOV AX, WORD PTR [BP + 08h]             ; s_word
                   LES DI, DWORD PTR [BP + 0Ah]            ; s_dest
                   CLD
                   REPNZ SCASW
                   MAC_JumpShort <L_MemScanW_00>
MemScanW           ENDP
MemScan            ENDP
MemCmp             PROC FAR ; void far pascal MemCmp(void far *s_dest, void far *s_src, unsigned int s_length)
MemCmpB            PROC FAR ; void far pascal MemCmpB(void far *s_dest, void far *s_src, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   JCXZ SHORT L_MemCmp_00
                   LDS SI, DWORD PTR [BP + 08h]            ; s_src
                   LES DI, DWORD PTR [BP + 0Ch]            ; s_dest
                   CLD
                   REPZ CMPSB
                   MOV AL, BYTE PTR DS:[SI - 0001h]
                   XOR AH, AH
                   MOV CL, BYTE PTR ES:[DI - 0001h]
                   XOR CH, CH
                   SUB AX, CX
L_MemCmp_00        LABEL SHORT
L_MemCmpW_00       LABEL SHORT
                   POP CX
                   POP DI
                   POP SI
                   POP ES
                   POP DS
                   POP BP
                   RETF 4 + 4 + 2
MemCmpB            ENDP
MemCmpW            PROC FAR ; void far pascal MemCmpW(void far *s_dest, void far *s_src, unsigned int s_length)
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   JCXZ SHORT L_MemCmpW_00
                   LDS SI, DWORD PTR [BP + 08h]            ; s_src
                   LES DI, DWORD PTR [BP + 0Ch]            ; s_dest
                   CLD
                   REPZ CMPSW
                   MOV AX, WORD PTR DS:[SI - 0001h]
                   MOV CX, WORD PTR ES:[DI - 0001h]
                   SUB AX, CX
                   MAC_JumpShort <L_MemCmpW_00>
MemCmpW            ENDP
MemCmp             ENDP
WhatBit            PROC NEAR ; unsigned int far pascal WhatBit(unsigned int s_byte, unsigned int s_index)
                   PUSH BP
                   MOV BP, SP
                   PUSH CX
                   XOR AX, AX
                   MOV CL, BYTE PTR [BP + 06h + DEF_Word_Low]
                   INC CL
                   MOV CH, BYTE PTR [BP + 08h + DEF_Word_Low]
                   SHL CH, CL
                   ADC AL, DEF_Null
                   POP CX
                   POP BP
                   RETF 2 + 2
WhatBit            ENDP
_WhatBit           PROC NEAR ; unsigned int near pascal _WhatBit(unsigned int s_byte, unsigned int s_index)
                   PUSH BP
                   MOV BP, SP
                   PUSH CX
                   XOR AX, AX
                   MOV CL, BYTE PTR [BP + 04h + DEF_Word_Low]
                   INC CL
                   MOV CH, BYTE PTR [BP + 06h + DEF_Word_Low]
                   SHL CH, CL
                   ADC AL, DEF_Null
                   POP CX
                   POP BP
                   RETN 2 + 2
_WhatBit           ENDP
IncBitCount        PROC NEAR ; void near pascal IncBitCount(void)
                   ASSUME DS:BSS_MEMORY
                   PUSH DS
                   PUSH BSS_MEMORY
                   POP DS
                   INC BYTE PTR BSS_MEMORY:D_Bit_BitPosition
                   CMP BYTE PTR BSS_MEMORY:D_Bit_BitPosition, 08h
                   JB SHORT L_IncBitCount_00
                   MOV BYTE PTR BSS_MEMORY:D_Bit_BitPosition, DEF_Null
                   INC WORD PTR BSS_MEMORY:D_Bit_BytePosition
L_IncBitCount_00   LABEL SHORT
                   POP DS
                   ASSUME DS:DATA_MEMORY
                   RETN
IncBitCount        ENDP
OpenBit            PROC FAR ; void far pascal OpenBit(void far *s_mem)
                   PUSH BP
                   MOV BP, SP
                   ASSUME DS:BSS_MEMORY
                   PUSH DS
                   PUSH BSS_MEMORY
                   POP DS
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_mem high
                   POP WORD PTR BSS_MEMORY:D_Bit_Address[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset] ; s_mem low
                   POP WORD PTR BSS_MEMORY:D_Bit_Address[DEF_Far_Offset]
                   MOV WORD PTR BSS_MEMORY:D_Bit_BytePosition, DEF_Null
                   MOV WORD PTR BSS_MEMORY:D_Bit_BitPosition, DEF_Null
                   POP DS
                   ASSUME DS:DATA_MEMORY
                   POP BP
                   RETF 4
OpenBit            ENDP
ReadBit            PROC FAR ; unsigned int far pascal ReadBit(void)                  
                   PUSH DS
                   PUSH ES
                   PUSH DI
                   ASSUME DS:BSS_MEMORY
                   PUSH BSS_MEMORY
                   POP DS
                   LES DI, DWORD PTR BSS_MEMORY:D_Bit_Address
                   ADD DI, WORD PTR BSS_MEMORY:D_Bit_BytePosition
                   MOV AL, BYTE PTR ES:[DI]
                   PUSH AX
                   PUSH WORD PTR BSS_MEMORY:D_Bit_BitPosition
                   CALL NEAR PTR CODE_MEMORY:_WhatBit
                   CALL NEAR PTR CODE_MEMORY:IncBitCount
                   ASSUME DS:DATA_MEMORY
                   POP DI
                   POP ES
                   POP DS
                   RETF
ReadBit            ENDP
WriteBitSet        PROC FAR ; void far pascal WriteBitSet(void)
                   PUSH DS
                   PUSH ES
                   PUSH DI
                   PUSH AX
                   PUSH CX
                   ASSUME DS:BSS_MEMORY
                   PUSH BSS_MEMORY
                   POP DS
                   MOV AH, 01h
                   XOR CH, CH
                   MOV CL, BYTE PTR BSS_MEMORY:D_Bit_BitPosition[DEF_Word_Low]
                   SHL AH, CL
                   LES DI, DWORD PTR BSS_MEMORY:D_Bit_Address
                   ADD DI, WORD PTR BSS_MEMORY:D_Bit_BytePosition
                   OR CX, CX
                   JNZ SHORT L_WriteBitSet_00
                   MOV BYTE PTR ES:[DI], CL
L_WriteBitSet_00   LABEL SHORT
                   MOV AL, BYTE PTR ES:[DI]
                   OR AL, AH
                   MOV BYTE PTR ES:[DI], AL
                   CALL NEAR PTR CODE_MEMORY:IncBitCount
                   ASSUME DS:DATA_MEMORY
                   POP CX
                   POP AX
                   POP DI
                   POP ES
                   POP DS
                   RETF
WriteBitSet        ENDP
WriteBitClear      PROC FAR ; void far pascal WriteBitClear(void)
                   PUSH DS
                   PUSH ES
                   PUSH DI
                   PUSH AX
                   PUSH CX
                   ASSUME DS:BSS_MEMORY
                   PUSH BSS_MEMORY
                   POP DS
                   MOV AH, 01h
                   XOR CH, CH
                   MOV CL, BYTE PTR BSS_MEMORY:D_Bit_BitPosition[DEF_Word_Low]
                   SHL AH, CL
                   NOT AH
                   LES DI, DWORD PTR BSS_MEMORY:D_Bit_Address
                   ADD DI, WORD PTR BSS_MEMORY:D_Bit_BytePosition
                   OR CX, CX
                   JNZ SHORT L_WriteBitClear_00
                   MOV BYTE PTR ES:[DI], CL
L_WriteBitClear_00 LABEL SHORT
                   MOV AL, BYTE PTR ES:[DI]
                   AND AL, AH
                   MOV BYTE PTR ES:[DI], AL
                   CALL NEAR PTR CODE_MEMORY:IncBitCount
                   ASSUME DS:DATA_MEMORY
                   POP CX
                   POP AX
                   POP DI
                   POP ES
                   POP DS
                   RETF
WriteBitClear      ENDP
CloseBit           PROC FAR ; unsigned int far pascal CloseBit(void)
                   ASSUME DS:BSS_MEMORY                   
                   PUSH DS
                   PUSH BSS_MEMORY
                   POP DS
                   MOV AX, WORD PTR BSS_MEMORY:D_Bit_BytePosition
                   POP DS
                   ASSUME DS:DATA_MEMORY
                   RETF
CloseBit           ENDP
CODE_MEMORY        ENDS

                   ASSUME CS:CODE_MEMORY, DS:DATA_MEMORY, ES:NOTHING, SS:STACK_DEFAULT
DATA_MEMORY        SEGMENT
DATA_MEMORY        ENDS

                   ASSUME CS:CODE_MEMORY, DS:BSS_MEMORY, ES:NOTHING, SS:STACK_DEFAULT
BSS_MEMORY         SEGMENT
D_Bit_Address      DW ?, ?
D_Bit_BytePosition DW ?
D_Bit_BitPosition  DW ?
BSS_MEMORY         ENDS
                   END

; End of source
