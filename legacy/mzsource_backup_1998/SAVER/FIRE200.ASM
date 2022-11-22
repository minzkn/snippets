COMMENT #
 Name : Percieve kernel
 File : "FIRE.ASM"
 Code : Cho JaeHyuk
 Copy : Copyright (c) 1998 by MINZ
#

.286

           assume cs:_CODE16, ds:_CODE16, es:_CODE16
_CODE16    segment
           org 100h
_SAVER_fire:
           mov ax, 0013h
           int 10h
           xor al, al
           mov dx, 03c8h
           out dx, al
           inc dx
           xor cl, cl
@@:
           mov al, cl
           out dx, al
           xor al, al
           out dx, al
           out dx, al
           inc cl
           cmp cl, 64
           jne short @B
           xor cl, cl
@@:
           mov al, 63
           out dx, al
           mov al, cl
           out dx, al
           xor al, al
           out dx, al
           inc cl
           cmp cl, 64
           jne short @B
           xor cl, cl
@@:
           mov al, 63
           out dx, al
           out dx, al
           mov al, cl
           out dx, al
           inc cl
           cmp cl, 64
           jne short @B
           xor cl, cl
           mov al, 63
@@:
           out dx, al
           inc cl
           cmp cl, 64
           jne short @B

l_SAVER_fire_0000:
           push 0a000h
           pop ds
           mov cx, 32
@@:         
           push 320
           call near ptr _random
           add ax, (200 * 320)
           push ax
           push 240
           call near ptr _random
           pop bx
           mov byte ptr [bx], al
           loop short @B

           mov bx, 48000
           xor cx, cx
l_SAVER_fire_0001:
           xor ax, ax           
           mov al, byte ptr [bx]
           mov cl, byte ptr [bx + 319]
           add ax, cx
           mov cl, byte ptr [bx + 320]
           add ax, cx
           mov cl, byte ptr [bx + 321]
           add ax, cx
           shr ax, 2           
           dec ax
           dec ax
           or ah, ah
           jnz short @F
           mov byte ptr [bx], al
           push bx
           mov cx, 64000 - 1
           sub cx, bx
           mov bx, cx
           xor cx, cx
           mov byte ptr [bx], al
           pop bx
@@:
           inc bx
           cmp bx, 0fa00h
           jne short l_SAVER_fire_0001

           in al, 60h
           dec al
           jnz short l_SAVER_fire_0000
           mov ax, 0003h
           int 10h
           int 20h
_random    proc near 
           push bp
           mov bp, sp
           in al, 40h
           add ax, word ptr cs:l_rand_0000
           mov word ptr cs:l_rand_0000, ax
           xor dx, dx
           div word ptr [bp + 4]
           mov ax, dx
           pop bp
           retn 2
l_rand_0000 dw 0a5a5h
_random    endp
_CODE16    ends
           end _SAVER_fire

; End of source
