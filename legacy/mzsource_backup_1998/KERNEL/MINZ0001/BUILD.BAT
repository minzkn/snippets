@ECHO OFF
ECHO.
ECHO Building system. . .
ECHO.
ECHO.

ECHO Compiling.
TASM.EXE /os /t /mx /m4 /z *.ASM
BCC.EXE -ml -c -3 -M- -K- -k- -N- MAIN.C

ECHO Library.
DEL MZLIB.LIB
TLIB.EXE MZLIB.LIB + @MZLIB.TBL

ECHO Linking.
TLINK.EXE /x /Tde /3 KERNEL.OBJ MZLIB.LIB, KERNEL.EXE
TLINK.EXE /x /Tde /3 MZLIB.LIB MAIN.OBJ, KERNELC.EXE

ECHO Deleting.
DEL *.OBJ

ECHO End of build.
ECHO.

REM End of batch
