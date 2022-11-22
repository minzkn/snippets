COMMENT #

#

DEF_ASM_IS         EQU "IS.ASM"

INCLUDE            COMMON.INC

PUBLIC             IsUpper, IsLower
PUBLIC             IsSpace
PUBLIC             IsDigit, IsHex
PUBLIC             IsControl
PUBLIC             IsNest

PUBLIC             T_IS

                   ASSUME CS:CODE_IS, DS:DATA_IS, ES:NOTHING, SS:STACK_DEFAULT
CODE_IS            SEGMENT
P_IS_LoadTable     PROC NEAR ; unsigned int near pascal P_IS_LoadTable(unsigned int s_number, unsigned int s_mask)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH BX
                   MOV AX, DATA_IS
                   MOV DS, AX
                   XOR AX, AX
                   MOV BL, BYTE PTR [BP + 06h]             ; s_character
                   XOR BH, BH
                   MOV BL, BYTE PTR DATA_IS:T_IS[BX + DEF_NULL]
                   TEST BL, BYTE PTR [BP + 04h]            ; s_mask
                   JZ SHORT CODE_IS:L_IS_LoadTable_00
                   INC AL
L_IS_LoadTable_00  LABEL SHORT
                   POP BX
                   POP DS
                   POP BP
                   RETN 2 + 2
P_IS_LoadTable     ENDP
IsUpper            PROC FAR ; unsigned int far pascal IsUpper(unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h]                ; s_character
                   PUSH DEF_IS_UPP
                   CALL NEAR PTR CODE_IS:P_IS_LoadTable
                   POP BP
                   RETF 2
IsUpper            ENDP
IsLower            PROC FAR ; unsigned int far pascal IsLower(unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h]                ; s_character
                   PUSH DEF_IS_LOW
                   CALL NEAR PTR CODE_IS:P_IS_LoadTable
                   POP BP
                   RETF 2
IsLower            ENDP
IsSpace            PROC FAR ; unsigned int far pascal IsSpace(unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h]                ; s_character
                   PUSH DEF_IS_SP 
                   CALL NEAR PTR CODE_IS:P_IS_LoadTable
                   POP BP
                   RETF 2
IsSpace            ENDP
IsDigit            PROC FAR ; unsigned int far pascal IsDigit(unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h]                ; s_character
                   PUSH DEF_IS_DIG
                   CALL NEAR PTR CODE_IS:P_IS_LoadTable
                   POP BP
                   RETF 2
IsDigit            ENDP
IsHex              PROC FAR ; unsigned int far pascal IsHex(unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h]                ; s_character
                   PUSH DEF_IS_HEX OR DEF_IS_DIG
                   CALL NEAR PTR CODE_IS:P_IS_LoadTable
                   POP BP
                   RETF 2
IsHex              ENDP
IsControl          PROC FAR ; unsigned int far pascal IsControl(unsigned int s_character)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 06h]                ; s_character
                   PUSH DEF_IS_CTL
                   CALL NEAR PTR CODE_IS:P_IS_LoadTable
                   POP BP
                   RETF 2
IsControl          ENDP
IsNest             PROC FAR ; unsigned int far pascal IsNest(unsigned int s_min, unsigned int s_max, unsigned int s_number)
                   PUSH BP
                   MOV BP, SP
                   MOV AX, WORD PTR [BP + 06h]             ; s_number
                   CMP AX, WORD PTR [BP + 08h]             ; s_max
                   JBE SHORT CODE_IS:L_IsNest_01
L_IsNest_00        LABEL SHORT
                   MOV AX, 0FFFFh
                   JMP SHORT CODE_IS:L_IsNest_02
L_IsNest_01        LABEL SHORT
                   CMP AX, WORD PTR [BP + 0Ah]             ; s_min
                   JB SHORT CODE_IS:L_IsNest_00
                   SUB AX, WORD PTR [BP + 0Ah]             ; s_min
L_IsNest_02        LABEL SHORT                   
                   POP BP
                   RETF 2 + 2 + 2
IsNest             ENDP
CODE_IS            ENDS

                   ASSUME CS:CODE_IS, DS:DATA_IS, ES:NOTHING, SS:STACK_DEFAULT
DATA_IS            SEGMENT
T_IS               DB DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL
                   DB DEF_IS_CTL, DEF_IS_CTL OR DEF_IS_SP, DEF_IS_SP OR DEF_IS_CTL, DEF_IS_SP OR DEF_IS_CTL, DEF_IS_SP OR DEF_IS_CTL, DEF_IS_SP OR DEF_IS_CTL, DEF_IS_CTL,DEF_IS_CTL
                   DB DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL,DEF_IS_CTL
                   DB DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL, DEF_IS_CTL,DEF_IS_CTL
                   DB DEF_IS_SP,  DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN,DEF_IS_PUN
                   DB DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN,DEF_IS_PUN
                   DB DEF_IS_DIG, DEF_IS_DIG, DEF_IS_DIG, DEF_IS_DIG, DEF_IS_DIG, DEF_IS_DIG, DEF_IS_DIG,DEF_IS_DIG
                   DB DEF_IS_DIG, DEF_IS_DIG, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN,DEF_IS_PUN
                   DB DEF_IS_PUN, DEF_IS_UPP OR DEF_IS_HEX, DEF_IS_HEX OR DEF_IS_UPP, DEF_IS_UPP OR DEF_IS_HEX, DEF_IS_UPP OR DEF_IS_HEX, DEF_IS_UPP OR DEF_IS_HEX, DEF_IS_UPP OR DEF_IS_HEX, DEF_IS_UPP
                   DB DEF_IS_UPP, DEF_IS_UPP, DEF_IS_UPP, DEF_IS_UPP, DEF_IS_UPP, DEF_IS_UPP, DEF_IS_UPP,DEF_IS_UPP
                   DB DEF_IS_UPP, DEF_IS_UPP, DEF_IS_UPP, DEF_IS_UPP, DEF_IS_UPP, DEF_IS_UPP, DEF_IS_UPP,DEF_IS_UPP
                   DB DEF_IS_UPP, DEF_IS_UPP, DEF_IS_UPP, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN,DEF_IS_PUN
                   DB DEF_IS_PUN, DEF_IS_LOW OR DEF_IS_HEX, DEF_IS_HEX OR DEF_IS_LOW, DEF_IS_LOW OR DEF_IS_HEX, DEF_IS_LOW OR DEF_IS_HEX, DEF_IS_LOW OR DEF_IS_HEX, DEF_IS_LOW OR DEF_IS_HEX, DEF_IS_LOW
                   DB DEF_IS_LOW, DEF_IS_LOW, DEF_IS_LOW, DEF_IS_LOW, DEF_IS_LOW, DEF_IS_LOW, DEF_IS_LOW,DEF_IS_LOW
                   DB DEF_IS_LOW, DEF_IS_LOW, DEF_IS_LOW, DEF_IS_LOW, DEF_IS_LOW, DEF_IS_LOW, DEF_IS_LOW,DEF_IS_LOW
                   DB DEF_IS_LOW, DEF_IS_LOW, DEF_IS_LOW, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN, DEF_IS_PUN,DEF_IS_CTL
DATA_IS            ENDS

                   ASSUME CS:CODE_IS, DS:BSS_IS, ES:NOTHING, SS:STACK_DEFAULT
BSS_IS             SEGMENT
BSS_IS             ENDS
                   END

; End of source
