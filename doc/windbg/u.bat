@set sysname=ModifyCMD
@set filename=%sysname%.sys
@echo %sysname%
@echo %filename%
dosys.exe -u %sysname% C:\work\%filename%