COMMENT #
 Name: MINZ Sound Blaster DSP v2.0
 Code: Code by Cho JaeHyuk
 Copy: Copyright(c)1999 by MINZ software
 What: Sound Blaster 100% complete
#

.386

SIZE_STACK         equ 1024
ADDRESS_DSP        equ 220h

_TEXT              segment para public use16 'CODE'
_TEXT              ends
_DATA              segment para public use16 'DATA'
_DATA              ends
_STACK             segment para stack use16 'STACK'
_STACK             ends
_HEAP              segment para public use16 'HEAP'
_HEAP              ends

Include            sample.inc

Public             puts
Public             dsp_write
Public             dsp_read
Public             dsp_init
Public             dsp_malloc
Public             dsp_time_constant
Public             dsp_halt_dma
Public             dsp_speaker_on
Public             dsp_speaker_off
Public             dsp_play
Public             dsp_record
Public             dsp_dma_count
Public             dsp_dma_status

Public             _data_ptr          
Public             _aligned_ptr       
Public             _aligned_physical  


                   assume cs:_TEXT, ds:_DATA, es:nothing, ss:_STACK 
_TEXT              segment
                   org 0
dsp_start:
                   cli
                   mov ax, _STACK
                   mov ss, ax
                   mov sp, SIZE_STACK
                   sti
                   mov ax, _DATA
                   mov ds, ax
                   xor ax, ax
                   mov es, ax

                   push offset _string00
                   call near ptr puts

; ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
                   call far ptr dsp_init
                   push 0e1h
                   call far ptr dsp_write
                   call far ptr dsp_read
                   mov word ptr _DATA:_major[0], ax
                   call far ptr dsp_read
                   mov word ptr _DATA:_minor[0], ax
                   cmp word ptr _DATA:_major[0], 3
                   jne short l_play_01
                   push si
                   mov si, offset _DATA:_major3[0]
                   mov cx, 9
                   mov dx, ADDRESS_DSP + 04h
                   cld
l_play_00:
                   lodsb
                   out dx, al
                   inc dx
                   lodsb
                   out dx, al
                   dec dx
                   loop short l_play_00
                   pop si
l_play_01:
                   call far ptr dsp_malloc
                   mov si, offset _DATA:_sample[0]
                   lodsd
                   cmp eax, 046464952h
                   je short l_RIFF_OK
                   push offset _DATA:_string02[0]
                   call near ptr puts
                   jmp near ptr l_exit
l_RIFF_OK:
                   lodsd
                   mov dword ptr _DATA:_rLen, eax
                   lodsd
                   mov dword ptr _DATA:_wID, eax
                   cmp eax, 045564157h
                   je short l_WAVE_OK
                   push offset _DATA:_string02[0]
                   call near ptr puts
                   jmp near ptr l_exit
l_WAVE_OK:
                   lodsd
                   mov dword ptr _DATA:_fID, eax
                   lodsd
                   mov dword ptr _DATA:_fLen, eax
                   mov dword ptr _DATA:_fNext, eax
                   mov bx, si
                   sub bx, offset _DATA:_sample[0]
                   add word ptr _DATA:_fNext[0], bx
                   adc word ptr _DATA:_fNext[2], 0
                   lodsw
                   mov word ptr _DATA:_wFormatTag, ax
                   lodsw
                   mov word ptr _DATA:_nChannels, ax
                   lodsw
                   mov word ptr _DATA:_nSamplesPerSec, ax
                   lodsw
                   mov word ptr _DATA:_nAvgBytesPerSec, ax
                   mov si, word ptr _DATA:_fNext[0]
                   add si, offset _DATA:_sample[0]
                   lodsd
                   mov dword ptr _DATA:_dID, eax
                   cmp eax, 61746164h
                   je short l_DATA_CHUNK_OK
                   push offset _DATA:_string02[0]
                   call near ptr puts
                   jmp near ptr l_exit
l_DATA_CHUNK_OK:
                   lodsd
                   mov dword ptr _DATA:_dLen, eax
                   xor edx, edx
                   mov eax, 1000000
                   div dword ptr _DATA:_nSamplesPerSec
                   mov edx, 256
                   sub edx, eax                   
                   push dx
                   call far ptr dsp_time_constant
                   call far ptr dsp_speaker_on
                   mov word ptr _DATA:_prev_entry, si
                   push offset _DATA:_string03[0]
                   call near ptr puts
l_play:            
                   cmp dword ptr _DATA:_dLen, 0
                   je short l_play_end
                   mov si, word ptr _DATA:_prev_entry
                   cmp dword ptr _DATA:_dLen, 65000
                   jg short l_play__00
                   mov eax, dword ptr _DATA:_dLen
                   jmp short l_play__01
