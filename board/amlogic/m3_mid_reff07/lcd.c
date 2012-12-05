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
#include <asm/arch/lcd.h>
#include <asm/arch/osd.h>
#include <asm/arch/osd_hw.h>
#include <aml_i2c.h>
#include <lcd_aml.h>
#ifdef CONFIG_SN7325
#include <sn7325.h>
#endif

#define DEBUG

static void lvds_port_enable(void);
static void lvds_port_disable(void);

static void lcd_power_on(void);
static void lcd_power_off(void);
void power_on_backlight(void);
void power_off_backlight(void);
extern int lvds_probe(void);
extern int lvds_remove(void);
static void set_backlight_level(unsigned level);
static unsigned aml_get_bl_level(void);

//HV070WSA_100
#define LCD_WIDTH       1024
#define LCD_HEIGHT      600
#define MAX_WIDTH       1344
#define MAX_HEIGHT      635
#define VIDEO_ON_PIXEL  80
#define VIDEO_ON_LINE   22

/*
For 8726M, cpt CLAA070MA22BW panel 7" */ 
/*#define LCD_WIDTH       800 
#define LCD_HEIGHT      600
#define MAX_WIDTH       1010
#define MAX_HEIGHT      660
#define VIDEO_ON_LINE   22
*/
#define DEFAULT_BL_LEVEL	128

// Define LVDS physical PREM SWING VCM REF
static lvds_phy_control_t lcd_lvds_phy_control = 
{
    .lvds_prem_ctl = 0x1,		//0xf  
    .lvds_swing_ctl = 0x1,	    //0x3  
    .lvds_vcm_ctl = 0x1,
    .lvds_ref_ctl = 0xf, 
};

//Define LVDS data mapping, bit num.
static lvds_config_t lcd_lvds_config=
{
	.lvds_repack=1,   //data mapping  //0:THine mode, 1:VESA mode
	.pn_swap=0,		  //0:normal, 1:swap
	.bit_num=1,       // 0:10bits, 1:8bits, 2:6bits, 3:4bits
	.lvds_phy_control = &lcd_lvds_phy_control,
};

int  video_dac_enable(unsigned char enable_mask)
{
	debug("%s\n", __FUNCTION__);
	CLEAR_CBUS_REG_MASK(VENC_VDAC_SETTING, enable_mask&0x1f);
	return 0;
}

int  video_dac_disable()
{
	debug("%s\n", __FUNCTION__);
	SET_CBUS_REG_MASK(VENC_VDAC_SETTING, 0x1f);
    return 0;    
}   

lcdConfig_t lcd_config =
{
    .width      = LCD_WIDTH,
    .height     = LCD_HEIGHT,
    .max_width  = MAX_WIDTH,
    .max_height = MAX_HEIGHT,
	  .video_on_pixel = VIDEO_ON_PIXEL,
    .video_on_line = VIDEO_ON_LINE,
   // .pll_ctrl = 0x063c,      
    .pll_ctrl = 0x10233,//0x10216
    .div_ctrl = 0x18813,
    .clk_ctrl = 0x1111,
    .gamma_cntl_port = (1 << LCD_GAMMA_EN) | (0 << LCD_GAMMA_RVS_OUT) | (1 << LCD_GAMMA_VCOM_POL),
    .gamma_vcom_hswitch_addr = 0,
    .rgb_base_addr = 0xf0,
    .rgb_coeff_addr = 0x74a,
    .pol_cntl_addr = (0x0 << LCD_CPH1_POL) |(0x1 << LCD_HS_POL) | (0x1 << LCD_VS_POL),
    .dith_cntl_addr = 0x600,
    .sth1_hs_addr = 10,
    .sth1_he_addr = 20,
    .sth1_vs_addr = 0,
    .sth1_ve_addr = MAX_HEIGHT-1,
    .stv1_hs_addr = 10,
    .stv1_he_addr = 20,
    .stv1_vs_addr = 2,
    .stv1_ve_addr = 4,
    .inv_cnt_addr = (0<<LCD_INV_EN) | (0<<LCD_INV_CNT),
    .tcon_misc_sel_addr = (1<<LCD_STV1_SEL) | (1<<LCD_STV2_SEL),
    .dual_port_cntl_addr = (1<<LCD_TTL_SEL) | (1<<LCD_ANALOG_SEL_CPH3) | (1<<LCD_ANALOG_3PHI_CLK_SEL) ,
    .flags = LCD_DIGITAL_LVDS,
    .screen_width = 16,
    .screen_height = 9,
    .sync_duration_num = 512,
    .sync_duration_den = 10,
	  .lvds_config = &lcd_lvds_config,
    .power_on=lcd_power_on,
    .power_off=lcd_power_off,
};

