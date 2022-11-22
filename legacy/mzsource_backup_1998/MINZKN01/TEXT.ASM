COMMENT #
 Code by JaeHyuk Cho    
#

DEF_ASM_TEXT       EQU "TEXT.ASM"

INCLUDE            COMMON.INC

PUBLIC             Putc_00, Putc_01
PUBLIC             Putc, SetPutc
PUBLIC             Puts_00
PUBLIC             Putn_00
PUBLIC             Puts, Putn, SetPuts, SetPutn
PUBLIC             PutXY_00
PUBLIC             PutXY, SetPutXY
PUBLIC             GetXY_00
PUBLIC             GetXY, SetGetXY
PUBLIC             PutLineX, PutLineY
PUBLIC             Box, Rectangle, Window, ClearScreen
PUBLIC             SetWindow
PUBLIC             GetText, PutText
PUBLIC             WhereX_00, WhereY_00
PUBLIC             WhereX, WhereY
PUBLIC             SetWhereX, SetWhereY
PUBLIC             MoveCursor_00
PUBLIC             MoveCursor, CursorX, CursorY, CursorXY
PUBLIC             SetMoveCursor
PUBLIC             Blink_00
PUBLIC             Blink, SetBlink
PUBLIC             PutsXY, PutnXY, PutmXY
PUBLIC             PrintF, _PrintF, PrintK, _PrintK
PUBLIC             PutDump

PUBLIC             TEXT_VideoMemory
PUBLIC             TEXT_Page
PUBLIC             TEXT_Color
PUBLIC             TEXT_Character
PUBLIC             TEXT_Res
PUBLIC             TEXT_Window
PUBLIC             TEXT_TabLength
PUBLIC             TEXT_LineTable

                   ASSUME CS:CODE_TEXT, DS:DATA_TEXT, ES:NOTHING, SS:STACK_DEFAULT
