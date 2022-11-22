COMMENT #
 Name: MINZ O/S
 File: MOUSE.ASM
 Copy: Copyright(c)1996-1999 by Cho JaeHyuk
 What: MINZ operating system kernel source
#

.286

FILE_MOUSE_ASM equ 'MOUSE.ASM'

Include ..\SEGMENT.INC

Public             open_mouse
Public             close_mouse
Public             process_mouse

                   assume cs:_MOUSE_TEXT, ds:DGROUP, es:nothing, ss:_STACK
_MOUSE_TEXT        segment
open_mouse         proc far ; void far pascal open_mouse(void)
                   retf
open_mouse         endp
close_mouse        proc far ; void far pascal close_mouse(void)
                   retf
close_mouse        endp
process_mouse      proc far ; void far pascal process_mouse(void)
                   retf
process_mouse      endp
_MOUSE_TEXT        ends
                   end

; End of source
