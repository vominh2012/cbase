@echo off

REM del *.pdb

set COMMON_FLAGS=/D_DEBUG /Od /W3 /Z7 /EHsc /wd4996 /nologo /MDd
set BUILD_FLAGS=%COMMON_FLAGS% /link

cl /FC /fsanitize=addressx base.cpp test.cpp /Fetest.exe %BUILD_FLAGS% 
REM cl base.cpp test.cpp /Fetest.exe %BUILD_FLAGS% 
lib /nologo base.obj

REM cl dll_export_gen.cpp  %BUILD_FLAGS% 
REM cl scratch.c /Fescratch.exe %BUILD_FLAGS% 

cl /D_DEBUG /EHsc /W4 /MDd debug_malloc.cpp

REM del *.ilk
del *.obj
