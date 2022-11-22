COMMENT #

#

DEF_ASM_KERNEL     EQU "KENEL.ASM"

INCLUDE            COMMON.INC

PUBLIC             KernelMain                              ; Kernel main procedure
;
PUBLIC             S_STR_Company, S_STR_Name, S_STR_VersionHigh, S_STR_VersionLow, S_STR_VersionType

                   ASSUME CS:CODE_KERNEL, DS:DATA_KERNEL, ES:NOTHING, SS:STACK_DEFAULT
CODE_KERNEL        SEGMENT
KernelMain         PROC FAR ; void far pascal KernelMain(void)                   
                   PUSH DS
                   PUSH ES
                   PUSH FS
                   PUSH GS
                   PUSHAD
                   ; Init segment
                   MOV AX, DATA_KERNEL
                   MOV DS, AX
                   ; Print logo message
                   PUSH DATA_KERNEL
                   PUSH OFFSET DATA_KERNEL:S_STR_Company
                   PUSH DATA_KERNEL
                   PUSH OFFSET DATA_KERNEL:S_STR_VersionType
                   PUSH WORD PTR DATA_KERNEL:S_STR_VersionLow
                   PUSH WORD PTR DATA_KERNEL:S_STR_VersionHigh
                   PUSH DATA_KERNEL
                   PUSH OFFSET DATA_KERNEL:S_STR_Name
                   PUSH DATA_KERNEL
                   PUSH OFFSET DATA_KERNEL:S_MSG_Logo
                   CALL FAR PTR CODE_TEXT:PrintF
                   ADD SP, 0004h + 0004h + 0002h + 0002h + 0004h + 0004h
                   ; Print minimum memory above kernel
                   ASSUME DS:BSS_INFO
                   PUSH DS
                   MOV AX, BSS_INFO
                   MOV DS, AX
                   PUSH WORD PTR BSS_INFO:INFO_ProgramSize[DEF_DWord_High]
                   PUSH WORD PTR BSS_INFO:INFO_ProgramSize[DEF_DWord_Low]
                   PUSH DATA_KERNEL
                   PUSH OFFSET DATA_KERNEL:S_MSG_ProgramSize
                   CALL FAR PTR CODE_TEXT:PrintF
                   ADD SP, 0004h + 0004h
                   POP DS
                   ASSUME DS:DATA_KERNEL



                   ; Print shutdown message
                   PUSH DATA_KERNEL
                   PUSH OFFSET DATA_KERNEL:S_STR_Name
                   PUSH DATA_KERNEL
                   PUSH OFFSET DATA_KERNEL:S_MSG_Shutdown
                   CALL FAR PTR CODE_TEXT:PrintF
                   ADD SP, 0004h + 0004h
                   ; Return
                   POPAD
                   POP GS
                   POP FS
                   POP ES
                   POP DS
                   RETF
KernelMain         ENDP
CODE_KERNEL        ENDS

                   ASSUME CS:CODE_KERNEL, DS:DATA_KERNEL, ES:NOTHING, SS:STACK_DEFAULT
DATA_KERNEL        SEGMENT
S_STR_Company      DB DEF_Company, DEF_ASCII_EndOfString
S_STR_Name         DB DEF_Name, DEF_ASCII_EndOfString
S_STR_VersionHigh  DW DEF_Version_High
S_STR_VersionLow   DW DEF_Version_Low
S_STR_VersionType  DB DEF_Version_Type, DEF_ASCII_EndOfString
S_MSG_Logo         DB 0FEh, " $s operating system  Version$X.$02X $s  Copyright (c) $s"
                   DB DEF_ASCII_CarrigeReturn, DEF_ASCII_LineFeed
                   DB DEF_ASCII_CarrigeReturn, DEF_ASCII_LineFeed
                   DB DEF_ASCII_EndOfString
S_MSG_Shutdown     DB DEF_ASCII_CarrigeReturn, DEF_ASCII_LineFeed
                   DB 0FEh, " Shutdown $s operating system."
                   DB DEF_ASCII_CarrigeReturn, DEF_ASCII_LineFeed
                   DB DEF_ASCII_CarrigeReturn, DEF_ASCII_LineFeed
                   DB DEF_ASCII_EndOfString
S_MSG_ProgramSize  DB 0FEh, " Minimun memory above kernel : $,LU + ", 0E0h, " Bytes."
                   DB DEF_ASCII_CarrigeReturn, DEF_ASCII_LineFeed
                   DB DEF_ASCII_EndOfString
DATA_KERNEL        ENDS

                   ASSUME CS:CODE_KERNEL, DS:BSS_KERNEL, ES:NOTHING, SS:STACK_DEFAULT
BSS_KERNEL         SEGMENT
BSS_KERNEL         ENDS
                   END 

; End of source
