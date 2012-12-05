/***************************************************************************************************************
 * RSA-ENC-DEC workable code.
 *
 * Author: Haixiang.bao <haixiang.bao@amlogic.com>
 *
 * Copyright (c) 2012 Amlogic Inc.
 * 
 * Create date: 2012.07.22
 *
 **************************************************************************************************************/
// #include <stdio.h>
// #include <stdlib.h>
#include <linux/string.h>
#include "apis.h"
#include "romboot.h"
#include "softcrypto.h"
#include "tfm.h"
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/time.h>

//RSA key : psudo code for demo
//0 is success, 1 for fail
static int get_rsa_key(fp_int *n,fp_int *e,fp_int*d)
{
	//here the long number is little
	//to replace N,E,D...
	unsigned char szTxtN[] = {0xB9,0x14,0xF5,0x2B,0x27,0x16,0x97,0x8A,0x19,0x54,0x8C,0x38,0xB9,0xA5,0xA9,0xA1,0x6C,0x8A,0x3A,0x2A,0x8D,0x1D,0x10,0x44,0x8B,0x53,0x8A,0xA4,0xE4,0x49,0x82,0xA1,0xD5,0x1D,0x3B,0x0A,0x2B,0xA0,0x48,0x0A,0x9D,0xBA,0xF7,0x79,0xC9,0xF2,0x17,0xE2,0x6B,0x32,0x8F,0xC9,0xED,0x13,0xA8,0x38,0xD1,0x27,0x2A,0xBD,0xE1,0xE8,0x8C,0xAF,0x0A,0x15,0x58,0xF7,0xF5,0x8D,0xAA,0x85,0xBB,0xEB,0x51,0x74,0x66,0x8C,0x00,0xB4,0x1E,0x97,0x14,0xD6,0x1B,0x3A,0x3E,0x54,0xED,0x8C,0x82,0xDA,0x3E,0x06,0x68,0xFB,0x51,0x0C,0x72,0xF6,0xE3,0x0F,0x09,0xF2,0xB3,0xFF,0xD7,0xF2,0xF5,0x5D,0x06,0x58,0xFC,0x9D,0xB3,0x53,0x5F,0x45,0x2F,0x22,0x17,0x1E,0x4A,0x4F,0x0A,0xBF,0x04,0x73};
	unsigned char szTxtE[] = {0x4B,0x37,0x01};
	unsigned char szTxtD[] = {0xCF,0x8D,0xF9,0xC9,0x57,0x53,0x4D,0xD7,0x36,0xFD,0x27,0x3E,0xA9,0x3A,0x76,0xE8,0xC1,0x50,0x7F,0x95,0x65,0xF8,0x99,0x14,0xD7,0x6C,0x3F,0x1A,0x9D,0x50,0xD3,0x95,0x7C,0xC8,0xFE,0xE3,0x04,0x85,0xF9,0x5A,0xCF,0x48,0x42,0x15,0x83,0x0E,0xE2,0xB6,0xC6,0xC8,0x4F,0xB8,0xA3,0x9B,0xF2,0xAB,0x95,0x20,0xE0,0x0D,0x10,0x16,0x6D,0xFB,0x95,0x38,0x15,0x24,0x82,0x15,0xEC,0x4F,0xF1,0xCF,0xFA,0x76,0x83,0x74,0x8F,0x0B,0xA4,0x79,0x45,0xEB,0x61,0xA8,0xD5,0x26,0x60,0x3D,0x6C,0x8D,0x46,0x9A,0xDE,0xFB,0x5C,0x26,0x77,0x5F,0x2A,0xCD,0x97,0xD6,0x13,0x90,0x72,0xAF,0x4E,0x6F,0x54,0x63,0xFE,0xCB,0xF6,0x69,0x97,0xB1,0x5C,0xA3,0xD6,0x7A,0xDA,0xDF,0x74,0xC9,0x35,0x4F};

        void* n_address = (void*)(0x49000000 + 16 * 1024 + 384 + 256);
        void* e_address = n_address + 128;

        memcpy((void*)szTxtN, n_address, 128);
        memcpy((void*)szTxtE, e_address, 3);
	
	int nRet = 1;
	if(!n || !e || !d)
		return nRet;
		
	fp_init(n);
	fp_init(e);
	fp_init(d);
	memcpy(n->dp,szTxtN,sizeof(szTxtN));
	memcpy(e->dp,szTxtE,sizeof(szTxtE));
	memcpy(d->dp,szTxtD,sizeof(szTxtD));
	n->used=FP_SIZE;
	fp_clamp(n);			
	e->used=FP_SIZE;
	fp_clamp(e);			
	d->used=FP_SIZE;
	fp_clamp(d);
	
	nRet = 0;		
	
	return nRet;	
}

