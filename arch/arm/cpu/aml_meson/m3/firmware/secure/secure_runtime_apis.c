/***************************************************************************************************************
 * Secure Domain runtime APIs
 * 
 *
 * Copyright (c) 2012 Amlogic Inc.
 * 
 * Create date: 2012.07.22
 *
 **************************************************************************************************************/
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

#define P_CI				COMMON_DATA_BASE
#define P_IN				(P_CI+(((sizeof(secure_ci_t)+3)/4)*4))
#define P_SN				(P_IN+(((sizeof(secure_in_t)+3)/4)*4))
#define P_SECU_RA	(P_SN+(((sizeof(secure_sn_t)+3)/4)*4))
#define P_AUTH_RA	(P_SECU_RA+(((sizeof(secure_ra_t)+3)/4)*4))

static secure_sn_t	*psn;
static secure_ra_t	*pauth_ra;
static secure_ra_t	*psecu_ra;
static secure_in_t	*pin;
static secure_ci_t	*pci;

static int secure_cmd_init(void);
//static int secure_cmd_update_auth_ra(char *pSrc, int nDataLen);
static int secure_cmd_rsa_proc_dec_me(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,int procedure);
static int secure_cmd_rsa_dec_via_secu_key(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,int procedure);
static int secure_cmd_sha_proc_enc_iv(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen);
static int secure_cmd_gain_sn(char *pOUT, int *pOUTLen);
static int secure_cmd_rsa_proc_dec_sn(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,int procedure);
static int secure_cmd_rsa_proc_dec_in(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,int procedure);
static int secure_cmd_check_in_free();
static int secure_cmd_burn_sn_raw(char *pSRC, int nDataLen, char *pOUT, int *pOUTLen);
static int secure_cmd_burn_in_raw(char *pSRC, int nDataLen, char *pOUT, int *pOUTLen);
static int secure_cmd_burn_mac(char *pSRC, int nDataLen, char *pOUT, int *pOUTLen);
static int secure_cmd_gain_mac(char *pSRC, int nDataLen, char *pOUT, int *pOUTLen);
static int secure_cmd_sha_proc_common(char *pSRC, int nDataLen, char *pOUT, int *pOUTLen);
static int secure_cmd_gain_ci(char *pSRC, int nDataLen, char *pOUT, int *pOUTLen);

static int secure_gain_sn(void);
static int secure_gain_ci(void);
static int secure_gain_in(void);

static int get_auth_ra_key(fp_int *n, fp_int *e, fp_int *d);
static int get_secu_ra_key(fp_int *n, fp_int *e, fp_int *d);

extern int do_comm_sha256sum(char *pSRC, int nDataLen, char *pOUT);
extern int do_rsa_proc(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,fp_int *key, int procedure);

int secure_runtime_call(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,int procedure, int cmd)
{
	psn = P_SN;
	pauth_ra = P_AUTH_RA;
	psecu_ra = P_SECU_RA;
	pin = P_IN;
	pci = P_CI;
	
	switch(cmd){
		case SECURE_CMD_INIT:
			return(secure_cmd_init());			
		case SECURE_CMD_GAIN_SN:
			return(secure_cmd_gain_sn(pOUT, pOUTLen));			
		case SECURE_CMD_RSA_PROC_DEC_ME:
			return(secure_cmd_rsa_proc_dec_me(pSRC, nDataLen, pOUT, pOUTLen, procedure));
		case SECURE_CMD_SHA_PROC_ENC_IV:			
			return(secure_cmd_sha_proc_enc_iv(pSRC, nDataLen, pOUT, pOUTLen));	
		case SECURE_CMD_BURN_SN:
			return(secure_cmd_rsa_proc_dec_sn(pSRC, nDataLen, pOUT, pOUTLen, procedure));
		case SECURE_CMD_BURN_IN:
			return(secure_cmd_rsa_proc_dec_in(pSRC, nDataLen, pOUT, pOUTLen, procedure));
		case SECURE_CMD_RSA_DEC_VIA_SECU_KEY:
			return (secure_cmd_rsa_dec_via_secu_key(pSRC, nDataLen, pOUT, pOUTLen, procedure));
		case SECURE_CMD_CHECK_IN_FREE:
			return (secure_cmd_check_in_free());
		
		case SECURE_CMD_BURN_SN_RAW:
			return (secure_cmd_burn_sn_raw(pSRC, nDataLen, pOUT, pOUTLen));
		case SECURE_CMD_BURN_IN_RAW:
			return (secure_cmd_burn_in_raw(pSRC, nDataLen, pOUT, pOUTLen));
		
		case SECURE_CMD_BURN_MAC:
			return (secure_cmd_burn_mac(pSRC, nDataLen, pOUT, pOUTLen));
		case SECURE_CMD_GAIN_MAC:
			return (secure_cmd_gain_mac(pSRC, nDataLen, pOUT, pOUTLen));		
		
		case SECURE_CMD_SHA_PROC_COMMON:
			return (secure_cmd_sha_proc_common(pSRC, nDataLen, pOUT, pOUTLen));
		
		case SECURE_CMD_GAIN_CI:
				return (secure_cmd_gain_ci(pSRC, nDataLen, pOUT, pOUTLen));
				
//		case SECURE_CMD_UPDATE_AUTH_RA:
//			return(secure_cmd_update_auth_ra(pSRC, nDataLen));						
	}
	return -1;   // not support function	
}

