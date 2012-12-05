/** #######################################
    Sha256 Calculator
-- Static Space Used: 64 * 4 Bytes
###########################################
*/

#include "Sha256Calc.h"

SZ_T Sha256Calc_calcBlock( SZ_INT* DwordBufPtr, SZ_INT* regPtr );

SZ_INT Sha256Calc_k[ 64 ] = {
       0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
       0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
       0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
       0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
       0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
       0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
       0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
       0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};


SZ_T  Sha256Calc_init( Sha256Calc* t ){	return Sha256Calc_reset( t ); }

SZ_T Sha256Calc_uninit( Sha256Calc* t ){ return RET_OK; }

SZ_T Sha256Calc_reset( Sha256Calc* t ){ 
	SZ_INT x;
	t->reg[0]=0x6a09e667;
	t->reg[1]=0xbb67ae85;
	t->reg[2]=0x3c6ef372;
	t->reg[3]=0xa54ff53a;
	t->reg[4]=0x510e527f;
	t->reg[5]=0x9b05688c;
	t->reg[6]=0x1f83d9ab;
	t->reg[7]=0x5be0cd19;
	for( x = 0; x < 16; x ++ ){ t->Padding[ x ] = t->DwordBuf[ x ] = 0; }
	t->ByteNumLo = t->ByteNumHi = t->DwordBufBytes = 0;
	return RET_OK; 
}


SZ_T Sha256Calc_calculate( Sha256Calc* t, SZ_CHAR* dp, SZ_T dl ){
	SZ_INT w,x,y,z;
	SZ_INT reg[ 8 ];

	/** 
	   Set x, y, z, w 
	   x -- Dword Buffer Offset
	   y -- Bytes Offset in a Dword * 8
	   z -- Source Data Idx
	   w -- 64Bytes Block Number
	*/
	z = ( SZ_INT )(( t->ByteNumLo & 0x7fffffff ) + dl);
	x = (( t->ByteNumLo >> 31 ) & 0x01 ) + (( z >> 31 ) & 0x01 );
	z = ( z & 0x7fffffff ) | ( x << 31 );
	y = t->ByteNumHi + ( x >> 1 );
	if( y > 0x1fffffff ){ return RET_OVER_LIMIT; }

	t->ByteNumLo = z; 
	t->ByteNumHi = y;

	for( w = 0; w < 16; w ++ ){ t->Padding[ w ] = t->DwordBuf[ w ]; }
	for( w = 0; w < 8; w ++ ){ reg[ w ] = t->reg[ w ]; }
	x = t->DwordBufBytes / 4;
	y = ( t->DwordBufBytes & 3 ) * 8;
	w = ( SZ_INT )((( t->DwordBufBytes + dl ) >> 6 ) & 0x03ffffff );
	z = 0;

	/** 
	    64Bytes Full Blocks Loop -- SHA1 is Big endian Order !
	*/
	while( w > 0 ){
		while( x < 16 ){
			t->Padding[ x ] = ( t->Padding[ x ] & ~( 0x000000ff << ( 24 - y ))) | (( dp[ z ] & 0x000000ff ) << ( 24 - y ));
			y = y + 8; x += ( y >> 5 ); y &= 0x1f; z ++; 
		}
		Sha256Calc_calcBlock( t->Padding, reg );
		x = y = 0; w --;
	}
	
	/** Clear Dirty Data */
	if(( x | y ) == 0 ){
		for( w = 0; w < 16; w ++ ){ t->DwordBuf[ w ] = t->Padding[ w ] = 0; }
	}

	/** 
	   Last Blocks Loop 
	   z is how many bytes used. dl is bigger than z. -- Also Big endian Order !
	*/
	w = ( SZ_INT )( dl - z ); 
	while( w > 0 ){
		t->DwordBuf[ x ] = t->Padding[ x ] = 
			( t->Padding[ x ] & ~( 0x000000ff << ( 24 - y ))) | (( dp[ z ] & 0x000000ff ) << ( 24 - y ));
		y = y + 8; x += ( y >> 5 ); y &= 0x1f; z ++;
		w --;
	}

	/** Append 0x80 */
	t->Padding[ x ] |= 0x00000080 << ( 24 - y ); 

	/** Save Old Value */
	for( w = 0; w < 8; w ++ ){ t->reg[ w ] = reg[ w ]; }
	
	/** Check if Length + 0x80 could append */
	t->DwordBufBytes =  x * 4  + ( y >> 3 );
	if( t->DwordBufBytes > 55 ){
		Sha256Calc_calcBlock( t->Padding, reg );
		for( w = 0; w < 16; w ++ ){ t->Padding[ w ] = 0; }
	}
		
	/** Append Length */
	t->Padding[ 15 ] = t->ByteNumLo << 3;
	t->Padding[ 14 ] = (( t->ByteNumLo >> 29 ) & 0x3 ) | ( t->ByteNumHi << 3 );
	Sha256Calc_calcBlock( t->Padding, reg );

	/** Output ordered Value -- big endian order */
	for( w = 0, x = 0; w < 32; w ++, x = ( x + 8 ) & 0x1f ){
		/** high byte first output value */
		t->Value[ w ] = ( SZ_CHAR )( reg[ w >> 2 ] >> ( 24 - x ));
	}

	return RET_OK;
}


