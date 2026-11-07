@echo off
rem jlink_upload.bat <JLink.exe> <dispositivo> <kHz> <archivo.hex>
setlocal
set "JLINK=%~1"
set "DEV=%~2"
set "SPEED=%~3"
set "HEX=%~4"
set "SCRIPT=%~dp4jlink_upload.jlink"
(
  echo r
  echo h
  echo loadfile "%HEX%"
  echo r
  echo g
  echo qc
) > "%SCRIPT%"
"%JLINK%" -device %DEV% -if SWD -speed %SPEED% -autoconnect 1 -NoGui 1 -ExitOnError 1 -CommandFile "%SCRIPT%"
exit /b %ERRORLEVEL%
