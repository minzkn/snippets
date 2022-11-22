; /***************************************
;  *         Copyright(c)1998-1998       * 
;  *        MC Compiler source (SOUN)    * 
;  *         Code by Cho JaeHyuk         * 
;  *          MINZ (02)822-2744          * 
;  ***************************************/

; SOUN.ASM

.286

include            mcseg.inc

extrn              strncpy: far, pascal

public             _dos_chdir
public             _dos_findfirst
public             _dos_findnext
public             _dos_getcurdir
public             _dos_getcwd
public             _dos_getdisk
public             _dos_mkdir
public             _dos_rmdir
public             _dos_setdisk
public             _dos_creat
public             _dos_open
public             _dos_close
public             _dos_unlink
public             _dos_read
public             _dos_write
public             _dos_lseek
public             _dos_getfileattr
public             _dos_setfileattr

                   assume cs:_TEXT
_TEXT              segment byte public use16 'CODE'
_dos_chdir         proc far ; unsigned int far pascal _dos_chdir(void far *)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, 3b00h
                   lds dx, dword ptr [bp + 6]
                   int 21h
                   pop ds
                   pop bp
                   retf 4
_dos_chdir         endp
_dos_findfirst     proc far ; unsigned int far pascal _dos_findfirst(void far *, void far *, unsigned int)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, 1a00h
                   lds dx, dword ptr [bp +8]
                   int 21h
                   mov ax, 4e00h
                   mov cx, word ptr [bp + 6]
                   lds dx, dword ptr [bp + 12]
                   int 21h
                   pop ds
                   pop bp
                   retf 4 + 4 + 2
_dos_findfirst     endp
_dos_findnext      proc far ; unsigned int far pascal _dos_findnext(void far *)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, 1a00h
                   lds dx, dword ptr [bp + 6]
                   int 21h
                   mov ax, 4f00h
                   int 21h
                   pop ds
                   pop bp
                   retf 4
_dos_findnext      endp
_dos_getcurdir     proc far ; unsigned int far pascal _dos_getcurdir(unsigned int, void far *)
                   push bp
                   mov bp, sp
                   push ds
                   push si
                   mov ax, 4700h
                   mov dx, word ptr [bp + 10]
                   lds si, dword ptr [bp + 6]
                   int 21h
                   pop si
                   pop ds
                   pop bp
                   retf 2 + 4
_dos_getcurdir     endp
_dos_getcwd        proc far ; void far * far pascal _dos_getcwd(void far *, unsigned int)
                   push bp
                   mov bp, sp
                   add sp, 256
                   push es
                   push di
                   call far ptr _TEXT:_dos_getdisk
                   add al, 'A'
                   les di, dword ptr [bp + 8]
                   cld
                   stosb
                   mov al, ':'
                   stosb
                   mov al, '\'
                   stosb
                   lea ax, [bp - 256]
                   push 0
                   push ss
                   push ax
                   call far ptr _TEXT:_dos_getcurdir
                   lea ax, [bp - 256]
                   push es
                   push di
                   push ss
                   push ax
                   mov ax, word ptr [bp + 6]
                   sub ax, 4
                   push ax
                   call far ptr _TEXT:strncpy
                   mov ax, word ptr [bp + 8]
                   mov dx, word ptr [bp + 10]
                   pop di
                   pop es
                   mov sp, bp
                   pop bp
                   retf 4 + 2
_dos_getcwd        endp
_dos_getdisk       proc far ; unsigned int far pascal _dos_getdisk(void)
                   mov ax, 1900h
                   int 21h
                   xor ah, ah
                   retf
_dos_getdisk       endp
_dos_mkdir         proc far ; unsigned int far pascal _dos_mkdir(void far *)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, 3900h
                   lds dx, dword ptr [bp + 6]
                   int 21h
                   pop ds
                   pop bp
                   retf 4
_dos_mkdir         endp
_dos_rmdir         proc far ; unsigned int far pascal _dos_rmdir(void far *)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, 3a00h
                   lds dx, dword ptr [bp + 6]
                   int 21h
                   pop ds
                   pop bp
                   retf 4
