#ifndef __EFUSE_M3_H
#define __EFUSE_M3_H

#include <config.h>
#include <common.h>

typedef struct efuseinfo_item{
	char title[40];	
	unsigned offset;    // write offset
	unsigned enc_len;
	unsigned data_len;
	int we;    // write enable 	 	
	int bch_en;
	int bch_reverse;
} efuseinfo_item_t;


typedef struct efuseinfo{
	struct efuseinfo_item *efuseinfo_version;
	int size;
	int version;
}efuseinfo_t;

typedef int (*pfn) (char *title, efuseinfo_item_t *info); 
typedef int(*pfn_byPos)(unsigned pos, efuseinfo_item_t *info);

char *efuse_dump(void);
unsigned efuse_readcustomerid(void);
void efuse_getinfo_version(efuseinfo_item_t *info);

int efuse_getinfo(char *title, efuseinfo_item_t *info);
int efuse_read_usr(char *buf, size_t count, loff_t *ppos);
int efuse_write_usr(char* buf, size_t count, loff_t* ppos);


#if defined(CONFIG_AML_MESION_3) //|| defined(CONFIG_M1) 
// for M1, M2, M3, A3 efuse length 
#define EFUSE_BYTES            384  //(EFUSE_BITS/8)
#define EFUSE_DWORDS            96  //(EFUSE_BITS/32)
#else
// for m6 and after efuse length
#define EFUSE_BYTES				512   //(EFUSE_BITS/8)
#define EFUSE_DWORDS		128   //(EFUSE_BITS/32)
#endif

#endif

