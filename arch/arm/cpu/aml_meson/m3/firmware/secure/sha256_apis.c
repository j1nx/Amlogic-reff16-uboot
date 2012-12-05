#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <romboot.h>
#include "softcrypto.h"
#include <tfm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <Sha256Calc.h>
#include <sha256.h>
#include <config.h>
#include <apis.h>
//====================================================================
int do_comm_sha256sum(char *pSRC, int nDataLen, char *pOUT)
{
	Sha256Calc s1;
	Sha256Calc_reset(&s1);
    Sha256Calc_calculate(&s1,pSRC,nDataLen);
    memcpy(pOUT, s1.Value, 32);
    return 0;	
}

int do_efuse_sha256sum(char *pSRC, int nDataLen, char *pOUT)
{
	sha256_hash_t dest;
	sha256sum(&dest, pSRC, nDataLen);
	memcpy(pOUT, dest, 32);
	return 0;	
}