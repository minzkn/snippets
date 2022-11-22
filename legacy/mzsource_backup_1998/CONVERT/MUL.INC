; MUL.INC

.286

include mcseg.inc

_TEXT   SEGMENT BYTE PUBLIC 'CODE'
        ASSUME  CS:_TEXT
        public  LXMUL@
        public  F_LXMUL@
LXMUL@          PROC    FAR
F_LXMUL@:
                push    si
                xchg    si,ax   
                xchg    ax,dx
                test    ax,ax
                jz      nohi1
                mul     bx
nohi1:        
                jcxz    nohi2     
                xchg    cx, ax    
                mul     si        
                add     ax, cx    
nohi2:
                xchg    ax,si
                mul     bx        
                add     dx,si     
                pop     si
                ret
LXMUL@          ENDP
_TEXT   ENDS

; End of source
