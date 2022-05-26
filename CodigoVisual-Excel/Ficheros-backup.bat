@echo off
set/p x="Pulse 'y' si quiere clonar los tres archivos: "
:while
if %x%== y copy /y FicheroTemperatura.txt CopiaTemperatura.txt && copy /y FicheroLuminosidad.txt CopiaLuminosidad.txt && copy /y FicheroHumedad.txt CopiaHumedad.txt
timeout /t 1
goto :while
pause