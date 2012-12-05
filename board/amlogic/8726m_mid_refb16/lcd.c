/*
 * AMLOGIC T13 LCD panel driver.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the named License,
 * or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Author:  Elvis Yu <elvis.yu@amlogic.com>
 *
 */

/*
For Ramos MID use 8726M */ 
#include <common.h>
#include <asm/arch/am_regs.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/gpio.h>
#include <asm/arch/tcon.h>
#include <asm/arch/osd.h>
#include <asm/arch/osd_hw.h>
#include <aml_i2c.h>
#include <lcd_aml.h>
#include <sn7325.h>


#define msleep(a) udelay(a * 1000)

#define LCD_WIDTH       1024
#define LCD_HEIGHT      768
#define MAX_WIDTH       2084
#define MAX_HEIGHT      800
#define VIDEO_ON_LINE   22
#define DEFAULT_BL_LEVEL	128

static void lcd_power_on(void);
static void lcd_power_off(void);
void power_on_backlight(void);
void power_off_backlight(void);

int  video_dac_enable(unsigned char enable_mask)
{
    CLEAR_CBUS_REG_MASK(VENC_VDAC_SETTING, enable_mask&0x1f);
	  return 0;
}

int  video_dac_disable()
{
    SET_CBUS_REG_MASK(VENC_VDAC_SETTING, 0x1f);
    return 0;    
}   

tcon_conf_t tcon_config =
{
    .width      = LCD_WIDTH,
    .height     = LCD_HEIGHT,
    .max_width  = MAX_WIDTH,
    .max_height = MAX_HEIGHT,
    .video_on_line = VIDEO_ON_LINE,    
    .pll_ctrl = 0x619, //clk=100M, frame_rate=60Hz
    .clk_ctrl = 0x401, 
    .gamma_cntl_port = (1 << LCD_GAMMA_EN) | (0 << LCD_GAMMA_RVS_OUT) | (1 << LCD_GAMMA_VCOM_POL),
    .gamma_vcom_hswitch_addr = 0,
    .rgb_base_addr = 0xf0,
    .rgb_coeff_addr = 0x74a,
    .pol_cntl_addr = (0x0 << LCD_CPH1_POL) |(0x1 << LCD_HS_POL) | (0x1 << LCD_VS_POL),
    .dith_cntl_addr = 0x600,
    .sth1_hs_addr = 0,
    .sth1_he_addr = 0,
    .sth1_vs_addr = 0,
    .sth1_ve_addr = 0,
    .sth2_hs_addr = 0,
    .sth2_he_addr = 0,
    .sth2_vs_addr = 0,
    .sth2_ve_addr = 0,
    .oeh_hs_addr = 67,
    .oeh_he_addr = 67+LCD_WIDTH,
    .oeh_vs_addr = VIDEO_ON_LINE,
    .oeh_ve_addr = VIDEO_ON_LINE+LCD_HEIGHT-1,
    .vcom_hswitch_addr = 0,
    .vcom_vs_addr = 0,
    .vcom_ve_addr = 0,
    .cpv1_hs_addr = 0,
    .cpv1_he_addr = 0,
    .cpv1_vs_addr = 0,
    .cpv1_ve_addr = 0,
    .cpv2_hs_addr = 0,
    .cpv2_he_addr = 0,
    .cpv2_vs_addr = 0,
    .cpv2_ve_addr = 0,
    .stv1_hs_addr = 0,
    .stv1_he_addr = 0,
    .stv1_vs_addr = 0,
    .stv1_ve_addr = 0,
    .stv2_hs_addr = 0,
    .stv2_he_addr = 0,
    .stv2_vs_addr = 0,
    .stv2_ve_addr = 0,
    .oev1_hs_addr = 0,
    .oev1_he_addr = 0,
    .oev1_vs_addr = 0,
    .oev1_ve_addr = 0,
    .oev2_hs_addr = 0,
    .oev2_he_addr = 0,
    .oev2_vs_addr = 0,
    .oev2_ve_addr = 0,
    .oev3_hs_addr = 0,
    .oev3_he_addr = 0,
    .oev3_vs_addr = 0,
    .oev3_ve_addr = 0,
    .inv_cnt_addr = (0<<LCD_INV_EN) | (0<<LCD_INV_CNT),
    .tcon_misc_sel_addr = (1<<LCD_STV1_SEL) | (1<<LCD_STV2_SEL),
    .dual_port_cntl_addr = (1<<LCD_TTL_SEL) | (1<<LCD_ANALOG_SEL_CPH3) | (1<<LCD_ANALOG_3PHI_CLK_SEL),
    .flags = 0,
    .screen_width = 4,
    .screen_height = 3,
    .sync_duration_num = 60,
    .sync_duration_den = 1,
    .power_on=lcd_power_on,
    .power_off=lcd_power_off,
};

