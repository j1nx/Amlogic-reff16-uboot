#include <asm/arch/am_reg_addr.h>
#define MSR_CLK_REG0                               0x21d7
#define MSR_CLK_REG1                               0x21d8
#define MSR_CLK_REG2                               0x21d9

#define Wr(addr, data) *(volatile unsigned long *)(0xc1100000|(addr<<2))=data
#define Rd(addr) *(volatile unsigned long *)(0xc1100000|(addr<<2))

unsigned long    clk_util_clk_msr(   unsigned long   clk_mux );



void pll_clk_list(void)
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

// -----------------------------------------
// clk_util_clk_msr
// -----------------------------------------
// from periphs_top
//
// .clk0           ( am_ring_osc_clk_out[0]    ),       // ring oscilator
// .clk1           ( am_ring_osc_clk_out[1]    ),       // ring oscilator
// .clk2           ( ext_clk_to_msr_i          ),       // external audio I2S AMLCLK
// .clk3           ( cts_a9_clk                ),       // A9 clock
// .clk4           ( cts_a9_periph_clk         ),
// .clk5           ( cts_a9_axi_clk            ),
// .clk6           ( cts_a9_at_clk             ),
// .clk7           ( cts_a9_apb_clk            ),
// .clk8           ( cts_arc625_clk            ),       //  ARC625 clock
// .clk9           ( sys_pll_div3              ),
// .clk10          ( ddr_pll_clk               ),       // DDR clock
// .clk11          ( other_pll_clk             ),       // Other PL output clock
// .clk12          ( aud_pll_clk               ),       // Audio PLL output clock
// .clk13          ( demod_pll_clk240          ),       // DEMOD PLL output clock
// .clk14          ( demod_pll_adc_clk         ),       // DEMOD PLL output clock
// .clk15          ( demod_pll_wifi_adc_clk    ),       // DEMOD PLL output clock
// .clk16          ( demod_pll_adc_clk_57      ),       // DEMOD PLL output clock
// .clk17          ( demod_pll_clk400          ),       // DEMOD PLL output clock
// .clk18          ( demod_pll_wifi_dac_clk    ),       // DEMOD PLL output clock
// .clk19          ( vid_pll_clk               ),       // Video Clock
// .clk20          ( vid_pll_ref_clk           ),
// .clk21          ( HDMI_CH0_TMDSCLK          ),       // HDMI clock
//
// For Example
//
// unsigend long    ddr_pll_clk   = clk_util_clk_msr( 10,   // mux sel 10
//                                                    50 ); // measure for 50uS
//
// returns a value in "ddr_pll_clk" in Hz 
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
unsigned long    clk_util_clk_msr(unsigned long   clk_mux)
{
     unsigned long   uS_gate_time=64;
		Wr(MSR_CLK_REG0,0);
    // Set the measurement gate to 100uS
    Wr(MSR_CLK_REG0, (Rd(MSR_CLK_REG0) & ~(0xFFFF << 0)) | ((uS_gate_time-1) << 0) );
    // Disable continuous measurement
    // disable interrupts
    Wr(MSR_CLK_REG0, (Rd(MSR_CLK_REG0) & ~((1 << 18) | (1 << 17))) );
    Wr(MSR_CLK_REG0, (Rd(MSR_CLK_REG0) & ~(0xf << 20)) | ((clk_mux << 20) |  // Select MUX 
                                                          (1 << 19) |     // enable the clock
                                                          (1 << 16)) );    // enable measuring
    { unsigned long dummy_rd = Rd(MSR_CLK_REG0); }
    // Wait for the measurement to be done
    while( (Rd(MSR_CLK_REG0) & (1 << 31)) ) {} 
    // disable measuring
    Wr(MSR_CLK_REG0, (Rd(MSR_CLK_REG0) & ~(1 << 16)) | (0 << 16) );

    unsigned long   measured_val = Rd(MSR_CLK_REG2);
    // Return value in Hz*measured_val
    return (measured_val);
}


