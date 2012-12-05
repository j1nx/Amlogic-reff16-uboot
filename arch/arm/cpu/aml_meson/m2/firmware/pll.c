#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <memtest.h>
#include <config.h>
#include <asm/arch/firm/io.h>
//T_ROM_BOOT_RETURN_INFO * romboot_info=(T_ROM_BOOT_RETURN_INFO *)C_ROM_BOOT_DEBUG;
#ifndef FIRMWARE_IN_ONE_FILE
void pll_initial(struct pll_clk_settings * plls) 
#else
static void pll_initial(struct pll_clk_settings * plls) 
#endif
{
    unsigned demod_pll_cntl3=(unsigned)P_HHI_DEMOD_PLL_CNTL3;
	/*
	*   Sys Pll initial .
	*/
	
	writel(0x65e31ff, P_HHI_SYS_PLL_CNTL2);
	writel(0x1649a941,P_HHI_SYS_PLL_CNTL3);
	writel(plls->sys_pll_cntl,P_HHI_SYS_PLL_CNTL);

	writel(SYS_PLL_RESET,P_RESET5_REGISTER);
	
	// setting bus clock
	writel(// A9 clk set to system clock/2
			(0 << 10) | // 0 - sys_pll_clk, 1 - audio_pll_clk
			(1 << 0 ) | // 1 - sys/audio pll clk, 0 - XTAL
			(1 << 4 ) | // APB_CLK_ENABLE
			(1 << 5 ) | // AT_CLK_ENABLE
			(0 << 2 ) | // div1
			(1 << 7 )   // Connect A9 to the PLL divider output
			,P_HHI_A9_CLK_CNTL);
    // hint for the uboot usage 
	romboot_info->a9_clk = plls->a9_clk;
	/**
	 * clk81 settings From kernel
	 */
    writel(0x65e31ff  ,P_HHI_OTHER_PLL_CNTL2);
    writel(0x1649a941 , P_HHI_OTHER_PLL_CNTL3);
    writel(plls->other_pll_cntl,P_HHI_OTHER_PLL_CNTL);
	
	writel(OTHER_PLL_RESET,P_RESET5_REGISTER);
    
    
    //* Enable Video PLL , unkown why we must enable it . 
    // Just test result . Todo : To know whether we can remove it .
    writel(0x65e31ff  ,P_HHI_VID_PLL_CNTL2);
    writel(0x1649a941 ,P_HHI_VID_PLL_CNTL3);
    writel(0x8210     ,P_HHI_VID_PLL_CNTL);//video pll  power down 
    writel(0x210      ,P_HHI_VID_PLL_CNTL);

	writel(VIDEO_PLL_RESET,P_RESET5_REGISTER);
	
    //enable clk81
	writel(plls->mpeg_clk_cntl,P_HHI_MPEG_CLK_CNTL);
	romboot_info->clk81 = plls->clk81;

}

STATIC_PREFIX void ddr_pll_init(struct ddr_set * ddr_setting) 
{

    writel(0x65e31ff,P_HHI_DDR_PLL_CNTL2);
	writel(0x1649a941,P_HHI_DDR_PLL_CNTL3);

#ifdef CONFIG_AML_DDR_PLL_FINE_TUNE
    if(readl(P_PREG_STICKY_REG0) == 0xa2011){
    	writel(readl(P_PREG_STICKY_REG1),P_HHI_DDR_PLL_CNTL);
    	//writel(0,P_PREG_STICKY_REG0);
    	//writel(0,P_PREG_STICKY_REG1);
	}
	else
#endif
	{
		writel(ddr_setting->ddr_pll_cntl,P_HHI_DDR_PLL_CNTL);
	}  

	writel(DDR_PLL_RESET,P_RESET5_REGISTER);
	
}
//Measure clk

STATIC_PREFIX unsigned long    clk_util_clk_msr(unsigned long   clk_mux)
{
    unsigned long   measured_val;
    unsigned long   uS_gate_time=64;
    writel(0,P_MSR_CLK_REG0);
    // Set the measurement gate to 64uS
    clrsetbits_le32(P_MSR_CLK_REG0,0xffff,(uS_gate_time-1));
    // Disable continuous measurement
    // disable interrupts
    clrbits_le32(P_MSR_CLK_REG0,(1 << 18) | (1 << 17));
//    Wr(MSR_CLK_REG0, (Rd(MSR_CLK_REG0) & ~((1 << 18) | (1 << 17))) );
    clrsetbits_le32(P_MSR_CLK_REG0,
        (0xf << 20)|(1<<19)|(1<<16),
        (clk_mux<<20) |(1<<19)|(1<<16));
    { unsigned long dummy_rd = readl(P_MSR_CLK_REG0); }
    // Wait for the measurement to be done
    while( (readl(P_MSR_CLK_REG0) & (1 << 31)) ) {} 
    // disable measuring
    clrbits_le32(P_MSR_CLK_REG0, 1<<16 );

    measured_val = readl(P_MSR_CLK_REG2);
    // Return value in Hz*measured_val
    // Return Mhz
    return (measured_val>>6);
}
