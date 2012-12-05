/*
 * romboot.h
 *
 *  Created on: 2010-6-22
 *      Author: yuhao
 *      This file described romboot area usage and power on settings
 */

#ifndef ROMBOOT_H_
#define ROMBOOT_H_
#include <config.h>
#define RAM_START       0x49000000
//#define RAM_START       0x1c00000 //nike debug
#define RAM_SIZE        0x20000
#define RAM_END         (RAM_START+RAM_SIZE)
#define MEMORY_LOC      RAM_START

#define ROMBOOT_START   0x49800000
#define ROM_SIZE        (16*1024)
#define ROMBOOT_END     (ROMBOOT_START+ROM_SIZE)


#define GL_DATA_ADR         (RAM_END-256)
#define READ_SIZE       32*1024     // Size for data reading
#define CHECK_SIZE      8*1024      // Size for data checking
#define NOR_START_ADDR  0x40000000


#define POR_ROM_BOOT_ENABLE                         (1<<5)
#define POR_JTAG_ENABLE                             (1<<6)
//Power On setting
#define POR_GET_INTL_CFG(a)                         (a&7)
#define POR_INTL_NAND                               7
#define POR_INTL_NAND_SP                            6
#define POR_INTL_NAND_RB                            5
#define POR_INTL_NAND_SP_RB                         4
#define POR_INTL_SPI                                3
#define POR_INTL_SDIO_C                             2
#define POR_INTL_SDIO_B                             1
#define POR_INTL_SDIO_A                             0


#define POR_GET_EXT_CFG(a)                         ((a>>3)&3)
#define POR_EXT_SDIO_C                              2
#define POR_EXT_SDIO_B                              1
#define POR_EXT_SDIO_A                              0

#define POR_GET_CRYSTAL(a)                          ((a>>8)&1)
#define POR_CRYSTAL_24M                             1
#define POR_CRYSTAL_25M                             0
#define POR_GET_DDR(a)                              ((a>>7)&1)
#define POR_DDR2                                    1
#define POR_DDR3                                    0



/*
 * magic word 
 */
#define MAGIC_WORD1         0x4f53454d  /* "MESO" */
#define MAGIC_WORD2         0x33412d4e  /* "N-A3" */


#define SPI_MEM_BASE                                0x40000000
#define AHB_SRAM_BASE                               RAM_START  // AHB-SRAM-BASE
#define C_ROM_BOOT_DEBUG                            (GL_DATA_ADR)
#define MAGIC_STRUCT_OFF							(0x1b0)
/**
 * rom debug area structure offsets
 */
#if  CONFIG_JTAG_CONSOLE
#define CONFIG_JTAG_CONSOLE_OUTPUT_MASK 4095
#define CONFIG_JTAG_CONSOLE_INPUT_MASK  4095
#define CONFIG_JTAG_CONSOLE_OUTPUT_BUF  (RAM_END-257-CONFIG_JTAG_CONSOLE_OUTPUT_MASK)
#define CONFIG_JTAG_CONSOLE_INPUT_BUF  (CONFIG_JTAG_CONSOLE_OUTPUT_BUF-1-CONFIG_JTAG_CONSOLE_INPUT_MASK)
#define ROM_STACK_END       CONFIG_JTAG_CONSOLE_INPUT_BUF
#endif
#ifndef __ASSEMBLY__
typedef short (* DataRead)(unsigned src,unsigned mem,unsigned count,unsigned ext);
#if  CONFIG_JTAG_CONSOLE
struct __io_buffer{
    unsigned   mask;
    char  *buf;
    unsigned   rd;
    unsigned   wr;
};

#endif
typedef struct {
    unsigned  por_cfg; // current boot configuration
    unsigned  boot_id; // boot from which device
    short       init[2];
    short       load[2][4];
    short       dchk[2][4];
    DataRead  read;
    unsigned  ext;
    unsigned  nand_info_adr;
    unsigned  loop;
    unsigned  efuse_bch_uncor;
    unsigned  a9_clk;
    unsigned clk81;
    int (*tstc)(void);
    int (*getc)(void);
    void (*putc)(const char c);
} T_ROM_BOOT_RETURN_INFO;

typedef struct data_format{
    unsigned  magic[2];
    unsigned short crc[2];
} DataFormat;
extern  T_ROM_BOOT_RETURN_INFO * romboot_info;
extern  DataFormat * magic_info;
#endif
/**
 *
 */
#ifndef ROM_STACK_END
#define    ROM_STACK_END  	(RAM_END-256)
#endif
/**
 * This Section is about the romboot spl's first sector structure
 */
#ifndef __ASSEMBLY__

struct romboot_boot_settings{
	struct ddr_timing_set{
        unsigned short cl; // cas latency
        unsigned short t_faw;
        unsigned short t_mrd;
        unsigned short t_1us_pck;
        unsigned short t_100ns_pck;
        unsigned short t_init_us;
        unsigned short t_ras;
        unsigned short t_rc;
        unsigned short t_rcd;
        unsigned short t_refi_100ns;
        unsigned short t_rfc;
        unsigned short t_rp;
        unsigned short t_rrd;
        unsigned short t_rtp;
        unsigned short t_wr;
        unsigned short t_wtr;
        unsigned short t_xp;
        unsigned short t_xsrd;       // init to 0 so that if only one of them is defined, this is chosen
        unsigned short t_xsnr;
        unsigned short t_exsr;
        unsigned short t_al;     // Additive Latency
        unsigned short t_clr;    // cas_latency for DDR2 (nclk cycles)
        unsigned short t_dqs;    // distance between data phases to different ranks
        unsigned short t_cwl;     // cas write latency.
  		unsigned short t_mod;     // MRS command  
  		unsigned short t_zqcl;    // ZQ calibration long period in clock cycles.
 		unsigned short t_cksrx;   // CKE maintained high before issuing self refresh command. 
  		unsigned short t_cksre;   // Time after Self Refresh Entry that CKE is heold high before going low. 
 		unsigned short t_cke;     // CKE minimum pulse width in memory clock cycles. 
		unsigned short lane;
	}__attribute__ ((packed)) ddr;
//	unsigned short system_clk;
	unsigned ddr_pll_cntl;//400M for DDR 800 , 333M for DDR667
	unsigned sys_pll_cntl;//0x7c
	unsigned other_pll_cntl;
	unsigned mpeg_clk_cntl;
	unsigned clk81;//0x80
	unsigned a9_clk;
	unsigned spi_setting;
	unsigned nfc_cfg;
	unsigned sdio_cmd_clk_divide;
	unsigned sdio_time_short;//0x90
	unsigned demod_pll400m_cntl;
}__attribute__ ((packed));
extern struct romboot_boot_settings  __hw_setting;
extern DataFormat  __magic_word;
#else
#endif
/* Data Check Return */                     // 
#define ERROR_MAGIC_CHECK_SUM       19      // 
#define ERROR_MAGIC_WORD_ERROR      20      // 
/* NAND Return */                           // 
#define ERROR_NAND_TIMEOUT          21      // 
#define ERROR_NAND_ECC              22      // 
#define ERROR_NAND_MAGIC_WORD       23      // 
#define ERROR_NAND_INIT_READ        24      // 

//
#endif /* ROMBOOT_H_ */
