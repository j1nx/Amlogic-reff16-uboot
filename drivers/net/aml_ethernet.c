/*******************************************************************
 * 
 *  Copyright C 2005 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: Enthernet driver for nike.
 *
 *  Author: Min Chu
 *  Created: 2009-3-18 
 *
 *******************************************************************/

//#include <cmd_confdefs.h>			/* Command configs	*/
#include <asm/u-boot.h>	/* Board data structure	*/
#include <asm/global_data.h>
#include <asm/arch/am_reg_addr.h>
#include <asm/arch/aml_emac_lan8700.h>
#include <asm/arch/am_eth_reg.h>
#include <config.h>				
#include <malloc.h>
#include <common.h>
#include <net.h>
#include <linux/types.h>
#include <asm/cache.h>

extern unsigned __net_dma_start;
extern unsigned __net_dma_end;

struct _gStruct* 	gS = NULL;
struct _rx_desc* 	rx = NULL;
struct _tx_desc* 	tx = NULL;
struct _rx_desc* 	current_rx = NULL;
struct _tx_desc* 	current_tx = NULL;
static int initialised = 0 ;
char def_mac[10] = { 0x00 ,0x01 ,0x02 ,0x03 ,0x04 ,0x05  };
static unsigned long phy_Identifier=0;
static unsigned int   g_speed_enforce=0;
static unsigned int  g_mdc_clock_range=ETH_MAC_4_GMII_Addr_CR_100_150;
#define PHY_SMSC_8700			0x7c0c4
#define PHY_ATHEROS_8032		0x004dd023
#define PHY_IC_IP101ALF         0x02430c54

//#define ET_DEBUG
/*
  * phyid [in]:
  * reg [in]: physical layer chip id
  * val [in]: 
  */
static void phy_reg_wr(int phyid, unsigned int reg, unsigned int val)
{
	unsigned long busy = 0, tmp = 0;
	unsigned int phyaddr;
	unsigned int phyreg;
	unsigned long reg4;

	phyaddr = phyid << ETH_MAC_4_GMII_Addr_PA_P;
	phyreg  = reg << ETH_MAC_4_GMII_Addr_GR_P;

	reg4 = phyaddr | phyreg | g_mdc_clock_range | ETH_MAC_4_GMII_Addr_GW | ETH_MAC_4_GMII_Addr_GB;
	writel(val, ETH_MAC_5_GMII_Data);
	writel(reg4, ETH_MAC_4_GMII_Addr);
	busy = 1;
	while (busy) 
	{
		tmp = readl(ETH_MAC_4_GMII_Addr);
		busy = tmp&1;
	}
}

/*
  * phyid [in]:
  * reg [in]: physical layer chip id
  * val [out]: 
  */
static unsigned int phy_reg_rd(int phyid, unsigned int reg)
{
	unsigned long busy = 0, tmp = 0;
	unsigned int phyaddr;
	unsigned int phyreg;
	unsigned long reg4;

	phyaddr = phyid << ETH_MAC_4_GMII_Addr_PA_P;
	phyreg  = reg << ETH_MAC_4_GMII_Addr_GR_P;
	reg4 = phyaddr | phyreg | g_mdc_clock_range | ETH_MAC_4_GMII_Addr_GB;
	writel(reg4, ETH_MAC_4_GMII_Addr);
	
	busy = 1;
	while (busy) 
	{
		tmp = readl(ETH_MAC_4_GMII_Addr);
		busy = tmp&1;
	}

	tmp = readl(ETH_MAC_5_GMII_Data);
	return tmp;
}
//void dcache_flush_range(unsigned start, unsigned size);
///void dcache_clean_range(unsigned start, unsigned size);
//void dcache_invalid_range(unsigned start, unsigned size);

static inline void _dcache_flush_range_for_net(unsigned startAddr, unsigned endAddr)
{
		dcache_clean_range(startAddr,endAddr-startAddr+1);
    return;
}

static inline void _dcache_inv_range_for_net(unsigned startAddr, unsigned endAddr)
{
		dcache_flush_range(startAddr,endAddr-startAddr+1);
		dcache_invalid_range(startAddr,endAddr-startAddr+1);
    return;
}

