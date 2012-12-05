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



int audio_init(void )
{
	int m,n;
	int chip=62362;
	switch(chip)
		{
		case 62362:

		//Pinmux
		SET_CBUS_REG_MASK(0x202d,(1<<6)| (1<<11)| (1<<15)| (1<<19));

		}

		//audio_pll_clk
		//XTAL->pll_fin_audio
		WRITE_CBUS_REG(0x105e,(0<<14)| (0<<15)| (1<<11));

		//audio_pll_clk=pll_fin_audio*m/n
		m=16;
		n=1;
		/// 25*16=400Mhz
		WRITE_CBUS_REG(0x105b,(n<<9)| (m<<0));

		WRITE_CBUS_REG(0x1516,1);//enable the audio i2S
///
}

int wifi_init(void )
{
	int n, m,od;
	n=1;
	m=48;
	od=0;

	//clk=freq*m/n
	WRITE_CBUS_REG(0x106d,
	(n&0x1f)<<9 |
	(m&0x1ff) |
	od<<16
	);

	SET_CBUS_REG_MASK(0x106f,
	3<<26
	);



///
}


int video_init()
{
	int n, m,od,od1,xd;
	int n1, m1;
	n=3;
	m=71;
	od=1;
	od1=1;
	xd=2;
	WRITE_CBUS_REG(0x105c,
	n<<9 |
	m<<0|
//	1<<15 |//disable pll 
	od<<16 |
	od1<<18 |
	xd<<20
	);
	
	CLEAR_CBUS_REG_MASK(0x105c,
		1<<15
		);/*enable pll*/


	/*
31 R/W 0 Enable
30 R/W 0 Unused
29-28 R/W 0 Mode: This value determines the operating mode of the modulator:
0: Fixed: use the Nominal M/N values
1: Toggle between the Nominal M/N values and the Low M/N values
2: Toggle between the Nominal M/N values and the High M/N values
3: Toggle between the Low M/N values and the High M/N Values
27-23 R/W 2 N-High
22-14 R/W 25 M-High
13-9 R/W 2 N-Low
8-0 R/W 25 M-Low
	
	*/
int mode;
	m=20;
	n=1;
	m1=20;
	n1=1;
	mode=0;
		WRITE_CBUS_REG(0x1084,
	m<<0 |
	n<<9 |
	m1<<14 |
	n1<<23 |
	mode<<28
	);
	

///HDMI PLL control
		WRITE_CBUS_REG(0x107c,
			4<<0 | //predi
			5<<8 | //Postdiv
			2<<16 | //N
			0<<24  //OD
			);
	///HDMI PLL control2
			WRITE_CBUS_REG(0x107e,
		1<<3 | //IREXT_PU
		1<<5 | //VBG_PU
		1<<6 | //IBIPT_SEL
		1<<8  //IBIC_SEL
		);
	
}
void audio_test(void )
{
audio_init();
wifi_init();
video_init();
serial_puts("audio int end\n");
}