static void lcd_setup_gama_table(lcdConfig_t *pConf)
{
    int i;
    /* is OK*/
	debug("%s\n", __FUNCTION__);
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
	debug("%s\n", __FUNCTION__);

	
	/*BL_EN -> GPIOD_1: 1
    WRITE_CBUS_REG(0x2013, READ_CBUS_REG(0x2013)|(1<<17));
    WRITE_CBUS_REG(0x2012, READ_CBUS_REG(0x2012)&(~(1<<17)));
    set_gpio_val(GPIOD_bank_bit0_9(1), GPIOD_bit_bit0_9(1), 1);
    set_gpio_mode(GPIOD_bank_bit0_9(1), GPIOD_bit_bit0_9(1), GPIO_OUTPUT_MODE);*/

	
	mdelay(20);
	    lvds_port_enable();
	mdelay(100);
		set_backlight_level(0);
    WRITE_MPEG_REG(PERIPHS_PIN_MUX_0, READ_MPEG_REG(PERIPHS_PIN_MUX_0) & ~(1 << 19));
	WRITE_MPEG_REG(PERIPHS_PIN_MUX_3, READ_MPEG_REG(PERIPHS_PIN_MUX_3) & ~((1 << 24)|(1 << 25)));    
    WRITE_MPEG_REG(PERIPHS_PIN_MUX_7, READ_MPEG_REG(PERIPHS_PIN_MUX_7) & ~(1 << 17));

		set_gpio_val(GPIOC_bank_bit0_15(9), GPIOC_bit_bit0_15(9), 1);    //set GPIOC_9 as  BL_EN low to power off backlight
		set_gpio_mode(GPIOC_bank_bit0_15(9), GPIOC_bit_bit0_15(9), GPIO_OUTPUT_MODE);//set CM_PWDN1 output
		mdelay(10);
		WRITE_MPEG_REG(PERIPHS_PIN_MUX_2, READ_MPEG_REG(PERIPHS_PIN_MUX_2)|(1 << 3));	//GPIOD_1 enable PWM_D for LCD_BL_PWM
		WRITE_MPEG_REG(PERIPHS_PIN_MUX_1, READ_MPEG_REG(PERIPHS_PIN_MUX_1)&~(1 << 28));	//GPIOD_1 disable LCD_VGHL_PWM
		SET_CBUS_REG_MASK(PWM_MISC_REG_CD, (1 << 1)|(1 << 23));	//Enable PWM_D clock,Enable PWM_D output,select XTAL
   // WRITE_MPEG_REG(PREG_PAD_GPIO2_O, READ_MPEG_REG(PREG_PAD_GPIO2_O) | (1 << 9));    
	
		mdelay(100);
		set_backlight_level(250);
	

}
#define PWM_MAX			3000   //set pwm_freq=24MHz/PWM_MAX (Base on XTAL frequence: 24MHz, 0<PWM_MAX<65535)
#define BL_MAX_LEVEL	255
#define BL_MIN_LEVEL	0
void power_off_backlight(void)
{
	debug("%s\n", __FUNCTION__);
	//BL_EN -> GPIOD_1: 0
	
	/*set_gpio_val(GPIOD_bank_bit0_9(1), GPIOD_bit_bit0_9(1), 0);
	set_gpio_mode(GPIOD_bank_bit0_9(1), GPIOD_bit_bit0_9(1), GPIO_OUTPUT_MODE);*/
	
    WRITE_MPEG_REG(PERIPHS_PIN_MUX_0, READ_MPEG_REG(PERIPHS_PIN_MUX_0) & ~(1 << 19));
	WRITE_MPEG_REG(PERIPHS_PIN_MUX_3, READ_MPEG_REG(PERIPHS_PIN_MUX_3) & ~((1 << 24)|(1 << 25)));    
    WRITE_MPEG_REG(PERIPHS_PIN_MUX_7, READ_MPEG_REG(PERIPHS_PIN_MUX_7) & ~(1 << 17));

	set_gpio_val(GPIOC_bank_bit0_15(9), GPIOC_bit_bit0_15(9), 0);    //set GPIOC_9 as  BL_EN low to power off backlight
	set_gpio_mode(GPIOC_bank_bit0_15(9), GPIOC_bit_bit0_15(9), GPIO_OUTPUT_MODE);//set CM_PWDN1 output
	
	WRITE_CBUS_REG_BITS(PWM_PWM_D, PWM_MAX, 0, 16);  //pwm low
	WRITE_CBUS_REG_BITS(PWM_PWM_D, 0, 16, 16);  //pwm high
    
    CLEAR_CBUS_REG_MASK(PWM_MISC_REG_CD, ((1 << 23) | (1<<1)));  //disable pwm clk & pwm output	
 //   WRITE_MPEG_REG(PREG_PAD_GPIO2_O, READ_MPEG_REG(PREG_PAD_GPIO2_O) & ~(1 << 9)); 
	
	mdelay(20);
	lvds_port_disable();
	mdelay(20);
}

