/*
 * (C) Copyright 2006-2010
 * Amlogic .
 * Author :
 *	Jerry Yu <jerry.yu@amlogic.com>
 */

#ifndef __M3_MID_REFF09_H
#define __M3_MID_REFF09_H
//#define DEBUG 1
/*
 * High Level Configuration Options
 */
#define CONFIG_ARMCORTEXA9	   1	 /* This is an ARM V7 CPU core */

#define CONFIG_MESON		   1	 /* in a Amlogic Meson core */

#define CONFIG_AMLROM_U_BOOT	   1    /**
										Meson support Romboot mode
								    */
#define CONFIG_BOARD_8726M_ARM    1

#define CONFIG_MEMSIZE  512     //unit is MB

#define AMLOGIC_REBOOT_MODE

#define CONFIG_UBI_SUPPORT
#ifdef	CONFIG_UBI_SUPPORT
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#define CONFIG_RBTREE
#define CONFIG_MTD_DEVICE               /* needed for mtdparts commands */
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTDPARTS

#define MTDIDS_DEFAULT		"nand1=nandflash1\0"
#define MTDPARTS_DEFAULT	"mtdparts=nandflash1:256m@168m(system)\0"
#endif

#define CONFIG_CMD_AUTOSCRIPT
#define CONFIG_AML_AUTOSCRIPT
#define AML_AUTOSCRIPT  "aml_autoscript"
#define SCAN_MMC_PARTITION  4

#define HAS_AO_MODULE

#define CONFIG_AML_I2C	//add by Elvis Yu
#define CONFIG_PMU_ACT8942
#define CONFIG_BQ27x00_BATTERY	1

#define CONFIG_SWITCH_BOOT_MODE
#define CONFIG_EFUSE
#define CONFIG_SARADC   1
#define SCAN_DEVICE_PARTITION	4

#define CONFIG_USB_DEVICE
#define CONFIG_FASTBOOT
#define CONFIG_USBPORT_BASE IO_USB_A_BASE
//#define CONFIG_AML_NANDBCH16
#define CONFIG_UCL		1
/**
 * NAND define
 *
 */

#include <asm/arch/cpu.h>	/* get chip and board defs */

/*
 * Display CPU and Board information
 */

/* Clock Defines */
#define CONFIG_SYS_CLK      600 /*600Mhz  a9 clk@todo redefine it later
                                 */
#define CONFIG_CRYSTAL_MHZ		24
#undef CONFIG_USE_IRQ			   /* no support for IRQs */
#define CONFIG_MISC_INIT_R

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_CMDLINE_EDITING 1       /* Command-line editing */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1
#define CONFIG_REVISION_TAG		1
#define CONFIG_CMD_KGDB			1
//#define CONFIG_SERIAL_TAG       1

/*
 * Size of malloc() pool
 */
						/* Sector */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (1 << 26))
#define CONFIG_SYS_GBL_DATA_SIZE	128	/* bytes reserved for */
						/* initial data */
/*
 * Hardware drivers
 */

/*osd*/
#if 0
#define CONFIG_AMLOSD
#define CONFIG_AMLVIDEO
#define CONFIG_CMD_BMP
#define CONFIG_CMD_TV
#define CONFIG_CMD_OSD
#define CONFIG_AMLHDMI
#define CONFIG_LOGDISPLAY
#define FB_ADDR 0x84900000
#define OSD_WIDTH      720
#define OSD_HEIGHT     576
#endif

/* config LCD output */ 
#define CONFIG_LCD_AML
#define CONFIG_LCD_AML_TTL
#define CONFIG_CMD_BMP
#define LCD_BPP LCD_COLOR24
#define CURRENT_OSD OSD2
#define FB_ADDR 0x84900000

#define OSD_WIDTH   800
#define OSD_HEIGHT  600
#define OSD_BPP     OSD_COLOR24

