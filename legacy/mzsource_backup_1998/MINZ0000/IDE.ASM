COMMENT #
 NOTE: 2000.10.03 (TUE)
  - Reference
    "The SCSI Bus & IDE Interface (SECOND EDITION) : ASSISON-WESLEY"
    "Linux source (linux/kernel/hd.c) : (C) 1991 Linus Tovalds "
#

DEF_ASM_IDE        EQU "IDE.ASM"

INCLUDE            COMMON.INC

DEF_IDE_Fixed_00   = 01F0h
DEF_IDE_Fixed_01   = 0170h
;
DEF_IDE_PORT_Data  = 0000h
DEF_IDE_PORT_Error = 0001h
DEF_IDE_PORT_NSector = 0002h
DEF_IDE_PORT_Sector = 0003h
DEF_IDE_PORT_LCylinder = 0004h
DEF_IDE_PORT_HCylinder = 0005h
DEF_IDE_PORT_Current = 0006h
DEF_IDE_PORT_Status = 0007h
DEF_IDE_PORT_PreComp = DEF_IDE_PORT_Error
DEF_IDE_PORT_Command = DEF_IDE_PORT_Status
DEF_IDE_PORT_CMD   = 03F6h
;
DEF_IDE_PORT_CMD_SRST = 04h
;
DEF_IDE_ERR_AMNF   = 01h
DEF_IDE_ERR_TK0NF  = 02h
DEF_IDE_ERR_ABRT   = 04h
DEF_IDE_ERR_MCR    = 08h
DEF_IDE_ERR_IDNF   = 10h
DEF_IDE_ERR_MC     = 20h
DEF_IDE_ERR_UNC    = 40h
DEF_IDE_ERR_BBK    = 80h
;
DEF_IDE_STAT_ERR   = 01h
DEF_IDE_STAT_IDX   = 02h
DEF_IDE_STAT_CORR  = 04h
DEF_IDE_STAT_DRQ   = 08h
DEF_IDE_STAT_DSC   = 10h
DEF_IDE_STAT_DF    = 20h
DEF_IDE_STAT_DRDY  = 40h
DEF_IDE_STAT_BSY   = 80h
;
DEF_IDE_TimeOutCount = 0FFFFh
DEF_IDE_FixedMax   = 0002h
DEF_IDE_DeviceMax  = 0002h

PUBLIC             IDE_Identify
PUBLIC             IDE_Read, IDE_Write, IDE_Seek
;
PUBLIC             IDE_BasePort

                   ASSUME CS:CODE_IDE, DS:DATA_IDE, ES:NOTHING, SS:STACK_DEFAULT
CODE_IDE           SEGMENT
IDE_GetBasePort    PROC NEAR ; DX near pascal IDE_GetBasePort(unsigned int s_fixed, unsigned int s_index)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH BX
                   MOV BX, DATA_IDE
                   MOV DS, BX
                   MOV BX, WORD PTR [BP + 06h]             ; s_fixed
                   SHL BX, 01h
                   MOV DX, WORD PTR DATA_IDE:IDE_BasePort[BX]
                   ADD DX, WORD PTR [BP + 04h]             ; s_index
                   POP BX
                   POP DS
                   POP BP
                   RETN 2 + 2
IDE_GetBasePort    ENDP
IDE_WhileBusy      PROC NEAR ; CX near pascal IDE_WhileBusy(unsigned int s_fixed)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   PUSH WORD PTR [BP + 04h]                ; s_fixed
                   PUSH DEF_IDE_PORT_Status
                   CALL NEAR PTR CODE_IDE:IDE_GetBasePort
                   MOV CX, DEF_IDE_TimeOutCount
L_IDE_WhileBusy_00 LABEL SHORT
                   IN AL, DX
                   AND AL, (DEF_IDE_STAT_BSY OR DEF_IDE_STAT_DRDY OR DEF_IDE_STAT_DF OR DEF_IDE_STAT_DSC OR DEF_IDE_STAT_ERR)
                   CMP AL, DEF_IDE_STAT_DRDY OR DEF_IDE_STAT_DSC
                   LOOPNE SHORT L_IDE_WhileBusy_00
                   POP DX                   
                   POP AX
                   POP BP
                   RETN 2
