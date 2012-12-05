#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <asm/arch/romboot.h>
#include <memtest.h>
#include <config.h>
#include <asm/arch/firm/io.h>

int chip_reset(void)
{
#ifdef AML_BOOT_SPI  	
	/* if uboot is on flash then boot agian from flash */
	writel(WATCHDOG_ENABLE_MASK, IREG_WATCHDOG_CONTROL0);
#endif	
	WRITE_CBUS_REG(WATCHDOG_RESET, 0);
    WRITE_CBUS_REG(WATCHDOG_TC, 1 << 22 | 100); /*100*10uS=1ms*/
    WRITE_CBUS_REG(WATCHDOG_RESET, 0);
    __udelay(10000);/**/
    serial_puts("Chip watchdog reset error!!!please reset it by hardware\n");
    while (1) ;
	return 0;
}


void cooling(void)
{
	int i;
	writel(0,P_WATCHDOG_TC);//disable Watchdog
	//GPIOX_53 reset chip power ctrl

	clrbits_le32(P_PREG_FGPIO_O, 1<<21);
	clrbits_le32(P_PREG_FGPIO_EN_N, 1<<21);
	for(i=0; i<800; i++)
	{
		__udelay(1000);
	}
	//vcc_12v/24v power down GPIOX_70
	clrbits_le32(P_PREG_GGPIO_O, 1<<6);
	clrbits_le32(P_PREG_GGPIO_EN_N, 1<<6);

	setbits_le32(P_PERIPHS_PIN_MUX_2,((1<<29)|(1<<30)));
	writel(0x18003033, P_UART1_CONTROL);

	serial_puts("\nstandby...\n");

	writel(0x209861f1, P_HHI_GCLK_MPEG0);
	writel(0x208b8028, P_HHI_GCLK_MPEG1);
	writel(0xfffffc07, P_HHI_GCLK_MPEG2);
	writel(0xffc40021, P_HHI_GCLK_OTHER);

	//analog off
	WRITE_CBUS_REG(SAR_ADC_REG3, 0x3008510a);
	//WRITE_CBUS_REG(VGHL_PWM_REG0, 0x0); //the same with off
	//WRITE_CBUS_REG(WIFI_ADC_SAMPLING, 0x0); //the same with off
	WRITE_APB_REG(ADC_EN_ADC, 0x0); //the same with off
	//WRITE_AHB_REG(WIFI_ADC_DAC, 0x0); //the same with off
	//WRITE_AHB_REG(ADC_EN_CMLGEN_RES, 0x0); //the same with off
	//WRITE_AHB_REG(WIFI_SARADC, 0x0); //the same with off
	//usb off
	WRITE_CBUS_REG(PREI_USB_PHY_REG, 0x8040012b);

	//clock off
	//WRITE_CBUS_REG(HHI_DEMOD_CLK_CNTL, 0x400); //the same with off
	//WRITE_CBUS_REG(HHI_SATA_CLK_CNTL, 0x0); //the same with off
	//WRITE_CBUS_REG(HHI_ETH_CLK_CNTL, 0x207); //the same with off
	//WRITE_CBUS_REG(HHI_WIFI_CLK_CNTL, 0x0); //the same with off
	WRITE_CBUS_REG(HHI_VID_CLK_CNTL, 0x840e);
	WRITE_CBUS_REG(HHI_AUD_CLK_CNTL, 0x800018);
	WRITE_CBUS_REG(HHI_MALI_CLK_CNTL, 0x202);
	WRITE_CBUS_REG(HHI_HDMI_CLK_CNTL, 0x203);
	WRITE_CBUS_REG(HHI_MPEG_CLK_CNTL, 0x1083);

	//pll off
	WRITE_CBUS_REG(HHI_DEMOD_PLL_CNTL, 0x8232);
	WRITE_CBUS_REG(HHI_VID_PLL_CNTL, 0x8641);
	WRITE_CBUS_REG(HHI_AUD_PLL_CNTL, 0xca80);
	WRITE_CBUS_REG(HHI_OTHER_PLL_CNTL, 0x887d);

#ifdef SYSTEM_16K
	if (READ_CBUS_REG(HHI_MPEG_CLK_CNTL)&(1<<8))
		CLEAR_CBUS_REG_MASK(HHI_MPEG_CLK_CNTL, (1<<8)); // clk81 = xtal
	SET_CBUS_REG_MASK(HHI_MPEG_CLK_CNTL, (1<<9));		// xtal_rtc = rtc
	WRITE_CBUS_REG_BITS(HHI_MPEG_CLK_CNTL, 0x1, 0, 6);	// devider = 2
	WRITE_CBUS_REG_BITS(HHI_MPEG_CLK_CNTL, 0, 12, 2);	// clk81 src -> xtal_rtc
	SET_CBUS_REG_MASK(HHI_MPEG_CLK_CNTL, (1<<8));		// clk81 = xtal_rtc / devider
#else
	CLEAR_CBUS_REG_MASK(HHI_MPEG_CLK_CNTL, (1<<8)); // clk81 = xtal
	WRITE_CBUS_REG_BITS(HHI_MPEG_CLK_CNTL, 0x1e, 0, 6); // devider = 30
	WRITE_CBUS_REG_BITS(HHI_MPEG_CLK_CNTL, 0, 12, 2);	// clk81 src -> xtal_rtc
	SET_CBUS_REG_MASK(HHI_MPEG_CLK_CNTL, (1<<8));		// clk81 = xtal_rtc / devider
#endif
	CLEAR_CBUS_REG_MASK(HHI_A9_CLK_CNTL, (1<<7));		// clka9 = xtal_rtc / 2
#ifdef SYSTEM_16K
	SET_CBUS_REG_MASK(PREG_CTLREG0_ADDR, 1);
#endif
	WRITE_CBUS_REG(HHI_A9_AUTO_CLK0,
		(2 << 24)	 |	 // sleep select 1000uS timebase
		(0x20 << 16)	  |   // Set the delay wakeup time (32mS)
		(0 << 5)		|	// don't clear the FIQ global mask
		(0 << 4)		|	// don't clear the IRQ global mask
		(2 << 2));				  // Set interrupt wakeup only
	WRITE_CBUS_REG(HHI_A9_AUTO_CLK1,
		(0 << 20)				|	// start delay timebase
		(1 << 12)	 |	 // 1uS enable delay
		(1 << 8)   |   // 1uS gate delay
		(1 << 0));		   // 1us start delay
	SET_CBUS_REG_MASK(HHI_A9_AUTO_CLK0, 1 << 0);
	SET_CBUS_REG_MASK(HHI_SYS_PLL_CNTL, (1<<15));		// turn off sys pll
	
	while(1)
	{
		if(serial_tstc())	break;
	}
	//vcc_12v/24v power on
	setbits_le32(P_PREG_GGPIO_EN_N, 1<<6);
	for(i=0; i<800; i++)
	{
		__udelay(1000);
	}
	//GPIOX_53 reset chip power ctrl
	setbits_le32(P_PREG_FGPIO_O, 1<<21);

	memory_pll_init(0,NULL);

	serial_puts("\ngate clock on...\n");


	writel(0xffffffff, P_HHI_GCLK_MPEG0);
	writel(0xffffffff, P_HHI_GCLK_MPEG1);
	writel(0xffffffff, P_HHI_GCLK_MPEG2);
	writel(0xffffffff, P_HHI_GCLK_OTHER);
#if 0
	//analog on
	WRITE_CBUS_REG(SAR_ADC_REG3, 0x2008510a);
	//WRITE_CBUS_REG(VGHL_PWM_REG0, 0x0);	//the same with off
	//WRITE_CBUS_REG(WIFI_ADC_SAMPLING, 0x0); //the same with off
	//WRITE_APB_REG(ADC_EN_ADC, 0x0); //the same with off
	//WRITE_AHB_REG(WIFI_ADC_DAC, 0x0); //the same with off
	//WRITE_AHB_REG(ADC_EN_CMLGEN_RES, 0x0); //the same with off
	//WRITE_AHB_REG(WIFI_SARADC, 0x0); //the same with off
	//usb on
	WRITE_CBUS_REG(PREI_USB_PHY_REG, 0x80400128);

	//clock on
	//WRITE_CBUS_REG(HHI_DEMOD_CLK_CNTL, 0x400); //the same with off
	//WRITE_CBUS_REG(HHI_SATA_CLK_CNTL, 0x0); //the same with off
	//WRITE_CBUS_REG(HHI_ETH_CLK_CNTL, 0x207); //the same with off
	//WRITE_CBUS_REG(HHI_WIFI_CLK_CNTL, 0x0); //the same with off
	WRITE_CBUS_REG(HHI_VID_CLK_CNTL, 0x840f);
	WRITE_CBUS_REG(HHI_AUD_CLK_CNTL, 0x800018);
	WRITE_CBUS_REG(HHI_MALI_CLK_CNTL, 0x302);
	WRITE_CBUS_REG(HHI_HDMI_CLK_CNTL, 0x303);
	WRITE_CBUS_REG(HHI_MPEG_CLK_CNTL, 0x1183);
	//pll on
	WRITE_CBUS_REG(HHI_DEMOD_PLL_CNTL, 0x232);
	WRITE_CBUS_REG(HHI_VID_PLL_CNTL, 0x641);
	WRITE_CBUS_REG(HHI_AUD_PLL_CNTL, 0x4a80);
	//WRITE_CBUS_REG(HHI_OTHER_PLL_CNTL, 0x87d);
#endif

	
	return 0;
}


void freezing_main(void)
{
    int i;
	//writel((1<<22)|100000,P_WATCHDOG_TC);//enable Watchdog 1 seconds
	//Adjust 1us timer base
	WRITE_CBUS_REG_BITS(PREG_CTLREG0_ADDR,CONFIG_CRYSTAL_MHZ,4,5);
	/*
        Select TimerE 1 us base
    */
	clrsetbits_le32(P_ISA_TIMER_MUX,0x7<<8,0x1<<8);

	cooling();

	chip_reset();
}
