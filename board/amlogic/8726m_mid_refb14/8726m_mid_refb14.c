#include <common.h>
#include <asm/mach-types.h>
#include <asm/arch/memory.h>
#include <asm/arch/am_reg_addr.h>
#include <asm/arch/aml_card_sd.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/am_eth_reg.h>
#include <share_kernel.h>
#include <asm/arch/gpio.h>
#include <asm/arch/nand.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/mtd.h>
#include <asm/arch/usb.h>
#include <asm/arch/osd.h>

#include <font/ISO_88591_18.h>
#include <font/ISO_88591_20.h>
#include <font/ISO_88591_24.h>
#include <font/ISO_88591_32.h>
#include <font/ISO_88591_40.h>

#include <asm/arch/i2c.h>
#include <aml_i2c.h>
#include <sn7325.h>

/**
 * Routine: board_init
 * Description: Early hardware init.
 */
 

static struct aml_nand_platform aml_nand_mid_platform[] = {
};

struct aml_nand_device aml_nand_mid_device = {
	.aml_nand_platform = aml_nand_mid_platform,
	.dev_num = 0,
};

#ifdef CONFIG_USB_DWC_OTG_HCD
static void set_usb_a_vbus_power(char is_power_on)
{
#define USB_A_POW_GPIO	PREG_EGPIO
#define USB_A_POW_GPIO_BIT	3
#define USB_A_POW_GPIO_BIT_ON	1
#define USB_A_POW_GPIO_BIT_OFF	0
	if(is_power_on){
		printk(KERN_INFO "set usb port power on (board gpio %d)!\n",USB_A_POW_GPIO_BIT);
		set_gpio_mode(USB_A_POW_GPIO,USB_A_POW_GPIO_BIT,GPIO_OUTPUT_MODE);
		set_gpio_val(USB_A_POW_GPIO,USB_A_POW_GPIO_BIT,USB_A_POW_GPIO_BIT_ON);
	}
	else	{
		printk(KERN_INFO "set usb port power off (board gpio %d)!\n",USB_A_POW_GPIO_BIT);		
		set_gpio_mode(USB_A_POW_GPIO,USB_A_POW_GPIO_BIT,GPIO_OUTPUT_MODE);
		set_gpio_val(USB_A_POW_GPIO,USB_A_POW_GPIO_BIT,USB_A_POW_GPIO_BIT_OFF);		
	}
}

struct amlogic_usb_config usb_config={
	USB_PHY_CLOCK_SEL_XTAL_DIV2,
	0, // no use if clock sel == xtal or xtal/2
	CONFIG_M1_USBPORT_BASE,
	0,
	set_usb_a_vbus_power, //set_vbus_power
};
#endif


int board_init(void)
{
    /*
    @todo implement this function
    */
	DECLARE_GLOBAL_DATA_PTR;
	gd->bd->bi_arch_number=MACH_TYPE_MESON_8626M;
	gd->bd->bi_boot_params=BOOT_PARAMS_OFFSET;

#ifdef CONFIG_CMD_KGDB
	kgdb_init();
#endif
#ifdef CONFIG_USB_DWC_OTG_HCD
    board_usb_init(&usb_config);
#endif
	init_osd_param();
	return 0;
}

/*
 * Routine: misc_init_r
 * Description: Init ethernet (done here so udelay works)
 */
int misc_init_r(void)
{
/*
    @todo implement this function
    */

	return 0;
}

/*
 * Routine: set_muxconf_regs
 * Description: Setting up the configuration Mux registers specific to the
 *		hardware. Many pins need to be moved from protect to primary
 *		mode.
 */
void set_muxconf_regs(void)
{
    /*
    @todo implement this function
    */
}