static void power_on_lcd(void)
{
	/*debug("%s\n", __FUNCTION__);
	//GPIOA27 -> LCD_PWR_EN#: 0  lcd 3.3v
    set_gpio_val(GPIOA_bank_bit0_27(27), GPIOA_bit_bit0_27(27), 0);
    set_gpio_mode(GPIOA_bank_bit0_27(27), GPIOA_bit_bit0_27(27), GPIO_OUTPUT_MODE);
    mdelay(30);
    
    //GPIOC2 -> VCCx3_EN: 1
    set_gpio_val(GPIOC_bank_bit0_15(2), GPIOC_bit_bit0_15(2), 1);
    set_gpio_mode(GPIOC_bank_bit0_15(2), GPIOC_bit_bit0_15(2), GPIO_OUTPUT_MODE);
    mdelay(30);*/
    
	      //LCD_PWR_EN# LCD_3.0V
    WRITE_MPEG_REG(PERIPHS_PIN_MUX_0, READ_MPEG_REG(PERIPHS_PIN_MUX_0) & ~(1 << 10));
   	WRITE_MPEG_REG(PERIPHS_PIN_MUX_7, READ_MPEG_REG(PERIPHS_PIN_MUX_7) & ~(1 << 15));    
    WRITE_MPEG_REG(PREG_PAD_GPIO0_EN_N, READ_MPEG_REG(PREG_PAD_GPIO0_EN_N) & ~(1 << 27));
   	WRITE_MPEG_REG(PREG_PAD_GPIO0_O, READ_MPEG_REG(PREG_PAD_GPIO0_O) & ~(1 << 27)); 
	  mdelay(10);
}

static void power_off_lcd(void)
{
	debug("%s\n", __FUNCTION__);

	//LCD_PWR_EN# LCD_3.0V
    WRITE_MPEG_REG(PERIPHS_PIN_MUX_0, READ_MPEG_REG(PERIPHS_PIN_MUX_0) & ~(1 << 10));
   	WRITE_MPEG_REG(PERIPHS_PIN_MUX_7, READ_MPEG_REG(PERIPHS_PIN_MUX_7) & ~(1 << 15));    
    WRITE_MPEG_REG(PREG_PAD_GPIO0_EN_N, READ_MPEG_REG(PREG_PAD_GPIO0_EN_N) & ~(1 << 27));
	      WRITE_MPEG_REG(PREG_PAD_GPIO0_O, READ_MPEG_REG(PREG_PAD_GPIO0_O) | (1 << 27)); 
	  mdelay(10);
}
static void lvds_port_enable(void)
{	
//	printk("\n\nLVDS port enable.\n");	
    WRITE_MPEG_REG(LVDS_GEN_CNTL, READ_MPEG_REG(LVDS_GEN_CNTL) | (1<<3));
	//enable minilvds_data channel
	WRITE_MPEG_REG(LVDS_PHY_CNTL4, READ_MPEG_REG(LVDS_PHY_CNTL4) | (0x2f<<0));
	mdelay(30);
}
static void lvds_port_disable(void)
{		
	//disable minilvds_data channel
	WRITE_MPEG_REG(LVDS_PHY_CNTL4, READ_MPEG_REG(LVDS_PHY_CNTL4) & ~(0x7f<<0));		
	WRITE_MPEG_REG(LVDS_GEN_CNTL, READ_MPEG_REG(LVDS_GEN_CNTL) & ~(1<<3));
}