//JAVA_BUFF_DUMP is for dump buffer to compare bewteen big & little
//#define JAVA_BUFF_DUMP

#define RSA_1024_ENC_BLK_SIZE_MAX (127)
#define RSA_1024_BLK_SIZE_MAX     (128)
#define JAVA_BYTE_ARR_SIZE_MAX    (117)
#define JAVA_BYTE_ARR_PAD_VAL     (0xFF)
#define JAVA_BYTE_ARR_PAD_ID_1    (1)
#define JAVA_BYTE_ARR_PAD_ID_0    (0)
#define PC_BYTE_ARR_PAD_VAL       (0)

static int aml_buffer_proc_before(char *pBuffer,int nDataLen, int nBufferLen,int procedure)
{
	int nRet = -1;
	if(!pBuffer || !nBufferLen)		
		return nRet;
	
	int i = 0;
	unsigned char *pSRC = (unsigned char *)pBuffer;
	unsigned char szTemp[RSA_1024_BLK_SIZE_MAX];
	memset(szTemp,0,sizeof(szTemp));

#ifdef JAVA_BUFF_DUMP	
	printf("ORG-DATA before convert :\n");
	for(i = 0;i< nDataLen;++i)
	printf( "%02X",pSRC[i]);	
	printf("\n");
 
#endif
	
	switch(procedure & (RSA_ENC_DEC_MASK))
	{
	case ENC_WITH_PUB:
	case ENC_WITH_PRIV:{
			switch(procedure & RSA_BUF_IN_TYPE_MASK)
			{
			case RSA_BUF_IN_TYPE_LITTLE:{
				//padding with 0xff, little to big
				memset(szTemp,JAVA_BYTE_ARR_PAD_VAL,sizeof(szTemp));
				szTemp[RSA_1024_ENC_BLK_SIZE_MAX-1]= JAVA_BYTE_ARR_PAD_ID_1;
				szTemp[nDataLen]= JAVA_BYTE_ARR_PAD_ID_0;
				for(i = 0;i< nDataLen;++i)
					szTemp[i] = pBuffer[nDataLen-1-i];	
				memcpy(pSRC,szTemp,	nBufferLen);			
				}break;
			case RSA_BUF_IN_TYPE_BIG:{
				memset(pSRC+nDataLen,JAVA_BYTE_ARR_PAD_VAL,(RSA_1024_ENC_BLK_SIZE_MAX-nDataLen));
				pSRC[RSA_1024_ENC_BLK_SIZE_MAX-1]= JAVA_BYTE_ARR_PAD_ID_1;
				pSRC[nDataLen]= JAVA_BYTE_ARR_PAD_ID_0;								
			}break;
			}
		}break;
	case DEC_WITH_PRIV:
	case DEC_WITH_PUB:{		
			switch(procedure & RSA_BUF_IN_TYPE_MASK)
			{
			case RSA_BUF_IN_TYPE_LITTLE:{		
				}break;
			case RSA_BUF_IN_TYPE_BIG:{
				for(i = 0;i<nDataLen;++i)
					szTemp[i] = pSRC[nDataLen-1-i];	
				memcpy(pSRC,szTemp,	nBufferLen);
				}break;		
			}break;						
		}break;
		default: break;		
	}
		
	nRet = 0;

#ifdef JAVA_BUFF_DUMP	
	printf("ORG-DATA after convert :\n");
	for(i = 0;i< nBufferLen;++i)
	printf( "%02X",pSRC[i]);	
	printf("\n"); 
#endif

	return nRet;
}

