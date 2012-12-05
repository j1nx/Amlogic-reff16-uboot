
/*
 * Caution: Assembly code in sleep.S makes assumtion on the order
 * of the members of this structure.
 */
struct meson_pm_config {
	void *pctl_reg_base;
	void *mmc_reg_base;
	void *hiu_reg_base;
    unsigned power_key;
    unsigned ddr_clk;
    void *ddr_reg_backup;
	unsigned core_voltage_adjust;
	int sleepcount;
	void (*set_vccx2)(int power_on);
	void (*set_exgpio_early_suspend)(int power_on);
};



/*
* tvafe power tvin
*/

/* defines */
//hdmi
#define HDMI_ADDR_PORT                              0x2000
#define HDMI_DATA_PORT                              0x2004
#define RX_BASE_ADDR                                0x04000 // Base address for on-chip HDMI RX
#define RX_AUDIO_SPDIF                              (RX_BASE_ADDR+0x059)
#define RX_AUDIO_I2S                                (RX_BASE_ADDR+0x05A)
#define HHI_HDMI_CLK_CNTL                           0x1073
#define PERIPHS_PIN_MUX_3                           0x202f
#define PERIPHS_PIN_MUX_6                           0x2032

//top clock
#define HHI_VAFE_CLKXTALIN_CNTL                     0x108f
#define HHI_VAFE_CLKOSCIN_CNTL                      0x1090
#define HHI_VAFE_CLKIN_CNTL                         0x1091
#define HHI_TVFE_AUTOMODE_CLK_CNTL                  0x1092
#define HHI_VAFE_CLKPI_CNTL                         0x1093
#define VDIN_MEAS_CLK_CNTL                          0x1094

//tvafe
#define TVFE_TOP_CTRL                               ((0x1B00+0x72)<<2)
    #define COMP_CLK_ENABLE_BIT             15
    #define COMP_CLK_ENABLE_WID             1
    #define DCLK_ENABLE_BIT                 14
    #define DCLK_ENABLE_WID                 1
    #define VAFE_MCLK_EN_BIT                11
    #define VAFE_MCLK_EN_WID                1
    #define EDID_CLK_EN_BIT                 10
    #define EDID_CLK_EN_WID                 1
    #define TVFE_ADC_CLK_DIV_BIT            8
    #define TVFE_ADC_CLK_DIV_WID            2
#define TVFE_RST_CTRL                                ((0x1B00+0x74)<<2)
    #define ALL_CLK_RST_BIT                 0
    #define ALL_CLK_RST_WID                 1
#define ADC_REG_21                                   ((0x1A00+0x21)<<2)
    #define FULLPDZ_BIT                     0
    #define FULLPDZ_WID                     1

//vdin
#define VDIN_COM_CTRL0                                0x1202
#define VDIN_COM_GCLK_CTRL                            0x121b


//read/write register function
#if 1
#define WRITE_CBUS_REG(addr, data)      (*(volatile unsigned long *)(0xc1100000|(addr<<2))=data)
#define READ_CBUS_REG(addr)             (*(volatile unsigned long *)(0xc1100000|(addr<<2)))

#define WRITE_CBUS_REG_BITS(reg, val, start, len) \
    WRITE_CBUS_REG(reg, Rd(reg) & ~(((1L<<(len))-1)<<(start)) | ((unsigned int)(val) << (start)))

#define READ_CBUS_REG_BITS(reg, start, len) \
    ((READ_CBUS_REG(reg) >> (start)) & ((1L<<(len))-1))

#define WRITE_APB_REG(addr, data)       (*(volatile unsigned long *)(0xd0040000 +(addr ))=data)
#define READ_APB_REG(addr)              (*(volatile unsigned long *)(0xd0040000 + (addr )))

#define WRITE_APB_REG_BITS(reg, val, start, len) \
    WRITE_APB_REG(reg, Rd(reg) & ~(((1L<<(len))-1)<<(start)) | ((unsigned int)(val) << (start)))

#define READ_APB_REG_BITS(reg, start, len) \
    ((READ_APB_REG(reg) >> (start)) & ((1L<<(len))-1))

