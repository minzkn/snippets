COMMENT #
 NOTE : 2000.10.07 (SAT)
  - MZLIB : "DOS.LIB"
#

DEF_ASM_DOS        EQU "DOS.ASM"

INCLUDE            COMMON.INC

PUBLIC             DOS_chdir
PUBLIC             DOS_findfirst
PUBLIC             DOS_findnext
PUBLIC             DOS_getcurdir
PUBLIC             DOS_getcwd
PUBLIC             DOS_getdisk
PUBLIC             DOS_mkdir
PUBLIC             DOS_rmdir
PUBLIC             DOS_setdisk
PUBLIC             DOS_creat
PUBLIC             DOS_open
PUBLIC             DOS_close
PUBLIC             DOS_unlink
PUBLIC             DOS_read
PUBLIC             DOS_write
PUBLIC             DOS_lseek
PUBLIC             DOS_getfileattr
PUBLIC             DOS_setfileattr

                   ASSUME CS:CODE_DOS, DS:DATA_DOS, ES:NOTHING, SS:STACK_DEFAULT
CODE_DOS           SEGMENT
DOS_chdir          PROC FAR ; unsigned int far pascal DOS_chdir(void far *)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 3B00h
                   LDS DX, DWORD PTR [BP + 06h]            ; Path string pointer (End of ASCII_ENDSTRING)
                   INT DEF_INT_Dos
                   POP DS
                   POP BP
                   RETF 4
DOS_chdir          ENDP
DOS_findfirst      PROC FAR ; unsigned int far pascal DOS_findfirst(void far *, void far *, unsigned int)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 1A00h
                   LDS DX, DWORD PTR [BP + 08h]            ; File name list string
                   INT DEF_INT_Dos
                   MOV AX, 4E00h
                   MOV CX, WORD PTR [BP + 06h]             ; Attrib
                   LDS DX, DWORD PTR [BP + 0Ch]            ; DTA
                   INT DEF_INT_Dos
                   POP DS
                   POP BP
                   RETF 4 + 4 + 2
DOS_findfirst      ENDP
DOS_findnext       PROC FAR ; unsigned int far pascal DOS_findnext(void far *)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 1A00h
                   LDS DX, DWORD PTR [BP + 06h]            ; DTA
                   INT DEF_INT_Dos
                   MOV AX, 4F00h
                   INT DEF_INT_Dos
                   POP DS                 
                   POP BP
                   RETF 4
DOS_findnext       ENDP
DOS_getcurdir      PROC FAR ; unsigned int far pascal DOS_getcurdir(unsigned int, void far *)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 4700h
                   MOV DX, WORD PTR [BP + 0Ah]             ; Length
                   LDS SI, DWORD PTR [BP + 06h]            ; Buffer
                   INT DEF_INT_Dos
                   POP DS
                   POP BP
                   RETF 2 + 4
DOS_getcurdir      ENDP
DOS_getcwd         PROC FAR ; void far * far pascal DOS_getcwd(void far *, unsigned int)
                   PUSH BP
                   MOV BP, SP
                   SUB SP, WORD PTR [BP + 06h]             ; Length
                   DEC SP
                   CALL FAR PTR CODE_DOS:DOS_getdisk
                   ADD AL, 'A'
                   LES DI, DWORD PTR [BP + 08h]            ; Buffer
                   CLD
                   STOSB
                   MOV AL, ':'
                   STOSB
                   MOV AL, '\'
                   STOSB
                   MOV AX, BP
                   SUB AX, WORD PTR [BP + 06h]             ; Length
                   DEC AX
                   PUSH AX
                   PUSH DEF_Null
                   PUSH SS
                   PUSH AX
                   CALL FAR PTR CODE_DOS:DOS_getcurdir
                   POP AX
                   PUSH DS
                   PUSH SS
                   POP DS
                   MOV SI, AX
                   MOV CX, WORD PTR [BP + 06h]             ; Length
                   SUB CX, 0004h
                   CLD
                   REPZ MOVSB
                   XOR AL, AL
                   STOSB
                   POP DS
                   MOV AX, WORD PTR [BP + 08h]             ; Low Buffer
                   MOV DX, WORD PTR [BP + 0Ah]             ; High Buffer
                   MOV SP, BP
                   POP BP
                   RETF 4 + 2 
DOS_getcwd         ENDP
DOS_getdisk        PROC FAR ; unsigned int far pascal DOS_getdisk(void)
                   MOV AX, 1900h
                   INT DEF_INT_Dos
                   XOR AH, AH
                   RETF  
DOS_getdisk        ENDP
DOS_mkdir          PROC FAR ; unsigned int far pascal DOS_mkdir(void far *)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 3900h
                   LDS DX, DWORD PTR [BP + 06h]            ; Directory name
                   INT DEF_INT_Dos
                   POP DS
                   POP BP
                   RETF 4
DOS_mkdir          ENDP
DOS_rmdir          PROC FAR ; unsigned int far pascal DOS_rmdir(void far *)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 3A00h
                   LDS DX, DWORD PTR [BP + 06h]            ; Directory name
                   INT DEF_INT_Dos
                   POP DS
                   POP BP
                   RETF 4
