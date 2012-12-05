#ifndef __ROM_BOOT_H_
#define __ROM_BOOT_H_
#ifndef __ASSEMBLY__
//#include <stdint.h>
#endif
#include "config.h"

#define POR_ROM_BOOT_ENABLE                         (1<<5)
#define POR_JTAG_ENABLE                             (1<<6)

#define POR_GET_1ST_CFG(a)                          (a&7)
#define POR_1ST_NAND                                7
#define POR_1ST_NAND_RB                             6
#define POR_1ST_SPI                                 5
#define POR_1ST_SPI_RESERVED                        4
#define POR_1ST_SDIO_C                              3
#define POR_1ST_SDIO_B                              2
#define POR_1ST_SDIO_A                              1
#define POR_1ST_NEVER_CHECKED                       0

#define POR_GET_2ND_CFG(a)                          ((a>>3)&3)
#define POR_2ND_SDIO_B                              3
#define POR_2ND_SDIO_A                              2
#define POR_2ND_SDIO_C                              1
#define POR_2ND_NEVER_CHECKED                       0

#define POR_GET_CRYSTAL(a)                          ((a>>9)&1)
#define POR_CRYSTAL_24M                             1
#define POR_CRYSTAL_25M                             0

#define POR_GET_USB_CFG(a)                          ((a>>8)&1)
#define POR_USB_ENABLE                              1
#define POR_USB_DISABLE                             0

//** ECC mode 7, dma 528 bytes(data+parity),Short mode , no scramble 
#define DEFAULT_ECC_MODE  ((2<<20)|(1<<17)|(7<<14)|(1<<13)|(48<<6)|1)

#define RSA_ENCRYPT_POS (0)
#define RSA_ENCRYPT_END (416)
#ifndef __ASSEMBLY__
typedef struct timing_set{
	unsigned nfio;
	unsigned sdio;
	unsigned sfio;
}Timing;
typedef short (* DataRead)(unsigned src,unsigned mem,unsigned count,unsigned ext);
typedef short (* DataInitRead)(unsigned  dev_id,unsigned count);
typedef short (* DataInit)(unsigned  dev_id);

typedef struct {
    unsigned  por_cfg; // current boot configuration
    unsigned  boot_id; // boot from which device
    short     init[2];
    short     load[2][4];
    short     dchk[2][4];
    DataRead  read;
    unsigned  ext;
    unsigned  nand_info_adr;
    unsigned  loop;
    unsigned  efuse_bch_uncor;
} T_ROM_BOOT_RETURN_INFO;

typedef struct data_format{
    unsigned long long  magic;
    unsigned short crc[2];
} DataFormat;
typedef struct
{
	uint8_t pad[372-256/8-14*16-32*2]; //20 bytes
	uint16_t aesmask[32];	//64 bytes
	uint8_t aeskey[14*16];	//224bytes
	uint8_t shasum[256/8];	//32 bytes
} BL2StorageKey;//372bytes
//#include <tfm.h>
typedef struct
{
	uint8_t m[128*3];
	uint8_t shasum[256/8];
}EncryptStorage;
unsigned trustzone_lock(unsigned tag);
extern short romdecrypt(char *src);
#include <tfm.h>
extern short romencrypt(char *src,fp_int * key,uint8_t * aeskey);

#define C_ROM_BOOT_DEBUG                            ((volatile T_ROM_BOOT_RETURN_INFO *)(GL_DATA_ADR))
#define C_ROM_BOOT_DEBUG_LOG                        (volatile unsigned long *)(GL_DATA_ADR + 0xe0)
#endif
//#define DEBUG_EFUSE
#define CPU_START_ADDR_OFFSET						(GL_DATA_ADR+0x80)
#if (!defined NULL)
#define NULL                                        (void*)0
#endif

#define CARD_TYPE_SD		0
#define CARD_TYPE_SDHC		1
#define CARD_TYPE_MMC		2
#define CARD_TYPE_EMMC		3

#define MAGIC_WORD1         0x4848334d  /* "M3HH" */
#define MAGIC_WORD2         0x30564552  /* "REV0" */
#define MAGIC_WORD64		0x305645524848334d /* "M3HHREV0" */
#define STORAGE_KEY_OFFSET (READ_SIZE - 2048)//keep 2048 for keys
/** registers **/
#ifdef FORCE_POR
#define P_ASSIST_POR_CONFIG							(volatile unsigned long *)&force_por_cfg
#else
#define P_ASSIST_POR_CONFIG                         (volatile unsigned long *)0xc1107d54
#endif
#define P_PAD_PULL_UP_REG3                          (volatile unsigned long *)0xc11080f4