static void set_tcon_pinmux(void)
{
	debug("%s\n", __FUNCTION__);
	/* TCON control pins pinmux */
    clear_mio_mux(1, 0x0f<<11); // disable cph50(11),cph1(12),cph2(13),cph3(14)
#ifdef USE_CLKO
    set_mio_mux(1, 1<<21); // enable clko
#else
    set_mio_mux(1, 1<<14); // enable cph1
#endif
    set_mio_mux(1, 1<<17); // enable oeh
    set_mio_mux(0, 0x3f<<0);   //For 8bits RGB
}
static void lcd_power_on(void)
{
	debug("%s\n", __FUNCTION__);
	video_dac_disable();
   // set_tcon_pinmux();
    power_on_lcd();
    #ifdef CONFIG_HAVE_SYS_TEST
    power_on_backlight();	//changed by Elvis
    #endif        
}
static void lcd_power_off(void)
{
	debug("%s\n", __FUNCTION__);
	//power_off_backlight();
    power_off_lcd();
}

#define ELVIS_PANEL 1
#if defined(ELVIS_PANEL)

#define PWM_TCNT        (600-1)
#define PWM_MAX_VAL    (420)

static void aml_8726m_bl_init(void)
{
	debug("%s\n", __FUNCTION__);
	SET_CBUS_REG_MASK(PWM_MISC_REG_AB, (1 << 0));
    SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_2, (1<<31));
}
static unsigned bl_level;

static unsigned aml_get_bl_level(void)
{
    return bl_level;
}

static void set_backlight_level(unsigned level)
{
	printf("**********set_backlight_level level is %d******************** \n",level);
	bl_level=level;
	level = level>BL_MAX_LEVEL ? BL_MAX_LEVEL:(level<BL_MIN_LEVEL ? BL_MIN_LEVEL:level);		
	level = level * PWM_MAX / BL_MAX_LEVEL -8*level;	
    WRITE_MPEG_REG(PWM_PWM_D, (level << 16) | ((PWM_MAX - level) << 0));
}


#endif

static void lcd_io_init(void)
{
    debug("\n\nT13 LCD Init.\n\n");

    //set_tcon_pinmux();

    power_on_lcd();
#ifdef CONFIG_HAVE_SYS_TEST
    power_on_backlight();
#endif
}

int osd_hw_setup(void)
{
    debug("%s\n",__FUNCTION__);
    osd_setup(	0,
                0,
                lcd_config.width,
                lcd_config.height,
                lcd_config.width,
                lcd_config.height * 2,
                0,
                0,
                lcd_config.width - 1,
                lcd_config.height - 1,
                FB_ADDR,
                &default_color_format_array[COLOR_INDEX_24_RGB],
                CURRENT_OSD);
    return 0;
}

int lcd_enable(void)
{

    debug("%s\n",__FUNCTION__);
    lcd_setup_gama_table(&lcd_config);
    lcd_io_init();
    lvds_probe();
    osd_init_hw();
    osd_hw_setup();
    return 0;
}

void lcd_disable(void)
{
	debug("%s\n", __FUNCTION__);
	//power_off_backlight();
    power_off_lcd();
    lvds_remove();
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
	.set_bl_level	=	set_backlight_level,
};