#else
#define CBUS_REG_OFFSET(reg) ((reg) << 2)
#define CBUS_REG_ADDR(reg)	 (IO_CBUS_BASE + CBUS_REG_OFFSET(reg))
#define WRITE_CBUS_REG(reg, val) __raw_writel(val, CBUS_REG_ADDR(reg))
#define READ_CBUS_REG(reg) (__raw_readl(CBUS_REG_ADDR(reg)))
#define WRITE_CBUS_REG_BITS(reg, val, start, len) \
    WRITE_CBUS_REG(reg,	(READ_CBUS_REG(reg) & ~(((1L<<(len))-1)<<(start)) )| ((unsigned)((val)&((1L<<(len))-1)) << (start)))
#define READ_CBUS_REG_BITS(reg, start, len) \
    ((READ_CBUS_REG(reg) >> (start)) & ((1L<<(len))-1))

#define APB_REG_OFFSET(reg)  (reg)
#define APB_REG_ADDR(reg)	 (IO_APB_BUS_BASE + APB_REG_OFFSET(reg))
#define WRITE_APB_REG(reg, val) __raw_writel(val, APB_REG_ADDR(reg))
#define READ_APB_REG(reg) (__raw_readl(APB_REG_ADDR(reg)))
#define WRITE_APB_REG_BITS(reg, val, start, len) \
    WRITE_APB_REG(reg,	(READ_APB_REG(reg) & ~(((1L<<(len))-1)<<(start)) )| ((unsigned)((val)&((1L<<(len))-1)) << (start)))
#define READ_APB_REG_BITS(reg, start, len) \
    ((READ_APB_REG(reg) >> (start)) & ((1L<<(len))-1))
#endif

static unsigned int hdmi_rd_reg(unsigned long addr)
{
    unsigned long data;
    WRITE_APB_REG(HDMI_ADDR_PORT, addr);
    WRITE_APB_REG(HDMI_ADDR_PORT, addr);

    data = READ_APB_REG(HDMI_DATA_PORT);

    return (data);
}

static void hdmi_wr_reg(unsigned long addr, unsigned long data)
{
    unsigned long rd_data;

    WRITE_APB_REG(HDMI_ADDR_PORT, addr);
    WRITE_APB_REG(HDMI_ADDR_PORT, addr);

    WRITE_APB_REG(HDMI_DATA_PORT, data);
    rd_data = hdmi_rd_reg (addr);
    if (rd_data != data)
    {
       //while(1){};
    }
}

/* tvin power down control */
static void tvafe_hw_powerdown(void)
{
    // enable clock first

    //main clk up
    WRITE_CBUS_REG(HHI_VAFE_CLKXTALIN_CNTL, 0x100);
    WRITE_CBUS_REG(HHI_VAFE_CLKOSCIN_CNTL, 0x100);
    WRITE_CBUS_REG(HHI_VAFE_CLKIN_CNTL, 0x100);
    WRITE_CBUS_REG(HHI_VAFE_CLKPI_CNTL, 0x100);
    WRITE_CBUS_REG(HHI_TVFE_AUTOMODE_CLK_CNTL, 0x100);

    //tvfe power up
    WRITE_APB_REG_BITS(TVFE_TOP_CTRL, 1, COMP_CLK_ENABLE_BIT, COMP_CLK_ENABLE_WID);
    WRITE_APB_REG_BITS(TVFE_TOP_CTRL, 1, EDID_CLK_EN_BIT, EDID_CLK_EN_WID);
    WRITE_APB_REG_BITS(TVFE_TOP_CTRL, 1, DCLK_ENABLE_BIT, DCLK_ENABLE_WID);
    WRITE_APB_REG_BITS(TVFE_TOP_CTRL, 1, VAFE_MCLK_EN_BIT, VAFE_MCLK_EN_WID);
	WRITE_APB_REG_BITS(TVFE_TOP_CTRL, 3, TVFE_ADC_CLK_DIV_BIT, TVFE_ADC_CLK_DIV_WID);
    //adc power up
    WRITE_APB_REG_BITS(ADC_REG_21, 1, FULLPDZ_BIT, FULLPDZ_WID);

    /*reset module*/
    WRITE_APB_REG_BITS(TVFE_RST_CTRL, 1, ALL_CLK_RST_BIT, ALL_CLK_RST_WID);
    WRITE_APB_REG_BITS(TVFE_RST_CTRL, 0, ALL_CLK_RST_BIT, ALL_CLK_RST_WID);

    // disable
    //adc power down
    WRITE_APB_REG_BITS(ADC_REG_21, 0, FULLPDZ_BIT, FULLPDZ_WID);

    //tvfe power down
    WRITE_APB_REG_BITS(TVFE_TOP_CTRL, 0, COMP_CLK_ENABLE_BIT, COMP_CLK_ENABLE_WID);
    WRITE_APB_REG_BITS(TVFE_TOP_CTRL, 0, EDID_CLK_EN_BIT, EDID_CLK_EN_WID);
    WRITE_APB_REG_BITS(TVFE_TOP_CTRL, 0, DCLK_ENABLE_BIT, DCLK_ENABLE_WID);
    WRITE_APB_REG_BITS(TVFE_TOP_CTRL, 0, VAFE_MCLK_EN_BIT, VAFE_MCLK_EN_WID);
    WRITE_APB_REG_BITS(TVFE_TOP_CTRL, 0, TVFE_ADC_CLK_DIV_BIT, TVFE_ADC_CLK_DIV_WID);

    //main clk down
    WRITE_CBUS_REG(HHI_VAFE_CLKXTALIN_CNTL, 0);
    WRITE_CBUS_REG(HHI_VAFE_CLKOSCIN_CNTL, 0);
    WRITE_CBUS_REG(HHI_VAFE_CLKIN_CNTL, 0);
    WRITE_CBUS_REG(HHI_VAFE_CLKPI_CNTL, 0);
    WRITE_CBUS_REG(HHI_TVFE_AUTOMODE_CLK_CNTL, 0);
}

