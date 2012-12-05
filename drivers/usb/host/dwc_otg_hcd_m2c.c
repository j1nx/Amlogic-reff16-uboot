#include <common.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <usb.h>
#include <malloc.h>
#include <linux/list.h>


#include "dwc_otg_regs.h"
#include "dwc_otg_hcd.h"

#define kmalloc(x,y)			malloc(x)
#define kfree(x)			    free(x)

#define flush_cpu_cache()  do{ dcache_flush();}while(0)//flush_cache //do{_invalidate_dcache();AV_AHB_bus_invalidate();}while(0)	//for porting

static dwc_otg_device_t dwc_otg_dev;

//////////////////////
#include <asm/arch/usb.h>
static char * clock_src_name[]={
		"XTAL input",
		"XTAL input divided by 2",
		"Other PLL 600MHz(M2C)",
		"DDR PLL 528MHz(M2C)",		
};
/*
   cfg = 0 : XTAL input
   cfg = 1 : XTAL input divided by 2
   cfg = 2 : other PLL
   cfg = 3 : DDR pll   
  */
static void dwc_udelay(int nDelay)
{
	int nMax;		
	do{
		nMax = 5000;
		while(nMax--);
	}while(nDelay--);
}

#define SET_CBUS_REG_MASK_USB(reg,mask)    { SET_CBUS_REG_MASK(reg,mask);   dwc_udelay(10);}
#define CLEAR_CBUS_REG_MASK_USB(reg,mask)  { CLEAR_CBUS_REG_MASK(reg,mask); dwc_udelay(10);}

void set_usb_phy_config(int clk_sel)
{
	int divider =0;         
	// ------------------------------------------------------------
	//  CLK_SEL: These bits select the source for the 12Mhz: 
	// 0 = XTAL input (24, 25, 27Mhz)
	// 1 = XTAL input divided by 2
	// 2 = other PLL output
	// 3 = DDR pll clock (typically 400mhz)	
	CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG, PREI_USB_PHY_CLK_SEL);
	//clk_sel = 0; // 24M CLK 
	//clk_sel = 1; // 12M, Phy default setting is 12Mhz
	//clk_sel = 2; // other PLL, M2C:600M;M2B:xxx
	//clk_sel = 3; // DDR, 528M, M2C:528M;M2B:xxx
	
	printf("clk_sel: %s\n",clock_src_name[clk_sel]);
	
	SET_CBUS_REG_MASK(PREI_USB_PHY_REG, (clk_sel<<5 ));

	CLEAR_CBUS_REG_MASK(PREI_USB_PHY_REG,PREI_USB_PHY_CLK_DIV);

	switch(clk_sel)
	{
	case USB_PHY_CLOCK_SEL_XTAL:divider = 1;      break; //24/12 - 1 = 1;
	case USB_PHY_CLOCK_SEL_XTAL_DIV2:divider = 0; break; //12/12 - 1 = 0;
	case USB_PHY_CLOCK_SEL_OTHER_PLL:divider = 49;break; //M2C: 600/12 - 1 = 49;M2B: XXX
	case USB_PHY_CLOCK_SEL_DDR_PLL:divider = 43;  break; //M2C: 528/12 - 1 = 43;M2B: XXX
	default: printf("Error! Unknown PLL setting! Please check!\n"); break;
	//case USB_PHY_CLOCK_SEL_DEMOD_PLL:
	//case USB_PHY_CLOCK_SEL_DEMOD_PLL_1:
	//case USB_PHY_CLOCK_SEL_SYS_PLL_3:
	//case USB_PHY_CLOCK_SEL_SYS_PLL_4:break;
	}
	
	SET_CBUS_REG_MASK(PREI_USB_PHY_REG, (divider << 24));
	  
	// Open clock gate, to enable CLOCK to usb phy 
	SET_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_CLK_GATE);	
	
    /*  Reset USB PHY A  */
    SET_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_A_AHB_RSET);
       
    CLEAR_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_A_AHB_RSET);
    
    SET_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_A_CLK_RSET);
       
    CLEAR_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_A_CLK_RSET);
    
    SET_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_A_PLL_RSET);
    
    CLEAR_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_A_PLL_RSET);
    
    // ------------------------------------------------------------ 
    // Reset the PHY A by setting POR high for 10uS.
    SET_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_A_POR);    
    // Set POR to the PHY high
    CLEAR_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_A_POR);
    
        
    //SET_CBUS_REG_MASK_USB(PREI_ARC_INTR0_MASK,USB_PHY_A_INTR_BIT);


    /* Reset USB PHY B */
    SET_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_B_AHB_RSET);
     
    CLEAR_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_B_AHB_RSET);
    
    SET_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_B_CLK_RSET);
    
    CLEAR_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_B_CLK_RSET);
    
    SET_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_B_PLL_RSET);
     
    CLEAR_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_B_PLL_RSET);

    // ------------------------------------------------------------ 
    // Reset the PHY B by setting POR high for 10uS.
    SET_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_B_POR);    
    // Set POR to the PHY high
    CLEAR_CBUS_REG_MASK_USB(PREI_USB_PHY_REG, PREI_USB_PHY_B_POR);
    
    //SET_CBUS_REG_MASK_USB(PREI_ARC_INTR0_MASK,USB_PHY_B_INTR_BIT);
}
/**
 * Control USB VBus Power by GPIO Line.
 * bsp_init() must be called first.
 * @param [in] is_power_on TRUE of Power on VBus, FALSE of Power off VBus
 */
void
bsp_set_dwc_chip_0_gpio_power(char is_power_on)
{
  
}
/////////////////////////
/*
 * Host module config 
 */
static dwc_otg_core_params_t dwc_otg_module_params_host = {
    DWC_OTG_CAP_PARAM_NO_HNP_SRP_CAPABLE,       /* otg_cap */
    0,                          /* dma_enable */
    -1,                         /* dma_burst_size */
    DWC_SPEED_PARAM_HIGH,       /* speed */
    0,                          /* host_support_fs_ls_low_power */
    0,                          /* host_ls_low_power_phy_clk */
    1,                          /* enable_dynamic_fifo */
    -1,                         /* data_fifo_size */
    0x100,                      /* dev_rx_fifo_size */
    0x100,                      /* dev_nperio_tx_fifo_size */
    {-1,                        /* dev_perio_tx_fifo_size *//* 0 */
     -1,                        /* 1 */
     -1,                        /* 2 */
     -1,                        /* 3 */
     -1,                        /* 4 */
     -1,                        /* 5 */
     -1,                        /* 6 */
     -1,                        /* 7 */
     -1,                        /* 8 */
     -1,                        /* 9 */
     -1,                        /* 10 */
     -1,                        /* 11 */
     -1,                        /* 12 */
     -1,                        /* 13 */
     -1},                       /* 14 */
    320,                        /* host_rx_fifo_size */
    256,                        /* host_nperio_tx_fifo_size */
    -1,                         /* host_perio_tx_fifo_size */
    65535,                      /* max_transfer_size */
    128,                        /* max_packet_count */
    6,                          /* host_channels */
    2,                          /* dev_endpoints */
    -1,                         /* phy_type */
    -1,                         /* phy_utmi_width */
    0,                          /* phy_ulpi_ddr */
    0,                          /* i2c_enable */
};

/**
 * This function disables the controller's Global Interrupt in the AHB Config
 * register.
 *
 * @param[in] _core_if Programming view of DWC_otg controller.
 */
static void
dwc_otg_disable_global_interrupts(dwc_otg_core_if_t * _core_if)
{
    gahbcfg_data_t  ahbcfg = {.d32 = 0 };
    ahbcfg.b.glblintrmsk = 1;   /* Enable interrupts */
    dwc_modify_reg32(&_core_if->core_global_regs->gahbcfg, ahbcfg.d32, 0);
}
/**
 * This function enables the controller's Global Interrupt in the AHB Config
 * register.
 *
 * @param[in] _core_if Programming view of DWC_otg controller.
 */
