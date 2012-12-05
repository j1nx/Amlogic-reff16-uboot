
#include <config.h>
#include <efuse_m3.h>
#include "efuse_regs.h"

/** efuse layout
http://wiki-sh.amlogic.com/index.php/How_To_burn_the_info_into_E-Fuse
0~3					licence					1 check byte				4 bytes(in total)
4~10				mac						1 check byte				7 bytes(in total)
12~322			hdcp					10 check byte				310 bytes(in total)
322~328 			mac_bt				1 check byte				7 bytes(in total)
330~336 			mac_wifi				1 check byte				7 bytes(in total)
337~384  		usid						2 check byte		 		48 bytes(in total)
*/

/**
	new efuse version 0.0 (for exist MP production, compitable version)
*/
/** efuse layout before M3 (for M1, M2, A3)
0~3					licence					1 check byte				4 bytes(in total)
4~10				mac						1 check byte				7 bytes(in total)
12~322			hdcp					10 check byte				310 bytes(in total)
322~328 			mac_bt				1 check byte				7 bytes(in total)
330~336 			mac_wifi				1 check byte				7 bytes(in total)
337~379  		usid						2 check byte		 		44 bytes(in total)
380~384			version				1 check byte 				4 bytes(in total) (it should be zero)
*/

/**
 * efuse version 0.1 (for M3 ) 
 * M3 efuse: read all free efuse data maybe fail on addr 0 and addr 0x40
 * so M3 EFUSE layout avoid using 0 and 0x40
 */
/** 
1~38				usid						2 check byte				38 bytes(in total)
39~45	 			mac_wifi				1 check byte				7 bytes(in total)
46~52	 			mac_bt				1 check byte				7 bytes(in total)
53~59				mac						1 check byte				7 bytes(in total)
60~63				licence					1 check byte				4 bytes(in total)
68~377			hdcp					10 check byte				310 bytes(in total)
380~383			version 				1 check byte				4 bytes(in total) (version=1)
* 		3 valid byte is 1byte for version and 2byte for customerid . version is 1
* 		
*/

/**
 * efuse version 0.2 (after M6 )
*/
/** 
0~2					licence					3bytes ( some basic feature control)
3						version 				1byte  ( must be 2)
4~7					customerid			4bytes (0 , keep , should not be 0)
8~135				rsa key					128bytes (if Secure Boot enable )
136~435			hdcp 					300bytes			
436~441			mac0					6bytes for the main network interface
442~447			mac1					6bytes for the second network interface or bt 
448~453			mac2					6bytes for the second network interface or bt 
454~511			userid 					58bytes (not used)
*/
	
static efuseinfo_item_t efuseinfo_v1[] =
{
	{
		.title = "usid",
		.offset = V1_EFUSE_USID_OFFSET,
		.enc_len = V1_EFUSE_USID_ENC_LEN, //35,
		.data_len = V1_EFUSE_USID_DATA_LEN, //35,
		.we = 0,		
		.bch_en = V1_EFUSE_USID_BCH_EN, //0
		.bch_reverse = V1_EFUSE_USID_BCH_REVERSE, //0,
	},	
	{
		.title = "mac_wifi",
		.offset = 39,
		.enc_len = 7,
		.data_len = 6,
		.we = 1,		
		.bch_en=1,
		.bch_reverse = 0,
	},
	{
		.title = "mac_bt",
		.offset = 46,
		.enc_len = 7,
		.data_len = 6,
		.we=1,	
		.bch_en=1,
		.bch_reverse = 0,
	},
	{
		.title = "mac",
		.offset = 53,
		.enc_len = 7,
		.data_len = 6,
		.we = 1,		
		.bch_en=1,
		.bch_reverse = 0,
	},
	{
		.title = "licence",
		.offset = 60,
		.enc_len = 4,
		.data_len = 3,
		.we = 0,		
		.bch_en=1,
		.bch_reverse = 1,
	},	
	{
		.title = "hdcp",
		.offset = 68,
		.enc_len = 310,
		.data_len = 300,
		.we = 0,		
		.bch_en=1,
		.bch_reverse = 0,
	},
	{
		.title= "version",     //1B(version=1)+2B(machid)
		.offset=EFUSE_VERSION_OFFSET, //380,
		.enc_len = EFUSE_VERSION_ENC_LEN, //4,
		.data_len = EFUSE_VERSION_DATA_LEN, //3,
		.we = 1,		
		.bch_en=EFUSE_VERSION_BCH_EN, //1;
		.bch_reverse = EFUSE_VERSION_BCH_REVERSE, //0,
	},
};

