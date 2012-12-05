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
#if 0
    if(addr[15]!=CONFIG_AML_UBOOT_MAGIC)
        return -1;

    for(i=0;i<size>>1;i++)
        check_sum^=p[i];
#endif
    return 0;
}


STATIC_PREFIX int fw_load_intl(unsigned por_cfg,unsigned target,unsigned size)
{
    int rc=0;
    unsigned * mem;
    switch(POR_GET_INTL_CFG(por_cfg))
    {
        case POR_INTL_SPI:
            mem=(unsigned *)(SPI_MEM_BASE+READ_SIZE);
            spi_init();
            if((rc=check_sum(mem,0,0))==0)
            {
               ipl_memcpy((unsigned*)target,mem,size);
            }
            break;
        case  POR_INTL_NAND:
        case  POR_INTL_NAND_RB:
        case  POR_INTL_NAND_SP:
        case  POR_INTL_NAND_SP_RB:
           rc=nf_read(target,size);
           break;
        case  POR_INTL_SDIO_C:
           rc=sdio_read(target,size,0);
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
    power_on_ext_dev();
    return sdio_init(por_cfg);
}
STATIC_PREFIX int fw_load_extl(unsigned por_cfg,unsigned target,unsigned size)
{
    int rc=sdio_read(target,size,por_cfg);
    if(!rc)
        rc=check_sum((unsigned*)target,magic_info->crc[1],size);
    return rc;
}