#define P_PREG_PAD_GPIO0_EN_N                       (volatile unsigned long *)0xc1108030
#define P_PREG_PAD_GPIO0_O                          (volatile unsigned long *)0xc1108034
#define P_PREG_PAD_GPIO0_I                          (volatile unsigned long *)0xc1108038
#define P_PREG_PAD_GPIO1_EN_N                       (volatile unsigned long *)0xc110803c
#define P_PREG_PAD_GPIO1_O                          (volatile unsigned long *)0xc1108040
#define P_PREG_PAD_GPIO1_I                          (volatile unsigned long *)0xc1108044
#define P_PREG_PAD_GPIO2_EN_N                       (volatile unsigned long *)0xc1108048
#define P_PREG_PAD_GPIO2_O                          (volatile unsigned long *)0xc110804c
#define P_PREG_PAD_GPIO2_I                          (volatile unsigned long *)0xc1108050
#define P_PREG_PAD_GPIO3_EN_N                       (volatile unsigned long *)0xc1108054
#define P_PREG_PAD_GPIO3_O                          (volatile unsigned long *)0xc1108058
#define P_PREG_PAD_GPIO3_I                          (volatile unsigned long *)0xc110805c
#define P_PREG_PAD_GPIO4_EN_N                       (volatile unsigned long *)0xc1108060
#define P_PREG_PAD_GPIO4_O                          (volatile unsigned long *)0xc1108064
#define P_PREG_PAD_GPIO4_I                          (volatile unsigned long *)0xc1108068
#define P_PREG_PAD_GPIO5_EN_N                       (volatile unsigned long *)0xc110806c
#define P_PREG_PAD_GPIO5_O                          (volatile unsigned long *)0xc1108070
#define P_PREG_PAD_GPIO5_I                          (volatile unsigned long *)0xc1108074

#define P_PERIPHS_PIN_MUX_0                         (volatile unsigned long *)0xc11080b0
#define P_PERIPHS_PIN_MUX_1                         (volatile unsigned long *)0xc11080b4
#define P_PERIPHS_PIN_MUX_2                         (volatile unsigned long *)0xc11080b8
#define P_PERIPHS_PIN_MUX_3                         (volatile unsigned long *)0xc11080bc
#define P_PERIPHS_PIN_MUX_4                         (volatile unsigned long *)0xc11080c0
#define P_PERIPHS_PIN_MUX_5                         (volatile unsigned long *)0xc11080c4
#define P_PERIPHS_PIN_MUX_6                         (volatile unsigned long *)0xc11080c8
#define P_PERIPHS_PIN_MUX_7                         (volatile unsigned long *)0xc11080cc
#define P_PERIPHS_PIN_MUX_8                         (volatile unsigned long *)0xc11080d0
#define P_ISA_TIMERE                                (volatile unsigned long *)0xc1109954
#define P_PREG_CTLREG0_ADDR                         (volatile unsigned long *)0xc1108000
#define P_WATCHDOG_TC                               (volatile unsigned long *)0xc1109900
#define P_WATCHDOG_RESET                            (volatile unsigned long *)0xc1109904
#define P_SPI_FLASH_CTRL                            (volatile unsigned long *)0xc1108c88
#define P_SPI_FLASH_CTRL1                           (volatile unsigned long *)0xc1108c8c
#ifndef M3_SIM
#define P_AM_ANALOG_TOP_REG1                        (volatile unsigned long *)0xc11081bc
#define P_HHI_DDR_PLL_CNTL                          (volatile unsigned long *)0xc11041a0
#define P_HHI_DDR_PLL_CNTL2                         (volatile unsigned long *)0xc11041a4
#define P_HHI_DDR_PLL_CNTL3                         (volatile unsigned long *)0xc11041a8
#define P_HHI_DDR_PLL_CNTL4                         (volatile unsigned long *)0xc11041ac
#define P_HHI_SYS_PLL_CNTL                          (volatile unsigned long *)0xc1104260
#define P_HHI_SYS_PLL_CNTL2                         (volatile unsigned long *)0xc1104264
#define P_HHI_SYS_PLL_CNTL3                         (volatile unsigned long *)0xc1104268
#define P_HHI_SYS_PLL_CNTL4                         (volatile unsigned long *)0xc110426c
#define P_HHI_MPLL_CNTL                             (volatile unsigned long *)0xc1104280
#define P_HHI_MPLL_CNTL2                            (volatile unsigned long *)0xc1104284
#define P_HHI_MPLL_CNTL3                            (volatile unsigned long *)0xc1104288
#define P_HHI_MPLL_CNTL4                            (volatile unsigned long *)0xc110428c
#define P_HHI_MPLL_CNTL5                            (volatile unsigned long *)0xc1104290
#define P_HHI_MPLL_CNTL6                            (volatile unsigned long *)0xc1104294
#define P_HHI_MPLL_CNTL7                            (volatile unsigned long *)0xc1104298
#define P_HHI_MPLL_CNTL8                            (volatile unsigned long *)0xc110429c
#define P_HHI_MPLL_CNTL9                            (volatile unsigned long *)0xc11042a0
#define P_HHI_MPLL_CNTL10                           (volatile unsigned long *)0xc11042a4
#else
/**
 * PLL relativer define
 */