extern void
dwc_otg_enable_global_interrupts(dwc_otg_core_if_t * _core_if)
{
    gahbcfg_data_t  ahbcfg = {.d32 = 0 };
    ahbcfg.b.glblintrmsk = 1;   /* Enable interrupts */
    dwc_modify_reg32(&_core_if->core_global_regs->gahbcfg, 0, ahbcfg.d32);
}

static dwc_otg_core_if_t *
dwc_otg_cil_init(const uint32_t * _reg_base_addr, dwc_otg_core_params_t * _core_params)
{

    dwc_otg_core_if_t *core_if = 0;
    // dwc_otg_dev_if_t *dev_if = 0;
    dwc_otg_host_if_t *host_if = 0;
    uint8_t        *reg_base = (uint8_t *) _reg_base_addr;
    int             i = 0;

    DWC_DEBUGPL(DBG_CILV, "%s(%p,%p)\n", __func__, _reg_base_addr, _core_params);

    core_if = (dwc_otg_core_if_t *) kmalloc(sizeof(dwc_otg_core_if_t), 0);

    if (core_if == 0) {
        DWC_DEBUGPL(DBG_CIL, "Allocation of dwc_otg_core_if_t failed\n");
        return 0;
    }

    memset(core_if, 0, sizeof(dwc_otg_core_if_t));

    core_if->core_params = _core_params;
    core_if->core_global_regs = (dwc_otg_core_global_regs_t *) reg_base;

    /*
     * Allocate the Host Mode structures.
     */
    host_if = (dwc_otg_host_if_t *) kmalloc(sizeof(dwc_otg_host_if_t), 0);

    if (host_if == 0) {
        DWC_DEBUGPL(DBG_CIL, "Allocation of dwc_otg_host_if_t failed\n");
        // kfree(dev_if);
        kfree(core_if);
        return 0;
    }

    host_if->host_global_regs = (dwc_otg_host_global_regs_t *)
        (reg_base + DWC_OTG_HOST_GLOBAL_REG_OFFSET);

    host_if->hprt0 = (uint32_t *) (reg_base + DWC_OTG_HOST_PORT_REGS_OFFSET);

    for (i = 0; i < MAX_EPS_CHANNELS; i++) {
        host_if->hc_regs[i] = (dwc_otg_hc_regs_t *)
            (reg_base + DWC_OTG_HOST_CHAN_REGS_OFFSET + (i * DWC_OTG_CHAN_REGS_OFFSET));
        DWC_DEBUGPL(DBG_CILV, "hc_reg[%d]->hcchar=%p\n", i, &host_if->hc_regs[i]->hcchar);
    }

    host_if->num_host_channels = MAX_EPS_CHANNELS;
    core_if->host_if = host_if;

    for (i = 0; i < MAX_EPS_CHANNELS; i++) {
        core_if->data_fifo[i] =
            (uint32_t *) (reg_base + DWC_OTG_DATA_FIFO_OFFSET + (i * DWC_OTG_DATA_FIFO_SIZE));
        DWC_DEBUGPL(DBG_CILV, "data_fifo[%d]=0x%08x\n", i, (unsigned) core_if->data_fifo[i]);
    }

    core_if->pcgcctl = (uint32_t *) (reg_base + DWC_OTG_PCGCCTL_OFFSET);

    /*
     * Store the contents of the hardware configuration registers here for
     * easy access later.
     */
    core_if->hwcfg1.d32 = dwc_read_reg32(&core_if->core_global_regs->ghwcfg1);
    core_if->hwcfg2.d32 = dwc_read_reg32(&core_if->core_global_regs->ghwcfg2);
    core_if->hwcfg3.d32 = dwc_read_reg32(&core_if->core_global_regs->ghwcfg3);
    core_if->hwcfg4.d32 = dwc_read_reg32(&core_if->core_global_regs->ghwcfg4);

    DWC_DEBUGPL(DBG_CILV, "hwcfg1=%08x\n", core_if->hwcfg1.d32);
    DWC_DEBUGPL(DBG_CILV, "hwcfg2=%08x\n", core_if->hwcfg2.d32);
    DWC_DEBUGPL(DBG_CILV, "hwcfg3=%08x\n", core_if->hwcfg3.d32);
    DWC_DEBUGPL(DBG_CILV, "hwcfg4=%08x\n", core_if->hwcfg4.d32);

    DWC_DEBUGPL(DBG_CILV, "op_mode=%0x\n", core_if->hwcfg2.b.op_mode);
    DWC_DEBUGPL(DBG_CILV, "arch=%0x\n", core_if->hwcfg2.b.architecture);
    DWC_DEBUGPL(DBG_CILV, "num_dev_ep=%d\n", core_if->hwcfg2.b.num_dev_ep);
    DWC_DEBUGPL(DBG_CILV, "num_host_chan=%d\n", core_if->hwcfg2.b.num_host_chan);
    DWC_DEBUGPL(DBG_CILV, "nonperio_tx_q_depth=0x%0x\n", core_if->hwcfg2.b.nonperio_tx_q_depth);
    DWC_DEBUGPL(DBG_CILV, "host_perio_tx_q_depth=0x%0x\n", core_if->hwcfg2.b.host_perio_tx_q_depth);
    DWC_DEBUGPL(DBG_CILV, "dev_token_q_depth=0x%0x\n", core_if->hwcfg2.b.dev_token_q_depth);

    DWC_DEBUGPL(DBG_CILV, "Total FIFO SZ=%d\n", core_if->hwcfg3.b.dfifo_depth);
    DWC_DEBUGPL(DBG_CILV, "xfer_size_cntr_width=%0x\n", core_if->hwcfg3.b.xfer_size_cntr_width);
    
    /*
     * Set the SRP sucess bit for FS-I2c
     */
    core_if->srp_success = 0;
    core_if->srp_timer_started = 0;
    core_if->temp_buffer = kmalloc(DWC_OTG_MAX_TRANSFER_SIZE, 0);
    if(!core_if->temp_buffer) {
        kfree(core_if->host_if);
        kfree(core_if);
        return 0;
    }        

    return core_if;

}
/**
 * This function initializes the commmon interrupts, used in both
 * device and host modes.
 *
 * @param[in] _core_if Programming view of the DWC_otg controller
 *
 */
static void
dwc_otg_enable_common_interrupts(dwc_otg_core_if_t * _core_if)
{
    dwc_otg_core_global_regs_t *global_regs = _core_if->core_global_regs;
    gintmsk_data_t  intr_mask = {.d32 = 0 };

    /*
     * Clear any pending OTG Interrupts 
     */
    dwc_write_reg32(&global_regs->gotgint, 0xFFFFFFFF);

    /*
     * Clear any pending interrupts 
     */
    dwc_write_reg32(&global_regs->gintsts, 0xFFFFFFFF);

    /*
     * Enable the interrupts in the GINTMSK. 
     */
    intr_mask.b.modemismatch = 1;
    intr_mask.b.otgintr = 1;

    if (!_core_if->dma_enable) {
        intr_mask.b.rxstsqlvl = 1;
    }

    intr_mask.b.conidstschng = 1;
    intr_mask.b.wkupintr = 1;
    intr_mask.b.disconnect = 1;
    intr_mask.b.usbsuspend = 1;
    intr_mask.b.sessreqintr = 1;
    dwc_write_reg32(&global_regs->gintmsk, intr_mask.d32);
}
/**
 * Do core a soft reset of the core.  Be careful with this because it
 * resets all the internal state machines of the core.
 */
