/*

 *
 */

#ifndef _CPU_H
#define _CPU_H

#define CONFIG_AML_MESION_1         1
/**
    M1 default Settings
    We should support SD/SDHC/MMC , SPI , NAND as defautly
*/
/** For NAND and SPI , we should support MTD device **/
#define CONFIG_MTD_DEVICE     1
#define CONFIG_MTD_PARTITIONS 1
#define CONFIG_CMD_MTDPARTS   1

/** Support SPI Flash **/

#define CONFIG_CMD_SF               1 //@todo move it to Config ?
#define SPI_FLASH_CACHELINE         64 //amlogic special setting. in M1 , SPI_A for SPI flash
#define CONFIG_AMLOGIC_SPI_FLASH    1
#define CONFIG_SPI_FLASH            1
//#define CONFIG_SPI_FLASH_ATMEL      1
#define CONFIG_SPI_FLASH_MACRONIX   1
//#define CONFIG_SPI_FLASH_SPANSION   1
#define CONFIG_SPI_FLASH_SST        1
//#define CONFIG_SPI_FLASH_STMICRO    1
#define CONFIG_SPI_FLASH_WINBOND    1
#define CONFIG_SPI_FLASH_EON        1
/** support nand flash **/
#define CONFIG_CMD_NAND       		1 //@todo move it to Config ?
#define CONFIG_NAND_AML             1
#define CONFIG_NAND_AML_M1          1
#define CONFIG_SYS_MAX_NAND_DEVICE	1		/* Max number of */
#define CONFIG_SYS_NAND_MAX_CHIPS	4

#ifndef CONFIG_SYS_NAND_BASE_LIST
#define CONFIG_SYS_NAND_BASE_LIST   {NFC_BASE}
#endif

#ifndef CONFIG_AMLROM_U_BOOT
#define CONFIG_AMLROM_U_BOOT 1
#elif CONFIG_AMLROM_U_BOOT!=1
#undef CONFIG_AMLROM_U_BOOT
#define CONFIG_AMLROM_U_BOOT 1
#endif

#if  CONFIG_AMLROM_U_BOOT
#define CONFIG_AML_BOOT_BIN         1
//support romboot mode
#define CONFIG_NAND_SP_BLOCK_SIZE 32
#define CONFIG_NAND_PAGES         1024
#define CONFIG_MMC					1
#define CONFIG_GENERIC_MMC			1
#define CONFIG_CMD_MMC				1
#define CONFIG_AML_SDIO				1

#include "romboot.h"
#elif CONFIG_AMLROM_U_BOOT==0

#endif



#endif /* _CPU_H */
