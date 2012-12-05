#include <linux/string.h>
#include "aes.h"
#include "aes_enc.h"
#include "aes_types.h"

//debug show the in/out data
#define AES_LOG_DATA

//AES key table
unsigned char g_AESkey[] = {
0xAD,0x93,0x00,0xC4,0x8E,0x50,0x20,0xC5,0x3F,0xBF,0x23,0x32,0x80,0x5A,0xC6,0xDF,
0x2F,0x7D,0x49,0xD9,0x15,0x8B,0x7F,0x04,0x2C,0x80,0xB0,0x62,0x78,0x25,0x8D,0x9C,
0x13,0x22,0x02,0x4A,0x55,0x23,0xBB,0xCB,0xF1,0xFB,0x2A,0xCC,0xBB,0x95,0xF4,0x50,
0xAE,0x08,0xD7,0xFB,0x80,0xF2,0x64,0x72,0xE3,0x3C,0xC4,0xB4,0xA3,0x50,0xD9,0xF1,
0x2A,0xDE,0xFC,0xD7,0x67,0xC8,0xDE,0xD0,0xF0,0x1E,0xE8,0x12,0xF9,0x57,0x25,0x36,
0x6D,0x71,0xD2,0xF8,0x1E,0x32,0x25,0x59,0x89,0x80,0xA3,0x59,0xD4,0xB6,0xDA,0x00,
0x8D,0xB8,0x5B,0x95,0x96,0x47,0x07,0xBD,0xED,0x68,0xDF,0xB9,0xD5,0x93,0x34,0x8F,
0xC6,0x66,0x06,0x64,0x94,0xCC,0x27,0x29,0x3A,0x8F,0x58,0x2E,0x70,0x7D,0x22,0xE7,
0x9D,0x62,0xAA,0xD1,0x0C,0xD2,0xD7,0x76,0xBD,0x40,0xCD,0x87,0x4E,0xC8,0x4C,0x80,
0x86,0xC2,0xB8,0x97,0xA3,0xDC,0x8F,0x8C,0x45,0xCC,0x26,0x40,0xBD,0xEB,0x3F,0xAF,
0x55,0x1E,0x88,0xFC,0x38,0xC0,0x06,0x1C,0xDA,0xDB,0xE4,0xFA,0x2B,0xFB,0x6D,0x6F,
0x19,0x62,0x0A,0xC4,0xEA,0xF0,0xE3,0x47,0xDB,0x47,0x83,0xE8,0x50,0x17,0xDF,0xA8,
0x29,0x37,0xB4,0x0A,0x19,0x1B,0x2D,0xDB,0x86,0xC8,0xBB,0xD1,0x52,0xD5,0x8F,0xC8,
0x2B,0xBC,0xE7,0x8A,0xF4,0xA1,0xE2,0x4D,0xAC,0xFC,0xB2,0x6F,0xDA,0x82,0xAB,0x86,
0xB7,0x95,0x6B,0xD7,0xA9,0x07,0xC7,0xB8,0x2D,0xBF,0x86,0xB4,0xBF,0xF4,0xC8,0xFD,
0x50,0x43,0xEB,0x8D,0xAB,0x16,0x91,0xBB,0x6B,0x5E,0x60,0x21,0x57,0x44,0x61,0x06,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	AES encrypt/decrypt
//
//	param:
//
//		bEncryptFlag[IN]: flag for encrypt or decrypt. 1: encrypt; 0:decrypt
//		pIN[IN]: input buffer for contain the plain text or the encrypted text
//		nINLen[IN]: buffer length for both pIN and pOUT. AES algorithm not change the data length when encrypt/decrypt.
//		pOUT[OUT]: buffer for contain the output text.
//		pOUTLen[OUT]: data length of output data which the AES processed(unit is byte).
//
//	return: return 0 for success, other value for fail
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int do_aes_internal(unsigned char bEncryptFlag,unsigned char * pIN, int nINLen, unsigned char *pOUT, int * pOUTLen)
{
	int nRet = -1;
	int nAESblkLen=16;
	//check
	if(!pIN || !nINLen || !pOUT || !pOUTLen)
		return nRet;

	//clear memory first?
	memset(pOUT,0,nINLen);

	aes256_ctx_t ctx;
	memset(&ctx,0,sizeof(ctx));
	unsigned char blk_buf[64];
	memcpy(ctx.key,g_AESkey,14*sizeof(aes_roundkey_t));

	int nLoadLen = nAESblkLen;
	*pOUTLen = 0;
	do{
		memset(blk_buf,0,sizeof(blk_buf));
		memcpy(blk_buf,pIN+*pOUTLen,nLoadLen);
		if(bEncryptFlag)
			aes256_enc(blk_buf,&ctx);
		else
			aes256_dec(blk_buf,&ctx);
		memcpy(pOUT+*pOUTLen,blk_buf,nLoadLen);
		*pOUTLen += nLoadLen;
		if(*pOUTLen + nLoadLen > nINLen)
			nLoadLen = nINLen - *pOUTLen;

	}while(*pOUTLen != nINLen);

	nRet = 0;

	return nRet;

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	AES encrypt/decrypt demo code for uboot command format
//
//	param:
//
//		<sub cmd>[IN]: sub command for AES verify. "aes-enc": AES encrypt; "aes-dec": AES decrypt.
//		<in>[IN]: input buffer for contain the text to be AES.(hex format address)
//		<OUT>[OUT]: buffer for contain the output text of AES processed.(hex format address)
//		nINLen[IN]: buffer length for both IN and OUT. AES algorithm not change the data length when encrypt/decrypt.
//
//	return: return 0 for success, other value for fail
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int do_aes_process(int argc,char* argv[])
{
	//aeschk <sub cmd> <in> <out> <len>
	//sub cmd:
	//		aes-enc : encode <in> by aes algorithm
	//		aes-dec : decode <in> by aes algorithm
	//in:
	//    input buffer
	//out:
	//    output buffer
	//len:
	//    legal buffer length for in/out
	//

	int nRet = -1;
	unsigned char bEncryptFlag = 0;

	char * szSubCmd = argv[1];

	//paser sub-cmd for encrypt/decrypt
	if (strcmp(szSubCmd, SUBCMD_AES_ENC) == 0)
		bEncryptFlag = 1;
	else if (strcmp(szSubCmd, SUBCMD_AES_DEC) == 0)
		bEncryptFlag = 0;
	else
		return nRet;

	int nSRC,nDST,nLen;
	char *endp;

	//get source/target buffer and length
	nSRC = simple_strtoul(argv[2], &endp, 16);
	nDST = simple_strtoul(argv[3], &endp, 16);
	nLen = simple_strtoul(argv[4], &endp, 16);

	printf("AES %s: src = 0x%8X dst = 0x%8X len = 0x%X\n\n",(bEncryptFlag? "encrypt":"decrypt"),nSRC,nDST,nLen);

	nLen = ((nLen + 15) >> 4 )<<4;

	unsigned char *pSRC = (unsigned char *)nSRC;		//source buffer
	unsigned char *pDST = (unsigned char *)nDST;		//target buffer
	unsigned char *pCHK = (unsigned char *)nDST + nLen; //check buffer

	int nOutLen = 0;

	//try to encrypt/decrypt according to sub-cmd
	nRet = do_aes_internal( bEncryptFlag, pSRC,nLen,pDST,&nOutLen);

	//dump org and processed data
#ifdef AES_LOG_DATA
	int nIdx;
	printf("ORG data is : \n");
	for(nIdx = 0;nIdx < nLen;++nIdx)
		printf("%02X%s", *(pSRC + nIdx), ((nIdx + 1)%16? " ": "\n"));

	printf("\n%s data is : \n",(bEncryptFlag? "Encrypted":"Decrypted"));
	for(nIdx = 0;nIdx < nLen;++nIdx)
		printf("%02X%s", *(pDST + nIdx), ((nIdx + 1)%16? " ": "\n"));

	printf("\n\nTry to verify : %s \n",(bEncryptFlag? "decrypt":"encrypt"));
#endif //#ifdef AES_LOG_DATA

	//try to verify according to sub-cmd
	nRet = do_aes_internal( !bEncryptFlag, pDST,nLen,pCHK,&nOutLen);

	//dump the data of verify
#ifdef AES_LOG_DATA
	printf("%s data is : \n",(bEncryptFlag? "Decrypted":"Encrypted"));
	for(nIdx = 0;nIdx < nLen;++nIdx)
		printf("%02X%s", *(pCHK + nIdx), ((nIdx + 1)%16? " ": "\n"));
#endif //#ifdef AES_LOG_DATA

	//AES result
	if(memcmp(pSRC,pCHK,nLen))
		printf("\n\nERROR! ---- AES enc-dec fail!\n\n");
	else
		printf("\n\nSUCCESS! ---- AES enc-dec pass!\n\n");


	return nRet;
}