void
dwc_otg_core_reset(dwc_otg_core_if_t * _core_if)
{
    dwc_otg_core_global_regs_t *global_regs = _core_if->core_global_regs;
    volatile grstctl_t greset = {.d32 = 0 };
    int             count = 0;

    DBG("%s\n", __func__);
    /*
     * Wait for AHB master IDLE state. 
     */
    do {
        dwc_udelay(1);
        greset.d32 = dwc_read_reg32(&global_regs->grstctl);
        if (++count > 100000) {
            DBG("%s() HANG! AHB Idle GRSTCTL=%0x\n", __func__, greset.d32);
            return;
        }
    }
    while (greset.b.ahbidle == 0);

    /*
     * Core Soft Reset 
     */
    count = 0;
    greset.b.csftrst = 1;
    dwc_write_reg32(&global_regs->grstctl, greset.d32);
    do {
        dwc_udelay(1);
        greset.d32 = dwc_read_reg32(&global_regs->grstctl);
        if (++count > 1000000) {
            DBG("%s() HANG! Soft Reset GRSTCTL=%0x\n", __func__, greset.d32);
            break;
        }
    }
    while (greset.b.csftrst == 1);

    /*
     * Wait for 3 PHY Clocks
     */
    wait_ms(100);
}

/**
 * Initializes the FSLSPClkSel field of the HCFG register depending on the PHY
 * type.
 */
static void
init_fslspclksel(dwc_otg_core_if_t * _core_if)
{
    uint32_t        val;
    hcfg_data_t     hcfg;

    if (_core_if->core_params->phy_type == DWC_PHY_TYPE_PARAM_FS) {
        /*
         * Full speed PHY 
         */
        val = DWC_HCFG_48_MHZ;
    } else {
        /*
         * High speed PHY running at full speed or high speed 
         */
        val = DWC_HCFG_30_60_MHZ;
    }

    DWC_DEBUGPL(DBG_CIL, "Initializing HCFG.FSLSPClkSel to 0x%1x\n", val);
    hcfg.d32 = dwc_read_reg32(&_core_if->host_if->host_global_regs->hcfg);
    hcfg.b.fslspclksel = val;
    dwc_write_reg32(&_core_if->host_if->host_global_regs->hcfg, hcfg.d32);
}

/**
 * Initializes the DevSpd field of the DCFG register depending on the PHY type
 * and the enumeration speed of the device.
 */
static void
init_devspd(dwc_otg_core_if_t * _core_if)
{
    uint32_t        val;
    dcfg_data_t     dcfg;

    if (_core_if->core_params->phy_type == DWC_PHY_TYPE_PARAM_FS) {
        /*
         * Full speed PHY 
         */
        val = 0x3;
    } else if (_core_if->core_params->speed == DWC_SPEED_PARAM_FULL) {
        /*
         * High speed PHY running at full speed 
         */
        val = 0x1;
    } else {
        /*
         * High speed PHY running at high speed 
         */
        val = 0x0;
    }

    DWC_DEBUGPL(DBG_CIL, "Initializing DCFG.DevSpd to 0x%1x\n", val);
    dcfg.d32 = dwc_read_reg32(&_core_if->dev_if->dev_global_regs->dcfg);
    dcfg.b.devspd = val;
    dwc_write_reg32(&_core_if->dev_if->dev_global_regs->dcfg, dcfg.d32);
}

/**
 * This function initializes the DWC_otg controller registers and
 * prepares the core for device mode or host mode operation.
 *
 * @param _core_if Programming view of the DWC_otg controller
 *
 */
