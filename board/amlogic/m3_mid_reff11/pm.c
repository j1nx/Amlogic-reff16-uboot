/*
 * Meson Power Management Routines
 *
 * Copyright (C) 2010 Amlogic, Inc. http://www.amlogic.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */


#include <common.h>
#include <asm/mach-types.h>
#include <asm/arch/memory.h>
#include <asm/arch/am_reg_addr.h>
#include <asm/arch/aml_card_sd.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/am_eth_reg.h>
#include <asm/arch/clk_set.h>
#include <share_kernel.h>
#include <asm/arch/gpio.h>
#include <asm/arch/ao_regs.h>
#include <asm/arch/am_regs.h>

static int early_suspend_flag = 0;

#include <asm/arch/power_gate.h>
unsigned char GCLK_ref[GCLK_IDX_MAX];
EXPORT_SYMBOL(GCLK_ref);

static void turn_off_audio_DAC(void)
{
    int wr_val;
    
    SET_CBUS_REG_MASK(HHI_GCLK_MPEG1, (1 << 13));//open ADC gate
    wr_val = 0;
    WRITE_APB_REG(ADAC_RESET, wr_val);
    WRITE_APB_REG(ADAC_POWER_CTRL_REG1, wr_val);
    WRITE_APB_REG(ADAC_POWER_CTRL_REG2, wr_val);

    wr_val = 1;
    WRITE_APB_REG(ADAC_LATCH, wr_val);
    wr_val = 0;
    WRITE_APB_REG(ADAC_LATCH, wr_val);
    CLEAR_CBUS_REG_MASK(HHI_GCLK_MPEG1, (1 << 13));//close ADC gate
} /* turn_off_audio_DAC */

#define ON  1
#define OFF 0

//#define EARLY_SUSPEND_USE_XTAL
//#define AML_SUSPEND_DEBUG

static int mask_save[4];

//static void meson_sram_push(void *dest, void *src, unsigned int size)
//{
//    int res = 0;
//    memcpy(dest, src, size);
//    flush_icache_range((unsigned long)dest, (unsigned long)(dest + size));
//    res = memcmp(dest, src, size);
//    printf("compare code in sram addr = 0x%x, size = 0x%x, result = %d", (unsigned)dest, size, res);
//}

#define GATE_OFF(_MOD) do {power_gate_flag[GCLK_IDX_##_MOD] = IS_CLK_GATE_ON(_MOD);CLK_GATE_OFF(_MOD);} while(0)
#define GATE_ON(_MOD) do {if (power_gate_flag[GCLK_IDX_##_MOD]) CLK_GATE_ON(_MOD);} while(0)
#define GATE_SWITCH(flag, _MOD) do {if (flag) GATE_ON(_MOD); else GATE_OFF(_MOD);} while(0)
static int power_gate_flag[GCLK_IDX_MAX];


/*
 * Don't call this function when usb device is operating.
 *
 * por_flg:
 *      USB_CTL_POR_ON  -- power on ctl if ctl enabled
 *      USB_CTL_POR_OFF -- power off ctl if ctl enabled
 *      USB_CTL_POR_ENABLE -- power on and enable ctl
 *      USB_CTL_POR_DISABLE -- power off and disable ctl
 */
static char usb_ctl_a_state = USB_CTL_POR_ENABLE;
static char usb_ctl_b_state = USB_CTL_POR_ENABLE;
void set_usb_ctl_por(int index, int por_flag)
{

    int msk = PREI_USB_PHY_A_POR;
    char * msg;
    char * ctl;
    char * state;

    if (index == USB_CTL_INDEX_B) {
        msk = PREI_USB_PHY_B_POR;
        ctl = "B";
        state = &usb_ctl_b_state;
    } else if (index == USB_CTL_INDEX_A) {
        msk = PREI_USB_PHY_A_POR;
        ctl = "A";
        state = &usb_ctl_a_state;
    } else {
        printf("error usb_ctl index %d\n", index);
        return;
    }


    switch (por_flag) {
    case USB_CTL_POR_ON:
        if (*state > USB_CTL_POR_ENABLE) {
            return;
        }
        msg = "on";
        CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG, msk);
        break;
    case USB_CTL_POR_OFF:
        if (*state > USB_CTL_POR_ENABLE) {
            return;
        }
        msg = "off";
        SET_CBUS_REG_MASK(PREI_USB_PHY_REG, msk);
        break;
    case USB_CTL_POR_ENABLE:
        msg = "enable";
        CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG, msk);
        *state = USB_CTL_POR_ENABLE;
        break;
    case USB_CTL_POR_DISABLE:
        msg = "disable";
        SET_CBUS_REG_MASK(PREI_USB_PHY_REG, msk);
        *state = USB_CTL_POR_DISABLE;
        break;
    default:
        printf("error usb_ctl por_flag %d\n", por_flag);
        return;
    }


    printf("usb controller %s %s\n", ctl, msg);

}
EXPORT_SYMBOL(set_usb_ctl_por);


