# Game SDK library

#!include <win32.mak>

TARGET = $(TARGET)

all: $(TARGET)

cflags = /FR"Release\" /GS /analyze- /W3 /Zc:wchar_t /I"include" /I"external\include" /Zi /O2 /Fd"Release\vc142.pdb" /Zc:inline /fp:precise /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_DLLEXPORT" /D "_VC80_UPGRADE=0x0710" /D "_MBCS" /errorReport:prompt /WX- /Zc:forScope /Gd /Oy- /MD /FC /Fa"Release\" /EHsc /nologo /Fo"Release\" /Fp"Release\gamesdk_dll.pch" /diagnostics:column 
cdebug = 

conlibs = "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /DEBUG /MACHINE:X86 /SAFESEH /INCREMENTAL
ldflags = /OUT:"Release\$(TARGET)" /MANIFEST /PDB:"Release\gamesdk_dll.pdb" /DYNAMICBASE:NO /DEBUG /DLL /MACHINE:X86 /OPT:REF /SAFESEH /INCREMENTAL:NO /PGD:"Release\gamesdk_dll.pgd" /SUBSYSTEM:WINDOWS /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /ManifestFile:"Release\$(TARGET).intermediate.manifest" /OPT:ICF /ERRORREPORT:PROMPT /NOLOGO /LIBPATH:"external\lib"

#ldebug = 

#SRCS = "CryptoFactory.cpp" "cast128.cpp" "cast256.cpp" "constants.cpp" "Crypto.cpp" "event_listener.cpp" "event_listener_impl.cpp" "event_trigger.cpp" "event_trigger_impl.cpp" "exception.cpp" "GameInfo.cpp" "GameInfo_Impl.cpp" "GamePortal.cpp" "GamePortal_Impl.cpp" "GAMESDK.cpp" "GameSession.cpp" "GameSession_Impl.cpp" "exception_impl.cpp" "inputstream.cpp" "InputStream_Memory.cpp" "log.cpp" "Log2File.cpp" "Log2File_Impl.cpp" "NetException.cpp" "NetSession.cpp" "event_trigger_socket.cpp" "host.cpp" "hostgroup.cpp" "host_impl.cpp" "inputstream_packet.cpp" "inputstream_packet_impl.cpp" "inputstream_socket.cpp" "IPAddress.cpp" "netobject_client.cpp" "netobject_client_impl.cpp" "netobject_controller.cpp" "netobject_controller_impl.cpp" "netobject_server.cpp" "netobject_server_impl.cpp" "NetInterface.cpp" "NetInterface_Impl.cpp" "Network_Setup.cpp" "NotifyController.cpp" "outputstream_packet.cpp" "outputstream_packet_impl.cpp" "outputstream_socket.cpp" "packet.cpp" "packet_impl.cpp" "socket.cpp" "socket_impl.cpp" "socket_selector.cpp" "outputstream.cpp" "outputstream_memory.cpp" "Player.cpp" "Player_Impl.cpp" "slot.cpp" "slot_impl.cpp" "System.cpp" "Thread.cpp" "Timer.cpp" "TimerTask.cpp" "Timer_Impl.cpp" "Util.cpp" 

OBJS = CryptoFactory.obj cast128.obj cast256.obj constants.obj Crypto.obj event_listener.obj event_listener_impl.obj event_trigger.obj event_trigger_impl.obj exception.obj GameInfo.obj GameInfo_Impl.obj GamePortal.obj GamePortal_Impl.obj GAMESDK.obj GameSession.obj GameSession_Impl.obj exception_impl.obj inputstream.obj InputStream_Memory.obj log.obj Log2File.obj Log2File_Impl.obj NetException.obj NetSession.obj event_trigger_socket.obj host.obj hostgroup.obj host_impl.obj inputstream_packet.obj inputstream_packet_impl.obj inputstream_socket.obj IPAddress.obj netobject_client.obj netobject_client_impl.obj netobject_controller.obj netobject_controller_impl.obj netobject_server.obj netobject_server_impl.obj NetInterface.obj NetInterface_Impl.obj Network_Setup.obj NotifyController.obj outputstream_packet.obj outputstream_packet_impl.obj outputstream_socket.obj packet.obj packet_impl.obj socket.obj socket_impl.obj socket_selector.obj outputstream.obj outputstream_memory.obj Player.obj Player_Impl.obj slot.obj slot_impl.obj System.obj Thread.obj Timer.obj TimerTask.obj Timer_Impl.obj Util.obj 

.src.obj:
  $(cc) $(cdebug) $(cflags) $*.cpp

$(TARGET): $(OBJS)
  link $(ldflags) $(ldebug) $(conflags) $(conlibs) $(OBJS)
  
clean:
  -del Release\$(TARGET)\*.obj
  -del Release\$(TARGET)\*.exe

.PHONY: all clean
