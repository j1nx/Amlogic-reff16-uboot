
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
#include <asm/arch/nand.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/mtd.h>
#include <asm/arch/osd.h>
#include <asm/arch/osd_hw.h>
#include <asm/arch-m1/tv_out.h>
#include <asm/arch-m3/usb.h>


#include <asm/arch/i2c.h>
#include <aml_i2c.h>

/**
 * Routine: board_init
 * Description: Early hardware init.
 */
//just compile pass 
static struct aml_nand_platform aml_nand_mid_platform[] = {
#ifdef CONFIG_NAND_BOOT
   {
        .name = NAND_BOOT_NAME,
        .chip_enable_pad = AML_NAND_CE0,
        .ready_busy_pad = AML_NAND_CE0,
        .platform_nand_data = {
            .chip =  {
                .nr_chips = 1,
				.options = (NAND_TIMING_MODE5 | NAND_ECC_BCH60_1K_MODE),
            },
        },
        .T_REA = 20,
        .T_RHOH = 15,
    },
#endif
    {
		.name = NAND_MULTI_NAME,
		.chip_enable_pad = (AML_NAND_CE0 | (AML_NAND_CE1 << 4) | (AML_NAND_CE2 << 8) | (AML_NAND_CE3 << 12)),
		.ready_busy_pad = (AML_NAND_CE0 | (AML_NAND_CE0 << 4) | (AML_NAND_CE1 << 8) | (AML_NAND_CE1 << 12)),
        .platform_nand_data = {
            .chip =  {
				.nr_chips = 4,
				.options = (NAND_TIMING_MODE5 | NAND_ECC_BCH60_1K_MODE | NAND_TWO_PLANE_MODE),
            },
        },
        .T_REA = 20,
        .T_RHOH = 15,
        .ran_mode = 1,
    }
};

struct aml_nand_device aml_nand_mid_device = {
    .aml_nand_platform = aml_nand_mid_platform,
    .dev_num = ARRAY_SIZE(aml_nand_mid_platform),
};

#ifdef CONFIG_USB_DWC_OTG_HCD
static void set_usb_vbus_power(char is_power_on)
{
    /*GPIOD4*/
	#define USB_B_POW_GPIO         GPIOD_bank_bit0_9(4)
	#define USB_B_POW_GPIO_BIT     GPIOD_bit_bit0_9(4)
	#define USB_B_POW_GPIO_BIT_ON   1

  printk(KERN_INFO "set usb b port power on (board gpio %d)!\n",USB_B_POW_GPIO_BIT);
  set_gpio_mode(USB_B_POW_GPIO, USB_B_POW_GPIO_BIT, GPIO_OUTPUT_MODE);
  set_gpio_val(USB_B_POW_GPIO, USB_B_POW_GPIO_BIT, USB_B_POW_GPIO_BIT_ON);
}

struct amlogic_usb_config usb_config_m3={
	USB_PHY_CLOCK_SEL_XTAL_DIV2,
	0, // no use if clock sel == xtal or xtal/2
	CONFIG_M3_USBPORT_BASE,
	USB_ID_MODE_SW_HOST,
	set_usb_vbus_power, //set_vbus_power
};
#endif

int board_init(void)
{
    /*
    @todo implement this function
    */
	DECLARE_GLOBAL_DATA_PTR;
	gd->bd->bi_arch_number=2958;
	gd->bd->bi_boot_params=BOOT_PARAMS_OFFSET;

#ifdef CONFIG_CMD_KGDB

	kgdb_init();
#endif

#ifdef CONFIG_USB_DWC_OTG_HCD
	board_usb_init(&usb_config_m3);
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
	unsigned v;
	
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,(3<<17));//reg6[17/18]=0
#ifdef CONFIG_NET_CLK_EXTERNAL
	//rmii 50 in
	//set clock
    eth_clk_set_invert(7,50*CLK_1M,50*CLK_1M);
  eth_set_pinmux(ETH_BANK0_GPIOY1_Y9, ETH_CLK_IN_GPIOY0_REG6_18, 0);
