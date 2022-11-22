COMMENT #

#

DEF_ASM_PORT       EQU "PORT.ASM"

INCLUDE            COMMON.INC

PUBLIC             InPort, InPortB, InPortW
PUBLIC             OutPort, OutPortB, OutPortW
PUBLIC             InPortx, InPortxB, InPortxW
PUBLIC             OutPortx, OutPortxB, OutPortxW
PUBLIC             InPorts, InPortsB, InPortsW
PUBLIC             OutPorts, OutPortsB, OutPortsW

                   ASSUME CS:CODE_PORT, DS:DATA_PORT, ES:NOTHING, SS:STACK_DEFAULT
CODE_PORT          SEGMENT
InPort             PROC FAR ; unsigned int far pascal InPort(unsigned int s_port)
InPortB            PROC FAR ; unsigned int far pascal InPort(unsigned int s_port)
                   PUSH BP
                   MOV BP, SP
                   PUSH DX
                   MOV DX, WORD PTR [BP + 06h]             ; s_port
                   IN AL, DX
                   XOR AH, AH
                   POP DX
                   POP BP
                   RETF 2
InPortB            ENDP
InPortW            PROC FAR ; unsigned int far pascal InPortW(unsigned int s_port)
                   PUSH BP
                   MOV BP, SP
                   PUSH DX
                   MOV DX, WORD PTR [BP + 06h]             ; s_port
                   IN AX, DX
                   POP DX
                   POP BP
                   RETF 2
InPortW            ENDP
InPort             ENDP
OutPort            PROC FAR ; void far pascal OutPort(unsigned int s_port, unsigned int s_byte)
OutPortB           PROC FAR ; void far pascal OutPortB(unsigned int s_port, unsigned int s_byte)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   MOV DX, WORD PTR [BP + 08h]             ; s_port
                   MOV AL, BYTE PTR [BP + 06h]             ; s_byte
                   OUT DX, AL
                   POP DX
                   POP AX
                   POP BP
                   RETF 2 + 2
OutPortB           ENDP
OutPortW           PROC FAR ; void far pascal OutPortW(unsigned int s_port, unsigned int s_word)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   MOV DX, WORD PTR [BP + 08h]             ; s_port
                   MOV AX, WORD PTR [BP + 06h]             ; s_word
                   OUT DX, AX
                   POP DX
                   POP AX
                   POP BP
                   RETF 2 + 2
OutPortW           ENDP
OutPort            ENDP
InPortx            PROC FAR ; unsigned int far pascal InPortx(unsigned int s_port, unsigned int s_index)
InPortxB           PROC FAR ; unsigned int far pascal InPortxB(unsigned int s_port, unsigned int s_index)
                   PUSH BP
                   MOV BP, SP
                   PUSH DX
                   MOV DX, WORD PTR [BP + 08h]             ; s_port
                   MOV AL, BYTE PTR [BP + 06h]             ; s_index
                   OUT DX, AL
                   INC DX
                   IN AL, DX
                   XOR AH, AH
                   POP DX
                   POP BP
                   RETF 2 + 2
InPortxB           ENDP
InPortxW           PROC FAR ; unsigned int far pascal InPortxW(unsigned int s_port, unsigned int s_index)
                   PUSH BP
                   MOV BP, SP
                   PUSH DX
                   MOV DX, WORD PTR [BP + 08h]             ; s_port
                   MOV AX, WORD PTR [BP + 06h]             ; s_index
                   OUT DX, AX
                   INC DX
                   INC DX
                   IN AX, DX
                   POP DX
                   POP BP
                   RETF 2 + 2
InPortxW           ENDP
InPortx            ENDP
OutPortx           PROC FAR ; void far pascal OutPortx(unsigned int s_port, unsigned int s_index, unsigned int s_byte)
OutPortxB          PROC FAR ; void far pascal OutPortxB(unsigned int s_port, unsigned int s_index, unsigned int s_byte)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   MOV DX, WORD PTR [BP + 0Ah]             ; s_port
                   MOV AL, BYTE PTR [BP + 08h]             ; s_index
                   OUT DX, AL
                   INC DX
                   MOV AL, BYTE PTR [BP + 06h]             ; s_byte
                   OUT DX, AL
                   POP DX
                   POP AX
                   POP BP
                   RETF 2 + 2 + 2
