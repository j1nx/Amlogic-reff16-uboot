#ifndef _SOFT_CRYPTO_H_
#define _SOFT_CRYPTO_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define SOFT_ENC	0	/* MODE == encrypt */
#define SOFT_DEC	1	/* MODE == decrypt */

#define    SOFT_ECB        1    /*  Are we inputLening in ECB mode?   */
#define    SOFT_CBC        2    /*  Are we inputLening in CBC mode?   */
#define    SOFT_CFB1       3

#define            KEY_LEN_ERROR                           1001
#define            OTHER_ERROR                             1002
#define            DATA_LEN_ERROR                          1003

/* UINT2 defines a two byte word */
#define	UINT2		unsigned short int

/* UINT4 defines a four byte word */
#define	UINT4		unsigned long int
#define POINTER		unsigned char *
#define PROTO_LIST(list) list
#define size_t  unsigned int

//在win系统中，long为4个字节，long long 为8个字节。
//其他系统，我没有测试过，请测试后修改宏
typedef unsigned int SilcUInt32;
typedef signed int SilcInt32;

#ifndef WIN32
	typedef unsigned long long SilcUInt64;
	typedef signed long long SilcInt64;
#else
	typedef unsigned __int64 SilcUInt64;
	typedef signed __int64 SilcInt64;
#endif

typedef struct _DES_CTX{
	unsigned long subkeys[32];              /* subkeys */
	unsigned long iv[2];                    /* initializing vector */
	int encflag;                            /* encrypt flag */
	int  mode;                             /* ECB or CBC */
}DES_CTX, *PDES_CTX;

typedef struct _DES3_CTX{
	unsigned long subkeys[3][32];              /* subkeys */
	unsigned long iv[2];                    /* initializing vector */
	int encflag;                            /* encrypt flag */
	int mode;                             /* ECB or CBC */
}DES3_CTX, *PDES3_CTX;

typedef struct _AES_CTX{
	unsigned char  direction;	/*  Key used for encrypting or decrypting? */
	int   keyLen;	/*  Length of the key  */
	char  keyMaterial[65];  /*  Raw key data in ASCII,e.g., user input or KAT values */
	int   blockLen;   /* block length */
	unsigned char  keySched[15][4][8];	/* key schedule		*/

	unsigned char  mode;            /* MODE_ECB, MODE_CBC, or MODE_CFB1 */
	unsigned char  IV[65]; /* A possible Initialization Vector for 
						   inputLening */
}AES_CTX, *PAES_CTX;

typedef struct rc4_key_st
{
	unsigned int x,y;
	unsigned int data[256];
}RC4_KEY,*PRC4_KEY;

typedef struct {
	unsigned char state[16];                                 /* state */
	unsigned char checksum[16];                           /* checksum */
	unsigned int count;                 /* number of bytes, modulo 16 */
	unsigned char buffer[16];                         /* input buffer */
} MD2_CTX;