DOS_rmdir          ENDP
DOS_setdisk        PROC FAR ; unsigned int far pascal DOS_setdisk(unsigned int)
                   PUSH BP
                   MOV BP, SP
                   MOV AX, 0E00h
                   MOV DX, WORD PTR [BP + 06h]             ; Disk number
                   INT DEF_INT_Dos
                   XOR AH, AH
                   POP BP
                   RETF 2 
DOS_setdisk        ENDP
DOS_creat          PROC FAR ; unsigned int far pascal DOS_creat(void far *, unsigned int)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 3C00h
                   MOV CX, WORD PTR [BP + 06h]             ; Attrib
                   LDS DX, DWORD PTR [BP + 08h]            ; File name
                   INT DEF_INT_Dos
                   JNC SHORT L_DOS_creat_00
                   MOV AX, 0FFFFh
L_DOS_creat_00     LABEL SHORT
                   POP DS
                   POP BP
                   RETF 4 + 2
DOS_creat          ENDP
DOS_open           PROC FAR ; unsigned int far pascal DOS_open(void far *, unsigned int)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 3D00h
                   MOV CX, WORD PTR [BP + 06h]             ; Attrib
                   LDS DX, DWORD PTR [BP + 08h]            ; File name
                   INT DEF_INT_Dos
                   JNC SHORT L_DOS_open_00
                   MOV AX, 0FFFFh
L_DOS_open_00      LABEL SHORT
                   POP DS
                   POP BP
                   RETF 4 + 2
DOS_open           ENDP
DOS_close          PROC FAR ; unsigned int far pascal DOS_close(unsigned int)
                   PUSH BP
                   MOV BP, SP
                   MOV AX, 3E00h
                   MOV BX, WORD PTR [BP + 06h]             ; Handle
                   INT DEF_INT_Dos
                   POP BP
                   RETF 2
DOS_close          ENDP
DOS_unlink         PROC FAR ; unsigned int far pascal DOS_unlink(void far *)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 4100h
                   LDS DX, DWORD PTR [BP + 06h]            ; File name
                   INT DEF_INT_Dos
                   POP DS
                   POP BP
                   RETF 4
DOS_unlink         ENDP
DOS_read           PROC FAR ; unsigned int far pascal DOS_read(unsigned int, void far *, unsigned int)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 3F00h
                   MOV BX, WORD PTR [BP + 0Ch]             ; Handle
                   MOV CX, WORD PTR [BP + 06h]             ; Length
                   LDS DX, DWORD PTR [BP + 08h]            ; Buffer
                   INT DEF_INT_Dos
                   JNC SHORT L_DOS_read_00
                   MOV AX, 0FFFFh
L_DOS_read_00      LABEL SHORT                   
                   POP DS
                   POP BP
                   RETF 2 + 4 + 2
DOS_read           ENDP
DOS_write          PROC FAR ; unsigned int far pascal DOS_write(unsigned int, void far *, unsigned int)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 4000h
                   MOV BX, WORD PTR [BP + 0Ch]             ; Handle
                   MOV CX, WORD PTR [BP + 06h]             ; Length
                   LDS DX, DWORD PTR [BP + 08h]            ; Buffer
                   INT DEF_INT_Dos
                   JNC SHORT L_DOS_write_00
                   MOV AX, 0FFFFh
L_DOS_write_00     LABEL SHORT                   
                   POP DS
                   POP BP
                   RETF 2 + 4 + 2
DOS_write          ENDP
DOS_lseek          PROC FAR ; unsigned int far pascal DOS_lseek(unsigned int, unsigned long, unsigned int)
                   PUSH BP
                   MOV BP, SP
                   MOV AH, 42h
                   MOV AL, BYTE PTR [BP + 06h]             ; Mode
                   MOV BX, WORD PTR [BP + 0Ch]             ; Handle
                   MOV CX, WORD PTR [BP + 0Ah]             ; High Offset
                   MOV DX, WORD PTR [BP + 08h]             ; Low Offset
                   INT DEF_INT_Dos
                   POP BP
                   RETF 2 + 4 + 2
DOS_lseek          ENDP
DOS_getfileattr    PROC FAR ; unsigned int far pascal DOS_getfileattr(void far *, unsigned int far *)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 4300h
                   LDS DX, DWORD PTR [BP + 0Ah]            ; File name
                   INT DEF_INT_Dos
                   LDS BX, DWORD PTR [BP + 06h]            ; Attrib value pointer
                   MOV WORD PTR [BX], CX
                   POP DS
                   POP BP
                   RETF 4 + 4
DOS_getfileattr    ENDP
DOS_setfileattr    PROC FAR ; unsigned int far pascal DOS_setfileattr(void far *, unsigned int)
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   MOV AX, 4301h
                   MOV CX, WORD PTR [BP + 06h]             ; Attrib
                   LDS DX, DWORD PTR [BP + 08h]            ; File name
                   INT DEF_INT_Dos
                   POP DS
                   POP BP
                   RETF 4 + 2
DOS_setfileattr    ENDP
CODE_DOS           ENDS

                   ASSUME CS:CODE_DOS, DS:DATA_DOS, ES:NOTHING, SS:STACK_DEFAULT
DATA_DOS           SEGMENT
DATA_DOS           ENDS

                   ASSUME CS:CODE_DOS, DS:BSS_DOS, ES:NOTHING, SS:STACK_DEFAULT
BSS_DOS            SEGMENT
BSS_DOS            ENDS
                   END

; End of source
