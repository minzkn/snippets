COMMENT #
 Name: Suga O/S
 File: SEGMENT.INC
 Copy: Copyright(c)1998-1999 by Cho JaeHyuk
 What: Defined segment location
#

APP_NAME           equ 'SUGA'

ORG_ENTRY          equ 0000h
NULL               equ 0000h

SIZE_BYTE          equ 1
SIZE_WORD          equ 2
SIZE_DWORD         equ 4
SIZE_FWORD         equ 6
SIZE_QWORD         equ 8
SIZE_TBYTE         equ 10
SIZE_PARA          equ 16
SIZE_PAGE          equ 512
SIZE_BUFFER        equ 512
SIZE_STACK         equ 0fff0h
SIZE_SECTOR        equ 512
SIZE_SEGMENT       equ 10000h

_TEXT              segment byte public use16 'CODE'
_TEXT              ends
_FARDATA           segment byte public use16 'FAR_DATA'
_FARDATA           ends
_OVERLAY           segment byte public use16 'OVRINFO'
_OVERLAY           ends
_1STUB             segment byte public use16 'STUBSEG'
_1STUB             ends
_INIT              segment byte public use16 'INITDATA'
_INIT              ends
_INITEND           segment byte public use16 'INITDATA'
_INITEND           ends
_EXIT              segment byte public use16 'EXITDATA'
_EXIT              ends
_EXITEND           segment byte public use16 'EXITDATA'
_EXITEND           ends
_CONST             segment byte public use16 'CONST'
_CONST             ends
_DATA              segment byte public use16 'DATA'
_DATA              ends
_FPSEG             segment byte public use16 'DATA'
_FPSEG             ends
_CVTSEG            segment byte public use16 'DATA'
_CVTSEG            ends
_SCNSEG            segment byte public use16 'DATA'
_SCNSEG            ends
_BSS               segment byte public use16 'BSS'
_BSS               ends
_BSSEND            segment byte public use16 'BSSEND'
_BSSEND            ends
_FARBSS            segment byte public use16 'FAR_BSS'
_FARBSS            ends
_STACK             segment para stack use16 'STACK'
_STACK             ends
_HEAP              segment para private use16 'HEAP'
_HEAP              ends

                   DGROUP GROUP _CONST, _DATA, _CVTSEG, _SCNSEG, _BSS, _BSSEND

; End of source