static void hdmi_hw_powerdown(void)
{
    //enable clock
    WRITE_CBUS_REG( HHI_HDMI_CLK_CNTL,  ((1 << 9)  |   // select "other" PLL
                             (1 << 8)  |   // Enable gated clock
                             (3 << 0)) );  // Divide the "other" PLL output by 4

    //hdmi audio gate disable
    hdmi_wr_reg(RX_AUDIO_I2S, 0x0);
    hdmi_wr_reg(RX_AUDIO_SPDIF, 0x0);

    //phy power down
    hdmi_wr_reg(RX_BASE_ADDR+0x864, hdmi_rd_reg(RX_BASE_ADDR+0x864)    | (1<<7)); // rx0_eq_pd = 1
    hdmi_wr_reg(RX_BASE_ADDR+0x86C, hdmi_rd_reg(RX_BASE_ADDR+0x86C)    | (1<<7)); // rx1_eq_pd = 1
    hdmi_wr_reg(RX_BASE_ADDR+0x874, hdmi_rd_reg(RX_BASE_ADDR+0x874)    | (1<<7)); // rx2_eq_pd = 1
    hdmi_wr_reg(RX_BASE_ADDR+0x87C, hdmi_rd_reg(RX_BASE_ADDR+0x87C)    | (1<<7)); // rx3_eq_pd = 1
    //pr_info("hdmirx_hw: rx3_eq_pd\n");
    hdmi_wr_reg(RX_BASE_ADDR+0x866, hdmi_rd_reg(RX_BASE_ADDR+0x866)    | (1<<6)); // cdr0_pd = 1
    hdmi_wr_reg(RX_BASE_ADDR+0x86E, hdmi_rd_reg(RX_BASE_ADDR+0x86E)    | (1<<6)); // cdr1_pd = 1
    hdmi_wr_reg(RX_BASE_ADDR+0x876, hdmi_rd_reg(RX_BASE_ADDR+0x876)    | (1<<6)); // cdr2_pd = 1
    hdmi_wr_reg(RX_BASE_ADDR+0x87E, hdmi_rd_reg(RX_BASE_ADDR+0x87E)    | (1<<6)); // cdr3_pd = 1
    //pr_info("hdmirx_hw: cdr3_pd\n");
    hdmi_wr_reg(RX_BASE_ADDR+0x005, 0<<0); // [2:0] port_en = 0

    hdmi_wr_reg(RX_BASE_ADDR+0x097, hdmi_rd_reg(RX_BASE_ADDR+0x097)|
                                (1<<7)| // rxref_pd = 1
                                (1<<6)| // rxref_pd_vgap = 1
                                (1<<5));// rxref_pd_op2 = 1
}

