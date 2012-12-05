/*
 * (C) Copyright 2006-2010
 * Amlogic .
 * Author :
 *	Jerry Yu <jerry.yu@amlogic.com>
 */

#ifndef __CONFIG_H
#define __CONFIG_H
/*
 * High Level Configuration Options
 */
#define CONFIG_ARC700	   1	 /* This is an ARM V7 CPU core */
#define CONFIG_MESON		   1	 /* in a Amlogic Meson core */
#define CONFIG_MESON1		   1	 /* which is meson */

#define CONFIG_AMLROM_U_BOOT	   1    /**
										Meson support Romboot mode
								    */

#define CONFIG_SERIAL_NONE  1
 
/* gadmei uboot display config*/
#define CONFIG_A3_I2C  1  //gadmei power ic use i2c 
#define CONFIG_A3_DISPLAY 1//gadmei uboot lcd display

//#define POWER_AXP209  1   //add AXP209 support
//#define TTL_LCD_CONFIG 1  //add TTL lcd support
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
#undef  CONFIG_USE_IRQ			   /* no support for IRQs */
#define CONFIG_MISC_INIT_R

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1
//#define CONFIG_CMD_NET			1
//#define CONFIG_NET_MULTI        1
//#define CONFIG_CMD_PING
//#define CONFIG_CMD_DHCP

/*
 * Size of malloc() pool
 */
						/* Sector */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (1 << 19))
#define CONFIG_SYS_GBL_DATA_SIZE	128	/* bytes reserved for */
						/* initial data */
/*
 * Hardware drivers
 */



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



/* commands to include */
#include <config_cmd_default.h>
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_NFS
#define CONFIG_CMD_EXT2		/* EXT2 Support			*/
#define CONFIG_CMD_FAT		/* FAT support			*/
//#define CONFIG_CMD_JFFS2	/* JFFS2 Support		*/
//
//#define CONFIG_CMD_I2C		/* I2C serial bus support	*/
//#define CONFIG_CMD_MMC		/* MMC support			*/
//#define CONFIG_CMD_ONENAND	/* ONENAND support		*/

//
#undef CONFIG_CMD_ONENAND
#undef CONFIG_CMD_FLASH		/* flinfo, erase, protect	*/
#undef CONFIG_CMD_FPGA		/* FPGA configuration Support	*/
#undef CONFIG_CMD_IMI		/* iminfo			*/
#undef CONFIG_CMD_IMLS		/* List all found images	*/

#define CONFIG_SYS_NO_FLASH 1

/*
 * TWL4030
 */
//#define CONFIG_TWL4030_POWER		1


							/* NAND devices */
#define CONFIG_JFFS2_NAND
/* nand device jffs2 lives on */
#define CONFIG_JFFS2_DEV		"nand0"
/* start of jffs2 partition */
#define CONFIG_JFFS2_PART_OFFSET	0x680000
#define CONFIG_JFFS2_PART_SIZE		0xf980000	/* sz of jffs2 part */

/* Environment information */
#define CONFIG_BOOTDELAY	0

#define CONFIG_BOOTFILE		uImage
/*rgb_data_addr/bmp_load_addr and so on also defined in lcd_init_gadmei.c*/
#define CONFIG_EXTRA_ENV_SETTINGS \
	"rgb_data_addr=0x84300000\0" \
 	"bmp_load_addr=0x84900000\0"  \
	"bmp_in_nand_addr=0x4000000\0" \
 	"bmp_nand_read_size=0x500000\0" \
	"loadaddr=0x82000000\0" \
	"usbtty=cdc_acm\0" \
	"console=ttyS2,115200n8\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"root=/dev/mmcblk0p2 rw " \
		"rootfstype=ext3 rootwait\0" \
	"nandargs=setenv bootargs console=${console} " \
		"root=/dev/mtdblock4 rw " \
		"rootfstype=jffs2\0" \
	"loadbootscript=fatload mmc 0 ${loadaddr} boot.scr\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source ${loadaddr}\0" \
	"loaduimage=fatload mmc 0 ${loadaddr} uImage\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"bootm ${loadaddr}\0" \
	"nandboot=echo Booting from nand ...; " \
		"run nandargs; " \
		"onenand read ${loadaddr} 280000 400000; " \
		"bootm ${loadaddr}\0" \
		"boardname=m1_dpf\0" \
		"chipname=6236m\0" \
		"machid=2951\0" \
    "bootargs=root=254:1 rw init=/linuxrc console=ttyS1,115200 rootfstype=ext2\0"
