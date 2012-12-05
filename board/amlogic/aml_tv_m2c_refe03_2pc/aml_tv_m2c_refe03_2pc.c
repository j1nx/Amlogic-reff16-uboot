
#include <common.h>
#include <asm/mach-types.h>
#include <asm/arch/memory.h>
#include <asm/arch/am_reg_addr.h>
#include <asm/arch/aml_card_sd.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/am_eth_reg.h>
#include <asm/arch/gpio.h>
#include <asm/arch/nand.h>
#include <asm/arch/usb.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/mtd.h>
#include <share_kernel.h>
#include <asm/arch/osd.h>
#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/io.h>
#include "saradc_reg.h"


static long panel_power_up = 0;
static long panel_power_down = 0;
static long backlight_on = 2000000;
static long backlight_off = 100;

static unsigned int TCON_PWM_HE_MAX;
static unsigned int TCON_PWM_VS_MAX;

static unsigned int TCON_PWM_HS1;
static unsigned int TCON_PWM_HE1;
static unsigned int TCON_PWM_HS2;
static unsigned int TCON_PWM_HE2;
static unsigned int TCON_PWM_HS3;
static unsigned int TCON_PWM_HE3;

static unsigned int TCON_PWM_VS1;
static unsigned int TCON_PWM_VS2;
static unsigned int TCON_PWM_VS3;

/**
 * Routine: board_init
 * Description: Early hardware init.
 */
extern void init_pwm_tcon();
extern int osd_width;
extern int osd_height;

static struct aml_nand_platform aml_nand_mid_platform[] = {
#ifdef CONFIG_NAND_BOOT
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
#endif	
	{
		.name = NAND_NORMAL_NAME,
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
	}
};

#ifdef CONFIG_USB_DWC_OTG_HCD
struct amlogic_usb_config usb_config_m2c={
	USB_PHY_CLOCK_SEL_XTAL_DIV2,
	0, // no use if clock sel == xtal or xtal/2
	CONFIG_M2_USBPORT_BASE,
	USB_ID_MODE_SW_HOST,
	0, //set_vbus_power
};
#endif

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
	gd->bd->bi_arch_number=2956;
	gd->bd->bi_boot_params=BOOT_PARAMS_OFFSET;

#ifdef CONFIG_CMD_KGDB

	kgdb_init();
#endif
#ifdef CONFIG_USB_DWC_OTG_HCD
	board_usb_init(&usb_config_m2c);
#endif
    //init_osd_param();
    unsigned long arg = simple_strtoul(getenv ("768p"), NULL, 16);
    if(1 == arg){
        osd_width = 1368; 
        osd_height = 768;
    }else{
        osd_width = 1920; 
        osd_height = 1080;
    }
    init_pwm_tcon();    
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
          
	//eth_clk_set(ETH_CLKSRC_SYS_D3,900*CLK_1M/3,50*CLK_1M);
	eth_clk_set(ETH_CLKSRC_SYS_D3,get_cpu_clk()*2/3,50*CLK_1M);
	// GPIOX59-X67 for M2_socket
	// GPIOE_57/NA nRst;
	eth_set_pinmux(ETH_BANK1_GPIOX59_X67,ETH_CLK_OUT_GPIOX68_REG3_14,0);

	CLEAR_CBUS_REG_MASK(PREG_ETHERNET_ADDR0, 1);
	SET_CBUS_REG_MASK(PREG_ETHERNET_ADDR0, (1 << 1));
	SET_CBUS_REG_MASK(PREG_ETHERNET_ADDR0, 1);
	udelay(100);
	#if 0
	/*reset*/
	set_gpio_mode(PREG_FGPIO, 25, GPIO_OUTPUT_MODE);
	set_gpio_val(PREG_FGPIO, 25, 0);
	udelay(100);   //GPIOE_bank_bit16_21(16) reset end;
	set_gpio_val(PREG_FGPIO, 25, 1);
	#endif

	/*reset*/    
    set_gpio_val(GPIOX_bank_bit32_63(57), GPIOX_bit_bit32_63(57), 1);
	set_gpio_mode(GPIOX_bank_bit32_63(57), GPIOX_bit_bit32_63(57), 0);
	udelay(100);
    set_gpio_val(GPIOX_bank_bit32_63(57), GPIOX_bit_bit32_63(57), 0);
    set_gpio_mode(GPIOX_bank_bit32_63(57), GPIOX_bit_bit32_63(57), 0);
    udelay(100);
    set_gpio_val(GPIOX_bank_bit32_63(57), GPIOX_bit_bit32_63(57), 1);
	set_gpio_mode(GPIOX_bank_bit32_63(57), GPIOX_bit_bit32_63(57), 0);

	udelay(10);    //waiting reset end;
	aml_eth_init(bis);
	return 0;
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
    setbits_le32(P_PREG_EGPIO_EN_N ,(0x1<<27));
    cpu_sdio_power_on(1);
    return cpu_sdio_init(port);
}

