@echo off
::
:: Change the path, if the path is changed
::
SET WSDL_EXE="..\bin\wsdl.exe"

::
:: Change the url, if the game service was changed
::
:: @SET WSDL_URI="http://195.34.7.52:9007/game-service/services/GameService?wsdl"
SET WSDL_URI="http://localhost:9007/game-service/services/GameService3?wsdl"

 
::
:: Change the path or the name wsdl file, if they were changed
::
SET WSDL_FILE=%1
IF "%WSDL_FILE%"=="" SET WSDL_FILE="..\src\wsdl\GameService3.wsdl"

::
:: Deleting wsdl file
::
DEL %WSDL_FILE% /Q

::
:: Saving wsdl file
::
%WSDL_EXE% %WSDL_URI% %WSDL_FILE%
