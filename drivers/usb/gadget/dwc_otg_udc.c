/* This source partially based on Synopsys HS OTG Linux Software Driver and 
 * documentation
 *
 * (C) Copyright 2011
 * Tianhui.Wang, <tianhui.wang@amlogic.com>, amlogic(shanghai),Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307	 USA
 *
 */
#include <common.h>
#include <config.h>
#include <usbdevice.h>
#include <asm/errno.h>
#include <asm/arch/usb.h>
#include <usb/dwc_otg/dwc_otg_udc.h>
#include <usb/dwc_otg/dwc_otg_platform.h>
#include <usb/dwc_otg/dwc_otg_pcd.h>
#include "ep0.h"

#define flush_cpu_cache()  {}//do{ dcache_flush();}while(0)
//#define __DEBUG_DWC_OTG_UDC__

#define DWC_OTG_ERR(fmt, args...)\
	serial_printf(" %s:ERROR : "fmt,\
				__FUNCTION__, ##args)
#ifdef __DEBUG_DWC_OTG_UDC__
#define DWC_OTG_DBG(fmt,args...)\
		serial_printf(" %s: "fmt,\
				__FUNCTION__, ##args)

#else
#define DWC_OTG_DBG(fmt,args...)
#endif
/*The speed depend on phy setting which is related to DSTS.bit0*/
#define USE_FULL_SPEED

static struct usb_device_instance *udc_device = 0;
dwc_otg_udc_state_t udc_state;
/*used for enumeration*/
static struct urb *ep0_urb = NULL;
dwc_ep_t g_dwc_eps[MAX_NUM_ENDPOINT]; 
static int dwc_otg_pcd_handle_enum_done_intr(void);
static int dwc_otg_pcd_handle_usb_reset_intr(void);
static void dwc_otg_ep_start_transfer(struct usb_endpoint_instance *endpoint ,int is_in);
static int dwc_otg_pcd_setup(struct urb *urb);
static void do_setup_status_phase(struct urb *urb,int is_in);
static struct udc_driver *udc_drv = NULL;

/* start usb gadget */
#include <asm/arch/usb.h>
struct amlogic_usb_config usb_config={
	USB_PHY_CLOCK_SEL_XTAL_DIV2,
	0, // no use if clock sel == xtal or xtal/2
	CONFIG_USBPORT_BASE,
	0,
	0, //set_vbus_power
};
static int dwc_otg_start_clk(void)
{	
	serial_printf("usb agdget start!\nusb-controller name:dwc_otg_usb_pcd\n");
	board_usb_init(&usb_config);
	amlogic_usb_config_t * usb_config;   
   	usb_config = board_usb_start();
    	if(!usb_config && !usb_config->base_addr){    	
    		return -1;      
	}
	return 0;
	
}

static void dwc_otg_udc_state_transition_up (usb_device_state_t initial, usb_device_state_t final)
{
	if (initial < final) {
		switch (initial) {
		case STATE_ATTACHED:
			usbd_device_event_irq (udc_device,DEVICE_HUB_CONFIGURED, 0);
			if (final == STATE_POWERED)
				break;
		case STATE_POWERED:
			usbd_device_event_irq (udc_device, DEVICE_RESET, 0);
			if (final == STATE_DEFAULT)
				break;
		case STATE_DEFAULT:
			usbd_device_event_irq (udc_device, DEVICE_ADDRESS_ASSIGNED, 0);
			if (final == STATE_ADDRESSED)
				break;
		case STATE_ADDRESSED:
			usbd_device_event_irq (udc_device, DEVICE_CONFIGURED,0);
		case STATE_CONFIGURED:
			break;
		default:
			break;
		}
	}
}

/* dev initialization */
static void dwc_otg_enable_common_interrupts(void)
{
        gintmsk_data_t intr_mask = { 0};
        dwc_write_reg32(DWC_REG_GOTGINT, 0xFFFFFFFF);         /* Clear any pending OTG Interrupts */
        dwc_write_reg32(DWC_REG_GINTSTS, 0xFFFFFFFF);         /* Clear any pending interrupts */

        /* Enable the interrupts in the GINTMSK. */
        intr_mask.b.modemismatch = 1;
        intr_mask.b.otgintr = 1;
        intr_mask.b.rxstsqlvl = 1;
        intr_mask.b.conidstschng = 1;
        intr_mask.b.wkupintr = 1;
        intr_mask.b.disconnect = 1;
        intr_mask.b.usbsuspend = 1;
        intr_mask.b.sessreqintr = 1;
        dwc_write_reg32(DWC_REG_GINTMSK, intr_mask.d32);
}

static void dwc_otg_core_reset(void)
{
    grstctl_t greset = {.d32 = 0 };
    int             count = 0;

    do {
        udelay(10);
        greset.d32 = dwc_read_reg32(DWC_REG_GRSTCTL);
        if (++count > 100000) {       
            return;
        }
    }while (greset.b.ahbidle == 0);

    count = 0;
    greset.b.csftrst = 1;
    dwc_write_reg32(DWC_REG_GRSTCTL, greset.d32);
    do {
        greset.d32 = dwc_read_reg32(DWC_REG_GRSTCTL);
        if (++count > 1000000) {            
            break;
        }
    }while (greset.b.csftrst == 1);

    udelay(10000);
}
static void dwc_otg_enable_device_interrupts(void)
{
        gintmsk_data_t intr_mask = {  0};
        dwc_write_reg32( DWC_REG_GINTMSK, 0);
        dwc_write_reg32( DWC_REG_GINTSTS, 0xFFFFFFFF); 

        /* Enable the common interrupts */
        dwc_otg_enable_common_interrupts( );

        /* Enable interrupts */
        intr_mask.b.usbreset = 1;
        intr_mask.b.enumdone = 1;
        intr_mask.b.epmismatch = 1;
        intr_mask.b.inepintr = 1;
        intr_mask.b.outepintr = 1;
        intr_mask.b.erlysuspend = 1;

        dwc_modify_reg32( DWC_REG_GINTMSK, intr_mask.d32, intr_mask.d32);
}
void dwc_otg_pcd_flush_tx_fifo(const int _num) 
{
        grstctl_t greset = { 0};
        int count = 0;
        greset.b.txfflsh = 1;
        greset.b.txfnum = _num;
        dwc_write_reg32(DWC_REG_GRSTCTL, greset.d32 );
        
        do {
                greset.d32 = dwc_read_reg32( DWC_REG_GRSTCTL);
                if (++count > 10000){                       
                        break;
                }

        } while (greset.b.txfflsh == 1);
        /* Wait for 3 PHY Clocks*/
        udelay(1);
}

void dwc_otg_pcd_flush_rx_fifo(void) 
{
        grstctl_t greset = { 0};
        int count = 0;
        greset.b.rxfflsh = 1;
        dwc_write_reg32( DWC_REG_GRSTCTL, greset.d32 );
        
        do {
                greset.d32 = dwc_read_reg32( DWC_REG_GRSTCTL);
                if (++count > 10000){                        
                        break;
                }
        } while (greset.b.rxfflsh == 1);  
        udelay(1);
}
static void dwc_otg_core_dev_init(void)//p249 6.1.2
{
        dcfg_data_t dcfg = { 0};
        grstctl_t resetctl = { 0 };
        int i;
        fifosize_data_t nptxfifosize;       
 
        dwc_write_reg32(DWC_REG_PCGCCTL, 0);/* Restart the Phy Clock ???*/       
        
	dcfg.d32 = dwc_read_reg32(DWC_REG_DCFG);/* Device configuration register */
#ifdef USE_FULL_SPEED
	dcfg.b.devspd = 1;//Hi speed phy run at Full speed
#else
	dcfg.b.devspd = 0;
#endif
	dcfg.b.perfrint = DWC_DCFG_FRAME_INTERVAL_80;
	dwc_write_reg32(DWC_REG_DCFG, dcfg.d32);
	
        /* Configure data FIFO sizes */	
	dwc_write_reg32(DWC_REG_GRXFSIZ, 256 );	/* Rx FIFO */	
	nptxfifosize.b.depth  = 256;		/* Non-periodic Tx FIFO */
	nptxfifosize.b.startaddr = 256;
	dwc_write_reg32(DWC_REG_GNPTXFSIZ, nptxfifosize.d32 );
 
	dwc_otg_pcd_flush_tx_fifo( 0x10); 		/* all Tx FIFOs */
	dwc_otg_pcd_flush_rx_fifo();
	
	resetctl.b.intknqflsh = 1;		/* Flush the Learning Queue. */
	dwc_write_reg32( DWC_REG_GRSTCTL, resetctl.d32);

        /* Clear all pending Device Interrupts */
        dwc_write_reg32( DWC_REG_DIEPMSK, 0 );
        dwc_write_reg32( DWC_REG_DOEPMSK, 0 );
        dwc_write_reg32( DWC_REG_DAINT, 0xFFFFFFFF );
        dwc_write_reg32( DWC_REG_DAINTMSK, 0 );

        for (i=0; i < MAX_NUM_ENDPOINT; i++) {
		depctl_data_t depctl;
		depctl.d32 = dwc_read_reg32(DWC_REG_IN_EP_REG(i));
		if (depctl.b.epena) {
			depctl.d32 = 0;
			depctl.b.epdis = 1;	//disable the ep
			depctl.b.snak = 1;	//send NAK to Host
		} else {
			depctl.d32 = 0;
		}
		dwc_write_reg32( DWC_REG_IN_EP_REG(i),depctl.d32);

		depctl.d32 = dwc_read_reg32(DWC_REG_OUT_EP_REG(i));
		if (depctl.b.epena) {
			depctl.d32 = 0;
			depctl.b.epdis = 1;
			depctl.b.snak = 1;
		} else {
			depctl.d32 = 0;
		}
		dwc_write_reg32( DWC_REG_OUT_EP_REG(i), depctl.d32);

		/* Device IN/OUT Endpoint Transfer Size */
		dwc_write_reg32(DWC_REG_IN_EP_TSIZE(i), 0);
		dwc_write_reg32(DWC_REG_OUT_EP_TSIZE(i), 0);
		/* Device IN/OUT Endpoint DMA Address Register */
		dwc_write_reg32( DWC_REG_IN_EP_DMA(i), 0);
		dwc_write_reg32( DWC_REG_OUT_EP_DMA(i), 0);
        }
		
        dwc_otg_enable_device_interrupts(); //USB Reset/Enumeration Done/Early Suspend/USB Suspend/SOF---p249 step2	
}
static void dwc_otg_enable_global_interrupts(void)
{
        gahbcfg_data_t ahbcfg = { 0};
        ahbcfg.b.glblintrmsk = 1; /* Enable interrupts */
        dwc_modify_reg32(DWC_REG_GAHBCFG, 0, ahbcfg.d32);
}
int udc_init(void)
{
	u32 reg_base = DWC_REG_BASE;
	serial_printf("dwc otg udc init....reg_base:%x\n",reg_base);

	if(dwc_otg_start_clk()){
		DWC_OTG_ERR("dwc otg udc init clk failed\n");
		return -1;
	}

	udc_device = 0;
	udc_state = STATE_NOT_READY;

	int         snpsid;
	snpsid = dwc_read_reg32(DWC_REG_GSNPSID);
	
	if ((snpsid & 0xFFFFF000) != 0x4F542000) {
		DWC_OTG_ERR("Bad value for SNPSID: 0x%08x\n", snpsid);
		return -1;
	}
	DWC_OTG_DBG("dwc core init is started!\n");
	/*core init..............................................*/
	gahbcfg_data_t  ahbcfg = {.d32 = 0 };//p247 step2	
	ahbcfg.b.glblintrmsk = 1;//Global Interrupt Mask bit	
	dwc_modify_reg32(DWC_REG_GAHBCFG, ahbcfg.d32, 0);

	dwc_otg_core_reset();//should reset the core so we can get the rst_value for all reg

	gusbcfg_data_t  usbcfg = {.d32 = 0 };
	usbcfg.d32 = dwc_read_reg32(DWC_REG_GUSBCFG);	//p248 step3
	usbcfg.b.srpcap = 0;	//SRP capability is not enabled
	usbcfg.b.hnpcap = 0;     //HNP capability is not enabled	
#ifdef USE_FULL_SPEED
	dcfg_data_t     dcfg;
	DWC_OTG_DBG("Core work on Full Speed\n");
	usbcfg.b.physel = 1;  // Work at full speed
	dwc_write_reg32(DWC_REG_GUSBCFG, usbcfg.d32);	

	dwc_otg_core_reset();

	dcfg.d32 = dwc_read_reg32(DWC_REG_DCFG);
	dcfg.b.devspd = 1;//Hi speed phy run at Full speed
	dwc_write_reg32(DWC_REG_DCFG, dcfg.d32);
#else
	DWC_OTG_DBG("Core work on High Speed\n");
	usbcfg.b.physel = 0;// Work at high speed
	usbcfg.b.ulpi_utmi_sel = 1;//ULPI InterfaceDWC_OTG_DBG("dwc_otg_pcd_handle_np_tx_fifo_empty_intr()\n");
	usbcfg.b.phyif = 1; // 16 bit
	usbcfg.b.ddrsel = 0;//4-bit-wide data bus
	dwc_write_reg32(DWC_REG_GUSBCFG, usbcfg.d32);

#endif
	ahbcfg.d32 = 0;//p247 step2
	ahbcfg.b.dmaenable = 0;//DMA Mode bit
	dwc_modify_reg32(DWC_REG_GAHBCFG, ahbcfg.d32, 0);
	/* Enable common interrupts */
	dwc_otg_enable_common_interrupts();//unmask OTG Interrupt --p248 step4
	/*end core init...........................................*/
	if(dwc_read_reg32(DWC_REG_GINTSTS) & 0x1){
		DWC_OTG_ERR("Host Mode\n");
          	return -1;
	} else {
		DWC_OTG_DBG("Device Mode\n");
		dwc_otg_core_dev_init();//p249 6.1.2
	}
	dwc_modify_reg32(DWC_REG_DCTL,0,2);
	udelay(100);
	dwc_modify_reg32(DWC_REG_DCTL,2,0);
	/* Enable the global interrupt after all the interrupt handlers are installed.  */
	dwc_otg_enable_global_interrupts();

	/* the device is ready to accept SOF packets and perform control transfers on control endpoint 0. */
	return 0;
}

/* next functions do controller interrupt */
static void ep0_out_start(void)
{
	deptsiz0_data_t doeptsize0 = { 0};
	//depctl_data_t doepctl = { 0 };
	
	doeptsize0.b.supcnt = 3;    
	doeptsize0.b.pktcnt = 1;
	doeptsize0.b.xfersize = 8*3;
	dwc_write_reg32( DWC_REG_OUT_EP_TSIZE(0),doeptsize0.d32 );       
#if 0 //enable ep0_out just for DMA
	doepctl.d32 = dwc_read_reg32(DWC_REG_OUT_EP_REG(0));
	doepctl.b.epena = 1;
	//doepctl.b.usbactep = 1;
	doepctl.d32 = 0x80008000;
	dwc_write_reg32(DWC_REG_OUT_EP_REG(0),doepctl.d32);
#endif
	flush_cpu_cache();
}
static int dwc_otg_pcd_handle_usb_reset_intr(void)
{

        depctl_data_t doepctl = { 0};
        daint_data_t daintmsk = { 0};
        doepmsk_data_t doepmsk = { 0};
        diepmsk_data_t diepmsk = { 0};
        dcfg_data_t dcfg = { 0 };
        depctl_data_t diepctl = { 0};
        depctl_data_t diepctl_rd = { 0};	
        grstctl_t resetctl = { 0 };
        dctl_data_t dctl = { 0 };
        int i = 0;
        gintsts_data_t gintsts;                        
	//gintmsk_data_t gintmask = { 0 };

        DWC_OTG_DBG("USB RESET\n");
        
        doepctl.b.snak = 1; /* Set NAK for all OUT EPs */
        for (i=0; i < MAX_NUM_ENDPOINT; i++) {//6.4.1.1 step 1
                dwc_write_reg32(DWC_REG_OUT_EP_REG(i), doepctl.d32 );
        }
       
        dctl.b.rmtwkupsig = 1; /* Clear the Remote Wakeup Signalling */
        dwc_modify_reg32( DWC_REG_DCTL,dctl.d32, 0 );

        /* Disable all active IN EPs */
        diepctl.b.epdis = 1;
        diepctl.b.snak = 1;
        for (i=0; i < MAX_NUM_ENDPOINT; i++) {
                diepctl_rd.d32 = dwc_read_reg32(DWC_REG_IN_EP_REG(i));
                if (diepctl_rd.b.epena) {
                        dwc_write_reg32(DWC_REG_IN_EP_REG(i),diepctl.d32 );  
                }
        }

        /* Flush the NP Tx FIFO */
        dwc_otg_pcd_flush_tx_fifo( 0 );
        /* Flush the Learning Queue */
        resetctl.b.intknqflsh = 1;
        dwc_write_reg32( DWC_REG_GRSTCTL, resetctl.d32);

        daintmsk.b.inep0 = 1;//6.4.1.1 step 2
        daintmsk.b.outep0 = 1;
        dwc_write_reg32( DWC_REG_DAINTMSK, daintmsk.d32 );

        doepmsk.b.setup = 1;
        doepmsk.b.xfercompl = 1;
        doepmsk.b.ahberr = 1;
        doepmsk.b.epdisabled = 1;
        dwc_write_reg32( DWC_REG_DOEPMSK, doepmsk.d32 );

        diepmsk.b.xfercompl = 1;
        diepmsk.b.timeout = 1;
        diepmsk.b.epdisabled = 1;
        diepmsk.b.ahberr = 1;
        dwc_write_reg32( DWC_REG_DIEPMSK, diepmsk.d32 );       
        /* Reset Device Address */
        dcfg.d32 = dwc_read_reg32( DWC_REG_DCFG);
        dcfg.b.devaddr = 0;
        dwc_write_reg32( DWC_REG_DCFG, dcfg.d32);

        /* setup EP0 to receive SETUP packets 6.4.1.1 step5*/
	ep0_out_start();
        
	/* Clear interrupt */
	gintsts.d32 = 0;
	gintsts.b.usbreset = 1;
	dwc_write_reg32 ( DWC_REG_GINTSTS, gintsts.d32);

	if(udc_device)
		usbd_device_event_irq(udc_device, DEVICE_RESET, 0);
	
	flush_cpu_cache();	
        return 1;
}
static int dwc_otg_pcd_handle_enum_done_intr(void)
{
	gintsts_data_t gintsts;
	gusbcfg_data_t gusbcfg;
	dsts_data_t dsts;
	depctl_data_t diepctl;
	depctl_data_t doepctl;
	dctl_data_t dctl ={0};        

	DWC_OTG_DBG("SPEED ENUM\n");

	/* Read the Device Status and Endpoint 0 Control registers */
	dsts.d32 = dwc_read_reg32(DWC_REG_DSTS);//6.4.1.2 step1
	diepctl.d32 = dwc_read_reg32(DWC_REG_IN_EP_REG(0) );//6.4.1.2 step2
	doepctl.d32 = dwc_read_reg32(DWC_REG_OUT_EP_REG(0));

	/* Set the MPS of the IN EP based on the enumeration speed */
	diepctl.b.mps = DWC_DEP0CTL_MPS_64;
	dwc_write_reg32(DWC_REG_IN_EP_REG(0) , diepctl.d32);
#if 1
	doepctl.b.epena = 1;
	dwc_write_reg32(DWC_REG_OUT_EP_REG(0), doepctl.d32);
#endif
	dctl.b.cgnpinnak = 1;
	dwc_modify_reg32(DWC_REG_DCTL, dctl.d32, dctl.d32);

	/* Set USB turnaround time based on device speed and PHY interface. */
	gusbcfg.d32 = dwc_read_reg32(DWC_REG_GUSBCFG);
	gusbcfg.b.usbtrdtim = 5;
	dwc_write_reg32(DWC_REG_GUSBCFG, gusbcfg.d32);

	/* Clear interrupt */
	gintsts.d32 = 0;
	gintsts.b.enumdone = 1;
	dwc_write_reg32(DWC_REG_GINTSTS,gintsts.d32 );

	g_dwc_eps[0].ep0state = EP0_IDLE;
	return 1;
}

static struct usb_endpoint_instance *dwc_otg_find_ep (int ep_num,int is_in)
{
	int i,dir = 0;

	for (i = 0; i < udc_device->bus->max_endpoints; i++) {
		if ((udc_device->bus->endpoint_array[i].endpoint_address
			& USB_ENDPOINT_NUMBER_MASK)== ep_num){
			if(ep_num){
				dir = ((udc_device->bus->endpoint_array[i].endpoint_address &
			 		USB_ENDPOINT_DIR_MASK)== USB_DIR_IN);
				if(is_in != dir)
					continue;
			}
			
			return &udc_device->bus->endpoint_array[i];
		}
	}
	return NULL;
}
/* There will be some thing received from Host */
int ep_out_start(int len, int ep_num)
{
	struct usb_endpoint_instance *endpoint = NULL;
	int i;
	dwc_ep_t * ep;
	
	if(ep_num){
		ep = &g_dwc_eps[ep_num];
		endpoint = dwc_otg_find_ep(ep_num,0);
		ep->len = len? len:endpoint->rcv_packetSize;		
		dwc_otg_ep_start_transfer(endpoint,0);
		return 0;
	}

	for(i=1;i<MAX_NUM_ENDPOINT;i++){
		endpoint = &udc_device->bus->endpoint_array[i];
		if((endpoint->endpoint_address & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT){
			ep_num = endpoint->endpoint_address & USB_ENDPOINT_NUMBER_MASK;
			ep = &g_dwc_eps[ep_num];
			/* 
			 * it should always set the max value for OUT-ep because we don't kown 
			 * the data length received from host.if we kown the length,just set it.
			 */
			ep->len = len? len:endpoint->rcv_packetSize;
			dwc_otg_ep_start_transfer(endpoint,0);
		}
	}
	return 0;
}

static void complete_ep(struct usb_endpoint_instance *ep, int is_in)
{
	deptsiz_data_t deptsiz;  
	int ep_num = ep->endpoint_address & USB_ENDPOINT_NUMBER_MASK;
	DWC_OTG_DBG("ep%d,ept %p\n",ep_num,ep);
	if (is_in) {		
		deptsiz.d32 = dwc_read_reg32(DWC_REG_IN_EP_TSIZE(ep_num));
		if (deptsiz.b.xfersize == 0 && deptsiz.b.pktcnt == 0) {
			;
		} 		
	} else {
		deptsiz.d32 = dwc_read_reg32(DWC_REG_OUT_EP_TSIZE(ep_num));		
		ep->last = ep->rcv_packetSize - deptsiz.b.xfersize;		
		usbd_rcv_complete(ep,ep->last,0);
		if(udc_drv && udc_drv->complete)
			udc_drv->complete(ep->rcv_urb,ep->last,0);
		else
			ep_out_start(0,0);
	}
}

static void ep0_complete_request(dwc_ep_t* ep0)
{
        deptsiz0_data_t deptsiz;
	int is_last = 0;

	if (ep0->ep0state == EP0_STATUS) 
		 is_last = 1;    
        else if (ep0_urb->actual_length == 0) {
		ep0->len = 0;
		dwc_otg_ep_start_transfer(ep0_urb->endpoint,ep0->is_in);	
	}
	else if (ep0->is_in) {
		deptsiz.d32 = dwc_read_reg32(DWC_REG_IN_EP_TSIZE(0));
		DWC_OTG_DBG("%d\n",deptsiz.b.xfersize);
		if (deptsiz.b.xfersize == 0) 
			do_setup_status_phase(ep0_urb,0);		
        } 
	else 
		do_setup_status_phase(ep0_urb,1);

        if (is_last)  
                ep0_urb->actual_length = 0;
        
        return;       
}
static void handle_ep0(int is_in)
{
	dwc_ep_t * ep0 = &g_dwc_eps[0];
	//ep0->is_in = is_in;

        switch (ep0->ep0state){
        case EP0_DISCONNECT:
		DWC_OTG_DBG("EP0 DISCONNECT\n");
                break;
                
	case EP0_IDLE:
                ep0->request_config = 0;
                DWC_OTG_DBG("Enter PCD Setup()\n");
                dwc_otg_pcd_setup(ep0_urb);
                break;

	case EP0_IN_DATA_PHASE: 
		//ep0->is_in = 1;          
		DWC_OTG_DBG("EP0_IN_DATA_PHASE:%d\n",ep0_urb->actual_length);
		//if (ep0_urb->endpoint->sent >= ep0_urb->actual_length) 		
			ep0_complete_request(ep0);		
                break;

	case EP0_OUT_DATA_PHASE:
		//ep0->is_in = 0;
		DWC_OTG_DBG("EP0_OUT_DATA_PHASE\n");
                ep0_complete_request(ep0);
                break;                

	case EP0_STATUS:
		ep0_complete_request(ep0);
		DWC_OTG_DBG("EP0_STATUS\n");
                ep0->ep0state = EP0_IDLE;
                break;

        case EP0_STALL:
                DWC_OTG_ERR("EP0 STALLed\n");
                break;
        }
}
static void do_setup_status_phase(struct urb *urb,int is_in)
{
	struct usb_endpoint_instance *endpoint = urb->endpoint;

	dwc_ep_t * ep0 = &g_dwc_eps[0];
	ep0->ep0state = EP0_STATUS;
        if (ep0->ep0state == EP0_STALL){
                return;
        }
	urb->actual_length = 0;
	endpoint->sent = 0;
	ep0->len = 0;
	dwc_otg_ep_start_transfer(endpoint,is_in);
	ep0_out_start();
}

#define CLEAR_IN_EP_INTR(__epnum,__intr) \
do { \
        diepint_data_t diepint = { 0 }; \
	diepint.b.__intr = 1; \
	dwc_write_reg32(DWC_REG_IN_EP_INTR(__epnum), \
			diepint.d32); \
} while (0)
static int dwc_otg_pcd_handle_in_ep_intr(void)
{


	diepint_data_t diepint = { 0 };
	//depctl_data_t diepctl = { 0 };
	u32 ep_intr;
	u32 epnum = 0;
	//gintmsk_data_t intr_mask = {0};
	gintsts_data_t gintsts;

	struct usb_endpoint_instance *ep = NULL;	

	/* Read in the device interrupt bits */
	ep_intr = (dwc_read_reg32(DWC_REG_DAINT) & dwc_read_reg32( DWC_REG_DAINTMSK));
	ep_intr = ((ep_intr & 0xffff));

	/* Clear the INEPINT in GINTSTS */
	/* Clear all the interrupt bits for all IN endpoints in DAINT */
	gintsts.d32 = 0;
	gintsts.b.inepint = 1;
	dwc_write_reg32 (DWC_REG_GINTSTS, gintsts.d32);
	dwc_write_reg32(DWC_REG_DAINT, 0xFFFF );

	DWC_OTG_DBG( "%x %x\n",dwc_read_reg32(DWC_REG_DAINT),dwc_read_reg32(DWC_REG_DAINTMSK)); 

	flush_cpu_cache();
	/* Service the Device IN interrupts for each endpoint */
	while( ep_intr ) {
		if (ep_intr&0x1) {
			ep = dwc_otg_find_ep(epnum,1);			
			diepint.d32 = (dwc_read_reg32( DWC_REG_IN_EP_INTR(epnum)) & dwc_read_reg32(DWC_REG_DAINTMSK));      
			//DWC_OTG_DBG( "ep%d,intr:%x\n",epnum,dwc_read_reg32( DWC_REG_IN_EP_INTR(epnum))); 
			/* Transfer complete */
			if ( diepint.b.xfercompl ) {                       
#if 0     
				/* Disable the NP Tx FIFO Empty Interrrupt */
				intr_mask.b.nptxfempty = 1;
				dwc_modify_reg32( DWC_REG_GINTMSK, intr_mask.d32, 0);
#endif
				/* Clear the bit in DIEPINTn for this interrupt */
				CLEAR_IN_EP_INTR(epnum,xfercompl);
				/* Complete the transfer Device ---> Host */
				if (epnum == 0) {
					handle_ep0(0);//do_setup_status_phase(ep0_urb,1);//It really need to send a IN status packet to Host ?
				} else 
					complete_ep(ep,1);
			}
			/* Endpoint disable  */
			if ( diepint.b.epdisabled ) {
				DWC_OTG_DBG("IN-Endpoint disable intr occur\n");
				//handle_in_ep_disable_intr( epnum );
				/* Clear the bit in DIEPINTn for this interrupt */
				CLEAR_IN_EP_INTR(epnum,epdisabled);
			}
			/* AHB Error */
			if ( diepint.b.ahberr ) {
				/* Clear the bit in DIEPINTn for this interrupt */
				CLEAR_IN_EP_INTR(epnum,ahberr);
			}
			/* TimeOUT Handshake (non-ISOC IN EPs) */
			if ( diepint.b.timeout ) { 
				//handle_in_ep_timeout_intr( epnum );
				CLEAR_IN_EP_INTR(epnum,timeout);
			}
			/* IN Token received with TxF Empty */
			if (diepint.b.intktxfemp){
				DWC_OTG_DBG("IN Token Rev while TxFIFO empty\n");
				CLEAR_IN_EP_INTR(epnum,intktxfemp);
			}                                        
			/* IN Token Received with EP mismatch */
			if (diepint.b.intknepmis){
				DWC_OTG_DBG("IN Token Rev but not match TxFIFO data\n");
				CLEAR_IN_EP_INTR(epnum,intknepmis);
			}
			/* IN Endpoint NAK Effective */
			if (diepint.b.inepnakeff){
				CLEAR_IN_EP_INTR(epnum,inepnakeff);
			}
		}
		epnum++;
		ep_intr >>=1;
	}

	return 0;

}

void dwc_otg_ep_enable(struct usb_endpoint_instance *endpoint)
{
	depctl_data_t depctl = {0};
	daint_data_t daintmsk = {0};
	int epctl;
	int ep_isout, eptype = 0, ep_size;

	int ep_num = endpoint->endpoint_address & USB_ENDPOINT_NUMBER_MASK;

	ep_isout = (endpoint->endpoint_address & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT;
	eptype = ep_isout ? endpoint->rcv_attributes : endpoint->tx_attributes;
	ep_size = ep_isout ? endpoint->rcv_packetSize : endpoint->tx_packetSize;

	DWC_OTG_DBG("ep_num:%d,%s,mps:%d,eptype:%d\n",ep_num,ep_isout?"out":"in",ep_size,eptype);
	if(!ep_isout){
		epctl = DWC_REG_IN_EP_REG(ep_num);
		daintmsk.ep.in = 1<<ep_num; //ep1:BULK_IN
	}
	else{
		epctl = DWC_REG_OUT_EP_REG(ep_num);
		daintmsk.ep.out = 1<<ep_num;//ep2:BULK_OUT
	}
	
	depctl.d32 = dwc_read_reg32(epctl);
	if (!depctl.b.usbactep) {
		depctl.b.mps = ep_size;
		depctl.b.eptype = eptype;
		depctl.b.setd0pid = 1;
		depctl.b.txfnum = 0;
		depctl.b.usbactep = 1;
		dwc_write_reg32(epctl, depctl.d32);
	}
 	dwc_modify_reg32(DWC_REG_DAINTMSK, 0, daintmsk.d32);
	return;
}

/* Host --->  Device*/
static void dwc_otg_read_packet(u8 *_dest, u16 _bytes,u8 ep_num)
{
	int i;
	u32 buff =0;
	u8*  pbyte = _dest;
	if(_dest == NULL){
		DWC_OTG_ERR("urb->buffer is point to NULL\n");
		return;
	}
	DWC_OTG_DBG("urb->buffer is point to %p,len:%d\n",_dest,_bytes);
	if(((unsigned int)_dest &0x3) == 0){
		for(i=0; i<_bytes; i+=4){
			*(u32*)_dest = dwc_read_reg32(DWC_REG_DATA_FIFO_START + ep_num * 0x1000);
			_dest+=4;
		}
	}else{
		for (i=0; i<_bytes; i++) {
			if((i % 4) ==0)
				buff =dwc_read_reg32(DWC_REG_DATA_FIFO_START + ep_num * 0x1000);				

			*(u8*)pbyte ++ = buff;
			buff >>= 8;
		}
	}
}

static int dwc_otg_pcd_setup(struct urb *urb)
{
	dcfg_data_t dcfg = {0};
	dwc_ep_t * ep0 = &g_dwc_eps[0];
	struct usb_endpoint_instance *endpoint = NULL;
	int i,ret = 0;

	if(ep0->request_enable == 0)
		return -1;

	ep0->request_enable = 0;

	if(ep0_recv_setup(ep0_urb)){
		DWC_OTG_ERR("ep0 setup\n");
		return -1;
	}

	if((urb->device_request.bmRequestType & USB_REQ_DIRECTION_MASK)
		== USB_REQ_HOST2DEVICE){
		ep0->ep0state = EP0_OUT_DATA_PHASE;
		ep0->is_in = 0;
	}
	else{
		ep0->ep0state = EP0_IN_DATA_PHASE;	
		ep0->is_in = 1;
	}
	if ((urb->device_request.bmRequestType & USB_TYPE_RESERVED) == USB_TYPE_STANDARD) {		
		switch (urb->device_request.bRequest) {
			case USB_REQ_SET_ADDRESS://6.4.1.3			
				dcfg.b.devaddr = urb->device->address;
				dwc_modify_reg32(DWC_REG_DCFG,0, dcfg.d32);
				do_setup_status_phase(urb,1);//need a zero packet? IN status packet?
				dwc_otg_udc_state_transition_up (urb->device->device_state,
							STATE_ADDRESSED);
				return 0;

			case USB_REQ_SET_INTERFACE:
			case USB_REQ_SET_CONFIGURATION:	
				ep0->request_config = 1;
				dwc_otg_udc_state_transition_up (urb->device->device_state,
								STATE_CONFIGURED);
				if(!udc_drv){
					for(i=1;i<MAX_NUM_ENDPOINT;i++){
						endpoint = &udc_device->bus->endpoint_array[i];
						dwc_otg_ep_enable(endpoint);
					}
				}		
	
			default:
				break;
						
		}
		if(udc_drv && udc_drv->setup)
			udc_drv->setup(urb);
		ret = udc_endpoint_write(urb->endpoint);
	}
	else if(((urb->device_request.bmRequestType & USB_REQ_DIRECTION_MASK) == USB_REQ_DEVICE2HOST)
			&& (urb->actual_length)){
			DWC_OTG_DBG("Vendor setup IN\n");
			ret = udc_endpoint_write(urb->endpoint);
	}
	else{
		DWC_OTG_DBG("Vendor setup OUT: %d\n",urb->device_request.wLength);		
		if(urb->device_request.wLength){
			ep0->len = urb->device_request.wLength;
			dwc_otg_ep_start_transfer(urb->endpoint,0);
		}
		do_setup_status_phase(urb,1);	

		//if(urb->device_request.bRequest = 0x22)
			//ep_in_bulk_enable();/* ready for IN transfer now */	
	}

	return ret;
}
static int dwc_otg_pcd_handle_out_ep_intr(void)
{
#define CLEAR_OUT_EP_INTR(__epnum,__intr) \
do { \
        doepint_data_t doepint = { 0 }; \
	doepint.b.__intr = 1; \
	dwc_write_reg32(DWC_REG_OUT_EP_INTR(__epnum), \
			doepint.d32); \
} while (0)

	u32 ep_intr;
	doepint_data_t doepint = { 0 };
	u32 epnum = 0;
	gintsts_data_t gintsts;
	//deptsiz_data_t deptsiz;
	int ret = 0;
	struct usb_endpoint_instance *ep = NULL;
        
	DWC_OTG_DBG( "\n" );

	/* Read in the device interrupt bits */
	ep_intr = (dwc_read_reg32(DWC_REG_DAINT) &
                dwc_read_reg32( DWC_REG_DAINTMSK));
	ep_intr =( (ep_intr & 0xffff0000) >> 16);

	/* Clear the OUTEPINT in GINTSTS */
	gintsts.d32 = 0;
	gintsts.b.outepintr = 1;
	dwc_write_reg32 (DWC_REG_GINTSTS, gintsts.d32);
	dwc_write_reg32(DWC_REG_DAINT, 0xFFFF0000 );

	while( ep_intr ) {
		if (ep_intr&0x1) {
			ep = dwc_otg_find_ep(epnum,0);

			doepint.d32 = (dwc_read_reg32( DWC_REG_OUT_EP_INTR(epnum)) & dwc_read_reg32(DWC_REG_DOEPMSK));
            
			/* Transfer complete */
			if ( doepint.b.xfercompl ) {
				DWC_OTG_DBG("EP%d OUT Xfer Complete\n", epnum);

				/* Clear the bit in DOEPINTn for this interrupt */
				CLEAR_OUT_EP_INTR(epnum,xfercompl);

				if (epnum == 0) 
					;//ohh,should FIX me here!
				else 
					complete_ep(ep,0);
				ret = 1;
			}
			/* Endpoint disable  */
			if ( doepint.b.epdisabled ) {	
				/* Clear the bit in DOEPINTn for this interrupt */
				DWC_OTG_DBG("dwc_otg:OUT-Endpoint disable intr occur\n");
				CLEAR_OUT_EP_INTR(epnum,epdisabled);
			}
			/* AHB Error */
			if ( doepint.b.ahberr ) {                            
				CLEAR_OUT_EP_INTR(epnum,ahberr);
			}
			/* Setup Phase Done (contorl EPs) */
			if ( doepint.b.setup ) {
				//if(!ep0_recv_setup(ep0_urb)){
				//	ret = dwc_otg_pcd_setup(ep0_urb);
				//}else
				DWC_OTG_DBG("ep0 setup\n");
				handle_ep0(0);
				CLEAR_OUT_EP_INTR(epnum,setup);
			}
		}
		epnum++;
		ep_intr >>=1;
	}

	return ret;

#undef CLEAR_OUT_EP_INTR
}


static void dwc_otg_read_setup_packet(u32 *_dest)
{
	/* Pop 2 DWORDS off the receive data FIFO into memory */
	_dest[0] = dwc_read_reg32(DWC_REG_DATA_FIFO_START);
	_dest[1] = dwc_read_reg32(DWC_REG_DATA_FIFO_START);
}

static int dwc_otg_pcd_handle_rx_status_q_level_intr(void)//p185
{
	gintmsk_data_t gintmask = { 0 };
	device_grxsts_data_t status;
	gintsts_data_t gintsts;

	/* Disable the Rx Status Queue Level interrupt */
	gintmask.b.rxstsqlvl= 1;
	dwc_modify_reg32( DWC_REG_GINTMSK, gintmask.d32, 0);

	/* Get the Status from the top of the FIFO */
	status.d32 = dwc_read_reg32( DWC_REG_GRXSTSP);//p190
	int ep_num = status.b.epnum;
	dwc_ep_t *ep;

	//int i;
	struct usb_endpoint_instance *endpoint = dwc_otg_find_ep(ep_num,0);
	struct urb *urb;
	u8* data = NULL;
	u8* p = NULL;
	if(ep_num)
		urb = endpoint->rcv_urb;
	else
		urb = ep0_urb;

	ep = &g_dwc_eps[ep_num];
	data = urb->buffer;
	
	switch (status.b.pktsts) {
		case DWC_DSTS_GOUT_NAK:
			DWC_OTG_DBG( "Global OUT NAK\n");
			break;

        case DWC_STS_DATA_UPDT://OUT data packet received p190
			DWC_OTG_DBG( "1.OUT Data Packet:%d\n",status.b.bcnt);
			if (status.b.bcnt){
				/** @todo NGS Check for buffer overflow? */
				dwc_otg_read_packet(data,status.b.bcnt,ep_num);
				//urb->actual_length += status.b.bcnt;	
				DWC_OTG_DBG( "2.OUT Data Packet:%d\n",status.b.bcnt);		
			}
			break;

        case DWC_STS_XFER_COMP:
			DWC_OTG_DBG("OUT Complete\n");
			break;

        case DWC_DSTS_SETUP_COMP:
			DWC_OTG_DBG("SETUP Complete\n");
			break;

        case DWC_DSTS_SETUP_UPDT://SETUP data packet received p190
			DWC_OTG_DBG("SETUP update\n");
			dwc_otg_read_setup_packet((u32*)&urb->device_request);
			p = (u8*)&urb->device_request;
#ifdef __DEBUG_DWC_OTG_UDC__
			//for(i=0;i<8;i++)
			//	serial_printf("%x-",p[i]);
			//serial_printf("\n");
#endif
			ep->request_enable = 1;
			ep->ep0state = EP0_IDLE;
			/*NOT FIX setup---data-OUT. Now the data is in urb->buffer! we should send it to host*/

			break;

        default:
			DWC_OTG_DBG( "Invalid Packet Status (0x%0x)\n", status.b.pktsts);
			break;
                
	}
	/* Enable the Rx Status Queue Level interrupt */
	dwc_modify_reg32( DWC_REG_GINTMSK, 0, gintmask.d32);
	/* Clear interrupt */
	gintsts.d32 = 0;
	gintsts.b.rxstsqlvl = 1;
	dwc_write_reg32 ( DWC_REG_GINTSTS, gintsts.d32);

	return 0;
}
#if 0
static int dwc_otg_pcd_handle_np_tx_fifo_empty_intr(void)
{
	gintsts_data_t gintsts;
        
	DWC_OTG_DBG("\n");
	/* Get the epnum from the IN Token Learning Queue. */

	/* Clear interrupt */
	gintsts.d32 = 0;
	gintsts.b.nptxfempty = 1;
	dwc_write_reg32 (DWC_REG_GINTSTS, gintsts.d32);
	flush_cpu_cache();
	return 1;
}
#endif
static void dwc_otg_pcd_irq()
{
	gotgint_data_t gotgint;
	gintsts_data_t  gintr_status,gintr_status_back;
	gintsts_data_t  gintr_msk;

	int handled;
	do{
		handled = 0;

		gotgint.d32 = dwc_read_reg32(DWC_REG_GOTGINT);
		if(gotgint.d32)
			dwc_write_reg32(DWC_REG_GOTGINT,gotgint.d32);

		gintr_msk.d32 = dwc_read_reg32(DWC_REG_GINTMSK);
		gintr_status_back.d32 = gintr_status.d32 = dwc_read_reg32(DWC_REG_GINTSTS);
		if((gintr_status.d32 & gintr_msk.d32 ) == 0)
			break;
		
		gintr_status.d32 = gintr_status.d32 & gintr_msk.d32;
		//DWC_OTG_DBG("irq: %x\n",gintr_status.d32);
		
		if (gintr_status.b.rxstsqlvl ) {
			dwc_otg_pcd_handle_rx_status_q_level_intr();
			handled = 1;
		}
		if (gintr_status.b.nptxfempty) {
#if 0
	   		dwc_otg_pcd_handle_np_tx_fifo_empty_intr();
			handled = 1;
#endif
		}
		if (gintr_status.b.usbreset){
	   		dwc_otg_pcd_handle_usb_reset_intr();
			handled = 1;
		}
		if (gintr_status.b.enumdone){
			dwc_otg_pcd_handle_enum_done_intr();
			handled = 1;
		}	
		if (gintr_status.b.inepint){				
			dwc_otg_pcd_handle_in_ep_intr();
			handled = 1;

		}
		if (gintr_status.b.outepintr){
	    		dwc_otg_pcd_handle_out_ep_intr();
			handled = 1;
		}
		dwc_write_reg32(DWC_REG_GINTSTS,gintr_status_back.d32);
		flush_cpu_cache();		
		
	}while(handled);	
}

void udc_irq(void)
{
	if (udc_state != STATE_READY) {
		return;
	}
	dwc_otg_pcd_irq();
	return;
}
static int dwc_otg_ep_write_packet(struct usb_endpoint_instance *endpoint)
{
	u32 i;
	u32 dword_count;
	u32 fifo;
	u32 temp_data ;
	int wait_cnt = 0;
	int len_push_fifo = 0;
	struct urb *urb; 
	gnptxsts_data_t tx_status = {0};

	u16 ep_num = endpoint->endpoint_address & USB_ENDPOINT_NUMBER_MASK;
	urb = ep_num? endpoint->tx_urb:ep0_urb;

	int byte_count = MIN(urb->actual_length, endpoint->tx_packetSize);
	if (byte_count < 0)
		return -1;

	u8 *data_buff = urb->buffer;   

	dword_count =  (byte_count + 3) / 4;

	do{
		if(wait_cnt >= 5000)//5ms
			return -1;
		tx_status.d32 = dwc_read_reg32(DWC_REG_GNPTXSTS);
		wait_cnt++;			
	}while(tx_status.b.nptxqspcavail == 0
			&& tx_status.b.nptxfspcavail < dword_count);//6.4.2.2.1

	fifo = DWC_REG_DATA_FIFO_START + ep_num * 0x1000;
	wait_cnt = 5000;
	
	while (tx_status.b.nptxqspcavail > 0 && tx_status.b.nptxfspcavail > dword_count &&
	       len_push_fifo < urb->actual_length) {//p329

		for (i=0; i<dword_count; i++) {
			temp_data =get_unaligned(data_buff);
			dwc_write_reg32( fifo, temp_data );
			data_buff += 4;
		}

		len_push_fifo += byte_count;
		endpoint->last += byte_count;
		byte_count =  MIN(urb->actual_length- len_push_fifo, endpoint->tx_packetSize);
		dword_count =  (byte_count + 3) / 4;

		DWC_OTG_DBG("%d-%d\n",len_push_fifo,urb->actual_length);
		if(len_push_fifo >= urb->actual_length)
			break;
		while(wait_cnt--){
			tx_status.d32 = dwc_read_reg32(DWC_REG_GNPTXSTS);
			if(tx_status.b.nptxqspcavail > 0)
				break;						
		}
	}
	if(ep_num){
		diepint_data_t diepint = { 0 };
		usbd_tx_complete(endpoint);
		if(udc_drv && udc_drv->complete)
			udc_drv->complete(urb,urb->actual_length,0);
		while(!(diepint.b.xfercompl))
			diepint.d32 = (dwc_read_reg32( DWC_REG_IN_EP_INTR(ep_num)) & dwc_read_reg32(DWC_REG_DAINTMSK)); 
	}
	flush_cpu_cache();
	return 0;
}

static void dwc_otg_ep_start_transfer(struct usb_endpoint_instance *endpoint ,int is_in)
{
	depctl_data_t depctl;
	deptsiz_data_t deptsiz;
	int ep_mps = endpoint->tx_packetSize;
	u16 ep_num = endpoint->endpoint_address & USB_ENDPOINT_NUMBER_MASK;
	struct urb *urb = NULL;
	u32 epctl_reg,epctltsize_reg;
	int len = 0;
	dwc_ep_t * ep = &g_dwc_eps[ep_num];

	if(!ep_num){
		switch (ep->ep0state) {
			case EP0_STATUS:
			case EP0_IN_DATA_PHASE:
				break;
			case EP0_OUT_DATA_PHASE:
				if (ep->request_config) {
				        ep->is_in = 1;
				        ep->ep0state = EP0_STATUS;
				}
				else if(urb->actual_length == 0)
				{
				        ep->is_in = 1;
				        ep->ep0state = EP0_STATUS;                                
				}
				break;
			default:
				return;
		}
		urb = ep0_urb;
	}
	else if(is_in)
		urb = endpoint->tx_urb;
	
	else
		urb = endpoint->rcv_urb;
	
	len = ep->len;

	if(is_in == 0 && ep_num)
		DWC_OTG_DBG("ep_num%d,len=%d,%s\n",ep_num,len,is_in?"in":"out");

	if(is_in){
		epctl_reg = DWC_REG_IN_EP_REG(ep_num);
		epctltsize_reg = DWC_REG_IN_EP_TSIZE(ep_num);
	}else{
		epctl_reg = DWC_REG_OUT_EP_REG(ep_num);
		epctltsize_reg = DWC_REG_OUT_EP_TSIZE(ep_num);	
	}

	depctl.d32 = dwc_read_reg32(epctl_reg);
	deptsiz.d32 = dwc_read_reg32(epctltsize_reg);	

	if (len == 0) {
		deptsiz.b.xfersize = is_in?0:ep_mps;
		deptsiz.b.pktcnt = 1;
	} else {
		deptsiz.b.pktcnt = (len + (ep_mps - 1)) /ep_mps;
		deptsiz.b.xfersize = is_in ? len:deptsiz.b.pktcnt * ep_mps;
	}

	/* Fill size and count */
	dwc_write_reg32(epctltsize_reg,deptsiz.d32);
	
	/* EP enable */
	depctl.b.cnak = 1;
	depctl.b.epena = 1;
	dwc_write_reg32 (epctl_reg, depctl.d32);
	
	/* IN endpoint */
#if 0
	if (is_in) {
		gintmsk_data_t intr_mask = {0};		
		intr_mask.b.nptxfempty = 1;

		dwc_modify_reg32( DWC_REG_GINTSTS,intr_mask.d32, 0);
		dwc_modify_reg32( DWC_REG_GINTMSK,intr_mask.d32, intr_mask.d32);                
	} 
#endif
}

void udc_set_nak(int ep_num)
{
#if 0
	DWC_OTG_DBG("%d\n",ep_num);
	depctl_data_t diepctl;
	diepctl.d32 = dwc_read_reg32(DWC_REG_IN_EP_REG(ep_num));
	diepctl.b.snak = 1;
	dwc_write_reg32(DWC_REG_IN_EP_REG(ep_num),diepctl.d32 );  

	depctl_data_t doepctl;
	doepctl.d32 = dwc_read_reg32(DWC_REG_IN_EP_REG(ep_num));
	doepctl.b.snak = 1;
	dwc_write_reg32(DWC_REG_OUT_EP_REG(ep_num), doepctl.d32 );
#endif
}
/* The func just for bulk_OUT transfer */
void udc_unset_nak(int ep_num)
{
#if 0
	DWC_OTG_DBG("%d\n",ep_num);
	depctl_data_t diepctl;
	diepctl.d32 = dwc_read_reg32(DWC_REG_IN_EP_REG(ep_num));
	diepctl.b.epena = 1;
	diepctl.b.cnak = 1;
	dwc_write_reg32(DWC_REG_IN_EP_REG(ep_num),diepctl.d32 );  

	depctl_data_t doepctl;
	doepctl.d32 = dwc_read_reg32(DWC_REG_IN_EP_REG(ep_num));
	doepctl.b.epena = 1;
	doepctl.b.cnak = 1;
	dwc_write_reg32(DWC_REG_OUT_EP_REG(ep_num), doepctl.d32 );
#endif
	ep_out_start(0,0);
}
/* Write some data to an endpoint which starting a packet transmission.  TO HOST */
int udc_endpoint_write(struct usb_endpoint_instance *endpoint)
{	
	
	int ret = -1;
	if(endpoint == NULL)
		return ret;

	int ep_num = endpoint->endpoint_address & USB_ENDPOINT_NUMBER_MASK;
	dwc_ep_t * ep = &g_dwc_eps[ep_num];

	if(ep_num){
		if(endpoint->tx_urb->actual_length)
			DWC_OTG_DBG("Starting transmit on ep%d, len:%d\n",ep_num,endpoint->tx_urb->actual_length);
	}
	if (endpoint->tx_urb || !ep_num){				
		ep->len =  ep_num? endpoint->tx_urb->actual_length:ep0_urb->actual_length;
		dwc_otg_ep_start_transfer(endpoint,1);
		if(!dwc_otg_ep_write_packet(endpoint)){
			ret = 0;
			DWC_OTG_DBG("udc_endpoint_write:successful\n");
		}
		else{
			endpoint->last = 0;
			endpoint->sent = 0;
		}
	}	
	return ret;
}
/* Associate a physical endpoint with endpoint instance */
void udc_setup_ep(struct usb_device_instance *device, unsigned int id,
		  struct usb_endpoint_instance *endpoint)
{
	int ep_num, ep_addr, ep_isout, eptype = 0, ep_size;
	udc_device = device;
	
	ep_addr = endpoint->endpoint_address;
	ep_num = ep_addr & USB_ENDPOINT_NUMBER_MASK;

	if (ep_num >= 4) {
		DWC_OTG_ERR("unable to setup ep %d!\n", ep_num);
		return;
	}
	if(ep_num){
		ep_isout = (ep_addr & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT;
		eptype = ep_isout ? endpoint->rcv_attributes : endpoint->tx_attributes;
		ep_size = ep_isout ? endpoint->rcv_packetSize : endpoint->tx_packetSize;
		//g_dwc_eps[ep_num].is_in = !ep_isout;
		g_dwc_eps[ep_num].type = eptype;
		g_dwc_eps[ep_num].maxpacket = ep_size;		
	}

	DWC_OTG_DBG("id:%d,ep_num:%d,%s,mps:%d,eptype:%d\n", id,ep_num,ep_isout?"out":"in",ep_size,eptype);

}

/* Connect the USB device to the bus */
void udc_connect(void)
{
	DWC_OTG_DBG("dwc otg udc connect....\n");
	dwc_modify_reg32(DWC_REG_DCTL,2,0);
	udc_state = STATE_READY;
}

void udc_disconnect(void)
{
	DWC_OTG_DBG("dwc otg udc disconnect....\n");

	if (udc_state != STATE_ERROR) {
		udc_state = STATE_NOT_READY;
	}
	dwc_modify_reg32(DWC_REG_DCTL,0,2);	
}

void udc_enable(struct usb_device_instance *device)
{
	if (udc_state == STATE_ERROR) {
		return;
	}
	//udc_device = device;
	DWC_OTG_DBG("device:%p\n",device);
	if (!ep0_urb) {
		ep0_urb = usbd_alloc_urb(device, device->bus->endpoint_array);
	} else {
		DWC_OTG_DBG("udc_enable: ep0_urb already allocated %p\n", ep0_urb);
	}
}

void udc_disable(void)
{
	udc_state = STATE_NOT_READY;
	udc_device = NULL;
}

/* Enable dev, init udc pointers */
void udc_startup_events(struct usb_device_instance *device)
{
	usbd_device_event_irq(device, DEVICE_INIT, 0);	//called to respond to various usb events
	usbd_device_event_irq(device, DEVICE_CREATE, 0);
	udc_enable(device);
}

int udc_register_driver(struct udc_driver* udc_driver)
{
	if(!udc_driver)
		return -1;

	udc_drv = udc_driver;
	DWC_OTG_DBG("registed a drv %s\n", udc_drv->driver_name);
	if(udc_drv->bind)
		return udc_drv->bind(udc_drv->usb_dev);
	return 0;
}
int udc_unregister_driver(struct udc_driver* udc_driver)
{
	if(!udc_driver)
		return -1;

	DWC_OTG_DBG("unregisted a drv %s\n", udc_drv->driver_name);
	if(udc_drv->bind)
		udc_drv->unbind(udc_drv->usb_dev);
	udc_drv = 0;	
	
	return 0;
}
void udc_ep_enable(struct usb_endpoint_instance *endpoint)
{
	dwc_otg_ep_enable(endpoint);
}

int do_gadget(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	return dwc_otg_start_clk();
}
U_BOOT_CMD(
	gadgetstart,CONFIG_SYS_MAXARGS, 1, do_gadget,
	"please run cmd: gadget start",
	" - This cmd set the uboot goto fastboot mode.\n"
	" if you want to come back,run 'reboot-bootloader' cmd in fastboot console,\n"
	" then you should enter ctrl+c to cancel pre_cmd sending to target."
);