static int  sdio_detect(unsigned port)
{
	return (readl(P_PREG_EGPIO_I)&(1<<11))?1:0;
	//return (readl(P_PREG_EGPIO_I)&(1<<27))?1:0;
}

static void sdio_pwr_on(unsigned port)
{
	cpu_sdio_power_on(1);
	//clrbits_le32(P_PREG_CGPIO_O,(1<<12));
	//clrbits_le32(P_PREG_CGPIO_EN_N,(1<<12));
}

static void sdio_pwr_off(unsigned port)
{
	cpu_sdio_power_off(1);
	//setbits_le32(P_PREG_CGPIO_O,(1<<12));
	//clrbits_le32(P_PREG_CGPIO_EN_N,(1<<12));
}

static void sdio_pwr_prepare(unsigned port)
{
    cpu_sdio_pwr_prepare(port);
    //clrbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<10);
    //clrbits_le32(P_PREG_EGPIO_EN_N,(0x1<<31));
    //clrbits_le32(P_PREG_EGPIO_O,(0x1<<31));
    //clrbits_le32(P_PREG_CGPIO_EN_N,0x1f);
    //clrbits_le32(P_PREG_CGPIO_O,0x1f);
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
	mmc->voltages = MMC_VDD_32_33 | MMC_VDD_33_34;
	mmc->host_caps =  MMC_MODE_4BIT | MMC_MODE_HS;
	mmc->bus_width = 1;	
	mmc->clock = 200000;
	mmc->f_min = 200000;
	mmc->f_max = 24000000;
	mmc_register(mmc);
}
int board_mmc_init(bd_t	*bis)
{
	board_mmc_register(SDIO_PORT_A);
//	board_mmc_register(SDIO_PORT_B);
//	board_mmc_register(SDIO_PORT_C);
//	board_mmc_register(SDIO_PORT_B1);
    //USB_PWR_CTL POWER ON
    set_gpio_val(GPIOX_bank_bit32_63(44), GPIOX_bit_bit32_63(44), 1);
	set_gpio_mode(GPIOX_bank_bit32_63(44), GPIOX_bit_bit32_63(44), 0);
	return 0;
}
//int serial_set_pin_port(int port)
//{
//    setbits_le32(P_PERIPHS_PIN_MUX_2,((1<<15)|(1<<11)));
//    return 0;
//}

extern int aml_autoscript(void);

inline int get_source_key()
{
    int adc_val = get_adc_sample(0);
    printf("get_source_key: 0x%x\n", adc_val);
    return (((adc_val > 0x226) && (adc_val < 0x294)) ? 1 : 0);
}

static void led_off()
{
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_2, 1<<17);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_4, 1<<12);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, 1<<8);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, 1<<23);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_2, 1<<17);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_2, 1<<28);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_4, 1<<22);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7, 1<<17);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_10, 1<<6);
    
    set_gpio_val(GPIOX_bank_bit32_63(56), GPIOX_bit_bit32_63(56), 1);
    set_gpio_mode(GPIOX_bank_bit32_63(56), GPIOX_bit_bit32_63(56), 0);
    set_gpio_val(GPIOX_bank_bit32_63(48), GPIOX_bit_bit32_63(48), 0);
    set_gpio_mode(GPIOX_bank_bit32_63(48), GPIOX_bit_bit32_63(48), 0);    
}

static void led_on()
{
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_2, 1<<17);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_4, 1<<12);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, 1<<8);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, 1<<23);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_2, 1<<17);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_2, 1<<28);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_4, 1<<22);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7, 1<<17);
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_10, 1<<6);
    
    set_gpio_val(GPIOX_bank_bit32_63(56), GPIOX_bit_bit32_63(56), 0);
    set_gpio_mode(GPIOX_bank_bit32_63(56), GPIOX_bit_bit32_63(56), 0);
    set_gpio_val(GPIOX_bank_bit32_63(48), GPIOX_bit_bit32_63(48), 1);
    set_gpio_mode(GPIOX_bank_bit32_63(48), GPIOX_bit_bit32_63(48), 0);    
}