IDE_WhileBusy      ENDP
IDE_WriteByte      PROC NEAR ; CX near pascal IDE_WriteByte(unsigned int s_fixed, unsigned int s_port, unsigned int s_byte)
                   PUSH BP
                   MOV BP, SP
                   PUSH WORD PTR [BP + 08h]                ; s_fixed
                   CALL NEAR PTR CODE_IDE:IDE_WhileBusy
                   JCXZ SHORT L_IDE_WriteByte_00
                   PUSH AX
                   PUSH DX
                   MOV DX, WORD PTR [BP + 06h]             ; s_port
                   MOV AL, BYTE PTR [BP + 04h]             ; s_byte
                   OUT DX, AL
                   POP DX
                   POP AX
L_IDE_WriteByte_00 LABEL SHORT
                   POP BP
                   RETN 2 + 2 + 2
IDE_WriteByte      ENDP
IDE_ReadByte       PROC NEAR ; CX, AX near pascal IDE_ReadByte(unsigned int s_fixed, unsigned int s_port)
                   PUSH BP
                   MOV BP, SP
                   XOR AX, AX
                   PUSH WORD PTR [BP + 06h]                ; s_fixed
                   CALL NEAR PTR CODE_IDE:IDE_WhileBusy
                   JCXZ SHORT L_IDE_ReadByte_00
                   PUSH DX
                   MOV DX, WORD PTR [BP + 04h]             ; s_port
                   IN AL, DX
                   POP DX
L_IDE_ReadByte_00  LABEL SHORT
                   POP BP
                   RETN 2 + 2
IDE_ReadByte       ENDP
MAC_IDE_WriteByte  MACRO m_fixed, m_port, m_byte
                   PUSH &m_fixed
                   PUSH &m_port
                   PUSH &m_byte
                   CALL NEAR PTR CODE_IDE:IDE_WriteByte
                   ENDM
IDE_Setup          PROC NEAR ; CX near pascal IDE_Setup
                   ; unsigned int s_fixed        : 10h
                   ; unsigned int s_device       : 0Eh
                   ; unsigned int s_head         : 0Ch
                   ; unsigned int s_cylinder     : 0Ah
                   ; unsigned int s_sector       : 08h
                   ; unsigned int s_nsector      : 06h
                   ; unsigned int s_command      : 04h
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   PUSH WORD PTR [BP + 10h]                ; s_fixed
                   PUSH DEF_IDE_PORT_Data
                   CALL NEAR PTR CODE_IDE:IDE_GetBasePort
                   ADD DX, 0002h ; The sector count register
                   MAC_IDE_WriteByte <WORD PTR [BP + 10h]>, <DX>, <WORD PTR [BP + 06h]>
                   JCXZ SHORT L_IDE_Setup_00
                   INC DX ; The sector number register
                   MAC_IDE_WriteByte <WORD PTR [BP + 10h]>, <DX>, <WORD PTR [BP + 08h]>
                   JCXZ SHORT L_IDE_Setup_00
                   INC DX ; The cylinder low-number register
                   XOR AH, AH
                   MOV AL, BYTE PTR [BP + 0Ah + DEF_Word_Low]
                   MAC_IDE_WriteByte <WORD PTR [BP + 10h]>, <DX>, <AX>
                   JCXZ SHORT L_IDE_Setup_00
                   INC DX ; The cylinder high-number register
                   XOR AH, AH
                   MOV AL, BYTE PTR [BP + 0Ah + DEF_Word_High]
                   MAC_IDE_WriteByte <WORD PTR [BP + 10h]>, <DX>, <AX>
                   JCXZ SHORT L_IDE_Setup_00
                   INC DX ; The drive / head register
                   MOV AL, BYTE PTR [BP + 0Ch]             ; s_head
                   AND AL, 0Fh
                   MOV AH, BYTE PTR [BP + 0Eh]             ; s_device
                   AND AH, 01h
                   SHL AH, 04h
                   OR AL, AH
                   OR AL, 0A0h ; CHS mode
                   XOR AH, AH
                   MAC_IDE_WriteByte <WORD PTR [BP + 10h]>, <DX>, <AX>
                   JCXZ SHORT L_IDE_Setup_00
                   INC DX ; The command register
                   MAC_IDE_WriteByte <WORD PTR [BP + 10h]>, <DX>, <WORD PTR [BP + 04h]>
                   JCXZ SHORT L_IDE_Setup_00
                   PUSH WORD PTR [BP + 10h]                ; s_fixed
                   CALL NEAR PTR CODE_IDE:IDE_WhileBusy
                   JCXZ SHORT L_IDE_Setup_00
