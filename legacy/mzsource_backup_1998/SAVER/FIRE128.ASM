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
l_rand_0000 label word
           mov ax, 0013h
           int 10h
           xor ax, ax
           mov dx, 03c8h
           out dx, al
           inc dx
l_rgb_00:
           xor ah, ah
l_rgb_01:
           out dx, al
           inc ah
           cmp ah, 4 * 3
           jl short l_rgb_01
           inc al
           cmp al, 64
           jl short l_rgb_00

           push 0a000h
           pop ds

l_SAVER_fire_0000:
           mov cx, 32
@@:         
           mov bx, 320
           call near ptr _random
           add ax, (200 * 320)
           push ax
           mov bx, 256
           call near ptr _random
           pop bx
           mov byte ptr [bx], al
           loop short @B

           mov bx, 32000
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
           or ah, ah
           jnz short @F
           mov byte ptr [bx], al
@@:
           inc bx
           cmp bh, 0fah
           jne short l_SAVER_fire_0001

           in al, 60h
           dec al           
           jnz short l_SAVER_fire_0000
           mov ax, 0003h
           int 10h
           int 20h
_random    proc near 
           in al, 40h
           add ax, word ptr cs:l_rand_0000
           mov word ptr cs:l_rand_0000, ax
           xor dx, dx
           div bx
           mov ax, dx           
           retn
_random    endp
_CODE16    ends
           end _SAVER_fire

; End of source
