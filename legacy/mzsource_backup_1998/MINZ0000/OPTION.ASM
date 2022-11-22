COMMENT #

#

DEF_ASM_OPTION     EQU "OPTION.ASM"

INCLUDE            COMMON.INC

                   ASSUME CS:DATA_OPTION, DS:DATA_INFO, ES:NOTHING, SS:STACK_DEFAULT
DATA_OPTION        SEGMENT
DATA_OPTION        ENDS
                   END

; End of source
