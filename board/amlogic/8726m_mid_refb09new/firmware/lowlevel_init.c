#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <asm/arch/romboot.h>
#include <memtest.h>
#include <config.h>
#include <asm/arch/firm/io.h>

inline void power_hold(void)
{   
	*((unsigned long*)0xc11080b0) &= ~((1<<8) | (1<<12));	//PERIPHS_PIN_MUX_0	clr	bit[8,12]

	*((unsigned long*)0xc11080b8) &= ~((1<<28) | (1<<30));	//PERIPHS_PIN_MUX_2	clr	bit[28,30]

	*((unsigned long*)0xc11080d4) &= ~(1<<23);	//PERIPHS_PIN_MUX_9	clr	bit[23]

	*((unsigned long*)0xc11080e0) &= ~(1<<6);	//PERIPHS_PIN_MUX_12	clr	bit[6]

	*((unsigned long*)0xC1108030) &= ~(1<<12);	//GPIOA8		OEN = 0 (output) 

	*((unsigned long*)0xC1108034) |= 1<<12;	//GPIOA8		Output level = 1 (pull high) 
}

inline void backlight_off(void)
{
	*((unsigned long*)0xc11080b0) &= ~((1<<9) | (1<<22));	//PERIPHS_PIN_MUX_0	clr	bit[9,22]---TCON_CPH2;LCD_VGHL_PWM

	*((unsigned long*)0xc11080b8) &= ~((1<<29) | (1<<31));	//PERIPHS_PIN_MUX_2	clr	bit[29,31]---DEMOD_IF_PWM;PWM_A

	*((unsigned long*)0xc11080d4) &= ~(1<<22);	//PERIPHS_PIN_MUX_9	clr	bit[22]---WIFI Debug

	*((unsigned long*)0xc11080e0) &= ~(1<<7);	//PERIPHS_PIN_MUX_12	clr	bit[7]---ENC_7

	*((unsigned long*)0xC1108034) &= ~(1<<11);	//GPIOA7		Output level = 0 (pull low) 

	*((unsigned long*)0xC1108030) &= ~(1<<11);	//GPIOA7		OEN = 0 (output) 
}

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

	power_hold();
	backlight_off();

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
