#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <asm/arch/romboot.h>
#include <memtest.h>
#include <config.h>
#include <asm/arch/firm/io.h>

void lowlevel_init(void* cur,void * target)
{
    int i;
    
    if(POR_GET_CRYSTAL(romboot_info->por_cfg)==POR_CRYSTAL_24M)
    {
        WRITE_CBUS_REG_BITS(PREG_CTLREG0_ADDR,24,4,5);
    }
    else
        WRITE_CBUS_REG_BITS(PREG_CTLREG0_ADDR,25,4,5);
  //  writel(0,P_WATCHDOG_TC);
     writel((1<<22)|300000,P_WATCHDOG_TC);
	/*
        Select TimerE 1 us base
    */
	clrsetbits_le32(P_ISA_TIMER_MUX,0x7<<8,0x1<<8);
    
    memory_pll_init(0,NULL);
    enable_cache();
#if CONFIG_ENABLE_SPL_DEBUG_ROM
    __udelay(100000);//wait for a uart input
	if(serial_tstc())
	{
	    debug_rom(__FILE__,__LINE__);
	}
#else
    __udelay(1000);//delay 1 ms , wait pll ready
#endif
//	writel((0<<22)|1000000,P_WATCHDOG_TC);//enable Watchdog
//    writel((0<<22)|300000,P_WATCHDOG_TC);
	unsigned por_cfg;
#if CONFIG_ENABLE_SPL_DEBUG_ROM
	if(ddr_init_test())
	    debug_rom(__FILE__,__LINE__);
#else
    do{
    }while(ddr_init_test());
    
#endif
    writel((0<<22)|300000,P_WATCHDOG_TC);
    disable_cache();
   	serial_puts("Systemp Started\n");
}
