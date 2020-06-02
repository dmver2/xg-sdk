/*	$Id: defs.h,v 1.4 2006/01/18 17:05:02  Exp $
** Header file with some common definitions
** for CAST-128 & CAST-256 algorithms
*/ 

#ifndef _DEFS_H_
#define _DEFS_H_

#include "types.h"

/* Macros to access 8-bit bytes out of a 32-bit word */
#define U8a(x) ( (u_char)  (x>>24) )
#define U8b(x) ( (u_char) ((x>>16)  & 255) )
#define U8c(x) ( (u_char) ((x>>8)   & 255) )
#define U8d(x) ( (u_char) ((x)      & 255) )

/* Circular left shift */
#define ROL(x, n) ( ((x)<<(n)) | ((x)>>(32-(n))) )

extern const u_long s_box1[];
extern const u_long s_box2[];
extern const u_long s_box3[];
extern const u_long s_box4[];
extern const u_long s_box5[];
extern const u_long s_box6[];
extern const u_long s_box7[];
extern const u_long s_box8[];

#endif /* ifndef _DEFS_H_ */