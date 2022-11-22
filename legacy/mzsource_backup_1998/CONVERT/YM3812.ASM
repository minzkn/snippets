; /***************************************
;  *         Copyright(c)1998-1998       * 
;  *        MC Compiler source (SOUN)    * 
;  *         Code by Cho JaeHyuk         * 
;  *          MINZ (02)822-2744          * 
;  ***************************************/

; SOUN.ASM

.286

include            mcseg.inc

public             YM_register
public             YM_enablewave
public             YM_disablewave
public             YM_method1
public             YM_method2
public             YM_method3
public             YM_method4
public             YM_method5
public             YM_method6
public             YM_selectwave
public             YM_frequency_on
public             YM_frequency_off
public             YM_iscard

                   assume cs:_TEXT
_TEXT              segment byte public use16 'CODE'
YM_register        proc far ; void far pascal YM_register(unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   mov dx, 0388h
                   mov al, byte ptr [bp + 8]
                   out dx, al
                   mov cx, 0006h
YM_reg00@:         in al, dx
                   loop short YM_reg00@
                   inc dx
                   mov al, [bp + 6]
                   out dx, al
                   dec dx
                   mov cx, 35
YM_reg01@:         in al, dx
                   loop short YM_reg01@
                   pop bp
                   retf 2 + 2
YM_register        endp
YM_enablewave      proc far ; void far pascal YM_enablewave(void)
                   push 0001h
                   push 0020h
                   call far ptr _TEXT:YM_register
                   retf
YM_enablewave      endp
YM_disablewave     proc far ; void far pascal YM_disablewave(void)
                   push 0001h
                   push 0000h
                   call far ptr _TEXT:YM_register
                   retf
YM_disablewave     endp
YM_method1         proc far ; void far pascal YM_method1(word, word, word, word, word, word)
                   push bp
                   mov bp, sp
                   mov ax, 0020h
                   add ax, word ptr [bp + 16]
                   push ax
                   mov dx, word ptr [bp + 14]
                   shl dx, 7
                   mov ax, word ptr [bp + 12]
                   shl ax, 6
                   or dx, ax
                   mov ax, word ptr [bp + 10]
                   shl ax, 5
                   or dx, ax
                   mov ax, word ptr [bp + 8]
                   shl ax, 4
                   or ax, dx
                   or ax, word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2 + 2 + 2 + 2
YM_method1         endp
YM_method2         proc far ; void far pascal YM_method2(word, word, word)
                   push bp
                   mov bp, sp
                   mov ax, 0040h
                   add ax, word ptr [bp + 10]
                   push ax
                   mov ax, word ptr [bp + 8]
                   shl ax, 6
                   or ax, word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_method2         endp
YM_method3         proc far ; void far pascal YM_method3(word, word, word)
                   push bp
                   mov bp, sp
                   mov ax, 0060h
                   add ax, word ptr [bp + 10]
                   push ax
                   mov ax, word ptr [bp + 8]
                   shl ax, 4
                   or ax, word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_method3         endp
YM_method4         proc far ; void far pascal YM_method4(word, word, word)
                   push bp
                   mov bp, sp
                   mov ax, 0080h
                   add ax, word ptr [bp + 10]
                   push ax
                   mov ax, word ptr [bp + 8]
                   shl ax, 4
                   or ax, word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_method4         endp
YM_method5         proc far ; void far pascal YM_method5(word, word, word)
                   push bp
                   mov bp, sp
                   push 00bdh
                   mov dx, word ptr [bp + 10]
                   shl dx, 7
                   mov ax, word ptr [bp + 8]
                   shl ax, 6
                   or dx, ax
                   mov ax, word ptr [bp + 6]
                   shl ax, 5
                   or dx, ax
                   mov ax, word ptr [bp + 6]
                   and ax, 001fh
                   or ax, dx
                   push ax                   
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_method5         endp
YM_method6         proc far ; void far pascal YM_method6(word, word, word)
                   push bp
                   mov bp, sp
                   mov ax, 00c0h
                   add ax, word ptr [bp + 10]
                   push ax
                   mov ax, word ptr [bp + 8]
                   shl ax, 1
                   or ax, word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_method6         endp
YM_selectwave      proc far ; void far pascal YM_selectwave(unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   mov ax, 00e0h
                   add ax, word ptr [bp + 8]
                   push ax
                   push word ptr [bp + 6]
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2
YM_selectwave      endp
YM_frequency_on    proc far ; void far pascal YM_frquency_on(unsigned int, unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   mov ax, 00a0h
                   add ax, word ptr [bp + 10]
                   push ax
                   push word ptr [bp + 8]
                   call far ptr _TEXT:YM_register
                   mov ax, 00b0h
                   add ax, word ptr [bp + 10]
                   push ax
                   mov ax, word ptr [bp + 8]
                   shr ax, 8
                   or ax, word ptr [bp + 6]
                   or ax, 0032h
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_frequency_on    endp
YM_frequency_off   proc far ; void far pascal YM_frquency_off(unsigned int, unsigned int, unsigned int)
                   push bp
                   mov bp, sp
                   mov ax, 00a0h
                   add ax, word ptr [bp + 10]
                   push ax
                   push word ptr [bp + 8]
                   call far ptr _TEXT:YM_register
                   mov ax, 00b0h
                   add ax, word ptr [bp + 10]
                   push ax
                   mov ax, word ptr [bp + 8]
                   shr ax, 8
                   or ax, word ptr [bp + 6]
                   push ax
                   call far ptr _TEXT:YM_register
                   pop bp
                   retf 2 + 2 + 2
YM_frequency_off   endp
YM_iscard          proc far ; unsigned int far pascal YM_iscard(void)
                   push 0004h
                   push 0060h
                   call far ptr _TEXT:YM_register
                   push 0004h
                   push 0080h
                   call far ptr _TEXT:YM_register
                   mov dx, 0388h
                   in al, dx
                   push ax ; ---------- _time[0]
                   push 0002h
                   push 00ffh
                   call far ptr _TEXT:YM_register
                   push 0004h
                   push 0021h
                   call far ptr _TEXT:YM_register
                   mov dx, 0388h
                   mov cx, 201
YM_iscard00@:      in al, dx
                   loop short YM_iscard00@
                   push ax ; ---------- _time[1]
                   push 0004h
                   push 0060h
                   call far ptr _TEXT:YM_register
                   push 0004h
                   push 0080h
                   call far ptr _TEXT:YM_register
                   pop dx ; ----------- _time[1]
                   pop ax ; ----------- _time[0]
                   and dx, 00e0h
                   and ax, 00e0h
                   or ax, ax
                   jnz YM_iscard01@
                   cmp dx, 00c0h
                   jne YM_iscard01@
                   mov ax, 0001h
                   retf
YM_iscard01@:      xor ax, ax
                   retf
YM_iscard          endp
_TEXT              ends

                   assume ds:DGROUP
_CONST             segment byte public use16 'CONST'
YM_instrument      db  0,  0,  0,  0,  1,  1, 15, 15,  1,  5,  3,  0 ; unsigned char YM_instrument[6][12]
                   db  0,  0,  1,  0,  0,  0, 18, 13,  1, 14,  3,  0
                   db  0,  0,  1,  1,  1,  0, 15, 11,  7,  7,  0,  0
                   db  0,  0,  0,  1,  1,  1, 20,  3,  2,  2,  2,  0
                   db  0,  0,  1,  1,  2,  1, 22,  3,  2,  2,  2,  0
                   db  0,  0,  1,  1,  1,  0, 28, 15,  2,  2,  2,  0
YM_frequency       dw 345, 365, 387, 410, 435, 460, 488, 517, 547, 580, 615, 651 ; unsigned int YM_frequency[12]
_CONST             ends

                   assume ds:DGROUP
_DATA              segment byte public use16 'DATA'
_DATA              ends
                   end

; End of source 