int eth_io_init(void)
{
#if 0
	int chip=2953;
	int selectclk,n;
	int clk;
	char *s = getenv("machid");
	if(s)
		{
			chip=simple_strtoul(s,NULL,10);
			printf("get machid from env=%d\n",chip);
		}
	else
		{
			printf("not set the machid,used default(%d)\n",chip);
		}

	
	udelay(100);
	writel(readl(ETH_PLL_CNTL) & ~(1 << 0), ETH_PLL_CNTL); // Disable the Ethernet clocks     
	// ---------------------------------------------
	// Test 50Mhz Input Divide by 2
	// ---------------------------------------------
	// Select divide by 2
	writel(readl(ETH_PLL_CNTL) | (0 << 3), ETH_PLL_CNTL); // desc endianess "same order"   
	writel(readl(ETH_PLL_CNTL) | (0 << 2), ETH_PLL_CNTL); // data endianess "little"    
	writel(readl(ETH_PLL_CNTL) | (1 << 1), ETH_PLL_CNTL); // divide by 2 for 100M     
	writel(readl(ETH_PLL_CNTL) | (1 << 0), ETH_PLL_CNTL);  // enable Ethernet clocks   
	/*
	5,6,7 sata
	4-extern pad
	3-other_pll_clk
	2-ddr_pll_clk
	1-APLL_CLK_OUT_400M
	0----sys_pll_div3 (333~400Mhz)
	*/
	clk=get_cpu_clk();//a9 clk
	clk=clk<<1;//sys_pll;
	printf("get sys clk=%d\n",clk);
	if(((clk/3)%50000000)!=0)
		{
			printf("ERROR:a9_clk*2 must 3 times of 50M,a9 clk=%d\n",clk>>1);
			n=6;
		}
	else
		{
			n=(int)((clk/3)/50000000);
		}
	
	selectclk=0;//SYS_div3
	
	WRITE_CBUS_REG(HHI_ETH_CLK_CNTL,
		(n-1)<<0 |
		selectclk<<9 |
		1<<8//enable clk
		); 
	
	//writel(0x70b,(0xc1100000+0x1076*4));  // enable Ethernet clocks   for other clock 600/12 
	//writel(0x107,(0xc1100000+0x1076*4));  // enable Ethernet clocks   for sys clock 1200/3/8
	udelay(100);

	switch (chip) {
		case 72662://7266m
		case 86262://8626m
		case 2952://8626m
			///GPIOD15-24 for 8626M;
			///GPIOD12	nRst;
			///GPIOD13    n_int;
			printf("Set 8626m \n");
			SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_5,0x3ff<<1);
			//set_gpio_mode(PREG_GGPIO,12,GPIO_OUTPUT_MODE);
			//set_gpio_val(PREG_GGPIO,12,0);
			//udelay(100);	//waiting reset end;
			//set_gpio_val(PREG_GGPIO,12,1);
			//udelay(10);	//waiting reset end;


			CLEAR_CBUS_REG_MASK(PREG_GGPIO_EN_N,1<<10);
			CLEAR_CBUS_REG_MASK(PREG_GGPIO_O,1<<10);
			udelay(100);
			SET_CBUS_REG_MASK(PREG_GGPIO_O,1<<10);
			udelay(10);	//waiting reset end;
			break;
		case 72663://7266m--sz
		case 86263://8626m--sz
		case 2953:///8626m--sz	
			///GPIOD15-24 for 8626M;
			///GPIOD12	nRst;
			///GPIOD13    n_int;
			printf("Set 8626m-SZ \n");
			SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_5,0x3ff<<1);
			//set_gpio_mode(PREG_GGPIO,12,GPIO_OUTPUT_MODE);
			//set_gpio_val(PREG_GGPIO,12,0);
			//udelay(100);	//waiting reset end;
			//set_gpio_val(PREG_GGPIO,12,1);
			//udelay(10);	//waiting reset end;


			CLEAR_CBUS_REG_MASK(PREG_HGPIO_EN_N,1<<16);
			CLEAR_CBUS_REG_MASK(PREG_HGPIO_O,1<<16);
			udelay(100);
			SET_CBUS_REG_MASK(PREG_HGPIO_O,1<<16);
			udelay(10);	//waiting reset end;
			break;	
		case 62362://6236m
		case 2951://6236m	
			///GPIOC3-12 for 8626M;
			///GPIOC0_nRst;
			///GPIOC1_nInt;
			//set_mio_mux(3,0x3ff<<1);
				printf("Set 6236m\n");
			SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_3,0x3ff<<1);

			//set_gpio_mode(PREG_FGPIO,0,GPIO_OUTPUT_MODE);
			//set_gpio_val(PREG_FGPIO,0,0);
			//udelay(100);	//waiting reset end;
			//set_gpio_val(PREG_FGPIO,0,1);
			//udelay(10);	//waiting reset end;

			CLEAR_CBUS_REG_MASK(PREG_FGPIO_EN_N,1<<0);
			CLEAR_CBUS_REG_MASK(PREG_FGPIO_O,1<<0);
			udelay(100);
			SET_CBUS_REG_MASK(PREG_FGPIO_O,1<<0);
			udelay(10);	//waiting reset end;
			break;
		default:
			printf("Unsupport MACHID=%d\n",chip);	
		break;
	}
	#endif

	return 0;
}

static unsigned int detect_phyid(void)
{
	unsigned int testval = 0;
	int i;
	static int phy_id=-1;

	if(phy_id!=-1)
		return phy_id;
	for (i = 0; i < 32; i++) 
	{
		testval = phy_reg_rd(i, PHY_SR);	//read the SR register..
		if (testval !=0x0000 && testval !=0xffff) 
		{
			phy_id=i;
			return phy_id;
		}
	}
	return 0xffff;
}

void set_mac_addrs(void *ptr)
{
	unsigned int mac_filter = 0;
	unsigned char * p = (unsigned char *)ptr;
	writel((ETH_MAC_0_Configuration_PS_MII | ETH_MAC_0_Configuration_FES_100M | ETH_MAC_0_Configuration_DM
			| ETH_MAC_0_Configuration_RE | ETH_MAC_0_Configuration_TE), ETH_MAC_0_Configuration);
	writel((ETH_MAC_1_Frame_Filter_PM |ETH_MAC_1_Frame_Filter_RA), ETH_MAC_1_Frame_Filter);

	mac_filter = (p[5]<<8) | p[4];
	writel(mac_filter, ETH_MAC_Addr0_High);
	mac_filter = (p[3]<<24) | (p[2]<<16) | (p[1]<<8) | p[0];
	writel(mac_filter, ETH_MAC_Addr0_Low);
}

