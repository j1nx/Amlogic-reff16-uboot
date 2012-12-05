/***************************************************************************************************************
 * RSA-ENC-DEC workable code.
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

static int aml_buffer_proc_before(char *pBuffer,int nDataLen, int nBufferLen,int procedure);
static int aml_buffer_proc_after(char *pBuffer,int * pnDataLen, int nBufferLen,int procedure);

int do_rsa_proc(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,fp_int *key, int procedure)
{
	// fp_int *key--> key[0], key[1], key[2]	
	int in_size = nDataLen;
	int nret=-1;
	int nlen;		
	fp_int c,m;
	fp_int* k,* t;	
	
	unsigned char *pSRCBuff = (unsigned char *)pSRC;
	unsigned char *pOUTBuff = (unsigned char *)pOUT;	
	memset(pOUTBuff,0,*pOUTLen);
	*pOUTLen = 0;
	
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
	default: 
		//printf("Internal error! Not support this enc-dec mode %d\n",procedure);
		//dbg_print("Internal error! Not support this enc-dec mode ", procedure);
		return nret; break;
	}
		
	//buffer process
	if((procedure & RSA_BUF_IN_TYPE_MASK) != RSA_BUF_IN_TYPE_IGNORE)
		aml_buffer_proc_before(pSRCBuff,nDataLen,i_len,procedure);
	
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
	if((procedure & RSA_BUF_OUT_TYPE_MASK) != RSA_BUF_OUT_TYPE_IGNORE)
		aml_buffer_proc_after(pOUTBuff,pOUTLen,o_len,procedure);	
	
	nret = 0;	
	return nret;		
}


//=======================================================================================
// MISC
//=======================================================================================
static int aml_buffer_proc_before(char *pBuffer,int nDataLen, int nBufferLen,int procedure)
{
	int nRet = -1;
	int i;
	if(!pBuffer || !nBufferLen)		
		return nRet;
	if(nDataLen > nBufferLen)
		return nRet;	
	
	unsigned char *pSRC = (unsigned char *)pBuffer;
	unsigned char szTemp[RSA_1024_BLK_SIZE_MAX];
	memset(szTemp,0,sizeof(szTemp));

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
	return nRet;
}

static int aml_buffer_proc_after(char *pBuffer,int * pnDataLen, int nBufferLen,int procedure)
{
	int nRet = -1;
	int i;
	if(!pBuffer || !nBufferLen)		
		return nRet;
		
	if(RSA_1024_BLK_SIZE_MAX < nBufferLen )
	{
		/*printf("Error! Input buffer length can not exceed %d!\n",
		RSA_1024_BLK_SIZE_MAX);*/
		//dbg_print("Error! Input buffer length can not exceed ", RSA_1024_BLK_SIZE_MAX);
		return nRet;
	}
	if(*pnDataLen > nBufferLen)
		return nRet;
			
	unsigned char *pSRC = (unsigned char *)pBuffer;
	unsigned char szTemp[RSA_1024_BLK_SIZE_MAX]; //128 bytes
	memset(szTemp,PC_BYTE_ARR_PAD_VAL,sizeof(szTemp));

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
	return nRet;
}