static int secure_cmd_init(void)
{	
	if(psn->status != 0)
		secure_gain_sn();
	if(pci->status != 0)
		secure_gain_ci();
	if(pin->status != 0)
		secure_gain_in();
	
	if(!psn->status && !pci->status && !pin->status && !pauth_ra->status)
		return 0;
	
	return -1;	
}

//====================================================================
// Command APIs
//====================================================================
static int secure_cmd_gain_sn(char *pOUT, int *pOUTLen)
{
	int ret=-1;
	if(psn->status != 0)
		secure_gain_sn();
	
	if(psn->status == 0){
		if(*pOUTLen >= CONFIG_EFUSE_SN_DATA_SIZE){
			memcpy(pOUT, &(psn->factory_id[0]), CONFIG_EFUSE_SN_DATA_SIZE);
			*pOUTLen = CONFIG_EFUSE_SN_DATA_SIZE;
			ret = 0;
		}
	}	
	return ret;
}


//--------------------------------------------------------------------------------------------------------------------
static int secure_cmd_rsa_proc_dec_me(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,int procedure)
{	
	int nret = -1;
		
	if((nDataLen!= 128) || (*pOUTLen < 127))
		return nret;
	if ( NULL == pSRC || NULL == pOUT )
		return nret;
		
	//prepare key
	fp_int key[3];
	memset(&key[0],0,sizeof(key));		
	if(get_auth_ra_key(&key[0],&key[1],&key[2]))
		return nret;
	
	return(do_rsa_proc(pSRC, nDataLen, pOUT, pOUTLen, &(key[0]), procedure));	
}

//-------------------------------------------------------------------------------------
static int secure_cmd_rsa_dec_via_secu_key(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,int procedure)
{
	int ret = -1;
	if((nDataLen!= 128) || (*pOUTLen < 127))
		return ret;
	if ( NULL == pSRC || NULL == pOUT )
		return ret;
		
	//prepare key
	fp_int key[3];
	memset(&key[0],0,sizeof(key));		
	if(get_secu_ra_key(&key[0],&key[1],&key[2]))
		return ret;
	
	return(do_rsa_proc(pSRC, nDataLen, pOUT, pOUTLen, &(key[0]), procedure));		
}

//-------------------------------------------------------------------------------------
static int secure_cmd_rsa_proc_dec_sn(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,int procedure)
{
	int nret = -1;
/*		
	if((nDataLen!= 128) || (*pOUTLen < 127))
		return nret;
	if ( NULL == pSRC || NULL == pOUT )
		return nret;
*/
	if((nDataLen!= 128) || (NULL == pSRC))
		return nret;				
	if(psn->status == 0)
		return nret;
		
	char out[128];
	int outlen = 128;
	//prepare key
	fp_int key[3];
	memset(&key[0],0,sizeof(key));		
	if(get_auth_ra_key(&key[0],&key[1],&key[2]))
		return nret;
	
	memset(out, 0, 128);
	nret = 	do_rsa_proc(pSRC, nDataLen, out, &outlen, &(key[0]), procedure);		
	if(nret == 0)
		nret = set_sn(out, FROM_KERNEL);
	
	if(nret == 0){
		psn->status = 0;
		memcpy(&(psn->factory_id[0]), out, CONFIG_EFUSE_SN_DATA_SIZE);
	}
	
#ifdef SECURE_DEBUG
	memcpy(pOUT, &(psn->factory_id[0]), CONFIG_EFUSE_SN_DATA_SIZE);
#ifdef SECURE_EFUSE_DEBUG		
	char *pbch = (char*)((unsigned)pOUT + 24);
	efuse_debug_get(151, 26, pbch);
	//efuse_debug_get(151, 26, pOUT);
#endif		
#endif	
	return nret;
}

