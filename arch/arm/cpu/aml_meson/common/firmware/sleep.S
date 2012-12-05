/*
 * (C) Copyright 2010, Amlogic, Inc. http://www.amlogic.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR /PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <asm/arch/firm/sleep.h>
#include <asm/arch/firm/dmc.h>
#include <asm/arch/firm/pctl.h>

#define P_LED_PWM_REG0		0xc1108768
#define P_PCTL_IOCR_ADDR	0xd0040248
#define P_PCTL_SCTL_ADDR	0xd0040004
#define P_MMC_PHY_CTRL		0xd0041380	//没有这部分资料
#define P_PCTL_PHYCR_ADDR	0xd0040240
#define P_PCTL_DLLCR9_ADDR	0xd00402a8
#define P_HIU_DDR_PLL_CTRL	0xc11041a0
#define P_HIU_GCLK_MPEG		0xc1104140
#define P_A9_AUTO_CLK0		0xc11041E0




    .text
/*
 * Move Meson into deep sleep state
 *
 * Note: This code is copied to internal SRAM by PM code. When the Meson
 *     wakes up it continues execution at the point it went to sleep.
 */
.globl meson_cpu_suspend;
meson_cpu_suspend:
    stmfd   sp!, {r0-r12, lr}        @ save registers on stack

    ldmia   r0, {r0-r6}
    /*
        r0: pctl_reg_base;
        r1: mmc_reg_base;
        r2: hiu_reg_base;	P_LED_PWM_REG0
        r3: power_key;
        r4: ddr_clk;
        r5: ddr_reg_backup;
        r6: core_voltage_adjust;
    */
    
    /* turn on auto byte bypass */
	ldr    r0, =P_PCTL_IOCR_ADDR
    ldr     r8, [r0]
    orr     r8, r8, #(1<<25)	//Automatically put Byte Lane DLLs into Bypass mode when memories are in Self Refresh
    str     r8, [r0]

    /*
     * Switch DDR to self-refresh mode.
     */
    ldr    r0, =P_PCTL_SCTL_ADDR
    mov     r8, #0x3	//SLEEP (move to Low_power from Access)
    str     r8, [r0]
	
    mov     r8, #MS_DLY
0:  subs    r8, r8, #0x1
    bne     0b
	
    /* DDR PHY power saving*/
	ldr    r0, =P_MMC_PHY_CTRL
    ldr     r8, [r0]
    orr     r8, r8, #(1<<0)
    orr     r8, r8, #(1<<8)
    orr     r8, r8, #(1<<13)
    str     r8, [r0]

    /* hold dll reset */
	ldr    r0, =P_PCTL_PHYCR_ADDR
    ldr     r8, [r0]
    bic     r8, r8, #(1<<6)	//Command Lane DLL Soft Reset
    str     r8, [r0]

    /* enable dll bypass */	//这个好想写错了，找Robin确认
	ldr    r0, =P_PCTL_DLLCR9_ADDR
    ldr     r8, [r0]
    orr     r8, r8, #(1<<31)	//DLL Disable: A disabled DLL is bypassed.
    str     r8, [r0]
	

	ldr    r0, =P_HIU_DDR_PLL_CTRL
#ifdef TURN_OFF_DDR_PLL
    /* turn off ddr pll */
    ldr     r8, [r0]
    orr     r8, r8, #(1<<15)
    str     r8, [r0]
#else
    /* lower ddr pll */    
    ldr     r7, [r0]
    ldr		r4, =0x00120234	//Elvis Fool
    //str     r4, [r0]	//.ddr_clk = 0x00120234, // 312m, 0x00110220, //384m
#endif

    /* ddr power gate */
	ldr		r0, =P_HIU_GCLK_MPEG
    ldr     r8, [r0]
    bic     r8, r8, #0x41
    str     r8, [r0]

	/* r9: led_pwm_reg0, r10: value of led_pwm_reg0*/
    ldr    r9, =P_LED_PWM_REG0
    ldr     r10, [r9]
    /* lower core voltage */
    bic     r0, r10, #0x0f
    //orr     r0, r0, r6	//.core_voltage_adjust = 5,
    orr     r0, r0, #5
    str     r0, [r9]
	
    /* System goes to sleep beyond after this instruction */
	ldr		r0, =P_A9_AUTO_CLK0	//START:  If this bit set to 1, then set to 0, the auto-clock switching module will start executing a clock ratio change.
    ldr     r8, [r0]
    orr     r8, r8, #(1<<1)
    str     r8, [r0]
    bic     r8, r8, #(1<<1)
    str     r8, [r0]

	//wait for interrupt
    wfi

	/* Wake up from sleep */
	ldr		r0, =P_A9_AUTO_CLK0
    ldr     r8, [r0]
    bic     r8, r8, #(1<<0)	//ENABLE:  If this bit is set to 1, then the auto-clock ratio module will execute a change once the START pulses is generated.
    str     r8, [r0]

    /* restore core voltage */
    str     r10, [r9]
    mov     r8, #VOLTAGE_DLY
2:  subs    r8, r8, #0x1
    bne     2b

    /* ddr power gate */
	ldr		r0, =P_HIU_GCLK_MPEG
    ldr     r8, [r0]
    orr     r8, r8, #0x41
    str     r8, [r0]

	ldr		r0, =P_HIU_DDR_PLL_CTRL
#ifdef TURN_OFF_DDR_PLL
    /* turn on ddr pll */
    ldr     r8, [r0]
    bic     r8, r8, #(1<<15)
    str     r8, [r0] 
#else
    /* restore ddr pll */
    str     r7, [r2, #0]
#endif

    /* Wait for PLL to lock */
    mov     r8, #MS_DLY
3:  subs    r8, r8, #0x1
    bne     3b

    /* disable dll bypass */
	ldr		r0, =P_PCTL_DLLCR9_ADDR
    ldr     r8, [r0]
    bic     r8, r8, #(1<<31)
    str     r8, [r0]

    /* release reset */
	ldr		r0, =P_PCTL_PHYCR_ADDR
    ldr     r8, [r0]
    orr     r8, r8, #(1<<6)
    str     r8, [r0]

    /* DDR PHY leave power saving mode*/
	ldr		r0, =P_MMC_PHY_CTRL
    ldr     r8, [r0]
    bic     r8, r8, #(1<<8)
    str     r8, [r0]

    /* ddr back to normal mode */
	ldr    r0, =P_PCTL_SCTL_ADDR
    mov     r8, #0x4
    str     r8, [r0]

    mov     r8, #MS_DLY
9:  subs    r8, r8, #0x1
    bne     9b

    /* Restore registers and return */
    ldmfd   sp!, {r0-r12, pc}

