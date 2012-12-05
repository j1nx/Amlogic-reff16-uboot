
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
#include <asm/arch/osd.h>

#include <asm/arch/i2c.h>
#include <aml_i2c.h>
#include <sn7325.h>
#include "saradc_reg.h"

#define msleep(a) udelay(a * 1000)

/**
 * Routine: board_init
 * Description: Early hardware init.
 */

static struct aml_nand_platform aml_nand_mid_platform[] = {
	{
		.name = NAND_BOOT_NAME,
		.chip_enable_pad = AML_NAND_CE0,
		.ready_busy_pad = AML_NAND_CE0,
		.platform_nand_data = {
			.chip =  {
				.nr_chips = 1,
				.options = (NAND_TIMING_MODE5 | NAND_ECC_BCH16_MODE),
			},
    	},
		.T_REA = 20,
		.T_RHOH = 15,
	},
	{
		.name = NAND_NORMAL_NAME,
		.chip_enable_pad = (AML_NAND_CE0 | (AML_NAND_CE1 << 4) | (AML_NAND_CE2 << 8) | (AML_NAND_CE3 << 12)),
		.ready_busy_pad = (AML_NAND_CE0 | (AML_NAND_CE0 << 4) | (AML_NAND_CE1 << 8) | (AML_NAND_CE1 << 12)),
		.platform_nand_data = {
			.chip =  {
				.nr_chips = 4,
				.options = (NAND_TIMING_MODE5 | NAND_ECC_BCH16_MODE | NAND_TWO_PLANE_MODE),
			},
    	},
		.T_REA = 20,
		.T_RHOH = 15,
	}
};

struct aml_nand_device aml_nand_mid_device = {
	.aml_nand_platform = aml_nand_mid_platform,
	.dev_num = ARRAY_SIZE(aml_nand_mid_platform),
};

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

#if defined(CONFIG_CMD_NET)
/*
 * Routine: setup_net_chip
 * Description: Setting up the configuration GPMC registers specific to the
 *		Ethernet hardware.
 */
static void setup_net_chip(void)
{
    /*
    @todo implement this function
    */
	
}

extern int aml_eth_init(bd_t *bis);
int board_eth_init(bd_t *bis)
{
    /*
    @todo implement this function
    */
	///GPIOD15-24 for 8626M;
	///GPIOD12	nRst;
	///GPIOD13    n_int;
	printf("Set 8626m \n");
	//eth_clk_set(ETH_CLKSRC_SYS_D3,900*CLK_1M/3,50*CLK_1M);
	//eth_clk_set(ETH_CLKSRC_SYS_D3,get_cpu_clk()*2/3,50*CLK_1M);	
	eth_clk_set(ETH_CLKSRC_APLL_CLK,400*CLK_1M,50*CLK_1M);
	///GPIOD15-24 for 8626M;
	///GPIOD12	nRst;
	///GPIOD13    n_int;
	eth_set_pinmux(ETH_BANK2_GPIOD15_D23,ETH_CLK_OUT_GPIOD7_REG4_20,0);
	
	//power hold
	setbits_le32(P_PREG_AGPIO_O,(1<<8));
	clrbits_le32(P_PREG_AGPIO_EN_N,(1<<8));
	udelay(10);	//waiting reset end;
        aml_eth_init(bis);
        return 0;
}
#endif /* (CONFIG_CMD_NET) */

u32 get_board_rev(void)
{
    /*
    @todo implement this function
    */
	return 0x20;
}
static int  sdio_init(unsigned port)
{
	setbits_le32(P_PREG_CGPIO_EN_N,1<<5);

    return cpu_sdio_init(port);
}
static int  sdio_detect(unsigned port)
{
    printf("\n\n***detetc sd io ***\n\n");
	return 0;//(readl(P_PREG_CGPIO_I)&(1<<5))?1:0;
}
static void sdio_pwr_prepare(unsigned port)
{
    /// @todo NOT FINISH
	///do nothing here
}
static void sdio_pwr_on(unsigned port)
{
	clrbits_le32(P_PREG_CGPIO_O,(1<<5));
	clrbits_le32(P_PREG_CGPIO_EN_N,(1<<5));//test_n
    /// @todo NOT FINISH
}
static void sdio_pwr_off(unsigned port)
{
	setbits_le32(P_PREG_CGPIO_O,(1<<5));
	clrbits_le32(P_PREG_CGPIO_EN_N,(1<<5));//test_n

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
//	board_mmc_register(SDIO_PORT_B1);
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
    //reset
    printk( "amlogic sn7325 driver: init.\n"); 
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
    //printf("get_adc_sample(4): 0x%x\n", adc_val);
    return(((adc_val >= 0) && (adc_val < 900)) ? 1 : 0);
}


