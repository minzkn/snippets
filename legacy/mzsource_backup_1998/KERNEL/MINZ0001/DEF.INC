COMMENT #

#

DEF_NULL           = 0000h
DEF_NIL            = DEF_NULL
;
DEF_False          = 0000h
DEF_True           = 0001h
DEF_Off            = 0000h
DEF_On             = 0001h
DEF_Fail           = 0000h
DEF_Success        = 0001h
;
DEF_SIZE_Null      = 0000h
DEF_SIZE_Byte      = 0001h
DEF_SIZE_Word      = 0002h
DEF_SIZE_DWord     = 0004h
DEF_SIZE_FWord     = 0006h
DEF_SIZE_QWord     = 0008h
DEF_SIZE_TWord     = 000Ah
DEF_SIZE_Para      = 0010h
DEF_SIZE_Page      = 0100h
DEF_SIZE_Sector    = 0200h
DEF_SIZE_Far       = 0002h + 0002h
DEF_SIZE_Descriptor= DEF_SIZE_QWord
;
DEF_Word_Low       = DEF_SIZE_Null
DEF_Word_High      = DEF_SIZE_Byte
DEF_DWord_Low      = DEF_SIZE_Null
DEF_DWord_High     = DEF_SIZE_Word
DEF_FWord_Low      = DEF_SIZE_Null
DEF_FWord_High     = DEF_SIZE_DWord
DEF_QWord_Low      = DEF_SIZE_Null
DEF_QWord_High     = DEF_SIZE_DWord
DEF_Far_Offset     = DEF_DWord_Low
DEF_Far_Segment    = DEF_DWord_High
;
DEF_INT_BreakPoint = 03h
DEF_INT_Timer      = 08h
DEF_INT_Video      = 10h
DEF_INT_Disk       = 13h
DEF_INT_Keyboard   = 16h
DEF_INT_ReBoot     = 19h
DEF_INT_Dos        = 21h
DEF_INT_Mouse      = 33h
;
DEF_ASCII_EndOfString = 00h
DEF_ASCII_Beep     = 07h
DEF_ASCII_BackSpace= 08h
DEF_ASCII_Tab      = 09h
DEF_ASCII_Space    = 20h
DEF_ASCII_LineFeed = 0Ah
DEF_ASCII_CarrigeReturn = 0Dh
;
DEF_INFO_IntVectors = 256d
; Defined filed =============================================================
DEF_Company        EQU <"MINZ SOFTWARE">
DEF_Name           EQU <"MINZ">
DEF_Version_High   = 0001h
DEF_Version_Low    = 0000h
DEF_Version_Type   EQU <"Registered">
DEF_STRING_Max     = 0FFFFh
DEF_SIZE_STACK_DEFAULT = 0800h

; End of source
