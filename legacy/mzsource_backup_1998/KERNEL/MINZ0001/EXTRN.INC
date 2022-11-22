COMMENT #

#

IFDEF              DEF_ASM_OPTION
ELSE
ENDIF
;
IFDEF              DEF_ASM_INFO 
ELSE
 EXTRN             Is_Intel80x86 : FAR                     ; Is Intel 80x86 CPU ?
 EXTRN             Is_Intel80x286 : FAR                    ; Is Intel 80x286 CPU ?
 EXTRN             Is_Intel80x386 : FAR                    ; Is Intel 80x386 CPU ?
 EXTRN             Is_Intel80x486 : FAR                    ; Is Intel 80x486 CPU ?
 EXTRN             Is_Intel80x586 : FAR                    ; Is Intel 80x586 CPU ?
 EXTRN             WhatCPU : FAR                           ; What CPU type?
 ;
 EXTRN             INFO_Entry : DWORD                      ; CODE_INIT's CS:IP entry point
 EXTRN             INFO_StartUpReg : BYTE                  ; Startup register value
 EXTRN             INFO_IntVector : DWORD                  ; Startup interrupt vectors
 EXTRN             INFO_ProgramSize : DWORD                ; Minimum memory above kernel
ENDIF
;
IFDEF              DEF_ASM_KERNEL
ELSE
 EXTRN             KernelMain : FAR                        ; Kernel main procedure
 ;
 EXTRN             S_STR_Company : BYTE
 EXTRN             S_STR_Name : BYTE
 EXTRN             S_STR_VersionHigh : WORD
 EXTRN             S_STR_VersionLow : WORD
 EXTRN             S_STR_VersionType : BYTE
ENDIF
;
IFDEF              DEF_ASM_INIT
 DEF_AcceptTime_Debugging = 0010h
ELSE
 EXTRN             HaltDos, ShutDown, HaltSystem : FAR     ; Halt family
 EXTRN             ResetVideo : FAR                        ; Reset video bios call
ENDIF
;
IFDEF              DEF_ASM_CALC
 DEF_RANDOM_Value  = 015A4E35h
ELSE
 EXTRN             Randomize, Rand, Random : FAR           ; Random value family
 EXTRN             ToPhysical, ToLogical, ToOptimalLogical : FAR ; Convert address type
 EXTRN             Center : FAR                            ; Center value
 EXTRN             DivL : FAR                              ; Division long
ENDIF
;
IFDEF              DEF_ASM_MEMORY
ELSE
 EXTRN             Peek, PeekB, PeekW : FAR                ; Peek family
 EXTRN             Poke, PokeB, PokeW : FAR                ; Poke family
 EXTRN             Swap, SwapB, SwapW : FAR                ; Swap family
 EXTRN             MemSet, MemSetB, MemSetW, MemClear : FAR; Memory set family
 EXTRN             MemCpy, MemCpyB, MemCpyW : FAR          ; Memory copy family
 EXTRN             MemScan, MemScanB, MemScanW : FAR       ; Memory scan family
 EXTRN             MemCmp, MemCmpB, MemCmpW : FAR          ; Memory compare family
 EXTRN             WhatBit : FAR                           ; What is bit
 EXTRN             OpenBit, CloseBit, ReadBit, WriteBitSet, WriteBitClear ; Bit adjust family
ENDIF
;
IFDEF              DEF_ASM_TEXT
 DEF_PrintF_Max    = 0100h
 DEF_PrintK_Max    = 0400h