void power_gate_init(void)
{
    GATE_INIT(DDR);
    GATE_INIT(VLD_CLK);
    GATE_INIT(IQIDCT_CLK);
    GATE_INIT(MC_CLK);
    GATE_INIT(AHB_BRIDGE);
    GATE_INIT(ISA);
    GATE_INIT(APB_CBUS);
    GATE_INIT(_1200XXX);
    GATE_INIT(SPICC);
    GATE_INIT(I2C);
    GATE_INIT(SAR_ADC);
    GATE_INIT(SMART_CARD_MPEG_DOMAIN);
    GATE_INIT(RANDOM_NUM_GEN);
    GATE_INIT(UART0);
    GATE_INIT(SDHC);
    GATE_INIT(STREAM);
    GATE_INIT(ASYNC_FIFO);
    GATE_INIT(SDIO);
    GATE_INIT(AUD_BUF);
    GATE_INIT(HIU_PARSER);
    GATE_INIT(AMRISC);
    GATE_INIT(BT656_IN);
    GATE_INIT(ASSIST_MISC);
    GATE_INIT(VENC_I_TOP);
    GATE_INIT(VENC_P_TOP);
    GATE_INIT(VENC_T_TOP);
    GATE_INIT(VENC_DAC);
    GATE_INIT(VI_CORE);
    GATE_INIT(SPI2);
    GATE_INIT(MDEC_CLK_ASSIST);
    GATE_INIT(MDEC_CLK_PSC);
    GATE_INIT(SPI1);
    GATE_INIT(AUD_IN);
    GATE_INIT(ETHERNET);
    GATE_INIT(DEMUX);
    GATE_INIT(AIU_AI_TOP_GLUE);
    GATE_INIT(AIU_IEC958);
    GATE_INIT(AIU_I2S_OUT);
    GATE_INIT(AIU_AMCLK_MEASURE);
    GATE_INIT(AIU_AIFIFO2);
    GATE_INIT(AIU_AUD_MIXER);
    GATE_INIT(AIU_MIXER_REG);
    GATE_INIT(AIU_ADC);
    GATE_INIT(BLK_MOV);
    GATE_INIT(UART1);
    GATE_INIT(LED_PWM);
    GATE_INIT(VGHL_PWM);
    GATE_INIT(GE2D);
    GATE_INIT(USB0);
    GATE_INIT(USB1);
    GATE_INIT(RESET);
    GATE_INIT(NAND);
    GATE_INIT(HIU_PARSER_TOP);
    GATE_INIT(MDEC_CLK_DBLK);
    GATE_INIT(MDEC_CLK_PIC_DC);
    GATE_INIT(VIDEO_IN);
    GATE_INIT(AHB_ARB0);
    GATE_INIT(EFUSE);
    GATE_INIT(ROM_CLK);
    GATE_INIT(AHB_DATA_BUS);
    GATE_INIT(AHB_CONTROL_BUS);
    GATE_INIT(HDMI_INTR_SYNC);
    GATE_INIT(HDMI_PCLK);
    GATE_INIT(MISC_USB1_TO_DDR);
    GATE_INIT(MISC_USB0_TO_DDR);
    GATE_INIT(AIU_PCLK);
    GATE_INIT(MMC_PCLK);
    GATE_INIT(MISC_DVIN);
    GATE_INIT(MISC_RDMA);
    GATE_INIT(UART2);
    GATE_INIT(VENCI_INT);
    GATE_INIT(VIU2);
    GATE_INIT(VENCP_INT);
    GATE_INIT(VENCT_INT);
    GATE_INIT(VENCL_INT);
    GATE_INIT(VENC_L_TOP);
    GATE_INIT(VCLK2_VENCI);
    GATE_INIT(VCLK2_VENCI1);
    GATE_INIT(VCLK2_VENCP);
    GATE_INIT(VCLK2_VENCP1);
    GATE_INIT(VCLK2_VENCT);
    GATE_INIT(VCLK2_VENCT1);
    GATE_INIT(VCLK2_OTHER);
    GATE_INIT(VCLK2_ENCI);
    GATE_INIT(VCLK2_ENCP);
    GATE_INIT(DAC_CLK);
    GATE_INIT(AIU_AOCLK);
    GATE_INIT(AIU_AMCLK);
    GATE_INIT(AIU_ICE958_AMCLK);
    GATE_INIT(VCLK1_HDMI);
    GATE_INIT(AIU_AUDIN_SCLK);
    GATE_INIT(ENC480P);
    GATE_INIT(VCLK2_ENCT);
    GATE_INIT(VCLK2_ENCL);
    GATE_INIT(VCLK2_VENCL);
    GATE_INIT(VCLK2_OTHER1);
}

