
#include <common.h>
#include <asm/mach-types.h>
#include <asm/arch/memory.h>
#include <asm/arch/am_reg_addr.h>
#include <asm/arch/aml_card_sd.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/am_eth_reg.h>
#include <asm/arch/uart.h>
#include <asm/arch/clock.h>
#include <asm/arch/nand.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/mtd.h>
#include <share_kernel.h>

/**
 * Routine: board_init
 * Description: Early hardware init.
 */
 

static struct aml_nand_platform aml_nand_platform[] = {
#ifdef CONFIG_AMLROM_NANDBOOT
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
		.chip_enable_pad = (AML_NAND_CE0 | (AML_NAND_CE1 << 4) | (AML_NAND_CE2 << 8) | (AML_NAND_CE3 << 12)),
		.ready_busy_pad = (AML_NAND_CE0 | (AML_NAND_CE0 << 4) | (AML_NAND_CE1 << 8) | (AML_NAND_CE1 << 12)),
		.platform_nand_data = {
			.chip =  {
				.nr_chips = 4,
				.options = (NAND_TIMING_MODE5 | NAND_ECC_BCH16_MODE),
			},
    	},
		.T_REA = 20,
		.T_RHOH = 15,
	}
};

struct aml_nand_device aml_nand_mid_device = {
	.aml_nand_platform = aml_nand_platform,
	.dev_num = ARRAY_SIZE(aml_nand_platform),
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
          
  //  	eth_clk_set(ETH_CLKSRC_SYS_D3,900*CLK_1M/3,50*CLK_1M);
	//eth_clk_set(ETH_CLKSRC_SYS_D3,get_cpu_clk()*2/3,50*CLK_1M);
	eth_clk_set(ETH_CLKSRC_APLL_CLK,400*CLK_1M,50*CLK_1M);
	///GPIOD15-24 for 8626M;
	///GPIOD12	nRst;
	///GPIOD13    n_int;
	eth_set_pinmux(ETH_BANK2_GPIOD15_D23,ETH_CLK_OUT_GPIOD24_REG5_1,0);
	/*disalbe*/
	//reset:LCD_G5
	writel(readl(ETH_PLL_CNTL) & ~(1 << 0), ETH_PLL_CNTL); // Disable the Ethernet clocks     
	// ---------------------------------------------
	// Test 50Mhz Input Divide by 2
	// ---------------------------------------------
	// Select divide by 2
	writel(readl(ETH_PLL_CNTL) | (0 << 3), ETH_PLL_CNTL); // desc endianess "same order"   
	writel(readl(ETH_PLL_CNTL) | (0 << 2), ETH_PLL_CNTL); // data endianess "little"    
	writel(readl(ETH_PLL_CNTL) | (1 << 1), ETH_PLL_CNTL); // divide by 2 for 100M     
	writel(readl(ETH_PLL_CNTL) | (1 << 0), ETH_PLL_CNTL);  // enable Ethernet clocks   
	udelay(100);
	/*reset*/
	CLEAR_CBUS_REG_MASK(PREG_GGPIO_EN_N,1<<12);
	CLEAR_CBUS_REG_MASK(PREG_GGPIO_O,1<<12);
	udelay(100);
	SET_CBUS_REG_MASK(PREG_GGPIO_O,1<<12);
	udelay(10);	//waiting reset end;
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
	setbits_le32(P_PREG_CGPIO_EN_N,1<<25);

    return cpu_sdio_init(port);
}
static int  sdio_detect(unsigned port)
{
	return (readl(P_PREG_CGPIO_I)&(1<<25))?1:0;
}
static void sdio_pwr_on(unsigned port)
{
	clrbits_le32(P_PREG_CGPIO_O,(1<<26));
	clrbits_le32(P_PREG_CGPIO_EN_N,(1<<26));
    /// @todo NOT FINISH
}
static void sdio_pwr_off(unsigned port)
{
    setbits_le32(P_PREG_CGPIO_O,(1<<26));
	clrbits_le32(P_PREG_CGPIO_EN_N,(1<<26));//test_n
	
	/// @todo NOT FINISH
}

static void sdio_pwr_prepare(unsigned port)
{
    cpu_sdio_pwr_prepare(port);
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
	//mmc->host_caps = MMC_MODE_4BIT;
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
	return 0;
}
int serial_set_pin_port(int port)
{
    /*UARTA pinmux*/
    if(port==UART_A)
        setbits_le32(P_PERIPHS_PIN_MUX_2,((1<<15)|(1<<11)));
    
    /*UARTB pinmux*/
    if(port==UART_B){
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, ((1 << 15) | (1 << 19))); //Audio Out
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_8, ((1 << 8) | (1 << 8))); //Audio In
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, ((1 << 16) | (1 << 20))); //I2C Master B
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, ((1 << 17) | (1 << 21))); //I2C Slave
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, ((1 << 25) | (1 << 26))); //PWM
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_0, (1 << 29)); //JTAG enable
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_0, (1 << 30)); //JTAG disable
//        *(volatile unsigned*)0xc11084e4 = 0x3196;//0x83196;//UART_B
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_1,((1 << 18) | (1 << 22)));
    }
    return 0;
}

#ifdef CONFIG_SWITCH_BOOT_MODE

inline int get_key()
{
//    int adc_val = get_adc_sample(4);
//    printf("get_adc_sample(4): 0x%x\n", adc_val);
//    //return(((adc_val >= 0x320) && (adc_val < 0x370)) ? 1 : 0);
//    return((adc_val < 0x60) ? 1 : 0);
}

int switch_boot_mode(void)
{
    unsigned long hold_time = 50000, polling_time = 10000, tmp;

    printf("ENTER U-BOOT SWITCH MODE !!!!\n");

//    saradc_enable();

    //added by pengtj for added fool idle
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


    if (reboot_mode == AMLOGIC_FACTORY_RESET_REBOOT )
    {
    	  printf("Recovery Start from upgrade apk...\n");
        run_command ("run recoveryboot", 0);
        hang();

    }
    else if(hold_time > 0)
    {
        printf("Normal Start...\n");
        return  1;
    }
    else
    {
       	printf("Recovery Start...\n");
        run_command ("run recoveryboot", 0);
        hang();
    }
}
#endif


