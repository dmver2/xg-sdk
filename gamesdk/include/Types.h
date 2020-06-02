/*++

Copyright (c) 1991-1996 Microsoft Corporation

Module Name:

  types.h

Abstract:

   This module contains the primitive system data types described
   in section 2.6 of IEEE P1003.1/Draft 13 as well as type definitions
   for sockets and streams

--*/

#ifndef _TYPES_H
#define _TYPES_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <tchar.h>
#include <string>

#pragma warning(disable : 4995)

#if defined(_TCHAR_DEFINED) && defined(_MBCS)
	#define stringA std::string
	#define stringW std::wstring
	#define string stringA
#elif defined(_TCHAR_DEFINED) && defined(_UNICODE)
	#define stringA std::string
	#define stringW std::wstring
	#define string stringW
#else
	#define stringA std::string
	#define string stringA
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
 *   POSIX data types
 */


typedef unsigned char	u_char;
typedef unsigned short	u_short;
typedef unsigned short	ushort;
typedef unsigned int	   u_int;
typedef unsigned long	u_long;

typedef unsigned int    uint;
typedef unsigned long   ulong;
typedef unsigned char   uchar;

typedef char            *caddr_t;
typedef int             key_t;
// typedef long            time_t;

// typedef signed char         byte;
typedef signed short        int16;
typedef signed int          int32;
typedef signed __int64      int64;
typedef unsigned char       byte;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned __int64    uint64;

#ifdef __cplusplus
}
#endif

#endif  /* _TYPES_H */
