COMMENT #

#

MAC_DefineSegment  MACRO m_name, m_align, m_combine, m_use, m_class
                   &m_name SEGMENT &m_align &m_combine &m_use '&m_class'
                   &m_name ENDS
                   ENDM
;
MAC_JumpShort      MACRO m_label
                   LOCAL L_MAC_JumpShort_00
                   DB 0EBh
                   DB (OFFSET &m_label) - (OFFSET L_MAC_JumpShort_00)
L_MAC_JumpShort_00 LABEL SHORT
                   ENDM
;
MAC_JumpNear       MACRO m_label
                   LOCAL L_MAC_JumpNear_00
                   DB 0E9h
                   DB (OFFSET &m_label) - (OFFSET L_MAC_JumpNear_00)
L_MAC_JumpNear_00  LABEL SHORT
                   ENDM
;
MAC_JumpFar        MACRO m_segment, m_offset, m_label:=<>
                   DB 0EAh
&m_label           DW &m_offset, &m_segment
                   ENDM
;
MAC_CallNear       MACRO m_label
                   LOCAL L_MAC_CallNear_00
                   DB 0E8h
                   DB (OFFSET &m_label) - (OFFSET L_MAC_CallNear_00)
L_MAC_CallNear_00  LABEL SHORT                  
                   ENDM
;
MAC_CallFar        MACRO m_segment, m_offset, m_label:=<>
                   DB 09Ah
&m_label           DW &m_offset, &m_segment
                   ENDM
;
MAC_ClearCache     MACRO
                   DB 0EBh
                   DB DEF_NULL
                   ENDM
;
MAC_Halt           MACRO
                   HLT
                   ENDM
;
MAC_ShutDown       MACRO
                   MAC_JumpShort <$>
                   ENDM
;
MAC_RETF_Imm       MACRO m_num, m_label:=<>
                   DB 0CAh
&m_label           DW &m_num
                   ENDM
;
MAC_Beep           MACRO
                   PUSH 1000h
                   PUSH 100d
                   CALL FAR PTR CODE_SPEAKER:Beep
                   ENDM

; End of source
