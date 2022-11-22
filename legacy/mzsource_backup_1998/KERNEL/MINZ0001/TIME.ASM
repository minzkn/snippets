COMMENT #
 NOTE: 2000.10.03 [TUE]
  - Delay : 1/1000sec parameter   ex.) Delay(6000ms) = 6sec delay
#

DEF_ASM_TIME       EQU "TIME.ASM"

INCLUDE            COMMON.INC

PUBLIC             ResetTimer
PUBLIC             Delay
PUBLIC             TimerDummy, OpenTimer, CloseTimer, SetTimerRate, SetTimerCallBack, TimerHandler, GetClockTick

PUBLIC             TIMER_IRQ_Enabled
;
PUBLIC             TIMER_OldVector
PUBLIC             TIMER_IRQ_CallBack
PUBLIC             TIMER_IRQ_Active
PUBLIC             TIMER_Speed
PUBLIC             TIMER_Count
PUBLIC             TIMER_ClockTick

                   ASSUME CS:CODE_TIME, DS:DATA_TIME, ES:NOTHING, SS:STACK_DEFAULT
CODE_TIME          SEGMENT
ResetTimer         PROC FAR ; void far pascal ResetTimer(void)
                   PUSH AX
                   PUSHF
                   CLI
                   MOV AL, 00110110b
                   OUT 43h, AL
                   XOR AL, AL
                   OUT 40h, AL
                   MAC_ClearCache ; Delay for apply
                   OUT 40h, AL
                   MAC_ClearCache ; Delay for apply
                   POPF
                   POP AX
                   RETF
ResetTimer         ENDP
Delay              PROC FAR ; void far pascal Delay(unsigned int s_ms)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX                   
                   CALL FAR PTR CODE_TIME:OpenTimer
                   PUSH DEF_DELAY_Value
                   CALL FAR PTR CODE_TIME:SetTimerRate
L_Delay_00         LABEL SHORT                   
                   CALL FAR PTR CODE_TIME:GetClockTick
                   CMP AX, WORD PTR [BP + 06h]
                   JB SHORT L_Delay_00
                   CALL FAR PTR CODE_TIME:CloseTimer
                   POP DX
                   POP AX
                   POP BP
                   RETF 2
Delay              ENDP
TimerDummy         PROC FAR ; void far pascal TimerDummy(void)
                   PUSH DS
                   ASSUME DS:BSS_TIME
                   PUSH BSS_TIME
                   POP DS
                   ADD WORD PTR BSS_TIME:TIMER_ClockTick[DEF_DWord_Low], 0001h
                   ADC WORD PTR BSS_TIME:TIMER_ClockTick[DEF_DWord_High], DEF_Null
                   ASSUME DS:DATA_TIME
                   POP DS
                   RETF
TimerDummy         ENDP
OpenTimer          PROC FAR ; void far pascal OpenTimer(void)
                   PUSH DS
                   PUSH AX
                   PUSH DX
                   ASSUME DS:DATA_TIME
                   MOV AX, DATA_TIME
                   MOV DS, AX
                   CMP WORD PTR DATA_TIME:TIMER_IRQ_Enabled, DEF_False
                   JNE SHORT L_OpenTimer_00
                   ASSUME DS:BSS_TIME
                   MOV AX, BSS_TIME
                   MOV DS, AX
                   XOR AX, AX
                   MOV WORD PTR BSS_TIME:TIMER_ClockTick[DEF_DWord_Low], AX
                   MOV WORD PTR BSS_TIME:TIMER_ClockTick[DEF_DWord_High], AX
                   MOV WORD PTR BSS_TIME:TIMER_IRQ_CallBack[DEF_Far_Offset], OFFSET CODE_TIME:TimerDummy
                   MOV WORD PTR BSS_TIME:TIMER_IRQ_CallBack[DEF_Far_Segment], CODE_TIME
                   PUSH DEF_INT_Timer
                   CALL FAR PTR CODE_INT:GetVect
                   MOV WORD PTR BSS_TIME:TIMER_OldVector[DEF_Far_Offset], AX
                   MOV WORD PTR BSS_TIME:TIMER_OldVector[DEF_Far_Segment], DX
                   MOV WORD PTR BSS_TIME:TIMER_IRQ_Active, DEF_False
                   PUSH CODE_TIME
                   PUSH OFFSET CODE_TIME:TimerHandler
                   PUSH DEF_INT_Timer
                   CALL FAR PTR CODE_INT:SetVect
                   CALL FAR PTR CODE_TIME:ResetTimer
                   MOV WORD PTR BSS_TIME:TIMER_Speed, 0FFFFh
                   ASSUME DS:DATA_TIME
                   MOV AX, DATA_TIME
                   MOV DS, AX
                   INC WORD PTR DATA_TIME:TIMER_IRQ_Enabled
L_OpenTimer_00     LABEL SHORT
                   POP DX
                   POP AX
                   POP DS
                   RETF
OpenTimer          ENDP
CloseTimer         PROC FAR ; void far pascal CloseTimer(void)
                   PUSH DS
                   PUSH AX
                   ASSUME DS:DATA_TIME
                   MOV AX, DATA_TIME
                   MOV DS, AX
                   CMP WORD PTR DATA_TIME:TIMER_IRQ_Enabled, DEF_False
                   JE SHORT L_CloseTimer_00
                   CALL FAR PTR CODE_TIME:ResetTimer
                   ASSUME DS:BSS_TIME
                   MOV AX, BSS_TIME
                   MOV DS, AX
                   PUSH WORD PTR BSS_TIME:TIMER_OldVector[DEF_Far_Segment]
                   PUSH WORD PTR BSS_TIME:TIMER_OldVector[DEF_Far_Offset]
                   PUSH DEF_INT_Timer
                   CALL FAR PTR CODE_INT:SetVect
                   ASSUME DS:DATA_TIME
                   MOV AX, DATA_TIME
                   MOV DS, AX
                   DEC WORD PTR DATA_TIME:TIMER_IRQ_Enabled