inline int get_key_value(void)
{
	return get_adc_sample(4);
}

#define is_ac_connected ((READ_CBUS_REG(ASSIST_HW_REV)&(1<<9))? 1:0)

int logo_display(void)
{
    int ret = 0;
    run_command ("nand read ${aml_logo_name} ${loadaddr} 0 ${aml_logo_size}", 0);
    //ret = run_command ("bmp display ${loadaddr} 0 0", 0);
    ret = run_command ("bmp display ${loadaddr} 0 0", 0);
    run_command ("lcd bl on", 0);

}

static void powerkey_led_on(void)
{
	//red light off
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_8, (1<<5));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_12, (1<<11));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_0, (1<<13));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, (1<<2));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_9, (1<<18));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_11, (1<<4));
	set_gpio_val(GPIOA_bank_bit(3), GPIOA_bit_bit0_14(3), 1);
	set_gpio_mode(GPIOA_bank_bit(3), GPIOA_bit_bit0_14(3), GPIO_OUTPUT_MODE);

	//green light on
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_5, ((1<<29)|(1<<30)));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_11, (1<<22));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, (1<<10));
	set_gpio_val(GPIOE_bank_bit16_21(20), GPIOE_bit_bit16_21(20), 0);
	set_gpio_mode(GPIOE_bank_bit16_21(20), GPIOE_bit_bit16_21(20), GPIO_OUTPUT_MODE);

	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_12, (1<<29));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_5, ((1<<14)|(1<<20)));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7, ((1<<12)|(1<<13)));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_8, (1<<21));
	set_gpio_val(GPIOD_bank_bit2_24(23), GPIOD_bit_bit2_24(23), 1);
	set_gpio_mode(GPIOD_bank_bit2_24(23), GPIOD_bit_bit2_24(23), GPIO_OUTPUT_MODE);
}

static void powerkey_led_off(void)
{
	//green light off
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_5, ((1<<29)|(1<<30)));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_11, (1<<22));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, (1<<10));
	set_gpio_val(GPIOE_bank_bit16_21(20), GPIOE_bit_bit16_21(20), 1);
	set_gpio_mode(GPIOE_bank_bit16_21(20), GPIOE_bit_bit16_21(20), GPIO_OUTPUT_MODE);

	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_12, (1<<29));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_5, ((1<<14)|(1<<20)));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7, ((1<<12)|(1<<13)));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_8, (1<<21));
	set_gpio_val(GPIOD_bank_bit2_24(23), GPIOD_bit_bit2_24(23), 0);
	set_gpio_mode(GPIOD_bank_bit2_24(23), GPIOD_bit_bit2_24(23), GPIO_OUTPUT_MODE);
}


extern  int aml_autoscript(void);

