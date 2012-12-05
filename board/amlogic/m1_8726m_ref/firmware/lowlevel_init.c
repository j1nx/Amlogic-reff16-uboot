#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <asm/arch/romboot.h>
#include <memtest.h>
#include <config.h>
#include <asm/arch/firm/io.h>

void lowlevel_init(void* cur,void * target)
{
#if 0    
	if(cur != target) //r0!=r1
	{
	    //running from spi
    	// take me as a spi rom boot mode
		romboot_info->por_cfg = POR_INTL_SPI |
		    (READ_CBUS_REG(ASSIST_POR_CONFIG)&(~POR_INTL_CFG_MASK));
		romboot_info->boot_id = 0;//boot from spi
		/// Release pull up registers .
	}
#endif	

	//changed by Elvis, add uart rx pull up
	//pull up LINUX_RX(B15--->GPIOE_19) reg (7422y v1.pdf)
	//GPIOE_19(M1-Apps v25- 2010-07-19-BGA372_297-6.xls serach B15)
	WRITE_CBUS_REG( PAD_PULL_UP_REG3, READ_CBUS_REG(PAD_PULL_UP_REG3) & ~(1<<2) );	// Meson-pull-up-down_table.xlsx ( GPIOE_19 is PAD_PULL_UP_REG3---0x203e 2bits)
	writel((1<<22)|100000,P_WATCHDOG_TC);//enable Watchdog 1 seconds
	//Adjust 1us timer base
	WRITE_CBUS_REG_BITS(PREG_CTLREG0_ADDR,CONFIG_CRYSTAL_MHZ,4,5);
	/*
        Select TimerE 1 us base
    	*/
	clrsetbits_le32(P_ISA_TIMER_MUX,0x7<<8,0x1<<8);
    	memory_pll_init(0,NULL);
	serial_put_dword(get_timer(0));
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
	unsigned por_cfg;
#if CONFIG_ENABLE_SPL_DEBUG_ROM	
	if(ddr_init_test())
	    debug_rom(__FILE__,__LINE__);
#else
    do{
    }while(ddr_init_test());

#endif	    
	writel(0,P_WATCHDOG_TC);//disable Watchdog    
	serial_puts("Systemp Started\n");
}