#if 1
#define CONFIG_BOOTCOMMAND \
	"nand read 82000000 0x800000 0x800000; bootm 82000000" 
#endif

#if 0
#define CONFIG_BOOTCOMMAND \
	"nand read 82000000 0x800000 0x800000; bootm 82000000" 			//nand read avos code cmd
#endif 

#if 0
#define CONFIG_BOOTCOMMAND \
	"mmcinfo;fatload mmc 0 81000000 u-boot-aml.bin.ro; nand rom_protect off;nand rom_write 0x81000000 0 60000;"	
#endif

#if 0
#define CONFIG_BOOTCOMMAND \
	"mmcinfo;fatload mmc 0 81000000 u-boot-aml.bin.ro; nand rom_protect off;nand rom_write 0x81000000 0 60000;fatload mmc 0 82000000 app_A3.uboot.img;nand device 0; nand erase 0x800000 0x800000; nand wr_rd_cmp 0x82000000 0x83000000 0x800000 0x800000; bootm 0x83000000" 
#endif

#if 0
#define CONFIG_BOOTCOMMAND \
	"mmcinfo;fatload mmc 0 81000000 u-boot-aml.bin.ro; nand rom_protect off;nand rom_write 0x81000000 0 60000;nand device 0; nand erase 0x800000 0x6000000;fatload mmc 0 84000000 fs.bin;nand wr_rd_cmp 0x84000000 0x80000000 0x01800000 0x02400000;fatload mmc 0 82000000 app_A3.uboot.img;nand wr_rd_cmp 0x82000000 0x83000000 0x800000 0x800000; bootm 0x83000000" 
#endif

#if 0
#define CONFIG_BOOTCOMMAND \
	"mmcinfo;fatload mmc 0 82000000 app_A3.uboot.img;nand read 84000000 0x800000 0x800000;cmp.b 0x82000000 0x84000000 0x5c1018;" 
#endif

#if 0
#define CONFIG_BOOTCOMMAND \
	"mmcinfo;fatload mmc 0 82000000 app_A3.uboot.img;nand erase 0x800000 0x100000;nand wr_rd_cmp 82000000 0x83000000 0x800000  0x100000" 
#endif

#define CONFIG_AUTO_COMPLETE	1
/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_PROMPT		"MESON_DPF # "
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
#define CONFIG_HARDWARE_FIN     2400000 // 25Mhz crystal .
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
#define PHYS_MEMORY_SIZE     0x08000000 // 128M
#define CONFIG_SYS_MEMTEST_START	0x80000000	/* memtest works on	*/      
#define CONFIG_SYS_MEMTEST_END		0x07000000	/* 0 ... 120 MB in DRAM	*/  

/* SDRAM Bank Allocation method */
#define SDRC_R_B_C		1

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */


#define CONFIG_SYS_MAX_FLASH_SECT	520	/* max number of sectors */
						/* on one chip */
#define CONFIG_SYS_MAX_FLASH_BANKS	2	/* max number of flash banks */
#define CONFIG_SYS_MONITOR_LEN		(256 << 10)	/* Reserve 2 sectors */

#define CONFIG_SYS_FLASH_BASE		boot_flash_base

/* Monitor at start of flash */
#define CONFIG_SPI_BOOT 1

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
	#define CONFIG_ENV_OFFSET       0x800000
	#define CONFIG_ENV_BLOCK_NUM    4
#else
#define CONFIG_SYS_ENV_IS_NOWHERE 1
#endif



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
#define CONFIG_IPADDR				10.18.9.97				/* Our ip address */
#define CONFIG_GATEWAYIP			10.18.9.1			/* Our getway ip address */
#define CONFIG_SERVERIP				10.18.9.113		/* Tftp server ip address */
#define CONFIG_NETMASK				255.255.255.0
#endif /* (CONFIG_CMD_NET) */

/*
 * BOOTP fields
 */

#define CONFIG_BOOTP_SUBNETMASK		0x00000001
#define CONFIG_BOOTP_GATEWAY		0x00000002
#define CONFIG_BOOTP_HOSTNAME		0x00000004
#define CONFIG_BOOTP_BOOTPATH		0x00000010

#define CONFIG_AMLROM_NANDBOOT          

//#define CONFIG_LZO	 1
#define CONFIG_LZMA  1
#define CONFIG_BOOTM_AVOS 1
#endif /* __CONFIG_H */
