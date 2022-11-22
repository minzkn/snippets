COMMENT #
 
#

DEF_ASM_NODE       EQU "NODE.ASM"

INCLUDE            COMMON.INC

                   ASSUME CS:CODE_NODE, DS:DATA_NODE, ES:NOTHING, SS:STACK_DEFAULT
CODE_NODE          SEGMENT
; Head -> data0 -> data1 -> data2 -> end -> null

CODE_NODE          ENDS

                   ASSUME CS:CODE_NODE, DS:DATA_NODE, ES:NOTHING, SS:STACK_DEFAULT
DATA_NODE          SEGMENT
DATA_NODE          ENDS

                   ASSUME CS:CODE_NODE, DS:BSS_NODE, ES:NOTHING, SS:STACK_DEFAULT
BSS_NODE           SEGMENT
BSS_NODE           ENDS
                   END

; End of source