OutPortxB          ENDP
OutPortxW          PROC FAR ; void far pascal OutPortxW(unsigned int s_port, unsigned int s_index, unsigned int s_word)
                   PUSH BP
                   MOV BP, SP
                   PUSH AX
                   PUSH DX
                   MOV DX, WORD PTR [BP + 0Ah]             ; s_port
                   MOV AX, WORD PTR [BP + 08h]             ; s_index
                   OUT DX, AX
                   INC DX
                   INC DX
                   MOV AX, WORD PTR [BP + 06h]             ; s_word
                   OUT DX, AX
                   POP DX
                   POP AX
                   POP BP
                   RETF 2 + 2 + 2
OutPortxW          ENDP
OutPortx           ENDP
InPorts            PROC FAR ; void far pascal InPorts(void far *s_dest, unsigned int s_port, unsigned int s_length)
InPortsB           PROC FAR ; void far pascal InPortsB(void far *s_dest, unsigned int s_port, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH ES
                   PUSH DI
                   PUSH CX
                   PUSH DX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   MOV DX, WORD PTR [BP + 08h]             ; s_port
                   LES DI, DWORD PTR [BP + 0Ah]            ; s_dest
                   CLD
                   REPZ INSB
L_InPortsW_00      LABEL SHORT
                   POP DX
                   POP CX
                   POP DI
                   POP ES
                   POP BP
                   RETF 4 + 2 + 2
InPortsB           ENDP
InPortsW           PROC FAR ; void far pascal InPortsB(void far *s_dest, unsigned int s_port, unsigned int s_length)
                   PUSH BP
                   MOV BP, SP
                   PUSH ES
                   PUSH DI
                   PUSH CX
                   PUSH DX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   MOV DX, WORD PTR [BP + 08h]             ; s_port
                   LES DI, DWORD PTR [BP + 0Ah]            ; s_dest
                   CLD
                   REPZ INSW
                   MAC_JumpShort <L_InPortsW_00>
InPortsW           ENDP
InPorts            ENDP
OutPorts           PROC FAR ; void far pascal OutPorts(void far *s_src, unsigned int s_port, unsigned int s_length)
OutPortsB          PROC FAR ; void far pascal OutPortsB(void far *s_src, unsigned int s_port, unsigned int s_length) 
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH SI
                   PUSH CX
                   PUSH DX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   MOV DX, WORD PTR [BP + 08h]             ; s_port
                   LDS SI, DWORD PTR [BP + 0Ah]            ; s_dest
                   CLD
                   REPZ OUTSB
L_OutPortsW_00     LABEL SHORT
                   POP DX
                   POP CX
                   POP SI
                   POP DS
                   POP BP
                   RETF 4 + 2 + 2
OutPortsB          ENDP
OutPortsW          PROC FAR ; void far pascal OutPortsW(void far *s_src, unsigned int s_port, unsigned int s_length) 
                   PUSH BP
                   MOV BP, SP
                   PUSH DS
                   PUSH SI
                   PUSH CX
                   PUSH DX
                   MOV CX, WORD PTR [BP + 06h]             ; s_length
                   MOV DX, WORD PTR [BP + 08h]             ; s_port
                   LDS SI, DWORD PTR [BP + 0Ah]            ; s_dest
                   CLD
                   REPZ OUTSW
                   MAC_JumpShort <L_OutPortsW_00>
OutPortsW          ENDP
OutPorts           ENDP
CODE_PORT          ENDS

                   ASSUME CS:CODE_PORT, DS:DATA_PORT, ES:NOTHING, SS:STACK_DEFAULT
DATA_PORT          SEGMENT
DATA_PORT          ENDS

                   ASSUME CS:CODE_PORT, DS:BSS_PORT, ES:NOTHING, SS:STACK_DEFAULT
BSS_PORT           SEGMENT
BSS_PORT           ENDS
                   END

; End of source

