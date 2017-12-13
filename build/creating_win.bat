:START
@echo off
echo             Engine-Skript
echo             =============
echo.
echo   [1] Windows32 Debug
echo   [2] Windows32 Release
echo   [3] Alle Obj-dateien löschen
echo   [4] Erstelle makefile
echo   [5] TileGameCoop build
echo   [6] CMD
echo   [7] Exit
echo.

set asw=0
set /p asw="Bitte Auswahl eingeben: "

SET folder="win\%DATE%\"
SET folder_release="win\game\"
SET folderTiled="win\tiled\"

if %asw%==1 goto WIND
if %asw%==2 goto WINR
if %asw%==4 goto MAKEFILE
if %asw%==3 goto OBJDELETING
if %asw%==5 goto TILED_BUILD
if %asw%==6 goto CMD
goto THEEND

:WIND
del %folder% /s /q
xcopy "..\bin\windebug32" %folder% /I
xcopy "..\res" %folder% /E /C /Q /I /Y
goto END

:WINR
del %folder_release% /s /q
xcopy "..\bin\winrelease32" %folder_release% /I
xcopy "..\res" %folder_release% /E /C /Q /I /Y
del %folder_release%\start.bat
del %folder_release%\test.bat
del %folder_release%\level.bat
goto END

:MAKEFILE
cbp2make.exe -in ..\src\TileGameCoop.cbp -out ..\src\makefile
goto END

:TILED_BUILD
del %folderTiled% /s /q
xcopy "..\bin\windebug32" %folderTiled% /I
xcopy "..\res" %folderTiled% /E /C /Q /I /Y
goto END

:CMD
cd ..\src\
cmd
goto :END


:END
goto START
:THEEND