l_play__00:
                   mov eax, 65000
l_play__01:
                   sub dword ptr _DATA:_dLen, eax
                   mov ecx, eax
                   les di, dword ptr _DATA:_aligned_ptr
                   rep movsb
                   push ax
                   call far ptr dsp_play
                   call far ptr dsp_dma_status
l_play__02:
                   call far ptr dsp_dma_status
                   or al, al
                   jnz short l_play__04
                   mov ax, 1                   
                   int 16h
                   or al, al
                   jz short l_play__03
                   call far ptr dsp_halt_dma
                   mov dword ptr _DATA:_dLen, 0
                   jmp short l_play__04
l_play__03:        
                   jmp short l_play__02                   
l_play__04:
                   jmp short l_play
l_play_end:
                   call far ptr dsp_speaker_off
; ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
l_exit:
                   push offset _string01
                   call near ptr puts
                   mov ax, 4c00h
                   int 21h
puts               proc near ; void near pascal puts(void *)
                   push bp
                   mov bp, sp
                   push ds
                   push si
                   mov ax, _DATA
                   mov ds, ax
                   mov si, word ptr [bp + 4]
                   mov ah, 0eh
                   mov bx, 7
                   cld
l_puts_00:
                   lodsb
                   or al, al
                   jz short l_puts_01
                   int 10h
                   jmp short l_puts_00
l_puts_01:
                   pop si
                   pop ds
                   pop bp
                   retn 2
puts               endp
dsp_write          proc far ; void far pascal dsp_write(unsigned int)
                   push bp
                   mov bp, sp
                   mov dx, ADDRESS_DSP + 0ch
                   mov cx, 0ffffh
l_dsp_write_00:
                   dec cx
                   jz short l_dsp_write_01
                   in al, dx
                   test al, 80h
                   jnz short l_dsp_write_00
                   mov al, byte ptr [bp + 6]
                   out dx, al
l_dsp_write_01:
                   pop bp
                   retf 2
dsp_write          endp
dsp_read           proc far ; unsigned int far pascal dsp_read(void)
                   mov dx, ADDRESS_DSP + 0eh
                   mov cx, 0ffffh
l_dsp_read_00:
                   dec cx
                   jz short l_dsp_read_01
                   in al, dx
                   test al, 80h
                   jz short l_dsp_read_00
                   mov dx, ADDRESS_DSP + 0ah
                   in al, dx
                   jmp short l_dsp_read_02
l_dsp_read_01:
                   xor al, al
l_dsp_read_02:
                   xor ah, ah 
                   retf
dsp_read           endp
dsp_init           proc far ; unsigned int far pascal dsp_init(void)
                   mov dx, ADDRESS_DSP + 0eh
                   in al, dx
                   mov dx, ADDRESS_DSP + 06h
                   mov al, 01h
                   out dx, al
                   in al, dx
                   in al, dx
                   in al, dx
                   in al, dx
                   xor al, al
                   out dx, al
                   mov cx, 100
l_dsp_init_00:
                   mov dx, ADDRESS_DSP + 0eh
                   in al, dx
                   test al, 80h
                   jz short l_dsp_init_01
                   mov dx, ADDRESS_DSP + 0ah
                   in al, dx
                   cmp al, 0aah
                   je short l_dsp_init_02
l_dsp_init_01:
                   loop short l_dsp_init_00
                   xor ax, ax
                   jmp short l_dsp_init_03
l_dsp_init_02:
                   mov ax, 1
l_dsp_init_03:
                   retf
dsp_init           endp
dsp_malloc         proc far ; void far pascal dsp_malloc(void)
                   mov ax, _HEAP
                   mov word ptr _DATA:_data_ptr[2], ax
                   mov word ptr _DATA:_data_ptr[0], 0
                   mov dx, 10h
                   mul dx
                   add ax, word ptr _DATA:_data_ptr[0]
                   adc dx, 0
                   add ax, 0ffffh
                   adc dx, 0
                   and dx, 000fh
                   xor ax, ax
                   and dx, 15
                   mov word ptr _DATA:_aligned_physical[0], dx
                   shl dx, 12
                   mov word ptr _DATA:_aligned_ptr[0], ax
                   mov word ptr _DATA:_aligned_ptr[2], dx
                   retf
dsp_malloc         endp
dsp_time_constant  proc far ; void far pascal dsp_time_constant(unsigned int)
                   push bp  ; [bp + 6] = 256LU-(1000000LU/samples per second)
                   mov bp, sp
                   mov dx, ADDRESS_DSP + 0eh
                   in al, dx
                   push 40h
                   call far ptr dsp_write
                   push word ptr [bp + 6]
                   call far ptr dsp_write
                   pop bp
                   retf 2