L_IDE_Setup_00     LABEL SHORT
                   POP DX
                   POP AX
                   POP BP
                   RETN 2 + 2 + 2 + 2 + 2 + 2 + 2 
IDE_Setup          ENDP
IDE_Result         PROC NEAR ; unsigned int near pascal IDE_Result(unsigned int s_fixed)
                   PUSH BP
                   MOV BP, SP
                   PUSH DX
                   XOR AX, AX
                   PUSH WORD PTR [BP + 04h]                ; s_fixed
                   PUSH DEF_IDE_PORT_Status
                   CALL NEAR PTR CODE_IDE:IDE_GetBasePort
                   IN AL, DX
                   TEST AL, DEF_IDE_STAT_ERR
                   JZ SHORT L_IDE_Result_00
                   PUSH WORD PTR [BP + 04h]                ; s_fixed
                   PUSH DEF_IDE_PORT_Error
                   CALL NEAR PTR CODE_IDE:IDE_GetBasePort
                   IN AL, DX
                   MAC_JumpShort <L_IDE_Result_01>
L_IDE_Result_00    LABEL SHORT
                   MOV AH, AL
                   AND AL, (DEF_IDE_STAT_BSY OR DEF_IDE_STAT_DRDY OR DEF_IDE_STAT_DF OR DEF_IDE_STAT_DSC OR DEF_IDE_STAT_ERR)
                   CMP AL, DEF_IDE_STAT_DRDY OR DEF_IDE_STAT_DSC
                   JE SHORT L_IDE_Result_01
                   MOV AL, AH
                   MOV AH, 01h ; If status then AH=1 else AH=0
L_IDE_Result_01    LABEL SHORT
                   POP DX
                   POP BP
                   RETN 2
IDE_Result         ENDP
IDE_Identify       PROC FAR ; unsigned int far pascal IDE_Identify(unsigned int s_fixed, unsigned int s_device, void far *s_buffer)
                   PUSH BP
                   MOV BP, SP
                   PUSH CX
                   PUSH WORD PTR [BP + 0Ch]                ; s_fixed
                   PUSH WORD PTR [BP + 0Ah]                ; s_device
                   PUSH DEF_Null
                   PUSH DEF_Null
                   PUSH 0001h
                   PUSH 0001h
                   PUSH 00ECh ; Identify command
                   CALL NEAR PTR CODE_IDE:IDE_Setup
                   JCXZ SHORT L_IDE_Identify_00
                   PUSH DX
                   PUSH WORD PTR [BP + 0Ch]                ; s_fixed
                   PUSH DEF_IDE_PORT_Data
                   CALL NEAR PTR CODE_IDE:IDE_GetBasePort
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_buffer high
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset] ; s_buffer low
                   PUSH DX
                   PUSH DEF_SIZE_Sector SHR 01h
                   CALL FAR PTR CODE_PORT:InPortsW
                   POP DX
L_IDE_Identify_00  LABEL SHORT
                   PUSH WORD PTR [BP + 0Ch]                ; s_fixed
                   CALL NEAR PTR CODE_IDE:IDE_Result
                   POP CX
                   POP BP
                   RETF 2 + 2 + 4
IDE_Identify       ENDP
IDE_Read           PROC FAR ; unsigned int far pascal IDE_Read
                   ; unsigned int s_fixed        : 12h
                   ; unsigned int s_device       : 10h
                   ; unsigned int s_head         : 0Eh
                   ; unsigned int s_cylinder     : 0Ch
                   ; unsigned int s_sector       : 0Ah
                   ; void far * s_buffer         : 06h
                   PUSH BP
                   MOV BP, SP
                   PUSH CX
                   PUSH WORD PTR [BP + 12h]                ; s_fixed
                   PUSH WORD PTR [BP + 10h]                ; s_device
                   PUSH WORD PTR [BP + 0Eh]                ; s_head
                   PUSH WORD PTR [BP + 0Ch]                ; s_cylinder
                   PUSH WORD PTR [BP + 0Ah]                ; s_sector
                   PUSH 0001h
                   PUSH 0020h ; Read sector command
                   CALL NEAR PTR CODE_IDE:IDE_Setup
                   JCXZ SHORT L_IDE_Read_00
                   PUSH DX
                   PUSH WORD PTR [BP + 12h]                ; s_fixed
                   PUSH DEF_IDE_PORT_Data
                   CALL NEAR PTR CODE_IDE:IDE_GetBasePort
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_buffer high
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset] ; s_buffer low
                   PUSH DX
                   PUSH DEF_SIZE_Sector SHR 01h
                   CALL FAR PTR CODE_PORT:InPortsW
                   POP DX
