@set sysname=ModifyCMD
@set filename=%sysname%.sys
@echo %sysname%
@echo %filename%
del %filename%
pause
copy \\VBOXSVR\trunk\Driver\%sysname%\%sysname%\objchk_win7_x86\i386\%filename% .\%filename%
pause
dosys.exe -i %sysname% C:\work\%filename%