#else
	/* Use Misc PLL for the source of ethernet */
	eth_clk_set(ETH_CLKSRC_MISC_CLK, get_misc_pll_clk(), (50 * CLK_1M));
	/* Use Internal clock output from GPIOY0*/
	eth_set_pinmux(ETH_BANK0_GPIOY1_Y9, ETH_CLK_OUT_GPIOY0_REG6_17, 0);
#endif //CONFIG_NET_CLK_EXTERNAL
	
	/*disalbe*/
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

	/* reset phy with GPIOD_7*/
	set_gpio_mode(GPIOD_bank_bit0_9(7), GPIOD_bit_bit0_9(7), GPIO_OUTPUT_MODE);
	set_gpio_val(GPIOD_bank_bit0_9(7), GPIOD_bit_bit0_9(7), 0);
	udelay(2000);    
	set_gpio_val(GPIOD_bank_bit0_9(7), GPIOD_bit_bit0_9(7), 1);
	udelay(2000);	//waiting reset end;
	
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
    //todo add card detect as input 	
    setbits_le32(P_PREG_PAD_GPIO5_EN_N,1<<29);//CARD_6

    return cpu_sdio_init(port);
}
static int  sdio_detect(unsigned port)
{
    setbits_le32(P_PREG_PAD_GPIO5_EN_N,1<<29);//CARD_6
    return readl(P_PREG_PAD_GPIO5_I)&(1<<29)?1:0;
}
static void sdio_pwr_prepare(unsigned port)
{
    /// @todo NOT FINISH
	///do nothing here
}
static void sdio_pwr_on(unsigned port)
{
    //set CARD_8 to low
    clrbits_le32(P_PREG_PAD_GPIO5_O,(1<<31));
    clrbits_le32(P_PREG_PAD_GPIO5_EN_N,(1<<31));
}
static void sdio_pwr_off(unsigned port)
{
    //set CARD_8 to high
    setbits_le32(P_PREG_PAD_GPIO5_O,(1<<31));
    clrbits_le32(P_PREG_PAD_GPIO5_EN_N,(1<<31));
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
	mmc->bus_width = 1;
	mmc->clock = 300000;
	mmc->f_min = 200000;
	mmc->f_max = 50000000;
	mmc_register(mmc);
}
int board_mmc_init(bd_t	*bis)
{
//	board_mmc_register(SDIO_PORT_A);
	board_mmc_register(SDIO_PORT_B);
//	board_mmc_register(SDIO_PORT_C);
//	board_mmc_register(SDIO_PORT_B1);
	return 0;
}

extern int osd_width;
extern int osd_height;