void power_init_off(void)
{
#if 0
    CLEAR_CBUS_REG_MASK(HHI_DEMOD_CLK_CNTL, (1 << 8));
    CLEAR_CBUS_REG_MASK(HHI_SATA_CLK_CNTL, (1 << 8));
    CLEAR_CBUS_REG_MASK(HHI_ETH_CLK_CNTL, (1 << 8));
    CLEAR_CBUS_REG_MASK(HHI_WIFI_CLK_CNTL, (1 << 0));
    SET_CBUS_REG_MASK(HHI_DEMOD_PLL_CNTL, (1 << 15));
#endif
}

void power_gate_switch(int flag)
{
#ifndef ADJUST_CORE_VOLTAGE
    GATE_SWITCH(flag, LED_PWM);
#endif
    //GATE_SWITCH(flag, DDR);
    GATE_SWITCH(flag, VLD_CLK);
    GATE_SWITCH(flag, IQIDCT_CLK);
    GATE_SWITCH(flag, MC_CLK);
    //GATE_SWITCH(flag, AHB_BRIDGE);
    //GATE_SWITCH(flag, ISA);
    //GATE_SWITCH(flag, APB_CBUS);
    //GATE_SWITCH(flag, _1200XXX);
    GATE_SWITCH(flag, SPICC);
    GATE_SWITCH(flag, I2C);
    GATE_SWITCH(flag, SAR_ADC);
    GATE_SWITCH(flag, SMART_CARD_MPEG_DOMAIN);
    GATE_SWITCH(flag, RANDOM_NUM_GEN);
    //GATE_SWITCH(flag, UART0);
    GATE_SWITCH(flag, SDHC);
    GATE_SWITCH(flag, STREAM);
    GATE_SWITCH(flag, ASYNC_FIFO);
    GATE_SWITCH(flag, SDIO);
    GATE_SWITCH(flag, AUD_BUF);
    //GATE_SWITCH(flag, HIU_PARSER);
    GATE_SWITCH(flag, AMRISC);
    GATE_SWITCH(flag, BT656_IN);
    //GATE_SWITCH(flag, ASSIST_MISC);
    GATE_SWITCH(flag, VI_CORE);
    GATE_SWITCH(flag, SPI2);
    GATE_SWITCH(flag, MDEC_CLK_ASSIST);
    GATE_SWITCH(flag, MDEC_CLK_PSC);
    GATE_SWITCH(flag, SPI1);
    GATE_SWITCH(flag, AUD_IN);
    GATE_SWITCH(flag, ETHERNET);
    GATE_SWITCH(flag, AIU_AI_TOP_GLUE);
    GATE_SWITCH(flag, AIU_IEC958);
    GATE_SWITCH(flag, AIU_I2S_OUT);
    GATE_SWITCH(flag, AIU_AMCLK_MEASURE);
    GATE_SWITCH(flag, AIU_AIFIFO2);
    GATE_SWITCH(flag, AIU_AUD_MIXER);
    GATE_SWITCH(flag, AIU_MIXER_REG);
    GATE_SWITCH(flag, AIU_ADC);
    GATE_SWITCH(flag, BLK_MOV);
    //GATE_SWITCH(flag, UART1);
    GATE_SWITCH(flag, VGHL_PWM);
    GATE_SWITCH(flag, GE2D);
    GATE_SWITCH(flag, USB0);
    GATE_SWITCH(flag, USB1);
    //GATE_SWITCH(flag, RESET);
    GATE_SWITCH(flag, NAND);
    GATE_SWITCH(flag, HIU_PARSER_TOP);
    GATE_SWITCH(flag, MDEC_CLK_DBLK);
    GATE_SWITCH(flag, MDEC_CLK_PIC_DC);
    GATE_SWITCH(flag, VIDEO_IN);
    //GATE_SWITCH(flag, AHB_ARB0);
    GATE_SWITCH(flag, EFUSE);
    GATE_SWITCH(flag, ROM_CLK);
    //GATE_SWITCH(flag, AHB_DATA_BUS);
    //GATE_SWITCH(flag, AHB_CONTROL_BUS);
    GATE_SWITCH(flag, MISC_USB1_TO_DDR);
    GATE_SWITCH(flag, MISC_USB0_TO_DDR);
    GATE_SWITCH(flag, AIU_PCLK);
    //GATE_SWITCH(flag, MMC_PCLK);
    GATE_SWITCH(flag, UART2);
    GATE_SWITCH(flag, DAC_CLK);
    GATE_SWITCH(flag, AIU_AOCLK);
    GATE_SWITCH(flag, AIU_AMCLK);
    GATE_SWITCH(flag, AIU_ICE958_AMCLK);
    GATE_SWITCH(flag, AIU_AUDIN_SCLK);
}
EXPORT_SYMBOL(power_gate_switch);

