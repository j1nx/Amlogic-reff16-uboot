#ifndef _CONFIG_H_
#define _CONFIG_H_

#define RAM_START       0xD9000000
#define ROMBOOT_START   0xD9040000
#define NOR_START_ADDR  0xCC000000

#define RAM_SIZE        (128*1024 )
#define RAM_END         (RAM_START+RAM_SIZE)
#define MEMORY_LOC      RAM_START

#define ROM_SIZE        (64*1024)
#define ROMBOOT_END     (ROMBOOT_START+ROM_SIZE)


#define GL_DATA_ADR     (RAM_END-256)
#define READ_SIZE       32*1024     // Size for data reading
#define CHECK_SIZE      8*1024      // Size for data checking

/* USB PCD buff */
#define NAND_INFO_BUF       (RAM_END-2*1024)
#define NAND_INFO_BUF_SIZE  (2*1024-256)
#define BSS_SIZE            (30*1024)
#define BSS_START           (RAM_END-32*1024)
#define _STACK_END          (BSS_START+BSS_SIZE)
#define PCD_BUFF            NAND_INFO_BUF
#if 0
#define P_PWR_HOLD_CFG()		(*(volatile unsigned *)(0xc1100000+0x2017*4)&(1<<16))
#define P_PWR_SLEEP_CFG()		(*(volatile unsigned *)(0xc1100000+0x2017*4)&(1<<1))

#define P_AO_GPIO_O_EN_N                  ((volatile unsigned *)0xc8100024)
#else
/*#define ENABLE_M3_HOLDUP 1*/
#endif

#define CONFIG_ENABLE_SECURITY  0
#define CONFIG_TRUSTZONE 1

/*
#if CONFIG_TRUSTZONE
#undef MEMORY_LOC
#define JUMP_ADDR RAM_START
#define MEMORY_LOC RAM_START+READ_SIZE
#endif
*/
#if CONFIG_TRUSTZONE
#define RSA_KEYSIZE 1024
#define RSA_KEY_ROM_ADDR
#define is_secure_disable()		((license_info&LICCENSE_SECUREBOOT)==0)
#endif

#define EFUSE_LOC	EFUSE_EFUSE
#define EFUSE_EFUSE		0
#define EFUSE_MEMORY	1
#define EFUSE_NOR		2
#define EFUSE_SIZE		512
#if EFUSE_LOC==EFUSE_NOR

#else
#undef NOR_EFUSE_SIZE
#define NOR_EFUSE_SIZE 0
#endif
#ifndef __ASSEMBLY__
#define OUT_INIT()      
#define OUT_PUT(a)
#define STIMULUS_V_REG                  (volatile unsigned long *)(RAM_END-12)
#define STIMULUS_DONE                   0xFE000000
#define STIMULUS_FAIL                   0xFF000000

//#define OUTPUT_WORD(a) (*STIMULUS_V_REG)=a
//#define OUTPUT_STR(a)  (*STIMULUS_V_REG)=a
extern void serial_putdword(unsigned);
#define OUTPUT_WORD(a) serial_putdword(a)
extern void serial_puts(const char *);
#define OUTPUT_STR(a)  serial_puts(a)

#endif

#endif