L_CloseTimer_00    LABEL SHORT
                   POP AX
                   POP DS
                   RETF
CloseTimer         ENDP
SetTimerRate       PROC FAR ; void far pascal SetTimerRate(unsigned int s_rate)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH AX
                   PUSH DX
                   ASSUME DS:DATA_TIME
                   MOV AX, DATA_TIME
                   MOV DS, AX
                   CMP WORD PTR DATA_TIME:TIMER_IRQ_Enabled, DEF_False
                   JE SHORT L_SetTimerRate_00
                   MOV DX, 0012h
                   MOV AX, 34DEh
                   DIV WORD PTR [BP + 06h]                 ; s_rate
                   ASSUME DS:BSS_TIME
                   MOV DX, BSS_TIME
                   MOV DS, DX
                   MOV WORD PTR BSS_TIME:TIMER_Speed, AX
                   MOV DX, AX
                   XOR AX, AX
                   MOV WORD PTR BSS_TIME:TIMER_ClockTick[DEF_DWord_High], AX
                   MOV WORD PTR BSS_TIME:TIMER_ClockTick[DEF_DWord_Low], AX
                   PUSHF
                   CLI
                   MOV AL, 00110110b
                   OUT 43h, AL
                   MOV AL, DL
                   OUT 40h, AL
                   MAC_ClearCache
                   MOV AL, DH
                   OUT 40h, AL
                   MAC_ClearCache
                   POPF
                   ASSUME DS:DATA_TIME
L_SetTimerRate_00  LABEL SHORT                   
                   POP DX
                   POP AX
                   POP DS
                   POP BP
                   RETF 2
SetTimerRate       ENDP
SetTimerCallBack   PROC FAR ; void far pascal SetTimerCallBack(void far *s_callback)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH AX
                   ASSUME DS:BSS_TIME
                   MOV AX, BSS_TIME
                   MOV DS, AX
                   PUSHF
                   CLI
                   MOV AX, WORD PTR [BP + 06h + DEF_Far_Segment]
                   MOV WORD PTR BSS_TIME:TIMER_IRQ_CallBack[DEF_Far_Segment], AX
                   MOV AX, WORD PTR [BP + 06h + DEF_Far_Offset]
                   MOV WORD PTR BSS_TIME:TIMER_IRQ_CallBack[DEF_Far_Offset], AX
                   POPF
                   ASSUME DS:DATA_TIME
                   POP AX
                   POP DS
                   POP BP
                   RETF 4
SetTimerCallBack   ENDP
TimerHandler       PROC FAR ; void far pascal TimerHandler(void)
                   PUSHA
                   PUSH DS
                   ASSUME DS:BSS_TIME
                   MOV AX, BSS_TIME
                   MOV DS, AX
                   CMP WORD PTR BSS_TIME:TIMER_IRQ_Active, DEF_False
                   JNE SHORT L_TimerHandler_00
                   INC WORD PTR BSS_TIME:TIMER_IRQ_Active
                   CALL DWORD PTR BSS_TIME:TIMER_IRQ_CallBack
                   DEC WORD PTR BSS_TIME:TIMER_IRQ_Active
L_TimerHandler_00  LABEL SHORT
                   MOV AX, WORD PTR BSS_TIME:TIMER_Speed
                   ADD WORD PTR BSS_TIME:TIMER_Count, AX
                   JNC SHORT L_TimerHandler_01
                   PUSHF
                   CALL DWORD PTR BSS_TIME:TIMER_OldVector
                   MAC_JumpShort <L_TimerHandler_02>
L_TimerHandler_01  LABEL SHORT
                   MOV AL, 20h
                   OUT 20h, AL
L_TimerHandler_02  LABEL SHORT
                   POP DS
                   POPA
                   IRET
TimerHandler       ENDP
GetClockTick       PROC FAR ; unsigned long far pascal GetClockTick(void)
                   PUSH BP
                   MOV BP, SP
                   ASSUME DS:BSS_TIME
                   PUSH DS
                   MOV AX, BSS_TIME
                   MOV DS, AX
                   MOV DX, WORD PTR BSS_TIME:TIMER_ClockTick[DEF_DWord_High]
                   MOV AX, WORD PTR BSS_TIME:TIMER_ClockTick[DEF_DWord_Low]
                   POP DS
                   ASSUME DS:DATA_TIME
                   POP BP
                   RETF
GetClockTick       ENDP
CODE_TIME          ENDS

                   ASSUME CS:CODE_TIME, DS:DATA_TIME, ES:NOTHING, SS:STACK_DEFAULT
DATA_TIME          SEGMENT
TIMER_IRQ_Enabled  DW DEF_False                            ; Timer service enabled flag
DATA_TIME          ENDS

                   ASSUME CS:CODE_TIME, DS:BSS_TIME, ES:NOTHING, SS:STACK_DEFAULT
BSS_TIME           SEGMENT
TIMER_OldVector    DD ?                                    ; BIOS INT 08h routine
TIMER_IRQ_CallBack DD ?                                    ; User defined callback routine
TIMER_IRQ_Active   DW ?                                    ; Callback routine active flag
TIMER_Speed        DW ?                                    ; Timer speed in clock ticks
TIMER_Count        DW ?                                    ; Timer counter used to call the
TIMER_ClockTick    DD ?                                    ; Clock tick counter
BSS_TIME           ENDS
                   END

; End of source
