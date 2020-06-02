/*	$Id: decl6n.h,v 1.3 2005/12/09 08:37:07 dmitry.vereshchagin Exp $
 * preprocessor definitions
 */
#ifndef header_decl6n
#define header_decl6n

#if _MSC_VER > 1000
#pragma once
#endif

#include "Types.h"

#ifdef _DLLEXPORT
#define CxxLibExport  __declspec( dllexport )
#elif  _DLLIMPORT
#define CxxLibExport  __declspec( dllimport )
#else
#define CxxLibExport
#endif

#define DECLARE_NO_INSTANCE(clazz) private: \
	clazz() {};	\
	~clazz() {};

#define DECLARE_NO_COPY(clazz) private: \
	clazz(const clazz&) {}; \
	clazz& operator = (const clazz&) { return *this; };

#ifndef IN
#define IN
#endif // #ifndef IN

#ifndef OUT
#define OUT
#endif // #ifndef OUT

#ifndef IN_OUT
#define IN_OUT
#endif // #ifndef IN_OUT

#define ARRAY_LENGTH(x) sizeof (x) / sizeof (x[0])

#endif //#ifndef header_decl6n