#define P_HHI_SYS_PLL_CNTL							((volatile unsigned int*)(0xc1100000 + ((0x105a) << 2)))
#define P_HHI_SYS_PLL_CNTL2							((volatile unsigned int*)(0xc1100000 + ((0x104d) << 2)))
#define P_HHI_SYS_PLL_CNTL3							((volatile unsigned int*)(0xc1100000 + ((0x1056) << 2)))
#define P_HHI_OTHER_PLL_CNTL						((volatile unsigned int*)(0xc1100000 + ((0x1070) << 2)))
#define P_HHI_OTHER_PLL_CNTL2						((volatile unsigned int*)(0xc1100000 + ((0x1071) << 2)))
#define P_HHI_OTHER_PLL_CNTL3						((volatile unsigned int*)(0xc1100000 + ((0x1072) << 2)))
#define P_RESET5_REGISTER							((volatile unsigned int*)(0xc1100000 + ((0x1106) << 2)))
//#define P_RESET1_REGISTER							((volatile unsigned int*)(0xc1100000 + ((0x1102) << 2)))
#endif

#define P_HHI_MPEG_CLK_CNTL                         (volatile unsigned long *)0xc1104174
#define P_HHI_SYS_CPU_CLK_CNTL                      (volatile unsigned long *)0xc110419c
#define P_RESET1_REGISTER                           (volatile unsigned long *)0xc1104408

#define P_AO_RTI_STATUS_REG0						(volatile unsigned long *)0xc8100000
#define P_AO_RTI_STATUS_REG1						(volatile unsigned long *)0xc8100004



#define P_USB_ADDR0                                 (volatile unsigned long *)0xc1108400
#define P_PREG_MV_REG                               (volatile unsigned long *)0xc11081f8

#define EFUSE_CNTL0               0x0
#define EFUSE_CNTL1               0x1
#define EFUSE_CNTL2               0x2
#define EFUSE_CNTL3               0x3
#define EFUSE_CNTL4               0x4
#define EFUSE_Wr_reg(addr, data)  *(volatile unsigned long *)(0xDA000000 | (addr << 2) ) = (data)
#define EFUSE_Rd_reg(addr)        *(volatile unsigned long *)(0xDA000000 | (addr << 2) )

#ifndef __ASSEMBLY__
/**
 * These PART description how the efuse is used .
 */
#define EFUSE_LICENSE_OFFSET		0		//size=4
extern unsigned license_info;
	#define LICCENSE_SERIALBOOT_DISABLE		(1<<10)
	#define LICCENSE_SERIALINFO_DISABLE		(1<<9)
	#define LICCENSE_MACV					(1<<8)
	#define LICCENSE_SECUREBOOT				(1<<7)
	#define LICCENSE_PLLDISABLE				(1<<6)
	#define LICCENSE_CUSTOMERID				(1<<5)
#define EFUSE_CUSTOMERID			4
#define EFUSE_SECUREBOOT_RSAKEY		8		//size=256(1024bits)
void efuse_read(unsigned * dest,unsigned src,unsigned size);


extern short spi_init(unsigned dev_id);
extern short nfio_init(unsigned dev_id);
extern short sdio_init(unsigned dev_id);
extern short spi_init_read (unsigned dev_id,unsigned count);
extern short nfio_init_read(unsigned dev_id,unsigned count);
extern short sdio_init_read(unsigned dev_id,unsigned count);

void udelay(unsigned dly);
unsigned gettimer(unsigned timebase);
void watchdog_clear(void);
extern unsigned g_count;
extern unsigned license_info;
#endif
/** SDIO Return **/
#define ERROR_MOD(mod,num) ((short)(((mod<<6)|num)))

