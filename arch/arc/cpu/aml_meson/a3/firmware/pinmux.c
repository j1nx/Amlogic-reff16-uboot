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
#include <asm/arch/firm/config.h>
#include <asm/arch/romboot.h>
/*
#define BOARD_UART_PORT UART_PORT_0
STATIC_PREFIX void serial_hw_init(void)
{
    setbits_le32(P_PERIPHS_PIN_MUX_2,((1<<15)|(1<<11)));
    ///TODO , add pinmux setting
}*/
STATIC_PREFIX void spi_pinmux_init(void)
{
    // Disable NAND pin select just for safe or potential easy debug if system not switched off
    clrbits_le32(P_PERIPHS_PIN_MUX_4,0x3f<<14);
    // Enable SPI pin select
    setbits_le32(P_PERIPHS_PIN_MUX_6,1<<28);
    setbits_le32(P_PERIPHS_PIN_MUX_5,0x1f<<1);


}
STATIC_PREFIX void nf_pinmux_init(void)
{
//    // disable SPI
//	clrbits_le32(P_PERIPHS_PIN_MUX_1,(( (1<<29) | (1<<27) | (1<<25) | (1<<23))));
//	setbits_le32(P_PERIPHS_PIN_MUX_1,(1<<30) | (1<<28) | (1<<26) | (1<<24));
//	// Enable NAND
//	setbits_le32(P_PERIPHS_PIN_MUX_6,0x7ff);
////	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,0x7fff);
//	//card
//	clrbits_le32(P_PERIPHS_PIN_MUX_7,((1<<29) |(1<<28)|(1<<27)|(1<<26)|(1<<25)|(1<<24)));
}
STATIC_PREFIX void power_on_ext_dev(void)
{
    
    clrbits_le32(P_PREG_JTAG_GPIO_ADDR,
            (1<<20)     // test_n_gpio_o
            |(1<<16)); // test_n_gpio_en_n
}
STATIC_PREFIX int sdio_get_port(unsigned por_config)
{
    int dev=romboot_info->boot_id?POR_GET_EXT_CFG(por_config):POR_GET_INTL_CFG(por_config);
    return dev^(3);
    
}
STATIC_PREFIX unsigned enable_sdio(unsigned por_config)
{
    unsigned dev=sdio_get_port(por_config);
    switch(dev)
    {
        case POR_INTL_SDIO_C:
            //disable nand data pin select
            clrbits_le32(P_PERIPHS_PIN_MUX_4,1<<11);
            //set sdio C pinmux
            setbits_le32(P_PERIPHS_PIN_MUX_4,0x3f<<22);

            break;
        case POR_EXT_SDIO_B:
            setbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<20);
            break;
        case POR_EXT_SDIO_A:
            setbits_le32(P_PERIPHS_PIN_MUX_2,0x3f);
            break;
    }
    return dev;

}





