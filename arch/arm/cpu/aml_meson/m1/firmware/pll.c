/*
 *  This file is for M1 Only . If you want to implement your pll initial function 
 *  please copy this file into the arch/$(ARCH)/$(CPU)/$SOC/firmware directory .
 */



#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <memtest.h>
#include <config.h>
#include <asm/arch/firm/io.h>

#ifndef FIRMWARE_IN_ONE_FILE
void pll_initial(struct pll_clk_settings * plls) 
#else
static void pll_initial(struct pll_clk_settings * plls) 
#endif
{
    unsigned demod_pll_cntl3=(unsigned)P_HHI_DEMOD_PLL_CNTL3;

	writel(plls->sys_pll_cntl,P_HHI_SYS_PLL_CNTL);
	writel(// A9 clk set to system clock/2
			(0 << 10) | // 0 - sys_pll_clk, 1 - audio_pll_clk
			(1 << 0 ) | // 1 - sys/audio pll clk, 0 - XTAL
			(1 << 4 ) | // APB_CLK_ENABLE
			(1 << 5 ) | // AT_CLK_ENABLE
			(0 << 2 ) | // div1
			(1 << 7 )   // Connect A9 to the PLL divider output
			,P_HHI_A9_CLK_CNTL);
     
    
	romboot_info->a9_clk = plls->a9_clk;
#ifdef HARDWARE_MODE
	romboot_info->clk81 = plls->clk81;

	writel(plls->spi_setting,P_SPI_FLASH_CLOCK);
#endif	 
	/**
	 * clk81 settings From kernel
	 */
    writel(plls->other_pll_cntl,P_HHI_OTHER_PLL_CNTL);
	writel(plls->mpeg_clk_cntl,P_HHI_MPEG_CLK_CNTL);
#ifndef HARDWARE_MODE
	romboot_info->clk81 = plls->clk81;
#endif
	/*
	Demod PLL,1.2G for SATA/Ethernet
	*/
	writel(plls->demod_pll400m_cntl,P_HHI_DEMOD_PLL_CNTL);
	/*Demod PLL,400M,300M,240M*/
	
	writew(0x0c85,demod_pll_cntl3+2);

}
STATIC_PREFIX void ddr_pll_init(struct ddr_set * ddr_setting) 
{
    writel(ddr_setting->ddr_pll_cntl,P_HHI_DDR_PLL_CNTL);    
}
//Measure clk
#define clk_util_clk_msr(clk) clk_util_msr(clk,64)
unsigned    clk_util_msr( unsigned  clk_mux,unsigned time )
{

	writel(0,P_MSR_CLK_REG0);
    // Set the measurement gate to 64uS
	clrsetbits_le32(P_MSR_CLK_REG0,0xffff,time-1);
    // Disable continuous measurement
    // disable interrupts
    clrbits_le32(P_MSR_CLK_REG0,((1 << 18) | (1 << 17)));
	clrsetbits_le32(P_MSR_CLK_REG0,(0xf<<20),(clk_mux<<20)|(1<<19)|(1<<16));

	{ unsigned long dummy_rd = readl(P_MSR_CLK_REG0); }
    // Wait for the measurement to be done
    while( readl(P_MSR_CLK_REG0) & (1 << 31))  {};
    // disable measuring
	clrbits_le32(P_MSR_CLK_REG0,(1 << 16));
    // Return value in Hz*measured_val
    return (readl(P_MSR_CLK_REG2)>>6);
}
//
//STATIC_PREFIX unsigned long    clk_util_clk_msr(unsigned long   clk_mux)
//{
//    unsigned long   measured_val;
//    unsigned long   uS_gate_time=64;
//    writel(0,P_MSR_CLK_REG0);
//    // Set the measurement gate to 64uS
//    clrsetbits_le32(P_MSR_CLK_REG0,0xffff,(uS_gate_time-1));
//    // Disable continuous measurement
//    // disable interrupts
//    clrbits_le32(P_MSR_CLK_REG0,(1 << 18) | (1 << 17));
////    Wr(MSR_CLK_REG0, (Rd(MSR_CLK_REG0) & ~((1 << 18) | (1 << 17))) );
//    clrsetbits_le32(P_MSR_CLK_REG0,
//        (0xf << 20)|(1<<19)|(1<<16),
//        (clk_mux<<20) |(1<<19)|(1<<16));
//    { unsigned long dummy_rd = readl(P_MSR_CLK_REG0); }
//    // Wait for the measurement to be done
//    while( (readl(P_MSR_CLK_REG0) & (1 << 31)) ) {} 
//    // disable measuring
//    clrbits_le32(P_MSR_CLK_REG0, 1<<16 );
//
//    measured_val = readl(P_MSR_CLK_REG2);
//    // Return value in Hz*measured_val
//    // Return Mhz
//    return (measured_val>>6);
//}

STATIC_PREFIX void pll_clk_list(void)
{


    unsigned long   clk_freq;
    unsigned char clk_list[]={3,10,11};
    char *clk_list_name[]={"arm","ddr","other"};
    unsigned long  i;
	for(i=0;i<3;i++)
	{
		clk_freq = clk_util_clk_msr(clk_list[i]     // unsigned long   clk_mux,             // Measure A9 clock
								);   // unsigned long   uS_gate_time )       // 50us gate time
		serial_puts(clk_list_name[i]);
		serial_puts("_clk=");
		serial_put_dword(clk_freq);
		serial_puts("\n");
	}
}
#define CONFIG_AML_CLK_LIST_ENABLE 1