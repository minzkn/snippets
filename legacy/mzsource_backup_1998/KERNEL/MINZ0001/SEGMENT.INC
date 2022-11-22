COMMENT #

#

MAC_DefineSegment  <CODE_ALIGN>, <PARA>, <PUBLIC>, <USE16>, <CLASS_CODE_ALIGN>
MAC_DefineSegment  <DATA_OPTION>, <PARA>, <PUBLIC>, <USE16>, <CLASS_DATA_OPTION>
MAC_DefineSegment  <CODE_INIT>, <PARA>, <PUBLIC>, <USE16>, <CLASS_CODE_INIT>
MAC_DefineSegment  <CODE_MBS>, <PARA>, <PUBLIC>, <USE16>, <CLASS_CODE_MBS>
MAC_DefineSegment  <CODE_INFO>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_INFO>
MAC_DefineSegment  <CODE_CALC>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_CALC>
MAC_DefineSegment  <CODE_MEMORY>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_MEMORY>
MAC_DefineSegment  <CODE_TEXT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_TEXT>
MAC_DefineSegment  <CODE_TIME>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_TIME>
MAC_DefineSegment  <CODE_SPEAKER>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_SPEAKER>
MAC_DefineSegment  <CODE_INT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_INT>
MAC_DefineSegment  <CODE_STRING>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_STRING>
MAC_DefineSegment  <CODE_PORT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_PORT>
MAC_DefineSegment  <CODE_HEAP>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_HEAP>
MAC_DefineSegment  <CODE_IS>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_IS>
MAC_DefineSegment  <CODE_TO>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_TO>
MAC_DefineSegment  <CODE_FONT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_FONT>
MAC_DefineSegment  <CODE_COMPRESS>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_COMPRESS>
MAC_DefineSegment  <CODE_DOS>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_DOS>
MAC_DefineSegment  <CODE_IDE>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_IDE>
MAC_DefineSegment  <CODE_NODE>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_NODE>
;
MAC_DefineSegment  <DATA_ALIGN>, <PARA>, <PUBLIC>, <USE16>, <CLASS_DATA_ALIGN>
MAC_DefineSegment  <DATA_INIT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_INIT>
MAC_DefineSegment  <DATA_MBS>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_MBS>
MAC_DefineSegment  <DATA_INFO>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_INFO>
MAC_DefineSegment  <DATA_CALC>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_CALC>
MAC_DefineSegment  <DATA_MEMORY>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_MEMORY>
MAC_DefineSegment  <DATA_TEXT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_TEXT>
MAC_DefineSegment  <DATA_TIME>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_TIME>
MAC_DefineSegment  <DATA_SPEAKER>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_SPEAKER>
MAC_DefineSegment  <DATA_INT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_INT>
MAC_DefineSegment  <DATA_STRING>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_STRING>
MAC_DefineSegment  <DATA_PORT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_PORT>
MAC_DefineSegment  <DATA_HEAP>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_HEAP>
MAC_DefineSegment  <DATA_IS>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_IS>
MAC_DefineSegment  <DATA_TO>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_TO>
MAC_DefineSegment  <DATA_FONT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_FONT>
MAC_DefineSegment  <DATA_COMPRESS>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_COMPRESS>
MAC_DefineSegment  <DATA_DOS>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_DOS>
MAC_DefineSegment  <DATA_IDE>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_IDE>
MAC_DefineSegment  <DATA_NODE>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_NODE>
;
MAC_DefineSegment  <CODE_KERNEL>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_CODE_KERNEL>
MAC_DefineSegment  <_TEXT>, <BYTE>, <PUBLIC>, <USE16>, <CODE> ; Complete Borland C/C++
MAC_DefineSegment  <DATA_KERNEL>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_DATA_KERNEL>
MAC_DefineSegment  <_FARDATA>, <BYTE>, <PUBLIC>, <USE16>, <FAR_DATA> ; Complete Borland C/C++
MAC_DefineSegment  <_OVERLAY>, <BYTE>, <PUBLIC>, <USE16>, <OVRINFO> ; Complete Borland C/C++ 
MAC_DefineSegment  <_1STUB>, <BYTE>, <PUBLIC>, <USE16>, <STUBSEG> ; Complete Borland C/C++ 
MAC_DefineSegment  <_INIT>, <BYTE>, <PUBLIC>, <USE16>, <INITDATA> ; Complete Borland C/C++ 
MAC_DefineSegment  <_INITEND>, <BYTE>, <PUBLIC>, <USE16>, <INITDATA> ; Complete Borland C/C++ 
MAC_DefineSegment  <_EXIT>, <BYTE>, <PUBLIC>, <USE16>, <EXITDATA> ; Complete Borland C/C++ 
MAC_DefineSegment  <_EXITEND>, <BYTE>, <PUBLIC>, <USE16>, <EXITDATA> ; Complete Borland C/C++ 
MAC_DefineSegment  <_CONST>, <BYTE>, <PUBLIC>, <USE16>, <CONST> ; Complete Borland C/C++ 
MAC_DefineSegment  <_DATA>, <BYTE>, <PUBLIC>, <USE16>, <DATA> ; Complete Borland C/C++ 
MAC_DefineSegment  <_FPSEG>, <BYTE>, <PUBLIC>, <USE16>, <DATA> ; Complete Borland C/C++ 
MAC_DefineSegment  <_CVTSEG>, <BYTE>, <PUBLIC>, <USE16>, <DATA> ; Complete Borland C/C++ 
MAC_DefineSegment  <_SCNSEG>, <BYTE>, <PUBLIC>, <USE16>, <DATA> ; Complete Borland C/C++
MAC_DefineSegment  <_BSS>, <BYTE>, <PUBLIC>, <USE16>, <BSS> ; Complete Borland C/C++ 
MAC_DefineSegment  <_BSSEND>, <BYTE>, <PUBLIC>, <USE16>, <BSSEND> ; Complete Borland C/C++ 
MAC_DefineSegment  <_FARBSS>, <BYTE>, <PUBLIC>, <USE16>, <FAR_BSS> ; Complete Borland C/C++
;
MAC_DefineSegment  <BSS_ALIGN>, <PARA>, <PUBLIC>, <USE16>, <CLASS_BSS_ALIGN>
MAC_DefineSegment  <BSS_KERNEL>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_KERNEL>
MAC_DefineSegment  <BSS_INIT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_INIT>
MAC_DefineSegment  <BSS_MBS>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_MBS>
MAC_DefineSegment  <BSS_INFO>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_INFO>
MAC_DefineSegment  <BSS_CALC>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_CALC>
MAC_DefineSegment  <BSS_MEMORY>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_MEMORY>
MAC_DefineSegment  <BSS_TEXT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_TEXT>
MAC_DefineSegment  <BSS_TIME>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_TIME>
MAC_DefineSegment  <BSS_SPEAKER>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_SPEAKER>
MAC_DefineSegment  <BSS_INT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_INT>
MAC_DefineSegment  <BSS_STRING>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_STRING>
MAC_DefineSegment  <BSS_PORT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_PORT>
MAC_DefineSegment  <BSS_HEAP>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_HEAP>
MAC_DefineSegment  <BSS_IS>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_IS>
MAC_DefineSegment  <BSS_TO>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_TO>
MAC_DefineSegment  <BSS_FONT>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_FONT>
MAC_DefineSegment  <BSS_COMPRESS>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_COMPRESS>
MAC_DefineSegment  <BSS_DOS>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_DOS>
MAC_DefineSegment  <BSS_IDE>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_IDE>
MAC_DefineSegment  <BSS_NODE>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_NODE>
MAC_DefineSegment  <BSS_GO32>, <BYTE>, <PUBLIC>, <USE16>, <CLASS_BSS_GO32>
;
MAC_DefineSegment  <STACK_DEFAULT>, <PARA>, <STACK>, <USE16>, <CLASS_STACK_DEFAULT>
MAC_DefineSegment  <_STACK>, <PARA>, <PUBLIC>, <USE16>, <STACK> ; Complete Borland C/C++ 
;
MAC_DefineSegment  <HEAP_DEFAULT>, <PARA>, <PUBLIC>, <USE16>, <CLASS_HEAP_DEFAULT>
;
DGROUP             GROUP _CONST, _DATA, _CVTSEG, _SCNSEG, _BSS, _BSSEND

; End of source