ELSE
 EXTRN             Putc_00, Putc_01 : FAR                  ; Putc family
 EXTRN             Putc, SetPutc : FAR                     ; Call Putc family
 EXTRN             Puts_00 : FAR                           ; Puts family
 EXTRN             Putn_00 : FAR                           ; Putn family
 EXTRN             Puts, Putn, SetPuts, SetPutn : FAR      ; Call Puts & Putn family
 EXTRN             PutXY_00 : FAR                          ; PutXY family
 EXTRN             PutXY, SetPutXY : FAR                   ; Call PutXY family
 EXTRN             GetXY_00 : FAR                          ; GetXY family
 EXTRN             GetXY, SetGetXY : FAR                   ; Call GetXY family
 EXTRN             PutLineX, PutLineY : FAR                ; Put line family
 EXTRN             Box, Rectangle, Window, ClearScreen : FAR ; Box & Rectangle family
 EXTRN             SetWindow : FAR                         ; Set window area
 EXTRN             GetText, PutText : FAR                  ; Get & Put text family
 EXTRN             WhereX_00, WhereY_00 : FAR              ; Where cursor family
 EXTRN             WhereX, WhereY : FAR                    ; Call where cursor family
 EXTRN             SetWhereX, SetWhereY : FAR              ; Call where cursor family
 EXTRN             MoveCursor_00 : FAR                     ; Set cursor position family
 EXTRN             MoveCursor, CursorX, CursorY, CursorXY  ; Call set cursor position family
 EXTRN             SetMoveCursor : FAR                     ; Call set cursor position family
 EXTRN             Blink_00 : FAR                          ; Blank family
 EXTRN             Blink, SetBlink : FAR                   ; Call Blink family
 EXTRN             PutsXY, PutnXY, PutmXY : FAR            ; PutsXY & PutnXY & PutmXY family
 EXTRN             PrintF, _PrintF, PrintK, _PrintK : FAR  ; Print format - CDECL type call
 EXTRN             PutDump : FAR                           ; Put dump memory
 ;
 EXTRN             TEXT_VideoMemory : WORD
 EXTRN             TEXT_Page : WORD
 EXTRN             TEXT_Color : WORD
 EXTRN             TEXT_Character : WORD
 EXTRN             TEXT_Res : WORD
 EXTRN             TEXT_Window : WORD
 EXTRN             TEXT_TabLength : WORD                
 EXTRN             TEXT_LineTable : WORD
ENDIF
IFDEF              DEF_ASM_TIME
 DEF_DELAY_Value   = 1000d                                 ; 1/1000 second
ELSE
 EXTRN             ResetTimer : FAR                        ; Reset timer family
 EXTRN             Delay : FAR                             ; Delay family
 EXTRN             TimerDummy, OpenTimer, CloseTimer, SetTimerRate, SetTimerCallBack, TimerHandler, GetClockTick : FAR ; Timer family
 ;
 EXTRN             TIMER_IRQ_Enabled : WORD                ; Timer service enabled flag
 ;
 EXTRN             TIMER_OldVector : DWORD                 ; BIOS INT 08h routine
 EXTRN             TIMER_IRQ_CallBack : DWORD              ; User defined callback routine
 EXTRN             TIMER_IRQ_Active : WORD                 ; Callback routine active flag
 EXTRN             TIMER_Speed : WORD                      ; Timer speed in clock ticks
 EXTRN             TIMER_Count : WORD                      ; Timer counter used to call the
 EXTRN             TIMER_ClockTick : DWORD                 ; Clock tick counter
ENDIF
;
IFDEF              DEF_ASM_SPEAKER
ELSE
 EXTRN             Sound, NoSound, Beep : FAR              ; PC Speaker family
ENDIF
;
IFDEF              DEF_ASM_INT
ELSE
 EXTRN             NopInterrupt : FAR                      ; Nop interrupt service
 EXTRN             GetVect, SetVect : FAR                  ; Set interrupt vector family
ENDIF
;
IFDEF              DEF_ASM_STRING
 DEF_VSPrintF_Format_Character = '$'
ELSE
 EXTRN             StrLenC, StrLenN, StrLen : FAR          ; StrLen family
 EXTRN             StrCpyN, StrCpy : FAR                   ; StrCpy family
 EXTRN             StrCatN, StrCat : FAR                   ; StrCat family
 EXTRN             StrCmpN, StrCmp : FAR                   ; StrCmp family
 EXTRN             StrSetN, StrSet : FAR                   ; StrSet family
 EXTRN             StrUpr, StrLwr : FAR                    ; StrUpr/Lwr family
 EXTRN             StrInsert, StrInsertN, CharInsert, CharAddtion, CharAlign, GridInsert : FAR ; String insert family
 EXTRN             StrOptimize : FAR                       ; Optimize character string
 EXTRN             VSPrintF : FAR                          ; VSPrintF - Valiable argument format string
 EXTRN             SPrintF, _SPrintF : FAR                 ; SPrintF - CDECL type call