void early_power_gate_switch(int flag)
{
    GATE_SWITCH(flag, AMRISC);
    GATE_SWITCH(flag, AUD_IN);
    GATE_SWITCH(flag, BLK_MOV);
    GATE_SWITCH(flag, VENC_I_TOP);
    GATE_SWITCH(flag, VENC_P_TOP);
    GATE_SWITCH(flag, VENC_T_TOP);
    GATE_SWITCH(flag, VENC_DAC);
    GATE_SWITCH(flag, HDMI_INTR_SYNC);
    GATE_SWITCH(flag, HDMI_PCLK);
    GATE_SWITCH(flag, MISC_DVIN);
    GATE_SWITCH(flag, MISC_RDMA);
    GATE_SWITCH(flag, VENCI_INT);
    GATE_SWITCH(flag, VIU2);
    GATE_SWITCH(flag, VENCP_INT);
    GATE_SWITCH(flag, VENCT_INT);
    GATE_SWITCH(flag, VENCL_INT);
    GATE_SWITCH(flag, VENC_L_TOP);
    GATE_SWITCH(flag, VCLK2_VENCI);
    GATE_SWITCH(flag, VCLK2_VENCI1);
    GATE_SWITCH(flag, VCLK2_VENCP);
    GATE_SWITCH(flag, VCLK2_VENCP1);
    GATE_SWITCH(flag, VCLK2_VENCT);
    GATE_SWITCH(flag, VCLK2_VENCT1);
    GATE_SWITCH(flag, VCLK2_OTHER);
    GATE_SWITCH(flag, VCLK2_ENCI);
    GATE_SWITCH(flag, VCLK2_ENCP);
    GATE_SWITCH(flag, VCLK1_HDMI);
    GATE_SWITCH(flag, ENC480P);
    GATE_SWITCH(flag, VCLK2_ENCT);
    GATE_SWITCH(flag, VCLK2_ENCL);
    GATE_SWITCH(flag, VCLK2_VENCL);
    GATE_SWITCH(flag, VCLK2_OTHER1);
}
EXPORT_SYMBOL(early_power_gate_switch);

#define CLK_COUNT 7
static char clk_flag[CLK_COUNT];
static unsigned clks[CLK_COUNT] = {
    HHI_ETH_CLK_CNTL,
    HHI_VID_CLK_CNTL,
    HHI_VIID_CLK_CNTL,
    HHI_AUD_CLK_CNTL,
    HHI_MALI_CLK_CNTL,
    HHI_HDMI_CLK_CNTL,
    HHI_MPEG_CLK_CNTL,
};

static char clks_name[CLK_COUNT][32] = {
    "HHI_ETH_CLK_CNTL",
    "HHI_VID_CLK_CNTL",
    "HHI_VIID_CLK_CNTL",
    "HHI_AUD_CLK_CNTL",
    "HHI_MALI_CLK_CNTL",
    "HHI_HDMI_CLK_CNTL",
    "HHI_MPEG_CLK_CNTL",
};

#ifdef EARLY_SUSPEND_USE_XTAL
#define EARLY_CLK_COUNT 3
#else
#define EARLY_CLK_COUNT 2
#endif
static char early_clk_flag[EARLY_CLK_COUNT];
static unsigned early_clks[EARLY_CLK_COUNT] = {
    HHI_VID_CLK_CNTL,
    HHI_VIID_CLK_CNTL,
#ifdef EARLY_SUSPEND_USE_XTAL
    HHI_MPEG_CLK_CNTL,
#endif
};

static char early_clks_name[EARLY_CLK_COUNT][32] = {
    "HHI_VID_CLK_CNTL",
    "HHI_VIID_CLK_CNTL",
#ifdef EARLY_SUSPEND_USE_XTAL
    "HHI_MPEG_CLK_CNTL",
#endif
};

