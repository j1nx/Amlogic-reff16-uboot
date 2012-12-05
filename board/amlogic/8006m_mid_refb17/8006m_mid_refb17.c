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

#include <font/ISO_88591_18.h>
#include <font/ISO_88591_20.h>
#include <font/ISO_88591_24.h>
#include <font/ISO_88591_32.h>
#include <font/ISO_88591_40.h>

#include "saradc_reg.h"

/**
 * Routine: board_init
 * Description: Early hardware init.
 */
 
#define msleep(a) udelay(a * 1000)

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
	return (readl(P_PREG_CGPIO_I)&(1<<5))?1:0;
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

#if 0
void set_LCD_PWR_EN(unsigned    char    level)
{   
#if 0
	*((unsigned long*)0xc11080b0) &= ~(1<<13);	//PERIPHS_PIN_MUX_0	clr	bit[13]---disable TCON_CPV1

	*((unsigned long*)0xc11080b4) &= ~(1<<2);	//PERIPHS_PIN_MUX_1	clr	bit[2]---disable SD_D3_B

	*((unsigned long*)0xc11080d0) &= ~(1<<5);	//PERIPHS_PIN_MUX_8	clr	bit[5]---disable I2S_OUT[3]

	*((unsigned long*)0xc11080d4) &= ~(1<<18);	//PERIPHS_PIN_MUX_9	clr	bit[18]---disable WIFI Debug

	*((unsigned long*)0xc11080da) &= ~(1<<4);	//PERIPHS_PIN_MUX_11	clr	bit[4]---disable CP_POD

	*((unsigned long*)0xc11080e0) &= ~(1<<11);	//PERIPHS_PIN_MUX_12	clr	bit[11]---disable ENC_11


	*((unsigned long*)0xC1108030) &= ~(1<<7);	//GPIOA3		OEN = 0 (output) 

	if(level)
	{
		*((unsigned long*)0xC1108034) |= 1<<7;	//GPIOA3		Output level = 1 (pull high) 
	}
	else
	{
		*((unsigned long*)0xC1108034) &= ~(1<<7);	//GPIOA3		Output level = 0 (pull low) 
	}
#else
    //power unhold
    //printf("power hold\n");

	clear_mio_mux(8, 0x20);		
	clear_mio_mux(12, 0x800);		
	clear_mio_mux(0, 0x2000);		
	clear_mio_mux(1, 0x4);		
	clear_mio_mux(9, 0x40000);		
	clear_mio_mux(11, 0x10);		

	set_gpio_mode(GPIOA_bank_bit(3), GPIOA_bit_bit0_14(3), GPIO_OUTPUT_MODE);   // OEN = 0 (output) 
	set_gpio_val(GPIOA_bank_bit(3), GPIOA_bit_bit0_14(3), (level ? 1 : 0));   // Output level = 0 (pull high)
    //*(volatile unsigned long *)(0xC1108030) &= ~(1 << 12);              // OEN = 0 (output) 
    //*(volatile unsigned long *)(0xC1108034) &= ~(1 << 12);               // Output level = 0 (pull low)
#endif
}
#endif

#define is_ac_connected ((READ_CBUS_REG(ASSIST_HW_REV)&(1<<9))? 1:0)