#include <bmp_layout.h>
//Not full test down scale function
extern bmp_image_t *gunzip_bmp(unsigned long addr, unsigned long *lenp);
void bmp_scale(ulong src_addr, ulong dst_addr, __u32 new_width, __u32 new_height)
{
	printf("Begin scale logo...\n");
	bmp_image_t *bmp = (bmp_image_t *)src_addr;
	bmp_image_t *bmp_dst = (bmp_image_t *)dst_addr;	
	unsigned long len;

	char *pBuf = (char*)bmp+bmp->header.data_offset;

	if (!((bmp->header.signature[0]=='B') &&
		      (bmp->header.signature[1]=='M')))
			bmp = gunzip_bmp(src_addr, &len);

	if (!bmp) {
		printf("There is no valid bmp file at the given address\n");
		return;
	}	      

	memcpy(bmp_dst, bmp, sizeof(bmp_image_t));
	bmp_dst->header.width=new_width;
	bmp_dst->header.height=new_height;
	char *pBuf_dst = (char*)bmp_dst+bmp_dst->header.data_offset;

#if 1//Fast scale
	int   nWidth   ,   nHeight	 ,	 nNewWidth	 ,	 nNewHeight   ,   nNewWidthBit	 ,	 nWidthBit; 
	float m_xscale,m_yscale;
	int i,j,x,y,oldoffset;	
	char *pNewTmp = NULL; 
	
	m_xscale = (float)bmp_dst->header.width/(float)bmp->header.width;
	m_yscale = (float)bmp_dst->header.height/(float)bmp->header.height;
	nWidth = bmp->header.width;
	nHeight = bmp->header.height;
	nNewHeight = bmp_dst->header.width;
	nNewWidth =	bmp_dst->header.width;
	nNewWidthBit = ( 4 - nNewWidth * 3 % 4 )%4 + nNewWidth * 3;

	for( i=0; i<nNewHeight; i++ )
	{ 
		pNewTmp = pBuf_dst + nNewWidthBit * i; 
		for( j=0; j<nNewWidth * 3; j += 3 ) 
		{ 
			x = (int) (j/m_xscale); 
			y = (int) (i/m_yscale); 
			oldoffset = (y*nWidth*3 + x) - (y*nWidth*3 + x)%3; //correct positon in 3 byte mode
			memcpy(pNewTmp+j, pBuf + oldoffset, 3);
		} 
	} 
#else
//High quality
	int sw = bmp->header.width - 1, sh = bmp->header.height- 1, dw = bmp_dst->header.width - 1, dh = bmp_dst->header.height- 1;
    int B, N, x, y;
    int nPixelSize = bmp->header.bit_count/8;
    char * pLinePrev, *pLineNext;
	char *pSrc,*pDest;    
    char * pA, *pB, *pC, *pD;
    unsigned int i,j,k;
    for ( i = 0; i <= dh; ++i )
    {
        pDest = ( char * )(pBuf_dst+bmp_dst->header.width*i*nPixelSize);
        y = i * sh / dh;
        N = dh - i * sh % dh;
        pLinePrev = ( char * )(pBuf+bmp->header.width*y*nPixelSize);
        y++;
        pLineNext = ( N == dh ) ? pLinePrev : ( char * )(pBuf+bmp->header.width*y*nPixelSize);
        for ( j = 0; j <= dw; ++j )
        {
            x = j * sw / dw * nPixelSize;
            B = dw - j * sw % dw;
            pA = pLinePrev + x;
            pB = pA + nPixelSize;
            pC = pLineNext + x;
            pD = pC + nPixelSize;
            if ( B == dw )
            {
                pB = pA;
                pD = pC;
            }
            for ( k = 0; k < nPixelSize; ++k )
                *pDest++ = ( char )( int )(
                    ( B * N * ( *pA++ - *pB - *pC + *pD ) + dw * N * *pB++
                    + dh * B * *pC++ + ( dw * dh - dh * B - dw * N ) * *pD++
                    + dw * dh / 2 ) / (double)( dw * dh )
                );
        }
    }

#endif
	printf("End scale logo.\n");
}

int logo_display(void)
{
    int ret = 0;
    char buf[32];
	
    printf("logo_display: \n");

    run_command ("osd enable", 0);
    sprintf(buf, "osd open 24 %d %d 1", osd_width, osd_height);
    run_command (buf, 0);
    run_command ("osd fill 0", 0);

    run_command ("tv open ${outputmode}", 0);
    run_command ("nand read ${aml_logo_name} 82000000 0 ${aml_logo_size}", 0);

    bmp_scale(0x82000000, 0x84100000, osd_width, osd_height);
	 
    ret = run_command ("bmp display 84100000", 0);

#ifdef M3_DSP_VSYNC_INTERRUPT
		ret = run_command ("tvdsp check;tvdsp stop", 0);
#endif

    return ret;

}

//Power Function
inline void power_up(void)
{
    //VDDIO_EN (GPIOAO_2) is high
    clrbits_le32(P_AO_GPIO_O_EN_N, 1<<2);
    setbits_le32(P_AO_GPIO_O_EN_N, 1<<18);
    if(!strcmp(getenv("boardver"), "v0"))
        //VCC5V_EN (GPIOAO_6) is high
        setbits_le32(P_AO_GPIO_O_EN_N, 1<<22);
    else
        //VCC5V_EN (GPIOAO_6) is low
        clrbits_le32(P_AO_GPIO_O_EN_N, 1<<22);
    clrbits_le32(P_AO_GPIO_O_EN_N, 1<<6);
    //POWER LED (GPIOAO_10) is low
    clrbits_le32(P_AO_GPIO_O_EN_N, (1<<26)|(1<<10));
    //NET_LED (GPIOD_1) is high
    set_gpio_val(GPIOD_bank_bit0_9(1), GPIOD_bit_bit0_9(1), 1);
    set_gpio_mode(GPIOD_bank_bit0_9(1), GPIOD_bit_bit0_9(1), GPIO_OUTPUT_MODE);
    //Ampak POWER_LED (GPIOD_0) is high
    set_gpio_val(GPIOD_bank_bit0_9(0), GPIOD_bit_bit0_9(0), 1);
    set_gpio_mode(GPIOD_bank_bit0_9(0), GPIOD_bit_bit0_9(0), GPIO_OUTPUT_MODE);
}
//end of Power Function