static void lcd_setup_gama_table(tcon_conf_t *pConf)
{
    int i;
    const unsigned short gamma_adjust[256] = {
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
        32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
        64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
        96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
        128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
        160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
        192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
        224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
    };

    for (i=0; i<256; i++) {
        pConf->GammaTableR[i] = gamma_adjust[i] << 2;
        pConf->GammaTableG[i] = gamma_adjust[i] << 2;
        pConf->GammaTableB[i] = gamma_adjust[i] << 2;
    }
}

void power_on_backlight(void)
{
    //BL_PWM -> GPIOA_7: 1
    msleep(200);
    set_gpio_val(GPIOA_bank_bit(7), GPIOA_bit_bit0_14(7), 1);
    set_gpio_mode(GPIOA_bank_bit(7), GPIOA_bit_bit0_14(7), GPIO_OUTPUT_MODE);
}

void power_off_backlight(void)
{
    //BL_PWM -> GPIOA_7: 0
    set_gpio_val(GPIOA_bank_bit(7), GPIOA_bit_bit0_14(7), 0);
    set_gpio_mode(GPIOA_bank_bit(7), GPIOA_bit_bit0_14(7), GPIO_OUTPUT_MODE);
}

static void power_on_lcd(void)
{
    clear_mio_mux(0, ((1<<10) | (1<<19) | (1<<20)));		
    clear_mio_mux(12, (1<<8));		
    clear_mio_mux(9, (1<<21)); 
    set_gpio_val(GPIOA_bank_bit(6), GPIOA_bit_bit0_14(6), 1);   // VCCx2_EN level = 1 (pull high)
    set_gpio_mode(GPIOA_bank_bit(6), GPIOA_bit_bit0_14(6), GPIO_OUTPUT_MODE);   // OEN = 0 (output)
    //EIO -> OD4
    sn7325_set_io_dir(sn7325_get_io_dir() & ~OD4);

    //LCD3.3V
    sn7325_set_output_level(sn7325_get_output_level() |OD4);
    
    //EIO -> OD0: 0
    sn7325_set_io_dir(sn7325_get_io_dir() & ~OD0);

    //LCD3.3V  EIO -> OD0: 0
    sn7325_set_output_level(sn7325_get_output_level() & ~OD0);

}

static void power_off_lcd(void)
{
    set_gpio_val(GPIOA_bank_bit(6), GPIOA_bit_bit0_14(6), 0);   // VCCx2_EN level = 0 (pull low)
    //LCD3.3V  EIO -> OD7: 1
    sn7325_set_output_level(sn7325_get_output_level() | OD7);
    msleep(20);
}

