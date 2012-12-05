#include <config.h>
#include <asm/arch/romboot.h>
#include <asm/arch/firm/timing.h>
#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/io.h>
#include <asm/arch/firm/nand.h>
#include <asm/arch/firm/config.h>

STATIC_PREFIX void spi_init(void)
{
    spi_pinmux_init();
    writel(__plls.spi_setting,P_SPI_FLASH_CTRL);
}
#include <nfio.c>
#include <romboot.c>
void relocate_init(unsigned __TEXT_BASE,unsigned __TEXT_SIZE)
{
 
	unsigned por_cfg=romboot_info->por_cfg;
	unsigned boot_id=romboot_info->boot_id;
	unsigned size;
	int i;
	unsigned * mem;
	int rc=0;
    //int test_flag=1;
    //while(test_flag);

	size=__TEXT_SIZE;
	if(boot_id>1)
        boot_id=0;
	if(boot_id==0)
    {
        rc=fw_load_intl(por_cfg,__TEXT_BASE,size);
        if(!rc)
            return ;
        //rc=fw_init_extl(por_cfg);//INTL device  BOOT FAIL
	}
	
#if CONFIG_ENABLE_SPL_DEBUG_ROM	
    
    rc=fw_init_extl(por_cfg);//INTL device  BOOT FAIL
    if(rc==0)
	{
		rc=fw_load_extl(por_cfg,__TEXT_BASE,size);
	}
	while(rc||serial_tstc())
    {
        serial_put_dword(rc);
        debug_rom(__FILE__,__LINE__);
        if(rc)
            rc=fw_init_extl(por_cfg);//INTL device  BOOT FAIL
        if(rc==0)
            rc=fw_load_extl(por_cfg,__TEXT_BASE,size);
    }
#else
     rc = 1;
     while(rc) //don't need judge condition;if intl device boot fail,then boot from ext device
     {
        fw_init_extl(por_cfg);
        rc=fw_load_extl(por_cfg,__TEXT_BASE,size);
        if(rc)
        {
            debug_rom(__FILE__,__LINE__);
        }
     }
#endif    
    return ;
}


