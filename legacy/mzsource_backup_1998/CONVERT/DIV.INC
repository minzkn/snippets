; DIV.INC

.286        

include mcseg.inc

_TEXT   segment public byte 'CODE'
        assume  cs:_TEXT
        public  LDIV@
        public  F_LDIV@
        public  N_LDIV@
        public  LUDIV@
        public  F_LUDIV@
        public  N_LUDIV@
        public  LMOD@
        public  F_LMOD@
        public  N_LMOD@
        public  LUMOD@
        public  F_LUMOD@
        public  N_LUMOD@
N_LDIV@:
        pop     cx          
        push    cs
        push    cx
LDIV@:
F_LDIV@:
        xor     cx,cx       
        jmp     short common
N_LUDIV@:
        pop     cx          
        push    cs
        push    cx
LUDIV@:
F_LUDIV@:
        mov     cx,1        
        jmp     short common
N_LMOD@:
        pop     cx       
        push    cs
        push    cx
LMOD@:
F_LMOD@:
        mov     cx,2     
        jmp     short   common
N_LUMOD@:
        pop     cx       
        push    cs
        push    cx
LUMOD@:
F_LUMOD@:
        mov     cx,3     
common:
        push    bp
        push    si
        push    di
        mov     bp,sp   
        mov     di,cx
        mov     ax,10[bp] 
        mov     dx,12[bp] 
        mov     bx,14[bp] 
        mov     cx,16[bp] 
        or      cx,cx
        jnz     slow@ldiv 
        or      dx,dx
        jz      quick@ldiv
        or      bx,bx
        jz      quick@ldiv   
slow@ldiv:
        test    di,1                   
        jnz     positive               
        or      dx,dx   
        jns     onepos
        neg     dx
        neg     ax
        sbb     dx,0    
        or      di,0Ch
onepos:
        or      cx,cx   
        jns     positive
        neg     cx
        neg     bx
        sbb     cx,0   
        xor     di,4
positive:
        mov     bp,cx
        mov     cx,32  
        push    di     
        xor     di,di     
        xor     si,si     
xloop:
        shl     ax,1      
        rcl     dx,1
        rcl     si,1
        rcl     di,1
        cmp     di,bp     
        jb      nosub
        ja      subtract
        cmp     si,bx     
        jb      nosub
subtract:
        sub     si,bx
        sbb     di,bp     
        inc     ax        
nosub:
        loop    xloop
        pop     bx   
        test    bx,2 
        jz      usequo
        mov     ax,si
        mov     dx,di
        shr     bx,1
usequo:
        test    bx,4
        jz      finish
        neg     dx
        neg     ax
        sbb     dx,0
finish:
        pop     di
        pop     si
        pop     bp
        retf    8
quick@ldiv:
        div     bx  
        test    di,2
        jz      quick@quo
        xchg    ax,dx
quick@quo:
        xor     dx,dx
        jmp     short finish
_TEXT   ends

; End of source 