static unsigned uart_rate_backup;
static unsigned xtal_uart_rate_backup;
void clk_switch(int flag)
{
    int i;

    if (flag) {
        for (i = CLK_COUNT - 1; i >= 0; i--) {
            if (clk_flag[i]) {
                if ((clks[i] == HHI_VID_CLK_CNTL)||(clks[i] == HHI_VIID_CLK_CNTL)) {
                    WRITE_CBUS_REG_BITS(clks[i], clk_flag[i], 19, 2);
                } else if (clks[i] == HHI_MPEG_CLK_CNTL) {
                    udelay(1000);
                    SET_CBUS_REG_MASK(clks[i], (1 << 8)); // normal

                    CLEAR_CBUS_REG_MASK(UART0_CONTROL, (1 << 19) | 0xFFF);
                    SET_CBUS_REG_MASK(UART0_CONTROL, (((uart_rate_backup / (115200 * 4)) - 1) & 0xfff));
                    CLEAR_CBUS_REG_MASK(UART1_CONTROL, (1 << 19) | 0xFFF);
                    SET_CBUS_REG_MASK(UART1_CONTROL, (((uart_rate_backup / (115200 * 4)) - 1) & 0xfff));
                    CLEAR_AOBUS_REG_MASK(AO_UART_CONTROL, (1 << 19) | 0xFFF);
                    WRITE_AOBUS_REG_BITS(AO_UART_CONTROL, ((uart_rate_backup / (115200 * 4)) - 1) & 0xfff, 0, 12);
                } else {
                    SET_CBUS_REG_MASK(clks[i], (1 << 8));
                }
                clk_flag[i] = 0;
                printf("clk %s(%x) on\n", clks_name[i], clks[i]);
            }
        }
    } else {
        for (i = 0; i < CLK_COUNT; i++) {
            if ((clks[i] == HHI_VID_CLK_CNTL)||(clks[i] == HHI_VIID_CLK_CNTL)) {
                clk_flag[i] = READ_CBUS_REG_BITS(clks[i], 19, 2);
                if (clk_flag[i]) {
                    CLEAR_CBUS_REG_MASK(clks[i], (1<<19)|(1<<20));
                }
            } else if (clks[i] == HHI_MPEG_CLK_CNTL) {
                if (READ_CBUS_REG(clks[i]) & (1 << 8)) {
                    clk_flag[i] = 1;

                    udelay(1000);
                    CLEAR_CBUS_REG_MASK(clks[i], (1 << 8)); // 24M

                    CLEAR_CBUS_REG_MASK(UART0_CONTROL, (1 << 19) | 0xFFF);
                    SET_CBUS_REG_MASK(UART0_CONTROL, (((xtal_uart_rate_backup / (115200 * 4)) - 1) & 0xfff));
                    CLEAR_CBUS_REG_MASK(UART1_CONTROL, (1 << 19) | 0xFFF);
                    SET_CBUS_REG_MASK(UART1_CONTROL, (((xtal_uart_rate_backup / (115200 * 4)) - 1) & 0xfff));
                    CLEAR_AOBUS_REG_MASK(AO_UART_CONTROL, (1 << 19) | 0xFFF);
                    WRITE_AOBUS_REG_BITS(AO_UART_CONTROL, ((xtal_uart_rate_backup / (115200 * 4)) - 1) & 0xfff, 0, 12);
                }
            } else {
                clk_flag[i] = READ_CBUS_REG_BITS(clks[i], 8, 1) ? 1 : 0;
                if (clk_flag[i]) {
                    CLEAR_CBUS_REG_MASK(clks[i], (1 << 8));
                }
            }
            if (clk_flag[i]) {
                printf("clk %s(%x) off\n", clks_name[i], clks[i]);
            }
        }
    }
}
EXPORT_SYMBOL(clk_switch);

void early_clk_switch(int flag)
{
    int i;
    struct clk *sys_clk;

    if (flag) {
        for (i = EARLY_CLK_COUNT - 1; i >= 0; i--) {
            if (early_clk_flag[i]) {
                if ((early_clks[i] == HHI_VID_CLK_CNTL)||(early_clks[i] == HHI_VIID_CLK_CNTL)) {
                    WRITE_CBUS_REG_BITS(early_clks[i], early_clk_flag[i], 19, 2);
                } 
#ifdef EARLY_SUSPEND_USE_XTAL
                else if (early_clks[i] == HHI_MPEG_CLK_CNTL) {
                    udelay(1000);
                    SET_CBUS_REG_MASK(early_clks[i], (1 << 8)); // clk81 back to normal

                    CLEAR_CBUS_REG_MASK(UART0_CONTROL, (1 << 19) | 0xFFF);
                    SET_CBUS_REG_MASK(UART0_CONTROL, (((uart_rate_backup / (115200 * 4)) - 1) & 0xfff));
                    CLEAR_CBUS_REG_MASK(UART1_CONTROL, (1 << 19) | 0xFFF);
                    SET_CBUS_REG_MASK(UART1_CONTROL, (((uart_rate_backup / (115200 * 4)) - 1) & 0xfff));
                    CLEAR_AOBUS_REG_MASK(AO_UART_CONTROL, (1 << 19) | 0xFFF);
                    WRITE_AOBUS_REG_BITS(AO_UART_CONTROL, ((uart_rate_backup / (115200 * 4)) - 1) & 0xfff, 0, 12);
                } 
#endif
                else {
                    SET_CBUS_REG_MASK(early_clks[i], (1 << 8));
                }
                printf("late clk %s(%x) on\n", early_clks_name[i], early_clks[i]);
                early_clk_flag[i] = 0;
            }
        }
    } else {
        //sys_clk = clk_get_sys("clk81", NULL);
        uart_rate_backup = 200*1000*1000;//sys_clk->rate;
        //sys_clk = clk_get_sys("clk_xtal", NULL);
        xtal_uart_rate_backup = 24*1000*1000;//sys_clk->rate;

        for (i = 0; i < EARLY_CLK_COUNT; i++) {
            if ((early_clks[i] == HHI_VID_CLK_CNTL)||(early_clks[i] == HHI_VIID_CLK_CNTL)) {
                early_clk_flag[i] = READ_CBUS_REG_BITS(early_clks[i], 19, 2);
                if (early_clk_flag[i]) {
                    CLEAR_CBUS_REG_MASK(early_clks[i], (1<<19)|(1<<20));
                }
            } 
#ifdef EARLY_SUSPEND_USE_XTAL
            else if (early_clks[i] == HHI_MPEG_CLK_CNTL) {
                early_clk_flag[i] = 1;

                udelay(1000);
                CLEAR_CBUS_REG_MASK(early_clks[i], (1 << 8)); // 24M

                CLEAR_CBUS_REG_MASK(UART0_CONTROL, (1 << 19) | 0xFFF);
                SET_CBUS_REG_MASK(UART0_CONTROL, (((xtal_uart_rate_backup / (115200 * 4)) - 1) & 0xfff));
                CLEAR_CBUS_REG_MASK(UART1_CONTROL, (1 << 19) | 0xFFF);
                SET_CBUS_REG_MASK(UART1_CONTROL, (((xtal_uart_rate_backup / (115200 * 4)) - 1) & 0xfff));
                CLEAR_AOBUS_REG_MASK(AO_UART_CONTROL, (1 << 19) | 0xFFF);
                WRITE_AOBUS_REG_BITS(AO_UART_CONTROL, ((xtal_uart_rate_backup / (115200 * 4)) - 1) & 0xfff, 0, 12);
            }
#endif
            else {
                early_clk_flag[i] = READ_CBUS_REG_BITS(early_clks[i], 8, 1) ? 1 : 0;
                if (early_clk_flag[i]) {
                    CLEAR_CBUS_REG_MASK(early_clks[i], (1 << 8));
                }
            }
            if (early_clk_flag[i]) {
                printf("early clk %s(%x) off\n", early_clks_name[i], early_clks[i]);
            }
        }
    }
}
EXPORT_SYMBOL(early_clk_switch);