static int chip_reset(void)
{
#ifdef AML_BOOT_SPI  	
	/* if uboot is on flash then boot agian from flash */
	writel(WATCHDOG_ENABLE_MASK, IREG_WATCHDOG_CONTROL0);
#endif	
	WRITE_CBUS_REG(WATCHDOG_RESET, 0);
    WRITE_CBUS_REG(WATCHDOG_TC, 1 << 22 | 100); /*100*10uS=1ms*/
    WRITE_CBUS_REG(WATCHDOG_RESET, 0);
    __udelay(10000);/**/
    serial_puts("Chip watchdog reset error!!!please reset it by hardware\n");
    while (1) ;
	return 0;
}

static void init_pwm()
{
    unsigned long arg = simple_strtoul(getenv ("768p"), NULL, 16);
    if(1 == arg){
       TCON_PWM_HE_MAX = 1559;
       TCON_PWM_VS_MAX = 805;
    }else{
       TCON_PWM_HE_MAX = 2199;
       TCON_PWM_VS_MAX = 1126;
    }

    TCON_PWM_HS1 = 0;
    TCON_PWM_HE1 = TCON_PWM_HE_MAX-1;
    TCON_PWM_HS2 = 0;
    TCON_PWM_HE2 = TCON_PWM_HE_MAX-1;
    TCON_PWM_HS3 = 0;
    TCON_PWM_HE3 = TCON_PWM_HE_MAX-1;    

    TCON_PWM_VS1 = 0;
    TCON_PWM_VS2 = TCON_PWM_VS_MAX/3;
    TCON_PWM_VS3 = 2*TCON_PWM_VS_MAX/3;

	//Init Analog pwm pinmux
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_4, (1<<11));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, 1);
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, (1<<1));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7, (1<<18));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_10, (1<<6));
	
    SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, (1<<24));
    WRITE_CBUS_REG(PWM_MISC_REG_AB, 0x2);
    WRITE_CBUS_REG(PWM_PWM_B, 0);


    //Init Digital pwm pinmux, tcon GPIOB2, GPIOB3,GPIOB4
    SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_0, (1<<4)|(1<<5)|(1<<3));


    WRITE_CBUS_REG_BITS(TCON_MISC_SEL_ADDR, 1, OEH_SEL, 1);
    WRITE_CBUS_REG(OEH_HS_ADDR, TCON_PWM_HS1);
    WRITE_CBUS_REG(OEH_HE_ADDR, TCON_PWM_HE1);
    WRITE_CBUS_REG(OEH_VS_ADDR, TCON_PWM_VS1);

    
    WRITE_CBUS_REG(OEH_VE_ADDR, TCON_PWM_VS2);//180HZ


    WRITE_CBUS_REG_BITS(TCON_MISC_SEL_ADDR, 1, CPV1_SEL, 1);
    WRITE_CBUS_REG(CPV1_HS_ADDR, TCON_PWM_HS2);
    WRITE_CBUS_REG(CPV1_HE_ADDR, TCON_PWM_HE2);
    WRITE_CBUS_REG(CPV1_VS_ADDR, TCON_PWM_VS2);
    WRITE_CBUS_REG(CPV1_VE_ADDR, TCON_PWM_VS2+TCON_PWM_VS2);

	WRITE_CBUS_REG_BITS(TCON_MISC_SEL_ADDR, 1, OEV2_SEL, 1);
	WRITE_CBUS_REG_BITS(TCON_MISC_SEL_ADDR, 1, OEV1_SEL, 1);
	WRITE_CBUS_REG_BITS(TCON_MISC_SEL_ADDR, 1, OEV3_SEL, 1);
	CLEAR_CBUS_REG_MASK(TCON_MISC_SEL_ADDR, 1<<OEV_UNITE);
    WRITE_CBUS_REG(OEV1_HS_ADDR, TCON_PWM_HS3);
    WRITE_CBUS_REG(OEV1_HE_ADDR, TCON_PWM_HE3);
    WRITE_CBUS_REG(OEV1_VS_ADDR, TCON_PWM_VS3);
    WRITE_CBUS_REG(OEV1_VE_ADDR, TCON_PWM_VS3+TCON_PWM_VS2);
}