void
dwc_otg_core_init(dwc_otg_core_if_t * _core_if)
{
    dwc_otg_core_global_regs_t *global_regs = _core_if->core_global_regs;
    // dwc_otg_dev_if_t *dev_if = _core_if->dev_if;
    gahbcfg_data_t  ahbcfg = {.d32 = 0 };
    gusbcfg_data_t  usbcfg = {.d32 = 0 };
    gi2cctl_data_t  i2cctl = {.d32 = 0 };

    DWC_DEBUGPL(DBG_CILV, "dwc_otg_core_init(%p)\n", _core_if);

    /*
     * Reset the Controller 
     */
    dwc_otg_core_reset(_core_if);

    _core_if->total_fifo_size = _core_if->hwcfg3.b.dfifo_depth;
    _core_if->rx_fifo_size = dwc_read_reg32(&global_regs->grxfsiz);
    _core_if->nperio_tx_fifo_size = dwc_read_reg32(&global_regs->gnptxfsiz) >> 16;

    DWC_DEBUGPL(DBG_CIL, "Total FIFO SZ=%d\n", _core_if->total_fifo_size);
    DWC_DEBUGPL(DBG_CIL, "Rx FIFO SZ=%d\n", _core_if->rx_fifo_size);
    DWC_DEBUGPL(DBG_CIL, "NP Tx FIFO SZ=%d\n", _core_if->nperio_tx_fifo_size);

    /*
     * This programming sequence needs to happen in FS mode before any other
     * programming occurs 
     */
    if ((_core_if->core_params->speed == DWC_SPEED_PARAM_FULL) &&
        (_core_if->core_params->phy_type == DWC_PHY_TYPE_PARAM_FS)) {
        /*
         * If FS mode with FS PHY 
         */

        /*
         * core_init() is now called on every switch so only call the
         * following for the first time through. 
         */
        if (!_core_if->phy_init_done) {
            _core_if->phy_init_done = 1;
            DWC_DEBUGPL(DBG_CIL, "FS_PHY detected\n");
            usbcfg.d32 = dwc_read_reg32(&global_regs->gusbcfg);
            usbcfg.b.physel = 1;
            dwc_write_reg32(&global_regs->gusbcfg, usbcfg.d32);

            /*
             * Reset after a PHY select 
             */
            dwc_otg_core_reset(_core_if);
        }

        /*
         * Program DCFG.DevSpd or HCFG.FSLSPclkSel to 48Mhz in FS. Also do
         * this on HNP Dev/Host mode switches (done in dev_init and
         * host_init). 
         */
        if (dwc_otg_is_host_mode(_core_if)) {
            init_fslspclksel(_core_if);
        } else {
            init_devspd(_core_if);
        }

        if (_core_if->core_params->i2c_enable) {
            DWC_DEBUGPL(DBG_CIL, "FS_PHY Enabling I2c\n");
            /*
             * Program GUSBCFG.OtgUtmifsSel to I2C 
             */
            usbcfg.d32 = dwc_read_reg32(&global_regs->gusbcfg);
            usbcfg.b.otgutmifssel = 1;
            dwc_write_reg32(&global_regs->gusbcfg, usbcfg.d32);

            /*
             * Program GI2CCTL.I2CEn 
             */
            i2cctl.d32 = dwc_read_reg32(&global_regs->gi2cctl);
            i2cctl.b.i2cdevaddr = 1;
            i2cctl.b.i2cen = 0;
            dwc_write_reg32(&global_regs->gi2cctl, i2cctl.d32);
            i2cctl.b.i2cen = 1;
            dwc_write_reg32(&global_regs->gi2cctl, i2cctl.d32);
        }

    }
    /*
     * endif speed == DWC_SPEED_PARAM_FULL 
     */
    else {
        /*
         * High speed PHY. 
         */
        if (!_core_if->phy_init_done) {
            _core_if->phy_init_done = 1;
            /*
             * HS PHY parameters.  These parameters are preserved during soft 
             * reset so only program the first time.  Do a soft reset
             * immediately after setting phyif.  
             */
            usbcfg.b.ulpi_utmi_sel = _core_if->core_params->phy_type;
            if (usbcfg.b.ulpi_utmi_sel == 1) {
                /*
                 * ULPI interface 
                 */
                usbcfg.b.phyif = 0;
                usbcfg.b.ddrsel = _core_if->core_params->phy_ulpi_ddr;
            } else {
                /*
                 * UTMI+ interface 
                 */
                if (_core_if->core_params->phy_utmi_width == 16) {
                    usbcfg.b.phyif = 1;
                } else {
                    usbcfg.b.phyif = 0;
                }
            }

            dwc_write_reg32(&global_regs->gusbcfg, usbcfg.d32);

            /*
             * Reset after setting the PHY parameters 
             */
            dwc_otg_core_reset(_core_if);
        }
    }

    /*
     * Program the GAHBCFG Register.
     */
    switch (_core_if->hwcfg2.b.architecture) {

    case DWC_SLAVE_ONLY_ARCH:
        DWC_DEBUGPL(DBG_CIL, "Slave Only Mode\n");
        ahbcfg.b.nptxfemplvl_txfemplvl = DWC_GAHBCFG_TXFEMPTYLVL_HALFEMPTY;
        ahbcfg.b.ptxfemplvl = DWC_GAHBCFG_TXFEMPTYLVL_HALFEMPTY;
        _core_if->dma_enable = 0;
        break;

    case DWC_EXT_DMA_ARCH:
        DWC_DEBUGPL(DBG_CIL, "External DMA Mode\n");
        ahbcfg.b.hburstlen = DWC_GAHBCFG_INT_DMA_BURST_INCR8;
        _core_if->dma_enable = (_core_if->core_params->dma_enable != 0);
        break;

    case DWC_INT_DMA_ARCH:
        DWC_DEBUGPL(DBG_CIL, "Internal DMA Mode\n");
        // ahbcfg.b.hburstlen =
        // _core_if->core_params->dma_burst_size;//config by out world
        ahbcfg.b.hburstlen = DWC_GAHBCFG_INT_DMA_BURST_INCR8;   // config by
        // out world
        _core_if->dma_enable = (_core_if->core_params->dma_enable != 0);
        break;

    }

    ahbcfg.b.dmaenable = _core_if->dma_enable;
    dwc_write_reg32(&global_regs->gahbcfg, ahbcfg.d32);

    // _core_if->en_multiple_tx_fifo = _core_if->hwcfg4.b.ded_fifo_en;


    /*
     * Program the GUSBCFG register. 
     */
    usbcfg.d32 = dwc_read_reg32(&global_regs->gusbcfg);

    switch (_core_if->hwcfg2.b.op_mode) {
    case DWC_MODE_HNP_SRP_CAPABLE:
        usbcfg.b.hnpcap = (_core_if->core_params->otg_cap == DWC_OTG_CAP_PARAM_HNP_SRP_CAPABLE);
        usbcfg.b.srpcap = (_core_if->core_params->otg_cap != DWC_OTG_CAP_PARAM_NO_HNP_SRP_CAPABLE);
        break;

    case DWC_MODE_SRP_ONLY_CAPABLE:
        usbcfg.b.hnpcap = 0;
        usbcfg.b.srpcap = (_core_if->core_params->otg_cap != DWC_OTG_CAP_PARAM_NO_HNP_SRP_CAPABLE);
        break;

    case DWC_MODE_NO_HNP_SRP_CAPABLE:
        usbcfg.b.hnpcap = 0;
        usbcfg.b.srpcap = 0;
        break;

    case DWC_MODE_SRP_CAPABLE_DEVICE:
        usbcfg.b.hnpcap = 0;
        usbcfg.b.srpcap = (_core_if->core_params->otg_cap != DWC_OTG_CAP_PARAM_NO_HNP_SRP_CAPABLE);
        break;

    case DWC_MODE_NO_SRP_CAPABLE_DEVICE:
        usbcfg.b.hnpcap = 0;
        usbcfg.b.srpcap = 0;
        break;

    case DWC_MODE_SRP_CAPABLE_HOST:
        usbcfg.b.hnpcap = 0;
        usbcfg.b.srpcap = (_core_if->core_params->otg_cap != DWC_OTG_CAP_PARAM_NO_HNP_SRP_CAPABLE);
        break;

    case DWC_MODE_NO_SRP_CAPABLE_HOST:
        usbcfg.b.hnpcap = 0;
        usbcfg.b.srpcap = 0;
        break;
    }

    dwc_write_reg32(&global_regs->gusbcfg, usbcfg.d32);

    /*
     * Enable common interrupts 
     */
    dwc_otg_enable_common_interrupts(_core_if);

    /*
     * Do device or host intialization based on mode during PCD and HCD
     * initialization 
     */
    if (dwc_otg_is_host_mode(_core_if)) {
        DWC_DEBUGPL(DBG_ANY, "Host Mode\n");
        _core_if->op_state = A_HOST;
    } else {
        DWC_DEBUGPL(DBG_ANY, "Device Mode\n");
        _core_if->op_state = B_PERIPHERAL;
#ifdef DWC_DEVICE_ONLY
        dwc_otg_core_dev_init(_core_if);
#endif
    }

}
/** 
 * This function disables the Host Mode interrupts.
 *
 * @param _core_if Programming view of DWC_otg controller
 */
void
dwc_otg_disable_host_interrupts(dwc_otg_core_if_t * _core_if)
{
    dwc_otg_core_global_regs_t *global_regs = _core_if->core_global_regs;

    DWC_DEBUGPL(DBG_CILV, "%s()\n", __func__);

    dwc_write_reg32 (&global_regs->gintmsk, 0);
    dwc_modify_reg32 (&global_regs->gahbcfg, 1, 0);

}

/**
 * Halts the DWC_otg host mode operations in a clean manner. USB transfers are
 * stopped.
 */
void
dwc_otg_hcd_stop(dwc_otg_core_if_t * core_if)
{
    hprt0_data_t    hprt0 = {.d32 = 0 };
    pcgcctl_data_t hcgcctl = {.d32 = 0};

    if (core_if == 0)
        return;

    DBG("DWC OTG HCD STOP\n");

    /*
     * Turn off all host-specific interrupts. 
     */
    dwc_otg_disable_host_interrupts(core_if);

    /*
     * The root hub should be disconnected before this function is called.
     * The disconnect will clear the QTD lists (via ..._hcd_urb_dequeue)
     * and the QH lists (via ..._hcd_endpoint_disable).
     */

    /*
     * Turn off the vbus power 
     */
    DBG("PortPower off\n");
    hprt0.b.prtpwr = 0;
    dwc_write_reg32(&core_if->host_if->hprt0, hprt0.d32);

    hcgcctl.b.stoppclk  = 1;
    hcgcctl.b.gatehclk = 1;
    hcgcctl.b.pwrclmp = 1;
    //hcgcctl.b.rstpdwnmodule = 1;
	//printf("core_if->pcgcctl  : 0x%x, & = 0x%x\n",core_if->pcgcctl,&core_if->pcgcctl );
	
    dwc_write_reg32(core_if->pcgcctl,hcgcctl.d32);
    return;
}


/*
 * --- API functions ------------------------------------------------------ 
 */
int
submit_int_msg(struct usb_device *dev, unsigned long pipe, void *buffer, int len, int interval)
{
    DBG("dev=%p pipe=%#lx buf=%p size=%d int=%d", dev, pipe, buffer, len, interval);

    return -1;
}
static inline int
max_transfer_len(struct usb_device *dev, unsigned long pipe)
{
    unsigned        mpck = usb_maxpacket(dev, pipe);

    /*
     * One PTD can transfer 1023 bytes but try to always transfer multiples
     * of endpoint buffer size 
     */
    return 1023 / mpck * mpck;
}