static void netdev_chk(void)
{
	unsigned int rint, rint2;
	static unsigned int old_rint=-1;
	unsigned int id;
	int s100,full;

	id=detect_phyid();
	rint2 = 3000;
	if(g_speed_enforce)
	{
		//printf("use enforce net speed\n");
		rint=phy_reg_rd(id,PHY_SR);
		
	}else{
		do {
			rint = phy_reg_rd(id,PHY_SR);
			if((rint & PHY_SR_ANCOMPLETE))
				break;
			udelay(1000);
		} while (rint2-->0);
		if(!(rint & PHY_SR_ANCOMPLETE) )
		printf("phy auto link failed\n");
	}
	
	if(rint!=old_rint )
	{
	    switch(phy_Identifier)
	    	{
	    	case PHY_ATHEROS_8032:
				rint2 = phy_reg_rd(id,17);
				s100 = rint2 & (1<<14);
				full = ((rint2) & (1<<13));
				gS->linked = rint2&(1<<10);
				break;
			case PHY_IC_IP101ALF:
				rint2 = phy_reg_rd(id,1);
				gS->linked = rint2&(1<<2);
				rint2 = phy_reg_rd(id,0);
				s100 = (rint2 & (0x1<<8))? 1:0;
				rint2 = phy_reg_rd(id,5);
				full = (rint2 & (0x1<<7))? 1:0;
				break;
			case PHY_SMSC_8700:
			default:
				rint2 = phy_reg_rd(id,31);
				s100 = rint2 & (1<<3);
				full = ((rint2 >>4) & 1);
				gS->linked = rint2&(1<<2);
				break;
	    	}
		/* phy_auto_negotiation_set */
		if(full)
		{
			printf("duplex\n");
			writel(readl(ETH_MAC_0_Configuration) | ETH_MAC_0_Configuration_DM, ETH_MAC_0_Configuration);
		}
		else
		{
			printf("half duplex\n");
			writel(readl(ETH_MAC_0_Configuration) & ~ ETH_MAC_0_Configuration_DM, ETH_MAC_0_Configuration);
		}
		if(!s100)
		{
			printf("10m\n");
			writel(readl(ETH_MAC_0_Configuration) & ~ ETH_MAC_0_Configuration_FES_100M, ETH_MAC_0_Configuration);
			writel(readl(ETH_PLL_CNTL) & ~ETH_PLL_CNTL_DIVEN, ETH_PLL_CNTL);		// Disable the Ethernet clocks     
			// ---------------------------------------------
			// Test 50Mhz Input Divide by 2
			// ---------------------------------------------
			// Select divide by 20			
			writel(readl(ETH_PLL_CNTL) & ~ETH_PLL_CNTL_DESEND, ETH_PLL_CNTL);  	// desc endianess "same order"
			writel(readl(ETH_PLL_CNTL) & ~ETH_PLL_CNTL_DATEND, ETH_PLL_CNTL); 	// data endianess "little"
			writel(readl(ETH_PLL_CNTL) & ~ETH_PLL_CNTL_MACSPD, ETH_PLL_CNTL);	// divide by 20
			writel(readl(ETH_PLL_CNTL) | ETH_PLL_CNTL_DIVEN, ETH_PLL_CNTL);		// enable Ethernet clocks
		}
		else
		{
			printf("100m\n");
			writel(readl(ETH_MAC_0_Configuration) | ETH_MAC_0_Configuration_FES_100M, ETH_MAC_0_Configuration);	// program mac
			writel(readl(ETH_PLL_CNTL) & ~ETH_PLL_CNTL_DIVEN, ETH_PLL_CNTL);		// Disable the Ethernet clocks     
			// ---------------------------------------------
			// Test 50Mhz Input Divide by 2
			// ---------------------------------------------
			// Select divide by 2		
			writel(readl(ETH_PLL_CNTL) & ~ETH_PLL_CNTL_DESEND, ETH_PLL_CNTL);  	// desc endianess "same order"
			writel(readl(ETH_PLL_CNTL) & ~ETH_PLL_CNTL_DATEND, ETH_PLL_CNTL); 	// data endianess "little"
			writel(readl(ETH_PLL_CNTL) | ETH_PLL_CNTL_MACSPD, ETH_PLL_CNTL);	// divide by 2
			writel(readl(ETH_PLL_CNTL) | ETH_PLL_CNTL_DIVEN, ETH_PLL_CNTL);		// enable Ethernet clocks
		}
		
		/* link_changed */
#if defined(ET_DEBUG)
		unsigned int regs=0,val;		
		for(regs=0; regs<=31; regs++)
		{
			val = phy_reg_rd(id,regs);
			printf("reg[%d]=%x\n",regs,(unsigned)val);
		}
#endif		
		old_rint=rint;
	}
}

/* Reset and idle the chip, putting all registers into
 * a reasonable state */