_dos_rmdir         endp
_dos_setdisk       proc far ; unsigned int far pascal setdisk(unsigned int)
                   push bp
                   mov bp, sp
                   mov ax, 0e00h
                   mov dx, word ptr [bp + 6]
                   int 21h
                   xor ah, ah
                   pop bp
                   retf 2
_dos_setdisk       endp
_dos_creat         proc far ; unsigned int far pascal _dos_creat(void far *, unsigned int)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, 3c00h
                   mov cx, word ptr [bp + 6]
                   lds dx, dword ptr [bp + 8]
                   int 21h
                   pop ds
                   pop bp
                   retf 4 + 2
_dos_creat         endp
_dos_open          proc far ; unsigned int far pascal _dos_open(void far *, unsigned int)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, 3d00h
                   mov cx, word ptr [bp + 6]
                   lds dx, dword ptr [bp + 8]
                   int 21h
                   jnb short no_open_error
                   xor ax, ax
no_open_error:
                   pop ds
                   pop bp
                   retf 4 + 2
_dos_open          endp
_dos_close         proc far ; unsigned int far pascal _dos_close(unsigned int)
                   push bp
                   mov bp, sp
                   mov ax, 3e00h
                   mov bx, word ptr [bp + 6]
                   int 21h
                   pop bp
                   retf 2
_dos_close         endp
_dos_unlink        proc far ; unsigned int far pascal _dos_unlink(void far *)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, 4100h
                   lds dx, dword ptr [bp + 6]
                   int 21h
                   pop ds
                   pop bp
                   retf 4
_dos_unlink        endp
_dos_read          proc far ; unsigned int far pascal _dos_read(unsigned int, void far *, unsigned int)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, 3f00h
                   mov bx, word ptr [bp + 12]
                   mov cx, word ptr [bp + 6]
                   lds dx, dword ptr [bp + 8]
                   int 21h
                   pop ds
                   pop bp
                   retf 2 + 4 + 2
_dos_read          endp
_dos_write         proc far ; unsigned int far pascal _dos_write(unsigned int, void far *, unsigned int)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, 4000h
                   mov bx, word ptr [bp + 12]
                   mov cx, word ptr [bp + 6]
                   lds dx, dword ptr [bp + 8]
                   int 21h
                   pop ds
                   pop bp
                   retf 2 + 4 + 2
_dos_write         endp
_dos_lseek         proc far ; unsigned int far pascal _dos_lseek(unsigned int, unsigned long, unsigned int)
                   push bp
                   mov bp, sp
                   mov ah, 42h
                   mov al, byte ptr [bp + 6]
                   mov bx, word ptr [bp + 12]
                   mov cx, word ptr [bp + 10]
                   mov dx, word ptr [bp + 8]
                   int 21h
                   pop bp
                   retf 2 + 4 + 2
_dos_lseek         endp
_dos_getfileattr   proc far ; unsigned int far pascal _dos_getfileattr(void far *, unsigned int far *)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, 4300h
                   lds dx, dword ptr [bp + 10]
                   int 21h
                   lds bx, dword ptr [bp + 6]
                   mov word ptr [bx], cx
                   pop ds
                   pop bp
                   retf 4 + 4
_dos_getfileattr   endp
_dos_setfileattr   proc far ; unsigned int far pascal _dos_setfileattr(void far *, unsigned int)
                   push bp
                   mov bp, sp
                   push ds
                   mov ax, 4301h
                   mov cx, word ptr [bp + 6]
                   lds dx, dword ptr [bp + 8]
                   int 21h
                   pop ds
                   pop bp
                   retf 4 + 2
_dos_setfileattr   endp
_TEXT              ends

                   assume ds:DGROUP
_CONST             segment byte public use16 'CONST'
_CONST             ends

                   assume ds:DGROUP
_DATA              segment byte public use16 'DATA'
_DATA              ends
                   end

; End of source 