inline void power_on_off_sequence()
{
    char tmp[32];
    char nandargs[256];
    
    strcpy(nandargs, "set nandargs 'nand info;setenv bootargs mem=${memsize} a9_clk=${cpuclock} clk81=${gpuclock} mac=${ethaddr} board_ver=v2 logo=lvds1080p,loaded,osd1 console=${console}");
    
    //read power on/off sequence
    if(getenv("panel_power_up") != NULL){
        panel_power_up = simple_strtol(getenv("panel_power_up"), NULL, 10);
    }
    if(getenv("panel_power_down") != NULL){
        panel_power_down = simple_strtol(getenv("panel_power_down"), NULL, 10);
        sprintf(tmp, " panel_power_down=%ld", panel_power_down);
        strcat(nandargs, tmp);
    }
    if(getenv("backlight_on") != NULL){
        backlight_on = simple_strtol(getenv("backlight_on"), NULL, 10);
    }
    if(getenv("backlight_off") != NULL){
        backlight_off = simple_strtol(getenv("backlight_off"), NULL, 10);
        sprintf(tmp, " backlight_off=%ld", backlight_off);
        strcat(nandargs, tmp);
    }
    strcat(nandargs, "'");
    run_command(nandargs, 1);
    printf("nandargs[%s]\n", nandargs);
    
    printf("panel_power_up[%ld]panel_power_down[%ld]backlight_on[%ld]backlight_off[%ld]\n", panel_power_up, panel_power_down, backlight_on, backlight_off);

}

inline void init_display()
{
    unsigned long arg = simple_strtoul(getenv ("768p"), NULL, 16);
    
    init_pwm();
    run_command ("bl set 255", 1);
#ifdef TSD_SUPPORT 
    if(1 == arg){
        run_command ("osd open 24 1368 768 1;mmcinfo; mmc read 0 ${loadaddr} ${logoaddr} ${logosize}; bmp display ${loadaddr} 0 0", 0);
    }else{
        run_command ("osd open 24 1920 1080 1;mmcinfo; mmc read 0 ${loadaddr} ${logoaddr} ${logosize}; bmp display ${loadaddr} 0 0", 0);
    }
#else
    if(1 == arg){
        run_command ("osd open 24 1368 768 1;nand info; nand read ${loadaddr} ${logoaddr} ${logosize}; bmp display ${loadaddr} 0 0", 0);
    }else{
        run_command ("osd open 24 1920 1080 1;nand info; nand read ${loadaddr} ${logoaddr} ${logosize}; bmp display ${loadaddr} 0 0", 0);
    }
#endif
    udelay(panel_power_up);//T2
    set_gpio_val(GPIOX_bank_bit32_63(54), GPIOX_bit_bit32_63(54), 1);
    set_gpio_mode(GPIOX_bank_bit32_63(54), GPIOX_bit_bit32_63(54), 0);
    udelay(backlight_on);//T5
    run_command ("bl on", 1);
    udelay(1000000);
    run_command ("bl set 0", 1);
}

int switch_boot_mode(void)
{
    saradc_enable();
	run_command ("bl off", 1);

    if(get_source_key())
      aml_autoscript();
      
    switch(reboot_mode)
    {
        case CONFIG_COLD_HEART_KEY+1:
        case AMLOGIC_NORMAL_BOOT:
        {
            //led_on();
            printf("AMLOGIC_NORMAL_BOOT...\n");
            //power_on_off_sequence();
            init_display();
            return  1;
        }
        case AMLOGIC_FACTORY_RESET_REBOOT:
        {
            printf("AMLOGIC_FACTORY_RESET_REBOOT...\n");
            init_display();
            #ifdef TSD_SUPPORT
            run_command("run emmcrecoveryboot", 0);
            #else
            run_command("run recoveryboot", 0);
            #endif
            break;
        }
        case AMLOGIC_UPDATE_REBOOT:
        {
            printf("AMLOGIC_UPDATE_REBOOT...\n");
            init_display();
            #ifdef TSD_SUPPORT
            run_command("run emmcrecoveryboot", 0);
            #else
            run_command("run recoveryboot", 0);
            #endif
            break;
        }
        default:
        {
            printf("default boot......\n");
            unsigned long standby = simple_strtoul (getenv ("factory_standby"), NULL, 16);
            if(standby == 1){
               printf("reset uboot for entering standby\n");
               reboot_mode = AMLOGIC_COOL_REBOOT;
               //run_command("reset", 0);
               chip_reset();
            }
            else
            {
                //power_on_off_sequence();
                init_display();
            }
            break;
        }
    }
}

