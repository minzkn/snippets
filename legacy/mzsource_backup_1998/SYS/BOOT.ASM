; Code by Cho JaeHyuk
; Copyright(c)1997-1998 by MINZ

START_ADDRESS  equ 7c00h
ROOT_SIZE      equ 32
ROOT_CLUSTER   equ 1ah

KERNEL_SEG     equ 00a0h
KERNEL_OFF     equ 0000h
KERNEL_ADDRESS equ 00a00h

.286

           assume cs:MZBOOT, ds:MZBOOT, es:nothing, ss:nothing
MZBOOT     segment byte public use16 'MZBOOT'
           org 0

; --------------------- Start and boot_info ---------------
booting:   jmp near ptr start                             ; 7c00h  3 byte
           s_OEM                  db 'MINZ O/S'           ; 7c03h  8 byte
           s_spb                  dw 0                    ; 7c0bh  2 byte
           s_cps                  db 0                    ; 7c0dh  1 byte
           s_reserveds            dw 0                    ; 7c0eh  2 byte
           s_fats                 db 0                    ; 7c10h  1 byte
           s_roots                dw 0                    ; 7c11h  2 byte
           s_sectors              dw 0                    ; 7c13h  2 byte
           s_media                db 0                    ; 7c15h  1 byte
           s_fatps                dw 0                    ; 7c16h  2 byte
           s_tps                  dw 0                    ; 7c18h  2 byte
           s_heads                dw 0                    ; 7c1ah  2 byte
           s_hiddens              dw 0                    ; 7c1ch  2 byte
           s_adjust               dw 0                    ; 7c1eh  2 byte
            s_logicsl             dw 0                    ; 7c20h  2
            s_logicsh             dw 0                    ; 7c22h  2
           s_bootdrive            db 0                    ; 7c24h  1 byte
           s_head                 db 0                    ; 7c25h  1 byte
           s_signature            db 0                    ; 7c26h  1 byte
           s_serial               dd 0                    ; 7c27h  4 byte
           s_volume               db 11 dup (0)           ; 7c2bh  11 byte
           s_fat_system           db 8 dup (0)            ; 7c36h  8 byte
           c_rootl                dw 0
           c_rooth                dw 0
           c_bootl                dw 0
           c_booth                dw 0
           c_track                dw 0
           c_sector               db 0
           c_kernel               dd KERNEL_ADDRESS

; ---------------------- Initialize --------------------
start:     nop
           ; /* Copies boot */
           cli
           xor ax, ax
           mov ds, ax
           mov es, ax
           mov si, 7c00h
           mov di, 0500h
           mov cx, 256
           cld
           rep movsw
           db 0eah
           dw offset Relocate, 0050h

Relocate:  nop
           mov ax, cs
           mov ds, ax
           mov es, ax
           mov ss, ax
           mov sp, 1022
           sti

           ; /* Reset disk */
           xor ax, ax
           mov dl, byte ptr s_bootdrive
           int 13h
           jnb no_error
           jmp near ptr error
no_error:  nop

           ; /* Check long sector */
           mov ax, word ptr s_sectors
           or ax, ax
           jz ext_sector
           mov word ptr s_logicsl, ax
           xor ah, ah
ext_sector:nop

           ; /* Compute */
           mov al, byte ptr s_fats
           mul word ptr s_fatps
           add ax, word ptr s_hiddens
           adc dx, word ptr s_adjust
           add ax, word ptr s_reserveds
           adc dx, 0000h
           mov word ptr c_rootl, ax
           mov word ptr c_bootl, ax
           mov word ptr c_rooth, dx
           mov word ptr c_booth, dx
           mov ax, ROOT_SIZE
           mul word ptr s_roots
           mov bx, word ptr s_spb
           add ax, bx
           dec ax
           div bx
           add word ptr c_bootl, ax
           adc word ptr c_booth, 0000h

           ; /* Read root */           
           mov ax, word ptr c_rootl
           mov dx, word ptr c_rooth
           call compute
           jb error
           mov bx, 1024
           call read
           jb error

           ; /* Compare system name */
           mov cx, 512 / ROOT_SIZE
Comp_Loop: nop
           mov si, offset kernel_str
           mov di, bx
           mov cx, 8 + 3
           repe cmpsb
           je Comp_Success
           add bx, ROOT_SIZE
           loop short Comp_Loop
           jmp short error
Comp_Success: nop

           ; /* Found cluster */
           mov ax, word ptr [bx + ROOT_CLUSTER]
           dec ax
           dec ax
           mov bl, byte ptr s_cps
           xor bh, bh
           mul bx
           add ax, word ptr c_bootl
           adc dx, word ptr c_booth

           ; /* Read kernel */
           mov cx, 256 * 2 ; 256(128K) * 2 = 256Kbyte
read_kn:   push cx
           push ax
           push dx
           push ax ; -------------
           push dx
           mov ax, word ptr c_kernel
           mov dx, word ptr c_kernel + 2
           mov bx, 16
           div word ptr bx
           mov es, ax
           mov bx, word ptr c_kernel
           and bx, 000fh
           pop dx
           pop ax ; --------------
           call compute
           jb error_stak
           call read
           jb error_stak
           pop dx
           pop ax
           add ax, 1
           adc dx, 0
           mov bx, word ptr s_spb
           add word ptr c_kernel, bx
           adc word ptr c_kernel + 2, 0
           pop cx
           loop short read_kn
           db 0eah
           dw KERNEL_OFF, KERNEL_SEG          
error_stak:pop dx
           pop ax
           pop cx

; --------------------- Error --------------------------
error:     nop
           mov si, offset error_str
           call puts
           xor ax, ax           
           int 16h
           mov si, offset replace_str
           call puts
           int 19h

           replace_str  db 0dh, 0ah, 0feh, ' Replace the disk!', 0dh, 0ah, 0

; -------------------- Function -----------------------           

puts       proc near
           cld
           mov ah, 0eh
           mov bx, 7h
puts_00:   lodsb
           or al, al
           jz short return
           int 10h
           jmp short puts_00
compute    proc near
           cmp dx, word ptr s_tps
           jnb compute_00
           div word ptr s_tps
           inc dl
           mov byte ptr c_sector, dl
           xor dx, dx
           div word ptr s_heads
           mov byte ptr s_head, dl
           mov word ptr c_track, ax
           clc
           ret
compute_00:stc
return:    ret
compute    endp
puts       endp

           error_str    db 'Non-system or disk-error #KEY#', 0

read       proc near
           mov ax, 0201h
           mov cx, word ptr c_track
           shl ch, 06h
           or ch, byte ptr c_sector
           xchg ch, cl
           mov dl, byte ptr s_bootdrive
           mov dh, byte ptr s_head
           int 13h
           ret
read       endp

; -------------------- Data ----------------------------                     
           
           kernel_str   db 'MINZ    BIN'

           ; /* Partition initial */
           org 510
           db 055h, 0aah
MZBOOT     ends
_TSTACK    segment byte stack use16 'TSTACK'
_TSTACK    ends
           end booting

; End of source
