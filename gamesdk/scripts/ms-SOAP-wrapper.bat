@echo off

ECHO ================ Generate wrapper for soap client classes ======================
set WSDL_FILE="..\src\wsdl\GameService3.wsdl"
set OUT_DIR="..\src\soap2"

::
:: Delete all gen-src files
::
DEL %OUT_DIR%\*.* /Q

"c:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\bin\sproxy.exe" /wsdl %WSDL_FILE% /out:%OUT_DIR%\GameService3.h

ECHO ========== Generate wrapper for soap client classes was finished ===============
