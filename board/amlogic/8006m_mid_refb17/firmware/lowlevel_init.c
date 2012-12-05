#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <asm/arch/romboot.h>
#include <memtest.h>
#include <config.h>
#include <asm/arch/firm/io.h>
inline void power_hold(void)
{   
	*((unsigned long*)0xc11080b0) &= ~((1<<9) | (1<<22));	//PERIPHS_PIN_MUX_0	clr	bit[8,12]

	*((unsigned long*)0xc11080b8) &= ~((1<<29) | (1<<31));	//PERIPHS_PIN_MUX_2	clr	bit[28,30]

	*((unsigned long*)0xc11080d4) &= ~(1<<22);	//PERIPHS_PIN_MUX_9	clr	bit[23]

	*((unsigned long*)0xc11080e0) &= ~(1<<7);	//PERIPHS_PIN_MUX_12	clr	bit[6]

	*((unsigned long*)0xC1108030) &= ~(1<<11);	//GPIOA8		OEN = 0 (output) 

	*((unsigned long*)0xC1108034) |= 1<<11;	//GPIOA8		Output level = 1 (pull high) 
}

void lowlevel_init(void* cur,void * target)
{
    int i;
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
	power_hold();

	//writel((1<<22)|100000,P_WATCHDOG_TC);//enable Watchdog 1 seconds
	//Adjust 1us timer base
	WRITE_CBUS_REG_BITS(PREG_CTLREG0_ADDR,CONFIG_CRYSTAL_MHZ,4,5);
	/*
        Select TimerE 1 us base
    */
	clrsetbits_le32(P_ISA_TIMER_MUX,0x7<<8,0x1<<8);
    memory_pll_init(0,NULL);
	serial_put_dword(get_timer(0));
	serial_put_dword(readl(0xc1100000+0x200b*4));
#if CONFIG_ENABLE_SPL_DEBUG_ROM
    __udelay(100000);//wait for a uart input 
	if(serial_tstc()){
		writel(0,P_WATCHDOG_TC);//disable Watchdog	  
	    debug_rom(__FILE__,__LINE__);
		}
#else
    __udelay(1000);//delay 1 ms , wait pll ready	
#endif	

#if 1
#if CONFIG_ENABLE_SPL_DEBUG_ROM	
	if(ddr_init_test()){
		writel(0,P_WATCHDOG_TC);//disable Watchdog	  
	    debug_rom(__FILE__,__LINE__);
		}
#else

    do{
    }while(ddr_init_test());
#endif	    
#endif
	writel(0,P_WATCHDOG_TC);//disable Watchdog    
	serial_puts("Systemp Started\n");
}
