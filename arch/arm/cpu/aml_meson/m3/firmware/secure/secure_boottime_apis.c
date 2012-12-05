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
#include <config.h>
#include <apis.h>

#define IO_AHB_BASE		0x49000000
#define RSA_HASH_SIZE (32)

#define P_CI				COMMON_DATA_PHY_BASE
#define P_IN				(P_CI+(((sizeof(secure_ci_t)+3)/4)*4))
#define P_SN				(P_IN+(((sizeof(secure_in_t)+3)/4)*4))
#define P_SECU_RA	(P_SN+(((sizeof(secure_sn_t)+3)/4)*4))
#define P_AUTH_RA	(P_SECU_RA+(((sizeof(secure_ra_t)+3)/4)*4))

static secure_sn_t	*psn;
static secure_ra_t	*pauth_ra;
static secure_ra_t	*psecu_ra;
static secure_in_t	*pin;
static secure_ci_t	*pci;

/*#ifdef CONFIG_AUTH_RSA_IN_SPINOR
static int get_secu_ra_key(fp_int *n, fp_int *e, fp_int *d);
extern int do_rsa_proc(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,fp_int *key, int procedure);
#endif*/

extern int get_auth_ra_hash(char *pOut, int from);

// for AUTH RSA key in SPI
//void secure_boottime_init(char *pSrc, int nDataLen) __attribute__ ((section ("SECURE_BOOT_ENTRY")));
//void secure_boottime_init(char *pSrc, int nDataLen)
void secure_boottime_init() __attribute__ ((section ("SECURE_BOOT_ENTRY")));
void secure_boottime_init()
{	
	psn = P_SN;
	pauth_ra = P_AUTH_RA;
	psecu_ra = P_SECU_RA;
	pin = P_IN;
	pci = P_CI;
	
	// initial all global variables status 
	psn->status = -1;
	pauth_ra->status = -1;
	psecu_ra->status=-1;
	pin->status = -1;
	pci->status = -1;
		
#ifdef SECURE_EFUSE_DEBUG
extern void efuse_debug_init();
	efuse_debug_init();	
#endif	
		
	// intial secure RA from SRAM
	unsigned char *szTxtN = IO_AHB_BASE+SECU_RSA_N_OFFSET; //SRAM location
	unsigned char *szTxtE = IO_AHB_BASE+SECU_RSA_E_OFFSET; //SRAM location
	//unsigned char *szTxtD = IO_AHB_BASE+16*1024+1*1024-128*1; //SRAM location
		
	memset(psecu_ra, 0, sizeof(*psecu_ra));
	memcpy(psecu_ra->n, szTxtN, 128);
	memcpy(psecu_ra->e, szTxtE, 5);
	//memcpy(psecu_ra->d, szTxtD, 128);
	psecu_ra->status = 0;
	
	int ret;		
	char out[256];		
	int nOutLen=0;	
	int procedure;
	memset(pauth_ra, 0, sizeof(*pauth_ra));
	pauth_ra->status = -1;
	memset(out, 0, 256);
	
	// 1. authorize key decrypt by secure key
/*#ifdef CONFIG_AUTH_RSA_IN_SPINOR
	ret = -1;
	if(pSrc && (nDataLen==256)){	
		fp_int  key[3];
		memset(&key[0],0,sizeof(key));	
		if(!get_secu_ra_key(&key[0],&key[1],&key[2])){			
			procedure = DEC_WITH_PUB|RSA_BUF_IN_TYPE_IGNORE|RSA_BUF_OUT_TYPE_IGNORE;
			ret = do_rsa_proc(pSrc, nDataLen, out, &nOutLen, &(key[0]), procedure);
		}				
	}
#endif*/

#ifdef CONFIG_AUTH_RSA_IN_SRAM
	ret = -1;
	unsigned char *pAuth=IO_AHB_BASE+AUTH_RSA_N_OFFSET;
	memcpy(out, pAuth, 256);
	ret=0;	
	/*ret = -1;
	if(pSrc && (nDataLen==256)){		
		memcpy(out, pSrc, 256);
		ret = 0;
	}*/	
#endif	
	
// 2. authorize key hash verify, store it 	
	char hash[RSA_HASH_SIZE];
	char hashsum[RSA_HASH_SIZE];				
	if(!ret){
		ret = -1;		
		if((!get_auth_ra_hash(hash, FROM_UBOOT)) && (!do_efuse_sha256sum(out, 256, hashsum)))
				ret = 0;		
		if(!ret && (!memcmp(hash, hashsum, RSA_HASH_SIZE))){
				memcpy(pauth_ra->n, out, 128);
				memcpy(pauth_ra->e, out+128, 5);
				pauth_ra->status = 0;		
		}
	}	
	// for debug
	/* memcpy(pauth_ra->n, out, 128);
	memcpy(pauth_ra->e, out+128, 5);
	pauth_ra->status = 0;		*/
	
	// 3. initial SN/IN/CI
	char buf[128];
	char *pTemp;
	
	//SN
	memset(buf, 0, 128);
	pTemp = &(psn->factory_id[0]);
	if(!get_sn(buf, FROM_UBOOT)){		
		memcpy(pTemp, buf, CONFIG_EFUSE_SN_DATA_SIZE);
		psn->status = 0;
	}
	else{
		memset(pTemp, 0, CONFIG_EFUSE_SN_DATA_SIZE);
		psn->status = -1;
	}
	
	//IN
	memset(buf, 0, 128);
	pTemp = &(pin->factory_id[0]);
	if(!get_in(buf, FROM_UBOOT)){		
		memcpy(pTemp, buf, CONFIG_EFUSE_IN_DATA_SIZE);
		pin->status = 0;
	}
	else{
		memset(pTemp, 0, CONFIG_EFUSE_IN_DATA_SIZE);
		pin->status = -1;
	}
	
	//CI
	memset(buf, 0, 128);
	pTemp = &(pci->data[0]);	
	if(!get_ci(buf, FROM_UBOOT)){		
		memcpy(pTemp, buf, CONFIG_EFUSE_CI_DATA_SIZE);
		pci->status = 0;
	}
	else{
		memset(pTemp, 0, CONFIG_EFUSE_CI_DATA_SIZE);
		pci->status = -1;
	}

		
#ifdef SECURE_DEBUG				
		unsigned char *pdest;
		
		// EFUSE HASH output
		//memset(hash, 0xff, 32);		
		//get_auth_ra_hash(hash);		
		ret = 0;
		pdest = 0x49006000;
		memcpy(pdest, hash, 32);

		// SHA256 calculate HASH output
		//do_efuse_sha256sum(out, 256, hashsum);
		ret = 0;				
		pdest = 0x49006080;
		memcpy(pdest, hashsum, 32);	
		
		// authorize key output
		pdest = 0x49005000;
		memcpy(pdest, pauth_ra->n, 128);
		pdest = 0x49005080;
		memcpy(pdest, pauth_ra->e, 5);	

	// debug for init SN/CI/IN
	//unsigned char szSN[] = "MRVLCTCC20120712SN000001";
	//unsigned char szCI[] = "MRVL";
	//unsigned char szIN[] = "MRVL20120712CHIPSN000001";

	pdest = 0x49006100;
	memcpy(pdest, &(psn->factory_id[0]), CONFIG_EFUSE_SN_DATA_SIZE);

	pdest = 0x49006120;
	memcpy(pdest, &(pin->factory_id[0]), CONFIG_EFUSE_IN_DATA_SIZE);
	pdest = 0x49006140;
	memcpy(pdest, &(pci->data[0]), CONFIG_EFUSE_CI_DATA_SIZE);
	
#endif	

	return;	
}

//====================================================================
// interface APIs
//====================================================================
/*#ifdef CONFIG_AUTH_RSA_IN_SPINOR
static int get_secu_ra_key(fp_int *n, fp_int *e, fp_int *d)
{
	int nRet = 1;
	if(!n || !e || !d)
		return nRet;
	if(psecu_ra->status != 0)
		return nRet;
	
	fp_init(n);
	fp_init(e);
	fp_init(d);
	//memcpy(n->dp,szTxtN,128);
	//memcpy(e->dp,szTxtE,128);
	//memcpy(d->dp,szTxtD,128);
	memcpy(n->dp, psecu_ra->n, 128);
	memcpy(e->dp,psecu_ra->e,5);
	memcpy(d->dp, psecu_ra->d, 128);
	
	n->used=FP_SIZE;
	fp_clamp(n);			
	e->used=FP_SIZE;
	fp_clamp(e);			
	d->used=FP_SIZE;
	fp_clamp(d);
	
	nRet = 0;		
	
	return nRet;	
}
#endif */