//-------------------------------------------------------------------------------------
static int secure_cmd_burn_sn_raw(char *pSRC, int nDataLen, char *pOUT,int *pOUTLen)
{
	int ret = -1;
	if((nDataLen!= 24) || (NULL == pSRC))
		return ret;				
	if(psn->status == 0)
		return ret;
	
	char out[128];
	int outlen = 128;
	memset(out, 0, 128);
	memcpy(out, pSRC, 24);	
	ret = set_sn(out, FROM_KERNEL);
	
	if(ret == 0){
		psn->status = 0;
		memcpy(&(psn->factory_id[0]), out, CONFIG_EFUSE_SN_DATA_SIZE);
	}
	
#ifdef SECURE_EFUSE_DEBUG
	efuse_debug_get(151, 26, pOUT);
#endif	
		
		return ret;
}

//-------------------------------------------------------------------------------------
static int secure_cmd_check_in_free()
{	
	if(psn->status == 0)
		return -1;
	else
		return 0;	
}

//-------------------------------------------------------------------------------------
static int secure_cmd_rsa_proc_dec_in(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,int procedure)
{
	int nret = -1;
/*		
	if((nDataLen!= 128) || (*pOUTLen < 127))
		return nret;
	if ( NULL == pSRC || NULL == pOUT )
		return nret;
*/
	if((nDataLen!= 128) || (NULL == pSRC))
		return nret;					
	if(pin->status == 0)
		return nret;
		
	//prepare key
	fp_int key[3];
	memset(&key[0],0,sizeof(key));		
	if(get_auth_ra_key(&key[0],&key[1],&key[2]))
		return nret;
	
	char out[128];
	int outlen = 128;
	memset(out, 0, 128);
	nret = 	do_rsa_proc(pSRC, nDataLen, out, &outlen, &(key[0]), procedure);		
	if(nret == 0)
		nret = set_in(out, FROM_KERNEL);
		
	if(nret == 0){
		pin->status = 0;
		memcpy(&(pin->factory_id[0]), out, CONFIG_EFUSE_IN_DATA_SIZE);
	}

#ifdef SECURE_DEBUG
	memcpy(pOUT, &(pin->factory_id[0]), CONFIG_EFUSE_IN_DATA_SIZE);
#ifdef SECURE_EFUSE_DEBUG
	char *pbch = (char*)(&(pOUT[CONFIG_EFUSE_IN_DATA_SIZE]));
	efuse_debug_get(177, 26, pbch);
#endif		
#endif	
	return nret;
}

//-------------------------------------------------------------------------------------
int static secure_cmd_burn_in_raw(char *pSRC, int nDataLen, char *pOUT,int *pOUTLen)
{
	int ret = -1;
	if((nDataLen!= 24) || (NULL == pSRC))
		return ret;				
	if(pin->status == 0)
		return ret;
	
	char out[128];
	int outlen = 128;
	memset(out, 0, 128);
	memcpy(out, pSRC, 24);	
	ret = set_in(out, FROM_KERNEL);
	
	if(ret == 0){
		pin->status = 0;
		memcpy(&(pin->factory_id[0]), out, CONFIG_EFUSE_IN_DATA_SIZE);
	}
	
#ifdef SECURE_EFUSE_DEBUG
	efuse_debug_get(177, 26, pOUT);
#endif	
		
		return ret;	
}

//-------------------------------------------------------------------------------------
static int secure_cmd_sha_proc_enc_iv(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen) // sn/ci/in/iv(t2/r2) -> HASH -> xx
{
	int nret;
	nret = -1;
	if((nDataLen >= 204)  ||(*pOUTLen < 32))
		return nret;
	if(!pSRC || !pOUT)
		return nret;	
	
	if(psn->status != 0)
		secure_gain_sn();
	if(pci->status != 0)
		secure_gain_ci();
	if(pin->status != 0)
		secure_gain_in();	
	if(psn->status || pci->status || pin->status)
		return nret;
	
	char total[256];	
	char *ivBuffer=pSRC;
	memset(total, 0, 256);
	memset(pOUT, 0, *pOUTLen);
	char *pTemp = NULL;
	pTemp = &(psn->factory_id[0]);
	memcpy((void*)total, (void*)pTemp, 24);
	pTemp=&(pci->data[0]);
	memcpy((void*)total+24, (void*)pTemp, 4);
	pTemp=&(pin->factory_id[0]);
	memcpy((void*)total+24+4, (void*)pTemp, 24);
	memcpy((void*)total+24+4+24, (void*)(ivBuffer + 4), 14);
	memcpy((void*)(total + 24 + 4 + 24 + 14), (void*)(ivBuffer + 4 + 14), 16);
	
	do_comm_sha256sum(total, 82, pOUT);
	*pOUTLen = 32;
	nret = 0;		
	return nret;
}

