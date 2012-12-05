/*******************************************************************
 *
 *  Copyright C 2010 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: Serial driver.
 *
 *  Author: Jerry Yu
 *  Created: 2009-3-12
 *
 *******************************************************************/
#include <config.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/uart.h>
#include <asm/arch/firm/io.h>
#ifndef FIRMWARE_IN_ONE_FILE
#define STATIC_PREFIX
#else
#define STATIC_PREFIX static
#endif
/*
#define BOARD_UART_PORT UART_PORT_0
STATIC_PREFIX void serial_hw_init(void)
{
    setbits_le32(P_PERIPHS_PIN_MUX_2,((1<<15)|(1<<11)));
    ///TODO , add pinmux setting
}*/
STATIC_PREFIX void spi_pinmux_init()
{
//    // disable NAND
//    //enable spie
    clrbits_le32(P_PERIPHS_PIN_MUX_2,7<<19);
    clrsetbits_le32(P_PERIPHS_PIN_MUX_5,(0xf<<6),(0xf));

}
STATIC_PREFIX void nf_pinmux_init()
{
    // disable SPI
	clrbits_le32(P_PERIPHS_PIN_MUX_1,(( (1<<29) | (1<<27) | (1<<25) | (1<<23))));
	setbits_le32(P_PERIPHS_PIN_MUX_1,(1<<30) | (1<<28) | (1<<26) | (1<<24));
	// Enable NAND
	setbits_le32(P_PERIPHS_PIN_MUX_6,0x7ff);
//	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,0x7fff);
	//card
	clrbits_le32(P_PERIPHS_PIN_MUX_7,((1<<29) |(1<<28)|(1<<27)|(1<<26)|(1<<25)|(1<<24)));
}
static inline void disable_sdio(unsigned por_config)
{
    
    //disable power
    setbits_le32(P_PREG_PAD_GPIO5_O,(1<<31));
    clrbits_le32(P_PREG_PAD_GPIO5_EN_N,(1<<31));
        
    switch(POR_GET_2ND_CFG(por_config))
    {
        case POR_2ND_SDIO_A://SDIOA,GPIOX_31~GPIOX_36
            clrbits_le32(P_PERIPHS_PIN_MUX_8,0x3f);
            break;
        case POR_2ND_SDIO_B://SDIOB,GPIOX_37~GPIOX_42
            clrbits_le32(P_PERIPHS_PIN_MUX_2,0x3f<<10);
            break;
        case POR_2ND_SDIO_C:
            clrbits_le32(P_PERIPHS_PIN_MUX_6,(0x3f<<24));
            break;
    }
}
static inline unsigned enable_sdio(unsigned por_config)
{
    unsigned SD_boot_type=2;

//    //enable power
    clrbits_le32(P_PREG_PAD_GPIO5_O,(1<<31));
    clrbits_le32(P_PREG_PAD_GPIO5_EN_N,(1<<31));
                           
    switch(POR_GET_2ND_CFG(por_config))
    {
        case POR_2ND_SDIO_A://SDIOA,GPIOX_31~GPIOX_36
            setbits_le32(P_PERIPHS_PIN_MUX_8,0x3f);
            SD_boot_type=0;
            break;
        case POR_2ND_SDIO_B://SDIOB,GPIOX_37~GPIOX_42
            setbits_le32(P_PERIPHS_PIN_MUX_2,0x3f<<10);
            SD_boot_type=1;
            break;
        case POR_2ND_SDIO_C:
            clrbits_le32(P_PERIPHS_PIN_MUX_2,(0x1f<<22));
            setbits_le32(P_PERIPHS_PIN_MUX_6,(0x3f<<24));
            SD_boot_type=2;
            break;
    }
    return SD_boot_type;
}
STATIC_PREFIX int sdio_get_port(unsigned por_config)
{
    switch(POR_GET_2ND_CFG(por_config))
    {
        case POR_2ND_SDIO_A://SDIOA,GPIOX_31~GPIOX_36
            return 0;
        case POR_2ND_SDIO_B:
            return 1;
        case POR_2ND_SDIO_C:
            return 2;
    }
    return 1;
}