static void vdin_hw_powerdown(unsigned int offset)
{
    //set VDIN_MEAS_CLK_CNTL, select XTAL clock
    WRITE_CBUS_REG((VDIN_MEAS_CLK_CNTL + offset), 0x00000000);

    /* disable video data input */
    // [    4]  top.datapath_en  = 0
    WRITE_CBUS_REG_BITS((VDIN_COM_CTRL0 + offset), 0, 4, 1);

    /* mux null input */
    // [ 3: 0]  top.mux  = 0/(null, mpeg, 656, tvfe, cvd2, hdmi, dvin)
    WRITE_CBUS_REG_BITS((VDIN_COM_CTRL0 + offset), 0, 0, 4);

    /* disable clock of blackbar, histogram, histogram, line fifo1, matrix,
     * hscaler, pre hscaler, clock0
     */
    // [15:14]  Disable blackbar clock      = 01/(auto, off, on, on)
    // [13:12]  Disable histogram clock     = 01/(auto, off, on, on)
    // [11:10]  Disable line fifo1 clock    = 01/(auto, off, on, on)
    // [ 9: 8]  Disable matrix clock        = 01/(auto, off, on, on)
    // [ 7: 6]  Disable hscaler clock       = 01/(auto, off, on, on)
    // [ 5: 4]  Disable pre hscaler clock   = 01/(auto, off, on, on)
    // [ 3: 2]  Disable clock0              = 01/(auto, off, on, on)
    // [    0]  Enable register clock       = 00/(auto, off!!!!!!!!)
    WRITE_CBUS_REG((VDIN_COM_GCLK_CTRL + offset), 0x5554);
}

static void tvin_powerdown(void)
{
    tvafe_hw_powerdown();
    hdmi_hw_powerdown();
    vdin_hw_powerdown(0x00);  //vdin0
    vdin_hw_powerdown(0x70);  //vdin1
}

/////////////////////////////////////////////////


