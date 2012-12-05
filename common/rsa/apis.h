#ifndef __APIS_H__
#define __APIS_H__


/**********************************************************************************/
//readable command for function test
//rsa enc-dec sub command
#define SUBCMD_RSA_ENC_PUB   "rsa-enc-pub"
#define SUBCMD_RSA_DEC_PRV   "rsa-dec-prv"
#define SUBCMD_RSA_ENC_PRV   "rsa-enc-prv"
#define SUBCMD_RSA_DEC_PUB   "rsa-dec-pub"
//source/output file storage mode
#define RSA_BUFFER_TYPE_LITTLE  "buffer-little"
#define RSA_BUFFER_TYPE_BIG     "buffer-big"
/**********************************************************************************/



/**********************************************************************************/
//
//function: do_rsa_enc_dec()
//
//
/* return 0:ok 1:fail  */
//pSRC : buffer for process
//       enc : length >= 127
//       dec : length >= 128
//nDataLen : valid data length in pSRC
//       enc : nDataLen <= 117
//       dec : nDataLen = 128
//pOUT : buffer to contain the result
//       enc : length >= 128
//       dec : length >= 127
//pOUTLen : valid data length in pOUT
//procedure : rsa option
//			ENC_WITH_PUB  : encode with public key
//			ENC_WITH_PRIV : encode with private key
//			DEC_WITH_PRIV : decode with private key
//			DEC_WITH_PUB  : decode with public key
//			RSA_BUF_IN_TYPE_LITTLE : source file is little endian 
//			RSA_BUF_IN_TYPE_BIG : source file is big endian 
//			RSA_BUF_OUT_TYPE_LITTLE : output file is little endian 
//			RSA_BUF_OUT_TYPE_BIG : source file is big endian 

int do_rsa_enc_dec(char *pSRC, int nDataLen,char *pOUT,int *pOUTLen,int procedure);

#define RSA_ENC_DEC_MASK (0x0F)
#define ENC_WITH_PUB	 (0)
#define ENC_WITH_PRIV	 (1)
#define DEC_WITH_PRIV  	 (3)
#define DEC_WITH_PUB   	 (2)

#define RSA_BUF_IN_TYPE_MASK    (0x30)
#define RSA_BUF_IN_TYPE_LITTLE  (0x00)
#define RSA_BUF_IN_TYPE_BIG     (0x10)
#define RSA_BUF_OUT_TYPE_MASK    (0xC0)
#define RSA_BUF_OUT_TYPE_LITTLE  (0x00)
#define RSA_BUF_OUT_TYPE_BIG     (0x80)

/**********************************************************************************/


#endif /*__APIS_H__*/
