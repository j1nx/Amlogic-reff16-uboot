/* Meson USB */
#include <common.h>
#include <asm/io.h>
#include <asm/cache.h>
#include <asm/arch/usb.h>

#ifdef CONFIG_USB_DWC_OTG_HCD
static amlogic_usb_config_t * g_usb_cfg;
static char * clock_src_name[]={
		"XTAL input",
		"XTAL input divided by 2",
		"other PLL",
		"DDR PLL",
		"dmod PLL 3",
		"dmod PLL 1",
		"sys PLL divid 3",
		"sys PLL divid 4"
};
 
int set_usb_phy_id_mode(amlogic_usb_config_t * usb_cfg)
{
    int i;
    int time_dly = 50000;
    unsigned int reg;
    unsigned int port;
    unsigned int mode;

		mode = usb_cfg->id_mode;
		port = usb_cfg->base_addr & USB_PHY_PORT_MSK;
    if(port == USB_PHY_PORT_A)
    {
        reg = PREI_USB_PHY_A_REG3;
    }
    else if(port == USB_PHY_PORT_B)
    {
        reg = PREI_USB_PHY_B_REG4;
    }
    else
    {
        printf("id_mode this usb port is not exist!\n");
        return -1;
    }

    CLEAR_CBUS_REG_MASK(reg, PREI_USB_PHY_MODE_MASK);
    i=0;
    while(i++<time_dly){};
    
    switch(mode)
    {
        case USB_ID_MODE_SW_HOST:
        		printf("usb id mode: SW_HOST\n");
            SET_CBUS_REG_MASK(reg, PREI_USB_PHY_MODE_SW_HOST);
            break;
			
        case USB_ID_MODE_SW_DEVICE:
        		printf("usb id mode: SW_DEVICE\n");
            SET_CBUS_REG_MASK(reg, PREI_USB_PHY_MODE_SW_SLAVE);
            break;
			
        case USB_ID_MODE_HARDWARE:
        default:
        		printf("usb id mode: HARDWARE\n");
            SET_CBUS_REG_MASK(reg, PREI_USB_PHY_MODE_HW);
            break;
    }
    
    i=0;
    while(i++<time_dly){};

    return 0;
}
void set_usb_phy_clock(amlogic_usb_config_t * usb_cfg)
{
    int clk_sel = usb_cfg->clk_selecter;

	// ------------------------------------------------------------
	//  CLK_SEL: These bits select the source for the 12Mhz: 
	// 0 = XTAL input (24, 25, 27Mhz)
	// 1 = XTAL input divided by 2
	// 2 = other PLL output
	// 3 = DDR pll clock (typically 400mhz)
	// 4 = demod 240Mhz PLL output
	CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_CLK_SEL);
	//clk_sel = 0; // 24M CLK 
	//clk_sel = 1; // 12M, Phy default setting is 12Mhz
	//clk_sel = 2; // other PLL, 540M
	//clk_sel = 3; // DDR, 369M
	//clk_sel = 4; // demod, 240M
	
	printf("usb clk_sel: %s\n",clock_src_name[clk_sel]);
	SET_CBUS_REG_MASK(PREI_USB_PHY_REG, (clk_sel<<5 ));

	CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG,PREI_USB_PHY_CLK_DIV);
	switch(clk_sel)
	{
		case USB_PHY_CLOCK_SEL_XTAL:
			//XTAL 24M, Divide by 2
			SET_CBUS_REG_MASK(PREI_USB_PHY_REG, (1 << 24));
			break;
			
		case USB_PHY_CLOCK_SEL_XTAL_DIV2:
			//XTAL 24M, Divide by 1
			SET_CBUS_REG_MASK(PREI_USB_PHY_REG, (0 << 24));
			break;
			
		case USB_PHY_CLOCK_SEL_OTHER_PLL:
			//Other PLL running at 540M (540/(44+1)=12)
			SET_CBUS_REG_MASK(PREI_USB_PHY_REG, (44 << 24));
			break;
			
		case USB_PHY_CLOCK_SEL_DDR_PLL:
			//DDR runing 396MHz (396/(32+1)=12)
			SET_CBUS_REG_MASK(PREI_USB_PHY_REG, (32 << 24));
			break;
			
		case USB_PHY_CLOCK_SEL_DEMOD_PLL:
			// demod 240M (240/(19+1) = 12)
			SET_CBUS_REG_MASK(PREI_USB_PHY_REG, (19 << 24));
			break;
	}

	// Open clock gate, to enable CLOCK to usb phy 
	SET_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_CLK_GATE);

}
void set_usb_phy_reset(amlogic_usb_config_t * usb_cfg,int is_on)
{
	  int i;
    int time_dly = 50000;
    unsigned int port = usb_cfg->base_addr & USB_PHY_PORT_MSK;;

		if(port == USB_PHY_PORT_A){   
      if(is_on){
      	    /*  Reset USB PHY A  */
          SET_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_A_AHB_RSET);
          i=0;
          while(i++<time_dly){};  
          CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_A_AHB_RSET);
          i=0;
          while(i++<time_dly){};
          SET_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_A_CLK_RSET);
          i=0;
          while(i++<time_dly){};      
          CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_A_CLK_RSET);
          i=0;
          while(i++<time_dly){};
          SET_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_A_PLL_RSET);
          i=0;
          while(i++<time_dly){};
          CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_A_PLL_RSET);
          i=0;
          while(i++<time_dly){};
  		}
      // ------------------------------------------------------------ 
      // Reset the PHY A by setting POR high for 10uS.
      SET_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_A_POR);
      i=0;
      while(i++<time_dly){};
      // Set POR to the PHY high
      if(is_on){
        	CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_A_POR);
        	i=0;
        	while(i++<time_dly){};
    	}  
		}

		if(port == USB_PHY_PORT_B){		    
    		if(is_on){
            /* Reset USB PHY B */
            SET_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_B_AHB_RSET);
            i=0;
            while(i++<time_dly){};
            CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_B_AHB_RSET);
            i=0;
            while(i++<time_dly){};
            SET_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_B_CLK_RSET);
            i=0;
            while(i++<time_dly){};
            CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_B_CLK_RSET);
            i=0;
            while(i++<time_dly){};
            SET_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_B_PLL_RSET);
            i=0;
            while(i++<time_dly){};
            CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_B_PLL_RSET);
            i=0;
            while(i++<time_dly){};
    		}
        // ------------------------------------------------------------ 
        // Reset the PHY B by setting POR high for 10uS.
        SET_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_B_POR);
        i=0;
        while(i++<time_dly){};
    
        // Set POR to the PHY high
        if(is_on){
        	CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_B_POR);
        	i=0;
        	while(i++<time_dly){};
        }
  	}
}
amlogic_usb_config_t * board_usb_start(void)
{
	if(!g_usb_cfg)
		return 0;

	set_usb_phy_id_mode(g_usb_cfg);
	set_usb_phy_clock(g_usb_cfg);
	set_usb_phy_reset(g_usb_cfg,1);//on
	
	return g_usb_cfg;
}

int board_usb_stop(void)
{
	printf("board_usb_stop!\n");
	set_usb_phy_reset(g_usb_cfg,0);//off

	return 0;
}
void board_usb_init(amlogic_usb_config_t * usb_cfg)
{
	g_usb_cfg = usb_cfg;
}
#endif