COMMENT #
 NOTE: 2000.10.03 [TUE]
  - Heap header struct
    struct HEAP_Header{
            void far *s_next;
            unsigned int s_length;
            unsigned char s_data[s_length];
           }; // First entry = HEAP_DEFAULT:L_ENTRY_HEAP_DEFAULT
#

DEF_ASM_HEAP       EQU "HEAP.ASM"

INCLUDE            COMMON.INC

PUBLIC             New_00, New_00_00
PUBLIC             New, FarAlloc, SetNew, SetFarAlloc
PUBLIC             Delete_00
PUBLIC             Delete, FarFree, SetDelete, SetFarFree
;
PUBLIC             L_ENTRY_HEAP_DEFAULT

                   ASSUME CS:CODE_HEAP, DS:DATA_HEAP, ES:NOTHING, SS:STACK_DEFAULT
CODE_HEAP          SEGMENT
New_00             PROC FAR ; void far * far pascal New_00(unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH HEAP_DEFAULT
                   PUSH OFFSET HEAP_DEFAULT:L_ENTRY_HEAP_DEFAULT
                   PUSH DEF_Null
                   PUSH DEF_SIZE_HeapHeader
                   CALL FAR PTR CODE_MEMORY:MemSet
                   PUSH CODE_HEAP
                   PUSH OFFSET CODE_HEAP:New_00_00
                   CALL FAR PTR CODE_HEAP:SetNew
                   PUSH WORD PTR [BP + 06h]                ; s_length
                   CALL FAR PTR CODE_HEAP:New_00_00
                   POP BP
                   RETF 2
New_00             ENDP
New_00_Scan        PROC NEAR ; DS:BX near pascal New_00_Scan(unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   MOV AX, HEAP_DEFAULT
                   MOV DS, AX
                   MOV BX, OFFSET HEAP_DEFAULT:L_ENTRY_HEAP_DEFAULT
                   ADD WORD PTR [BP + 04h], DEF_SIZE_HeapHeader ; s_length
L_New_00_Scan_00   LABEL SHORT
                   MOV AX, WORD PTR DS:[BX + 0000h + DEF_Far_Segment]
                   OR AX, WORD PTR DS:[BX + 0000h + DEF_Far_Offset]
                   JZ SHORT L_New_00_Scan_01
                   PUSH DS
                   PUSH BX
                   CALL FAR PTR CODE_CALC:ToPhysical
                   ADD AX, DEF_SIZE_HeapHeader
                   ADC DX, DEF_Null
                   ADD AX, WORD PTR DS:[BX + 0004h]
                   ADC DX, DEF_Null
                   PUSH DX
                   PUSH AX
                   PUSH WORD PTR DS:[BX + 0000h + DEF_Far_Segment]
                   PUSH WORD PTR DS:[BX + 0000h + DEF_Far_Offset]
                   CALL FAR PTR CODE_CALC:ToPhysical
                   POP CX
                   SUB AX, CX
                   POP CX
                   SBB DX, CX
                   OR DX, DX
                   JNZ SHORT L_New_00_Scan_01
                   CMP AX, WORD PTR [BP + 04h]             ; s_length
                   JAE SHORT L_New_00_Scan_01
                   LDS BX, DWORD PTR DS:[BX + 0000h]
                   JMP SHORT L_New_00_Scan_00
L_New_00_Scan_01   LABEL SHORT
                   POP BP
                   RETN 2
New_00_Scan        ENDP
New_00_00          PROC FAR ; void far * far pascal New_00_00(unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH BX
                   PUSH CX
                   PUSH WORD PTR [BP + 06h]                ; s_length
                   CALL NEAR PTR CODE_HEAP:New_00_Scan
                   PUSH WORD PTR DS:[BX + 0000h + DEF_Far_Segment]
                   PUSH WORD PTR DS:[BX + 0000h + DEF_Far_Offset]
                   PUSH DS
                   PUSH BX
                   CALL FAR PTR CODE_CALC:ToPhysical
                   ADD AX, DEF_SIZE_HeapHeader
                   ADC DX, DEF_Null
                   ADD AX, WORD PTR DS:[BX + 0004h]
                   ADC DX, DEF_Null
                   PUSH DX
                   PUSH AX
                   CALL FAR PTR CODE_CALC:ToLogical
                   MOV WORD PTR DS:[BX + 0000h + DEF_Far_Segment], DX
                   MOV WORD PTR DS:[BX + 0000h + DEF_Far_Offset], AX
                   MOV DS, DX
                   MOV BX, AX
                   POP WORD PTR DS:[BX + 0000h + DEF_Far_Offset]
                   POP WORD PTR DS:[BX + 0000h + DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h]
                   POP WORD PTR DS:[BX + 0004h]
                   ADD AX, DEF_SIZE_HeapHeader
                   PUSH DX
                   PUSH AX
                   CALL FAR PTR CODE_CALC:ToOptimalLogical
                   IFDEF DEF_DisplayError ; Limit machanism
                    CMP DX, DEF_HEAP_Max
                    JB SHORT L_New_00_00_00
                    CMP BYTE PTR DATA_HEAP:D_HeapBusy[DEF_Word_Low], DEF_False
                    JNE SHORT L_New_00_00_00
                    PUSH DS
                    PUSH DATA_HEAP
                    POP DS
                    INC BYTE PTR DATA_HEAP:D_HeapBusy[DEF_Word_Low]
                    PUSH DATA_HEAP
                    PUSH OFFSET DATA_HEAP:S_NewFailed
                    CALL FAR PTR CODE_TEXT:Puts
                    DEC BYTE PTR DATA_HEAP:D_HeapBusy[DEF_Word_Low]
                    POP DS
                    L_New_00_00_00 LABEL SHORT
                   ENDIF
                   POP CX
                   POP BX
                   POP DS
                   POP BP
                   RETF 2
New_00_00          ENDP
New                PROC FAR ; void far * far pascal New(unsigned int s_length)
FarAlloc           PROC FAR ; void far * far pascal FarAlloc(unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h]                ; s_length
                   MAC_CallFar <CODE_HEAP>, <OFFSET CODE_HEAP:New_00>, <C_New>
                   POP BP
                   RETF 2
FarAlloc           ENDP
New                ENDP
SetNew             PROC FAR ; void far pascal SetNew(void far *s_function)
SetFarAlloc        PROC FAR ; void far pascal SetFarAlloc(void far *s_function)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_function high
                   POP WORD PTR CODE_HEAP:C_New[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset] ; s_function low
                   POP WORD PTR CODE_HEAP:C_New[DEF_Far_Offset]
                   POP BP
                   RETF 4
SetFarAlloc        ENDP
SetNew             ENDP
Delete_00          PROC FAR ; void far pascal Delete_00(void far *s_alloc)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH AX
                   PUSH BX
                   PUSH CX
                   PUSH DX
                   MOV AX, HEAP_DEFAULT
                   MOV DS, AX
                   MOV BX, OFFSET HEAP_DEFAULT:L_ENTRY_HEAP_DEFAULT
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]
                   CALL FAR PTR CODE_CALC:ToPhysical
                   OR AX, DX
                   JNZ SHORT L_Delete_00_01
L_Delete_00_00     LABEL SHORT
                   IFDEF DEF_DisplayError
                    PUSH DS
                    PUSH DATA_HEAP
                    POP DS
                    CMP BYTE PTR DATA_HEAP:D_HeapBusy[DEF_Word_High], DEF_True
                    JE SHORT L_Delete_BusyError
                    INC BYTE PTR DATA_HEAP:D_HeapBusy[DEF_Word_High]
                    PUSH DATA_HEAP
                    PUSH OFFSET DATA_HEAP:S_DeleteFailed
                    CALL FAR PTR CODE_TEXT:Puts
                    DEC BYTE PTR DATA_HEAP:D_HeapBusy[DEF_Word_High]
                    L_Delete_BusyError LABEL SHORT
                    POP DS
                   ENDIF
                   MAC_JumpShort <L_Delete_00_05>
L_Delete_00_01     LABEL SHORT
                   SUB AX, DEF_SIZE_HeapHeader
                   SBB DX, DEF_Null
L_Delete_00_02     LABEL SHORT
                   CMP WORD PTR DS:[BX + 0000h + DEF_Far_Offset], DEF_Null 
                   JNE SHORT L_Delete_00_03
                   CMP WORD PTR DS:[BX + 0000h + DEF_Far_Segment], DEF_Null
                   JE SHORT L_Delete_00_05
L_Delete_00_03     LABEL SHORT
                   PUSH AX
                   PUSH DX
                   PUSH DX
                   PUSH AX
                   PUSH WORD PTR DS:[BX + 0000h + DEF_Far_Segment]
                   PUSH WORD PTR DS:[BX + 0000h + DEF_Far_Offset]
                   CALL FAR PTR CODE_CALC:ToPhysical
                   POP CX
                   SUB AX, CX
                   POP CX
                   SBB DX, CX
                   OR AX, DX
                   JNZ SHORT L_Delete_00_04
                   MOV AX, BX
                   MOV DX, DS
                   LDS BX, DWORD PTR DS:[BX + 0000h]
                   PUSH WORD PTR DS:[BX + 0000h + DEF_Far_Segment]
                   PUSH WORD PTR DS:[BX + 0000h + DEF_Far_Offset]
                   MOV DS, DX
                   MOV BX, AX
                   POP WORD PTR DS:[BX + 0000h + DEF_Far_Offset]
                   POP WORD PTR DS:[BX + 0000h + DEF_Far_Segment]
                   POP DX
                   POP AX
                   MAC_JumpShort <L_Delete_00_05>
L_Delete_00_04     LABEL SHORT
                   POP DX
                   POP AX
                   LDS BX, DWORD PTR DS:[BX + 0000h]
                   MAC_JumpShort <L_Delete_00_02>
L_Delete_00_05     LABEL SHORT
                   POP DX
                   POP CX
                   POP BX
                   POP AX
                   POP DS
                   POP BP
                   RETF 4
Delete_00          ENDP
Delete             PROC FAR ; void far pascal Delete(void far *s_alloc)
FarFree            PROC FAR ; void far pascal FarFree(void far *s_alloc)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_alloc high
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset] ; s_alloc low
                   MAC_CallFar <CODE_HEAP>, <OFFSET CODE_HEAP:Delete_00>, <C_Delete>
                   POP BP
                   RETF 4
FarFree            ENDP
Delete             ENDP
SetDelete          PROC FAR ; void far pascal SetDelete(void far *s_function)
SetFarFree         PROC FAR ; void far pascal SetFarFree(void far *s_function)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_function high
                   POP WORD PTR CODE_HEAP:C_Delete[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset] ; s_function low
                   POP WORD PTR CODE_HEAP:C_Delete[DEF_Far_Offset]
                   POP BP
                   RETF 4
SetFarFree         ENDP
SetDelete          ENDP
CODE_HEAP          ENDS
                   
                   ASSUME CS:CODE_HEAP, DS:DATA_HEAP, ES:NOTHING, SS:STACK_DEFAULT
DATA_HEAP          SEGMENT
IFDEF              DEF_DisplayError
 D_HeapBusy        DB DEF_False, DEF_False
 S_NewFailed       DB "Fatal error : New procedure failed. Overflow detected.", DEF_ASCII_Beep
                   DB DEF_ASCII_CarrigeReturn, DEF_ASCII_LineFeed
                   DB DEF_ASCII_EndOfString
 S_DeleteFailed    DB "Fatal error : Delete procedure failed. NULL pointer detected.", DEF_ASCII_Beep
                   DB DEF_ASCII_CarrigeReturn, DEF_ASCII_LineFeed
                   DB DEF_ASCII_EndOfString
ENDIF              
DATA_HEAP          ENDS

                   ASSUME CS:CODE_HEAP, DS:BSS_HEAP, ES:NOTHING, SS:STACK_DEFAULT
BSS_HEAP           SEGMENT
BSS_HEAP           ENDS

                   ASSUME CS:CODE_HEAP, DS:DATA_HEAP, ES:NOTHING, SS:STACK_DEFAULT
HEAP_DEFAULT       SEGMENT
L_ENTRY_HEAP_DEFAULT LABEL BYTE
                   IFDEF DEF_ViewLastSegment
                    DB DEF_SIZE_Para DUP (0CDh)
                    DB "  LAST SEGMENT  "                  ; Show all segment
                    DB DEF_SIZE_Para DUP (0CDh)
                   ENDIF
HEAP_DEFAULT       ENDS
                   END

; End of source
