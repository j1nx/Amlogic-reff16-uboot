/** ######################################
    Sha256 Calculator
-- Relative Module: --
-- Author: Cylock
##########################################
*/

#ifndef _SHA256CALC_H
#define _SHA256CALC_H 1

#include "Base.h"

typedef struct _Sha256Calc{ 
	SZ_CHAR  Value[ 32 ]; 
	SZ_INT  DwordBufBytes;
	SZ_INT  ByteNumLo;
	SZ_INT  ByteNumHi;
	SZ_INT  reg[ 8 ]; /** h0 to h 7 -- old value store*/
	SZ_INT  DwordBuf[ 16 ]; /** data store */
	SZ_INT  Padding[ 64 ];
}Sha256Calc;

SZ_T Sha256Calc_init( Sha256Calc* ObjPtr );
SZ_T Sha256Calc_uninit( Sha256Calc* ObjPtr );
SZ_T Sha256Calc_reset( Sha256Calc* ObjPtr );
SZ_T Sha256Calc_calculate( Sha256Calc* ObjPtr, SZ_CHAR* DataPtr, SZ_T DataLen );

#endif