/**
 * Clears the transfer state for a host channel. This function is normally
 * called after a transfer is done and the host channel is being released.
 *
 * @param _core_if Programming view of DWC_otg controller.
 * @param _hc Identifies the host channel to clean up.
 */
void
dwc_otg_hc_cleanup(dwc_otg_core_if_t * _core_if, int hc_num)
{
    dwc_otg_hc_regs_t *hc_regs;

    /*
     * Clear channel interrupt enables and any unhandled channel interrupt
     * conditions.
     */
    hc_regs = _core_if->host_if->hc_regs[hc_num];
    dwc_write_reg32(&hc_regs->hcintmsk, 0);
    dwc_write_reg32(&hc_regs->hcint, 0xFFFFFFFF);
    dwc_write_reg32(&hc_regs->hctsiz, 0);
    dwc_write_reg32(&hc_regs->hcdma, 0);
    dwc_write_reg32(&hc_regs->hcchar, 0);

    flush_cpu_cache();

}

static int
dwc_otg_interrupt(dwc_otg_core_if_t * _core_if, int is_setup,int hcnum)
{
    gintsts_data_t  gintsts;
    dwc_otg_hc_regs_t *hc_regs;
    hctsiz_data_t   hctsiz;
    hcint_data_t    hcint;
    hcchar_data_t   hcchar;
    hcintmsk_data_t hcintmsk;
    // dwc_otg_host_if_t *host_if ;
//    int             hcnum = 0;
    dwc_otg_host_if_t *host_if = _core_if->host_if;

    gintsts.d32 = dwc_read_reg32(&_core_if->core_global_regs->gintsts);
    hc_regs = host_if->hc_regs[hcnum];
    hcint.d32 = dwc_read_reg32(&hc_regs->hcint);

    if (gintsts.b.hcintr) {
        //DBG("dwc_otg_interrupt: hcintr interrupt!\n");
        if (hcint.b.xfercomp) {
            hctsiz.d32 = dwc_read_reg32(&hc_regs->hctsiz);
            dwc_otg_hc_cleanup(_core_if, 0);
            if(is_setup)
                return 8;
            return (_core_if->transfer_size);// - hctsiz.b.xfersize);
        }
        if (hcint.b.ahberr) {
            ERR("dwc_otg_interrupt: ahberr interrupt!\n");
            hcintmsk.d32 = 0;
            hcintmsk.b.chhltd = 1;
            dwc_write_reg32(&hc_regs->hcintmsk, hcintmsk.d32);
            dwc_write_reg32(&hc_regs->hcint, ~hcintmsk.d32);
            hcchar.d32 = dwc_read_reg32(&hc_regs->hcchar);
            if (hcchar.b.chen == 0)
                return -1;
            else {
                hcchar.b.chen = 1;
                hcchar.b.chdis = 1;
                dwc_write_reg32(&hc_regs->hcchar, hcchar.d32);
            }

            return -1;

        }
        if (hcint.b.nyet) {
            DBG("dwc_otg_interrupt: nyet interrupt!\n");
            _core_if->host_if->do_ping = 1;	
            return -1;

        }
        if (hcint.b.ack) {
            DBG("dwc_otg_interrupt: ack interrupt!\n");
            _core_if->host_if->do_ping = 0;	
            return -1;

        }	
        if (hcint.b.xacterr) {
            ERR("dwc_otg_interrupt: xacterr interrupt!\n");
            return -1;

        }
        if (hcint.b.bblerr) {
            ERR("dwc_otg_interrupt: bblerr interrupt!\n");
            return -1;
        }
        if (hcint.b.datatglerr) {
            ERR("dwc_otg_interrupt: datatglerr interrupt!\n");
            return -1;
        }
    }
    if (gintsts.b.ptxfempty) {
        //DBG("dwc_otg_interrupt: ptxfempty interrupt!\n");
        return -1;
    }
    return -1;

}

/*
 * Do an USB transfer , dir = 0, out ;  dir = 1 , in
 */
