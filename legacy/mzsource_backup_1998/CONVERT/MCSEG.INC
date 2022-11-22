; /***************************************
;  *         Copyright(c)1998-1998       * 
;  *       MC Compiler source (MCSEG)    *
;  *         Code by Cho JaeHyuk         *
;  *          MINZ (02)822-2744          *
;  ***************************************/

; MCSEG.INC

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
_FONTDATA          segment byte public use16 'DATA'
_FONTDATA          ends
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
_STACK             segment byte stack use16 'STACK'
_STACK             ends
_HEAP              segment para private use16 'HEAP'
_HEAP              ends

                   ; /* Data group */
                   DGROUP GROUP _CONST, _FONTDATA, _DATA, _CVTSEG, _SCNSEG, _BSS, _BSSEND

; /* End of source */