u32 get_board_rev(void)
{
    /*
    @todo implement this function
    */
	return 0x20;
}
static int  sdio_init(unsigned port)
{
    if(port == SDIO_PORT_B1){
        printf("iNand Reset (GPIOE_17)\n");
        CLEAR_CBUS_REG_MASK(PREG_HGPIO_O,1<<17);
        CLEAR_CBUS_REG_MASK(PREG_HGPIO_EN_N,1<<17);
        mdelay(150);
        SET_CBUS_REG_MASK(PREG_HGPIO_O,1<<17);
        mdelay(50);
        }
    return cpu_sdio_init(port);
}
static int  sdio_detect(unsigned port)
{
    if(port == SDIO_PORT_A)
        return (readl(P_PREG_CGPIO_I)&(1<<0))?1:0;
    else
        return 0;
}
static void sdio_pwr_prepare(unsigned port)
{
}
static void sdio_pwr_on(unsigned port)
{
    if(port == SDIO_PORT_A){
        clrbits_le32(P_PREG_CGPIO_O,(1<<0));
        clrbits_le32(P_PREG_CGPIO_EN_N,(1<<0));
    	}
    /// @todo NOT FINISH
}
static void sdio_pwr_off(unsigned port)
{
    if(port == SDIO_PORT_A){
        setbits_le32(P_PREG_CGPIO_O,(1<<0));
        clrbits_le32(P_PREG_CGPIO_EN_N,(1<<0));
        }
	/// @todo NOT FINISH
}
static void board_mmc_register(unsigned port)
{
    AML_CARD_SD_INFO_t *aml_priv=cpu_sdio_get(port);
    if(aml_priv==NULL)
        return;
    struct mmc *mmc = (struct mmc *)malloc(sizeof(struct mmc));
    strncpy(mmc->name,aml_priv->name,31);
    mmc->priv = aml_priv;
	aml_priv->removed_flag = 1;
	aml_priv->inited_flag = 0;
	aml_priv->sdio_init=sdio_init;
	aml_priv->sdio_detect=sdio_detect;
	aml_priv->sdio_pwr_off=sdio_pwr_off;
	aml_priv->sdio_pwr_on=sdio_pwr_on;
	aml_priv->sdio_pwr_prepare=sdio_pwr_prepare;
	mmc->send_cmd = aml_sd_send_cmd;
	mmc->set_ios = aml_sd_cfg_swth;
	mmc->init = aml_sd_init;
	mmc->rca = 1;
	mmc->voltages = MMC_VDD_33_34;
	mmc->host_caps = MMC_MODE_4BIT | MMC_MODE_HS;
	//mmc->host_caps = MMC_MODE_4BIT;
	mmc->bus_width = 1;
	mmc->clock = 300000;
	mmc->f_min = 200000;
	mmc->f_max = 50000000;
	mmc_register(mmc);
}
int board_mmc_init(bd_t	*bis)
{
	board_mmc_register(SDIO_PORT_A);
//	board_mmc_register(SDIO_PORT_B);
//	board_mmc_register(SDIO_PORT_C);
	board_mmc_register(SDIO_PORT_B1);
	return 0;
}

#ifdef CONFIG_SWITCH_BOOT_MODE
struct aml_i2c_platform aml_i2c_plat = {
    .wait_count     = 1000000,
    .wait_ack_interval  = 5,
    .wait_read_interval = 5,
    .wait_xfer_interval = 5,
    .master_no      = AML_I2C_MASTER_B,
    .use_pio            = 0,
    .master_i2c_speed   = AML_I2C_SPPED_400K,

    .master_b_pinmux = {
        .scl_reg    = MESON_I2C_MASTER_B_GPIOB_0_REG,
        .scl_bit    = MESON_I2C_MASTER_B_GPIOB_0_BIT,
        .sda_reg    = MESON_I2C_MASTER_B_GPIOB_1_REG,
        .sda_bit    = MESON_I2C_MASTER_B_GPIOB_1_BIT,
    }
};

#ifdef CONFIG_EXIO_SN7325
void sn7325_pwr_rst(void)
{
    //reset GPIOD_20(AA5)
    set_gpio_val(GPIOD_bank_bit2_24(20), GPIOD_bit_bit2_24(20), 0); //low
    set_gpio_mode(GPIOD_bank_bit2_24(20), GPIOD_bit_bit2_24(20), GPIO_OUTPUT_MODE);

    udelay(2); //delay 2us

    set_gpio_val(GPIOD_bank_bit2_24(20), GPIOD_bit_bit2_24(20), 1); //high
    set_gpio_mode(GPIOD_bank_bit2_24(20), GPIOD_bit_bit2_24(20), GPIO_OUTPUT_MODE);
    //end
}
#endif