void cooling_or_hearting(void)
{
	unsigned long cold_heart_enable = simple_strtoul (getenv ("cold_heart_enable"), NULL, 16);
	printf("cold_heart_enable = %d reboot_mode=%x\n", cold_heart_enable, reboot_mode);
	//run_command ("set cold_heart_enable 0", 0);
	//run_command ("save", 0);	

	if(reboot_mode == AMLOGIC_COOL_REBOOT || cold_heart_enable == 1)
	{
	    printf("cold_reboot from key_power\n");
        //vcc_12v/24v power down
        set_gpio_val(GPIOX_bank_bit64_70(70), GPIOX_bit_bit64_70(70), 1);
        set_gpio_mode(GPIOX_bank_bit64_70(70), GPIOX_bit_bit64_70(70), 0);
        
        //LED off
        led_off(); 

	    reboot_mode = AMLOGIC_NORMAL_BOOT;
	    cold_heart_enable = 1;
	}
    else//normal boot
    {
        printf("normal boot, reset standby gpio\n");
        
	    //move to lowlevel_init
        //3V3_ON_OFF
        //set_gpio_val(GPIOB_bank_bit0_8(8), GPIOB_bit_bit0_8(8), 0);
        //set_gpio_mode(GPIOB_bank_bit0_8(8), GPIOB_bit_bit0_8(8), 0);
        //1V3_ON_OFF
        //set_gpio_val(GPIOB_bank_bit0_8(5), GPIOB_bit_bit0_8(5), 0);
        //set_gpio_mode(GPIOB_bank_bit0_8(5), GPIOB_bit_bit0_8(5), 0);
        //DDR_ON_OFF
        //set_gpio_val(GPIOB_bank_bit0_8(6), GPIOB_bit_bit0_8(6), 0);
        //set_gpio_mode(GPIOB_bank_bit0_8(6), GPIOB_bit_bit0_8(6), 0); 
        
   		__udelay(300000);
        //vcc_12v/24v power on
        set_gpio_val(GPIOX_bank_bit64_70(70), GPIOX_bit_bit64_70(70), 0);
        set_gpio_mode(GPIOX_bank_bit64_70(70), GPIOX_bit_bit64_70(70), 0);

        led_on();
        
    	//GPIOX_51 AMP_RESET
        set_gpio_val(GPIOX_bank_bit32_63(51), GPIOX_bit_bit32_63(51), 0);
        set_gpio_mode(GPIOX_bank_bit32_63(51), GPIOX_bit_bit32_63(51), GPIO_OUTPUT_MODE);
    	//GPIOX_50 USB_HUB_RESET
        //set_gpio_val(GPIOX_bank_bit32_63(50), GPIOX_bit_bit32_63(50), 0);
        //set_gpio_mode(GPIOX_bank_bit32_63(50), GPIOX_bit_bit32_63(50), GPIO_OUTPUT_MODE);

    	//GPIOX_32 si2176_RESET
        set_gpio_val(GPIOX_bank_bit32_63(32), GPIOX_bit_bit32_63(32), 0);
        set_gpio_mode(GPIOX_bank_bit32_63(32), GPIOX_bit_bit32_63(32), GPIO_OUTPUT_MODE);
    	
    	//PHY_RESET
    	set_gpio_val(GPIOX_bank_bit32_63(57), GPIOX_bit_bit32_63(57), 0);
        set_gpio_mode(GPIOX_bank_bit32_63(57), GPIOX_bit_bit32_63(57), 0);

    	//AMP_MUTE
        set_gpio_val(GPIOX_bank_bit64_70(69), GPIOX_bit_bit64_70(69), 0);
        set_gpio_mode(GPIOX_bank_bit64_70(69), GPIOX_bit_bit64_70(69), 0);
        
    }
    
	if(cold_heart_enable)
	{
		cold_copy2sram((unsigned long *)AHB_SRAM_BASE, (unsigned long *)CONFIG_COLD_HEART_STORE_ADDR, READ_SIZE);
	}
}