#define ERROR_NONE                  ERROR_MOD(0,0 )      // Success
#define ERROR_GO_IDLE1              ERROR_MOD(0,1 )      // SDIO Go IDLE1 error
#define ERROR_GO_IDLE2              ERROR_MOD(0,2 )      // SDIO Go IDLE2 error
#define ERROR_APP55_1               ERROR_MOD(0,3 )      // SDIO APP55_1 CMD error
#define ERROR_ACMD41                ERROR_MOD(0,4 )      //
#define ERROR_APP55_2               ERROR_MOD(0,5 )      //
#define ERROR_VOLTAGE_VALIDATION    ERROR_MOD(0,6 )      //
#define ERROR_SEND_CID1             ERROR_MOD(0,7 )      //
#define ERROR_SEND_RELATIVE_ADDR    ERROR_MOD(0,8 )      //
#define ERROR_SEND_CID2             ERROR_MOD(0,9 )      //
#define ERROR_SELECT_CARD           ERROR_MOD(0,10)      //
#define ERROR_APP55_RETRY3          ERROR_MOD(0,11)      //
#define ERROR_SEND_SCR              ERROR_MOD(0,12)      //
#define ERROR_READ_BLOCK            ERROR_MOD(0,13)      //
#define ERROR_STOP_TRANSMISSION     ERROR_MOD(0,14)      //
#define ERROR_MAGIC_WORDS           ERROR_MOD(0,15)      //
#define ERROR_CMD1                  ERROR_MOD(0,16)      //
#define ERROR_MMC_SWITCH_BUS        ERROR_MOD(0,17)      //
#define ERROR_MMC_SWITCH_BOOT       ERROR_MOD(0,18)      //
/* Data Check Return */                     // 
#define ERROR_MAGIC_CHECK_SUM       ERROR_MOD(1,0)   //
#define ERROR_MAGIC_WORD_ERROR      ERROR_MOD(1,1)      //
#define ERROR_READ_KEY				ERROR_MOD(1,2)
#define ERROR_ENCRYPT_HASH			ERROR_MOD(1,3)
#define ERROR_RAW_HASH				ERROR_MOD(1,4)
#define ERROR_AES_FORMAT			ERROR_MOD(1,5)
#define ERROR_FORMAT				ERROR_MOD(1,6)

/* NAND Return */                           // 
#define ERROR_NAND_TIMEOUT          ERROR_MOD(2,1)      //
#define ERROR_NAND_ECC              ERROR_MOD(2,2)      //
#define ERROR_NAND_MAGIC_WORD       ERROR_MOD(2,3)      //
#define ERROR_NAND_INIT_READ        ERROR_MOD(2,4)      //
#ifndef __ASSEMBLY__
/** SERIAL BOOT INTERFACE **/
#define SER_START_TAG 0x45454545
/**
 *from device to PC , str
 */
#define SER_STR_START		'S'
/**
 * from device to PC , serial_pack_t.type
 */
#define SER_DUMMY	'I'	///info
#define SER_ACKFAIL 'N' ///wrong return
#define SER_ACKOKAY 'Y' ///yes   return

/**
 * both direction
 * data
 */
/** from PC to DEVICE **/
/**
 * Enter the state ( write/read reg/mem)
 * Any command except DATA command will exit the stat
 */
#define SER_W_MEM			'W'


/**
 * command execute immediately
 */
#define SER_FUNCTION 		'f'
typedef	int (*serial_func_t)(void *,unsigned len);
#define SER_UNLOCK 			'L'
#define SER_READY 			'T'

typedef struct serial_pack_header_s
{
	unsigned start_tag;
	unsigned char ser; ///hex bcd serial number, a normal infomation must big than 127 .
	unsigned char type; ///data(both direction) , str(device to pc),cmd(pc to device) , result ( device to pc) ,info(device to pc)
#define SER_PACK_HEAD_TAG_SIZE 6
#define SER_PACK_HEAD_HEX_SIZE 12
#define SER_PACK_HEAD_BIN_SIZE 6
#define SER_PACK_HEAD_SIZE		12
	unsigned short len; ///package len ///hex
	union{
		/**
		 * from device to PC
		 */
		struct {
			unsigned char rv_buf_len;
			unsigned char reason;///hex value
		} result;
		/**
		 * from pc to device
		 */
#define SER_PACK_DECODE_ADDR(a,type) (typeof(type) )(((unsigned)a<<2)+MEMORY_LOC)
		unsigned  short addr;
		/**
		 * for test read cmd only
		 */

		#define SER_READY_DUMMY					0
		#define SER_READY_EXIT_TO_1ST			1
		#define SER_READY_EXIT_TO_2ND			2
		#define SER_READY_EXIT_TO_3RD			3
		unsigned  short subcmd;
	}ctx;
	unsigned short checksum;///hex value head checksum
} serial_pack_head_t;




/**
 * FAIL reason
 */
#define SER_FAIL_LEN_BIG_THAN_48	-2
#define SER_FAIL_CHECKSUM			-3
#define SER_FAIL_ILLEGAL_FORMAT		-4
#define SER_FAIL_ILLEGAL_CMD		-5
#define SER_FAIL_UNLOCK				-6
#define SER_FAIL_MISALIGNMENT		-7
#define SER_FAIL_DATA_CHECKSUM 		-8
#define SER_FAIL_BREAK_BOOT 		-9

extern const char * ltoa(unsigned long);
#define SET_LOG(a) {*C_ROM_BOOT_DEBUG_LOG=a;if((license_info&LICCENSE_SERIALINFO_DISABLE)==0)serial_puts(ltoa((unsigned long)a));}

#endif

#endif