static int eth_reset(struct _gStruct* emac_config)
{
	int i, k,phyad;
	unsigned int val,ori_ctl_val=0;
	struct _gStruct* m=emac_config;
#define NET_MAX_RESET_TEST	1000	    
	 if(g_speed_enforce) ori_ctl_val=phy_reg_rd(1, PHY_CR);

	 for(i=0;i<NET_MAX_RESET_TEST;i++)
	{
		writel(ETH_DMA_0_Bus_Mode_SWR, ETH_DMA_0_Bus_Mode);
		for (k= 0;k<1000; k++) 
		{
			udelay(100);
			if(!(readl(ETH_DMA_0_Bus_Mode)&ETH_DMA_0_Bus_Mode_SWR))
				break;
		}
		if(k>=1000)
		{
			printf("error to reset mac!\n");
			return -1;
		}
		
	 	phyad=detect_phyid();
		if(phyad>32 || phyad<0)
		{
			continue;
		}
		printf("detected phy id=%d\n",phyad);
	    	val = PHY_SPMD_MIIMODE_RMII | (PHY_MODE_BUS_ALL_AE<<PHY_SPMD_MODE_P)  | (phyad<<PHY_SPMD_PHYAD_P);
		phy_reg_wr(phyad, PHY_SPMD, val);
        	val = phy_reg_rd(phyad,2);
		phy_Identifier=val<<16;
		val = phy_reg_rd(phyad,3);
		phy_Identifier|=val;
		printf("find net phy id=%x\n",phy_Identifier);
	    	// soft reset

		phy_reg_wr(phyad, PHY_CR, PHY_CR_RST);
	      // may be smaller value??  Ask SMSC
		for (k=0; k<1000; k++) 
		{
	    		udelay(1000);
			val = phy_reg_rd(phyad, PHY_CR);
	    		if (!(val & PHY_CR_RST) )
	         		break;
		}
		if(k>=1000)
			continue;
		else
			break;
	} 
	if(i>=NET_MAX_RESET_TEST)
	{
		printf("Error to detected phy\n");
		return -1;
	}
	
	val = PHY_SPMD_MIIMODE_RMII | (PHY_MODE_BUS_ALL_AE<<PHY_SPMD_MODE_P)  | (phyad<<PHY_SPMD_PHYAD_P);
	phy_reg_wr(phyad, PHY_SPMD, val);
	val = PHY_CR_AN | PHY_CR_RSTAN;
	phy_reg_wr(phyad, PHY_CR, val); // we use A/N to send 'link code word'---FLP
	if(g_speed_enforce==1)
	{
		while(!(phy_reg_rd(1,1)&(1<<2))){}; //wait line status stable
		phy_reg_wr(phyad, PHY_CR, ori_ctl_val);
		while(!(phy_reg_rd(1,1)&(1<<2))){}; //wait new state stable
		
	}
	//while(1){};
	udelay(10);
	writel((~0), ETH_DMA_5_Status);							/* clear all status flag */
	writel(0, ETH_DMA_5_Status);
	writel(0, ETH_DMA_6_Operation_Mode);					/* stop RX and TX */
	val=readl(ETH_DMA_8_Missed_Frame_and_Overflow);		/* read to clean */

	writel(0, ETH_DMA_7_Interrupt_Enable);					/* disable all interrupt */
	writel((8<<ETH_DMA_0_Bus_Mode_PBL_P) | ETH_DMA_0_Bus_Mode_FB, ETH_DMA_0_Bus_Mode);

	writel((long)m->rx, ETH_DMA_3_Re_Descriptor_List_Addr);
	writel((long)m->tx, ETH_DMA_4_Tr_Descriptor_List_Addr);

	/* config the interrupt */
	writel(ETH_DMA_7_Interrupt_Enable_TUE | ETH_DMA_7_Interrupt_Enable_TJE
					| ETH_DMA_7_Interrupt_Enable_OVE | ETH_DMA_7_Interrupt_Enable_UNE | ETH_DMA_7_Interrupt_Enable_RIE
					| ETH_DMA_7_Interrupt_Enable_RUE | ETH_DMA_7_Interrupt_Enable_RSE | ETH_DMA_7_Interrupt_Enable_FBE
					| ETH_DMA_7_Interrupt_Enable_AIE | ETH_DMA_7_Interrupt_Enable_NIE, ETH_DMA_7_Interrupt_Enable);
	writel(0, ETH_MAC_Interrupt_Mask);
	printf("eth reset ok\n");
	return 0;
}

static void DMARXStart(void)
{
	writel(readl(ETH_DMA_6_Operation_Mode) | ETH_DMA_6_Operation_Mode_SR, ETH_DMA_6_Operation_Mode);
}

static void DMATXStart(void)
{
	writel(readl(ETH_DMA_6_Operation_Mode) | ETH_DMA_6_Operation_Mode_ST, ETH_DMA_6_Operation_Mode);

}

static void DMARXStop(void)
{
	writel(readl(ETH_DMA_6_Operation_Mode) & ~ETH_DMA_6_Operation_Mode_SR, ETH_DMA_6_Operation_Mode);

}

static void DMATXStop(void)
{
	writel(readl(ETH_DMA_6_Operation_Mode) & ~ETH_DMA_6_Operation_Mode_ST, ETH_DMA_6_Operation_Mode);
}

static void GetDMAStatus(unsigned int* mask,unsigned  int* status)
{
	*mask = readl(ETH_DMA_7_Interrupt_Enable);
	*status = readl(ETH_DMA_5_Status);
}

static void chk_dma_stat(void)
{
	unsigned int mask;
	unsigned int status;

	GetDMAStatus(&mask,&status);

   if(status&NOR_INTR_EN)  //Normal Interrupts Process
   {
   		if(status&TX_INTR_EN)  //Transmit Interrupt Process
   		{
   			writel(1<<0|1<<16, ETH_DMA_5_Status);
			//AVSemPost(tx_sem);
  		}
		if(status&RX_INTR_EN) //Receive Interrupt Process
		{
			writel(1<<6|1<<16, ETH_DMA_5_Status);
		}
		if(status&EARLY_RX_INTR_EN)
		{
			writel(EARLY_RX_INTR_EN|NOR_INTR_EN, ETH_DMA_5_Status);
		}
		if(status&TX_BUF_UN_EN)
		{
#if defined(ET_DEBUG)
				volatile unsigned long Cdma,Dstatus,mstatus,buf_addr,count;
     			Cdma = readl(ETH_DMA_18_Curr_Host_Tr_Descriptor);
     			
     			Dstatus=*(volatile unsigned long *)Cdma;
     			count=*(volatile unsigned long *)(Cdma+4);
     			buf_addr=*(volatile unsigned long *)(Cdma+8);
     			mstatus = readl(ETH_DMA_5_Status);
     			if((Dstatus==0x80000000) && (buf_addr!=0)&& count!=0)
     			{
     				//the apollo can go here.............
     				printf("Txdescriptor is invalid Current DMA=%x,Dstatus=%x\n",Cdma,Dstatus);
     				printf("Current status=%x,%x\n",mstatus);
     				//low_level_resume_output(); 
     			}
#endif
     			writel(1<<2|1<<16, ETH_DMA_5_Status);
		}
	}
	else if(status&ANOR_INTR_EN) //Abnormal Interrupts Process
	{
		if(status&RX_BUF_UN)
		{
			writel(RX_BUF_UN|ANOR_INTR_EN, ETH_DMA_5_Status);
		}
		if(status&RX_STOP_EN)
		{
			writel(RX_STOP_EN|ANOR_INTR_EN, ETH_DMA_5_Status);
		}
		if(status&RX_WATCH_TIMEOUT)
		{
			writel(RX_WATCH_TIMEOUT|ANOR_INTR_EN, ETH_DMA_5_Status);
		}
		if(status&FATAL_BUS_ERROR)
		{
			writel(FATAL_BUS_ERROR|ANOR_INTR_EN, ETH_DMA_5_Status);
			//to do: bus error, reset GMAC and DMA
            	printf("NET DMA FATAL_BUS_ERROR\n");		
            	printf("I restart the network....\n");		
            	eth_reset(gS);
		}
		if(status&EARLY_TX_INTR_EN)
		{
			writel(EARLY_TX_INTR_EN|ANOR_INTR_EN, ETH_DMA_5_Status);
			//AVSemPost(tx_sem);
			//	 NET_PRINT("GMAC's tx FIFO is full \n");  
			//	low_level_resume_output();
		}
		if(status&TX_STOP_EN)
		{
			writel(TX_STOP_EN|ANOR_INTR_EN, ETH_DMA_5_Status);
		}
		if(status&TX_JABBER_TIMEOUT)
		{
			writel(TX_JABBER_TIMEOUT|ANOR_INTR_EN, ETH_DMA_5_Status);
		}
		if(status&RX_FIFO_OVER)
		{
			writel(RX_FIFO_OVER|ANOR_INTR_EN, ETH_DMA_5_Status);	
			DMARXStart();
		}
		if(status&TX_UNDERFLOW)
		{
			writel(TX_UNDERFLOW|ANOR_INTR_EN, ETH_DMA_5_Status);
			DMATXStart();
		}
	}
}