int switch_boot_mode(void)
{
    unsigned long hold_time = 50000, polling_time = 10000, tmp;
    unsigned long upgrade_step;

    upgrade_step = simple_strtoul (getenv ("upgrade_step"), NULL, 16);
    printf("upgrade_step = %d    !!!!!!!!!!!!!!!!!!\n", upgrade_step);

    saradc_enable();

#ifdef	CONFIG_UBI_SUPPORT
    powerkey_hold(0);
    run_command ("ubi part system", 0);
    powerkey_hold(0);
    run_command ("ubifsmount system", 0);
    //run_command ("ubifsls", 0);
#endif
	powerkey_led_off();

    if(upgrade_step == 3)
    {
        switch(reboot_mode)
        {
            case AMLOGIC_NORMAL_BOOT:
            {
                printf("AMLOGIC_NORMAL_BOOT...\n");
                power_hold();
		//green led on red led off
		powerkey_led_on();
		SET_MPEG_REG_MASK(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);
                logo_display();
                return  1;
            }
            case AMLOGIC_FACTORY_RESET_REBOOT:
            {
                printf("AMLOGIC_FACTORY_RESET_REBOOT...\n");
                power_hold();
		//green led on red led off
		powerkey_led_on();
		SET_MPEG_REG_MASK(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);
                logo_display();
                run_command ("nand read ${recovery_name} ${loadaddr} 0 ${recovery_size}", 0);
				run_command ("bootm", 0);
                break;
            }
            case AMLOGIC_UPDATE_REBOOT:
            {
                printf("AMLOGIC_UPDATE_REBOOT...\n");
                power_hold();
		//green led on red led off
		powerkey_led_on();
		SET_MPEG_REG_MASK(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);
                logo_display();
                run_command ("set upgrade_step 0", 0);
                run_command ("save", 0);
                upgrade_step = 0;
                break;
            }
	    case AMLOGIC_CHARGING_REBOOT:
	    {
                printf("AMLOGIC_POWERDOWN... \n");
                if(is_ac_connected)
                { 
                    power_unhold();
		    hang();
                }
	    	}
            default:
            {
                printf("Default.... \n");
                if(is_ac_connected)
                { 
                    power_hold();
#ifdef CONFIG_BATTERY_CHARGING
			battery_charging();
#endif
                }
                else
                {
                    powerkey_hold(0);
                    if(get_powerkey_hold_count())
                    {
#ifdef CONFIG_BATTERY_CHARGING
                        if(get_battery_percentage() < 10)
                        {
                            power_low_display();
                            sdelay(2);
                            power_unhold();
                            printf("Low Power!!!\nPower Down!\n");
                            hang();
                        }
#endif
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
		//green led on red led off
		powerkey_led_on();
		SET_MPEG_REG_MASK(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);

                logo_display();
                break;
            }
        }
    }
    else
    {
        power_hold();
	powerkey_led_on();
	SET_MPEG_REG_MASK(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);

        logo_display();

        printf("Upgrade step %d...\n", upgrade_step);
    }

    if(upgrade_step == 0)
    {
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
        aml_autoscript();
        printf("Normal Start...\n");
        return  1;
    }
    else
    {
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
#include <adc_test.h>
#ifdef CONFIG_TOUCHSCREEN_TSC2007
#include <tsc2007.h>

//GPIOD_24
#define GPIO_TSC2007_PENIRQ ((GPIOD_bank_bit2_24(24)<<16) |GPIOD_bit_bit2_24(24)) 

static int tsc2007_get_pendown_state(void)
{
    return !gpio_get_value(GPIO_TSC2007_PENIRQ);
}

#define XLCD    800
#define YLCD    600
#define SWAP_XY 0
#define XPOL    0
#define YPOL    1
#define XMIN 130
#define XMAX 3970
#define YMIN 250
#define YMAX 4000

static int tsc2007_convert(int x, int y)
{
#if (SWAP_XY == 1)
    swap(x, y);
#endif
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
    x = (x- XMIN) * XLCD / (XMAX - XMIN);
    y = (y- YMIN) * YLCD / (YMAX - YMIN);
    y = y - 32 * YLCD / (y / 2 + YLCD);
    
    return (x << 16) | y;
}

struct tsc2007 tsc2007_pdata = {
	.addr = 0x48,
	.get_pendown_state = tsc2007_get_pendown_state,
	.convert = tsc2007_convert,
};
#endif

#ifdef CONFIG_TOUCHSCREEN_UOR6X5X
#include <uor6x5x.h>

//GPIOD_24
#define GPIO_UOR6X5X_PENIRQ ((GPIOD_bank_bit2_24(24)<<16) |GPIOD_bit_bit2_24(24)) 

static int uor6x5x_get_pendown_state(void)
{
    return gpio_get_value(GPIO_UOR6X5X_PENIRQ);
}

#define UOR6X5X_XLCD    800
#define UOR6X5X_YLCD    600
#define UOR6X5X_SWAP_XY 0
#define UOR6X5X_XPOL    0
#define UOR6X5X_YPOL    1
#define UOR6X5X_XMIN 200
#define UOR6X5X_XMAX 3900
#define UOR6X5X_YMIN 300
#define UOR6X5X_YMAX 3700

int uor6x5x_convert(int x, int y)
{
#if (UOR6X5X_SWAP_XY == 1)
    swap(x, y);
#endif
    if (x < UOR6X5X_XMIN) x = UOR6X5X_XMIN;
    if (x > UOR6X5X_XMAX) x = UOR6X5X_XMAX;
    if (y < UOR6X5X_YMIN) y = UOR6X5X_YMIN;
    if (y > UOR6X5X_YMAX) y = UOR6X5X_YMAX;
#if (UOR6X5X_XPOL == 1)
    x = UOR6X5X_XMAX + UOR6X5X_XMIN - x;
#endif
#if (UOR6X5X_YPOL == 1)
    y = UOR6X5X_YMAX + UOR6X5X_YMIN - y;
#endif
    x = (x- UOR6X5X_XMIN) * UOR6X5X_XLCD / (UOR6X5X_XMAX - UOR6X5X_XMIN);
    y = (y- UOR6X5X_YMIN) * UOR6X5X_YLCD / (UOR6X5X_YMAX - UOR6X5X_YMIN);
    //y = y - 32 * UOR6X5X_YLCD / (y / 2 + UOR6X5X_YLCD);
    
    return (x << 16) | y;
}

struct uor6x5x uor6x5x_pdata = {
	.addr = 0x48,
	.get_pendown_state = uor6x5x_get_pendown_state,
	.convert = uor6x5x_convert,
};
#endif

int touch_init(void)
{
#if defined(CONFIG_TOUCHSCREEN_TSC2007)
	return tsc2007_init(&tsc2007_pdata);
#elif defined(CONFIG_TOUCHSCREEN_UOR6X5X)
    return uor6x5x_init(&uor6x5x_pdata);
#else
	return 0;
#endif	
}

int touch_work(struct ts_event *tc) 
{
#if defined(CONFIG_TOUCHSCREEN_TSC2007)
	return tsc2007_work(&tsc2007_pdata, tc);
#elif defined(CONFIG_TOUCHSCREEN_UOR6X5X)
    return uor6x5x_work(&uor6x5x_pdata, tc);
#else
	return 0;
#endif	
}


#ifdef CONFIG_CAMERA_GC0308
static void gc0308_init(void)
{
	udelay(1000);
    SET_CBUS_REG_MASK(HHI_ETH_CLK_CNTL,0x30f);// 24M XTAL
    SET_CBUS_REG_MASK(HHI_DEMOD_PLL_CNTL,0x232);// 24M XTAL
    udelay(1000);

    eth_set_pinmux(ETH_BANK0_GPIOC3_C12,ETH_CLK_OUT_GPIOC12_REG3_1, 1);

	#ifdef CONFIG_EXIO_SN7325
	printk( "amlogic camera driver: init CONFIG_SN7325. \n");  

	//PP1: 0 
    //sn7325_set_io_dir(sn7325_get_io_dir() & ~PP1);
    //sn7325_set_output_level(sn7325_get_output_level() & ~PP1);

	//PP2: 0 
    sn7325_set_io_dir(sn7325_get_io_dir() & ~PP2);
    sn7325_set_output_level(sn7325_get_output_level() & ~PP2);

	//PP0: 0 
    sn7325_set_io_dir(sn7325_get_io_dir() & ~PP0);
    sn7325_set_output_level(sn7325_get_output_level() & ~PP0);
	
	//PP6: 0 
    //sn7325_set_io_dir(sn7325_get_io_dir() & ~PP6);
    //sn7325_set_output_level(sn7325_get_output_level() & ~PP6);

    //OD3: 0  
    sn7325_set_io_dir(sn7325_get_io_dir() & ~OD3);
    sn7325_set_output_level(sn7325_get_output_level() & ~OD3);

    //OD2: 0  
    //sn7325_set_io_dir(sn7325_get_io_dir() & ~OD2);
    //sn7325_set_output_level(sn7325_get_output_level() & ~OD2);
	msleep(10);

    //PP2: 1
    sn7325_set_io_dir(sn7325_get_io_dir() & ~PP2);
    sn7325_set_output_level(sn7325_get_output_level() | PP2);
	msleep(10);
	
    //OD3: 1
    sn7325_set_io_dir(sn7325_get_io_dir() & ~OD3);
    sn7325_set_output_level(sn7325_get_output_level() | OD3);
	msleep(20);
	#endif
}

static void gc0308_uninit(void)
{
    #ifdef CONFIG_EXIO_SN7325
	//PP2: 0 
	sn7325_set_io_dir(sn7325_get_io_dir() & ~PP2);
	sn7325_set_output_level(sn7325_get_output_level() & ~PP2);

    //PP0: 1
    sn7325_set_io_dir(sn7325_get_io_dir() & ~PP0);
    sn7325_set_output_level(sn7325_get_output_level() | PP0);

    //OD3: 0  
    sn7325_set_io_dir(sn7325_get_io_dir() & ~OD3);
    sn7325_set_output_level(sn7325_get_output_level() & ~OD3);

	msleep(20); 
    #endif
}
#endif

#ifdef CONFIG_CAMERA_GT2005
static void gt2005_init(void)
{
	udelay(1000);
    SET_CBUS_REG_MASK(HHI_ETH_CLK_CNTL,0x30f);// 24M XTAL
    SET_CBUS_REG_MASK(HHI_DEMOD_PLL_CNTL,0x232);// 24M XTAL
    udelay(1000);

    eth_set_pinmux(ETH_BANK0_GPIOC3_C12,ETH_CLK_OUT_GPIOC12_REG3_1, 1);

	#ifdef CONFIG_EXIO_SN7325
	printk( "amlogic camera driver: init CONFIG_SN7325. \n");
	//PP1: 0 
    sn7325_set_io_dir(sn7325_get_io_dir() & ~PP1);
    sn7325_set_output_level(sn7325_get_output_level() & ~PP1);

	//PP2: 0 
    //sn7325_set_io_dir(sn7325_get_io_dir() & ~PP2);
    //sn7325_set_output_level(sn7325_get_output_level() & ~PP2);

	//PP0: 1 
    //sn7325_set_io_dir(sn7325_get_io_dir() & ~PP0);
    //sn7325_set_output_level(sn7325_get_output_level() | PP0);
	
	//PP6: 0 
    sn7325_set_io_dir(sn7325_get_io_dir() & ~PP6);
    sn7325_set_output_level(sn7325_get_output_level() & ~PP6);

    //OD3: 0  
    //sn7325_set_io_dir(sn7325_get_io_dir() & ~OD3);
    //sn7325_set_output_level(sn7325_get_output_level() & ~OD3);

    //OD2: 0  
    sn7325_set_io_dir(sn7325_get_io_dir() & ~OD2);
    sn7325_set_output_level(sn7325_get_output_level() & ~OD2);
	msleep(10);
	
    //PP1: 1
    sn7325_set_io_dir(sn7325_get_io_dir() & ~PP1);
    sn7325_set_output_level(sn7325_get_output_level() | PP1);
	msleep(10);

    //OD2: 1
    sn7325_set_io_dir(sn7325_get_io_dir() & ~OD2);
    sn7325_set_output_level(sn7325_get_output_level() | OD2);
	msleep(10);

    //PP6: 1
    sn7325_set_io_dir(sn7325_get_io_dir() & ~PP6);
    sn7325_set_output_level(sn7325_get_output_level() | PP6);
	msleep(20);
    #endif
}

static void gt2005_uninit(void)
{
    #ifdef CONFIG_EXIO_SN7325
	//PP6: 0 
    sn7325_set_io_dir(sn7325_get_io_dir() & ~PP6);
    sn7325_set_output_level(sn7325_get_output_level() & ~PP6);

	msleep(20); 
    #endif
}
#endif

#ifdef CONFIG_AML_I2C
static struct i2c_board_info aml_i2c_info[] = {
#ifdef CONFIG_SENSORS_MXC622X
    {
        I2C_BOARD_INFO("gsensors(mxc622x)", 0x15),
		.device_init = NULL,
		.device_uninit = NULL,
    },
#endif

#ifdef CONFIG_EXIO_SN7325
    {
        I2C_BOARD_INFO("externio(sn7325)", SN7325_ADDR),
        .device_init = NULL,
        .device_uninit = NULL,
    },
#endif

#ifdef CONFIG_CAMERA_GT2005
    {
    	I2C_BOARD_INFO("camera(gt2005)", 0x78 >> 1),
    	.device_init = gt2005_init,
    	.device_uninit = NULL,
    },
#endif

#ifdef CONFIG_CAMERA_GC0308
    {
        I2C_BOARD_INFO("camera(gc0308)", 0x42 >> 1),
		.device_init = gc0308_init,
		.device_uninit = NULL,
    },
#endif

#ifdef CONFIG_SND_WM8900
    {
        I2C_BOARD_INFO("audiodec(wm8900)", 0x1A),
        .device_init = NULL,
        .device_uninit = NULL,
    },
#endif

#if defined(CONFIG_TOUCHSCREEN_TSC2007)
    {
        I2C_BOARD_INFO("touch(tsc2007)", 0x48),
		.device_init = NULL,
		.device_uninit = NULL,
    },
#elif defined(CONFIG_TOUCHSCREEN_UOR6X5X)
    {
	    I2C_BOARD_INFO("touch(uor6x5x)", 0x48),
	    .device_init = NULL,
	    .device_uninit = NULL,
	},
#endif
};
#endif

static struct adckey_info key_vol_plus_info[] = {
    {"vol+",   139, 60},
};

static struct adckey_info key_vol_minus_info[] = {
    {"vol-",   266, 60},
};

static struct adckey_info key_search_info[] = {
    {"search",387, 60},
};

static struct adckey_info key_exit_info[] = {
    {"exit",   509, 60},
};

static struct adckey_info key_menu_info[] = {
    {"menu",   633, 60},
};

static struct adckey_info key_home_info[] = {
    {"home",   763, 60},
};

static struct adc_info adc_info[] = {
    {"Press Key vol+",   CHAN_4, ADC_KEY,   &key_vol_plus_info},
    {"Press Key vol-",   CHAN_4, ADC_KEY,   &key_vol_minus_info},
    {"Press Key search", CHAN_4, ADC_KEY,   &key_search_info},
    {"Press Key exit",   CHAN_4, ADC_KEY,   &key_exit_info},
    {"Press Key menu",   CHAN_4, ADC_KEY,   &key_menu_info},
    {"Press Key home",   CHAN_4, ADC_KEY,   &key_home_info},
    {"Battery",          CHAN_5, ADC_OTHER, NULL},
};

void m1_init_machine(void)
{
    #ifdef ENABLE_FONT_RESOURCE
	InitFont();
	#endif
    #ifdef CONFIG_AML_I2C
	sys_test_i2c_init(&aml_i2c_info, ARRAY_SIZE(aml_i2c_info));
	#endif
	adc_init(&adc_info, ARRAY_SIZE(adc_info));
	power_on_backlight();
}

void usb_power_on(void)
{
    set_gpio_val(GPIOA_bank_bit(26), GPIOA_bit_bit23_26(26), 1); //high
    set_gpio_mode(GPIOA_bank_bit(26), GPIOA_bit_bit23_26(26), GPIO_OUTPUT_MODE);
}

#endif

#ifdef ENABLE_FONT_RESOURCE
#include <font/ISO_88591_24.h>

void InitFont(void)
{
	RegisterFont(&ISO_88591_24Font);
}
#endif