#define PLL_COUNT 4
static char pll_flag[PLL_COUNT];
static unsigned plls[PLL_COUNT] = {
    HHI_VID_PLL_CNTL,
    HHI_VIID_PLL_CNTL,
    HHI_AUD_PLL_CNTL,
    HHI_OTHER_PLL_CNTL,
};

static char plls_name[PLL_COUNT][32] = {
    "HHI_VID_PLL_CNTL",
    "HHI_VIID_PLL_CNTL",
    "HHI_AUD_PLL_CNTL",
    "HHI_OTHER_PLL_CNTL",
};

#define EARLY_PLL_COUNT 2
static char early_pll_flag[EARLY_PLL_COUNT];
static unsigned early_plls[EARLY_PLL_COUNT] = {
    HHI_VID_PLL_CNTL,
    HHI_VIID_PLL_CNTL,
};

static char early_plls_name[EARLY_PLL_COUNT][32] = {
    "HHI_VID_PLL_CNTL",
    "HHI_VIID_PLL_CNTL",
};

void pll_switch(int flag)
{
    int i;
    if (flag) {
        for (i = PLL_COUNT - 1; i >= 0; i--) {
            if (pll_flag[i]) {
                printf("pll %s(%x) on\n", plls_name[i], plls[i]);
                if ((plls[i]==HHI_VID_PLL_CNTL)||(plls[i]==HHI_VIID_PLL_CNTL)){
                    CLEAR_CBUS_REG_MASK(plls[i], (1 << 30));
                    pll_flag[i] = 0;
                }
                else{                
                    CLEAR_CBUS_REG_MASK(plls[i], (1 << 15));
                    pll_flag[i] = 0;
                }
            }
        }
        udelay(1000);
    } else {
        for (i = 0; i < PLL_COUNT; i++) {
        	  if ((plls[i]==HHI_VID_PLL_CNTL)||(plls[i]==HHI_VIID_PLL_CNTL))
        	  	  pll_flag[i] = READ_CBUS_REG_BITS(plls[i], 30, 1) ? 0 : 1;
        	  else	
                pll_flag[i] = READ_CBUS_REG_BITS(plls[i], 15, 1) ? 0 : 1;
            if (pll_flag[i]) {
                printf("pll %s(%x) off\n", plls_name[i], plls[i]);
                if ((plls[i]==HHI_VID_PLL_CNTL)||(plls[i]==HHI_VIID_PLL_CNTL)){
                    SET_CBUS_REG_MASK(plls[i], (1 << 30));
                }
                else{            
                    SET_CBUS_REG_MASK(plls[i], (1 << 15));
                }
            }
        }
    }
}
EXPORT_SYMBOL(pll_switch);

