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
    static __u32 sys_freq=0;
    if(sys_freq==0)
    {
        sys_freq=(clk_util_clk_msr(CLK_SYS)*1000000);
    }
    return sys_freq;
}
__u32 get_clk81()
{
    static __u32 clk81_freq=0;
	if(clk81_freq==0)
	{
	    clk81_freq=(clk_util_clk_msr(CLK_CLK81)*1000000);
	}
    return clk81_freq;
}
__u32 get_clk_ddr()
{
    static __u32 freq=0;
	if(freq==0)
	{
	    freq=(clk_util_clk_msr(CLK_DDR_PLL_CLK)*1000000);
	}
    return freq;
}

__u32 get_clk_ethernet_pad()
{
    static __u32 freq=0;
    if(freq==0)
    {
        freq=(clk_util_clk_msr(CLK_ETH_CLK50_I)*1000000);
    }
    return freq;
}

__u32 get_clk_cts_eth_rmii()
{
    static __u32 freq=0;
    if(freq==0)
    {
        freq=(clk_util_clk_msr(CLK_ETH_RMII)*1000000);
    }
    return freq;
}

// -----------------------------------------
// clk_util_clk_msr
// -----------------------------------------
// from twister_core.v
//        .clk_to_msr_in          ( { 1'b0,                       // [31]
//                                    1'b0,                       // [30]
//                                    clk_double50_msr,           // [29]
//                                    cts_sar_adc_clk,            // [28]
//                                    cts_audac_clkpi,            // [27]
//                                    sc_clk_int,                 // [26]
//                                    usb_clk_12mhz,              // [25]
//                                    lvds_fifo_clk,              // [24]
//                                    HDMI_CH3_TMDSCLK,           // [23]
//                                    mod_eth_clk50_i,            // [22]
//                                    mod_audin_amclk_i,          // [21]
//                                    cts_btclk27,                // [20]
//                                    cts_hdmi_sys_clk,           // [19]
//                                    cts_led_pll_clk,            // [18]
//                                    cts_vghl_pll_clk,           // [17]
//                                    cts_FEC_CLK_2,              // [16]
//                                    cts_FEC_CLK_1,              // [15]
//                                    cts_FEC_CLK_0,              // [14]
//                                    cts_amclk,                  // [13]
//                                    1'b0,                       // [12]
//                                    cts_eth_rmii,               // [11]
//                                    cts_dclk,                   // [10]
//                                    cts_sys_cpu_clk,            // [9]
//                                    cts_media_cpu_clk,          // [9]
//                                    clk81,                      // [7]
//                                    vid_pll_clk,                // [6]
//                                    aud_pll_clk,                // [5]
//                                    misc_pll_clk,               // [4]
//                                    ddr_pll_clk,                // [3]
//                                    sys_pll_clk,                // [2]
//                                    am_ring_osc_clk_out[1],     // [1]
//                                    am_ring_osc_clk_out[0]} ),  // [0]
// For Example
//
// unsigend long    clk81_clk   = clk_util_clk_msr( 2,      // mux select 2
//                                                  50 );   // measure for 50uS
//
// returns a value in "clk81_clk" in Hz 
//
// The "uS_gate_time" can be anything between 1uS and 65535 uS, but the limitation is
// the circuit will only count 65536 clocks.  Therefore the uS_gate_time is limited by
//
//   uS_gate_time <= 65535/(expect clock frequency in MHz) 
//
// For example, if the expected frequency is 400Mhz, then the uS_gate_time should 
// be less than 163.
//
// Your measurement resolution is:
//
//    100% / (uS_gate_time * measure_val )
//
#if 0
unsigned long    clk_util_clk_msr(   unsigned long   clk_mux)
{
    // Set the measurement gate to 100uS
    Wr(MSR_CLK_REG0, (Rd(MSR_CLK_REG0) & ~(0xFFFF << 0)) | ((64-1) << 0) );
    // Disable continuous measurement
    // disable interrupts
    Wr(MSR_CLK_REG0, (Rd(MSR_CLK_REG0) & ~((1 << 18) | (1 << 17))) );
    Wr(MSR_CLK_REG0, (Rd(MSR_CLK_REG0) & ~(0x1f << 20)) | ((clk_mux << 20) |  // Select MUX 
                                                          (1 << 19) |     // enable the clock
                                                          (1 << 16)) );    // enable measuring
    // Delay
    Rd(MSR_CLK_REG0); 
    // Wait for the measurement to be done
    while( (Rd(MSR_CLK_REG0) & (1 << 31)) ) {} 
    // disable measuring
    Wr(MSR_CLK_REG0, (Rd(MSR_CLK_REG0) & ~(1 << 16)) | (0 << 16) );

    unsigned long   measured_val = (Rd(MSR_CLK_REG2) & 0x000FFFFF); // only 20 bits

    if( measured_val == 0xFFFFF ) {     // 20 bits max
        return(0);
    } else {
        // Return value in MHz
        return(measured_val>>6);
    }
}
#endif
__u32    clk_util_clk_msr( __u32   clk_mux )
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
	__u32 msr=(readl(P_MSR_CLK_REG2)+31)&0x000FFFFF;
    // Return value in MHz*measured_val
    return (msr>>6);
}