static void aml_eth_halt(struct eth_device * net_current)
{

}

static int aml_eth_send(struct eth_device *net_current, volatile void *packet, int length)
{
	unsigned int mask;
   	unsigned int status;
	if(!initialised)
		return -1;

	netdev_chk();
	
	struct _tx_desc* pTx = current_tx;
	struct _tx_desc* pDma = (struct _tx_desc* )readl(ETH_DMA_18_Curr_Host_Tr_Descriptor);

	if(pDma!=NULL)
		_dcache_inv_range_for_net((unsigned long)pDma,(unsigned long)(pDma)+sizeof(struct _tx_desc)-1);
	if(pDma!=NULL && !(pDma->tdes0 & TDES0_OWN) && pTx !=pDma)
	{
		//this may not happend,if all the hardware work well...
		//to fixed a bug of the dma maybe lost setting some tx buf to own by host,..;
		//start the current_tx at pDMA
		pTx=pDma;	
	}
	if(pDma!=pTx)
		_dcache_inv_range_for_net((unsigned long)pTx,(unsigned long)(pTx+1)-1);

	if (length > ETH_MTU)
	{
		goto err;
	}
	
	if(length<14)
	{
		printf("pbuf len error,len=%d\n", length);
		goto err;
	}

	if(pTx->tdes0 & TDES0_OWN)
	{
#if 1
		volatile unsigned long Cdma,Dstatus,status;
		Cdma = readl(ETH_DMA_18_Curr_Host_Tr_Descriptor);
		Dstatus = readl(Cdma);
		status = readl(ETH_DMA_5_Status);
		printf("Current DMA=%x,Dstatus=%x\n", (unsigned int)Cdma, (unsigned int)Dstatus);
		printf("Current status=%x\n", (unsigned int)status);
		printf("no buffer to send\n");
#endif
		goto err;
	}

	if(!(unsigned char*)pTx->tdes2)
		goto err;
	current_tx=(struct _tx_desc*)pTx->tdes3; 
	memcpy((unsigned char*)pTx->tdes2, (unsigned char*)packet, length);
	_dcache_flush_range_for_net(packet,packet+1500);
	_dcache_inv_range_for_net(packet,packet+1500);//this is the uboot's problem
	//change for add to 60 bytes..
	//by zhouzhi
	if(length<60)
	{
		memset((unsigned char*)(pTx->tdes2 + length), 0, 60-length);
		length=60;
	}
	//pTx->tdes1 &= DescEndOfRing;     
	_dcache_flush_range_for_net((unsigned long)pTx->tdes2, (unsigned long)pTx->tdes2+length-1);	
	pTx->tdes1 = ((length<< TDES1_TBS1_P) & TDES1_TBS1_MASK) | TDES1_FS | TDES1_LS |TDES1_TCH | TDES1_IC;
	pTx->tdes0 = TDES0_OWN;
	_dcache_flush_range_for_net((unsigned long)pTx,(unsigned long)(pTx+1)-1);

	GetDMAStatus(&mask, &status);
	if(status & ETH_DMA_5_Status_TS_SUSP)
		writel(1, ETH_DMA_1_Tr_Poll_Demand);
	else
		DMATXStart();
#ifdef ET_DEBUG
	printf("Transfer starting...\n");
	GetDMAStatus(&mask, &status);
	printf("Current status=%x\n",status);	
#endif
	/* wait for transfer to succeed */
	//unsigned tmo = get_timer (0) + 5 * CONFIG_SYS_HZ;		//5S time out
	unsigned tmo = 0;	
	do
	{
		udelay(100);
		GetDMAStatus(&mask, &status);
		if (tmo++ >= 50000)
		{
			printf ("\ntransmission error %#x\n", status);
			break;
		}
	}while(!((status & ETH_DMA_5_Status_NIS) && (status & ETH_DMA_5_Status_TI)));

	if(status & ETH_DMA_5_Status_NIS)
	{
		if(status & ETH_DMA_5_Status_TI)
			writel(ETH_DMA_5_Status_NIS | ETH_DMA_5_Status_TI, ETH_DMA_5_Status);
		if(status & ETH_DMA_5_Status_TU)
			writel(ETH_DMA_5_Status_NIS | ETH_DMA_5_Status_TU, ETH_DMA_5_Status);
	}

#ifdef ET_DEBUG
	//test
	GetDMAStatus(&mask, &status);
	printf("Current status=%x\n",status);
#endif
	return 0;
err:	   
	return -1;
}

