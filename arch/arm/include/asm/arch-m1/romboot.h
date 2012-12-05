/*
 * romboot.h
 *
 *  Created on: 2010-6-22
 *      Author: yuhao
 *      This file described romboot area usage and power on settings
 */

#ifndef ROMBOOT_H_
#define ROMBOOT_H_
#define POR_ROM_BOOT_ENABLE                         (1<<7)
#define POR_JTAG_ENABLE                             (1<<6)
//Power On setting
#define POR_INTL_CFG_MASK                           (7<<0)
#define POR_INTL_NAND_LP                            (7<<0)
#define POR_INTL_SPI                                (5<<0)
#define POR_INTL_SDIO_B1                            (6<<0)
#define POR_INTL_NAND_SP                            (4<<0)
#define POR_GET_INTL_CFG(a)                         (a&POR_INTL_CFG_MASK)

#define POR_SDIO_CFG_MASK                           (3<<8)
#define POR_SDIO_A_ENABLE                           (3<<8)
#define POR_SDIO_B_ENABLE                           (1<<8)
#define POR_SDIO_C_ENABLE                           (2<<8)
//#define POR_SDIO_B1_ENABLE                          (0<<8)
#define POR_GET_SDIO_CFG(a)                         ((a&POR_SDIO_CFG_MASK))

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define POR_GET_1ST_CFG(a)       (a&7)
#define POR_1ST_NAND             7
#define POR_1ST_NAND_RB          6
#define POR_1ST_SPI              5
#define POR_1ST_SPI_RESERVED     4
#define POR_1ST_SDIO_C           3
#define POR_1ST_SDIO_B           2
#define POR_1ST_SDIO_A           1
#define POR_1ST_NEVER_CHECKED    0
#define POR_GET_2ND_CFG(a)       ((a>>3)&3)
#define POR_2ND_SDIO_B           3
#define POR_2ND_SDIO_A           2
#define POR_2ND_SDIO_C           1
#define POR_2ND_NEVER_CHECKED    0
#define POR_GET_CRYSTAL(a)       ((a>>7)&1)
#define POR_CRYSTAL_24M          1
#define POR_CRYSTAL_25M          0
#define POR_GET_USB_CFG(a)       ((a>>8)&1)
#define POR_USB_ENABLE           1
#define POR_USB_DISABLE          0
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/*
 * "MESON001"
 */
#define MAGIC_WORD1         0x4f53454d
#define MAGIC_WORD2         0x3130304e
#define READ_SIZE       	6*1024      // memory size for data reading

#define SPI_MEM_BASE                                0x40000000
#define AHB_SRAM_BASE                               0x49000000  // AHB-SRAM-BASE
#define C_ROM_BOOT_DEBUG                            ((0x49000000 + 0x1f00))
#define MAGIC_STRUCT_OFF							(0x1b0)
/**
 * rom debug area structure offsets
 */
#define POR_CFG	0
#define BOOT_ID	4
#define LOAD_0	8
#define LOAD_1	12
#define DCHK_0	16
#define DCHK_1	20
#define NAND_ADDR	24
#define CARD_TYPE	28
#ifndef __ASSEMBLY__
/**
 * rom debug area structure (c format)
 */
typedef struct {
    unsigned  por_cfg; // current boot configuration
    unsigned  boot_id; // boot from which device
    int       load[2];
    int       dchk[2];
    int       nand_addr;
    int       card_type;
    unsigned  clk81;
    unsigned  a9_clk;
} T_ROM_BOOT_RETURN_INFO;

#define C_ROM_BOOT_DEBUG_INFO   ((volatile T_ROM_BOOT_RETURN_INFO *)(C_ROM_BOOT_DEBUG))

/**
 * magic structure
 */
typedef struct data_format{
    unsigned  magic[2];
    unsigned short crc[2];
} DataFormat ;
extern  T_ROM_BOOT_RETURN_INFO * romboot_info;
extern  DataFormat * magic_info;
#endif
/**
 *
 */

#define HARDWARE_MODE_DDR_TEST  0

#if HARDWARE_MODE_DDR_TEST==1
// this is testing hardware mode
#define    ROM_STACK_END  	(AHB_SRAM_BASE+0x1d80)
#define    ROM_FIQ_STACK	(AHB_SRAM_BASE+0x1e00)
#define    ROM_IRQ_STACK  	(AHB_SRAM_BASE+0x1f00)
//#define    BASE_ADDR        SPI_MEM_BASE
#define    BASE_ADDR        0

#undef READ_SIZE
#define READ_SIZE       	(14*1024 )     // memory size for data reading

#define LOAD_REGION

#else
#define    ROM_STACK_END  	(AHB_SRAM_BASE+0x1d80)
#define    ROM_FIQ_STACK	(AHB_SRAM_BASE+0x1e00)
#define    ROM_IRQ_STACK  	(AHB_SRAM_BASE+0x1f00)

#define    BASE_ADDR        AHB_SRAM_BASE
#endif

#define    START_BASE_ADDR   BASE_ADDR

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

void print_romboot(void);

#else
#endif

#endif /* ROMBOOT_H_ */
