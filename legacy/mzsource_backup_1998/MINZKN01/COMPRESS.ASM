COMMENT #

#

DEF_ASM_COMPRESS   EQU "COMPRESS.ASM"

INCLUDE            COMMON.INC

PUBLIC             HUF_Encode, HUF_Decode
PUBLIC             Encode, Decode, SetCode

                   ASSUME CS:CODE_COMPRESS, DS:DATA_COMPRESS, ES:NOTHING, SS:STACK_DEFAULT
CODE_COMPRESS      SEGMENT
HUF_Encode         PROC FAR ; unsigned int far pascal HUF_Encode(void far *s_dest, void far *s_src, unsigned int s_srclength)
                   PUSH BP
                   MOV BP, SP

                   POP BP
                   RETF 4 + 4 + 2
HUF_Encode         ENDP
HUF_Decode         PROC FAR ; unsigned int far pascal HUF_Decode(void far *s_dest, void far *s_src)
                   PUSH BP
                   MOV BP, SP

                   POP BP
                   RETF 4 + 4
HUF_Decode         ENDP
Encode             PROC FAR ; unsigned int far pascal Encode(void far *s_dest, void far *s_src, unsigned int s_srclength)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 0Ch + DEF_Far_Segment] ; s_dest high
                   PUSH WORD PTR [BP + 0Ch + DEF_Far_Offset]  ; s_dest low
                   PUSH WORD PTR [BP + 08h + DEF_Far_Segment] ; s_src high
                   PUSH WORD PTR [BP + 08h + DEF_Far_Offset]  ; s_src low                   
                   PUSH WORD PTR [BP + 06h]                ; s_srclength
                   MAC_CallFar <CODE_COMPRESS>, <OFFSET CODE_COMPRESS:HUF_Encode>, <C_Encode>
                   POP BP
                   RETF 4 + 4 + 2
Encode             ENDP
Decode             PROC FAR ; unsigned int far pascal Decode(void far *s_dest, void far *s_src)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Segment] ; s_dest high
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Offset]  ; s_dest low
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_src high
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_src low                   
                   MAC_CallFar <CODE_COMPRESS>, <OFFSET CODE_COMPRESS:HUF_Decode>, <C_Decode>
                   POP BP
                   RETF 4 + 4
Decode             ENDP
SetCode            PROC FAR ; void far pascal SetCode(void far *s_encode, void far *s_decode)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Segment] ; s_encode high
                   PUSH WORD PTR [BP + 0Ah + DEF_Far_Offset]  ; s_encode low
                   POP WORD PTR CODE_COMPRESS:C_Encode[DEF_Far_Offset]
                   POP WORD PTR CODE_COMPRESS:C_Encode[DEF_Far_Segment]
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_decode high
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset]  ; s_decode low                   
                   POP WORD PTR CODE_COMPRESS:C_Decode[DEF_Far_Offset]
                   POP WORD PTR CODE_COMPRESS:C_Decode[DEF_Far_Segment]
                   POP BP
                   RETF 4 + 4
SetCode            ENDP
CODE_COMPRESS      ENDS

                   ASSUME CS:CODE_COMPRESS, DS:DATA_COMPRESS, ES:NOTHING, SS:STACK_DEFAULT
DATA_COMPRESS      SEGMENT
DATA_COMPRESS      ENDS

                   ASSUME CS:CODE_COMPRESS, DS:BSS_COMPRESS, ES:NOTHING, SS:STACK_DEFAULT
BSS_COMPRESS       SEGMENT
BSS_COMPRESS       ENDS
                   END 

; End of source
