@echo off
echo             Engine-Skript
echo             =============
echo.
echo   [1] Windows32 Debug
echo   [2] Windows32 Release
echo   [3] Alle Obj-dateien löschen
echo   [4] Erstelle makefile
echo   [5] CMD
echo   [6] Exit
echo.

set asw=0
set /p asw="Bitte Auswahl eingeben: "

SET folder="build\"

if %asw%==1 goto WIND
if %asw%==2 goto WINR
if %asw%==4 goto MAKEFILE
if %asw%==3 goto OBJDELETING
if %asw%==5 goto CMD
goto END

:WIND
del %folder% /s /q
xcopy "bin\WinDebug32" %folder% /I
xcopy "res" %folder% /E /C /Q /I /Y
goto END

:WINR
del %folder% /s /q
xcopy "bin\win\WinRelease32" %folder% /I
xcopy "res" %folder% /E /C /Q /I /Y
goto END

:MAKEFILE
cbp2make.exe -in src/TileGameCoop.cbp -out makefile
goto END



:CMD
cmd
goto :END


:END
