
#ifndef _BASE_H
#define _BASE_H 1

#include "SizeDef.h"

#ifndef BASE_INLINE
#define BASE_INLINE inline
#endif

#define NULL_PTR		0
#define NULL_HANDLE		0

#define FLAG_END		0
#define FLAG_NULL		0
#define FLAG_CONTINUE		1

#define RET_OK			0
#define RET_ERR			-1
#define RET_CREATE_FALSE 	1
#define RET_RELEASE_FALSE 	2
#define RET_ALLOC_MEM_FALSE 	3
#define RET_MEM_EMPTY		4
#define RET_HANDLE_EMPTY	5
#define RET_INVAILD_HANDLE	6
#define RET_INVAILD_PTR		7
#define RET_SRC_TOO_LONG 	8
#define RET_DST_TOO_SHORT       9
#define RET_OVER_LIMIT         10

#define USHR(a,n) ((a>>n)&~((~0)<<((sizeof(a)<<3 )-n))


/** 
   Normally, __cdecl is the C function protocol, but in win32,
it use the __stdcall, and win64, it use another protocol like __fastcall.
Here define a macro for call protocol.
*/
#ifndef METHODCALL
#define METHODCALL
#endif

/** ---------------------------------------------
   Interface Part 
 ------------------------------------------------
*/



/** ---------------------------------------------
  Memory Part
 ------------------------------------------------
*/
#define Base_AllocObj( n ) malloc( n )
#define Base_FreeObj( p )  free( p )
#define Base_AllocMem( n ) malloc( n )
#define Base_FreeMem( p )  free( p )

#endif
