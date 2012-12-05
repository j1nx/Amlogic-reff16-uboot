#include <config.h>
#include <asm/arch/cpu.h>
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
/*
STATIC_PREFIX void spi_init()
{
    spi_pinmux_init();
    writel(__plls.spi_setting,P_SPI_FLASH_CTRL);
}
*/
#ifndef CONFIG_NAND_AML_M3

#else
#include <nfio.c>
#endif
#include <romboot.c>

void relocate_init(unsigned __TEXT_BASE,unsigned __TEXT_SIZE)
{

	unsigned por_cfg=romboot_info->por_cfg;
	unsigned boot_id=romboot_info->boot_id;
	unsigned size;
	int i;
	unsigned * mem;
	int rc=0;
#ifdef CONFIG_SPL_ENABLE_CACHE
	enable_i_d_cache();
#endif//CONFIG_SPL_ENABLE_CACHE

    size=__TEXT_SIZE;
	if(boot_id>1)
        boot_id=0;
	if(boot_id==0)
    {
        rc=fw_load_intl(por_cfg,__TEXT_BASE,size);
        if(!rc)
        {
            #ifdef TEST_SPL_SPEND_TIME
            { extern int ddr_training_start;
                int read_uboot_end;
                read_uboot_end = TIMERE_GET();
                *((volatile unsigned int*)0x8f900000) = read_uboot_end;
                serial_puts("from inter boot spl finished time:");
                serial_put_dword(read_uboot_end-ddr_training_start);
                serial_puts("\n");
            }
            #else
            *((volatile unsigned int*)0x8f900000) = 0;
            #endif

            return ;
        }
        rc=fw_init_extl(por_cfg);//INTL device  BOOT FAIL
	}
	if(rc==0)
	{
		rc=fw_load_extl(por_cfg,__TEXT_BASE,size);
	}
#if CONFIG_ENABLE_SPL_DEBUG_ROM
    
    while(rc||serial_tstc())
    {
        unsigned char c;
        if(serial_tstc()&&serial_getc()!='e')
            break;
        serial_put_dword(rc);
        debug_rom(__FILE__,__LINE__);
        
#else
   while(rc)
   {
     	
#endif
		if(rc)
            rc=fw_init_extl(por_cfg);//INTL device  BOOT FAIL
        if(rc==0)
            rc=fw_load_extl(por_cfg,__TEXT_BASE,size);
    }
    load_ext(por_cfg,boot_id,__TEXT_BASE);
 //   __asm__ __volatile__("wfi");
#ifdef CONFIG_SPL_ENABLE_CACHE
	disable_i_d_cache();
#endif//CONFIG_SPL_ENABLE_CACHE
    #ifdef TEST_SPL_SPEND_TIME
    { extern int ddr_training_start;
        int read_uboot_end;
        read_uboot_end = TIMERE_GET();
        *((volatile unsigned int*)0x8f900000) = read_uboot_end;
        serial_puts("from ext boot spl finished time:");
        serial_put_dword(read_uboot_end-ddr_training_start);
        serial_puts("\n");
    }
    #else
    *((volatile unsigned int*)0x8f900000) = 0;
    #endif
    return ;
}