int logo_display(void)
{
    run_command ("nand read ${aml_logo_name} ${loadaddr} 0 ${aml_logo_size}", 0);
    run_command ("bmp display ${loadaddr} 0 0", 0);
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

static void powerkey_led_on(void)
{
	//red light off
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_12, (1<<21));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7, (1<<18));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_10, (1<<29));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_5, ((1<<3)|(1<<12)|(1<<17)));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_8, ((1<<29)|(1<<3)));
	set_gpio_val(GPIOD_bank_bit2_24(22), GPIOD_bit_bit2_24(22), 1);
	set_gpio_mode(GPIOD_bank_bit2_24(22), GPIOD_bit_bit2_24(22), GPIO_OUTPUT_MODE);

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
		powerkey_hold(0);
		powerkey_led_off();
    if(upgrade_step == 3)
    {
	switch(reboot_mode)
        {
            case AMLOGIC_NORMAL_BOOT:
            {
                printf("AMLOGIC_NORMAL_BOOT...\n");
                power_hold();
		powerkey_led_on();
		SET_MPEG_REG_MASK(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);
                logo_display();
		aml_autoscript();
                return  1;
            }
            case AMLOGIC_FACTORY_RESET_REBOOT:
            {
                printf("AMLOGIC_FACTORY_RESET_REBOOT...\n");
		power_hold();
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
                printf("AMLOGIC_CHARGING_REBOOT...\n");
                if(is_ac_connected)
                {
                    power_hold();
#ifdef CONFIG_BATTERY_CHARGING
					battery_charging();
#endif
					powerkey_led_on();

					SET_MPEG_REG_MASK(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);

					logo_display();
                }
                else
                {
                    powerkey_hold(0);
#ifdef CONFIG_BATTERY_CHARGING
                    if(get_powerkey_hold_count())
                    {
                    powerkey_led_on();
                    SET_MPEG_REG_MASK(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);
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
		   // if(powerkey_hold(1000))
                    {
#endif
			power_hold();
			powerkey_led_on();
			SET_MPEG_REG_MASK(VPP_MISC, VPP_POST_FG_OSD2 | VPP_PRE_FG_OSD2);
                        logo_display();
                        printf("Power Up!\n");
                    }
                    /*else
                    {
                        power_unhold();
                        printf("Power Down!\n");
                        hang();
                    }*/
                }
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
    aml_autoscript();

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
#include <adc_test.h>
#ifdef CONFIG_TOUCHSCREEN_UOR6X5X
#include <uor6x5x.h>

//GPIOD_24
#define GPIO_UOR6X5X_PENIRQ ((GPIOD_bank_bit2_24(24)<<16) |GPIOD_bit_bit2_24(24)) 

static int uor6x5x_get_pendown_state(void)
{
    return gpio_get_value(GPIO_UOR6X5X_PENIRQ);
}

#define UOR6X5X_XLCD    800
#define UOR6X5X_YLCD    480
#define UOR6X5X_SWAP_XY 0
#define UOR6X5X_XPOL    0
#define UOR6X5X_YPOL    1
#define UOR6X5X_XMIN 160
#define UOR6X5X_XMAX 3900
#define UOR6X5X_YMIN 346
#define UOR6X5X_YMAX 3616


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
#ifdef CONFIG_TOUCHSCREEN_UOR6X5X
    return uor6x5x_init(&uor6x5x_pdata);
#else
	return 0;
#endif	
}

int touch_work(struct ts_event *tc) 
{
#ifdef CONFIG_TOUCHSCREEN_UOR6X5X
	return uor6x5x_work(&uor6x5x_pdata, tc);
#else
	return 0;
#endif	
}

#ifdef CONFIG_CAMERA_GC0308
static void gc0308_init(void)
{
    printf("%s\n", __FUNCTION__);
	udelay(1000);
    SET_CBUS_REG_MASK(HHI_ETH_CLK_CNTL,0x31e);// 24M XTAL
    SET_CBUS_REG_MASK(HHI_DEMOD_PLL_CNTL,0x232);// 24M XTAL
    udelay(1000);

    eth_set_pinmux(ETH_BANK0_GPIOC3_C12,ETH_CLK_OUT_GPIOC12_REG3_1, 1);

	//30m power_disable	
	set_gpio_val(GPIOA_bank_bit(3), GPIOA_bit_bit0_14(3), 0);
    set_gpio_mode(GPIOA_bank_bit(3), GPIOA_bit_bit0_14(3), GPIO_OUTPUT_MODE);
	//30m pwd enable
	set_gpio_val(GPIOA_bank_bit(1), GPIOA_bit_bit0_14(1), 0);
    set_gpio_mode(GPIOA_bank_bit(1), GPIOA_bit_bit0_14(1), GPIO_OUTPUT_MODE);
	//30m reset low
	set_gpio_val(GPIOE_bank_bit16_21(21), GPIOE_bit_bit16_21(21), 0);
    set_gpio_mode(GPIOE_bank_bit16_21(21), GPIOE_bit_bit16_21(21), GPIO_OUTPUT_MODE);
	msleep(10);
	//30m power_enable	
	set_gpio_val(GPIOA_bank_bit(3), GPIOA_bit_bit0_14(3), 1);
    set_gpio_mode(GPIOA_bank_bit(3), GPIOA_bit_bit0_14(3), GPIO_OUTPUT_MODE);
	msleep(10);
	//30m reset high
	set_gpio_val(GPIOE_bank_bit16_21(21), GPIOE_bit_bit16_21(21), 1);
    set_gpio_mode(GPIOE_bank_bit16_21(21), GPIOE_bit_bit16_21(21), GPIO_OUTPUT_MODE);
	msleep(20);	
}

static void gc0308_uninit(void)
{
    printf("%s\n", __FUNCTION__);
	//30m power_disable	
	set_gpio_val(GPIOA_bank_bit(3), GPIOA_bit_bit0_14(3), 0);
    set_gpio_mode(GPIOA_bank_bit(3), GPIOA_bit_bit0_14(3), GPIO_OUTPUT_MODE);
	//30m pwd enable
	set_gpio_val(GPIOA_bank_bit(1), GPIOA_bit_bit0_14(1), 0);
    set_gpio_mode(GPIOA_bank_bit(1), GPIOA_bit_bit0_14(1), GPIO_OUTPUT_MODE);
	//30m reset low
	set_gpio_val(GPIOE_bank_bit16_21(21), GPIOE_bit_bit16_21(21), 0);
    set_gpio_mode(GPIOE_bank_bit16_21(21), GPIOE_bit_bit16_21(21), GPIO_OUTPUT_MODE);
	msleep(20);
}
#endif

#ifdef CONFIG_SENSORS_MXC622X
static void gsensor_mxc622x_init(void)
{
    printf("%s\n", __FUNCTION__);
	set_gpio_val(GPIOD_bank_bit2_24(16), GPIOD_bit_bit2_24(16), 0);
	set_gpio_mode(GPIOD_bank_bit2_24(16), GPIOD_bit_bit2_24(16), GPIO_OUTPUT_MODE);
	msleep(2);
	
	set_gpio_mode(GPIOD_bank_bit2_24(16), GPIOD_bit_bit2_24(16), GPIO_INPUT_MODE);

	set_gpio_val(GPIOB_bank_bit0_7(1), GPIOB_bit_bit0_7(1), 0);
	set_gpio_mode(GPIOB_bank_bit0_7(1), GPIOB_bit_bit0_7(1), GPIO_OUTPUT_MODE);
	msleep(20);	
}
#endif

#ifdef CONFIG_SND_SOC_RT5621
int audio_clock_config_table[][11][2]=
{
	/*{M, N, OD, XD-1*/
	{
	//24M
		{(71 <<  0) |(4   <<  9) | (1   << 14), (26-1)},//32K	
		{(143 <<  0) |(8   <<  9) | (1   << 14), (19-1)},//44.1K	
		{(128 <<  0) |(5   <<  9) | (1   << 14), (25-1)},//48K	
		{(128 <<  0) |(5   <<  9) | (0   << 14), (25-1)},//96K	
		{(213 <<  0) |(8   <<  9) | (0   << 14), (13-1)},//192K	 
		{(71 <<  0) |(8   <<  9) | (1   << 14), (52-1)},// 8K
		{(143 <<  0) |(8   <<  9) | (1   << 14), (76-1)},// 11025
		{(32 <<  0) |(5   <<  9) | (1   << 14), (25-1)},// 12K
		{(71 <<  0) |(8   <<  9) | (1   << 14), (26-1)},// 16K
		{(143 <<  0) |(8   <<  9) | (1   << 14), (38-1)},// 22050
		{(64 <<  0) |(5   <<  9) | (1   << 14), (25-1)}   // 24K
	},
	{
	//25M
		{(19 <<  0) |(1   <<  9) | (1   << 14), (29-1)},//32K	
		{(28 <<  0) |(1   <<  9) | (1   << 14), (31-1)},//44.1K	
		{(173 <<  0) |(8   <<  9) | (1   << 14), (22-1)},//48K	
		{(173 <<  0) |(8   <<  9) | (1   << 14), (11-1)},//96K	
		{(118 <<  0) |(5   <<  9) | (1   << 14), (6-1)},//192K	  
		{(19 <<  0) |(4   <<  9) | (1   << 14), (29-1)},// 8K
		{(7 <<  0) |(1   <<  9) | (1   << 14), (31-1)},// 11025
		{(173 <<  0) |(8   <<  9) | (1   << 14), (88-1)},// 12K
		{(19 <<  0) |(2   <<  9) | (1   << 14), (29-1)},// 16K
		{(14 <<  0) |(1   <<  9) | (1   << 14), (31-1)},// 22050
		{(173 <<  0) |(8   <<  9) | (1   << 14), (44-1)}// 24K
	}
};

void audio_set_clk()
{
    int i;
    struct clk *clk;
    int xtal = 0;
    int index=1;
	
    int (*audio_clock_config)[2];


	audio_clock_config = audio_clock_config_table[xtal];		
	
    // gate the clock off
    WRITE_MPEG_REG( HHI_AUD_CLK_CNTL, READ_MPEG_REG(HHI_AUD_CLK_CNTL) & ~(1 << 8));

    // Put the PLL to sleep
    WRITE_MPEG_REG( HHI_AUD_PLL_CNTL, READ_MPEG_REG(HHI_AUD_PLL_CNTL) | (1 << 15));

    // Bring out of reset but keep bypassed to allow to stablize
    WRITE_MPEG_REG( HHI_AUD_PLL_CNTL, (1 << 15) | (audio_clock_config[index][0] & 0x7FFF) );
    // Set the XD value
    WRITE_MPEG_REG( HHI_AUD_CLK_CNTL, (READ_MPEG_REG(HHI_AUD_CLK_CNTL) & ~(0xff << 0)) | audio_clock_config[index][1]);
    // delay 5uS
	for (i = 0; i < 500000; i++) ;
    // Bring the PLL out of sleep
    WRITE_MPEG_REG( HHI_AUD_PLL_CNTL, READ_MPEG_REG(HHI_AUD_PLL_CNTL) & ~(1 << 15));

    // gate the clock on
    WRITE_MPEG_REG( HHI_AUD_CLK_CNTL, READ_MPEG_REG(HHI_AUD_CLK_CNTL) | (1 << 8));   
}

static void rt5621_init(void)
{
    printf("%s\n", __FUNCTION__);
	//AUDIO_OUT_TEST_N
	clear_mio_mux(1, (1<<6));
	clear_mio_mux(0, (1<<17));
	clear_mio_mux(6, (1<<24));
	WRITE_CBUS_REG_BITS(0x200b,0,16,1); //Set TEST_N Output mode
	set_mio_mux(1, (1<<11)|(1<<15)|(1<<19));
	set_mio_mux(0, (1<<18));

	//AUDIO_IN_JTAG
	clear_mio_mux(1, (1<<6));
	set_mio_mux(1,(1<<11)|(1<<15)|(1<<19));
	set_mio_mux(8, (1<<8)|(1<<9)|(1<<10));
	set_mio_mux(8, (1<<11));

	msleep(2);
	WRITE_CBUS_REG_BITS(PERIPHS_PIN_MUX_1, 1, 11, 1);
	audio_set_clk();
	mdelay(10);
}
#endif

#ifdef CONFIG_AML_I2C
#include <aml_i2c.h>
extern void sys_test_i2c_init(struct i2c_board_info *i2c_info, unsigned int len);
static struct i2c_board_info aml_i2c_info[] = {
#ifdef CONFIG_SENSORS_MXC622X
		{
			I2C_BOARD_INFO("gsensors(mxc622x)", 0x15),
			.i2c_id = SOFT_I2C,
			.device_init = gsensor_mxc622x_init,
			.device_uninit = NULL,
		},
#endif
	
#ifdef CONFIG_CAMERA_GC0308
		{
			I2C_BOARD_INFO("camera(gc0308)", 0x42 >> 1),
			.i2c_id = SOFT_I2C,	
			.device_init = gc0308_init,
			.device_uninit = NULL,
		},
#endif
	
#ifdef CONFIG_SND_SOC_RT5621
		{
			I2C_BOARD_INFO("audiodec(alc5623)", 0x1a),
			.i2c_id = HARD_I2C,	
			.device_init = rt5621_init,
			.device_uninit = NULL,
		},
#endif
	
#ifdef CONFIG_TOUCHSCREEN_UOR6X5X
		{
			I2C_BOARD_INFO("touch(uor6x5x)", 0x48),
			.i2c_id = HARD_I2C,	
			.device_init = NULL,
			.device_uninit = NULL,
		},
#endif

#ifdef CONFIG_LTR_328ALS
       {
           I2C_BOARD_INFO("lsensor(ltr_328als)", 0x29),
		   .i2c_id = SOFT_I2C,	
		   .device_init = NULL,
		   .device_uninit = NULL,
	   },
#endif
};

struct aml_i2c_platform aml_i2c_plat = {
    .wait_count     = 1000000,
    .wait_ack_interval  = 5,
    .wait_read_interval = 5,
    .wait_xfer_interval = 5,
    .master_no      = AML_I2C_MASTER_A,
    .use_pio            = 0,
    .master_i2c_speed   = AML_I2C_SPPED_200K,

    .master_a_pinmux = {
        .scl_reg    = MESON_I2C_MASTER_A_GPIOC_21_REG,
        .scl_bit    = MESON_I2C_MASTER_A_GPIOC_21_BIT,
        .sda_reg    = MESON_I2C_MASTER_A_GPIOC_22_REG,
        .sda_bit    = MESON_I2C_MASTER_A_GPIOC_22_BIT,
    }
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
	RegisterFont(&ISO_88591_24Font);
	#endif
    #ifdef CONFIG_AML_I2C
	sys_test_i2c_init(aml_i2c_info, ARRAY_SIZE(aml_i2c_info));
	#endif
	adc_init(&adc_info, ARRAY_SIZE(adc_info));
	//run_command("lcd clear",0);
	power_on_backlight();
}

void usb_power_on(void)
{
    set_gpio_val(GPIOA_bank_bit(26), GPIOA_bit_bit23_26(26), 1); //high
    set_gpio_mode(GPIOA_bank_bit(26), GPIOA_bit_bit23_26(26), GPIO_OUTPUT_MODE);
}

#endif