CODE_TEXT          SEGMENT
Putc_00            PROC FAR ; void far pascal Putc_00(unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSHA                   
                   MOV AX, DATA_TEXT
                   MOV DS, AX                   
                   MOV AH, 0Eh ; BIOS Call - TTY
                   MOV AL, BYTE PTR [BP + 06h]             ; s_character
                   MOV BL, BYTE PTR DATA_TEXT:TEXT_Color
                   MOV BH, BYTE PTR DATA_TEXT:TEXT_Page
                   INT DEF_INT_Video
                   POPA
                   POP DS
                   POP BP
                   RETF 2
Putc_00            ENDP
IncCursor          PROC NEAR ; void near pascal IncCursor(void)
                   CALL FAR PTR CODE_TEXT:WhereX
                   INC AX
                   CMP AX, WORD PTR DATA_TEXT:TEXT_Window[0004h]
                   JBE SHORT L_IncCursor_00
                   CALL NEAR PTR CODE_TEXT:CarrigeReturn
                   CALL NEAR PTR CODE_TEXT:LineFeed
                   MAC_JumpShort <L_IncCursor_01>
L_IncCursor_00     LABEL SHORT
                   PUSH AX
                   CALL FAR PTR CODE_TEXT:CursorX
L_IncCursor_01     LABEL SHORT
                   RETN
IncCursor          ENDP
CarrigeReturn      PROC NEAR ; void near pascal CarrigeReturn(void)
                   PUSH WORD PTR DATA_TEXT:TEXT_Window[0000h]
                   CALL FAR PTR CODE_TEXT:CursorX                                      
                   RETN
CarrigeReturn      ENDP
Scroll             PROC NEAR ; void near pascal Scroll(void)
                   PUSH DS
                   PUSH ES
                   PUSH SI
                   PUSH DI
                   PUSH CX
                   PUSH DX
                   MOV CX, WORD PTR DATA_TEXT:TEXT_Window[0006h]
                   SUB CX, WORD PTR DATA_TEXT:TEXT_Window[0002h]
                   MOV DX, WORD PTR DATA_TEXT:TEXT_Window[0004h]
                   SUB DX, WORD PTR DATA_TEXT:TEXT_Window[0000h]
                   INC DX
                   MOV AX, WORD PTR DATA_TEXT:TEXT_Res[DEF_DWord_Low]
                   SHL AX, 01h
                   PUSH AX
                   PUSH AX
                   PUSH CX
                   PUSH DX
                   PUSH WORD PTR DATA_TEXT:TEXT_Window[0000h]
                   PUSH WORD PTR DATA_TEXT:TEXT_Window[0002h]
                   CALL NEAR PTR CODE_TEXT:GetTextOffset
                   POP DX
                   POP CX
                   POP AX
                   PUSH DS
                   POP ES
                   MOV DI, BX                                                         
                   ADD AX, BX
                   POP BX
                   MOV SI, AX
                   CLD
L_Scroll_00        LABEL SHORT
                   PUSH CX
                   PUSH SI
                   PUSH DI
                   MOV CX, DX
                   REPZ MOVSW
                   POP DI
                   POP SI
                   ADD SI, BX
                   ADD DI, BX
                   POP CX
                   LOOP SHORT L_Scroll_00
                   POP DX
                   POP CX
                   POP DI
                   POP SI
                   POP ES
                   POP DS
                   MOV AH, BYTE PTR DATA_TEXT:TEXT_Color
                   MOV AL, BYTE PTR DATA_TEXT:TEXT_Character
                   PUSH AX
                   PUSH WORD PTR DATA_TEXT:TEXT_Window[0000h]
                   PUSH WORD PTR DATA_TEXT:TEXT_Window[0004h]
                   PUSH WORD PTR DATA_TEXT:TEXT_Window[0006h]
                   CALL FAR PTR CODE_TEXT:PutLineX
                   RETN
Scroll             ENDP
LineFeed           PROC NEAR ; void near pascal LineFeed(void)
                   CALL FAR PTR CODE_TEXT:WhereY
                   INC AX
                   CMP AX, WORD PTR DATA_TEXT:TEXT_Window[0006h]
                   JBE SHORT L_LineFeed_00
                   CALL NEAR PTR CODE_TEXT:Scroll
                   MAC_JumpShort <L_LineFeed_01>
L_LineFeed_00      LABEL SHORT
                   PUSH AX
                   CALL FAR PTR CODE_TEXT:CursorY
L_LineFeed_01      LABEL SHORT                 
                   RETN
LineFeed           ENDP
BelBeep            PROC NEAR ; void near pascal BelBeep(void)
                   PUSH 1000d
                   PUSH 80h
                   CALL FAR PTR CODE_SPEAKER:Beep
                   RETN
BelBeep            ENDP
BackSpace          PROC NEAR ; void near pascal BackSpace(void)
                   CALL FAR PTR CODE_TEXT:WhereX
                   CMP AX, WORD PTR DATA_TEXT:TEXT_Window[0000h]
                   JA SHORT L_BackSpace_00
                   CALL FAR PTR CODE_TEXT:WhereY
                   CMP AX, WORD PTR DATA_TEXT:TEXT_Window[0002h]
                   JE SHORT L_BackSpace_01
                   PUSH WORD PTR DATA_TEXT:TEXT_Window[0004h]
                   PUSH AX
                   CALL FAR PTR CODE_TEXT:CursorXY
                   MAC_JumpShort <L_BackSpace_01>
L_BackSpace_00     LABEL SHORT
                   DEC AX
                   PUSH AX
                   CALL FAR PTR CODE_TEXT:CursorX
L_BackSpace_01     LABEL SHORT
                   RETN
BackSpace          ENDP
Tab                PROC NEAR ; void near pascal Tab(void)
                   PUSH CX
                   MOV CX, WORD PTR DATA_TEXT:TEXT_TabLength
L_Tab_00           LABEL SHORT
                   CALL NEAR PTR CODE_TEXT:IncCursor
                   LOOP SHORT L_Tab_00
                   POP CX
                   RETN
Tab                ENDP
T_Putc_01          DB DEF_ASCII_CarrigeReturn
                   DW OFFSET CODE_TEXT:CarrigeReturn
                   DB DEF_ASCII_LineFeed
                   DW OFFSET CODE_TEXT:LineFeed
                   DB DEF_ASCII_Beep
                   DW OFFSET CODE_TEXT:BelBeep
                   DB DEF_ASCII_BackSpace
                   DW OFFSET CODE_TEXT:BackSpace
                   DB DEF_ASCII_Tab
                   DW OFFSET CODE_TEXT:Tab
                   DB DEF_Null
Putc_01            PROC FAR ; void far pascal Putc_01(unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH AX
                   PUSH BX
                   MOV AX, DATA_TEXT
                   MOV DS, AX
                   MOV AH, BYTE PTR DATA_TEXT:TEXT_Color
                   MOV AL, BYTE PTR [BP + 06h]             ; s_character
                   MOV BX, OFFSET CODE_TEXT:T_Putc_01
L_Putc_01_00       LABEL SHORT
                   TEST BYTE PTR CODE_TEXT:[BX], 0FFh
                   JZ SHORT L_Putc_01_02
                   CMP AL, BYTE PTR CODE_TEXT:[BX]
                   JNE SHORT L_Putc_01_01
                   CALL WORD PTR CODE_TEXT:[BX + 0001h]
                   MAC_JumpShort <L_Putc_01_03>
L_Putc_01_01       LABEL SHORT
                   ADD BX, 0001h + 0002h
                   JMP SHORT L_Putc_01_00
L_Putc_01_02       LABEL SHORT
                   PUSH AX
                   CALL FAR PTR CODE_TEXT:WhereX
                   PUSH AX
                   CALL FAR PTR CODE_TEXT:WhereY
                   PUSH AX
                   CALL FAR PTR CODE_TEXT:PutXY
                   CALL NEAR PTR CODE_TEXT:IncCursor
L_Putc_01_03       LABEL SHORT
                   POP BX
                   POP AX
                   POP DS
                   POP BP
                   RETF 2
Putc_01            ENDP
Putc               PROC FAR ; void far pascal Putc(unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h]
                   MAC_CallFar <CODE_TEXT>, <OFFSET CODE_TEXT:Putc_00>, <C_Putc>
                   POP BP
                   RETF 2
Putc               ENDP
SetPutc            PROC FAR ; void far pascal SetPutc(void far *s_function)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_function high
                   POP WORD PTR CODE_TEXT:C_Putc[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_function low
                   POP WORD PTR CODE_TEXT:C_Putc[DEF_Far_Offset]
                   POP BP
                   RETF 4
SetPutc            ENDP
Puts_00            PROC FAR ; void far pascal Puts(void far *s_string)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH SI
                   PUSH AX
                   XOR AH, AH
                   LDS SI, DWORD PTR [BP + 06h]            ; s_string
                   CLD
L_Puts_00_00       LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT L_Puts_00_02
                   CMP AL, '\'
                   JNE SHORT L_Puts_00_01
                   LODSB
                   CMP AL, '\'
                   JE SHORT L_Puts_00_01
                   PUSH DS
                   PUSH DATA_TEXT
                   POP DS
                   MOV WORD PTR DATA_TEXT:TEXT_Color, AX
                   POP DS
                   JMP SHORT L_Puts_00_00
L_Puts_00_01       LABEL SHORT                   
                   PUSH AX
                   CALL FAR PTR CODE_TEXT:Putc
                   JMP SHORT L_Puts_00_00
L_Puts_00_02       LABEL SHORT                   
                   POP AX
                   POP SI
                   POP DS
                   POP BP
                   RETF 4
Puts_00            ENDP
Putn_00            PROC FAR ; void far pascal Putn_00(void far *s_string, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH SI
                   PUSH AX
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   JCXZ SHORT L_Putn_00_01
                   MOV AX, DATA_TEXT
                   MOV DS, AX
                   XOR AH, AH
                   LDS SI, DWORD PTR [BP + 08h]            ; s_string
                   CLD
                   MOV AL, BYTE PTR CODE_TEXT:T_Putc_01
                   MOV BYTE PTR CODE_TEXT:T_Putc_01, AH
                   PUSH AX
L_Putn_00_00       LABEL SHORT
                   LODSB
                   PUSH AX
                   CALL FAR PTR CODE_TEXT:Putc
                   LOOP SHORT L_Putn_00_00
L_Putn_00_01       LABEL SHORT
                   POP AX
                   MOV BYTE PTR CODE_TEXT:T_Putc_01, AL
                   POP CX
                   POP AX
                   POP SI
                   POP DS
                   POP BP
                   RETF 4 + 2
Putn_00            ENDP
Puts               PROC FAR ; void far pascal Puts(void far *s_string)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_string high
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_string low
                   MAC_CallFar <CODE_TEXT>, <OFFSET CODE_TEXT:Puts_00>, <C_Puts>
                   POP BP
                   RETF 4
Puts               ENDP
Putn               PROC FAR ; void far pascal Putn(void far *s_string, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 08h + DEF_Far_Segment] ; s_string high
                   PUSH WORD PTR [BP + 08h + DEF_Far_Offset]  ; s_string low
                   PUSH WORD PTR [BP + 06h]                   ; s_length
                   MAC_CallFar <CODE_TEXT>, <OFFSET CODE_TEXT:Putn_00>, <C_Putn>
                   POP BP
                   RETF 4 + 2
Putn               ENDP
SetPuts            PROC FAR ; void far pascal SetPuts(void far *s_function)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_function high
                   POP WORD PTR CODE_TEXT:C_Puts[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_function low
                   POP WORD PTR CODE_TEXT:C_Puts[DEF_Far_Offset]
                   POP BP
                   RETF 4
SetPuts            ENDP
SetPutn            PROC FAR ; void far pascal SetPutn(void far *s_function)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_function high
                   POP WORD PTR CODE_TEXT:C_Putn[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_function low
                   POP WORD PTR CODE_TEXT:C_Putn[DEF_Far_Offset]
                   POP BP
                   RETF 4
SetPutn            ENDP
GetTextOffset      PROC NEAR ; DS:BX near pascal GetTextOffset(unsigned int s_x, unsigned int s_y)
                   ; DS:BX = (s_y MUL (res[0] SHL 01h)) + (s_x SHL 01h)
                   ; Reserved DS, AX, BX, DX
                   PUSH BP
                   MOV BP, SP
                   MOV AX, DATA_TEXT
                   MOV DS, AX
                   MOV AX, WORD PTR DATA_TEXT:TEXT_Res[DEF_DWord_Low]
                   SHL AX, 01h
                   MUL WORD PTR [BP + 04h]                 ; s_y
                   MOV BX, WORD PTR [BP + 06h]             ; s_x
                   SHL BX, 01h
                   ADD AX, BX
                   LDS BX, DWORD PTR DATA_TEXT:TEXT_VideoMemory
                   ADD BX, AX
                   POP BP
                   RETN 2 + 2
GetTextOffset      ENDP
PutXY_00           PROC FAR ; void far pascal PutXY_00(unsigned int s_attr, unsigned int s_x, unsigned int s_y)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH AX
                   PUSH BX
                   PUSH DX
                   PUSH WORD PTR [BP + 08h]                ; s_x
                   PUSH WORD PTR [BP + 06h]                ; s_y
                   CALL NEAR PTR CODE_TEXT:GetTextOffset
                   MOV AX, WORD PTR [BP + 0Ah]             ; s_attr
                   MOV WORD PTR DS:[BX], AX
                   POP DX
                   POP BX
                   POP AX
                   POP DS
                   POP BP
                   RETF 2 + 2 + 2
PutXY_00           ENDP
PutXY              PROC FAR ; void far pascal PutXY(unsigned int s_attr, unsigned int s_x, unsigned int s_y)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 0Ah]                ; s_attr
                   PUSH WORD PTR [BP + 08h]                ; s_x
                   PUSH WORD PTR [BP + 06h]                ; s_y
                   MAC_CallFar <CODE_TEXT>, <OFFSET CODE_TEXT:PutXY_00>, <C_PutXY>
                   POP BP
                   RETF 2 + 2 + 2
PutXY              ENDP
SetPutXY           PROC FAR ; void far pascal SetPutXY(void far *s_function)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_function high
                   POP WORD PTR CODE_TEXT:C_PutXY[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_function low
                   POP WORD PTR CODE_TEXT:C_PutXY[DEF_Far_Offset]
                   POP BP
                   RETF 4
SetPutXY           ENDP
GetXY_00           PROC FAR ; unsigned int far pascal GetXY_00(unsigned int s_x, unsigned int s_y)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH BX
                   PUSH DX
                   PUSH WORD PTR [BP + 08h]                ; s_x
                   PUSH WORD PTR [BP + 06h]                ; s_y
                   CALL NEAR PTR CODE_TEXT:GetTextOffset
                   MOV AX, WORD PTR DS:[BX]
                   POP DX
                   POP BX
                   POP DS
                   POP BP
                   RETF 2 + 2
GetXY_00           ENDP
GetXY              PROC FAR ; unsigned int far pascal GetXY(unsigned int s_x, unsigned int s_y)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 08h]                ; s_x
                   PUSH WORD PTR [BP + 06h]                ; s_y
                   MAC_CallFar <CODE_TEXT>, <OFFSET CODE_TEXT:GetXY_00>, <C_GetXY>
                   POP BP
                   RETF 2 + 2
GetXY              ENDP
SetGetXY           PROC FAR ; void far pascal SetGetXY(void far *s_function)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_function high
                   POP WORD PTR CODE_TEXT:C_GetXY[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_function low
                   POP WORD PTR CODE_TEXT:C_GetXY[DEF_Far_Offset]
                   POP BP
                   RETF 4
SetGetXY           ENDP
PutLineX           PROC FAR ; void far pascal PutLineX(unsigned int s_attr, unsigned int s_x1, unsigned int s_x2, unsigned int s_y)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH CX
                   MOV CX, WORD PTR [BP + 08h]             ; s_x2
                   MOV AX, WORD PTR [BP + 0Ah]             ; s_x1
                   SUB CX, AX
                   INC CX
L_PutLineX_00      LABEL SHORT
                   PUSH WORD PTR [BP + 0Ch]                ; s_attr
                   PUSH AX
                   INC AX
                   PUSH WORD PTR [BP + 06h]                ; s_y
                   CALL FAR PTR CODE_TEXT:PutXY                   
                   LOOP SHORT L_PutLineX_00
L_PutLineY_00      LABEL SHORT
                   POP CX
                   POP AX
                   POP BP
                   RETF 2 + 2 + 2 + 2
PutLineX           ENDP
PutLineY           PROC FAR ; void far pascal PutLineY(unsigned int s_attr, unsigned int s_x, unsigned int s_y1, unsigned int s_y2)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_y2
                   MOV AX, WORD PTR [BP + 08h]             ; s_y1
                   SUB CX, AX
                   INC CX
L_PutLineY_01      LABEL SHORT
                   PUSH WORD PTR [BP + 0Ch]                ; s_attr
                   PUSH WORD PTR [BP + 0Ah]                ; s_x
                   PUSH AX
                   INC AX
                   CALL FAR PTR CODE_TEXT:PutXY
                   LOOP SHORT L_PutLineY_01
                   MAC_JumpShort <L_PutLineY_00>
PutLineY           ENDP
Box                PROC FAR ; void far pascal Box
                   ; unsigned int s_attr         : 0Eh
                   ; unsigned int s_x1           : 0Ch
                   ; unsigned int s_y1           : 0Ah
                   ; unsigned int s_x2           : 08h
                   ; unsigned int s_y2           : 06h
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH CX
                   MOV CX, WORD PTR [BP + 06h]             ; s_y2
                   MOV AX, WORD PTR [BP + 0Ah]             ; s_y1
                   SUB CX, AX
                   INC CX
L_Box_00           LABEL SHORT
                   PUSH WORD PTR [BP + 0Eh]                ; s_attr
                   PUSH WORD PTR [BP + 0Ch]                ; s_x1
                   PUSH WORD PTR [BP + 08h]                ; s_x2
                   PUSH AX
                   INC AX
                   CALL FAR PTR CODE_TEXT:PutLineX
                   LOOP SHORT L_Box_00
                   POP CX
                   POP AX
                   POP BP
                   RETF 2 + 2 + 2 + 2 + 2
Box                ENDP
Rectangle          PROC FAR ; void far pascal Rectangle
                   ; unsigned int s_attr & type  : 0Eh
                   ; unsigned int s_x1           : 0Ch
                   ; unsigned int s_y1           : 0Ah
                   ; unsigned int s_x2           : 08h
                   ; unsigned int s_y2           : 06h                  
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH AX
                   PUSH BX
                   PUSH CX
                   PUSH DX
                   MOV AX, DATA_TEXT
                   MOV DS, AX
                   MOV CX, WORD PTR [BP + 0Eh]             ; s_attr & type
                   MOV BL, CL
                   XOR BH, BH
                   SHL BX, 04h                   
                   MOV CL, BYTE PTR DATA_TEXT:TEXT_LineTable[BX + 0001h]
                   MOV AX, WORD PTR [BP + 0Ch]             ; s_x1
                   INC AX
                   MOV DX, WORD PTR [BP + 08h]             ; s_x2
                   DEC DX
                   PUSH CX
                   PUSH AX
                   PUSH DX
                   PUSH WORD PTR [BP + 0Ah]                ; s_y1
                   CALL FAR PTR CODE_TEXT:PutLineX
                   PUSH CX
                   PUSH AX
                   PUSH DX
                   PUSH WORD PTR [BP + 06h]                ; s_y2
                   CALL FAR PTR CODE_TEXT:PutLineX                   
                   MOV CL, BYTE PTR DATA_TEXT:TEXT_LineTable[BX + 0000h]
                   MOV AX, WORD PTR [BP + 0Ah]             ; s_y1
                   INC AX
                   MOV DX, WORD PTR [BP + 06h]             ; s_y2
                   DEC DX
                   PUSH CX
                   PUSH WORD PTR [BP + 0Ch]                ; s_x1
                   PUSH AX
                   PUSH DX
                   CALL FAR PTR CODE_TEXT:PutLineY
                   PUSH CX
                   PUSH WORD PTR [BP + 08h]                ; s_x2
                   PUSH AX
                   PUSH DX
                   CALL FAR PTR CODE_TEXT:PutLineY                   
                   MOV CL, BYTE PTR DATA_TEXT:TEXT_LineTable[BX + 0002h]
                   PUSH CX
                   PUSH WORD PTR [BP + 0Ch]                ; s_x1
                   PUSH WORD PTR [BP + 0Ah]                ; s_y1
                   CALL FAR PTR CODE_TEXT:PutXY
                   MOV CL, BYTE PTR DATA_TEXT:TEXT_LineTable[BX + 0004h]
                   PUSH CX
                   PUSH WORD PTR [BP + 08h]                ; s_x2
                   PUSH WORD PTR [BP + 0Ah]                ; s_y1
                   CALL FAR PTR CODE_TEXT:PutXY
                   MOV CL, BYTE PTR DATA_TEXT:TEXT_LineTable[BX + 0008h]
                   PUSH CX
                   PUSH WORD PTR [BP + 0Ch]                ; s_x1
                   PUSH WORD PTR [BP + 06h]                ; s_y2
                   CALL FAR PTR CODE_TEXT:PutXY
                   MOV CL, BYTE PTR DATA_TEXT:TEXT_LineTable[BX + 000Ah]
                   PUSH CX
                   PUSH WORD PTR [BP + 08h]                ; s_x2
                   PUSH WORD PTR [BP + 06h]                ; s_y2
                   CALL FAR PTR CODE_TEXT:PutXY
                   POP DX
                   POP CX
                   POP BX
                   POP AX
                   POP DS
                   POP BP
                   RETF 2 + 2 + 2 + 2 + 2
Rectangle          ENDP       
Window             PROC FAR ; void far pascal Window
                   PUSH BP
                   MOV BP, SP
                   ; unsigned int s_attr & type  : 0Eh
                   ; unsigned int s_x1           : 0Ch
                   ; unsigned int s_y1           : 0Ah
                   ; unsigned int s_x2           : 08h
                   ; unsigned int s_y2           : 06h                  
                   PUSH AX
                   MOV AX, WORD PTR [BP + 0Eh]             ; s_attr & type                    
                   MOV AL, DEF_ASCII_Space
                   PUSH AX
                   PUSH WORD PTR [BP + 0Ch]                ; s_x1
                   PUSH WORD PTR [BP + 0Ah]                ; s_y1
                   PUSH WORD PTR [BP + 06h]                ; s_y2
                   CALL FAR PTR CODE_TEXT:PutLineY
                   PUSH AX
                   PUSH WORD PTR [BP + 08h]                ; s_x2
                   PUSH WORD PTR [BP + 0Ah]                ; s_y1
                   PUSH WORD PTR [BP + 06h]                ; s_y2
                   CALL FAR PTR CODE_TEXT:PutLineY
                   PUSH WORD PTR [BP + 0Eh]                ; s_attr & type
                   INC WORD PTR [BP + 0Ch]                 ; s_x1
                   PUSH WORD PTR [BP + 0Ch]                ; s_x1
                   PUSH WORD PTR [BP + 0Ah]                ; s_y1
                   DEC WORD PTR [BP + 08h]                 ; s_x2
                   PUSH WORD PTR [BP + 08h]                ; s_x2
                   PUSH WORD PTR [BP + 06h]                ; s_y2
                   CALL FAR PTR CODE_TEXT:Rectangle
                   PUSH AX
                   INC WORD PTR [BP + 0Ch]                 ; s_x1
                   PUSH WORD PTR [BP + 0Ch]                ; s_x1
                   INC WORD PTR [BP + 0Ah]                 ; s_y1
                   PUSH WORD PTR [BP + 0Ah]                ; s_y1
                   DEC WORD PTR [BP + 08h]                 ; s_x2
                   PUSH WORD PTR [BP + 08h]                ; s_x2
                   DEC WORD PTR [BP + 06h]                 ; s_y2
                   PUSH WORD PTR [BP + 06h]                ; s_y2
                   CALL FAR PTR CODE_TEXT:Box
                   POP AX
                   POP BP
                   RETF 2 + 2 + 2 + 2 + 2
Window             ENDP
ClearScreen        PROC FAR ; void far pascal ClearScreen(unsigned int s_attr)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH DATA_TEXT
                   POP DS
                   PUSH WORD PTR [BP + 06h]                ; s_attr
                   PUSH WORD PTR DATA_TEXT:TEXT_Window[0000h]
                   PUSH WORD PTR DATA_TEXT:TEXT_Window[0002h]
                   PUSH WORD PTR DATA_TEXT:TEXT_Window[0004h]
                   PUSH WORD PTR DATA_TEXT:TEXT_Window[0006h]
                   CALL FAR PTR CODE_TEXT:Box
                   POP DS
                   POP BP
                   RETF 2
ClearScreen        ENDP
SetWindow          PROC FAR ; void far pascal SetWindow(unsigned int s_x1, unsigned int s_y1, unsigned int s_x2, unsigned int s_y2)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH DATA_TEXT
                   POP DS
                   PUSH WORD PTR [BP + 0Ch]                ; s_x1
                   POP WORD PTR DATA_TEXT:TEXT_Window[0000h]
                   PUSH WORD PTR [BP + 0Ah]                ; s_y1
                   POP WORD PTR DATA_TEXT:TEXT_Window[0002h]
                   PUSH WORD PTR [BP + 08h]                ; s_x2
                   POP WORD PTR DATA_TEXT:TEXT_Window[0004h]
                   PUSH WORD PTR [BP + 06h]                ; s_y2
                   POP WORD PTR DATA_TEXT:TEXT_Window[0006h]
                   POP DS
                   POP BP
                   RETF 2 + 2 + 2 + 2
SetWindow          ENDP
GetText            PROC FAR ; void far pascal GetText
                   ; void far *s_dest            : 0Eh
                   ; unsigned int s_x1           : 0Ch
                   ; unsigned int s_y1           : 0Ah
                   ; unsigned int s_x2           : 08h
                   ; unsigned int s_y2           : 06h
                   PUSH BP
                   MOV BP, SP
                   PUSH ES
                   PUSHA
                   LES DI, DWORD PTR [BP + 0Eh]            ; s_dest
                   CLD
                   MOV CX, WORD PTR [BP + 06h]             ; s_y2
                   MOV DX, WORD PTR [BP + 0Ah]             ; s_y1
                   SUB CX, DX
                   INC CX
L_GetText_00       LABEL SHORT
                   PUSH CX
                   MOV CX, WORD PTR [BP + 08h]             ; s_x2
                   MOV BX, WORD PTR [BP + 0Ch]             ; s_x1
                   SUB CX, BX
                   INC CX
L_GetText_01       LABEL SHORT
                   PUSH BX
                   PUSH DX
                   CALL FAR PTR CODE_TEXT:GetXY                   
                   STOSW
                   INC BX
                   LOOP SHORT L_GetText_01
                   INC DX
                   POP CX
                   LOOP SHORT L_GetText_00
L_PutText_00       LABEL SHORT
                   POPA
                   POP ES
                   POP BP
                   RETF 4 + 2 + 2 + 2 + 2
GetText            ENDP
PutText            PROC FAR ; void far pascal PutText
                   ; void far *s_src             : 0Eh
                   ; unsigned int s_x1           : 0Ch
                   ; unsigned int s_y1           : 0Ah
                   ; unsigned int s_x2           : 08h
                   ; unsigned int s_y2           : 06h
                   PUSH BP
                   MOV BP, SP
                   PUSH ES
                   PUSHA
                   LDS SI, DWORD PTR [BP + 0Eh]            ; s_src
                   CLD
                   MOV CX, WORD PTR [BP + 06h]             ; s_y2
                   MOV DX, WORD PTR [BP + 0Ah]             ; s_y1
                   SUB CX, DX
                   INC CX
L_PutText_01       LABEL SHORT
                   PUSH CX
                   MOV CX, WORD PTR [BP + 08h]             ; s_x2
                   MOV BX, WORD PTR [BP + 0Ch]             ; s_x1
                   SUB CX, BX
                   INC CX
L_PutText_02       LABEL SHORT
                   LODSW
                   PUSH AX
                   PUSH BX
                   PUSH DX
                   CALL FAR PTR CODE_TEXT:PutXY                                      
                   INC BX
                   LOOP SHORT L_PutText_02
                   INC DX
                   POP CX
                   LOOP SHORT L_PutText_01
                   MAC_JumpShort <L_PutText_00>
PutText            ENDP
WhereX_00          PROC FAR ; unsigned int far pascal WhereX_00(void)
                   PUSH DS
                   XOR AX, AX
                   MOV DS, AX
                   MOV AL, BYTE PTR DS:[0450h + DEF_Word_Low]                   
                   POP DS
                   RETF
WhereX_00          ENDP
WhereY_00          PROC FAR ; unsigned int far pascal WhereY_00(void)
                   PUSH DS
                   XOR AX, AX
                   MOV DS, AX
                   MOV AL, BYTE PTR DS:[0450h + DEF_Word_High]
                   POP DS
                   RETF
WhereY_00          ENDP
WhereX             PROC FAR ; unsigned int far pascal WhereX(void)
                   MAC_CallFar <CODE_TEXT>, <OFFSET CODE_TEXT:WhereX_00>, <C_WhereX>
                   RETF
WhereX             ENDP
WhereY             PROC FAR ; unsigned int far pascal WhereY(void)
                   MAC_CallFar <CODE_TEXT>, <OFFSET CODE_TEXT:WhereY_00>, <C_WhereY>
                   RETF
WhereY             ENDP
SetWhereX          PROC FAR ; void far pascal SetWhereX(void far *s_function)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_function high
                   POP WORD PTR CODE_TEXT:C_WhereX[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_function low
                   POP WORD PTR CODE_TEXT:C_WhereX[DEF_Far_Offset]
                   POP BP
                   RETF 4
SetWhereX          ENDP
SetWhereY          PROC FAR ; void far pascal SetWhereY(void far *s_function)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_function high
                   POP WORD PTR CODE_TEXT:C_WhereY[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_function low
                   POP WORD PTR CODE_TEXT:C_WhereY[DEF_Far_Offset]
                   POP BP
                   RETF 4
SetWhereY          ENDP
MoveCursor_00      PROC FAR ; void far pascal MoveCursor_00(unsigned int s_x, unsigned int s_y)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH AX
                   PUSH BX
                   PUSH CX
                   PUSH DX
                   XOR AX, AX
                   MOV DS, AX
                   MOV BX, WORD PTR [BP + 08h]             ; s_x
                   MOV AX, WORD PTR [BP + 06h]             ; s_y
                   CMP BX, 0FFFFh
                   JNE SHORT L_MoveCursor_00_00
                   MOV BL, BYTE PTR DS:[0450h + DEF_Word_Low]
                   XOR BH, BH
L_MoveCursor_00_00 LABEL SHORT
                   CMP AX, 0FFFFh
                   JNE SHORT L_MoveCursor_00_01
                   MOV AL, BYTE PTR DS:[0450h + DEF_Word_High]
                   XOR AH, AH
L_MoveCursor_00_01 LABEL SHORT
                   MOV BYTE PTR DS:[0450h + DEF_Word_Low], BL
                   MOV BYTE PTR DS:[0450h + DEF_Word_High], AL
                   MOV DX, DATA_TEXT
                   MOV DS, DX
                   MUL WORD PTR DATA_TEXT:TEXT_Res[DEF_DWord_Low]
                   ADD BX, AX
                   MOV DX, 03D4h
                   MOV AH, BH
                   MOV AL, 0Eh
                   OUT DX, AX
                   MOV AL, 0Fh
                   MOV AH, BL
                   OUT DX, AX
                   POP DX
                   POP CX
                   POP BX
                   POP AX
                   POP DS
                   POP BP
                   RETF 2 + 2
MoveCursor_00      ENDP
MoveCursor         PROC FAR ; void far pascal MoveCursor(unsigned int s_x, unsigned int s_y)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 08h]                ; s_x
                   PUSH WORD PTR [BP + 06h]                ; s_y
                   MAC_CallFar <CODE_TEXT>, <OFFSET CODE_TEXT:MoveCursor_00>, <C_MoveCursor>
                   POP BP
                   RETF 2 + 2
MoveCursor         ENDP
CursorX            PROC FAR ; void far pascal CursorX(unsigned int s_x)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h]                ; s_x
                   PUSH 0FFFFh
                   CALL FAR PTR CODE_TEXT:MoveCursor
                   POP BP
                   RETF 2
CursorX            ENDP
CursorY            PROC FAR ; void far pascal CursorY(unsigned int s_y)
                   PUSH BP
                   MOV BP, SP
                   PUSH 0FFFFh
                   PUSH WORD PTR [BP + 06h]                ; s_y
                   CALL FAR PTR CODE_TEXT:MoveCursor
                   POP BP
                   RETF 2
CursorY            ENDP
CursorXY           PROC FAR ; void far pascal CursorXY(unsigned int s_x, unsigned int s_y)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 08h]                ; s_x
                   PUSH WORD PTR [BP + 06h]                ; s_y
                   CALL FAR PTR CODE_TEXT:MoveCursor
                   POP BP
                   RETF 2
CursorXY           ENDP
SetMoveCursor      PROC FAR ; void far pascal SetMoveCursor(void far *s_function)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_function high
                   POP WORD PTR CODE_TEXT:C_MoveCursor[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_function low
                   POP WORD PTR CODE_TEXT:C_MoveCursor[DEF_Far_Offset]
                   POP BP
                   RETF 4
SetMoveCursor      ENDP
Blink_00           PROC FAR ; void far pascal Blink_00(unsigned int s_sw)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   MOV DX, 03D4h + 0006h
                   IN AL, DX
                   MOV DX, 03C0h
                   MOV AL, 10h
                   OUT DX, AL
                   INC DX
                   IN AL, DX
                   TEST WORD PTR [BP + 06h], 0FFFFh        ; s_sw
                   JZ SHORT L_Blink_00_00
                   OR AL, 08h
                   MAC_JumpShort <L_Blink_00_01>
L_Blink_00_00      LABEL SHORT
                   AND AL, 0F7h
L_Blink_00_01      LABEL SHORT
                   DEC DX
                   OUT DX, AL
                   MOV DX, 03D4h + 0006h
                   IN AL, DX
                   MOV DX, 03C0h
                   MOV AL, 20h
                   OUT DX, AL
                   POP DX
                   POP AX
                   POP BP
                   RETF 2
Blink_00           ENDP
Blink              PROC FAR ; void far pascal Blink(unsigned int s_sw)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h]                ; s_sw
                   MAC_CallFar <CODE_TEXT>, <OFFSET CODE_TEXT:Blink_00>, <C_Blink>
                   POP BP
                   RETF 2
Blink              ENDP
SetBlink           PROC FAR ; void far pascal SetBlink(void far *s_function)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_function high
                   POP WORD PTR CODE_TEXT:C_Blink[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_function low
                   POP WORD PTR CODE_TEXT:C_Blink[DEF_Far_Offset]
                   POP BP
                   RETF 4
SetBlink           ENDP
PutsXY             PROC FAR ; void far pascal PutsXY
                   ; unsigned int s_attr         : 0Eh
                   ; void far *s_string          : 0Ah
                   ; unsigned int s_x            : 08h
                   ; unsigned int s_y            : 06h
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH SI
                   PUSH AX
                   LDS SI, DWORD PTR [BP + 0Ah]            ; s_string
                   CLD
                   MOV AH, BYTE PTR [BP + 0Eh]             ; s_attr
L_PutsXY_00        LABEL SHORT
                   LODSB
                   OR AL, AL
                   JZ SHORT L_PutsXY_01
                   PUSH AX
                   PUSH WORD PTR [BP + 08h]                ; s_x
                   PUSH WORD PTR [BP + 06h]                ; s_y
                   CALL FAR PTR CODE_TEXT:PutXY
                   INC WORD PTR [BP + 08h]                 ; s_x
                   JMP SHORT L_PutsXY_00
L_PutsXY_01        LABEL SHORT
                   POP AX
                   POP SI
                   POP DS
                   POP BP
                   RETF 2 + 4 + 2 + 2
PutsXY             ENDP
PutnXY             PROC FAR ; void far pascal PutnXY(unsigned int s_attr, void far *s_string, unsigned int s_length, unsigned int s_x, unsigned int s_y)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH SI
                   PUSH AX
                   PUSH CX
                   MOV CX, WORD PTR [BP + 0Ah]             ; s_length
                   JCXZ SHORT L_PutnXY_00
                   LDS SI, DWORD PTR [BP + 0Ch]            ; s_string
                   CLD
                   MOV AH, BYTE PTR [BP + 10h]             ; s_attr
L_PutnXY_00        LABEL SHORT
                   LODSB
                   PUSH AX
                   PUSH WORD PTR [BP + 08h]                ; s_x
                   PUSH WORD PTR [BP + 06h]                ; s_y
                   CALL FAR PTR CODE_TEXT:PutXY
                   INC WORD PTR [BP + 08h]                 ; s_x
                   LOOP SHORT L_PutnXY_00
L_PutnXY_01        LABEL SHORT
                   POP CX
                   POP AX
                   POP SI
                   POP DS
                   POP BP
                   RETF 2 + 4 + 2 + 2 + 2
PutnXY             ENDP
PutmXY             PROC FAR ; void far pascal PutmXY(unsigned int s_attr, void far *s_string, unsigned int s_x1, unsigned int s_x2, unsigned int s_y)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   PUSH WORD PTR [BP + 0Ch + DEF_Far_Segment] ; s_string high
                   PUSH WORD PTR [BP + 0Ch + DEF_Far_Offset] ; s_string low
                   CALL FAR PTR CODE_STRING:StrLen
                   SHR AX, 01h
                   PUSH AX
                   PUSH WORD PTR [BP + 0Ah]                ; s_x1
                   PUSH WORD PTR [BP + 08h]                ; s_x2
                   CALL FAR PTR CODE_CALC:Center
                   POP DX
                   SUB AX, DX
                   PUSH WORD PTR [BP + 10h]                ; s_attr
                   PUSH WORD PTR [BP + 0Ch + DEF_Far_Segment] ; s_string high
                   PUSH WORD PTR [BP + 0Ch + DEF_Far_Offset] ; s_string low
                   PUSH AX
                   PUSH WORD PTR [BP + 06h]                ; s_y
                   CALL FAR PTR CODE_TEXT:PutsXY
                   POP DX
                   POP AX
                   POP BP
                   RETF 2 + 4 + 2 + 2 + 2
PutmXY             ENDP
_PrintF            PROC FAR ; void far cdecl _PrintF(void far *s_format, ...)
PrintF             PROC FAR ; void far cdecl PrintF(void far *s_format, ...)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   PUSH DEF_PrintF_Max
                   CALL FAR PTR CODE_HEAP:New
                   PUSH DX
                   PUSH AX
                   PUSH DX
                   PUSH AX
                   PUSH DX
                   PUSH AX
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_format high
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_format low
                   PUSH SS
                   LEA AX, WORD PTR [BP + 0Ah]             ; ...
                   PUSH AX
                   CALL FAR PTR CODE_STRING:VSPrintf
                   CALL FAR PTR CODE_TEXT:Puts
                   CALL FAR PTR CODE_HEAP:Delete
                   POP DX
                   POP AX
                   POP BP
                   RETF
PrintF             ENDP
_PrintF            ENDP
_PrintK            PROC FAR ; void far cdecl _PrintK(void far *s_format, ...)
PrintK             PROC FAR ; void far cdecl PrintK(void far *s_format, ...)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   PUSH DEF_PrintK_Max
                   CALL FAR PTR CODE_HEAP:New
                   PUSH DX
                   PUSH AX
                   PUSH DX
                   PUSH AX
                   PUSH DX
                   PUSH AX
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_format high
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_format low
                   PUSH SS
                   LEA AX, WORD PTR [BP + 0Ah]             ; ...
                   PUSH AX
                   CALL FAR PTR CODE_STRING:VSPrintf
                   CALL FAR PTR CODE_TEXT:Puts
                   CALL FAR PTR CODE_HEAP:Delete
                   POP DX
                   POP AX
                   POP BP
                   RETF
PrintK             ENDP
_PrintK            ENDP
PutDump            PROC FAR ; void far pascal PutDump(void far *s_buffer, unsigned long s_base, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH SI
                   PUSH AX
                   PUSH CX                   
                   LDS SI, DWORD PTR [BP + 0Ch]            ; s_buffer
                   CLD
                   XOR CX, CX
L_PutDump_00       LABEL SHORT                   
                   PUSH CX
                   MOV CX, DEF_SIZE_Para
                   PUSH WORD PTR [BP + 08h + DEF_DWord_High]
                   PUSH WORD PTR [BP + 08h + DEF_DWord_Low]
                   PUSH DATA_TEXT
                   PUSH OFFSET DATA_TEXT:TEXT_PutDump_00
                   CALL FAR PTR CODE_TEXT:PrintF
                   ADD SP, 0004h + 0004h                   
                   PUSH DS
                   PUSH SI
L_PutDump_01       LABEL SHORT                   
                   LODSB
                   XOR AH, AH
                   PUSH AX
                   PUSH DATA_TEXT
                   PUSH OFFSET DATA_TEXT:TEXT_PutDump_01
                   CALL FAR PTR CODE_TEXT:PrintF
                   ADD SP, 0004h + 0002h
                   CMP CX, 09h
                   JNE L_PutDump_02
                   PUSH '-'
                   CALL FAR PTR CODE_TEXT:Putc
                   PUSH ' '
                   CALL FAR PTR CODE_TEXT:Putc
L_PutDump_02       LABEL SHORT                   
                   LOOP SHORT L_PutDump_01
                   PUSH DEF_SIZE_Para
                   CALL FAR PTR CODE_TEXT:PutN
                   PUSH DEF_ASCII_CarrigeReturn
                   CALL FAR PTR CODE_TEXT:Putc
                   PUSH DEF_ASCII_LineFeed
                   CALL FAR PTR CODE_TEXT:Putc
                   POP CX
                   ADD CX, DEF_SIZE_Para
                   ADD WORD PTR [BP + 08h + DEF_DWord_Low], DEF_SIZE_Para ; s_base low
                   ADC WORD PTR [BP + 08h + DEF_DWord_High], DEF_Null ; s_base high
                   CMP CX, WORD PTR [BP + 06h]             ; s_length
                   JB SHORT L_PutDump_00
                   POP CX
                   POP AX
                   POP SI
                   POP DS
                   POP BP
                   RETF 4 + 4 + 2
PutDump            ENDP
CODE_TEXT          ENDS

                   ASSUME CS:CODE_TEXT, DS:DATA_TEXT, ES:NOTHING, SS:STACK_DEFAULT
DATA_TEXT          SEGMENT
TEXT_VideoMemory   DW 0000h, 0B800h
TEXT_Page          DW 0000h
TEXT_Color         DW 0007h
TEXT_Character     DW DEF_ASCII_Space
TEXT_Res           DW 80d, 25d
TEXT_Window        DW 0d, 0d, 79d, 24d
TEXT_TabLength     DW 0008h
TEXT_LineTable     DB 0B3h, 0C4h, 0DAh, 0C2h, 0BFh, 0C3h, 0C5h, 0B4h, 0C0h, 0C1h, 0D9h, "Dump"
                   DB 0BAh, 0CDh, 0C9h, 0CBh, 0BBh, 0CCh, 0CEh, 0B9h, 0C8h, 0CAh, 0BCh, "Dump"
                   DB 0B3h, 0CDh, 0D5h, 0D1h, 0B8h, 0C6h, 0D8h, 0B5h, 0D4h, 0CFh, 0BEh, "Dump"
                   DB 0BAh, 0C4h, 0D6h, 0D2h, 0B7h, 0C7h, 0D7h, 0B6h, 0D3h, 0D0h, 0BDh, "Dump"
TEXT_PutDump_00    DB "[$08LX] ", DEF_ASCII_EndOfString
TEXT_PutDump_01    DB "$02X ", DEF_ASCII_EndOfString
DATA_TEXT          ENDS

                   ASSUME CS:CODE_TEXT, DS:BSS_TEXT, ES:NOTHING, SS:STACK_DEFAULT
BSS_TEXT           SEGMENT
BSS_TEXT           ENDS
                   END

; End of source