typedef struct {
	UINT4 state[4];                                   /* state (ABCD) */
	UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

typedef struct {
	unsigned long state[5];
	unsigned long count[2];
	unsigned char buffer[64];
} SHA1_CTX;

typedef struct sha256_state{
	SilcUInt64 length;
	SilcUInt32 state[8];
	SilcUInt32 curlen;
	unsigned char buf[64];
} SHA256_CTX;


/* RSA key lengths.
*/
#define MIN_RSA_MODULUS_BITS 1024
#define MAX_RSA_MODULUS_BITS 1024
#define MAX_RSA_MODULUS_LEN ((MAX_RSA_MODULUS_BITS + 7) / 8)
#define MAX_RSA_PRIME_BITS ((MAX_RSA_MODULUS_BITS + 1) / 2)
#define MAX_RSA_PRIME_LEN ((MAX_RSA_PRIME_BITS + 7) / 8)

/* RSA public and private key.
*/
typedef struct {
	unsigned short bits;                           /* length in bits of modulus */
	unsigned char modulus[MAX_RSA_MODULUS_LEN];                    /* modulus */
	unsigned char exponent[MAX_RSA_MODULUS_LEN];           /* public exponent */
} R_RSA_PUBLIC_KEY;

typedef struct {
	unsigned short bits;                           /* length in bits of modulus */
	unsigned char modulus[MAX_RSA_MODULUS_LEN];                    /* modulus */
	unsigned char publicExponent[MAX_RSA_MODULUS_LEN];     /* public exponent */
	unsigned char exponent[MAX_RSA_MODULUS_LEN];          /* private exponent */
	unsigned char prime[2][MAX_RSA_PRIME_LEN];               /* prime factors */
	unsigned char primeExponent[2][MAX_RSA_PRIME_LEN];   /* exponents for CRT */
	unsigned char coefficient[MAX_RSA_PRIME_LEN];          /* CRT coefficient */
} R_RSA_PRIVATE_KEY;

/* Diffie-Hellman parameters.
*/
#define MAX_DH_BITS 128
typedef struct {
	unsigned char privateValue[MAX_DH_BITS];
	unsigned char priVallen;
	unsigned char prime[MAX_DH_BITS];                   /* prime */
	unsigned int  primeLen;
	unsigned char generator[MAX_DH_BITS];                /* generator */
	unsigned int generatorLen;                         /* length of generator */
} DH_KEY;

///////////Gen  Rand  /////////////////
	unsigned int Rand_SetSend(unsigned char *pSend, unsigned long ulSendlen);
	unsigned int Rand_GenRand(unsigned char *pRand, unsigned long ulRandLen);
//////////////////////////////////////////////////////////////

	//DES
	unsigned int soft_deskey(PDES_CTX pdes_ctx,int encflag,
					unsigned char *key,int mode,unsigned char *iv);
	unsigned int soft_des_enc(PDES_CTX pdes_ctx,unsigned char *indata,
		unsigned long inlen,unsigned char *outdata);
	unsigned int soft_des_dec(PDES_CTX pdes_ctx,unsigned char *indata,
		unsigned long inlen,unsigned char *outdata);

	//DES3
	unsigned int des2key(PDES3_CTX pdes_ctx,int encflag,
		unsigned char *key,int mode,unsigned char *iv);
	unsigned int des3key(PDES3_CTX pdes_ctx,int encflag,
		unsigned char *key,int mode,unsigned char *iv);
	unsigned int des3_enc(PDES3_CTX pdes_ctx,unsigned char *indata,
		unsigned long inlen,unsigned char *outdata);
	unsigned int des3_dec(PDES3_CTX pdes_ctx,unsigned char *indata,
		unsigned long inlen,unsigned char *outdata);

	//AES
	unsigned int aeskey(PAES_CTX pdes_ctx,int encflag,
		unsigned char *key,int keylen,int mode,unsigned char *iv);
	unsigned int aes_enc(PAES_CTX pdes_ctx,unsigned char *indata,
		unsigned long inlen,unsigned char *outdata);
	unsigned int aes_dec(PAES_CTX pdes_ctx,unsigned char *indata,
		unsigned long inlen,unsigned char *outdata);

	//RC4
	unsigned int rc4key(PRC4_KEY pdes_ctx,unsigned char *key,int keylen);
	unsigned int rc4_crypto(PRC4_KEY pdes_ctx,unsigned char *indata,
		unsigned long inlen,unsigned char *outdata);

	//MD2
	void MD2Init(MD2_CTX *pContext);
	void MD2Update(MD2_CTX *pContext, unsigned char *input, unsigned int inputlen);
	void MD2Final(unsigned char [16], MD2_CTX *pContext);

	//MD5
	void MD5Init(MD5_CTX *);
	void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
	void MD5Final(unsigned char [16], MD5_CTX *);

	//SHA1 
	void SHA1Init(SHA1_CTX* context);
	void SHA1Update(SHA1_CTX* context, unsigned char* data, unsigned int len);
	void SHA1Final(unsigned char digest[20], SHA1_CTX* context);

	//SHA256
	int SHA256Init(SHA256_CTX* context);
	int SHA256Update(SHA256_CTX* context, unsigned char* data, unsigned int len);
	int SHA256Final(unsigned char digest[32], SHA256_CTX* context);
	//RSA
	int RSA_GenerateKey(int bits, R_RSA_PUBLIC_KEY *pPublickey, R_RSA_PRIVATE_KEY *pPrivateKey);

	int soft_RSA_PublicEncrypt(unsigned char *indata, unsigned int indatalen, 
		unsigned char *outdata, unsigned int *outdatalen,R_RSA_PUBLIC_KEY *pPublicKey);
	int soft_RSA_PrivateEncrypt(unsigned char *indata, unsigned int indatalen, 
		unsigned char *outdata, unsigned int *outdatalen,R_RSA_PRIVATE_KEY *pPrivateKey);
	int soft_RSA_PublicDecrypt(unsigned char *indata, unsigned int indatalen, 
		unsigned char *outdata, unsigned int *outdatalen,R_RSA_PUBLIC_KEY *pPublicKey);
	int soft_RSA_PrivateDecrypt(unsigned char *indata, unsigned int indatalen,
		unsigned char *outdata, unsigned int *outdatalen,R_RSA_PRIVATE_KEY *pPrivateKey);

	int RSA_PublicEncryptWithPKCS(unsigned char *indata, unsigned int indatalen, 
		unsigned char *outdata, unsigned int *outdatalen,R_RSA_PUBLIC_KEY *pPublicKey);
	int RSA_PrivateEncryptWithPKCS(unsigned char *indata, unsigned int indatalen, 
		unsigned char *outdata, unsigned int *outdatalen,R_RSA_PRIVATE_KEY *pPrivateKey);
	int RSA_PublicDecryptWithPKCS(unsigned char *indata, unsigned int indatalen, 
		unsigned char *outdata, unsigned int *outdatalen,R_RSA_PUBLIC_KEY *pPublicKey);
	int RSA_PrivateDecryptWithPKCS(unsigned char *indata, unsigned int indatalen,
		unsigned char *outdata, unsigned int *outdatalen,R_RSA_PRIVATE_KEY *pPrivateKey);

	//DH
	int DH_GenerateKey(DH_KEY *pContext,                       /* new Diffie-Hellman parameters */
		unsigned int primeBits,                          /* length of prime in bits */
		unsigned int subPrimeBits                    /* length of subprime in bits */
		);
	int DH_GeneratePublicValue(
		unsigned char *publicValue,                             /* new public value */
		unsigned int pubValueLen,                    /* length of private value */
		DH_KEY *params                           /* Diffie-Hellman parameters */
		);
	int DH_ComputeAgreedKey(
		unsigned char *agreedKey,                                 /* new agreed key */
		unsigned char *otherPublicValue,                    /* other's public value */
		DH_KEY *params                           /* Diffie-Hellman parameters */
		);
	
#ifdef  __cplusplus
}
#endif

#endif 
