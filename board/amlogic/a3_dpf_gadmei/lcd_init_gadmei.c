#include <asm/arch/firm/io.h>
#include <asm/arch/firm/reg_addr.h>
#include <a3_osd_display/a3_16pp.h>
#include <a3_osd_display/bmp.h>
#include <mmc.h>
#include <aml_a3_hard_i2c.h>
#define OE2_CHANNEL 0
#define STV_CHANNEL 1
#define TP1_CHANNEL 2
#define CPV_CHANNEL 3
#define OE1_CHANNEL 4
#define POL_CHANNEL 5
#define EVEN_CHANNEL 6
#define PCLK_CHANNEL 7
#include <common.h>
#include <command.h>
extern osd_area_t osd_area[0];
static void inline i2c_power_on_backlight(void);
static void inline i2c_power_off_backlight(void);
void write_bmp_to_nand(void);
void write_uboot_to_nand(void);
/*
For A3, TL080WX800 8"
*/
#define LCD_WIDTH       1280
#define LCD_HEIGHT      768
#define MAX_WIDTH       1440
#define MAX_HEIGHT      790
#define VIDEO_ON_LINE   17

#define VIDEO_ON_PIXEL	64

#define START_L 15 // Display Start Line
unsigned short  GammaTableR1[256] ;
unsigned short  GammaTableG1[256];
unsigned short  GammaTableB1[256];

lcd_info lcd_plat_info= 
{
	.lcd_width= LCD_WIDTH,
	.lcd_height= LCD_HEIGHT,
	.width_position = 900,
	.hight_position = 600,
};
/*if you want edit below address ,you have to edit it in a3_dpf_jc.h at the same time*/
osd_display_info osd_display_plat_info=
{
	.osd_address=0x83000000,
	.bmp_load_addr_start = 0x84900000,
	.rgb_data_addr_start = 0x84300000,
	.bmp_in_nand_addr_start = 0x4000000,
	.bmp_nand_rw_size = 0x500000,
	.display_logo_num = 0,
};



// Define TCON0~7 Tcon Setting