ENDIF
;
IFDEF              DEF_ASM_PORT
ELSE
 EXTRN             InPort, InPortB, InPortW : FAR          ; InPort family
 EXTRN             OutPort, OutPortB, OutPortW : FAR       ; OutPort family
 EXTRN             InPortx, InPortxB, InPortxW : FAR       ; InPortx family
 EXTRN             OutPortx, OutPortxB, OutPortxW : FAR    ; OutPortx family
 EXTRN             InPorts, InPortsB, InPortsW : FAR       ; InPorts family
 EXTRN             OutPorts, OutPortsB, OutPortsW : FAR    ; OutPorts family
ENDIF 
;
IFDEF              DEF_ASM_HEAP
 DEF_SIZE_HeapHeader = 0004h + 0002h                       ; Next(4) + Size(2)
 DEF_HEAP_Max      = 0A000h                                ; Limit allocation machanism
ELSE
 EXTRN             New_00, New_00_00 : FAR                 ; Optimize allocation
 EXTRN             New, FarAlloc, SetNew, SetFarAlloc : FAR; New(FarAlloc) family
 EXTRN             Delete_00 : FAR                         ; Delete(FarFree) family
 EXTRN             Delete, FarFree, SetDelete, SetFarFree : FAR ; Delete(FarFree) family
 ;
 EXTRN             L_ENTRY_HEAP_DEFAULT : BYTE             ; Heap entry 
ENDIF
;
IFDEF              DEF_ASM_IS
 DEF_IS_SP          = 01h                                  ; Is space 
 DEF_IS_DIG         = 02h                                  ; Is digit indicator 
 DEF_IS_UPP         = 04h                                  ; Is upper case 
 DEF_IS_LOW         = 08h                                  ; Is lower case 
 DEF_IS_HEX         = 10h                                  ; [0..9] or [A-F] or [a-f] 
 DEF_IS_CTL         = 20h                                  ; Control 
 DEF_IS_PUN         = 40h                                  ; Punctuation 
ELSE
 EXTRN             IsUpper, IsLower : FAR
 EXTRN             IsSpace : FAR
 EXTRN             IsDigit, IsHex : FAR
 EXTRN             IsControl : FAR
 EXTRN             IsNest : FAR
 ;
 EXTRN             T_IS : BYTE
ENDIF
;
IFDEF              DEF_ASM_TO
 DEF_SIZE_LtoA_Buffer = 0050h
ELSE
 EXTRN             ToUpper, ToLower, ToAsciiNumber, ToNum : FAR
 EXTRN             LtoA, ItoA, AtoL, AtoI : FAR
ENDIF
;
IFDEF              DEF_ASM_FONT
ELSE
 EXTRN             FONT_English : BYTE                     ; English font #00(Default)
ENDIF
;
IFDEF              DEF_ASM_COMPRESS
ELSE
 EXTRN             HUF_Encode, HUF_Decode : FAR            ; Huffman compress algorithm
 EXTRN             Encode, Decode, SetCode : FAR           ; Code family
ENDIF
;
IFDEF              DEF_ASM_DOS
ELSE
 EXTRN             DOS_chdir : FAR
 EXTRN             DOS_findfirst : FAR
 EXTRN             DOS_findnext : FAR
 EXTRN             DOS_getcurdir : FAR
 EXTRN             DOS_getcwd : FAR
 EXTRN             DOS_getdisk : FAR
 EXTRN             DOS_mkdir : FAR
 EXTRN             DOS_rmdir : FAR
 EXTRN             DOS_setdisk : FAR
 EXTRN             DOS_creat : FAR
 EXTRN             DOS_open : FAR
 EXTRN             DOS_close : FAR
 EXTRN             DOS_unlink : FAR
 EXTRN             DOS_read : FAR
 EXTRN             DOS_write : FAR
 EXTRN             DOS_lseek : FAR
 EXTRN             DOS_getfileattr : FAR
 EXTRN             DOS_setfileattr : FAR
ENDIF
;
IFDEF              DEF_ASM_IDE
ELSE
 EXTRN             IDE_Identify : FAR                      ; Identify
 EXTRN             IDE_Read, IDE_Write, IDE_Seek : FAR     ; Read/Write/Seek
 ;
 EXTRN             IDE_BasePort : WORD                     ; Base port table
ENDIF
;
IFDEF              DEF_ASM_NODE
ELSE
ENDIF

; End of source