static void set_tcon_pinmux(void)
{
    /* TCON control pins pinmux */
    /* GPIOA_5 -> LCD_Clk, GPIOA_0 -> TCON_STH1, GPIOA_1 -> TCON_STV1, GPIOA_2 -> TCON_OEH, */
    set_mio_mux(0, ((1<<11)|(1<<14)));
    //set_mio_mux(4,(3<<0)|(3<<2)|(3<<4));   //For 8bits
    set_mio_mux(4,(1<<0)|(1<<2)|(1<<4));   //For 6bits
    //PP1 -> UPDN:0, PP2 -> SHLR:1
    sn7325_set_io_dir(sn7325_get_io_dir() & ~(PP1 | PP2));
    sn7325_set_output_level(sn7325_get_output_level() & ~PP1);
    sn7325_set_output_level(sn7325_get_output_level() | PP2);
}
static void lcd_power_on(void)
{
    video_dac_disable();
    set_tcon_pinmux();
    power_on_lcd();
	#ifdef CONFIG_HAVE_SYS_TEST
    power_on_backlight();	//changed by Elvis
    #endif  
}
static void lcd_power_off(void)
{
    power_off_backlight();
    power_off_lcd();
}

#define ELVIS_PANEL 1
#if defined(ELVIS_PANEL)

#define PWM_TCNT        (600-1)
#define PWM_MAX_VAL    (420)

static void aml_8726m_bl_init(void)
{
    unsigned val;
    
    WRITE_CBUS_REG_BITS(PERIPHS_PIN_MUX_0, 0, 22, 1);
    WRITE_CBUS_REG_BITS(PREG_AM_ANALOG_ADDR, 1, 0, 1);
    WRITE_CBUS_REG(VGHL_PWM_REG0, 0);
    WRITE_CBUS_REG(VGHL_PWM_REG1, 0);
    WRITE_CBUS_REG(VGHL_PWM_REG2, 0);
    WRITE_CBUS_REG(VGHL_PWM_REG3, 0);
    WRITE_CBUS_REG(VGHL_PWM_REG4, 0);
    val = (0 << 31)           |       // disable the overall circuit
          (0 << 30)           |       // 1:Closed Loop  0:Open Loop
          (PWM_TCNT << 16)    |       // PWM total count
          (0 << 13)           |       // Enable
          (1 << 12)           |       // enable
          (0 << 10)           |       // test
          (3 << 7)            |       // CS0 REF, Voltage FeedBack: about 0.27V
          (7 << 4)            |       // CS1 REF, Current FeedBack: about 0.54V
          (0 << 0);                   // DIMCTL Analog dimmer
    WRITE_CBUS_REG(VGHL_PWM_REG0, val);
    val = (1 << 30)           |       // enable high frequency clock
          (PWM_MAX_VAL << 16) |       // MAX PWM value
          (0 << 0);                  // MIN PWM value
    WRITE_CBUS_REG(VGHL_PWM_REG1, val);
    val = (0 << 31)       |       // disable timeout test mode
          (0 << 30)       |       // timeout based on the comparator output
          (0 << 16)       |       // timeout = 10uS
          (0 << 13)       |       // Select oscillator as the clock (just for grins)
          (1 << 11)       |       // 1:Enable OverCurrent Portection  0:Disable
          (3 << 8)        |       // Filter: shift every 3 ticks
          (0 << 6)        |       // Filter: count 1uS ticks
          (0 << 5)        |       // PWM polarity : negative
          (0 << 4)        |       // comparator: negative, Different with NikeD3
          (1 << 0);               // +/- 1
    WRITE_CBUS_REG(VGHL_PWM_REG2, val);
    val = (   1 << 16) |    // Feedback down-sampling = PWM_freq/1 = PWM_freq
          (   1 << 14) |    // enable to re-write MATCH_VAL
          ( 210 <<  0) ;  // preset PWM_duty = 50%
    WRITE_CBUS_REG(VGHL_PWM_REG3, val);
    val = (   0 << 30) |    // 1:Digital Dimmer  0:Analog Dimmer
          (   2 << 28) |    // dimmer_timebase = 1uS
          (1000 << 14) |    // Digital dimmer_duty = 0%, the most darkness
          (1000 <<  0) ;    // dimmer_freq = 1KHz
    WRITE_CBUS_REG(VGHL_PWM_REG4, val);
}
static unsigned bl_level;

