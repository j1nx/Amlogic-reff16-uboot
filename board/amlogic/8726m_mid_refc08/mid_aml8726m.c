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
				.options = (NAND_TIMING_MODE5 | NAND_ECC_BCH8_MODE),
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
	//setbits_le32(P_PREG_CGPIO_EN_N,1<<5);
	setbits_le32(P_PREG_DGPIO_EN_N,1<<11);//GPIOD13

    return cpu_sdio_init(port);
}
static int  sdio_detect(unsigned port)
{
	//return (readl(P_PREG_CGPIO_I)&(1<<5))?1:0;
	return (readl(P_PREG_DGPIO_I)&(1<<11))?1:0;//GPIOD13
}
static void sdio_pwr_prepare(unsigned port)
{
    /// @todo NOT FINISH
	///do nothing here
}
static void sdio_pwr_on(unsigned port)
{
//	clrbits_le32(P_PREG_CGPIO_O,(1<<5));
//	clrbits_le32(P_PREG_CGPIO_EN_N,(1<<5));//test_n
	clrbits_le32(P_PREG_DGPIO_O,(1<<11));
	clrbits_le32(P_PREG_DGPIO_EN_N,(1<<11));//GPIOD13
    /// @todo NOT FINISH
}
static void sdio_pwr_off(unsigned port)
{
//	setbits_le32(P_PREG_CGPIO_O,(1<<5));
//	clrbits_le32(P_PREG_CGPIO_EN_N,(1<<5));//test_n
	setbits_le32(P_PREG_DGPIO_O,(1<<11));
	clrbits_le32(P_PREG_DGPIO_EN_N,(1<<11));//GPIOD13

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
//    int ret = 0;
//    run_command ("osd open 24 1280 720 1", 0);
//    run_command ("tv open 720P", 0);
//    run_command ("nand read ${aml_logo_name} ${loadaddr} 0 ${aml_logo_size}", 0);
//	ret = run_command ("bmp display ${loadlogoaddr} 0 0", 0);
//    return ret;
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
  ret = run_command ("nand read ${aml_logo_name} ${loadlogoaddr} 0 ${aml_logo_size}", 0);
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
		run_command ("nand read ${aml_logo_name} ${loadlogoaddr} 0 ${aml_logo_size}", 0);
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
			
			run_command ("nand read ${aml_logo_name} ${loadlogoaddr} 0 ${aml_logo_size}", 0);
		    run_command ("osd open 24 1280 720 1", 0);
		    run_command ("tv open 720P", 0);
		}
		else if(!strcmp(p_outputmode,"1080p"))
		{			
			printf("Use 1080p mode!\n");			
			run_command ("nand read ${aml_logo_name} ${loadlogoaddr} 0 ${aml_logo_size}", 0);
		    run_command ("osd open 24 1920 1080 1", 0);
		    run_command ("tv open 1080P", 0);
		}
		else//default 480p
		{
			printf("Use default 480p mode!\n");			
			run_command ("nand read ${aml_logo_name} ${loadlogoaddr} 0 ${aml_logo_size}", 0);
		    run_command ("osd open 24 720 480 1", 0);
		    run_command ("tv open 480P", 0);		
		}		
		
		ret = run_command ("bmp display ${loadlogoaddr} 0 0", 0);

		return ret;
	}	
	else
	{
		printf("No outputmode env parameter. Use default 480p mode!\n");
		run_command ("nand read ${aml_logo_name} ${loadlogoaddr} 0 ${aml_logo_size}", 0);
	    run_command ("osd open 24 720 480 1", 0);
	    run_command ("tv open 480P", 0);

		ret = run_command ("bmp display ${loadlogoaddr} 0 0", 0);

		return ret;
	}
#endif
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
    unsigned long hold_time = 3000, polling_time = 10000, tmp;
    unsigned long upgrade_step;

    upgrade_step = simple_strtoul (getenv ("upgrade_step"), NULL, 16);
    printf("upgrade_step = %d    !!!!!!!!!!!!!!!!!!\n", upgrade_step);

#ifdef CONFIG_SARADC
    saradc_enable();
#endif

#ifdef ENABLE_FONT_RESOURCE
		RegisterFont(DEFAULT_FONT);
#endif
		//aml_autoscript();
		//logo_display();
		//return 1;

    if(upgrade_step == 3)
    {
		switch(reboot_mode)
        {
            case AMLOGIC_NORMAL_BOOT:
            {
                printf("AMLOGIC_NORMAL_BOOT...\n");
                printf("reboot_mode is %d...\n", reboot_mode);
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
				run_command ("bootm", 0);;
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
                printf("Default.... \n");
                power_hold();
                logo_display();
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
            //return  -1;
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

	powerkey_hold(0);
    if(!powerkey_hold(hold_time))
    {
    	//aml_autoscript();
    	if(upgrade_step == 0)
    	{
        	logo_display();
	        run_command ("set upgrade_step 3", 0);
	        run_command ("save", 0);
    	}
    	printf("Normal Start...\n");	
    	return 1;
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
extern void i2c_init(struct i2c_board_info *i2c_info, unsigned int len);
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
	i2c_init(aml_i2c_info, ARRAY_SIZE(aml_i2c_info));
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