void cooling(void)
{
	int i;
	unsigned int tmp_data;

	static struct meson_pm_config aml_pm_pdata = {
    .pctl_reg_base = (void *)IO_APB_BUS_BASE,
    .mmc_reg_base = (void *)APB_REG_ADDR(0x1000),
    .hiu_reg_base = (void *)CBUS_REG_ADDR(0x1000),
    .power_key = (1<<15),
    .ddr_clk = 0x00120234, // 312m, 0x00110220, //384m
    .sleepcount = 128,
    //.set_vccx2 = set_vccx2,	//Elvis Fool
    .core_voltage_adjust = 5,
	};

	tvin_powerdown();

	writel(0,P_WATCHDOG_TC);//disable Watchdog
	//amp  reset down GPIOX_51
	clrbits_le32(P_PREG_FGPIO_O, 1<<19);
	clrbits_le32(P_PREG_FGPIO_EN_N, 1<<19);
	
	__udelay(1000);

	//USB_PWR_CTL down x44
	clrbits_le32(P_PREG_FGPIO_O, 1<<12);
	clrbits_le32(P_PREG_FGPIO_EN_N, 1<<12);
    //mute down x69
	setbits_le32(P_PREG_GGPIO_O, 1<<5);
	clrbits_le32(P_PREG_GGPIO_EN_N, 1<<5);
	for(i=0; i<100; i++)
	{
		__udelay(1000);
	}
	//GPIOX_53 reset chip power ctrl BL_EN
	setbits_le32(P_PREG_FGPIO_O, 1<<21);
	clrbits_le32(P_PREG_FGPIO_EN_N, 1<<21);
	 //pwm x55
	clrbits_le32(P_PREG_FGPIO_O, 1<<23);
	clrbits_le32(P_PREG_FGPIO_EN_N, 1<<23);
	//pwm b2		
	setbits_le32(P_PREG_GGPIO_O, 1<<10);
    clrbits_le32(P_PREG_GGPIO_EN_N, 1<<10);
	//pwm b3		
	setbits_le32(P_PREG_GGPIO_O, 1<<11);
    clrbits_le32(P_PREG_GGPIO_EN_N, 1<<11);	
	for(i=0; i<100; i++)
	{
		__udelay(1000);
	}
	//GPIOX_48 LED_G
	clrbits_le32(P_PREG_FGPIO_O, 1<<16);
	clrbits_le32(P_PREG_FGPIO_EN_N, 1<<16);
	
	//GPIOX_56 LED_R
	setbits_le32(P_PREG_FGPIO_O, 1<<24);
	clrbits_le32(P_PREG_FGPIO_EN_N, 1<<24);
	//vcc_12v/24v power down GPIOX_70
	//clrbits_le32(P_PREG_GGPIO_O, 1<<6);
	//clrbits_le32(P_PREG_GGPIO_EN_N, 1<<6);

	serial_hw_init();
	//setbits_le32(P_PERIPHS_PIN_MUX_2,((1<<29)|(1<<30)));
	writel(0x18003033, P_UART1_CONTROL);

	serial_puts("\nstandby...\n");

	writel(0x209861f1, P_HHI_GCLK_MPEG0);
	writel(0x208b8028, P_HHI_GCLK_MPEG1);
	writel(0xfffffc07, P_HHI_GCLK_MPEG2);
	writel(0xffc40021, P_HHI_GCLK_OTHER);

	//analog off
	WRITE_CBUS_REG(SAR_ADC_REG3, 0x3008510a);
	//WRITE_CBUS_REG(VGHL_PWM_REG0, 0x0); //the same with off
	//WRITE_CBUS_REG(WIFI_ADC_SAMPLING, 0x0); //the same with off
	WRITE_APB_REG(ADC_EN_ADC, 0x0); //the same with off
	//WRITE_AHB_REG(WIFI_ADC_DAC, 0x0); //the same with off
	//WRITE_AHB_REG(ADC_EN_CMLGEN_RES, 0x0); //the same with off
	//WRITE_AHB_REG(WIFI_SARADC, 0x0); //the same with off
	//usb off
	WRITE_CBUS_REG(PREI_USB_PHY_REG, 0x8040012b);

	//clock off
	//WRITE_CBUS_REG(HHI_DEMOD_CLK_CNTL, 0x400); //the same with off
	//WRITE_CBUS_REG(HHI_SATA_CLK_CNTL, 0x0); //the same with off
	//WRITE_CBUS_REG(HHI_ETH_CLK_CNTL, 0x207); //the same with off
	//WRITE_CBUS_REG(HHI_WIFI_CLK_CNTL, 0x0); //the same with off
	WRITE_CBUS_REG(HHI_VID_CLK_CNTL, 0x840e);
	WRITE_CBUS_REG(HHI_AUD_CLK_CNTL, 0x800018);
	WRITE_CBUS_REG(HHI_MALI_CLK_CNTL, 0x202);
	WRITE_CBUS_REG(HHI_HDMI_CLK_CNTL, 0x203);
	WRITE_CBUS_REG(HHI_MPEG_CLK_CNTL, 0x1083);

	//pll off
	WRITE_CBUS_REG(HHI_DEMOD_PLL_CNTL, 0x8232);
	WRITE_CBUS_REG(HHI_VID_PLL_CNTL, 0x8641);
	WRITE_CBUS_REG(HHI_AUD_PLL_CNTL, 0xca80);
	WRITE_CBUS_REG(HHI_OTHER_PLL_CNTL, 0x887d);

#ifdef SYSTEM_16K
	if (READ_CBUS_REG(HHI_MPEG_CLK_CNTL)&(1<<8))
		CLEAR_CBUS_REG_MASK(HHI_MPEG_CLK_CNTL, (1<<8)); // clk81 = xtal
	SET_CBUS_REG_MASK(HHI_MPEG_CLK_CNTL, (1<<9));		// xtal_rtc = rtc
	WRITE_CBUS_REG_BITS(HHI_MPEG_CLK_CNTL, 0x1, 0, 6);	// devider = 2
	WRITE_CBUS_REG_BITS(HHI_MPEG_CLK_CNTL, 0, 12, 2);	// clk81 src -> xtal_rtc
	SET_CBUS_REG_MASK(HHI_MPEG_CLK_CNTL, (1<<8));		// clk81 = xtal_rtc / devider
#else
	CLEAR_CBUS_REG_MASK(HHI_MPEG_CLK_CNTL, (1<<8)); // clk81 = xtal
	WRITE_CBUS_REG_BITS(HHI_MPEG_CLK_CNTL, 0x1e, 0, 6); // devider = 30
	WRITE_CBUS_REG_BITS(HHI_MPEG_CLK_CNTL, 0, 12, 2);	// clk81 src -> xtal_rtc
	SET_CBUS_REG_MASK(HHI_MPEG_CLK_CNTL, (1<<8));		// clk81 = xtal_rtc / devider
#endif
	CLEAR_CBUS_REG_MASK(HHI_A9_CLK_CNTL, (1<<7));		// clka9 = xtal_rtc / 2
#ifdef SYSTEM_16K
	SET_CBUS_REG_MASK(PREG_CTLREG0_ADDR, 1);
#endif
	WRITE_CBUS_REG(HHI_A9_AUTO_CLK0,
		(2 << 24)	 |	 // sleep select 1000uS timebase
		(0x20 << 16)	  |   // Set the delay wakeup time (32mS)
		(0 << 5)		|	// don't clear the FIQ global mask
		(0 << 4)		|	// don't clear the IRQ global mask
		(2 << 2));				  // Set interrupt wakeup only
	WRITE_CBUS_REG(HHI_A9_AUTO_CLK1,
		(0 << 20)				|	// start delay timebase
		(1 << 12)	 |	 // 1uS enable delay
		(1 << 8)   |   // 1uS gate delay
		(1 << 0));		   // 1us start delay
	SET_CBUS_REG_MASK(HHI_A9_AUTO_CLK0, 1 << 0);	//ENABLE:  If this bit is set to 1, then the auto-clock ratio module will execute a change once the START pulses is generated.
	SET_CBUS_REG_MASK(HHI_SYS_PLL_CNTL, (1<<15));		// turn off sys pll
	/*
	while(1)
	{
		if(serial_tstc())	break;
	}*/

	WRITE_CBUS_REG(0x2620, 0x10001);
	WRITE_CBUS_REG(0x2621,  0x24);
	WRITE_CBUS_REG(0x2622,  0);
	WRITE_CBUS_REG(0x2623,  0);
	WRITE_CBUS_REG(0x2624,  0);
	enable_custom_trigger();
#ifdef CONFIG_MACH_MESON2_7366M_CST02
    WRITE_CBUS_REG(A9_0_IRQ_IN2_INTR_MASK, 1);    //enable gpio interrupt
#endif
#ifdef CONFIG_MACH_MESON2_7366M_REFE03
    WRITE_CBUS_REG(A9_0_IRQ_IN2_INTR_MASK, 1);    //enable gpio interrupt
#endif
    tmp_data = READ_CBUS_REG(PREG_CTLREG0_ADDR);
    WRITE_CBUS_REG(PREG_CTLREG0_ADDR, tmp_data | 0x1);
    tmp_data = READ_CBUS_REG(IR_DEC_REG0);
    WRITE_CBUS_REG(IR_DEC_REG0, tmp_data & 0xFFFFFF00);
    tmp_data = READ_CBUS_REG(IR_DEC_REG1);
    WRITE_CBUS_REG(IR_DEC_REG1, tmp_data | 0x00000001);
    WRITE_CBUS_REG(IR_DEC_REG1, tmp_data & 0xFFFFFFFE);

    //hub reset down x50
	clrbits_le32(P_PREG_FGPIO_O, 1<<18);
	clrbits_le32(P_PREG_FGPIO_EN_N, 1<<18);
	
	//GPIOX_32 si2176_RESET down
	clrbits_le32(P_PREG_FGPIO_O, 1<<0);
	clrbits_le32(P_PREG_FGPIO_EN_N, 1<<0);
	//PHY_RESET x57
	clrbits_le32(P_PREG_FGPIO_O, 1<<25);
	clrbits_le32(P_PREG_FGPIO_EN_N, 1<<25);

	
	//close ddr
	//gpiob_8 
	setbits_le32(P_PREG_GGPIO_O, 1<<16);
	clrbits_le32(P_PREG_GGPIO_EN_N, 1<<16);
	//gpiob_5 
	setbits_le32(P_PREG_GGPIO_O, 1<<13);
	clrbits_le32(P_PREG_GGPIO_EN_N, 1<<13);
	//gpiob_6 
	//setbits_le32(P_PREG_GGPIO_O, 1<<14);
	//clrbits_le32(P_PREG_GGPIO_EN_N, 1<<14);

 	for(i=0; i<100; i++)
	{
		__udelay(1000);
	}
	meson_cpu_suspend(aml_pm_pdata);
	for(i=0; i<100; i++)
	{
		__udelay(1000);
	}
    //GPIOX_54 reset chip panel power
	clrbits_le32(P_PREG_FGPIO_O, 1<<22);
	clrbits_le32(P_PREG_FGPIO_EN_N, 1<<22);
	 //pwm x55
	setbits_le32(P_PREG_FGPIO_O, 1<<23);
	clrbits_le32(P_PREG_FGPIO_EN_N, 1<<23);
	//pwm b2		
	clrbits_le32(P_PREG_GGPIO_O, 1<<10);
    clrbits_le32(P_PREG_GGPIO_EN_N, 1<<10);
	//pwm b3		
	clrbits_le32(P_PREG_GGPIO_O, 1<<11);
    clrbits_le32(P_PREG_GGPIO_EN_N, 1<<11);
	
	for(i=0; i<300; i++)
	{
		__udelay(1000);
	}
	//vcc_12v/24v power on
	setbits_le32(P_PREG_GGPIO_O, 1<<6);
	clrbits_le32(P_PREG_GGPIO_EN_N, 1<<6);
	//setbits_le32(P_PREG_GGPIO_EN_N, 1<<6);
	for(i=0; i<800; i++)
	{
		__udelay(1000);
	}
	//GPIOX_53 reset chip power ctrl
	//setbits_le32(P_PREG_FGPIO_O, 1<<21);

	memory_pll_init(0,NULL);

	serial_puts("\ngate clock on...\n");


	writel(0xffffffff, P_HHI_GCLK_MPEG0);
	writel(0xffffffff, P_HHI_GCLK_MPEG1);
	writel(0xffffffff, P_HHI_GCLK_MPEG2);
	writel(0xffffffff, P_HHI_GCLK_OTHER);
#if 0
	//analog on
	WRITE_CBUS_REG(SAR_ADC_REG3, 0x2008510a);
	//WRITE_CBUS_REG(VGHL_PWM_REG0, 0x0);	//the same with off
	//WRITE_CBUS_REG(WIFI_ADC_SAMPLING, 0x0); //the same with off
	//WRITE_APB_REG(ADC_EN_ADC, 0x0); //the same with off
	//WRITE_AHB_REG(WIFI_ADC_DAC, 0x0); //the same with off
	//WRITE_AHB_REG(ADC_EN_CMLGEN_RES, 0x0); //the same with off
	//WRITE_AHB_REG(WIFI_SARADC, 0x0); //the same with off
	//usb on
	WRITE_CBUS_REG(PREI_USB_PHY_REG, 0x80400128);

	//clock on
	//WRITE_CBUS_REG(HHI_DEMOD_CLK_CNTL, 0x400); //the same with off
	//WRITE_CBUS_REG(HHI_SATA_CLK_CNTL, 0x0); //the same with off
	//WRITE_CBUS_REG(HHI_ETH_CLK_CNTL, 0x207); //the same with off
	//WRITE_CBUS_REG(HHI_WIFI_CLK_CNTL, 0x0); //the same with off
	WRITE_CBUS_REG(HHI_VID_CLK_CNTL, 0x840f);
	WRITE_CBUS_REG(HHI_AUD_CLK_CNTL, 0x800018);
	WRITE_CBUS_REG(HHI_MALI_CLK_CNTL, 0x302);
	WRITE_CBUS_REG(HHI_HDMI_CLK_CNTL, 0x303);
	WRITE_CBUS_REG(HHI_MPEG_CLK_CNTL, 0x1183);
	//pll on
	WRITE_CBUS_REG(HHI_DEMOD_PLL_CNTL, 0x232);
	WRITE_CBUS_REG(HHI_VID_PLL_CNTL, 0x641);
	WRITE_CBUS_REG(HHI_AUD_PLL_CNTL, 0x4a80);
	//WRITE_CBUS_REG(HHI_OTHER_PLL_CNTL, 0x87d);
#endif

	
	return 0;
}

