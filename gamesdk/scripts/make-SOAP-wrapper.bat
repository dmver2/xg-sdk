
@ECHO ================ Generate wrapper for soap client classes ======================
@echo off
set WSDL_FILE="..\src\wsdl\GameService3.wsdl"

::
:: Delete all gen-src files
::
DEL ..\src\soap\*.* /Q

::
:: Change the path, if you installed axis library other directory
::
SET win_client_classpath=..\external\axis\wsdl2ws.jar;..\external\axis\axis.jar;..\external\axis\axis-ant.jar;..\external\axis\commons-discovery.jar;..\external\axis\commons-logging.jar;..\external\axis\jaxrpc.jar;..\external\axis\log4j-1.2.8.jar;..\external\axis\saaj.jar;..\external\axis\wsdl4j.jar;..\external\axis\xml-apis.jar

ECHO ================================================================================



ECHO =========================== Saving wsdl file ===================================

::
:: Get wsdl file from a web-site. 
:: Note: If you want change the url, you'll see the SaveWsdlInFile.exe file
::
:: CALL wsdl.bat %WSDL_FILE%

ECHO ================================================================================

ECHO ========================== Generating stubs files ==============================

java -classpath %win_client_classpath% org.apache.axis.wsdl.wsdl2ws.WSDL2Ws %WSDL_FILE% -o..\src\soap -lc++ -sclient

ECHO ================================================================================



ECHO ========== Generate wrapper for soap client classes was finished ===============