/*
 * each time receive a whole packet
 */
static int aml_eth_rx (struct eth_device * net_current)
{
	unsigned int mask;
   	unsigned int status;
	int rxnum=0;
	int len=0;
	struct _rx_desc* pRx; 
	
	if(!initialised)
		return -1;

	netdev_chk();

	/* Check packet ready or not */
	GetDMAStatus(&mask, &status);
	if(!((status & ETH_DMA_5_Status_NIS) && (status & ETH_DMA_5_Status_RI)))
	{
		return 0;
	}
	writel(ETH_DMA_5_Status_NIS | ETH_DMA_5_Status_RI, ETH_DMA_5_Status);	//clear the int flag

	if(!current_rx)
		current_rx = gS->rx;		
	pRx = current_rx;
	_dcache_inv_range_for_net((unsigned long)pRx,(unsigned long)(pRx+1)-1);
	while(!(pRx->rdes0 & RDES0_OWN))
	{    	         
		len = (pRx->rdes0 & RDES0_FL_MASK)>>RDES0_FL_P;
		if(14>=len)
		{
			printf("err len=%d\n",len);
			goto NEXT_BUF;
		}
		// pbuf_header(pb, -sizeof(short));
		_dcache_inv_range_for_net((unsigned long)pRx->rdes2,(unsigned long)pRx->rdes2+len-1);	

		if(!memcpy((unsigned char*)NetRxPackets[0], (unsigned char*)pRx->rdes2, len))
		{
			printf("memcp error\n");
			goto NEXT_BUF;
		}
		NEXT_BUF:
		pRx->rdes0 = RDES0_OWN;
		_dcache_flush_range_for_net((unsigned long)pRx,(unsigned long)(pRx+1)-1);
		pRx = (struct _rx_desc*)current_rx->rdes3;
		_dcache_inv_range_for_net((unsigned long)pRx,(unsigned long)(pRx+1)-1);
		current_rx = pRx; 
		rxnum++;
		NetReceive (NetRxPackets[0], len);
	}
	
//	NetReceive (NetRxPackets[0], len);
	return len;
}

static int aml_ethernet_init(struct eth_device * net_current, bd_t *bd)
{
	DECLARE_GLOBAL_DATA_PTR;
	unsigned net_dma_start_addr = (unsigned)&__net_dma_start;
	unsigned net_dma_end_addr = (unsigned)&__net_dma_end;
	unsigned tx_start,rx_start;
	struct _rx_desc * pRDesc;
	struct _tx_desc * pTDesc;
	unsigned char * bufptr;
	
	int i;
	printf("Amlogic eth init\n");
	if(initialised)
		return 0;
	
	/* init the dma descriptor */
	gS = (struct _gStruct*)malloc(sizeof(struct _gStruct));
	if(net_dma_start_addr!=0 && 
	(net_dma_end_addr - net_dma_start_addr) > (CTX_BUFFER_NUM+CRX_BUFFER_NUM)*CBUFFER_SIZE+
		CRX_BUFFER_NUM*sizeof(struct _rx_desc)+CTX_BUFFER_NUM*sizeof(struct _tx_desc)){

		rx = (struct _rx_desc*)((unsigned long)net_dma_start_addr + (CTX_BUFFER_NUM+CRX_BUFFER_NUM)*CBUFFER_SIZE);
		tx = (struct _tx_desc*)((char *)rx+CRX_BUFFER_NUM*sizeof(struct _rx_desc));		
	}
	else{
		printf("Error!! Ethernet DMA size is smaller");
		goto error_out;
	}
	gS->rx = rx;
	gS->tx = tx;		
	tx_start = net_dma_start_addr;
	rx_start = net_dma_start_addr + CTX_BUFFER_NUM*CBUFFER_SIZE;
	gS->rx_buf_addr = rx_start;
	gS->tx_buf_addr = tx_start;
	
	/* init mStruct */
	gS->current_rx_des = 0;
	gS->current_tx_des = 0;
	gS->rx_len = CRX_BUFFER_NUM;
	gS->tx_len = CTX_BUFFER_NUM;
	gS->buffer_len = CBUFFER_SIZE;
	gS->rx_frame_num = 0;
	gS->current_tx_ready = 0;
	gS->last_tx_sent = 0;
	gS->last_tx_desc_num = 0;
	gS->irq_handle = -1;
	gS->linked = 0;

	/* init RX desc */
	pRDesc = gS->rx;
	bufptr = (unsigned char *) gS->rx_buf_addr;
	for (i = 0; i < gS->rx_len - 1; i++) 
	{
		pRDesc->rdes0 = RDES0_OWN;
		pRDesc->rdes1 = RDES1_RCH | (gS->buffer_len & RDES1_RBS1_MASK);
		pRDesc->rdes2 = (unsigned long)bufptr;
		pRDesc->rdes3 = (unsigned long)pRDesc+sizeof(struct _rx_desc);
		pRDesc->reverse[0] = 0;
		pRDesc->reverse[1] = 0;
		pRDesc->reverse[2] = 0;
		pRDesc->reverse[3] = 0;
		bufptr += gS->buffer_len; 
		pRDesc = pRDesc+1;
	}
	pRDesc->rdes0 = RDES0_OWN;
	pRDesc->rdes1 = RDES1_RCH | RDES1_RER | (gS->buffer_len & RDES1_RBS1_MASK); //chain buf
	pRDesc->rdes2 = (unsigned long)bufptr;
	pRDesc->rdes3 = (unsigned long)gS->rx; 		//circle
	_dcache_flush_range_for_net((unsigned)gS->rx,(unsigned)gS->rx+sizeof(struct _rx_desc)*gS->rx_len);	

	/* init TX desc */
	pTDesc = (struct _tx_desc *)gS->tx;
	bufptr = (unsigned char *)gS->tx_buf_addr;
	for (i = 0; i < gS->tx_len - 1; i++) 
	{
		pTDesc->tdes0 = 0;
		pTDesc->tdes1 = TDES1_TCH | TDES1_IC;
		pTDesc->tdes2 = (unsigned long)bufptr;
		pTDesc->tdes3 = (unsigned long)pTDesc+sizeof(struct _tx_desc);
		pTDesc->reverse[0] = 0;
		pTDesc->reverse[1] = 0;
		pTDesc->reverse[2] = 0;
		pTDesc->reverse[3] = 0;
		bufptr += gS->buffer_len;
		pTDesc = pTDesc+1;
	}
	pTDesc->tdes0 = 0;
	pTDesc->tdes1 =TDES1_TCH | TDES1_TER | TDES1_IC; 	//chain buf, enable complete interrupt 
	pTDesc->tdes2 = (unsigned long)bufptr;
	pTDesc->tdes3 = (unsigned long)gS->tx; 		//circle
	current_tx = gS->tx;
	_dcache_flush_range_for_net((unsigned)gS->tx,(unsigned)gS->tx+sizeof(struct _tx_desc)*gS->tx_len);		

	/* peripheral io init */
	eth_io_init();

	/* reset */
	
	eth_reset(gS);
#ifndef CONFIG_RANDOM_MAC_ADDR	
	/* set mac addr */
	char *s = getenv("ethaddr");
	char *e;
	for (i = 0; i < 6; ++i) {
		gd->bd->bi_enetaddr[i] = s ? simple_strtoul(s, &e, 16) : 0;
		if (s)
			s = (*e) ? e + 1 : e;
	}
	set_mac_addrs(gd->bd->bi_enetaddr);
	//set_mac_addrs(def_mac);
	/* get the mac and ip */
	printf("MAC address is %02x:%02x:%02x:%02x:%02x:%02x\n", gd->bd->bi_enetaddr[0], gd->bd->bi_enetaddr[1], gd->bd->bi_enetaddr[2], gd->bd->bi_enetaddr[3], gd->bd->bi_enetaddr[4], gd->bd->bi_enetaddr[5]);	
#endif
	/* start the dma para, but don't start the receive dma */
	writel(ETH_DMA_6_Operation_Mode_EFC | ETH_DMA_6_Operation_Mode_TTC_16 | ETH_DMA_6_Operation_Mode_RSF | ETH_DMA_6_Operation_Mode_DT, ETH_DMA_6_Operation_Mode);
					// | ETH_DMA_6_Operation_Mode_RTC_32 | ETH_DMA_6_Operation_Mode_FUF
	
	netdev_chk();
	DMARXStart();

	initialised = 1;
	return (0);
error_out:
	return -1;
}

