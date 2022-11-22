COMMENT #

#

; More than Intel 80x286 CPU on protected mode descriptor
STRUC_Descriptor   STRUC ; 8 Bytes
 ; More than 80x286
 STRUC_LimitLow    DW ? ; Limit (Bit 15 ~ 0)
 STRUC_BaseLow     DW ? ; Base  (Bit 15 ~ 0)
 STRUC_BaseMid     DB ? ; Base  (Bit 23 ~ 16)
 STRUC_Access00    DB ? ; Access (P,DPL,S,E,ED/C,R/W,A)
 ; More than 80x386
 STRUC_Access01    DB ? ; Access (G,D,O,AVL,Limit Bit 19~16)
 STRUC_BaseHigh    DB ? ; Base  (Bit 31 ~ 24)
STRUC_Descriptor   ENDS

; End of source
