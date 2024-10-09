@echo off

REM del *.pdb

set COMMON_FLAGS=/Od /W2 /Z7 /EHsc /wd4996 /nologo /MD
set BUILD_FLAGS=%COMMON_FLAGS%  /link
cl test.c base.c /Fetest.exe %BUILD_FLAGS% 

del *.ilk
del *.obj