void early_pll_switch(int flag)
{
    int i;
    if (flag) {
        for (i = EARLY_PLL_COUNT - 1; i >= 0; i--) {
            if (early_pll_flag[i]) {
                if ((early_plls[i]==HHI_VID_PLL_CNTL)||(early_plls[i]==HHI_VIID_PLL_CNTL)){
                    CLEAR_CBUS_REG_MASK(early_plls[i], (1 << 30));
                    early_pll_flag[i] = 0;
                }
                else{
                    CLEAR_CBUS_REG_MASK(early_plls[i], (1 << 15));
                    early_pll_flag[i] = 0;
                }
                printf("late pll %s(%x) on\n", early_plls_name[i], early_plls[i]);
            }
        }
        udelay(1000);
    } else {
        for (i = 0; i < EARLY_PLL_COUNT; i++) {
            if ((early_plls[i]==HHI_VID_PLL_CNTL)||(early_plls[i]==HHI_VIID_PLL_CNTL))
                early_pll_flag[i] = READ_CBUS_REG_BITS(early_plls[i], 30, 1) ? 0 : 1;
            else
                early_pll_flag[i] = READ_CBUS_REG_BITS(early_plls[i], 15, 1) ? 0 : 1;
            if (early_pll_flag[i]) {
                printf("early pll %s(%x) off\n", early_plls_name[i], early_plls[i]);
                if ((early_plls[i]==HHI_VID_PLL_CNTL)||(early_plls[i]==HHI_VIID_PLL_CNTL))
                    SET_CBUS_REG_MASK(early_plls[i], (1 << 30));
                else
                    SET_CBUS_REG_MASK(early_plls[i], (1 << 15));
            }
        }
    }
}
EXPORT_SYMBOL(early_pll_switch);

typedef struct {
    char name[32];
    unsigned reg_addr;
    unsigned set_bits;
    unsigned clear_bits;
    unsigned reg_value;
    unsigned enable; // 1:cbus 2:apb 3:ahb 0:disable
} analog_t;

#define ANALOG_COUNT    3
static analog_t analog_regs[ANALOG_COUNT] = {
    {"SAR_ADC",             SAR_ADC_REG3,       1 << 28, (1 << 30) | (1 << 21),    0,  1},
#ifdef ADJUST_CORE_VOLTAGE
    {"LED_PWM_REG0",        LED_PWM_REG0,       1 << 13,          1 << 12,              0,  0}, // needed for core voltage adjustment, so not off
#else
    {"LED_PWM_REG0",        LED_PWM_REG0,       1 << 13,          1 << 12,              0,  1},
#endif
    {"VGHL_PWM_REG0",       VGHL_PWM_REG0,      1 << 13,          1 << 12,              0,  1},
};


void analog_switch(int flag)
{
    int i;
    unsigned reg_value = 0;

    if (flag) {
        printf("analog on\n");
        SET_CBUS_REG_MASK(AM_ANALOG_TOP_REG0, 1 << 1);      // set 0x206e bit[1] 1 to power on top analog
        for (i = 0; i < ANALOG_COUNT; i++) {
            if (analog_regs[i].enable && (analog_regs[i].set_bits || analog_regs[i].clear_bits)) {
                if (analog_regs[i].enable == 1) {
                    WRITE_CBUS_REG(analog_regs[i].reg_addr, analog_regs[i].reg_value);
                } else if (analog_regs[i].enable == 2) {
                    WRITE_APB_REG(analog_regs[i].reg_addr, analog_regs[i].reg_value);
                } else if (analog_regs[i].enable == 3) {
                    WRITE_AHB_REG(analog_regs[i].reg_addr, analog_regs[i].reg_value);
                }
            }
        }
    } else {
        printf("analog off\n");
        for (i = 0; i < ANALOG_COUNT; i++) {
            if (analog_regs[i].enable && (analog_regs[i].set_bits || analog_regs[i].clear_bits)) {
                if (analog_regs[i].enable == 1) {
                    analog_regs[i].reg_value = READ_CBUS_REG(analog_regs[i].reg_addr);
                    printf("%s(0x%x):0x%x", analog_regs[i].name, CBUS_REG_ADDR(analog_regs[i].reg_addr), analog_regs[i].reg_value);
                    if (analog_regs[i].clear_bits) {
                        CLEAR_CBUS_REG_MASK(analog_regs[i].reg_addr, analog_regs[i].clear_bits);
                        printf(" & ~0x%x", analog_regs[i].clear_bits);
                    }
                    if (analog_regs[i].set_bits) {
                        SET_CBUS_REG_MASK(analog_regs[i].reg_addr, analog_regs[i].set_bits);
                        printf(" | 0x%x", analog_regs[i].set_bits);
                    }
                    reg_value = READ_CBUS_REG(analog_regs[i].reg_addr);
                    printf(" = 0x%x\n", reg_value);
                } else if (analog_regs[i].enable == 2) {
                    analog_regs[i].reg_value = READ_APB_REG(analog_regs[i].reg_addr);
                    printf("%s(0x%x):0x%x", analog_regs[i].name, APB_REG_ADDR(analog_regs[i].reg_addr), analog_regs[i].reg_value);
                    if (analog_regs[i].clear_bits) {
                        CLEAR_APB_REG_MASK(analog_regs[i].reg_addr, analog_regs[i].clear_bits);
                        printf(" & ~0x%x", analog_regs[i].clear_bits);
                    }
                    if (analog_regs[i].set_bits) {
                        SET_APB_REG_MASK(analog_regs[i].reg_addr, analog_regs[i].set_bits);
                        printf(" | 0x%x", analog_regs[i].set_bits);
                    }
                    reg_value = READ_APB_REG(analog_regs[i].reg_addr);
                    printf(" = 0x%x\n", reg_value);
                } else if (analog_regs[i].enable == 3) {
                    analog_regs[i].reg_value = READ_AHB_REG(analog_regs[i].reg_addr);
                    printf("%s(0x%x):0x%x", analog_regs[i].name, AHB_REG_ADDR(analog_regs[i].reg_addr), analog_regs[i].reg_value);
                    if (analog_regs[i].clear_bits) {
                        CLEAR_AHB_REG_MASK(analog_regs[i].reg_addr, analog_regs[i].clear_bits);
                        printf(" & ~0x%x", analog_regs[i].clear_bits);
                    }
                    if (analog_regs[i].set_bits) {
                        SET_AHB_REG_MASK(analog_regs[i].reg_addr, analog_regs[i].set_bits);
                        printf(" | 0x%x", analog_regs[i].set_bits);
                    }
                    reg_value = READ_AHB_REG(analog_regs[i].reg_addr);
                    printf(" = 0x%x\n", reg_value);
                }
            }
        }
        CLEAR_CBUS_REG_MASK(AM_ANALOG_TOP_REG0, 1 << 1);    // set 0x206e bit[1] 0 to shutdown top analog
    }
}