int scan_mode = SCAN_MODE_PROGRESSIVE;
int osd_width=1280;
int osd_height=720;


static int get_tv_mode(char *mode_name)
{
	if((strcmp(mode_name, "1080P")==0) || (strcmp(mode_name, "1080p")==0))
	{
       scan_mode = 1; // SCAN_MODE_PROGRESSIVE;
		return TVOUT_1080P;
	}
	if((strcmp(mode_name, "1080I")==0) || (strcmp(mode_name, "1080i")==0))
	{
       scan_mode = 0; // SCAN_MODE_INTERLACE;
		return TVOUT_1080I;
	}
	if((strcmp(mode_name, "720P")==0) || (strcmp(mode_name, "720p")==0))
	{
       scan_mode = 1; // SCAN_MODE_PROGRESSIVE;
		return TVOUT_720P;
	}
	if((strcmp(mode_name, "576P")==0) || (strcmp(mode_name, "576p")==0))
	{
       scan_mode = 1; // SCAN_MODE_PROGRESSIVE;
		return TVOUT_576P;
	}
	if((strcmp(mode_name, "480P")==0) || (strcmp(mode_name, "480p")==0))
	{
       scan_mode = 1; // SCAN_MODE_PROGRESSIVE;
		return TVOUT_480P;
	}
	if((strcmp(mode_name, "576I")==0) || (strcmp(mode_name, "576i")==0))
	{
       scan_mode = 0; // SCAN_MODE_INTERLACE;
		return TVOUT_576I;
	}
	if((strcmp(mode_name, "480I")==0) || (strcmp(mode_name, "480i")==0))
	{
       scan_mode = 0; // SCAN_MODE_INTERLACE;
		return TVOUT_480I;
	}
	return TVOUT_MAX;
}

// for mbox, output mode could be changed by user, so osd size is determinate,
// we need to get osd size from env.
void get_osd_size()
{
    char buf[32];
	char tvmode[16];
	char * str;
	
	str = getenv("outputmode");
	sprintf(tvmode, "%s", str);

	tv_out_get_info(get_tv_mode(tvmode), &osd_width, &osd_height);
}

#ifdef CONFIG_SWITCH_BOOT_MODE

extern char version_string[];

static int detect_power_key(void)
{
    //GPIOAO3
    setbits_le32(P_AO_GPIO_O_EN_N,(1<<3));
    return !(readl(P_AO_GPIO_I)&(1<<3));
}

int switch_boot_mode(void)
{
    char *version=getenv ("version");
    power_up();
    get_osd_size();
    logo_display();

    if(detect_power_key()){
        reboot_mode = AMLOGIC_FACTORY_RESET_REBOOT;
#ifdef CONFIG_AML_AUTOSCRIPT
        aml_autoscript();
#endif
        }

    switch(reboot_mode){
        case AMLOGIC_FACTORY_RESET_REBOOT:
            printf("AMLOGIC_FACTORY_RESET_REBOOT...\n");
            run_command ("run recoveryboot", 0);
            break;
        case AMLOGIC_NORMAL_BOOT:
        default:
            printf("AMLOGIC_NORMAL_BOOT...\n");
            break;
        }
    if(strcmp(version,version_string)!=0){
        char cmd[256]={0};
        sprintf(cmd,"set version %s",version_string);
        run_command (cmd, 0);
        run_command ("save", 0);
        }
    return 1;
}
#endif