#define CONFIG_AML_FONT
#define ENABLE_FONT_RESOURCE
#ifdef ENABLE_FONT_RESOURCE
#define DEFAULT_FONT &ISO_88591_32Font
#endif


/* config battery charging */ 
#define CONFIG_BATTERY_CHARGING  1

/*
 * select serial console configuration
 */
//#define CONFIG_CONS_INDEX		0
#undef CONFIG_CONS_INDEX

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{4800, 9600, 19200, 38400, 57600,\
					115200}
#define CONFIG_MMC			   1
//#define CONFIG_OMAP3_MMC		1
#define CONFIG_DOS_PARTITION		1


/* USB
 * Enable CONFIG_MUSB_HCD for Host functionalities MSC, keyboard
 * Enable CONFIG_MUSB_UDD for Device functionalities.
 */
/* #define CONFIG_MUSB_UDC		1 */
#define CONFIG_M1_USBPORT_BASE	0xC90C0000
#define CONFIG_USB_STORAGE
#define CONFIG_USB_DWC_OTG_HCD
//#define CONFIG_AML_TINY_USBTOOL


/* commands to include */
#include <config_cmd_default.h>

#define CONFIG_CMD_CACHE_TEST

#define CONFIG_CMD_EXT2		/* EXT2 Support			*/
//#define CONFIG_CMD_FAT		/* FAT support			*/
//#define CONFIG_CMD_JFFS2	/* JFFS2 Support		*/
//
//#define CONFIG_CMD_I2C		/* I2C serial bus support	*/
//#define CONFIG_CMD_MMC		/* MMC support			*/
//#define CONFIG_CMD_ONENAND	/* ONENAND support		*/
#define CONFIG_CMD_USB
#define CONFIG_CMD_FAT

//#define CONFIG_CMD_NET			1
#if defined(CONFIG_CMD_NET)
#define CONFIG_NET_MULTI        1
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#endif /* (CONFIG_CMD_NET) */

//
#undef CONFIG_CMD_ONENAND
#undef CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/
#undef CONFIG_CMD_FPGA		/* FPGA configuration Support	*/
#undef CONFIG_CMD_IMI		/* iminfo			*/
#undef CONFIG_CMD_IMLS		/* List all found images	*/

#define CONFIG_SYS_NO_FLASH 1

#define CONFIG_CMD_NAND 

#define CONFIG_YAFFS2

/* Environment information */
#define CONFIG_BOOTDELAY	1

#define CONFIG_BOOTFILE		uImage

#if(CONFIG_MEMSIZE == 512)
#define BOARD_INFO_ENV  " mem=512M"
#else
#define BOARD_INFO_ENV ""
#endif

#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x82000000\0" \
	"testaddr=0x82400000\0" \
	"usbtty=cdc_acm\0" \
	"console=ttyS2,115200n8\0" \
	"mmcargs=setenv bootargs console=${console} " \
	"boardname=m1_mbox\0" \
	"chipname=8726m\0" \
	"bootargs=init=/init console=ttyS0,115200n8 nohlt a9_clk=600M clk81=192M hdmitx=vdacoff,powermode1,unplug_powerdown mem=512m logo=osd1,0x84100000,lcd,full\0" \
	"mtdids=" MTDIDS_DEFAULT \
	"mtdparts="MTDPARTS_DEFAULT \
	"bootloader_start=0\0" \
	"bootloader_size=60000\0" \
	"bootloader_path=u-boot-aml-ucl.bin\0" \
	"normal_name=boot\0" \
	"normal_start=0x8800000\0" \
	"normal_size=0x800000\0" \
	"recovery_name=recovery\0" \
	"recovery_start=0x6800000\0" \
	"recovery_size=0x800000\0" \
	"recovery_path=uImage_recovery\0" \
	"logo_name=logo\0" \
	"logo_start=0x4800000\0" \
	"logo_size=0x400000\0" \
	"aml_logo_name=aml_logo\0" \
	"aml_logo_start=0x5800000\0" \
	"aml_logo_size=0x400000\0"
	
	