static int
dwc_otg_submit_job(struct usb_device *dev, unsigned long pipe, int dir, void *buffer, int len,
                   int is_setup)
{

    dwc_otg_core_if_t *core_if = dwc_otg_dev.core_if;
    dwc_otg_hc_regs_t *hc_regs;
    hcchar_data_t   hcchar;
    hcsplt_data_t   hcsplt;
    hctsiz_data_t   hctsiz;
    int             devnum = usb_pipedevice(pipe);
    int             epnum = usb_pipeendpoint(pipe);
    int             dir_in = usb_pipein(pipe);
    int             toggle,chg_flag = 0;
    static int             hcnum = 0;
    hcintmsk_data_t hcintmsk;
    dwc_otg_host_if_t *host_if = core_if->host_if;
    int stat, timeout;
    int allchannel_int_mask;
    hcint_data_t    hcint;
    dwc_otg_host_global_regs_t *host_global_regs = host_if->host_global_regs;


    int             type = usb_pipetype(pipe);
    if (len >= 4096) {
        ERR("Too big job!\n");
        dev->status = USB_ST_CRC_ERR;
        return -E2BIG;
    }

    if (type == PIPE_ISOCHRONOUS) {
        ERR("isochronous transfers not supported");
        dev->status = USB_ST_CRC_ERR;
        return -EOPNOTSUPP;
    }

    //hcnum++;
    if(hcnum > 6) hcnum = 0;
    hc_regs = host_if->hc_regs[hcnum];
    
    /*
     * init hc char 
     */
    hcchar.d32 = 0;
    hcchar.b.devaddr = devnum;
    hcchar.b.epnum = epnum;
    hcchar.b.epdir = dir;
    hcchar.b.lspddev = 0;
    
    /* set hc interrupt mask */
    hcintmsk.d32 = dwc_read_reg32(&hc_regs->hcintmsk);
    //hcintmsk.b.xfercompl = 1;
    hcintmsk.d32 = 6;
    dwc_write_reg32(&hc_regs->hcintmsk, hcintmsk.d32 );
    
    /* clear int status*/
    hcint.d32 = dwc_read_reg32(&hc_regs->hcint);

    dwc_write_reg32(&hc_regs->hcint, hcint.d32 );

    /* set all channel interrupt mask */
    allchannel_int_mask = dwc_read_reg32(&host_global_regs->haintmsk);
    allchannel_int_mask |= 1;
    dwc_write_reg32(&host_global_regs->haintmsk, allchannel_int_mask);
    
    switch (usb_pipetype(pipe)) {
    case PIPE_CONTROL:
        hcchar.b.eptype = DWC_OTG_EP_TYPE_CONTROL;
        break;
    case PIPE_BULK:
        hcchar.b.eptype = DWC_OTG_EP_TYPE_BULK;
        break;
    case PIPE_INTERRUPT:
    case PIPE_ISOCHRONOUS:
        ERR("dwc_otg_submit_job: not support iso or interrupt transfer!\n");
        return -1;
    }

    hcchar.b.mps = usb_maxpacket(dev, pipe);

    dwc_write_reg32(&hc_regs->hcchar, hcchar.d32);
    hcsplt.d32 = 0;

    dwc_write_reg32(&hc_regs->hcsplt, hcsplt.d32);
    flush_cpu_cache();


    hctsiz.d32 = 0;

    hctsiz.b.pktcnt = (len  + usb_maxpacket(dev, pipe) - 1)/ usb_maxpacket(dev, pipe);
    if(hctsiz.b.pktcnt < 1)
        hctsiz.b.pktcnt = 1;
    if(hctsiz.b.pktcnt & 0x1)
	chg_flag = 1;
    toggle = usb_gettoggle(dev, epnum, dir);
    DBG("packet is :%d, max packet size = %d,toggle %d\n", 
		hctsiz.b.pktcnt, usb_maxpacket(dev, pipe), toggle);

    switch (toggle) {
    case 0:
        hctsiz.b.pid = DWC_OTG_HC_PID_DATA0;
        break;
    case 1:
        hctsiz.b.pid = DWC_OTG_HC_PID_DATA1;
        break;
    default:
        ERR("error toggle!\n");
        return -EPROTO;
    }
    if (is_setup)
        hctsiz.b.pid = DWC_OTG_HC_PID_SETUP;
    
    core_if->transfer_size = len;
    hctsiz.b.xfersize = len;

    if(!is_setup && dir == 0 && core_if->host_if->do_ping)
        hctsiz.b.dopng = 1;
    else
        hctsiz.b.dopng = 0;

    dwc_write_reg32(&hc_regs->hctsiz, hctsiz.d32);


    dwc_write_reg32(&hc_regs->hcdma, (uint32_t) buffer);
    flush_cpu_cache();

    hcchar.d32 = dwc_read_reg32(&hc_regs->hcchar);

    hcchar.b.multicnt = 1;      // for no split transfer set this bit 1
    hcchar.b.chen = 1;
    hcchar.b.chdis = 0;

    dwc_write_reg32(&hc_regs->hcchar, hcchar.d32);

    if (usb_pipebulk(pipe))
        timeout = 10000;
    else
        timeout = 1000;

    /*
     * Wait for it to complete 
     */
    for (;;) {
        dwc_udelay(2);
        /*
         * Check whether the controller is done 
         */
        stat = dwc_otg_interrupt(core_if, is_setup,hcnum);

        if (stat >= 0){
    		if(chg_flag) usb_dotoggle(dev, epnum, dir);
		break;
        }
            

        /*
         * Check the timeout 
         */
        if (--timeout)
            udelay(100);//dwc_udelay(100);
        else {
            DBG("CTL:TIMEOUT ");
            stat = -ETIME;
            break;
        }
    }

    return stat;
}
#define CTL_RETRY 5
int
submit_control_msg(struct usb_device *dev, unsigned long pipe, void *buffer,
                   int len, struct devrequest *setup)
{
    int             epnum = usb_pipeendpoint(pipe);
    int             dir_in = usb_pipein(pipe);
    int             done;
    int             ret,retry;
    //char            *buffer;    
    dwc_otg_core_if_t *core_if = dwc_otg_dev.core_if;
    
    /*
     * Setup phase 
     */
    retry = 0;
SETUP_RETRY:
    DBG("--- SETUP PHASE  --------------------try: %d\n",retry);
    DBG("dev=%ld ep=%ld buf=%x size=%d %s req:%d",
        usb_pipedevice(pipe), usb_pipeendpoint(pipe), buffer, len, (dir_in?"dir_in":"dir_out"),setup->request);

    usb_settoggle(dev, epnum, 1, 0);
    ret = dwc_otg_submit_job(dev, pipe, 0, setup, sizeof(struct devrequest), 1);
    if (ret < 0) {
       if(retry++ < CTL_RETRY)
            goto SETUP_RETRY;
	else{
           DBG("control setup phase error (ret = %d)", ret);
           return -1;
       }
    }

    /*
     * Data phase 
     */
    retry = 0;
DATA_RETRY:
    DBG("--- DATA PHASE   --------------------try: %d\n",retry);

   
    done = 0;
    usb_settoggle(dev, epnum, dir_in, 1);
    if (done < len) {

       wait_ms(10);
	 
        ret = dwc_otg_submit_job(dev, pipe,
                                 dir_in,
                                 (__u8 *) buffer, len, 0);
        
        if (ret < 0) {
                if(retry++ < CTL_RETRY)
                        goto DATA_RETRY;
                else{
                        DBG("control data phase error (ret = %d)", ret);
                        return -1;
                }
        }
        //if (!dir_in && ret < usb_maxpacket(dev, pipe))  /* short packet */
        //    break;

        done += ret;
    }

    /*
     * Status phase 
     */
    retry = 0;
STATUS_RETRY:
    DBG("--- STATUS PHASE -------------------try: %d\n",retry);
    wait_ms(10);

    usb_settoggle(dev, epnum, !dir_in, 1);
    ret = dwc_otg_submit_job(dev, pipe, !dir_in, NULL, 0, 0);
    if (ret < 0) {
          if(retry++ < CTL_RETRY)
               goto STATUS_RETRY;
          else{
               DBG("control status phase error (ret = %d)", ret);
               return -1;
          }
    }

    dev->act_len = done;
    DBG("return size = %d\n", done);
    dev->status = 0;

    return done;
}


int
submit_bulk_msg(struct usb_device *dev, unsigned long pipe, void *buf, int len)
{
    int             dir_out = usb_pipeout(pipe);
    int             max = max_transfer_len(dev, pipe);
    int             epnum = usb_pipeendpoint(pipe);

    int             done;
    int             ret;
    int             toggle;
    int             retry;
    char            *buffer=NULL;
    dwc_otg_core_if_t *core_if = dwc_otg_dev.core_if;

    DBG("--- BULK -----------------------------------------\n");
    DBG("dev=%ld ep=%ld buf=%x size=%d %s",
       usb_pipedevice(pipe), usb_pipeendpoint(pipe), buf, len, (dir_out?"dir_out":"dir_in"));
    if(len == 31) 
		DBG("scsi cmd from CBW: 0x%X\n",*(((unsigned char*)buf)+15));

    done = 0;
    if(dir_out) {
        memcpy(core_if->temp_buffer, buf, len);
    }
    buffer = core_if->temp_buffer;
    retry = 5;
    while (done < len) {
RETRY: 
       dwc_udelay(10);
	
        ret = dwc_otg_submit_job(dev, pipe,
                                 !dir_out,
                                 (__u8 *) buffer + done, max > len - done ? len - done : max, 0);
        if (ret < 0 ) {
            if(retry--){
                DBG("error on bulk message, retry: %d",retry);
                goto RETRY;
            }else{
                ERR("error on bulk message (ret = %d)", ret);
                return -1;
            }
        }       

        done += ret;

        //usb_dotoggle(dev, epnum, !dir_out);
        
        if (!dir_out && ret < max)      /* short packet */
            break;
        
    }
    DBG("return size = %d\n", done);
    if(!dir_out) {
        memcpy(buf, buffer, len);
    }
    dev->act_len = done;
    dev->status = 0;

    return 0;
}
/**
 * Flush a Tx FIFO.
 *
 * @param _core_if Programming view of DWC_otg controller.
 * @param _num Tx FIFO to flush.
 */
extern void
dwc_otg_flush_tx_fifo(dwc_otg_core_if_t * _core_if, const int _num)
{
    dwc_otg_core_global_regs_t *global_regs = _core_if->core_global_regs;
    grstctl_t       greset = { 0 };
    int             count = 0;

    DWC_DEBUGPL((DBG_CIL | DBG_PCDV), "Flush Tx FIFO %d\n", _num);

    greset.b.txfflsh = 1;
    greset.b.txfnum = _num;
    dwc_write_reg32(&global_regs->grstctl, greset.d32);

    do {
        greset.d32 = dwc_read_reg32(&global_regs->grstctl);
        if (++count > 10000) {
            WARN("%s() HANG! GRSTCTL=%0x GNPTXSTS=0x%08x\n",
                 "dwc_otg_flush_tx_fifo", greset.d32, dwc_read_reg32(&global_regs->gnptxsts));
            break;
        }

    } while (greset.b.txfflsh == 1);
    /*
     * Wait for 3 PHY Clocks
     */
    wait_ms(1);
}

/**
 * Flush Rx FIFO.
 *
 * @param _core_if Programming view of DWC_otg controller.
 */
