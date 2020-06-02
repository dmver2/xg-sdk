#ifndef __crc32_h
#define __crc32_h

#if defined ( __cplusplus )
extern "C" {
#endif

#ifdef _WIN32
 #ifndef STDCALL
  #define STDCALL   __stdcall
 #endif
 #ifdef _CRC32_DLL
  #ifdef _USRDLL
   #define CRC32_DLL_API   __declspec(dllexport)
  #else
   #define CRC32_DLL_API   __declspec(dllimport)
  #endif
 #else
  #define CRC32_DLL_API      /* void */
 #endif
#else
 #define STDCALL               /* void */
 #define CRC32_DLL_API         /* void */
#endif


CRC32_DLL_API 
unsigned long STDCALL 
crc32( unsigned long crc, const unsigned char * buf, unsigned int len );

CRC32_DLL_API 
const unsigned long * STDCALL get_crc_table();

#if defined ( __cplusplus )
};
#endif

#endif // __CRC32_H