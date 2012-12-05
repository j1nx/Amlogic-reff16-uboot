
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
#include <asm/arch/usb.h>
#include <asm/arch/osd.h>

/**
 * Routine: board_init
 * Description: Early hardware init.
 */
 
//#define ETHERNET_EXTERNEL_CLOCK

static struct aml_nand_platform aml_nand_platform[] = {
#ifdef CONFIG_AMLROM_NANDBOOT
	{
		.name = NAND_BOOT_NAME,
		.chip_enable_pad = AML_NAND_CE0,
		.ready_busy_pad = AML_NAND_CE0,
		.platform_nand_data = {
			.chip =  {
				.nr_chips = 1,
				.options = (NAND_TIMING_MODE5 | NAND_ECC_BCH16_MODE| NAND_TWO_PLANE_MODE),
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
				.options = (NAND_TIMING_MODE5 | NAND_ECC_BCH16_MODE| NAND_TWO_PLANE_MODE),
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

#ifdef CONFIG_USB_DWC_OTG_HCD
static void set_usb_a_vbus_power(char is_power_on)
{
/*
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
*/
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
	#ifdef ETHERNET_EXTERNEL_CLOCK
        eth_clk_set(ETH_CLKSRC_EXTERN_PAD_CLK,50*CLK_1M,50*CLK_1M);     //use externel clock
    #else
        eth_clk_set(ETH_CLKSRC_APLL_CLK,400*CLK_1M,50*CLK_1M);       //use internel clock
    #endif
	///GPIOD15-24 for 8626M;
	///GPIOD12	nRst;
	///GPIOD13    n_int;
#ifdef ETHERNET_EXTERNEL_CLOCK
	eth_set_pinmux(ETH_BANK2_GPIOD15_D23,ETH_CLK_IN_GPIOD24_REG5_0,0);  //use externel clock
#else
	eth_set_pinmux(ETH_BANK2_GPIOD15_D23,ETH_CLK_OUT_GPIOD24_REG5_1,0); //use internel clock
#endif
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
#endif


#if 0//High quality
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
  int i;
	char *p_outputmode=NULL;
	char *screen_width, *screen_height;
	char *output_width ,*output_height; 
	char *default_w, *default_h;
	int need_set_screen=0;
	char width[10],height[10];
  char *outputmode[7]={"480i","480p","576i","576p","720p","1080i","1080p"};
#if 1 //Use scale
//  ret = run_command ("nand read ${loadlogoaddr} ${aml_logo_start} ${aml_logo_size}", 0);
  ret = run_command ("nand read ${aml_logo_name} ${loadlogoaddr} 0 ${aml_logo_size}",0);
  if(((screen_width = getenv("screen_width")) == NULL) || ((screen_height = getenv("screen_height")) == NULL))
    need_set_screen = 1;	
  if((p_outputmode = getenv ("outputmode")) != NULL)
 	{
    for(i=0 ;i<7;i++)
    {
      if(!strcmp(p_outputmode,outputmode[i]))
      {
      	printf("i =%d\n",i);
      	printf("outputmode = %s\n",p_outputmode);
      	break;
      }
    }
    switch(i)
    {
      case 0:
      	output_width = getenv("480ioutputwidth");
      	output_height = getenv("480ioutputheight");
      	default_w = "720";
      	default_h = "480";
      	bmp_scale(0x84100000, 0x84900000, 720, 480);
		    run_command ("tv open 480I", 0);
		    run_command ("osd open 24 720 480 1", 0);
        break;
      case 1:
      	output_width = getenv("480poutputwidth");
      	output_height = getenv("480poutputheight");
        default_w = "720";
      	default_h = "480";
      	bmp_scale(0x84100000, 0x84900000, 720, 480);
		    run_command ("osd open 24 720 480 1", 0);
		    run_command ("tv open 480P", 0);		
        break;
      case 2:
      	output_width = getenv("576ioutputwidth");
      	output_height = getenv("576ioutputheight");
      	default_w = "720";
      	default_h = "576";
      	bmp_scale(0x84100000, 0x84900000, 720, 576);
		    run_command ("tv open 576I", 0);	
		    run_command ("osd open 24 720 576 1", 0);
        break;
      case 3:
      	output_width = getenv("576poutputwidth");
      	output_height = getenv("576poutputheight");
      	default_w = "720";
      	default_h = "576";
      	bmp_scale(0x84100000, 0x84900000, 720, 576);
		    run_command ("osd open 24 720 576 1", 0);
		    run_command ("tv open 576P", 0);	
        break;
      case 4:
      	output_width = getenv("720poutputwidth");
      	output_height = getenv("720poutputheight");
      	default_w = "1280";
      	default_h = "720";
      	bmp_scale(0x84100000, 0x84900000, 1280, 720);
		    run_command ("osd open 24 1280 720 1", 0);
		    run_command ("tv open 720P", 0);
        break;
      case 5:
      	output_width = getenv("1080ioutputwidth");
      	output_height = getenv("1080ioutputheight");
      	default_w = "1920";
      	default_h = "1080";
      	bmp_scale(0x84100000, 0x84900000, 1920, 1080);
		    run_command ("tv open 1080I", 0);
		    run_command ("osd open 24 1920 1080 1", 0);
        break;
      case 6:
      	output_width = getenv("1080poutputwidth");
      	output_height = getenv("1080poutputheight");
      	default_w = "1920";
      	default_h = "1080";
      	bmp_scale(0x84100000, 0x84900000, 1920, 1080);
		    run_command ("osd open 24 1920 1080 1", 0);
		    run_command ("tv open 1080P", 0);
        break;
      default:
      	output_width = getenv("480poutputwidth");
      	output_height = getenv("480poutputheight");
      	bmp_scale(0x84100000, 0x84900000, 720, 480);
		    run_command ("osd open 24 720 480 1", 0);
		    run_command ("tv open 480P", 0);		
        break;    
       				
    }	
    ret = run_command ("bmp display 84900000 0 0", 0);
 	  if((output_width  != NULL) && (output_height != NULL))
	  {
	    printf("output_width =%s,output_height=%s\n",output_width,output_height);
	    if((need_set_screen) || ((strcmp(screen_width,output_width) || strcmp(screen_height,output_height))))
		  {
		    strcpy(width,output_width);
		    strcpy(height,output_height);
		    setenv("screen_width",width);
		    setenv("screen_height",height);
//		   printf("width =%s,height=%s\n",width,height);
		  }	
    }else
    {
        printf("not set screen size yet\n");
        setenv("screen_width",default_w);
		    setenv("screen_height",default_h);
    }				
  }else
  {
  	printf("No outputmode env parameter. Use default 480p mode!\n");
//		run_command ("nand read ${loadlogoaddr} ${aml_logo_start} ${aml_logo_size}", 0);
	    run_command ("nand read ${aml_logo_name} ${loadlogoaddr} 0 ${aml_logo_size}",0);
	    run_command ("osd open 24 720 480 1", 0);
	    run_command ("tv open 480P", 0);

		ret = run_command ("bmp display ${loadlogoaddr} 0 0", 0);		
  }
 
  return ret;  
  
#else//Use multi logo. Note: It need add i mode and 576 i p mode.
	if((p_outputmode = getenv ("outputmode")) != NULL)
	{
		
		if(!strcmp(p_outputmode,"720p"))
		{
			printf("Use 720p mode!\n");
			
//			run_command ("nand read ${loadlogoaddr} ${aml_logo_start_720p} ${aml_logo_size}", 0);
	      run_command ("nand read ${aml_logo_name} ${loadlogoaddr} 0 ${aml_logo_size}",0);
		    run_command ("osd open 24 1280 720 1", 0);
		    run_command ("tv open 720P", 0);
		}
		else if(!strcmp(p_outputmode,"1080p"))
		{			
			printf("Use 1080p mode!\n");			
			//run_command ("nand read ${loadlogoaddr} ${aml_logo_start_1080p} ${aml_logo_size}", 0);
		    run_command ("nand read ${aml_logo_name} ${loadlogoaddr} 0 ${aml_logo_size}",0);
		    run_command ("osd open 24 1920 1080 1", 0);
		    run_command ("tv open 1080P", 0);
		}
		else//default 480p
		{
			printf("Use default 480p mode!\n");			
			//run_command ("nand read ${loadlogoaddr} ${aml_logo_start} ${aml_logo_size}", 0);
		    run_command ("nand read ${aml_logo_name} ${loadlogoaddr} 0 ${aml_logo_size}",0);
		    run_command ("osd open 24 720 480 1", 0);
		    run_command ("tv open 480P", 0);		
		}		
		
		ret = run_command ("bmp display ${loadlogoaddr} 0 0", 0);

		return ret;
	}	
	else
	{
		printf("No outputmode env parameter. Use default 480p mode!\n");
		//run_command ("nand read ${loadlogoaddr} ${aml_logo_start} ${aml_logo_size}", 0);
	    run_command ("nand read ${aml_logo_name} ${loadlogoaddr} 0 ${aml_logo_size}",0);
	    run_command ("osd open 24 720 480 1", 0);
	    run_command ("tv open 480P", 0);

		ret = run_command ("bmp display ${loadlogoaddr} 0 0", 0);

		return ret;
	}
#endif
}

inline int get_key()
{
    int adc_val = get_adc_sample(4);
    printf("get_adc_sample(4): 0x%x\n", adc_val);
    //return(((adc_val >= 0x320) && (adc_val < 0x370)) ? 1 : 0);
    return((adc_val < 0x60) ? 1 : 0);
}

extern int aml_autoscript(void);

int switch_boot_mode(void)
{
    unsigned long hold_time = 50000, polling_time = 10000, tmp;

    printf("ENTER U-BOOT SWITCH MODE !!!!\n");

    saradc_enable();

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

	logo_display();
    if (reboot_mode == AMLOGIC_FACTORY_RESET_REBOOT )
    {
    	  printf("Recovery Start from upgrade apk...\n");
	#ifdef CONFIG_UPGRADE
		into_recovery();
	#else
        run_command ("run recoveryboot", 0);
	#endif
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
       	//aml_autoscript();
	#ifdef CONFIG_UPGRADE
		into_recovery();
	#else
        run_command ("run recoveryboot", 0);
	#endif
        hang();
    }
}
#endif