//----------------------------------------------------------------------------------
static int secure_cmd_sha_proc_common(char *pSRC, int nDataLen, char *pOUT, int *pOUTLen)
{
	int nret;
	nret = -1;
	if(*pOUTLen < 32)
		return nret;
	if(!pSRC || !pOUT)
		return nret;	
	
	do_comm_sha256sum(pSRC, nDataLen, pOUT);
	*pOUTLen = 32;
	nret = 0;		
	return nret;	
}

//----------------------------------------------------------------------------------
static int secure_cmd_burn_mac(char *pSRC, int nDataLen, char *pOUT, int *pOUTLen)
{
	int ret = -1;
	if((nDataLen!= 6) || (NULL == pSRC))
		return ret;
	
	char out[128];
	int outlen = 128;
	memset(out, 0, 128);
	memcpy(out, pSRC, 6);	
	ret = set_mac(out, FROM_KERNEL);
	
#ifdef SECURE_EFUSE_DEBUG
	efuse_debug_get(53, 7, pOUT);
#endif	
	return ret;
}

//----------------------------------------------------------------------------------
static int secure_cmd_gain_mac(char *pSRC, int nDataLen, char *pOUT, int *pOUTLen)
{
	int ret = -1;
	if((*pOUTLen < 6) || (NULL == pOUT))
		return ret;
	
	char buf[128];
	memset(buf, 0, 128);
	ret = get_mac(buf, FROM_KERNEL);
	if(ret == 0){
		memcpy(pOUT, buf, CONFIG_EFUSE_MAC_DATA_SIZE);
	}
	return ret;	
}

//----------------------------------------------------------------------------------
static int secure_cmd_gain_ci(char *pSRC, int nDataLen, char *pOUT, int *pOUTLen)
{
	int ret=-1;
	if(pci->status != 0)
		secure_gain_ci();
	
	if(pci->status == 0){
		if(*pOUTLen >= CONFIG_EFUSE_CI_DATA_SIZE){
			memcpy(pOUT, &(pci->data[0]), CONFIG_EFUSE_CI_DATA_SIZE);
			*pOUTLen = CONFIG_EFUSE_CI_DATA_SIZE;
			ret = 0;
		}
	}	
	return ret;	
}

//====================================================================
// interface APIs
//====================================================================
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
	/*memcpy(n->dp,szTxtN,128);
	memcpy(e->dp,szTxtE,128);
	memcpy(d->dp,szTxtD,128);*/
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