int aml_eth_init(bd_t *bis)
{
	struct eth_device *dev;
	dev = (struct eth_device *) malloc(sizeof(*dev)); 
	memset(dev, 0, sizeof(struct eth_device));
	sprintf(dev->name,"Apollo EMAC");
	dev->init		= aml_ethernet_init;
	dev->halt 	= aml_eth_halt;
	dev->send	= aml_eth_send;
	dev->recv	= aml_eth_rx;
	return eth_register(dev);
	
}

static int do_phyreg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int phyad, reg, value;
	unsigned char *cmd = NULL;
	unsigned int i;

	if (argc  < 2) {
		return cmd_usage(cmdtp);
	}

	//phyad = simple_strtoul(argv[1], NULL, 16);
	phyad = 1;

	cmd = argv[1];
	switch (*cmd) {
	case 'd':
		printf("=== ethernet phy register dump:\n");
		for (i = 0; i < 32; i++)
			printf("[reg_%d] 0x%x\n", i, phy_reg_rd(phyad, i));
		break;
	case 'r':
		if (argc != 3) {
			return cmd_usage(cmdtp);
		}
		printf("=== ethernet phy register read:\n");
		reg = simple_strtoul(argv[2], NULL, 10);
		printf("[reg_%d] 0x%x\n", reg, phy_reg_rd(phyad, reg));

		break;
	case 'w':
		if (argc != 4) {
			return cmd_usage(cmdtp);
		}
		printf("=== ethernet phy register write:\n");
		reg = simple_strtoul(argv[2], NULL, 10);
		value = simple_strtoul(argv[3], NULL, 16);
		phy_reg_wr(phyad, reg, value);
		printf("[reg_%d] 0x%x\n", reg, phy_reg_rd(phyad, reg));
		break;

	default:
		return cmd_usage(cmdtp);
	}

	return 0;
}

static int do_macreg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int reg, value;
	unsigned char *cmd = NULL;
	unsigned int i;

	if (argc  < 2) {
		return cmd_usage(cmdtp);
	}

	cmd = argv[1];
	switch (*cmd) {
	case 'd':
		printf("=== ETH_MAC register dump:\n");
		for (i = 0x0000; i <= 0x00FC; i += 0x4)
			printf("[0x%04x] 0x%x\n", i, readl(ETH_BASE + i));
		printf("=== ETH_MMC register dump:\n");
		for (i = 0x0100; i <= 0x0284; i += 0x4)
			printf("[0x%04x] 0x%x\n", i, readl(ETH_BASE + i));
		printf("=== ETH_DMA register dump:\n");
		for (i = 0x1000; i <= 0x1054; i += 0x4)
			printf("[0x%04x] 0x%x\n", i, readl(ETH_BASE + i));
		break;
	case 'r':
		if (argc != 3) {
			return cmd_usage(cmdtp);
		}
		printf("=== ethernet mac register read:\n");
		reg = simple_strtoul(argv[2], NULL, 10);
		printf("[0x%04x] 0x%x\n", i, readl(ETH_BASE + reg));

		break;
	case 'w':
		if (argc != 4) {
			return cmd_usage(cmdtp);
		}
		printf("=== ethernet mac register write:\n");
		reg = simple_strtoul(argv[2], NULL, 10);
		value = simple_strtoul(argv[3], NULL, 16);
		writel(value, ETH_BASE + reg);
		printf("[0x%04x] 0x%x\n", reg, value);
		break;

	default:
		return cmd_usage(cmdtp);
	}

	return 0;
}

