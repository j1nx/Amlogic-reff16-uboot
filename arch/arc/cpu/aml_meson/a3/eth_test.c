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

static void	udelay        (unsigned long u)
{
	volatile register  int i;
	register int d=u;
	for(i=0;i<d*600/2;i++);
}
int eth_io_init(void)
{

	int chip=86262;

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


	writel(0x70b,(0xc1100000+0x1076*4));  // enable Ethernet clocks   for other clock 600/12 
	//writel(0x107,(0xc1100000+0x1076*4));  // enable Ethernet clocks   for sys clock 1200/3/8
	udelay(100);

	switch (chip) {
		case 86262://8626m
			///GPIOD15-24 for 8626M;
			///GPIOD12	nRst;
			///GPIOD13    n_int;
			uart_puts("Set 8626m \n");
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
		case 62362://6236m
			///GPIOC3-12 for 8626M;
			///GPIOC0_nRst;
			///GPIOC1_nInt;
			//set_mio_mux(3,0x3ff<<1);
				uart_puts("Set 6236m\n");
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
		break;
		;
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


	writel(0x70b,(0xc1100000+0x1076*4));  // enable Ethernet clocks   for other clock 600/12 
	//writel(0x107,(0xc1100000+0x1076*4));  // enable Ethernet clocks   for sys clock 1200/3/8
	udelay(100);
	return 0;
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
	reg4 = phyaddr | phyreg | ETH_MAC_4_GMII_Addr_CR_100_150 | ETH_MAC_4_GMII_Addr_GB;
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

	reg4 = phyaddr | phyreg | ETH_MAC_4_GMII_Addr_CR_100_150 | ETH_MAC_4_GMII_Addr_GW | ETH_MAC_4_GMII_Addr_GB;
	writel(val, ETH_MAC_5_GMII_Data);
	writel(reg4, ETH_MAC_4_GMII_Addr);
	busy = 1;
	while (busy) 
	{
		tmp = readl(ETH_MAC_4_GMII_Addr);
		busy = tmp&1;
	}
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
#define PHY_SMSC_8700			0x7c0c4
#define PHY_ATHEROS_8032		0x004dd023
/* Reset and idle the chip, putting all registers into
 * a reasonable state */
 unsigned long phy_Identifier;
static int eth_reset(void)
{
	int i, k,phyad;
	unsigned int val;
#define NET_MAX_RESET_TEST	100	    
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
			uart_puts("error to reset mac!\n");
			return -1;
		}
		
	 	phyad=detect_phyid();
		if(phyad>32 || phyad<0)
		{
			//uart_puts("Find ERROR Net PHY id=");
			//uart_put_num(phyad);
			//uart_puts("\n");
			continue;
		}
		
	     uart_puts("Find Net PHY\n");
	      uart_put_num(phyad);
		 uart_puts("\n");
	    val = PHY_SPMD_MIIMODE_RMII | (PHY_MODE_BUS_ALL_AE<<PHY_SPMD_MODE_P)  | (phyad<<PHY_SPMD_PHYAD_P);
		phy_reg_wr(phyad, PHY_SPMD, val);
        val = phy_reg_rd(phyad,2);
		phy_Identifier=val<<16;
		val = phy_reg_rd(phyad,3);
		phy_Identifier|=val;
		if(PHY_SMSC_8700==phy_Identifier)
			uart_puts("Find Net 8700\n");
		else if(PHY_ATHEROS_8032==phy_Identifier)
			uart_puts("Find Net 8032\n");
			
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
		 uart_puts("Can't Find Net PHY\n");
		return -1;
	}

	val = PHY_SPMD_MIIMODE_RMII | (PHY_MODE_BUS_ALL_AE<<PHY_SPMD_MODE_P)  | (phyad<<PHY_SPMD_PHYAD_P);
	phy_reg_wr(phyad, PHY_SPMD, val);
	
	val = PHY_CR_AN | PHY_CR_RSTAN;
	phy_reg_wr(phyad, PHY_CR, val);

	udelay(10);
	writel((~0), ETH_DMA_5_Status);							/* clear all status flag */
	writel(0, ETH_DMA_5_Status);
	writel(0, ETH_DMA_6_Operation_Mode);					/* stop RX and TX */
	val=readl(ETH_DMA_8_Missed_Frame_and_Overflow);		/* read to clean */

	writel(0, ETH_DMA_7_Interrupt_Enable);					/* disable all interrupt */
	writel((8<<ETH_DMA_0_Bus_Mode_PBL_P) | ETH_DMA_0_Bus_Mode_FB, ETH_DMA_0_Bus_Mode);

//	writel((long)m->rx, ETH_DMA_3_Re_Descriptor_List_Addr);
//	writel((long)m->tx, ETH_DMA_4_Tr_Descriptor_List_Addr);
 
	/* config the interrupt */
	writel(ETH_DMA_7_Interrupt_Enable_TUE | ETH_DMA_7_Interrupt_Enable_TJE
					| ETH_DMA_7_Interrupt_Enable_OVE | ETH_DMA_7_Interrupt_Enable_UNE | ETH_DMA_7_Interrupt_Enable_RIE
					| ETH_DMA_7_Interrupt_Enable_RUE | ETH_DMA_7_Interrupt_Enable_RSE | ETH_DMA_7_Interrupt_Enable_FBE
					| ETH_DMA_7_Interrupt_Enable_AIE | ETH_DMA_7_Interrupt_Enable_NIE, ETH_DMA_7_Interrupt_Enable);
	writel(0, ETH_MAC_Interrupt_Mask);
	return 0;
}

int eth_test_init(void )
{
int ret; 
eth_io_init();
ret=eth_reset();

if(ret==0)
		uart_puts("eth_reset ok\n");
else
		uart_puts("eth_reset failed\n");
udelay(1000);/*delay1s*/
}