extern void
dwc_otg_flush_rx_fifo(dwc_otg_core_if_t * _core_if)
{
    dwc_otg_core_global_regs_t *global_regs = _core_if->core_global_regs;
    volatile grstctl_t greset = { 0 };
    int             count = 0;

    DWC_DEBUGPL((DBG_CIL | DBG_PCDV), "%s\n", "dwc_otg_flush_rx_fifo");
    /*
     * 
     */
    greset.b.rxfflsh = 1;
    dwc_write_reg32(&global_regs->grstctl, greset.d32);

    do {
        greset.d32 = dwc_read_reg32(&global_regs->grstctl);
        if (++count > 10000) {
            WARN("%s() HANG! GRSTCTL=%0x\n", "dwc_otg_flush_rx_fifo", greset.d32);
            break;
        }
    } while (greset.b.rxfflsh == 1);
    /*
     * Wait for 3 PHY Clocks
     */
    wait_ms(1);
}
static void
dwc_otg_set_vbus_power(dwc_otg_core_if_t * _core_if, int is_power_on)
{
    bsp_set_dwc_chip_0_gpio_power(1);
}
/**
 * This function Reads HPRT0 in preparation to modify.  It keeps the
 * WC bits 0 so that if they are read as 1, they won't clear when you
 * write it back 
 */
static inline   uint32_t
dwc_otg_read_hprt0(dwc_otg_core_if_t * _core_if)
{
    hprt0_data_t    hprt0;
    hprt0.d32 = dwc_read_reg32(_core_if->host_if->hprt0);
    hprt0.b.prtena = 0;
    hprt0.b.prtconndet = 0;
    hprt0.b.prtenchng = 0;
    hprt0.b.prtovrcurrchng = 0;
    return hprt0.d32;
}
/** 
 * This function enables the Host mode interrupts.
 *
 * @param _core_if Programming view of DWC_otg controller
 */
void
dwc_otg_enable_host_interrupts(dwc_otg_core_if_t * _core_if)
{
    dwc_otg_core_global_regs_t *global_regs = _core_if->core_global_regs;
    gintmsk_data_t  intr_mask = { 0 };

    DWC_DEBUGPL(DBG_CIL, "%s()\n", "dwc_otg_enable_host_interrupts");

    /*
     * Disable all interrupts. 
     */
    dwc_write_reg32(&global_regs->gintmsk, 0);

    /*
     * Clear any pending interrupts. 
     */
    dwc_write_reg32(&global_regs->gintsts, 0xFFFFFFFF);

    /*
     * Enable the common interrupts 
     */
    dwc_otg_enable_common_interrupts(_core_if);

    /*
     * Enable host mode interrupts without disturbing common
     * interrupts.
     */
#ifndef NO_HOST_SOF
    intr_mask.b.sofintr = 1;
#endif
    intr_mask.b.portintr = 1;
    intr_mask.b.hcintr = 1;

    dwc_modify_reg32(&global_regs->gintmsk, intr_mask.d32, intr_mask.d32);
}

/**
 * This function initializes the DWC_otg controller registers for
 * host mode.
 *
 * This function flushes the Tx and Rx FIFOs and it flushes any entries in the
 * request queues. Host channels are reset to ensure that they are ready for
 * performing transfers.
 *
 * @param _core_if Programming view of DWC_otg controller
 *
 */
void
dwc_otg_core_host_init(dwc_otg_core_if_t * _core_if)
{
    dwc_otg_core_global_regs_t *global_regs = _core_if->core_global_regs;
    dwc_otg_host_if_t *host_if = _core_if->host_if;
    dwc_otg_core_params_t *params = _core_if->core_params;
    hprt0_data_t    hprt0 = { 0 };
    fifosize_data_t nptxfifosize;
    fifosize_data_t ptxfifosize;
    int             i;
    hcchar_data_t   hcchar;
    hcfg_data_t     hcfg;
    dwc_otg_hc_regs_t *hc_regs;
    int             num_channels;
    gotgctl_data_t  gotgctl = { 0 };

    DWC_DEBUGPL(DBG_CILV, "%s(%p)\n", "dwc_otg_core_host_init", _core_if);

    /*
     * Restart the Phy Clock 
     */
    dwc_write_reg32(_core_if->pcgcctl, 0);

    /*
     * Initialize Host Configuration Register 
     */
    init_fslspclksel(_core_if);
    if (_core_if->core_params->speed == DWC_SPEED_PARAM_FULL) {
        hcfg.d32 = dwc_read_reg32(&host_if->host_global_regs->hcfg);
        hcfg.b.fslssupp = 1;
        dwc_write_reg32(&host_if->host_global_regs->hcfg, hcfg.d32);
    }

    /*
     * Configure data FIFO sizes 
     */
    if (_core_if->hwcfg2.b.dynamic_fifo && params->enable_dynamic_fifo) {
        DWC_DEBUGPL(DBG_CIL, "Total FIFO Size=%d\n", _core_if->total_fifo_size);
        DWC_DEBUGPL(DBG_CIL, "Rx FIFO Size=%d\n", params->host_rx_fifo_size);
        DWC_DEBUGPL(DBG_CIL, "NP Tx FIFO Size=%d\n", params->host_nperio_tx_fifo_size);
        DWC_DEBUGPL(DBG_CIL, "P Tx FIFO Size=%d\n", params->host_perio_tx_fifo_size);

        /*
         * Rx FIFO 
         */
        DWC_DEBUGPL(DBG_CIL, "initial grxfsiz=%08x\n", dwc_read_reg32(&global_regs->grxfsiz));
        dwc_write_reg32(&global_regs->grxfsiz, params->host_rx_fifo_size);
        DWC_DEBUGPL(DBG_CIL, "new grxfsiz=%08x\n", dwc_read_reg32(&global_regs->grxfsiz));

        /*
         * Non-periodic Tx FIFO 
         */
        DWC_DEBUGPL(DBG_CIL, "initial gnptxfsiz=%08x\n", dwc_read_reg32(&global_regs->gnptxfsiz));
        nptxfifosize.b.depth = params->host_nperio_tx_fifo_size;
        nptxfifosize.b.startaddr = params->host_rx_fifo_size;
        dwc_write_reg32(&global_regs->gnptxfsiz, nptxfifosize.d32);
        DWC_DEBUGPL(DBG_CIL, "new gnptxfsiz=%08x\n", dwc_read_reg32(&global_regs->gnptxfsiz));

        /*
         * Periodic Tx FIFO 
         */
        DWC_DEBUGPL(DBG_CIL, "initial hptxfsiz=%08x\n", dwc_read_reg32(&global_regs->hptxfsiz));
        ptxfifosize.b.depth = params->host_perio_tx_fifo_size;
        ptxfifosize.b.startaddr = nptxfifosize.b.startaddr + nptxfifosize.b.depth;
        dwc_write_reg32(&global_regs->hptxfsiz, ptxfifosize.d32);
        DWC_DEBUGPL(DBG_CIL, "new hptxfsiz=%08x\n", dwc_read_reg32(&global_regs->hptxfsiz));
    }

    /*
     * Clear Host Set HNP Enable in the OTG Control Register 
     */
    gotgctl.b.hstsethnpen = 1;
    dwc_modify_reg32(&global_regs->gotgctl, gotgctl.d32, 0);

    /*
     * Make sure the FIFOs are flushed. 
     */
    dwc_otg_flush_tx_fifo(_core_if, 0x10 /* all Tx FIFOs */ );
    dwc_otg_flush_rx_fifo(_core_if);

    /*
     * Flush out any leftover queued requests. 
     */
    num_channels = _core_if->core_params->host_channels;
    for (i = 0; i < num_channels; i++) {
        hc_regs = _core_if->host_if->hc_regs[i];
        hcchar.d32 = dwc_read_reg32(&hc_regs->hcchar);
        hcchar.b.chen = 0;
        hcchar.b.chdis = 1;
        hcchar.b.epdir = 0;
        dwc_write_reg32(&hc_regs->hcchar, hcchar.d32);
    }

    /*
     * Halt all channels to put them into a known state. 
     */
    for (i = 0; i < num_channels; i++) {
        int             count = 0;
        hc_regs = _core_if->host_if->hc_regs[i];
        hcchar.d32 = dwc_read_reg32(&hc_regs->hcchar);
        hcchar.b.chen = 1;
        hcchar.b.chdis = 1;
        hcchar.b.epdir = 0;
        dwc_write_reg32(&hc_regs->hcchar, hcchar.d32);
        DWC_DEBUGPL(DBG_HCDV, "%s: Halt channel %d\n", "dwc_otg_core_host_init", i);
        do {
            hcchar.d32 = dwc_read_reg32(&hc_regs->hcchar);
            if (++count > 1000) {
                ERR("%s: Unable to clear halt on channel %d\n", "dwc_otg_core_host_init", i);
                break;
            }
        } while (hcchar.b.chen);
    }

    /*
     * Turn on the vbus power. 
     */
    ERR("Init: Port Power? op_state=%d\n", _core_if->op_state);
    if (_core_if->op_state == A_HOST) {
        hprt0.d32 = dwc_otg_read_hprt0(_core_if);
        ERR("Init: Power Port (%d)\n", hprt0.b.prtpwr);
        if (hprt0.b.prtpwr == 0) {
            hprt0.b.prtpwr = 1;
            dwc_write_reg32(host_if->hprt0, hprt0.d32);
            /*
             * pull gpio to switch power 
             */
            dwc_otg_set_vbus_power(_core_if, 1);
        }
    } else {
        dwc_otg_set_vbus_power(_core_if, 0);    // Power off VBus
    }

    dwc_otg_enable_host_interrupts(_core_if);
}

