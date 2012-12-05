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
#include <asm/arcregs.h>
static void __udelay(unsigned long usec);
#ifndef FIRMWARE_IN_ONE_FILE
void pll_initial(struct pll_clk_settings * plls) 
#else
static void pll_initial(struct pll_clk_settings * plls) 
#endif
{
	
	romboot_info->a9_clk = plls->a9_clk;
	/**
	 * clk81 settings From kernel
	 */
	writel(1<<12
			,P_HHI_SYS_CPU_CLK_CNTL);
	 
    writel(plls->sys_pll_cntl|0x8000,P_HHI_SYS_PLL_CNTL);
    writel(0x65e31ff  ,P_HHI_SYS_PLL_CNTL2);
    writel(0x1649a941 , P_HHI_SYS_PLL_CNTL3);
    writel(plls->sys_pll_cntl&(~0x8000),P_HHI_SYS_PLL_CNTL);
	

    writel(plls->other_pll_cntl|0x8000,P_HHI_OTHER_PLL_CNTL);
    writel(0x65e31ff  ,P_HHI_OTHER_PLL_CNTL2);
    writel(0x1649a941 , P_HHI_OTHER_PLL_CNTL3);
    writel(plls->other_pll_cntl&(~0x8000),P_HHI_OTHER_PLL_CNTL);

	writel(plls->mpeg_clk_cntl,
	    P_HHI_MPEG_CLK_CNTL);
	
	writel(plls->sys_clk_cntl
			,P_HHI_SYS_CPU_CLK_CNTL);
	
	romboot_info->clk81 = plls->clk81;
}
STATIC_PREFIX void ddr_pll_init(struct ddr_set * ddr_setting) 
{
    int i;
    writel(ddr_setting->ddr_pll_cntl|0x8000,P_HHI_DDR_PLL_CNTL);   
    writel(0x65e31ff,P_HHI_DDR_PLL_CNTL2);
	writel(0x1649a941,P_HHI_DDR_PLL_CNTL3);
    writel(ddr_setting->ddr_pll_cntl&(~(0x8000)),P_HHI_DDR_PLL_CNTL);    
#if 0
    for ( i = 0; i <= 1000; i ++) {          
       writel(ddr_setting->ddr_ctrl,P_MMC_DDR_CTRL);   // 
    }
#else
    __udelay(100);
    while(!(readl(P_HHI_DDR_PLL_CNTL3)&(1<<31)));
#endif
}
//Measure clk

STATIC_PREFIX unsigned long    clk_util_clk_msr(unsigned long   clk_mux)
{
    writel(0,P_MSR_CLK_REG0);
    // Set the measurement gate to 64uS
	clrsetbits_le32(P_MSR_CLK_REG0,0xffff,64-1);
    // Disable continuous measurement
    // disable interrupts
    clrbits_le32(P_MSR_CLK_REG0,((1 << 18) | (1 << 17)));
	clrsetbits_le32(P_MSR_CLK_REG0,(0xf<<20),(clk_mux<<20)|(1<<19)|(1<<16));

	readl(P_MSR_CLK_REG0); 
    // Wait for the measurement to be done
    while( readl(P_MSR_CLK_REG0) & (1 << 31))  {};
    // disable measuring
	clrbits_le32(P_MSR_CLK_REG0,(1 << 16));
	unsigned msr=(readl(P_MSR_CLK_REG2)+31)&0x000FFFFF;
    // Return value in MHz*measured_val
    return (msr>>6);
}

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
#define CONFIG_AML_CLK_LIST_ENABLE 0