#define CONFIG_BOOTCOMMAND  "nand read ${logo_name} 84100000 0 ${logo_size};nand read ${normal_name} ${loadaddr} 0 ${normal_size};bootm"

#define CONFIG_AUTO_COMPLETE	1
/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_PROMPT		"8726M_ref # "
#define CONFIG_SYS_CBSIZE		256	/* Console I/O Buffer Size */
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		16	/* max number of command */
						/* args */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		(CONFIG_SYS_CBSIZE)

#define CONFIG_SYS_LOAD_ADDR    0
/*
 * clk , timer setting 
 */
#define CONFIG_SYS_HZ			1000
#define CONFIG_HARDWARE_FIN     24000000 // 24Mhz crystal .
/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */

#define CONFIG_STACKSIZE	(128 << 10)	/* regular stack 128 KiB */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4 << 10)	/* IRQ stack 4 KiB */
#define CONFIG_STACKSIZE_FIQ	(4 << 10)	/* FIQ stack 4 KiB */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1	/* CS1 may or may not be populated */
#define PHYS_MEMORY_START    0x80000000 // from 500000
#if(CONFIG_MEMSIZE == 128)
#define PHYS_MEMORY_SIZE      0x8000000 // 128M
#elif(CONFIG_MEMSIZE == 256)
#define CONFIG_DDR_TYPE DDR_K4T1G164QE //128M/PCS DDR
#define PHYS_MEMORY_SIZE     0x10000000 // 256M
#elif(CONFIG_MEMSIZE == 512)
#define CONFIG_DDR_TYPE DDR_W972GG6JB	//256M/PCS DDR
#define PHYS_MEMORY_SIZE     0x20000000 // 512M
#else
#ERROR: Must config CONFIG_MEMSIZE
#endif
#define CONFIG_SYS_MEMTEST_START	0x80000000	/* memtest works on	*/      
#define CONFIG_SYS_MEMTEST_END		0x07000000	/* 0 ... 120 MB in DRAM	*/  

/* SDRAM Bank Allocation method */
#define SDRC_R_B_C		1

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

/* **** PISMO SUPPORT *** */

/* Configure the PISMO */
#define PISMO1_NAND_SIZE		GPMC_SIZE_128M
#define PISMO1_ONEN_SIZE		GPMC_SIZE_128M

#define CONFIG_SYS_MAX_FLASH_SECT	520	/* max number of sectors */
						/* on one chip */
#define CONFIG_SYS_MAX_FLASH_BANKS	2	/* max number of flash banks */
#define CONFIG_SYS_MONITOR_LEN		(256 << 10)	/* Reserve 2 sectors */

#define CONFIG_SYS_FLASH_BASE		boot_flash_base

/* Monitor at start of flash */
//#define CONFIG_ENV_IS_NOWHERE 1
//#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_FLASH_BASE
//#define CONFIG_SYS_ONENAND_BASE		ONENAND_MAP
//
//#define CONFIG_ENV_IS_IN_ONENAND	1
//#define ONENAND_ENV_OFFSET		0x260000 /* environment starts here */
//#define SMNAND_ENV_OFFSET		0x260000 /* environment starts here */
//
//#define CONFIG_SYS_ENV_SECT_SIZE	boot_flash_sec
//#define CONFIG_ENV_OFFSET		boot_flash_off
//#define CONFIG_ENV_ADDR			boot_flash_env_addr

//#define CONFIG_SPI_BOOT 1
#define CONFIG_NAND_BOOT 1

#ifdef CONFIG_NAND_BOOT
#define CONFIG_AMLROM_NANDBOOT 1
#endif

#ifdef CONFIG_SPI_BOOT
	#define CONFIG_ENV_OVERWRITE
	#define CONFIG_ENV_IS_IN_SPI_FLASH
	#define CONFIG_CMD_SAVEENV
	#define CONFIG_ENV_SIZE             0x2000
	#define CONFIG_ENV_SECT_SIZE        0x1000
	#define CONFIG_ENV_OFFSET           0x1f0000
