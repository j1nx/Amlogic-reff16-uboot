/*
 *  arch/arm/mach-meson/include/mach/clock.h
 *
 *  Copyright (C) 2010 AMLOGIC, INC.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ARCH_ARM_MESON_CLOCK_H_U_BOOT_
#define __ARCH_ARM_MESON_CLOCK_H_U_BOOT_
#include <common.h>
#include <asm/io.h>
#include <asm/types.h>
#include "am_regs.h"
#include "am_reg_addr.h"
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

#define AM_RING_OSC_CLK_OUT0      (0)
#define AM_RING_OSC_CLK_OUT1      (1)
#define EXT_CLK_TO_MSR_I          (2)
#define CTS_A9_CLK                (3)
#define CTS_A9_PERIPH_CLK         (4)
#define CTS_A9_AXI_CLK            (5)
#define CTS_A9_AT_CLK             (6)
#define CTS_A9_APB_CLK            (7)
#define CTS_ARC625_CLK            (8)
#define SYS_PLL_DIV3              (9)
#define DDR_PLL_CLK               (10)
#define OTHER_PLL_CLK             (11)
#define AUD_PLL_CLK               (12)
#define DEMOD_PLL_CLK240          (13)
#define DEMOD_PLL_ADC_CLK         (14)
#define DEMOD_PLL_WIFI_ADC_CLK    (15)
#define DEMOD_PLL_ADC_CLK_57      (16)
#define DEMOD_PLL_CLK400          (17)
#define DEMOD_PLL_WIFI_DAC_CLK    (18)
#define VID_PLL_CLK               (19)
#define VID_PLL_REF_CLK           (20)


#define clk_get_clk81_rate()  180000000
__u32    clk_util_clk_msr( __u32   clk_mux );
__u32 get_cpu_clk(void);
__u32 get_clk81(void);
__u32 get_other_clk(void);


#endif
