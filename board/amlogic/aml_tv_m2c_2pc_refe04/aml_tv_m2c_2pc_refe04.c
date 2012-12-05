
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
#include "saradc_reg.h"

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
				.options = (NAND_TIMING_MODE5 | NAND_ECC_BCH8_MODE | NAND_TWO_PLANE_MODE),
			},
    	},
		.T_REA = 20,
		.T_RHOH = 15,
	},
	{
		.name = NAND_NORMAL_NAME,
		.chip_enable_pad = AML_NAND_CE0,
		.ready_busy_pad = AML_NAND_CE0,
		.platform_nand_data = {
			.chip =  {
				.nr_chips = 1,
				.options = (NAND_TIMING_MODE5 | NAND_ECC_BCH8_MODE | NAND_TWO_PLANE_MODE),
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

#if 0
static int  sdio_init(unsigned port)
{
    setbits_le32(P_PREG_EGPIO_EN_N ,(0x1<<27));
    return cpu_sdio_init(port);
}

static int  sdio_detect(unsigned port)
{
	return (readl(P_PREG_EGPIO_I)&(1<<27))?1:0;
}

static void sdio_pwr_on(unsigned port)
{
	clrbits_le32(P_PREG_CGPIO_O,(1<<12));
	clrbits_le32(P_PREG_CGPIO_EN_N,(1<<12));
}

static void sdio_pwr_off(unsigned port)
{
	setbits_le32(P_PREG_CGPIO_O,(1<<12));
	clrbits_le32(P_PREG_CGPIO_EN_N,(1<<12));
}

static void sdio_pwr_prepare(unsigned port)
{
    clrbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<10);
    clrbits_le32(P_PREG_EGPIO_EN_N,(0x1<<31));
    clrbits_le32(P_PREG_EGPIO_O,(0x1<<31));
    clrbits_le32(P_PREG_CGPIO_EN_N,0x1f);
    clrbits_le32(P_PREG_CGPIO_O,0x1f);
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

#endif

extern int aml_autoscript(void);
extern char version_string[];


inline int get_source_key()
{
    int adc_val = get_adc_sample(0);
    printf("get_source_key: 0x%x\n", adc_val);
    return (((adc_val > 0x1cf) && (adc_val < 0x247)) ? 1 : 0);
}
int switch_boot_mode(void)
{
    saradc_enable();
    run_command ("bl off", 1);
    switch(reboot_mode)
    {
        case AMLOGIC_NORMAL_BOOT:
        {
            printf("AMLOGIC_NORMAL_BOOT...\n");
            run_command ("osd open 24 1920 1080 1;nand info; nand read ${loadaddr} ${logoaddr} ${logosize}; bmp display ${loadaddr} 0 0", 0);
        	run_command ("bl on", 1);
            return  1;
        }
        case AMLOGIC_FACTORY_RESET_REBOOT:
        {
            printf("AMLOGIC_FACTORY_RESET_REBOOT...\n");
            run_command("run recoveryboot", 0);
            break;
        }
        case AMLOGIC_UPDATE_REBOOT:
        {
            printf("AMLOGIC_UPDATE_REBOOT...\n");
			run_command("run recoveryboot", 0);
            break;
        }
        default:
        {
            printf("default boot......\n");
            unsigned long standby = simple_strtoul (getenv ("factory_standby"), NULL, 16);
            if(standby == 1){
               printf("reset uboot for entering standby\n");
               reboot_mode = AMLOGIC_COOL_REBOOT;
               run_command("reset", 0);
            }
            else
            {
              run_command ("osd open 24 1920 1080 1;nand info; nand read ${loadaddr} ${logoaddr} ${logosize}; bmp display ${loadaddr} 0 0", 0);
            	run_command ("bl on", 1);
            }
            break;
        }
    }
	//__udelay(50000);
	char *version=getenv ("version");

	//printf("version = %s\n", version);
	if((version && strcmp(version,version_string)!=0)||(version==NULL)||(version&&version[0]=='1'))
	{
        char cmd[256]={0};
		sprintf(cmd,"%s%s","set version ",version_string);
		//printf("version = %s\n", cmd);
		run_command (cmd, 0);
		run_command ("save", 0);
	}
    if(get_source_key()){
		saradc_disable();
      aml_autoscript();
    	}
	saradc_disable();
}

void cooling_or_hearting(void)
{
	unsigned long cold_heart_enable = simple_strtoul (getenv ("cold_heart_enable"), NULL, 16);
	printf("cold_heart_enable = %d\n", cold_heart_enable);
	run_command ("set cold_heart_enable 0", 0);
	run_command ("save", 0);

if(reboot_mode == AMLOGIC_COOL_REBOOT)
{
    printf("cold_reboot from key_power\n");
    //vcc_12v/24v power down
    set_gpio_val(GPIOX_bank_bit64_70(70), GPIOX_bit_bit64_70(70), 1);
    set_gpio_mode(GPIOX_bank_bit64_70(70), GPIOX_bit_bit64_70(70), 0);
    reboot_mode = AMLOGIC_NORMAL_BOOT;
    cold_heart_enable = 1;
}
else//normal boot
{
    printf("normal boot, reset standby gpio\n");
    
    //3V3_ON_OFF
    set_gpio_val(GPIOB_bank_bit0_8(8), GPIOB_bit_bit0_8(8), 0);
    set_gpio_mode(GPIOB_bank_bit0_8(8), GPIOB_bit_bit0_8(8), 0);
    //1V3_ON_OFF
    set_gpio_val(GPIOB_bank_bit0_8(5), GPIOB_bit_bit0_8(5), 0);
    set_gpio_mode(GPIOB_bank_bit0_8(5), GPIOB_bit_bit0_8(5), 0);

   	//__udelay(300000);
    //vcc_12v/24v power down
    set_gpio_val(GPIOX_bank_bit64_70(70), GPIOX_bit_bit64_70(70), 0);
    set_gpio_mode(GPIOX_bank_bit64_70(70), GPIOX_bit_bit64_70(70), 0);

	 //GPIOX_48 LED_G
    set_gpio_val(GPIOX_bank_bit32_63(48), GPIOX_bit_bit32_63(48), 1);
    set_gpio_mode(GPIOX_bank_bit32_63(48), GPIOX_bit_bit32_63(48), GPIO_OUTPUT_MODE);

	//GPIOX_56 LED_R
    set_gpio_val(GPIOX_bank_bit32_63(56), GPIOX_bit_bit32_63(56), 0);
    set_gpio_mode(GPIOX_bank_bit32_63(56), GPIOX_bit_bit32_63(56), GPIO_OUTPUT_MODE);

	//GPIOX_51 AMP_RESET
    set_gpio_val(GPIOX_bank_bit32_63(51), GPIOX_bit_bit32_63(51), 0);
    set_gpio_mode(GPIOX_bank_bit32_63(51), GPIOX_bit_bit32_63(51), GPIO_OUTPUT_MODE);

	//GPIOX_50 USB_HUB_RESET
    //set_gpio_val(GPIOX_bank_bit32_63(50), GPIOX_bit_bit32_63(50), 0);
    //set_gpio_mode(GPIOX_bank_bit32_63(50), GPIOX_bit_bit32_63(50), GPIO_OUTPUT_MODE);

	//GPIOX_32 si2176_RESET
    set_gpio_val(GPIOX_bank_bit32_63(32), GPIOX_bit_bit32_63(32), 0);
    set_gpio_mode(GPIOX_bank_bit32_63(32), GPIOX_bit_bit32_63(32), GPIO_OUTPUT_MODE);
	
	//PHY_RESET x57
	set_gpio_val(GPIOX_bank_bit32_63(57), GPIOX_bit_bit32_63(57), 0);
    set_gpio_mode(GPIOX_bank_bit32_63(57), GPIOX_bit_bit32_63(57), 0);

	//AMP_MUTE x69
    set_gpio_val(GPIOX_bank_bit64_70(69), GPIOX_bit_bit64_70(69), 1);
    set_gpio_mode(GPIOX_bank_bit64_70(69), GPIOX_bit_bit64_70(69), 0);

	//USB_PWR_CTL x44
	//set_gpio_val(GPIOX_bank_bit32_63(44), GPIOX_bit_bit32_63(44), 0);
   // set_gpio_mode(GPIOX_bank_bit32_63(44), GPIOX_bit_bit32_63(44), 0);
}

	if(cold_heart_enable)
	{
		cold_copy2sram((unsigned long *)AHB_SRAM_BASE, (unsigned long *)CONFIG_COLD_HEART_STORE_ADDR, READ_SIZE);
	}
}