static int aml_buffer_proc_after(char *pBuffer,int * pnDataLen, int nBufferLen,int procedure)
{
	int i = 0;
	int nRet = -1;
	if(!pBuffer || !nBufferLen)		
		return nRet;
		
	if(RSA_1024_BLK_SIZE_MAX < nBufferLen )
	{
		printf("Error! Input buffer length can not exceed %d!\n",
		RSA_1024_BLK_SIZE_MAX);
		return nRet;
	}
			
	unsigned char *pSRC = (unsigned char *)pBuffer;
	unsigned char szTemp[RSA_1024_BLK_SIZE_MAX]; //128 bytes
	memset(szTemp,PC_BYTE_ARR_PAD_VAL,sizeof(szTemp));

#ifdef JAVA_BUFF_DUMP
	printf("OUT-DATA before convert :\n");
	for(i = 0;i< * pnDataLen;++i)
	printf( "%02X",pSRC[i]);
	printf("\n");
#endif

	switch(procedure & (RSA_ENC_DEC_MASK))
	{
	case ENC_WITH_PUB:
	case ENC_WITH_PRIV:{
		switch(procedure & RSA_BUF_OUT_TYPE_MASK)
		{
		case RSA_BUF_OUT_TYPE_LITTLE:{}break;
		case RSA_BUF_OUT_TYPE_BIG:{
			for(i = 0;i<RSA_1024_BLK_SIZE_MAX;++i)
				szTemp[i] = pSRC[RSA_1024_BLK_SIZE_MAX-1-i];	
			memcpy(pSRC,szTemp,	nBufferLen);
			}break;
		}
	}break;
	case DEC_WITH_PRIV:
	case DEC_WITH_PUB:{
		switch(procedure & RSA_BUF_OUT_TYPE_MASK)
		{
		case RSA_BUF_OUT_TYPE_LITTLE:{					
			*pnDataLen -= 2;				
			for(;JAVA_BYTE_ARR_PAD_VAL == pSRC[* pnDataLen];--(*pnDataLen));			
			for(i = 0;i<*pnDataLen;++i)
				szTemp[i] = pSRC[*pnDataLen-i-1];
			memset(pSRC,PC_BYTE_ARR_PAD_VAL,nBufferLen);
			memcpy(pSRC,szTemp,	*pnDataLen);	
			}break;
		case RSA_BUF_OUT_TYPE_BIG:{
			*pnDataLen -= 2;				
			for(;JAVA_BYTE_ARR_PAD_VAL == pSRC[* pnDataLen];--(*pnDataLen));			
			memset(pSRC+* pnDataLen,PC_BYTE_ARR_PAD_VAL,(nBufferLen - * pnDataLen));				
			}break;		
		}break;
	}break;
	default: break;		
	}
						
	nRet = 0;
	
#ifdef JAVA_BUFF_DUMP	
	printf("OUT-DATA after convert :\n");
	for(i = 0;i< nBufferLen;++i)
	printf( "%02X",pSRC[i]);	
	printf("\n"); 
#endif

	return nRet;
}

int do_rsa_enc_dec(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,int procedure)
{	
	int in_size = nDataLen;
	int nret = 1;
	
	if ( NULL == pSRC || NULL == pOUT )
		return nret;
		
	//check nSRCLen for dec & enc
	//...	
	
	unsigned char *pSRCBuff = (unsigned char *)pSRC;
	unsigned char *pOUTBuff = (unsigned char *)pOUT;
	
	//clear output buffer??
	memset(pOUTBuff,0,*pOUTLen);
	*pOUTLen = 0;
	
	//prepare key
	fp_int key[3];
	memset(&key[0],0,sizeof(key));	
	//get key from efuse
	if(get_rsa_key(&key[0],&key[1],&key[2]))
		return nret;
	
	int nlen;		
	fp_int c,m;
	fp_int* k,* t;
	
	//get key length
	//for RSA1024 : 1028bit -> 128 bytes
	//dec-i_len : 128 dec-o_len:127
	//enc-i_len : 127 dec-o_len:128
	uint32_t bits=fp_count_bits(&key[0]);
	uint32_t o_len,i_len;

	//dec
	i_len=(bits+7)>>3;
	o_len=i_len-1;
	
	//pub key
	k=&key[1];

	switch(procedure & RSA_ENC_DEC_MASK)
	{
	case ENC_WITH_PRIV:o_len=i_len;--i_len;;k=&key[2];break;
	case DEC_WITH_PUB:break;
	case ENC_WITH_PUB:o_len=i_len;--i_len;break;
	case DEC_WITH_PRIV:k=&key[2];break;
	default: printf("Internal error! Not support this enc-dec mode %d\n",procedure);return nret; break;
	}

	//buffer process
	// aml_buffer_proc_before(pSRCBuff,nDataLen,i_len,procedure);

	for(nlen=0;nlen<in_size;nlen+=i_len)
	{		
		fp_init(&c);
		fp_init(&m);		
		memcpy(c.dp,pSRCBuff,i_len);
		pSRCBuff += i_len;
		c.used=FP_SIZE;
		fp_clamp(&c);		
		fp_exptmod(&c,k,&key[0],&m);			
		memcpy(pOUTBuff+*pOUTLen,m.dp,o_len);
		*pOUTLen += o_len;		
	}
	
	//buffer process
	// aml_buffer_proc_after(pOUTBuff,pOUTLen,o_len,procedure);
	
	
	nret = 0;
	
	return nret;
}