/** calculate 64 dwords block -- data is at first 16 dwords */
SZ_T Sha256Calc_calcBlock( SZ_INT* dp, SZ_INT* rp ){
	SZ_INT a,b,c,d,e,f,g,h;
	SZ_INT t0,t1,t2;
	SZ_INT x;

	/** extend 16s dword to 64 dwords */	
	for( x = 16; x < 64; x ++ ){

		t0 =  ((( dp[ x - 15 ] >> 7 ) & 0x1ffffff )  | ( dp[ x - 15 ] << 25 )) ^ 
		      ((( dp[ x - 15 ] >> 18 ) & 0x3fff )    | ( dp[ x - 15 ] << 14 )) ^ 
		      (( dp[ x - 15 ] >> 3  ) & 0x1fffffff );

		t1 =  ((( dp[ x - 2 ] >> 17 ) & 0x7fff ) | ( dp[ x - 2 ] << 15 )) ^ 
		      ((( dp[ x - 2 ] >> 19 ) & 0x1fff ) | ( dp[ x - 2 ] << 13 )) ^ 
		      (( dp[ x - 2 ] >> 10 ) & 0x3fffff );

		dp[ x ] = dp[ x - 16 ] + t0 + dp[ x - 7 ] + t1;
	}

	/** init value */
 	a = rp[0]; b = rp[1]; c = rp[2]; d = rp[3]; e = rp[4]; f = rp[5]; g = rp[6]; h = rp[ 7 ];

	/** main loop */
	for( x = 0; x < 64; x ++ ){
      
		t2 = ((( a >> 2 ) & 0x3fffffff ) | ( a << 30 )) ^ 
		     ((( a >> 13 ) & 0x7ffff ) | ( a << 19 )) ^
		     ((( a >> 22 ) & 0x3ff ) | ( a <<10 ));
		t0 = ( a & b ) ^ ( a & c ) ^ ( b & c );
		t2 = t2 + t0;
		
		t1 = ((( e >> 6 ) & 0x3ffffff ) | ( e << 26 )) ^ 
		     ((( e >> 11 ) & 0x1fffff ) | ( e << 21 )) ^ 
		     ((( e >> 25 ) & 0x7f ) | ( e << 7 ));
		t0 = ( e & f ) ^ ((~e) & g );
		t1 = h + t1 + t0 + Sha256Calc_k[ x ] + dp[ x ];
		h=g; g=f; f=e; e=d+t1; d=c; c=b; b=a; a=t1+t2; 
	}

	rp[0] += a; rp[1] += b; rp[2] += c; rp[3] += d; rp[4] += e; rp[5] += f; rp[6]+=g; rp[7]+=h;

	return RET_OK;
}