static unsigned aml_8726m_get_bl_level(void)
{
//    unsigned level = 0;
//
//    WRITE_CBUS_REG_BITS(VGHL_PWM_REG0, 1, 31, 1);
//    WRITE_CBUS_REG_BITS(VGHL_PWM_REG4, 0, 30, 1);
//    level = READ_CBUS_REG_BITS(VGHL_PWM_REG0, 0, 4);
    return bl_level;
}
#define BL_MAX_LEVEL 60000
static void aml_8726m_set_bl_level(unsigned level)
{
    unsigned cs_level,pwm_level,low,hi;
    
    bl_level = level;
        
    level = level*179/255;
    if(level>=120){ //120 - 179
        cs_level = 9 -(level - 120)/15;
        pwm_level = 85 + (level - 120)%15;   
    }
    else if(level>=20){ //20 - 119
        cs_level = 13 - (level -20)/25;
        pwm_level = 75 + (level - 20)%25;   
    }
    else{  //  <20
        cs_level = 13;
        pwm_level = 0;           
    }
        
    hi = (BL_MAX_LEVEL/100)*pwm_level;
    low = BL_MAX_LEVEL - hi;
    WRITE_CBUS_REG_BITS(VGHL_PWM_REG0, cs_level, 0, 4);   
    //SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_2, (1<<31)); 
    SET_CBUS_REG_MASK(PWM_MISC_REG_AB, (1 << 0));         
    WRITE_CBUS_REG_BITS(PWM_PWM_A,low,0,16);  //low
    WRITE_CBUS_REG_BITS(PWM_PWM_A,hi,16,16);  //hi    
}

#endif

static void lcd_io_init(void)
{
    debug("\n\nT13 LCD Init.\n\n");

    set_tcon_pinmux();

    power_on_lcd();
	#ifdef CONFIG_HAVE_SYS_TEST
    power_on_backlight();
	#endif
    aml_8726m_set_bl_level(DEFAULT_BL_LEVEL);
}

int osd_hw_setup(void)
{
    osd_setup(0,
                    0,
                    tcon_config.width,
                    tcon_config.height,
                    tcon_config.width,
                    tcon_config.height * 2,
                    0,
                    0,
                    tcon_config.width - 1,
                    tcon_config.height - 1,
                    FB_ADDR,
                    &default_color_format_array[COLOR_INDEX_24_RGB],
                    CURRENT_OSD);
}

int lcd_enable(void)
{
    sn7325_init();	
    
    lcd_setup_gama_table(&tcon_config);

    aml_8726m_bl_init();

    lcd_io_init();

    tcon_probe();

    osd_init_hw();

    osd_hw_setup();

    return 0;
}

void lcd_disable(void)
{
    power_off_backlight();
    power_off_lcd();
    tcon_remove();
}

vidinfo_t panel_info = 
{
	.vl_col	=	LCD_WIDTH,		/* Number of columns (i.e. 160) */
	.vl_row	=	LCD_HEIGHT,		/* Number of rows (i.e. 100) */

	.vl_bpix	=	COLOR_INDEX_24_RGB,				/* Bits per pixel, 24bpp */

	.lcd_base	=	FB_ADDR,		/* Start of framebuffer memory	*/

	.lcd_console_address	=	NULL,	/* Start of console buffer	*/
	.console_col	=	0,
	.console_row	=	0,
	
	.lcd_color_fg	=	0xffffff,
	.lcd_color_bg	=	0,
	.max_bl_level	=	255,

	.cmap	=	NULL,		/* Pointer to the colormap */

	.priv		=	NULL,			/* Pointer to driver-specific data */

	.lcd_enable	=	lcd_enable,
	.lcd_disable	=	lcd_disable,
	.lcd_bl_on	=	power_on_backlight,
	.lcd_bl_off	=	power_off_backlight,
	.set_bl_level	=	aml_8726m_set_bl_level,
};
