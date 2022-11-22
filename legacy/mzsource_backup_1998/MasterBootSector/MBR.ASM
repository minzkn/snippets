COMMENT #
 Filename          : mbr.asm
 Copyright         : Code by JaeHyuk CHO
 Release           : 2001.07.10 - 0.0.1

 Master boot record information
 OFFSET : Size   Comment
    00H : byte   boot indicator
          00H - non-bootable partition
          80H - bootable partition (one partition only but minzkn is first 80H)
    01H : byte   beginning sector head number
    02H : byte   beginning sector (2 high bits of cylinder)
    03H : byte   beginning cylinder (low order bits of cylinder)
    04H : byte   system indicator
          00H - unknown operating system (Empty partition)
          01H - FAT12
          02H - XENIX
          04H - FAT16
          05H - Extended partition       (DOS)
          06H - FAT16                    (32bit sector number)
          0BH - FAT32
          0CH - FAT32                    (64bit sector number)
          51H - Extended partition       (Ontrack)
          64H - Novell
          75H - PCIX
          82H - Linux swap
          83H - Linux native
          DBH - CP/M
          FFH - BBT
    05H : byte   ending sector head number
    06H : byte   ending sector (2 high bits of cylinder)
          񽼖񽔼񼭢񼆈  1st byte  (sector)
          � � 滥聊聊聊聊聊哪 Sector offset within cylinder
          滥聊哪哪哪哪哪哪哪 High order bits of cylinder
    07H : byte   ending cylinder (low order bits of cylinder)
          񽼖񽔼񼭢񼆈   2nd byte  (cylinder)
          滥聊聊聊聊聊聊哪哪 Low order bits of cylinder
    08H : dword  number of sectors preceding the partition
    0BH : dword  number of sectors in the partition
#

.186                                             ; Intel 80x86 CPU
.RADIX 000AH                                     ; Ten base - But...

DEF_SEGMENT_STACK  = 0050H                       ; 0050H:0000H
DEF_SEGMENT_CODE   = 0070H                       ; 0070H:0000H
DEF_SEGMENT_BOOT   = 07C0H                       ; 07C0H:0000H

                   ASSUME CS:CODE_BOOT, DS:NOTHING, ES:NOTHING, SS:NOTHING
CODE_BOOT          SEGMENT PARA PUBLIC USE16 'CLASS_CODE_BOOT'
                   ORG 0000H
L_BootUP:       
                   ; === Stack
                   CLI
                   MOV SP, DEF_SEGMENT_STACK
                   MOV SS, SP
                   MOV SP, 0200H
                   STI                           

                   ; === Get base IP register
                   CALL NEAR PTR CODE_BOOT:GetBaseIP
GetBaseIP          PROC NEAR
L_GetBaseIP:
                   POP SI
                   SUB SI, OFFSET CODE_BOOT:L_GetBaseIP - OFFSET CODE_BOOT:L_BootUP
GetBaseIP          ENDP                          

                   ; === Move MBR code 
                   PUSH CS
                   POP DS
                   MOV AX, DEF_SEGMENT_CODE
                   MOV ES, AX
                   XOR DI, DI                   
                   MOV CX, (0200H SHR 01H)
                   CLD
                   REPZ MOVSW

                   ; === Jump to it
                   MOV AX, OFFSET CODE_BOOT:L_NewBootUP
                   PUSH ES                   
                   PUSH AX
                   RETF

                   ; === Message data
L_ErrorMessage_00  DB 0DH, 0AH, 'Invalid partition!', 0DH, 0AH, 00H
L_ErrorMessage_01  DB 0DH, 0AH, 'Disk I/O error!', 0DH, 0AH, 00H
L_BootupMessage    DB 0DH, 0AH, '[MINZKN] Loading boot sector...', 0DH, 0AH, 00H
L_OtherMessage     DB 0DH, 0AH, 'Multi active boot sector!', 0DH, 0AH, 00H

L_NewBootUP:
                   
                   ; === Search(80H) boot partition
                   PUSH CS
                   POP DS
                   MOV SI, OFFSET CODE_BOOT:L_Partition
                   MOV CL, 04H ; Optimize - MOV CX, 0004H