inline int get_key()
{
    int adc_val = get_adc_sample(4);
    printf("get_adc_sample(4): 0x%x\n", adc_val);
    return(((adc_val >= 0x240) && (adc_val < 0x320)) ? 1 : 0);
}

inline int get_key_value(void)
{
	return get_adc_sample(4);
}

#define is_ac_connected ((READ_CBUS_REG(ASSIST_HW_REV)&(1<<9))? 1:0)

int logo_display(void)
{
    int ret = 0;
    run_command ("nand read ${loadaddr} ${aml_logo_start} ${aml_logo_size}", 0);
    ret = run_command ("bmp display ${loadaddr} 0 0", 0);
    run_command ("lcd bl on", 0);
    return ret;

}

inline void display_messge(char *msg)
{
#ifdef ENABLE_FONT_RESOURCE    
    run_command ("lcd clear", 0);
    //AsciiPrintf(msg, 250, 200, 0x80ff80);
    AsciiPrintf(msg, 0, 0, 0x00ff00);
    run_command ("lcd bl on", 0);
#else
		printf("%s\n",msg);
#endif    
}

extern int aml_autoscript(void);

int switch_boot_mode(void)
{
    unsigned long hold_time = 50000, polling_time = 10000, tmp;
    unsigned long upgrade_step;

		powerkey_hold(0);
    upgrade_step = simple_strtoul (getenv ("upgrade_step"), NULL, 16);
    printf("upgrade_step = %d    !!!!!!!!!!!!!!!!!!\n", upgrade_step);

    saradc_enable();
		
		
#ifdef ENABLE_FONT_RESOURCE
		RegisterFont(DEFAULT_FONT);
#endif
		aml_autoscript();
		powerkey_hold(0);
    if(upgrade_step == 3)
    {
	switch(reboot_mode)
        {
            case AMLOGIC_NORMAL_BOOT:
            {
                printf("AMLOGIC_NORMAL_BOOT...\n");
                power_hold();
                logo_display();
                return  1;
            }
            case AMLOGIC_FACTORY_RESET_REBOOT:
            {
                printf("AMLOGIC_FACTORY_RESET_REBOOT...\n");
                power_hold();
                logo_display();
                run_command ("nand read ${recovery_name} ${loadaddr} 0 ${recovery_size}", 0);
				run_command ("bootm", 0);
                break;
            }
            case AMLOGIC_UPDATE_REBOOT:
            {
                printf("AMLOGIC_UPDATE_REBOOT...\n");
                power_hold();
                logo_display();
                run_command ("set upgrade_step 0", 0);
                run_command ("save", 0);
                upgrade_step = 0;
                break;
            }
            default:
            {
                printf("AMLOGIC_CHARGING_REBOOT...\n");
                if(is_ac_connected)
                {
                    power_hold();
#ifdef CONFIG_BATTERY_CHARGING
					battery_charging();
#endif
					logo_display();
                }
                else
                {
                    powerkey_hold(0);
#ifdef CONFIG_BATTERY_CHARGING
                    if(get_powerkey_hold_count())
                    {
                    	  logo_display();
                        if(get_battery_percentage() < 10)
                        {
                            power_low_display();
                            sdelay(2);
                            power_unhold();
                            printf("Low Power!!!\nPower Down!\n");
                            hang();
                        }
#else
					if(powerkey_hold(1000))
                    {
#endif
                        logo_display();
                        power_hold();
                        printf("Power Up!\n");
                    }
                    else
                    {
                        power_unhold();
                        printf("Power Down!\n");
                        hang();
                    }
                }
                break;
            }
        }
    }
    else
    {
        power_hold();
        printf("Upgrade step %d...\n", upgrade_step);
    }

    if(upgrade_step == 0)
    {
        display_messge("upgrade step 1! Don't Power Off!");
	if(upgrade_bootloader())
        {
            run_command ("set upgrade_step 1", 0);
            run_command ("save", 0);
            run_command ("reset", 0);
            hang();
        }
        else
        {
            printf("### ERROR:  u-boot write failed!!!\n");
            return  -1;
        }
    }
    else if((upgrade_step >0) && (upgrade_step < 3))
    {
        if(upgrade_step == 1)
        {
            display_messge("upgrade step 2! Don't Power Off!");
            if(upgrade_env())
            {
                run_command ("set upgrade_step 2", 0);
                run_command ("save", 0);
                run_command ("reset", 0);
                hang();
            }
            else
            {
                run_command ("defenv", 0);
                run_command ("save", 0);
            }
        }
	display_messge("upgrade step 3! Don't Power Off!");
        run_command ("set upgrade_step 3", 0);
        run_command ("save", 0);
        //upgrade_partition();
        into_recovery();
    }

    //added by Elvis for added fool idle
    get_key();
    get_key();
    
    while(hold_time > 0)
    {
        udelay(polling_time);
        tmp = get_key();
        printf("get_key(): %d\n", tmp);
        if(!tmp)  break;
        hold_time -= polling_time;
    }

    if(hold_time > 0)
    {
        printf("Normal Start...\n");
        return  1;
    }
    else
    {
	display_messge("upgrading... please wait");
	if(upgrade_bootloader())
        {
            run_command ("set upgrade_step 1", 0);
            run_command ("save", 0);
            run_command ("reset", 0);
            hang();
        }
        if(upgrade_env())
        {
            run_command ("set upgrade_step 2", 0);
            run_command ("save", 0);
            run_command ("reset", 0);
            hang();
        }
        run_command ("set upgrade_step 3", 0);
        run_command ("save", 0);
#ifndef	CONFIG_UBI_SUPPORT
        //upgrade_partition();
#endif
        into_recovery();
    }
}
#endif

#ifdef CONFIG_HAVE_SYS_TEST
#if defined(CONFIG_TOUCHSCREEN_ADS7846)
#include <ads7846.h>
#include <spi.h>
#include <soft_spi.h>

#define SPI_0		0
#define SPI_1		1
#define SPI_2		2

// GPIOC_8(G20, XPT_CLK)
#define GPIO_SPI_SCK		((GPIOC_bank_bit0_26(8)<<16) |GPIOC_bit_bit0_26(8)) 
// GPIOC_7(G21, XPT_IN)
#define GPIO_SPI_MOSI		((GPIOC_bank_bit0_26(7)<<16) |GPIOC_bit_bit0_26(7)) 
// GPIOC_6(G22, XPT_OUT)
#define GPIO_SPI_MISO		((GPIOC_bank_bit0_26(6)<<16) |GPIOC_bit_bit0_26(6))
// GPIOC_0(J20, XPT_NCS)
#define GPIO_TSC2046_CS	((GPIOC_bank_bit0_26(0)<<16) |GPIOC_bit_bit0_26(0)) 
// GPIOC_4(H20, NPEN_IRQ)
#define GPIO_TSC2046_PENDOWN	((GPIOC_bank_bit0_26(4)<<16) |GPIOC_bit_bit0_26(4)) 

const struct soft_spi_gpio_data aml_spi_gpio_pdata = {
	.sck = GPIO_SPI_SCK,
	.mosi = GPIO_SPI_MOSI,
	.miso = GPIO_SPI_MISO,
};

#define XPOL 0
#define YPOL 1
#define XMIN 0
#define XMAX 0xfff
#define YMIN 250
#define YMAX 3800

int ads7846_convert(int x, int y)
{
    if (x < XMIN) x = XMIN;
    if (x > XMAX) x = XMAX;
    if (y < YMIN) y = YMIN;
    if (y > YMAX) y = YMAX;
#if (XPOL == 1)
    x = XMAX + XMIN - x;
#endif
#if (YPOL == 1)
    y = YMAX + YMIN - y;
#endif
    return (x << 16) | y;
}
static int ads7846_init_gpio(void)
{
	/* set input mode */
	gpio_direction_input(GPIO_TSC2046_PENDOWN);
	return 0;
}

static int ads7846_get_pendown_state(void)
{
	return !gpio_get_value(GPIO_TSC2046_PENDOWN);
}

static struct ads7846 ads7846_data = {
		.cs = GPIO_TSC2046_CS,
		.max_speed_hz = 500000,
		.bus_num = SPI_2,
		.mode = SPI_MODE_0,
		.get_pendown_state = ads7846_get_pendown_state,
		.convert = ads7846_convert,
		.init = ads7846_init_gpio,
};

#endif

int touch_init(void)
{
#ifdef CONFIG_TOUCHSCREEN_ADS7846
	return ads7846_init(&ads7846_data);
#else
	return 0;
#endif	
}

int touch_work(struct ts_event *tc) 
{
#ifdef CONFIG_TOUCHSCREEN_ADS7846
	return ads7846_work(&ads7846_data, tc);
#else
	return 0;
#endif	
}


#ifdef CONFIG_CAMERA_GC0308
static void gc0308_init(void)
{
	//no camera
}
#endif

#ifdef CONFIG_CAMERA_GT2005
static void gt2005_init(void)
{	
	//no camera
}
#endif

#ifdef CONFIG_AML_I2C
#include <aml_i2c.h>
extern void sys_test_i2c_init(struct i2c_board_info *i2c_info, unsigned int len);
static struct i2c_board_info aml_i2c_info[] = {
#ifdef CONFIG_SENSORS_MXC622X
    {
        I2C_BOARD_INFO("gsensors(mxc622x)", 0x15),
		.device_init = NULL,
    },
#endif

#ifdef CONFIG_CAMERA_GT2005
    {
    	I2C_BOARD_INFO("camera(gt2005)", 0x78 >> 1),
    	.device_init = gt2005_init,
    },
#endif

#ifdef CONFIG_CAMERA_GC0308
    {
        I2C_BOARD_INFO("camera(gc0308)", 0x42 >> 1),
		.device_init = gc0308_init,
    },
#endif

#ifdef CONFIG_SND_WM8900
    {
        I2C_BOARD_INFO("audiodec(wm8900)", 0x1A),
        .device_init = NULL,
    },
#endif

#ifdef CONFIG_EXIO_SN7325
    {
        I2C_BOARD_INFO("externio(sn7325)", SN7325_ADDR),
        .device_init = NULL,
    },
#endif

#ifdef CONFIG_TOUCHSCREEN_TSC2007
    {
        I2C_BOARD_INFO("touch(tsc2007)", 0x48),
		.device_init = NULL,
    },
#endif
};

struct aml_i2c_platform aml_i2c_plat = {
    .wait_count     = 1000000,
    .wait_ack_interval  = 5,
    .wait_read_interval = 5,
    .wait_xfer_interval = 5,
    .master_no      = AML_I2C_MASTER_B,
    .use_pio            = 0,
    .master_i2c_speed   = AML_I2C_SPPED_400K,

    .master_b_pinmux = {
        .scl_reg    = MESON_I2C_MASTER_B_GPIOB_0_REG,
        .scl_bit    = MESON_I2C_MASTER_B_GPIOB_0_BIT,
        .sda_reg    = MESON_I2C_MASTER_B_GPIOB_1_REG,
        .sda_bit    = MESON_I2C_MASTER_B_GPIOB_1_BIT,
    }
};

#endif

void m1_init_machine(void)
{
  #ifdef ENABLE_FONT_RESOURCE
	RegisterFont(&ISO_88591_24Font);
	#endif
    #ifdef CONFIG_AML_I2C
	sys_test_i2c_init(aml_i2c_info, ARRAY_SIZE(aml_i2c_info));
	#endif
	run_command("lcd clear",0);
	power_on_backlight();
}

void usb_power_on(void)
{
    set_gpio_val(GPIOA_bank_bit(26), GPIOA_bit_bit23_26(26), 1); //high
    set_gpio_mode(GPIOA_bank_bit(26), GPIOA_bit_bit23_26(26), GPIO_OUTPUT_MODE);
}

#endif
