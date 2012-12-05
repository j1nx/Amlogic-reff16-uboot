#include <config.h>
#include <asm/arch/romboot.h>
#include <asm/arch/firm/timing.h>
#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/io.h>
#include <asm/arch/firm/nand.h>
#ifndef FIRMWARE_IN_ONE_FILE
#define STATIC_PREFIX 
#else
#define STATIC_PREFIX static inline
#endif
#ifndef CONFIG_AML_UBOOT_MAGIC
#define CONFIG_AML_UBOOT_MAGIC 0x12345678
#endif
STATIC_PREFIX short check_sum(unsigned * addr,unsigned short check_sum,unsigned size)
{
    int i;
    unsigned short * p=(unsigned short *)addr;
    serial_put_dword(addr[15]);
    if(addr[15]!=CONFIG_AML_UBOOT_MAGIC)
        return -1;
#if 0        
    for(i=0;i<size>>1;i++)
        check_sum^=p[i];
#endif        
    return 0;
}

#ifdef CONFIG_BOARD_8726M_ARM
#define P_PREG_JTAG_GPIO_ADDR  (volatile unsigned long *)0xc110802c
#endif

STATIC_PREFIX int fw_load_intl(unsigned por_cfg,unsigned target, unsigned offset, unsigned size)
{
    int rc=0;
    unsigned * mem;
#ifdef CONFIG_BOARD_8726M_ARM
	//#error aaa
    //do that for test_n cause our chip into test mode
    (*P_PREG_JTAG_GPIO_ADDR)&= ~((1<<20)   /* test_n_gpio_o */
                               |(1<<16));  /* test_n_gpio_en_n */
#endif

    switch(por_cfg&POR_INTL_CFG_MASK)
    {
        case POR_INTL_SPI:
            mem=(unsigned *)(SPI_MEM_BASE + offset);
            spi_init();
            if((rc=check_sum(mem,0,0))==0)
            {
            	serial_puts("Boot From SPI");
							ipl_memcpy((unsigned*)target,mem,size);
            }
            break;
        case  POR_INTL_NAND_LP:
           rc=nf_lp_read(target,offset,size);
           break;
        case  POR_INTL_NAND_SP:
           rc=nf_sp_read(target,offset,size);
           break;
        case  POR_INTL_SDIO_B1:
			rc=sdio_read(target, offset, size, 0);
           break;
        default:
           return 1;
    }
    if(rc==0)
        rc=check_sum((unsigned*)target,magic_info->crc[1],size);
    return rc;
}
STATIC_PREFIX int fw_init_extl(unsigned por_cfg)
{
	int rc=sdio_init(por_cfg);
	serial_put_dword(rc);
    serial_puts("sdio init\n");
    
    return rc;
}
STATIC_PREFIX int fw_load_extl(unsigned por_cfg,unsigned target, unsigned offset, unsigned size)
{
    int rc=sdio_read(target, offset, size, por_cfg);
    serial_put_dword(rc);
    serial_puts("sdio read 2\n");
    if(!rc)
        rc=check_sum((unsigned*)target,magic_info->crc[1],size);
    return rc;
}