L_IsBoot:
                   CMP BYTE PTR CODE_BOOT:[SI], 80H
                   JE SHORT CODE_BOOT:L_ReadyOK
                   ADD SI, 0010H
                   LOOP SHORT CODE_BOOT:L_IsBoot
                   MOV SI, OFFSET CODE_BOOT:L_ErrorMessage_00
                   JMP SHORT CODE_BOOT:L_Error
L_ReadyOK:
                   PUSH SI

                   ; === Search(80H) other boot partition
                   JCXZ SHORT CODE_BOOT:L_OtherReadyOK
L_IsOtherBoot:
                   CMP BYTE PTR CODE_BOOT:[SI], 80H
                   JNE SHORT CODE_BOOT:L_IsOther
                   PUSH SI
                   MOV SI, OFFSET CODE_BOOT:L_OtherMessage
                   CALL NEAR PTR CODE_BOOT:Puts
                   POP SI
L_IsOther:
                   ADD SI, 0010H
                   LOOP SHORT CODE_BOOT:L_IsOtherBoot
L_OtherReadyOK:
                   
                   ; === Check disk
                   MOV CL, 04H ; Optimize - MOV CX, 0004H
L_IsDisk:
                   MOV SI, OFFSET CODE_BOOT:L_ErrorMessage_01
                   XOR AX, AX
                   INT 13H                   
                   JNC SHORT CODE_BOOT:L_DiskOK
                   LOOP SHORT L_IsDisk
                   JMP SHORT CODE_BOOT:L_Error
L_DiskOK:

                   POP SI
                   ; === Load boot sector
                   MOV CL, 04H ; Optimize - MOV CX, 0004H
L_IsRead:
                   MOV AX, DEF_SEGMENT_BOOT
                   MOV ES, AX
                   MOV WORD PTR CODE_BOOT:D_JumpAddress[0002H], AX
                   MOV AX, 0201H
                   XOR BX, BX
                   MOV CX, WORD PTR CODE_BOOT:[SI + 02H]
                   MOV DX, WORD PTR CODE_BOOT:[SI + 00H]
                   MOV SI, OFFSET CODE_BOOT:L_ErrorMessage_01
                   INT 13H
                   JNC SHORT CODE_BOOT:L_ReadOK
                   LOOP SHORT CODE_BOOT:L_IsRead
                   JMP SHORT CODE_BOOT:L_Error
L_ReadOK:
         
                   ; === Display message
                   MOV SI, OFFSET CODE_BOOT:L_BootupMessage
                   CALL NEAR PTR CODE_BOOT:Puts

                   ; === Jump to boot code (Intel 80x86)
                   DB 0EAH
D_JumpAddress      DW 0000H, 0FFFFH

                   ; === Print procedure
Puts               PROC NEAR
                   CLD
L_Puts_00:
                   LODSB
                   OR AL, AL
                   JZ SHORT L_Puts_01
                   MOV AH, 0EH
                   MOV BX, 0700H
                   INT 10H
                   JMP SHORT L_Puts_00
L_Puts_01:
                   RETN
Puts               ENDP

                   ; === Error 
L_Error:
                   CALL NEAR PTR CODE_BOOT:Puts
                   XOR AX, AX
                   INT 16H
                   JMP NEAR PTR L_NewBootUP

                   ; === Partition table
                   ORG 01BEH
L_Partition:
                   ; --- 01BEH = Partition 1 data table (16 bytes)
D_Partition_1      DB 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H
                   DB 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H
                   ; --- 01CEH = Partition 2 data table (16 bytes)
D_Partition_2      DB 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H
                   DB 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H
                   ; --- 01DEH = Partition 3 data table (16 bytes)
D_Partition_3      DB 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H
                   DB 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H
                   ; --- 01EEH = Partition 4 data table (16 bytes)
D_Partition_4      DB 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H
                   DB 00H, 00H, 00H, 00H, 00H, 00H, 00H, 00H
                   ; --- 01FEH = Signature 2 bytes
D_Partition_Check  DB 55H, 0AAH
CODE_BOOT          ENDS
                   END L_BootUP

; End of MBR code