L_IDE_Read_00      LABEL SHORT
                   PUSH WORD PTR [BP + 12h]                ; s_fixed
                   CALL NEAR PTR CODE_IDE:IDE_Result
                   POP CX
                   POP BP
                   RETF 2 + 2 + 2 + 2 + 2 + 4
IDE_Read           ENDP
IDE_Write          PROC FAR ; unsigned int far pascal IDE_Write
                   ; unsigned int s_fixed        : 12h
                   ; unsigned int s_device       : 10h
                   ; unsigned int s_head         : 0Eh
                   ; unsigned int s_cylinder     : 0Ch
                   ; unsigned int s_sector       : 0Ah
                   ; void far * s_buffer         : 06h
                   PUSH BP
                   MOV BP, SP
                   PUSH CX
                   PUSH WORD PTR [BP + 12h]                ; s_fixed
                   PUSH WORD PTR [BP + 10h]                ; s_device
                   PUSH WORD PTR [BP + 0Eh]                ; s_head
                   PUSH WORD PTR [BP + 0Ch]                ; s_cylinder
                   PUSH WORD PTR [BP + 0Ah]                ; s_sector
                   PUSH 0001h
                   PUSH 0030h ; Write sector command
                   CALL NEAR PTR CODE_IDE:IDE_Setup
                   JCXZ SHORT L_IDE_Write_00
                   PUSH DX
                   PUSH WORD PTR [BP + 12h]                ; s_fixed
                   PUSH DEF_IDE_PORT_Data
                   CALL NEAR PTR CODE_IDE:IDE_GetBasePort
                   PUSH WORD PTR [BP + 06h + DEF_Far_Segment] ; s_buffer high
                   PUSH WORD PTR [BP + 06h + DEF_Far_Offset] ; s_buffer low
                   PUSH DX
                   PUSH DEF_SIZE_Sector SHR 01h
                   CALL FAR PTR CODE_PORT:OutPortsW
                   POP DX
L_IDE_Write_00     LABEL SHORT
                   PUSH WORD PTR [BP + 12h]                ; s_fixed
                   CALL NEAR PTR CODE_IDE:IDE_Result
                   POP CX
                   POP BP
                   RETF 2 + 2 + 2 + 2 + 2 + 4
IDE_Write          ENDP
IDE_Seek           PROC FAR ; unsigned int far pascal IDE_Seek
                   PUSH BP
                   MOV BP, SP
                   ; unsigned int s_fixed        : 0Eh
                   ; unsigned int s_device       : 0Ch
                   ; unsigned int s_head         : 0Ah
                   ; unsigned int s_cylinder     : 08h
                   ; unsigned int s_sector       : 06h                   
                   PUSH CX
                   PUSH WORD PTR [BP + 0Eh]                ; s_fixed
                   PUSH WORD PTR [BP + 0Ch]                ; s_device
                   PUSH WORD PTR [BP + 0Ah]                ; s_head
                   PUSH WORD PTR [BP + 08h]                ; s_cylinder
                   PUSH WORD PTR [BP + 06h]                ; s_sector
                   PUSH 0001h
                   PUSH 0070h ; Write sector command
                   CALL NEAR PTR CODE_IDE:IDE_Setup
                   JCXZ SHORT L_IDE_Seek_00
L_IDE_Seek_00      LABEL SHORT
                   PUSH WORD PTR [BP + 0Eh]                ; s_fixed
                   CALL NEAR PTR CODE_IDE:IDE_Result
                   POP CX
                   POP BP
                   RETF 2 + 2 + 2 + 2 + 2
IDE_Seek           ENDP
CODE_IDE           ENDS

                   ASSUME CS:CODE_IDE, DS:DATA_IDE, ES:NOTHING, SS:STACK_DEFAULT
DATA_IDE           SEGMENT
IDE_BasePort       DW DEF_IDE_Fixed_00
                   DW DEF_IDE_Fixed_01
DATA_IDE           ENDS

                   ASSUME CS:CODE_IDE, DS:BSS_IDE, ES:NOTHING, SS:STACK_DEFAULT
BSS_IDE            SEGMENT
BSS_IDE            ENDS
                   END

; End of source