dsp_time_constant  endp
dsp_halt_dma       proc far ; void far pascal dsp_halt_dma(void)
                   push 0d0h
                   call far ptr dsp_write
                   retf
dsp_halt_dma       endp
dsp_speaker_on     proc far ; void far pascal dsp_speaker_on(void)
                   push 0d1h
                   call far ptr dsp_write
                   retf 
dsp_speaker_on     endp
dsp_speaker_off    proc far ; void far pascal dsp_speaker_off(void)
                   push 0d3h
                   call far ptr dsp_write
                   retf 
dsp_speaker_off    endp
dsp_play           proc far ; void far pascal dsp_play(unsigned int)
                   push bp
                   mov bp, sp
                   dec word ptr [bp + 8]
                   mov al, 05h
                   out 0ah, al
                   xor al, al
                   out 0ch, al
                   mov al, 49h
                   out 0bh, al
                   xor al, al
                   out 02h, al
                   out 02h, al
                   mov al, byte ptr _DATA:_aligned_physical[0]
                   out 83h, al
                   mov al, byte ptr [bp + 6]
                   out 03h, al
                   mov al, byte ptr [bp + 7]
                   out 03h, al
                   mov al, 01h
                   out 0ah, al
                   push 014h
                   call far ptr dsp_write
                   mov bx, word ptr [bp + 6]
                   xor ax, ax
                   mov al, bl
                   push ax
                   call far ptr dsp_write
                   mov al, bh
                   push ax
                   call far ptr dsp_write
                   pop bp
                   retf 2
dsp_play           endp
dsp_record         proc far ; void far pascal dsp_record(unsigned int)
                   push bp
                   mov bp, sp
                   dec word ptr [bp + 8]
                   mov al, 05h
                   out 0ah, al
                   xor al, al
                   out 0ch, al
                   mov al, 45h
                   out 0bh, al
                   xor al, al
                   out 02h, al
                   out 02h, al
                   mov al, byte ptr _DATA:_aligned_physical[0]
                   out 83h, al
                   mov al, byte ptr [bp + 6]
                   out 03h, al
                   mov al, byte ptr [bp + 7]
                   out 03h, al
                   mov al, 01h
                   out 0ah, al
                   push 024h
                   call far ptr dsp_write
                   mov bx, word ptr [bp + 6]
                   xor ax, ax
                   mov al, bl
                   push ax
                   call far ptr dsp_write
                   mov al, bh
                   push ax
                   call far ptr dsp_write
                   pop bp
                   retf 2
dsp_record         endp
dsp_dma_count      proc far ; unsigned int far pascal dsp_dma_count(void)
                   in al, 03h
                   mov bl, al
                   in al, 03h
                   mov bh, al
                   cmp bx, 0ffffh
                   jne short l_dma_count_00
                   mov dx, ADDRESS_DSP + 0eh
                   in al, dx
l_dma_count_00:
                   mov ax, bx
                   retf
dsp_dma_count      endp
dsp_dma_status     proc far ; unsigned int far pascal dsp_dma_status(void)
                   in al, 08h
                   and al, 2
                   xor ah, ah
                   retf
dsp_dma_status     endp
_TEXT              ends

                   assume ds:_DATA
_DATA              segment
_string00          db 'Sound Blaster Player R0.0.0 Copyright(c)1999 by MINZ software', 0dh, 0ah, 0
_string01          db 0dh, 0ah, 'End of sound effect.', 0dh, 0ah, 0
_string02          db 'Changed data! Sorry!', 0dh, 0ah, 0
_string03          db 'Press any key to QUIT!', 0
_major3            db 000h, 0ffh, 004h, 0ffh, 00ah, 000h, 000ch, 026h, 00eh, 020h, 022h, 099h, 026h, 000h, 028h, 000h, 02eh, 000h
_data_ptr          dd ?
_aligned_ptr       dd ?
_aligned_physical  dw ?
_major             dw ?
_minor             dw ?
_prev_entry        dw ?
_rID               dd ?
_rLen              dd ?
_wID               dd ?
_fID               dd ?
_fLen              dd ?
_fNext             dd ?
_wFormatTag        dw ?
_nChannels         dw ?
_nSamplesPerSec    dw ?
_nAvgBytesPerSec   dw ?
_dID               dd ?
_dLen              dd ?
_DATA              ends

_STACK             segment
                   db SIZE_STACK dup (?)
_STACK             ends

_HEAP              segment
_HEAP              ends
                   end dsp_start
           
; End of source