static mlvds_tcon_config_t mlvds_tconfig[8]=
{
    {OE2_CHANNEL, 0, 1400+5, 1400+5+30, START_L, START_L+768-1, 0, 0, 0, 0},
    {STV_CHANNEL, 1, 1400+10-(1440/2), 1400+10-(1440/2)+1448, START_L, START_L, 1400+10-(1440/2), 1400+10-(1440/2)+1448, START_L, START_L},
    {TP1_CHANNEL, 0, 1400+10, 1400+10+10, START_L-1, START_L+768, 1400+10+1448,1400+10+10+1448, START_L-1, START_L+768+1},
    {CPV_CHANNEL, 0, 700, 1400, START_L-1, START_L+768, 700+1448, 1400+1448, START_L-1, START_L+768},
    {OE1_CHANNEL, 0, 1300, 1440, START_L, START_L+768, 0, 0, 0, 0},
    {EVEN_CHANNEL, 0, 0xffff, 0xffff, 0xffff, 0xffff, 0, 0, 0, 0},
    {POL_CHANNEL, 0, 1400+10-(1440/2), 1400+10-(1440/2)+1440-1448, START_L, START_L+768, 1400+10-(1440/2)+1448, 1400+10-(1440/2)+1440-1448+1448, START_L, START_L+768},
    {PCLK_CHANNEL, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// Define miniLVDS physical PREM SWING VCM REF
static mlvds_phy_control_t tl080wx800_v0_phy = 
{
    .lvds_prem_ctl = 0x1,		//0x8
    .lvds_swing_ctl = 0x1,	//0x8
    .lvds_vcm_ctl = 0x1,
    .lvds_ref_ctl = 0xf, 
};


// Define miniLVDS dual/singal gate, pair num, bit num etc.
static mlvds_config_t tl080wx800_v0_m =
{
    .mlvds_insert_start = 0x45,
    .total_line_clk = 1448,
    .test_dual_gate = 1,
    .test_bit_num = 6,
    .test_pair_num = 6,
    //mLVDS TCON Channel 0-7 Control
    //1 enable this channel   0 disable this channel
    .set_mlvds_pinmux = ( (0 << 0)
                         |(1 << 1)
                         |(1 << 2)
                         |(1 << 3)
                         |(1 << 4)
                         |(1 << 5)
                         |(0 << 6)
                         |(0 << 7) ),
    .mlvds_tcon_config = &mlvds_tconfig[0],
    .mlvds_phy_control = &tl080wx800_v0_phy,
    .scan_function = 0,
    .phase_select = 1,
    .TL080_phase =7,
};

// Define LCD size, gamma control etc.
lcdConfig_t tl080wx800_v0 = {

    .width = LCD_WIDTH,
    .height = LCD_HEIGHT,
    .max_width = MAX_WIDTH,
    .max_height = MAX_HEIGHT,
    .video_on_line = VIDEO_ON_LINE,
    
    .pll_ctrl = 0x666,
    .clk_ctrl = 0x1fc1,
    
    .gamma_cntl_port = (1 << LCD_GAMMA_EN) | (0 << LCD_GAMMA_RVS_OUT) | (1 << LCD_GAMMA_VCOM_POL),
    .gamma_vcom_hswitch_addr = 0,
    
    .rgb_base_addr = 0xf0,
    .rgb_coeff_addr = 0x74a,
    .pol_cntl_addr = (0x1 << LCD_CPH1_POL) |(0x1 << LCD_HS_POL) | (0x1 << LCD_VS_POL),
    .dith_cntl_addr = 0x600,
    
    .inv_cnt_addr = (0 << LCD_INV_EN)|(0 << LCD_INV_CNT),
    .tcon_misc_sel_addr = (0 << LCD_STV1_SEL)|(0 << LCD_STV2_SEL),
    .dual_port_cntl_addr = (0x1 << LCD_TTL_SEL)|(0x1 << LCD_ANALOG_SEL_CPH3)|(0x1 << LCD_ANALOG_3PHI_CLK_SEL),
    .flags = LCD_DIGITAL_mLVDS,  //For miniLVDS LCD Interface     //LCD_DIGITAL,     //For TTL LCD Interface
    .screen_width = 5,
    .screen_height = 3, //43,25       //195.072:113.4
    .mlvds_config = &tl080wx800_v0_m,
};


#ifdef TTL_LCD_CONFIG
static lcdConfig_t lcd_config = {

    .width = LCD_WIDTH,
    .height = LCD_HEIGHT,
    .max_width = MAX_WIDTH,
    .max_height = MAX_HEIGHT,
	.video_on_pixel = VIDEO_ON_PIXEL,
    .video_on_line = VIDEO_ON_LINE,
    
    .pll_ctrl = 0x439,    //dclk=68.4MHz, frame_rate=60Hz
    .clk_ctrl = 0x3141,   //bit[3:0]=xd
    
    .gamma_cntl_port = (1 << LCD_GAMMA_EN) | (0 << LCD_GAMMA_RVS_OUT) | (1 << LCD_GAMMA_VCOM_POL),
    .gamma_vcom_hswitch_addr = 0,
    
    .rgb_base_addr = 0xf0,
    .rgb_coeff_addr = 0x74a,
    .pol_cntl_addr = (0x1 << LCD_CPH1_POL) |(0x1 << LCD_HS_POL) | (0x1 << LCD_VS_POL),
    .dith_cntl_addr = 0x600,
    
	.sth1_hs_addr = 20,
    .sth1_he_addr = 10,
    .sth1_vs_addr = 0,
    .sth1_ve_addr = MAX_HEIGHT - 1,
    .oeh_hs_addr = VIDEO_ON_PIXEL,
    .oeh_he_addr = VIDEO_ON_PIXEL+LCD_WIDTH,
    .oeh_vs_addr = VIDEO_ON_LINE,
    .oeh_ve_addr = VIDEO_ON_LINE+LCD_HEIGHT,
    .vcom_hswitch_addr = 5,
    .vcom_vs_addr = 0,
    .vcom_ve_addr = MAX_HEIGHT-1,
    .cpv1_hs_addr = 0,
	.cpv1_he_addr = 0,
	.cpv1_vs_addr = 0,
	.cpv1_ve_addr = 0,
    .stv1_hs_addr = 0,
    .stv1_he_addr = MAX_WIDTH - 1,
    .stv1_vs_addr = 5,
    .stv1_ve_addr = 2,
    .oev1_hs_addr = 0,
    .oev1_he_addr = 0,
    .oev1_vs_addr = 0,
    .oev1_ve_addr = 0,
	
    .inv_cnt_addr = (0 << LCD_INV_EN)|(0 << LCD_INV_CNT),
    .tcon_misc_sel_addr = (0 << LCD_STV1_SEL)|(0 << LCD_STV2_SEL),
    .dual_port_cntl_addr = (0x1 << LCD_TTL_SEL)|(0x1 << LCD_ANALOG_SEL_CPH3)|(0x1 << LCD_ANALOG_3PHI_CLK_SEL),
    .flags = LCD_DIGITAL_TTL,  
    .screen_width = 5,
    .screen_height = 3,     
};

static void set_lcd_pinmux(void)
{		
	//clear tcon pinmux
	WRITE_MPEG_REG(0x202c, READ_MPEG_REG(0x202c) & ~((0xff<<12)|(0x3ff<<20)));
	WRITE_MPEG_REG(0x202d, READ_MPEG_REG(0x202d) & ~(0xff<<2));
	WRITE_MPEG_REG(0x202e, READ_MPEG_REG(0x202e) & ~(1<<7));
	WRITE_MPEG_REG(0x2031, READ_MPEG_REG(0x2031) & ~(1<<28));
	
	//clear RGB pinmux
	WRITE_MPEG_REG(0x202f, READ_MPEG_REG(0x202f) & ~((0x7ff<<0)|(0x3f<<17)));
	WRITE_MPEG_REG(0x2030, READ_MPEG_REG(0x2030) & ~(0x7ff<<0));
	WRITE_MPEG_REG(0x2033, READ_MPEG_REG(0x2033) & ~(0x3f<<0));
	WRITE_MPEG_REG(0x202f, READ_MPEG_REG(0x202f) & ~(0x3f<<11));
	
	//lcd tcon
	WRITE_MPEG_REG(0x202d, READ_MPEG_REG(0x202d) | ((1<<4)|(0x7<<7)));
	//lcd 6bit
	WRITE_MPEG_REG(0x202f, READ_MPEG_REG(0x202f) | ((1<<11)|(1<<13)|(1<<15)));	
}

#endif
lcdConfig_t *get_lcd_config1(void)
{
#ifndef TTL_LCD_CONFIG		
	return &tl080wx800_v0;
#else
  return &lcd_config;
#endif
}


// Generate the gamma table
static void setup_gama_table1(void)
{
    int i;
    const unsigned short gamma_adjust[256] = 
    {
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
        GammaTableR1[i] = gamma_adjust[i] << 2;
        GammaTableG1[i] = gamma_adjust[i] << 2;
        GammaTableB1[i] = gamma_adjust[i] << 2;
    }
}

void lcd_power_on(void)
{
    WRITE_MPEG_REG(0x200f, READ_MPEG_REG(0x200f)&(~(1<<21))); 
    WRITE_MPEG_REG(0x2010, READ_MPEG_REG(0x2010)|(1<<21));  //set gpioC13, VCC_EN
    
    WRITE_MPEG_REG(0x200f, READ_MPEG_REG(0x200f)&(~(1<<23))); 
    WRITE_MPEG_REG(0x2010, READ_MPEG_REG(0x2010)&(~(1<<23)));  //clear gpioC15, LCD_PWR_EN
#ifdef TTL_LCD_CONFIG	
	    set_lcd_pinmux();
#endif
}

static void lcd_power_off(void)
{
    WRITE_MPEG_REG(0x200f, READ_MPEG_REG(0x200f)&(~(1<<21))); 
    WRITE_MPEG_REG(0x2010, READ_MPEG_REG(0x2010)&(~(1<<21)));  //set gpioC13, VCC_EN
    
    WRITE_MPEG_REG(0x200f, READ_MPEG_REG(0x200f)&(~(1<<23))); 
    WRITE_MPEG_REG(0x2010, READ_MPEG_REG(0x2010)|(1<<23));  //clear gpioC15, LCD_PWR_EN
    
    WRITE_MPEG_REG(0x200c, READ_MPEG_REG(0x200c)&(~(1<<5))); 
    WRITE_MPEG_REG(0x200d, READ_MPEG_REG(0x200d)&(~(1<<5)));  //set gpioA5, BL_PWM
}

void power_off_backlight(void)
{
    WRITE_MPEG_REG(0x200c, READ_MPEG_REG(0x200c)&(~(1<<5))); 
    WRITE_MPEG_REG(0x200d, READ_MPEG_REG(0x200d)&~((1<<5)));  //set gpioA5, BL_PWM
#ifdef CONFIG_A3_I2C
		i2c_power_off_backlight();
#endif
}

void inline power_on_backlight(void)
{
    WRITE_MPEG_REG(0x200c, READ_MPEG_REG(0x200c)&(~(1<<5))); 
    WRITE_MPEG_REG(0x200d, READ_MPEG_REG(0x200d)|(1<<5));  //set gpioA5, BL_PWM
#ifdef CONFIG_A3_I2C
		i2c_power_on_backlight();
#endif
}



#ifdef CONFIG_A3_I2C
/*use AXP192 power manager IC chip*/
static int Power_IICAdress = 0x68;
#ifndef POWER_AXP209 
static unsigned char data0 = 0xc4;
static unsigned char data1 = 0x0f;
static unsigned char data2 = 0x00;
static unsigned char data3 = 0x80;
static unsigned char data4 = 0x47;
static unsigned char data5 = 0x57;
static unsigned char data6 = 0x02;
static unsigned char data7 = 0x0a;
static unsigned char data8 = 0x07;
static void  AXP192_Init(void)
{
		int ret = 0;
		SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, ((1 << 24) | (1 << 25)));//set  hard i2c control io
		hard_i2c_para.device_id = Power_IICAdress;
		Init_hard_I2c();	
		ret = hard_i2c_write_0(&data0,1,0x33);	
		if(ret)
		{
				printf("i2c not write success!\n");	
		}		
		ret =hard_i2c_write_0(&data1,1,0x12);
		udelay(50);		
		ret =hard_i2c_write_0(&data2,1,0x93);	
		udelay(50);			
		ret =hard_i2c_write_0(&data3,1,0x9e);
		udelay(50);		      		
		ret =hard_i2c_write_0(&data4,1,0x32);
		udelay(50);	        	
		ret =hard_i2c_write_0(&data5,1,0x36);
		udelay(100000);	//fix white flash problem when backlight power on
		ret =hard_i2c_write_0(&data6,1,0x92);
		udelay(50);	
		ret =hard_i2c_write_0(&data7,1,0x99);
		if(ret)
		{
				printf("i2c not write success!\n");	
		}
}


#else
static unsigned char data0 = 0x4d;
static unsigned char data1 = 0x19;
static unsigned char data2 = 0x5f;
static unsigned char data3 = 0x00;
static unsigned char data4 = 0x30;
static unsigned char data5 = 0x01;
static unsigned char data6 = 0x63;
#define     LED_CTL_REG0   *(volatile unsigned *)0xc1108768
void led_analog_dimmer(unsigned new_dimmer)
{
	unsigned cur_dimmer = 0;
	
	if ((new_dimmer > 15) ||(new_dimmer <1))
		return;	
	cur_dimmer = 15 - new_dimmer;

	(LED_CTL_REG0) &=~(15<<0);
	(LED_CTL_REG0) += cur_dimmer;
}

static void  AXP209_Init(void)
{
		int ret = 0;
		SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, ((1 << 24) | (1 << 25)));//set  hard i2c control io
		hard_i2c_para.device_id = Power_IICAdress;
		Init_hard_I2c();	
		ret = hard_i2c_write_0(&data0,1,0x27);	
		if(ret)
		{
				printf("i2c not write success!\n");	
		}
		hard_i2c_para.device_id = Power_IICAdress;
		ret =hard_i2c_write_0(&data1,1,0x23);	
		udelay(100);
		ret =hard_i2c_write_0(&data6,1,0x30);
		udelay(100);
		ret =hard_i2c_write_0(&data2,1,0x12);	
		udelay(100);			
		ret =hard_i2c_write_0(&data3,1,0x93);
		udelay(100);
		ret =hard_i2c_write_0(&data4,1,0x29);
		udelay(100000);
		led_analog_dimmer(8);
		ret =hard_i2c_write_0(&data5,1,0x92);
		if(ret)
		{
			printf("i2c not write success!\n");	
		}
		
}
#endif



static void inline i2c_power_on_backlight(void)
{
#ifndef POWER_AXP209 
	 AXP192_Init();
#else
   AXP209_Init();
#endif
}
static void inline i2c_power_off_backlight(void)
{
#ifndef POWER_AXP209    
    hard_i2c_write_0(&data8,1,0x92);
#else
	  hard_i2c_write_0(&data3,1,0x92);
#endif  
}

void disable_i2c_pinmux(void)
{
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, ((1 << 24) | (1 << 25)));
}

#endif




static void power_on_lcd(void)
{ 
   lcd_power_on();
}

static void power_off_lcd(void)
{
	
}


void generate_gamma_table_dlCD(void)
{
	setup_gama_table1();
}
#ifndef TTL_LCD_CONFIG
LCDDesc lcd_platform_config={&tl080wx800_v0, power_on_lcd, power_off_lcd, power_on_backlight, power_off_backlight};
#else
LCDDesc lcd_platform_config={&lcd_config, power_on_lcd, power_off_lcd, power_on_backlight, power_off_backlight};
#endif