static int do_cbusreg(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int reg, value;
	unsigned char *cmd = NULL;
	unsigned int i;
	if (argc < 2) {
		return cmd_usage(cmdtp);
	}

	cmd = argv[1];
	switch (*cmd) {
	case 'r':
		printf("=== cbus register read:\n");
		reg = simple_strtoul(argv[2], NULL, 16);
		printf("[0x%04x] 0x%x\n", reg, READ_CBUS_REG(reg));

		break;
	case 'w':
		printf("=== cbus register write:\n");
		reg = simple_strtoul(argv[2], NULL, 16);
		value = simple_strtoul(argv[3], NULL, 16);
		WRITE_CBUS_REG(reg, value);
		printf("[0x%04x] 0x%x\n", reg, READ_CBUS_REG(reg));
		break;

	default:
		return cmd_usage(cmdtp);
	}

	return 0;
}

//loopback test.
static int do_autoping(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int value;
	char buffer[40];

	if (argc  < 2) {
		return cmd_usage(cmdtp);
	}
	value = phy_reg_rd(1, PHY_CR);

	phy_reg_wr(1, PHY_CR, value | (1 << 14)); //phy loopback
	while (1) {
		if (had_ctrlc()) {
			printf("Quit autoping...\n");
			return 0;
		}
		sprintf(buffer, "ping %s ", argv[1]);
		run_command(buffer, 0);
	}
	return 0;
}
//loopback test.
static int do_mdc_clk(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int value;
	char buffer[40];

	if (argc  < 2) {
		return cmd_usage(cmdtp);
	}

	g_mdc_clock_range=((simple_strtoul(argv[1], NULL, 16)&0xf)<<2);
	printf("set value:0x%x\n",g_mdc_clock_range);
	return 0;
}
int do_ethchk(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	netdev_chk();
	return 0;
}

static int do_ethrst(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	eth_reset(gS);
	return 0;
}
static int  do_netspeed(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int speed=100;
	int full=0;
	int data=0;
	
	if (argc  < 2) {

		switch(phy_Identifier)
	    	{
	    	case PHY_ATHEROS_8032:
		break;
		case PHY_IC_IP101ALF:
		break;
		case PHY_SMSC_8700:
		default:
		data = phy_reg_rd(1,31);
		speed = data & (1<<3);
		full = ((data >>4) & 1);
		printf("status: %sM %s-duplex\n",(speed?"100":"10"),(full?"full":"half"));
		break;
	    	}
		return cmd_usage(cmdtp);
	}
	speed = simple_strtoul(argv[1], NULL, 10);
	if((speed!=0)&&(speed!=100)&&(speed!=10)) return -1;
	data= phy_reg_rd(1,0);
	if(speed == 0)//disable net speed enforce mode,ie.  we enable A/N
	{
		g_speed_enforce=0;
		data |=(1<<12)|(1<<9); 
	}else{
		g_speed_enforce=1;
		data &=~(1<<12); 
		if(speed==100)//100M
		{
			data |= (1<<13) ;
		}else if(speed == 10 ){ //10M
			data &=~(1<<13);
		}
		if(strncmp(argv[2],"fu",2)==0)
		{
			data |= (1<<8);
		}else{
			data &=~(1<<8);
		}
	}
	phy_reg_wr(1, 0, data);
	return 0;
	
}
U_BOOT_CMD(
	netspd_f, 3, 1, do_netspeed,
	"enforce eth speed",
	"0            - disable enforce mode,enable A/N\n"
	"10  full   - enforce 10M full duplex mode\n"
	"10  half   - enforce 10M half duplex mode\n"
	"100 full   - enforce 100M full duplexmode\n"
	"100 half   - enforce 100M half duplexmode\n"
);
U_BOOT_CMD(
	phyreg, 4, 1, do_phyreg,
	"ethernet phy register read/write/dump",
	"d            - dump phy registers\n"
	"       r reg        - read phy register\n"
	"       w reg val    - write phy register"
);

U_BOOT_CMD(
	macreg, 4, 1, do_macreg,
	"ethernet mac register read/write/dump",
	"d            - dump mac registers\n"
	"       r reg        - read mac register\n"
	"       w reg val    - write mac register"
);

U_BOOT_CMD(
	cbusreg, 4, 1, do_cbusreg,
	"cbus register read/write",
	"r reg        - read cbus register\n"
	"        w reg val    - write cbus register"
);
U_BOOT_CMD(
	mdc_clk, 2, 1, do_mdc_clk,
	"do mdc clock",
	"mdc  data-- data is ETH_MAC_4_GMII_Addr[2..5]  "
);
U_BOOT_CMD(
	autoping, 2, 1, do_autoping,
	"do auto ping test",
	"ip"
);

U_BOOT_CMD(
	ethchk, 1, 1, do_ethchk,
	"check ethernet status",
	""
);

U_BOOT_CMD(
	ethrst, 1, 1, do_ethrst,
	"reset ethernet phy",
	"             - reset etherent phy\n"
);