void usb_switch(int is_on, int ctrl)
{
    int index, por;

    if (ctrl == 0) {
        index = USB_CTL_INDEX_A;
    } else {
        index = USB_CTL_INDEX_B;
    }

    if (is_on) {
        por = USB_CTL_POR_ON;
    } else {
        por = USB_CTL_POR_OFF;
    }

    set_usb_ctl_por(index, por);
}

#define         MODE_DELAYED_WAKE       0
#define         MODE_IRQ_DELAYED_WAKE   1
#define         MODE_IRQ_ONLY_WAKE      2

static void auto_clk_gating_setup(
    unsigned long sleep_dly_tb, unsigned long mode, unsigned long clear_fiq, unsigned long clear_irq,
    unsigned long   start_delay, unsigned long   clock_gate_dly, unsigned long   sleep_time, unsigned long   enable_delay)
{
}
extern void lcd_disable(void);
extern int lcd_enable(void);
int enter_boot_power_down()
{
    int key;
    int ac;
    
    power_gate_init();
    lcd_disable();
    video_dac_disable();
    camera_power_off() ;
    vcc2_power_off();
    //turn_off_audio_DAC();
    if (!early_suspend_flag) {
        printf("\n boot_suspend \n");
//        if (pdata->set_exgpio_early_suspend) {
//            pdata->set_exgpio_early_suspend(OFF);
//        }
        early_power_gate_switch(OFF);
        early_clk_switch(OFF);
        early_pll_switch(OFF);
        early_suspend_flag = 1;
    }
    
    printf("enter boot_pm_suspend!\n");
    analog_switch(OFF);
    usb_switch(OFF, 0);
    usb_switch(OFF, 1);
//    if (pdata->set_vccx2) {
//        pdata->set_vccx2(OFF);
//    }
    power_gate_switch(OFF);
    clk_switch(OFF);
    pll_switch(OFF);
    clrbits_le32(P_HHI_SYS_CPU_CLK_CNTL,1<<7); //change A9-->24M 

    
    printf("boot sleep ...\n");
//	WRITE_MPEG_REG_BITS(HHI_MALI_CLK_CNTL, 0, 9, 3); // mali use xtal
//    CLEAR_CBUS_REG_MASK(HHI_SYS_CPU_CLK_CNTL, 1<<7);  // a9 use xtal
//    SET_CBUS_REG_MASK(HHI_SYS_PLL_CNTL, (1 << 15));   // turn off sys pll
//    
//    meson_power_suspend();
    
    key = powerkey_scan();
    ac = axp_charger_is_ac_online();
    while((!key)&&ac) 
    {
        key = powerkey_scan();
        ac = axp_charger_is_ac_online();
    }
    setbits_le32(P_HHI_SYS_CPU_CLK_CNTL,1<<7); //change A9-->normal 

    
    printf("boot... wake up\n");
//    if (pdata->set_vccx2) {
//        pdata->set_vccx2(ON);
//    }
    SET_CBUS_REG_MASK(HHI_SYS_CPU_CLK_CNTL, (1 << 7));  // a9 use pll
    WRITE_MPEG_REG_BITS(HHI_MALI_CLK_CNTL, 3, 9, 3); // mali use pll

    pll_switch(ON);
    clk_switch(ON);
    power_gate_switch(ON);
    usb_switch(ON, 0);
    usb_switch(ON, 1);
    analog_switch(ON);
    if (early_suspend_flag) {
        early_pll_switch(ON);
        early_clk_switch(ON);
        early_power_gate_switch(ON);
        early_suspend_flag = 0;
        printf("boot sys_resume\n");
    }    
    
    printf("mlvds init\n");
    lcd_enable();  
    printf("mlvds init finish\n");  
    
    return ac;
}