static efuseinfo_item_t efuseinfo_v3[] =
{
	{
		.title = "usid",
		.offset = V1_EFUSE_USID_OFFSET,
		.enc_len = V1_EFUSE_USID_ENC_LEN, //35,
		.data_len = V1_EFUSE_USID_DATA_LEN, //35,
		.we = 0,		
		.bch_en = V1_EFUSE_USID_BCH_EN, //0
		.bch_reverse = V1_EFUSE_USID_BCH_REVERSE, //0,
	},	
	{
		.title = "mac_wifi",
		.offset = 39,
		.enc_len = 7,
		.data_len = 6,
		.we = 1,		
		.bch_en=1,
		.bch_reverse = 0,
	},
	{
		.title = "mac_bt",
		.offset = 46,
		.enc_len = 7,
		.data_len = 6,
		.we=1,	
		.bch_en=1,
		.bch_reverse = 0,
	},
	{
		.title = "mac",
		.offset = 53,
		.enc_len = 7,
		.data_len = 6,
		.we = 1,		
		.bch_en=1,
		.bch_reverse = 0,
	},
	{
		.title = "licence",
		.offset = 60,
		.enc_len = 4,
		.data_len = 3,
		.we = 0,		
		.bch_en=1,
		.bch_reverse = 0,
	},	
	{
		.title = "rsakey-boot",
		.offset = 68,
		.enc_len = 44,
		.data_len = 44,
		.we = 0,		
		.bch_en=0,
		.bch_reverse = 0,
	},
	{
		.title = "rsakey-comm",
		.offset = 112,
		.enc_len = 34,
		.data_len = 34,
		.we = 0,		
		.bch_en=0,
		.bch_reverse = 0,
	},
	{
		.title = "CI",
		.offset = 146,
		.enc_len = 5,
		.data_len = 4,
		.we = 0,		
		.bch_en=1,
		.bch_reverse = 0,
	},	
	{
		.title = "SN",
		.offset = 151,
		.enc_len = 26,
		.data_len = 26,
		.we = 0,		
		.bch_en=0,
		.bch_reverse = 0,
	},
	{
		.title = "IN",
		.offset = 177,
		.enc_len = 26,
		.data_len = 26,
		.we = 0,		
		.bch_en=0,
		.bch_reverse = 0,
	},	
	{
		.title = "hdcp",
		.offset = 234,
		.enc_len = 144,
		.data_len = 144,
		.we = 0,		
		.bch_en=0,
		.bch_reverse = 0,
	},
	{
		.title= "version",     //1B(version=1)+2B(machid)
		.offset=EFUSE_VERSION_OFFSET, //380,
		.enc_len = EFUSE_VERSION_ENC_LEN, //4,
		.data_len = EFUSE_VERSION_DATA_LEN, //3,
		.we = 1,		
		.bch_en=EFUSE_VERSION_BCH_EN, //1;
		.bch_reverse = EFUSE_VERSION_BCH_REVERSE, //0,
	},
};


efuseinfo_t efuseinfo[] = 
{
	{
		.efuseinfo_version = efuseinfo_v1,
		.size = sizeof(efuseinfo_v1)/sizeof(efuseinfo_item_t),
		.version =1,
	},
	{
		.efuseinfo_version = efuseinfo_v3,
		.size = sizeof(efuseinfo_v3)/sizeof(efuseinfo_item_t),
		.version =3,
	},
};

int efuseinfo_num = sizeof(efuseinfo)/sizeof(efuseinfo_t);
int efuse_active_version = -1;
unsigned efuse_active_customerid = 0;
pfn efuse_getinfoex = 0;
pfn_byPos efuse_getinfoex_byPos=0;
