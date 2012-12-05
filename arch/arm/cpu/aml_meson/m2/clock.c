/*
 *  Copyright (C) 2010 AMLOGIC, INC.
 *
 *  Y:\arch\arm\cpu\aml_meson\m1\clock.c
 * 
 *
 * License terms: GNU General Public License (GPL) version 2
 * Basic register address definitions in physical memory and
 * some block defintions for core devices like the timer.
 * 03/06/10
 *
 * author : jerry.yu
 */
 
#include <asm/arch/clock.h>
#include <asm/arch/romboot.h>
/*
	romboot_info->sys_clk=500000000;
    romboot_info->arm_clk=500000000;
    romboot_info->mpeg_clk=200000000;
*/
T_ROM_BOOT_RETURN_INFO * romboot_info=(const T_ROM_BOOT_RETURN_INFO *)C_ROM_BOOT_DEBUG;
__u32 get_cpu_clk()
{
     /*
    @todo implement this function
    */
    return romboot_info->a9_clk;
}
static __u32 clk81_freq=0;
__u32 get_clk81()
{
	if(clk81_freq!=0)
		return clk81_freq;
	clk81_freq= romboot_info->clk81;
//(clk_util_clk_msr(CLK_OTHER_PLL_CLK)*1000000)/((readl(P_HHI_MPEG_CLK_CNTL)&0x7f)+1);

     /*
    @todo implement this function
    */
    return clk81_freq;
}

__u32    clk_util_clk_msr( __u32   clk_mux )
{

	writel(0,P_MSR_CLK_REG0);
    // Set the measurement gate to 64uS
	clrsetbits_le32(P_MSR_CLK_REG0,0xffff,64-1);
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