static int      dwc_otg_hcd_enable = 0;


static void
dwc_otg_reset_port(dwc_otg_core_if_t * _core_if)
{
    hprt0_data_t    hprt0;
    hprt0.d32 = dwc_otg_read_hprt0(_core_if);
    hprt0.b.prtrst = 1;
    dwc_write_reg32(_core_if->host_if->hprt0, hprt0.d32);

    wait_ms(60);

    hprt0.b.prtrst = 0;
    dwc_write_reg32(_core_if->host_if->hprt0, hprt0.d32);
    wait_ms(20);

}
static void
dwc_otg_port_init(dwc_otg_core_if_t * _core_if)
{
    hprt0_data_t    hprt0;
    hprt0.d32 = 0;

    hprt0.d32 = dwc_otg_read_hprt0(_core_if);

    hprt0_data_t    hprt0_modify;

    /*
     * set port power
     */
    hprt0.b.prtpwr = 1;
    dwc_write_reg32(_core_if->host_if->hprt0, hprt0.d32);

    hprt0_modify.b.prtena = 0;
    hprt0_modify.b.prtconndet = 0;
    hprt0_modify.b.prtenchng = 0;
    hprt0_modify.b.prtovrcurrchng = 0;

    wait_ms(30);


    hprt0.d32 = dwc_read_reg32(_core_if->host_if->hprt0);
    DBG("%s port data is %d\n", __func__, hprt0.d32);

    hprt0_modify.d32 = dwc_read_reg32(_core_if->host_if->hprt0);

    if (hprt0.b.prtconndet) {
        /*
         * clear detect intr
         */
        wait_ms(30);
        dwc_write_reg32(_core_if->host_if->hprt0, hprt0_modify.d32);
        wait_ms(30);
        /*
         * reset port
         */
        dwc_otg_reset_port(_core_if);
        hprt0_modify.b.prtconndet = 1;


    }
    /*
     * Determine if the connected device is a high/full/low speed device 
     */
    hprt0.d32 = dwc_read_reg32(_core_if->host_if->hprt0);

    if (hprt0.b.prtenchng) {
        hprt0.d32 = dwc_read_reg32(_core_if->host_if->hprt0);

        hprt0_modify.b.prtena = 0;
        hprt0_modify.b.prtconndet = 0;
        hprt0_modify.b.prtenchng = 0;
        hprt0_modify.b.prtovrcurrchng = 0;
        hprt0_modify.b.prtenchng = 1;
        dwc_write_reg32(_core_if->host_if->hprt0, hprt0_modify.d32);

    }

//    _core_if->host_if->do_ping = 1; // init value
    _core_if->host_if->do_ping = 0; // init value

    if (hprt0.b.prtspd == DWC_HPRT0_PRTSPD_LOW_SPEED) {
        INFO("Lowspeed device found !\n");
    } else if (hprt0.b.prtspd == DWC_HPRT0_PRTSPD_FULL_SPEED) {
        INFO("Fullspeed device found !\n");
    } else if (hprt0.b.prtspd == DWC_HPRT0_PRTSPD_HIGH_SPEED) {
        INFO("Highspeed device found !\n");
    }

    /*
     * enable port
     */
}
/*
 * --- Init functions ------------------------------------------------------ 
 */
int
usb_lowlevel_init(void)
{
    dwc_otg_device_t *dwc_otg_device = &dwc_otg_dev;
    int             retval = 0;
    int32_t         snpsid;
    int             i;
    DBG("%s\n", __func__);
    
    set_usb_phy_config(USB_PHY_CLOCK_SEL_XTAL_DIV2);

	CLEAR_CBUS_REG_MASK(USB_ADDR4, 3<<22);
	udelay(100);

	SET_CBUS_REG_MASK(USB_ADDR4, 1<<23);
	udelay(100);

    memset(dwc_otg_device, 0, sizeof(dwc_otg_device_t));

    dwc_otg_device->base = CONFIG_M1_USBPORT_BASE;

    snpsid = dwc_read_reg32((uint32_t *) ((uint8_t *) dwc_otg_device->base + 0x40));

    if ((snpsid & 0xFFFFF000) != 0x4F542000) {
        ERR("Bad value for SNPSID: 0x%08x\n", snpsid);
        retval = -1;
        goto fail;
    }
    dwc_otg_module_params_host.dma_enable = 1;

    dwc_otg_device->core_if = dwc_otg_cil_init(dwc_otg_device->base, &dwc_otg_module_params_host);
    if (dwc_otg_device->core_if == 0) {
        ERR("CIL initialization failed!\n");
        retval = -1;
        goto fail;
    }
    /*
     * Disable the global interrupt until all the interrupt
     * handlers are installed.
     */
    dwc_otg_disable_global_interrupts(dwc_otg_device->core_if);
    /*
     * Initialize the DWC_otg core.
     */
    dwc_otg_core_init(dwc_otg_device->core_if);
    for (i = 0; i < 2; i++) {
        dwc_otg_hc_cleanup(dwc_otg_device->core_if, i);
    }

    dwc_otg_core_host_init(dwc_otg_device->core_if);

    /*
     * Enable the global interrupt after all the interrupt
     * handlers are installed.
     */
    dwc_otg_enable_global_interrupts(dwc_otg_device->core_if);

    dwc_otg_port_init(dwc_otg_device->core_if);

    dwc_otg_hcd_enable = 1;
    dwc_otg_device->disabled = 0;
    return 0;
  fail:
    return -1;
}

int
usb_lowlevel_stop(void)
{
    dwc_otg_core_if_t *core_if = dwc_otg_dev.core_if;

    DBG("%s\n", __func__);

    if(!dwc_otg_hcd_enable)
        return 0;

    dwc_otg_hcd_stop(core_if);
    set_usb_phy_config(0);

    return 0;
}
