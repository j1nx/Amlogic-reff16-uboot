/***************************************************************************************************************
 * Secure Domain runtime APIs
 *
 * obtain SN/IN/CI/RSA HASH from EFUSE 
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

int get_auth_ra_hash(char *pOut, int from);


int get_auth_ra_hash(char *pOut, int from)   
{
	unsigned pos=CONFIG_EFUSE_AUTH_HASH_OFFSET;
	unsigned datasize = CONFIG_EFUSE_AUTH_HASH_DATA_SIZE;
	unsigned bchstep = CONFIG_EFUSE_AUTH_HASH_BCH_STEP;	
	unsigned encsize;
	unsigned char encbuf[128];
	unsigned char decbuf[128];
	
//#ifdef CONFIG_BCH_PACK0 // pack 0 in last bchstep
	encsize = datasize + ((datasize+bchstep-1)/bchstep);
//#else
//	encsize = ((datasize+bchstep-1)/bchstep)*(bchstep+1);
//#endif	
	memset(encbuf, 0, 128);
	
//#ifndef SECURE_KERNEL_DEBUG	
	efuse_read(encbuf, encsize, &pos, from);
//#else
//const unsigned char auth_bch[]={
//	0x54, 0x55, 0x79, 0x69, 0xBE, 0x43, 0x65, 0x41, 0xD2, 0xE6, 0xCA, 0x4B, 0x66, 0x11, 0x7F, 0x82, 0x83, 
//	0x2B, 0x25, 0x0D, 0x5B, 0xDF, 0x30, 0xFC, 0xAA, 0x4F, 0xD6, 0x18, 0x72, 0x8A, 0x58, 0x64, 0xA9, 0xB2, 
//};
//	memcpy(encbuf, (char*)auth_bch, encsize);
//#endif	
	
	memset(decbuf, 0, 128);
	unsigned char* penc=encbuf;
	unsigned char* pdata = decbuf;
	unsigned enc_len=encsize;
	while(enc_len>=(bchstep+1)){
		efuse_bch_dec(penc, bchstep+1, pdata, 0);
		penc += (bchstep+1);
		pdata += bchstep;
		enc_len -= (bchstep+1);
	}
	if(enc_len > 0)
		efuse_bch_dec(penc, enc_len, pdata, 0);
	
	memcpy(pOut, decbuf, CONFIG_EFUSE_AUTH_HASH_DATA_SIZE);	
	
	return 0;
}

 
int get_sn(char *pOut, int from)
{
	unsigned pos=CONFIG_EFUSE_SN_OFFSET;
	unsigned datasize = CONFIG_EFUSE_SN_DATA_SIZE;
	unsigned bchstep = CONFIG_EFUSE_SN_BCH_STEP;	
	unsigned encsize;
	unsigned char encbuf[128];
	unsigned char decbuf[128];
		
//#ifdef CONFIG_BCH_PACK0 // pack 0 in last bchstep
	encsize = datasize + ((datasize+bchstep-1)/bchstep);
//#else
//	encsize = ((datasize+bchstep-1)/bchstep)*(bchstep+1);
//#endif	
	memset(encbuf, 0, 128);
	
//#ifndef SECURE_DEBUG	
	efuse_read(encbuf, encsize, &pos, from);
//#else
	//unsigned char szSN[] = "MRVLCTCC20120712SN000001";
//	unsigned char debugenc[] = {
//		0x4D, 0x52, 0x56, 0x4C, 0x43, 0x54, 0x43, 0x43, 0x32, 0x30, 0x31, 0x32, 0x14, 
//		0x30, 0x37, 0x31, 0x32, 0x53, 0x4E, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x8D, 
//	};
//	memcpy(encbuf, (char*)debugenc, encsize);
//#endif	
	
	memset(decbuf, 0, 128);
	unsigned char* penc=encbuf;
	unsigned char* pdata = decbuf;
	unsigned enc_len=encsize;
	while(enc_len>=(bchstep+1)){
		efuse_bch_dec(penc, bchstep+1, pdata, 0);
		penc += (bchstep+1);
		pdata += bchstep;
		enc_len -= (bchstep+1);
	}
	if(enc_len > 0)
		efuse_bch_dec(penc, enc_len, pdata, 0);
			
	memcpy(pOut, decbuf, datasize);	
		
	int i;
	for(i=0; i<datasize; i++){
		if(pOut[i])
			break;
	}
	if(i >= datasize)
		return -1;
	else	
		return 0;	
}

int get_in(char *pOut, int from)
{
	unsigned pos=CONFIG_EFUSE_IN_OFFSET;
	unsigned datasize = CONFIG_EFUSE_IN_DATA_SIZE;
	unsigned bchstep = CONFIG_EFUSE_IN_BCH_STEP;	
	unsigned encsize;
	unsigned char encbuf[128];
	unsigned char decbuf[128];
		
//#ifdef CONFIG_BCH_PACK0 // pack 0 in last bchstep
	encsize = datasize + ((datasize+bchstep-1)/bchstep);
//#else
//	encsize = ((datasize+bchstep-1)/bchstep)*(bchstep+1);
//#endif	
	memset(encbuf, 0, 128);	
	efuse_read(encbuf, encsize, &pos, from);	
	
	memset(decbuf, 0, 128);
	unsigned char* penc=encbuf;
	unsigned char* pdata = decbuf;
	unsigned enc_len=encsize;
	while(enc_len>=(bchstep+1)){
		efuse_bch_dec(penc, bchstep+1, pdata, 0);
		penc += (bchstep+1);
		pdata += bchstep;
		enc_len -= (bchstep+1);
	}
	if(enc_len > 0)
		efuse_bch_dec(penc, enc_len, pdata, 0);
			
	memcpy(pOut, decbuf, datasize);	
		
	int i;
	for(i=0; i<datasize; i++){
		if(pOut[i])
			break;
	}
	if(i >= datasize)
		return -1;
	else	
		return 0;	
}

int get_ci(char *pOut, int from)
{
	unsigned pos=CONFIG_EFUSE_CI_OFFSET;
	unsigned datasize = CONFIG_EFUSE_CI_DATA_SIZE;
	unsigned bchstep = CONFIG_EFUSE_CI_BCH_STEP;	
	unsigned encsize;
	unsigned char encbuf[128];
	unsigned char decbuf[128];
		
//#ifdef CONFIG_BCH_PACK0 // pack 0 in last bchstep
	encsize = datasize + ((datasize+bchstep-1)/bchstep);
//#else
//	encsize = ((datasize+bchstep-1)/bchstep)*(bchstep+1);
//#endif	
	memset(encbuf, 0, 128);
	efuse_read(encbuf, encsize, &pos, from);
	
	memset(decbuf, 0, 128);
	unsigned char* penc=encbuf;
	unsigned char* pdata = decbuf;
	unsigned enc_len=encsize;
	while(enc_len>=(bchstep+1)){
		efuse_bch_dec(penc, bchstep+1, pdata, 0);
		penc += (bchstep+1);
		pdata += bchstep;
		enc_len -= (bchstep+1);
	}
	if(enc_len > 0)
		efuse_bch_dec(penc, enc_len, pdata, 0);
			
	memcpy(pOut, decbuf, datasize);	
		
	int i;
	for(i=0; i<datasize; i++){
		if(pOut[i])
			break;
	}
	if(i >= datasize)
		return -1;
	else	
		return 0;	
}

static int check_version(int from)
{
	unsigned pos=CONFIG_EFUSE_VERSION_OFFSET;
	unsigned datasize = CONFIG_EFUSE_VERSION_DATA_SIZE;
	unsigned bchstep = CONFIG_EFUSE_VERSION_BCH_STEP;	
	unsigned encsize;
	unsigned char encbuf[128];
	unsigned char decbuf[128];
		
//#ifdef CONFIG_BCH_PACK0 // pack 0 in last bchstep
	encsize = datasize + ((datasize+bchstep-1)/bchstep);
//#else
//	encsize = ((datasize+bchstep-1)/bchstep)*(bchstep+1);
//#endif	
	memset(encbuf, 0, 128);
	efuse_read(encbuf, encsize, &pos, from);
	
	memset(decbuf, 0, 128);
	unsigned char* penc=encbuf;
	unsigned char* pdata = decbuf;
	unsigned enc_len=encsize;
	while(enc_len>=(bchstep+1)){
		efuse_bch_dec(penc, bchstep+1, pdata, 0);
		penc += (bchstep+1);
		pdata += bchstep;
		enc_len -= (bchstep+1);
	}
	if(enc_len > 0)
		efuse_bch_dec(penc, enc_len, pdata, 0);
			
	//memcpy(pOut, decbuf, datasize);	
	if(decbuf[0] != 3)
		return -1;
	else
		return 0;
}

int get_mac(char *pOut, int from)
{
	if(check_version(from) < 0)
		return -1;
			
	unsigned pos=CONFIG_EFUSE_MAC_OFFSET;
	unsigned datasize = CONFIG_EFUSE_MAC_DATA_SIZE;
	unsigned bchstep = CONFIG_EFUSE_MAC_BCH_STEP;	
	unsigned encsize;
	unsigned char encbuf[128];
	unsigned char decbuf[128];		
		
//#ifdef CONFIG_BCH_PACK0 // pack 0 in last bchstep
	encsize = datasize + ((datasize+bchstep-1)/bchstep);
//#else
//	encsize = ((datasize+bchstep-1)/bchstep)*(bchstep+1);
//#endif	
	memset(encbuf, 0, 128);
	efuse_read(encbuf, encsize, &pos, from);
	
	memset(decbuf, 0, 128);
	unsigned char* penc=encbuf;
	unsigned char* pdata = decbuf;
	unsigned enc_len=encsize;
	while(enc_len>=(bchstep+1)){
		efuse_bch_dec(penc, bchstep+1, pdata, 0);
		penc += (bchstep+1);
		pdata += bchstep;
		enc_len -= (bchstep+1);
	}
	if(enc_len > 0)
		efuse_bch_dec(penc, enc_len, pdata, 0);
			
	memcpy(pOut, decbuf, datasize);	
		
	int i;
	for(i=0; i<datasize; i++){
		if(pOut[i])
			break;
	}
	if(i >= datasize)
		return -1;
	else	
		return 0;		
}

int set_sn(char *pSrc, int from)
{
	unsigned pos = CONFIG_EFUSE_SN_OFFSET;
	unsigned datasize = CONFIG_EFUSE_SN_DATA_SIZE;
	unsigned bchstep = CONFIG_EFUSE_SN_BCH_STEP;
	unsigned encsize;
	unsigned char encbuf[128];
	unsigned char decbuf[128];
	
//#ifdef CONFIG_BCH_PACK0 // pack 0 in last bchstep
	encsize = datasize + ((datasize+bchstep-1)/bchstep);
//#else
//	encsize = ((datasize+bchstep-1)/bchstep)*(bchstep+1);
//#endif		
	
	memset(decbuf, 0, 128);
	memcpy(decbuf, pSrc, datasize);
	memset(encbuf, 0, 128);
	
	unsigned char *penc = encbuf;
	unsigned char *pdata = decbuf;
	unsigned data_len = datasize;
	while(data_len >= bchstep){
		efuse_bch_enc(pdata, bchstep, penc, 0);
		pdata += bchstep;
		penc += (bchstep+1);
		data_len -= bchstep;
	}
	if(data_len > 0){
//#ifdef CONFIG_BCH_PACK0
//		efuse_bch_enc(pdata, bchstep, penc, 0);
//#else		
		efuse_bch_enc(pdata, data_len, penc, 0);
//#endif		
	}
	
	int ret;
	ret = efuse_write(encbuf, encsize, &pos, from);
	return ret;	
}

int set_in(char *pSrc, int from)
{
	unsigned pos = CONFIG_EFUSE_IN_OFFSET;
	unsigned datasize = CONFIG_EFUSE_IN_DATA_SIZE;
	unsigned bchstep = CONFIG_EFUSE_IN_BCH_STEP;
	unsigned encsize;
	unsigned char encbuf[128];
	unsigned char decbuf[128];
	
//#ifdef CONFIG_BCH_PACK0 // pack 0 in last bchstep
	encsize = datasize + ((datasize+bchstep-1)/bchstep);
//#else
//	encsize = ((datasize+bchstep-1)/bchstep)*(bchstep+1);
//#endif		
	
	memset(decbuf, 0, 128);
	memcpy(decbuf, pSrc, datasize);
	memset(encbuf, 0, 128);
	
	unsigned char *penc = encbuf;
	unsigned char *pdata = decbuf;
	unsigned data_len = datasize;
	while(data_len >= bchstep){
		efuse_bch_enc(pdata, bchstep, penc, 0);
		pdata += bchstep;
		penc += (bchstep+1);
		data_len -= bchstep;
	}
	if(data_len > 0){
//#ifdef CONFIG_BCH_PACK0
//		efuse_bch_enc(pdata, bchstep, penc, 0);
//#else		
		efuse_bch_enc(pdata, data_len, penc, 0);
//#endif		
	}
	
	int ret;
	ret = efuse_write(encbuf, encsize, &pos, from);
	return ret;	
}

int set_mac(char *pSrc, int from)
{
	if(check_version(from) < 0)
		return -1;
		
	unsigned pos = CONFIG_EFUSE_MAC_OFFSET;
	unsigned datasize = CONFIG_EFUSE_MAC_DATA_SIZE;
	unsigned bchstep = CONFIG_EFUSE_MAC_BCH_STEP;
	unsigned encsize;
	unsigned char encbuf[128];
	unsigned char decbuf[128];
	
//#ifdef CONFIG_BCH_PACK0 // pack 0 in last bchstep
	encsize = datasize + ((datasize+bchstep-1)/bchstep);
//#else
//	encsize = ((datasize+bchstep-1)/bchstep)*(bchstep+1);
//#endif		
	
	memset(decbuf, 0, 128);
	memcpy(decbuf, pSrc, datasize);
	memset(encbuf, 0, 128);
	
	unsigned char *penc = encbuf;
	unsigned char *pdata = decbuf;
	unsigned data_len = datasize;
	while(data_len >= bchstep){
		efuse_bch_enc(pdata, bchstep, penc, 0);
		pdata += bchstep;
		penc += (bchstep+1);
		data_len -= bchstep;
	}
	if(data_len > 0){
//#ifdef CONFIG_BCH_PACK0
//		efuse_bch_enc(pdata, bchstep, penc, 0);
//#else		
		efuse_bch_enc(pdata, data_len, penc, 0);
//#endif		
	}
	
	int ret;
	ret = efuse_write(encbuf, encsize, &pos, from);
	return ret;	
}