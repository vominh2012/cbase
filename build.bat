@echo off

REM del *.pdb

set COMMON_FLAGS=/Od /W3 /Z7 /EHsc /wd4996 /nologo /MD
set BUILD_FLAGS=%COMMON_FLAGS% /link

cl base.cpp test.cpp /Fetest.exe %BUILD_FLAGS% 
lib /nologo base.obj

REM cl dll_export_gen.cpp  %BUILD_FLAGS% 
REM cl scratch.c /Fescratch.exe %BUILD_FLAGS% 

REM del *.ilk
REM del *.obj
