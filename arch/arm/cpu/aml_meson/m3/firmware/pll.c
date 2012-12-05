


#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <memtest.h>
#include <config.h>
#include <asm/arch/firm/io.h>
static void serial_init_i(unsigned config)
{
    serial_init_firmware((__plls.uart&(~0xfff))|((clk_util_clk_msr(7)*1000000)/(115200*4)));
}
static void wait_pll(unsigned clk,unsigned dest)
{
    unsigned cur;
    do{
        cur=clk_util_clk_msr(clk);
        
        serial_puts("wait pll");
        serial_put_hex(clk,8);
        serial_puts("target is");
        serial_put_hex(dest,16);
        serial_puts(" now it is");
        serial_put_dword(cur);
        __udelay(100);
    }while((cur<(dest-1)) || (cur >(dest+1)));
}

static void pll_initial(struct pll_clk_settings * plls) 
{
    //* sys pll
    clrbits_le32(P_HHI_SYS_CPU_CLK_CNTL,1<<7);
    clrbits_le32(P_HHI_MPEG_CLK_CNTL,1<<8);
    writel(plls->sys_pll_cntl|0x8000,P_HHI_SYS_PLL_CNTL);//800Mhz(0x664),600Mhz,400Mhz , 200Mhz
    writel(0x65e31ff  ,P_HHI_SYS_PLL_CNTL2);
    writel(0x1649a941 , P_HHI_SYS_PLL_CNTL3);
    writel(plls->sys_pll_cntl&(~0x8000),P_HHI_SYS_PLL_CNTL);

	writel(SYS_PLL_RESET,P_RESET5_REGISTER);

	M3_PLL_WAIT_FOR_LOCK(P_HHI_SYS_PLL_CNTL3);
	
    /** misc pll **/
    writel(plls->other_pll_cntl|0x8000,P_HHI_OTHER_PLL_CNTL);//800Mhz(0x664)
    writel(0x65e31ff  ,P_HHI_OTHER_PLL_CNTL2);
    writel(0x1649a941 , P_HHI_OTHER_PLL_CNTL3);
    writel(plls->other_pll_cntl&(~0x8000),P_HHI_OTHER_PLL_CNTL);

	writel(MISC_PLL_RESET,P_RESET5_REGISTER);

	M3_PLL_WAIT_FOR_LOCK(P_HHI_OTHER_PLL_CNTL3);
    
    //wait_pll(2,plls->sys_clk);	
    //wait_pll(4,plls->other_clk);
    serial_puts("\n\n\n");
    __udelay(3000);
	serial_wait_tx_empty();
	writel(plls->sys_clk_cntl
			,P_HHI_SYS_CPU_CLK_CNTL);
	serial_wait_tx_empty();
	writel(plls->mpeg_clk_cntl,
	    P_HHI_MPEG_CLK_CNTL);
	serial_init_i(__plls.uart);
	__udelay(1000);
	serial_put_dword(clk_util_clk_msr(7));
#if 0
    Wr(HHI_DDR_PLL_CNTL, timing_reg->ddr_pll_cntl |0x8000);//667,533,400((0x664)),333
    Wr(HHI_DDR_PLL_CNTL2,0x65e31ff);
    Wr(HHI_DDR_PLL_CNTL3,0x1649a941);
    Wr(HHI_DDR_PLL_CNTL, timing_reg->ddr_pll_cntl);
#endif	
	
#if 0			
    /** Setting sys cpu pll**/
	writel(plls->sys_pll_cntl,P_HHI_SYS_PLL_CNTL);
	writel( (1<<0)    |  //select sys pll for sys cpu
	        (1<<2)    |  // divided 2
	        (1<<4)    |  //APB_en
	        (1<<5)    |  //AT en
	        (1<<7)      // send to sys cpu
	          
			,P_HHI_SYS_CPU_CLK_CNTL);
	/**
	 * clk81 settings From kernel
	 */
    writel(plls->other_pll_cntl,P_HHI_OTHER_PLL_CNTL);
	writel(plls->mpeg_clk_cntl,P_HHI_MPEG_CLK_CNTL);
	
#endif	
    int i;
    for(i=0;i<46;i++)
    {
        serial_put_hex(i,8);
        serial_puts("=");
        serial_put_dword(clk_util_clk_msr(i));
    }

}
STATIC_PREFIX void ddr_pll_init(struct ddr_set * ddr_setting) 
{
    writel(ddr_setting->ddr_pll_cntl,P_HHI_DDR_PLL_CNTL);  

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

    measured_val = readl(P_MSR_CLK_REG2)&0x000FFFFF;
    // Return value in Hz*measured_val
    // Return Mhz
    return (measured_val>>6);
    // Return value in Hz*measured_val
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
    unsigned char clk_list[]={3,31,7};
    char *clk_list_name[]={"ddr","a9","clk81"};
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