#elif defined CONFIG_NAND_BOOT
	#define CONFIG_ENV_IS_IN_AML_NAND
	#define CONFIG_CMD_SAVEENV
	#define CONFIG_ENV_OVERWRITE
	#define CONFIG_ENV_SIZE         0x2000
	#define CONFIG_ENV_OFFSET       0x400000
	#define CONFIG_ENV_BLOCK_NUM    2
#else
#define CONFIG_SYS_ENV_IS_NOWHERE 1
#endif
/* Monitor at start of flash */

#if	PHYS_MEMORY_SIZE == 0x20000000
	#define UBOOTPATH		"u-boot-512M-UartB.bin"
#else
	#define UBOOTPATH		"u-boot-aml.bin"
#endif

/*-----------------------------------------------------------------------
 * CFI FLASH driver setup
 */
/* timeout values are in ticks */
#define CONFIG_SYS_FLASH_ERASE_TOUT	(100 * CONFIG_SYS_HZ)
#define CONFIG_SYS_FLASH_WRITE_TOUT	(100 * CONFIG_SYS_HZ)

/* Flash banks JFFS2 should use */
#define CONFIG_SYS_MAX_MTD_BANKS	(CONFIG_SYS_MAX_FLASH_BANKS + \
					CONFIG_SYS_MAX_NAND_DEVICE)
#define CONFIG_SYS_JFFS2_MEM_NAND
/* use flash_info[2] */
#define CONFIG_SYS_JFFS2_FIRST_BANK	CONFIG_SYS_MAX_FLASH_BANKS
#define CONFIG_SYS_JFFS2_NUM_BANKS	1

#ifndef __ASSEMBLY__
extern unsigned int boot_flash_base;
extern volatile unsigned int boot_flash_env_addr;
extern unsigned int boot_flash_off;
extern unsigned int boot_flash_sec;
extern unsigned int boot_flash_type;
#endif

/*----------------------------------------------------------------------------
 * SMSC9115 Ethernet from SMSC9118 family
 *----------------------------------------------------------------------------
 */
#if defined(CONFIG_CMD_NET)


#define CONFIG_AML_ETHERNET
#define CONFIG_HOSTNAME  			arm_m1
#define CONFIG_ETHADDR          	00:01:02:65:04:75	/* Ethernet address */
#define CONFIG_IPADDR				10.28.6.11				/* Our ip address */
#define CONFIG_GATEWAYIP			10.28.6.1			/* Our getway ip address */
#define CONFIG_SERVERIP				10.28.6.92		/* Tftp server ip address */
#define CONFIG_NETMASK				255.255.255.0
#endif /* (CONFIG_CMD_NET) */

/*
 * BOOTP fields
 */

#define CONFIG_BOOTP_SUBNETMASK		0x00000001
#define CONFIG_BOOTP_GATEWAY		0x00000002
#define CONFIG_BOOTP_HOSTNAME		0x00000004
#define CONFIG_BOOTP_BOOTPATH		0x00000010


#define CONFIG_LZO	 1
#define CONFIG_LZMA  1

#define CONFIG_ENABLE_ARC

//#define CONFIG_PLUS_BINARY  board/$(BOARDDIR)/arc.bin
//#define CONFIG_ARC_LOAD_ADDR    0x80000000
#define CONFIG_CACHE_L2X0 1
//#define CONFIG_SYS_NO_DCACHE 1
#define CONFIG_SYS_NO_ICACHE 1
//#define CONFIG_SKIP_LOWLEVEL_INIT 1
//#define CONFIG_MTD_DEBUG 1
//#define CONFIG_MTD_DEBUG_VERBOSE 0
#define CONFIG_AML_SUSPEND 1
#define CONFIG_VOLTAGE_AO12  1
#define CONFIG_VOLTAGE_DDR15 1
#endif /* __CONFIG_H */
