/* platform dirver header */
/*
 * (C) Copyright 2010 Amlogic, Inc
 *
 * Victor Wan, victor.wan@amlogic.com, 
 * 2010-03-24 @ Shanghai
 *
 */
 #include "platform.h"

/*
   cfg = 0 : EXT clock
   cfg = 1 : INT clock
  */
static void set_usb_phy_config(int cfg)
{

    int divider =0;
    int clk_sel = 0;
    int i;
    int time_dly = 500;

	// ------------------------------------------------------------
	//  CLK_SEL: These bits select the source for the 12Mhz: 
	// 0 = Oscillator pad input (when a 24Mhz XTAL is used), 
	// 1 = Oscillator pad input divided by 2 (when a 24mhz XTAL is used),
	// 2 = sys_pll_clk 
	// 3 = misc_pll_clk
	// 4 = ddr_pll_clk
	// 5 = audio_pll_clk
	// 6 = vid_pll_clk
	CLEAR_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_CLK_SEL);
	CLEAR_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_CLK_DIV);
	if(cfg == EXT_CLOCK)
	{/* crystal == 24M */
		clk_sel = 1; // 12M, Phy default setting is 12Mhz
		divider = 0; // 24M /(0 + 1) = 12M
	}
	else
	{
		clk_sel = 3; // misc_pll = 600M
		divider = 49; // (600 / (49 + 1)) = 12M
	}
	SET_PERIPHS_REG_BITS(PREI_USB_PHY_REG, (divider << 24));
	SET_PERIPHS_REG_BITS(PREI_USB_PHY_REG, (clk_sel<< 5 ));

	/* Force ID to 1 (device mode) */
#ifndef __USE_PORT_B
	SET_PERIPHS_REG_BITS(PREI_USB_PHY_B_REG4,PREI_USB_PHY_B_REG4_IDDIG_OVR);
	SET_PERIPHS_REG_BITS(PREI_USB_PHY_B_REG4,PREI_USB_PHY_B_REG4_IDDIG_VAL);
#else
	SET_PERIPHS_REG_BITS(PREI_USB_PHY_A_REG3,PREI_USB_PHY_A_REG3_IDDIG_OVR);
	SET_PERIPHS_REG_BITS(PREI_USB_PHY_A_REG3,PREI_USB_PHY_A_REG3_IDDIG_VAL);
#endif

	// Open clock gate, to enable CLOCK to usb phy 
	SET_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_CLK_GATE);
	i=0;
	while(i++<time_dly){};

	
#ifndef __USE_PORT_B
    /*  Reset USB PHY A  */
    SET_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_A_AHB_RSET);
    i=0;
    while(i++<time_dly){};  
    CLEAR_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_A_AHB_RSET);
    i=0;
    while(i++<time_dly){};
    SET_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_A_CLK_RSET);
    i=0;
    while(i++<time_dly){};      
    CLEAR_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_A_CLK_RSET);
    i=0;
    while(i++<time_dly){};
    SET_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_A_PLL_RSET);
    i=0;
    while(i++<time_dly){};
    CLEAR_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_A_PLL_RSET);
    i=0;
    while(i++<time_dly){};

    // ------------------------------------------------------------ 
    // Reset the PHY A by setting POR high for 10uS.
    SET_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_A_POR);
    i=0;
    while(i++<time_dly){};
    // Set POR to the PHY high

    CLEAR_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_A_POR);
    i=0;
    while(i++<time_dly){};

#else
    /* Reset USB PHY B */
    SET_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_B_AHB_RSET);
    i=0;
    while(i++<time_dly){};
    CLEAR_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_B_AHB_RSET);
    i=0;
    while(i++<time_dly){};
    SET_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_B_CLK_RSET);
    i=0;
    while(i++<time_dly){};
    CLEAR_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_B_CLK_RSET);
    i=0;
    while(i++<time_dly){};
    SET_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_B_PLL_RSET);
    i=0;
    while(i++<time_dly){};
    CLEAR_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_B_PLL_RSET);
    i=0;
    while(i++<time_dly){};

    // ------------------------------------------------------------ 
    // Reset the PHY B by setting POR high for 10uS.
    SET_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_B_POR);
    i=0;
    while(i++<time_dly){};

    // Set POR to the PHY high
    CLEAR_PERIPHS_REG_BITS(PREI_USB_PHY_REG, PREI_USB_PHY_B_POR);
    i=0;
    while(i++<time_dly){};
	
#endif

}

#if 0
int chip_watchdog(void)
{
	watchdog_clear();
	return 0;
};
#endif

#if 1
void usb_memcpy(char * dst,char * src,int len)
{
	while(len--)
	{
		*(unsigned char*)dst = *(unsigned char*)src;
		(unsigned char*)dst++;
		(unsigned char*)src++;
	}
}
void usb_memcpy_32bits(int *dst,int *src,int len)
{
	while(len--)
	{
		*dst = *src;
		dst++;
		src++;
	}
}
#endif