//-------------------------------------------------------------------------------------
static int get_auth_ra_key(fp_int *n, fp_int *e, fp_int *d)
{
	int nRet = 1;
	if(!n || !e || !d)
		return nRet;
	if(pauth_ra->status != 0)
		return nRet;
	
//	unsigned char szTxtN[] = {0xB9,0x14,0xF5,0x2B,0x27,0x16,0x97,0x8A,0x19,0x54,0x8C,0x38,0xB9,0xA5,0xA9,0xA1,0x6C,0x8A,0x3A,0x2A,0x8D,0x1D,0x10,0x44,0x8B,0x53,0x8A,0xA4,0xE4,0x49,0x82,0xA1,0xD5,0x1D,0x3B,0x0A,0x2B,0xA0,0x48,0x0A,0x9D,0xBA,0xF7,0x79,0xC9,0xF2,0x17,0xE2,0x6B,0x32,0x8F,0xC9,0xED,0x13,0xA8,0x38,0xD1,0x27,0x2A,0xBD,0xE1,0xE8,0x8C,0xAF,0x0A,0x15,0x58,0xF7,0xF5,0x8D,0xAA,0x85,0xBB,0xEB,0x51,0x74,0x66,0x8C,0x00,0xB4,0x1E,0x97,0x14,0xD6,0x1B,0x3A,0x3E,0x54,0xED,0x8C,0x82,0xDA,0x3E,0x06,0x68,0xFB,0x51,0x0C,0x72,0xF6,0xE3,0x0F,0x09,0xF2,0xB3,0xFF,0xD7,0xF2,0xF5,0x5D,0x06,0x58,0xFC,0x9D,0xB3,0x53,0x5F,0x45,0x2F,0x22,0x17,0x1E,0x4A,0x4F,0x0A,0xBF,0x04,0x73};
//	unsigned char szTxtE[] = {0x4B,0x37,0x01};
//	unsigned char szTxtD[] = {0xCF,0x8D,0xF9,0xC9,0x57,0x53,0x4D,0xD7,0x36,0xFD,0x27,0x3E,0xA9,0x3A,0x76,0xE8,0xC1,0x50,0x7F,0x95,0x65,0xF8,0x99,0x14,0xD7,0x6C,0x3F,0x1A,0x9D,0x50,0xD3,0x95,0x7C,0xC8,0xFE,0xE3,0x04,0x85,0xF9,0x5A,0xCF,0x48,0x42,0x15,0x83,0x0E,0xE2,0xB6,0xC6,0xC8,0x4F,0xB8,0xA3,0x9B,0xF2,0xAB,0x95,0x20,0xE0,0x0D,0x10,0x16,0x6D,0xFB,0x95,0x38,0x15,0x24,0x82,0x15,0xEC,0x4F,0xF1,0xCF,0xFA,0x76,0x83,0x74,0x8F,0x0B,0xA4,0x79,0x45,0xEB,0x61,0xA8,0xD5,0x26,0x60,0x3D,0x6C,0x8D,0x46,0x9A,0xDE,0xFB,0x5C,0x26,0x77,0x5F,0x2A,0xCD,0x97,0xD6,0x13,0x90,0x72,0xAF,0x4E,0x6F,0x54,0x63,0xFE,0xCB,0xF6,0x69,0x97,0xB1,0x5C,0xA3,0xD6,0x7A,0xDA,0xDF,0x74,0xC9,0x35,0x4F};
	
	fp_init(n);
	fp_init(e);
	fp_init(d);
	/*memcpy(n->dp,szTxtN,sizeof(szTxtN));
	memcpy(e->dp,szTxtE,sizeof(szTxtE));
	memcpy(d->dp,szTxtD,sizeof(szTxtD));*/
	memcpy(n->dp, pauth_ra->n, 128);
	memcpy(e->dp,pauth_ra->e,5);
	memcpy(d->dp, pauth_ra->d, 128);
	
	n->used=FP_SIZE;
	fp_clamp(n);			
	e->used=FP_SIZE;
	fp_clamp(e);			
	d->used=FP_SIZE;
	fp_clamp(d);
	
	nRet = 0;		
	
	return nRet;	
}
//================================================================
// EFUSE area
//================================================================
static int secure_gain_sn(void)  // from EFUSE
{	
	char buf[128];
	char *pTemp=&(psn->factory_id[0]);
	int ret;
	if(!get_sn(buf, FROM_KERNEL)){		
		memcpy(pTemp, buf, CONFIG_EFUSE_SN_DATA_SIZE);
		psn->status = 0;
		ret=0;
	}
	else{
		memset(pTemp, 0, CONFIG_EFUSE_SN_DATA_SIZE);
		psn->status = -1;
		ret = -1;
	}
	return ret;
}

static int secure_gain_in(void)   // form EFUSE
{
	char buf[128];
	char *pTemp = &(pin->factory_id[0]);;	
	int ret;
		
	memset(buf, 0, 128);
	pTemp = &(pin->factory_id[0]);
	if(!get_in(buf, FROM_KERNEL)){		
		memcpy(pTemp, buf, CONFIG_EFUSE_IN_DATA_SIZE);
		pin->status = 0;
		ret = 0;
	}
	else{
		memset(pTemp, 0, CONFIG_EFUSE_IN_DATA_SIZE);
		pin->status = -1;
		ret = -1;
	}
	return ret;
}

static int secure_gain_ci(void)	// from EFUSE
{
	char buf[128];
	char *pTemp= &(pci->data[0]);
	int ret;
	
	memset(buf, 0, 128);
	pTemp = &(pci->data[0]);
	if(!get_ci(buf, FROM_KERNEL)){		
		memcpy(pTemp, buf, CONFIG_EFUSE_CI_DATA_SIZE);
		pci->status = 0;
		ret = 0;
	}
	else{
		memset(pTemp, 0, CONFIG_EFUSE_CI_DATA_SIZE);
		pci->status = -1;
		ret = -1;
	}	
	return ret;
}

