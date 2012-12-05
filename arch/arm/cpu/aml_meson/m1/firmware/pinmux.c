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
#define P_PREG_JTAG_GPIO_ADDR                       0xc110802c
#define P_PREG_FGPIO_EN_N                           0xc110803c
#define P_PREG_FGPIO_O                              0xc1108040
#define P_PREG_FGPIO_I                              0xc1108044
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
//    clrbits_le32(P_PERIPHS_PIN_MUX_5,0x7fff);
//    //enable spie
//    setbits_le32(P_PERIPHS_PIN_MUX_2,0x3f<<7);

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
    

//    //enable power
/*    (*P_PREG_JTAG_GPIO_ADDR)&= ~((1<<20)   // test_n_gpio_o
                           |(1<<16)); // test_n_gpio_en_n*/
		clrbits_le32(P_PREG_JTAG_GPIO_ADDR,
                           (1<<16));
        
    switch(POR_GET_SDIO_CFG(por_config))
    {
        case POR_SDIO_A_ENABLE://SDIOA,GPIOX_31~GPIOX_36
            clrbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<23);
            clrbits_le32(P_PREG_EGPIO_EN_N,(0x3f<<13));
            clrbits_le32(P_PREG_EGPIO_O,(0x3f<<13));
            break;
        case POR_SDIO_B_ENABLE://SDIOB,GPIOX_37~GPIOX_42
            clrbits_le32(P_PERIPHS_PIN_MUX_1,0x3f);
            clrbits_le32(P_PREG_EGPIO_EN_N,(0x3f<<4));
            clrbits_le32(P_PREG_EGPIO_O,(0x3f<<4));
            break;
        case POR_SDIO_C_ENABLE:
            clrbits_le32(P_PERIPHS_PIN_MUX_2,(0xf<<16)|(1<<12)|(1<<8));//(0x3f<<21)
            clrbits_le32(P_PREG_EGPIO_EN_N,(0x3f<<21));
            clrbits_le32(P_PREG_EGPIO_O,(0x3f<<21));
            break;
        default://SDIOC, GPIOX_17~GPIOX_22, iNAND or eMMC, don't need to send CARD_EN
            // disable SPI and SDIO_B
            clrbits_le32(P_PERIPHS_PIN_MUX_7,(0x3f<<24));
            break;
    }
    setbits_le32(P_PREG_JTAG_GPIO_ADDR,
                           (1<<20));
    
}
static inline unsigned enable_sdio(unsigned por_config)
{
    unsigned SD_boot_type=2;

//    //enable power
/*    (*P_PREG_JTAG_GPIO_ADDR)&= ~((1<<20)   // test_n_gpio_o
                           |(1<<16)); // test_n_gpio_en_n*/
		clrbits_le32(P_PREG_JTAG_GPIO_ADDR,(1<<20)   // test_n_gpio_o
                           |(1<<16));
    switch(POR_GET_SDIO_CFG(por_config))
    {
        case POR_SDIO_A_ENABLE://SDIOA,GPIOX_31~GPIOX_36
            setbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<23);
            SD_boot_type=0;
            break;
        case POR_SDIO_B_ENABLE://SDIOB,GPIOX_37~GPIOX_42
            //diable SDIO_B1
            clrbits_le32(P_PERIPHS_PIN_MUX_7,(0x3f<<24));
            setbits_le32(P_PERIPHS_PIN_MUX_1,0x3f);
            SD_boot_type=1;
            break;
        case POR_SDIO_C_ENABLE:
            setbits_le32(P_PERIPHS_PIN_MUX_2,(0xf<<16)|(1<<12)|(1<<8));
            SD_boot_type=2;
            break;
        default://SDIOC, GPIOX_17~GPIOX_22, iNAND or eMMC, don't need to send CARD_EN
            // disable SPI and SDIO_B
            clrbits_le32(P_PERIPHS_PIN_MUX_1,( (1<<29) | (1<<27) | (1<<25) | (1<<23)|0x3f));
            clrbits_le32(P_PERIPHS_PIN_MUX_6,0x7fff);
            setbits_le32(P_PERIPHS_PIN_MUX_7,(0x3f<<24));
            SD_boot_type=1;
            break;
    }
    return SD_boot_type;
}
STATIC_PREFIX int sdio_get_port(unsigned por_config)
{
    switch(POR_GET_SDIO_CFG(por_config))
    {
        case POR_SDIO_A_ENABLE://SDIOA,GPIOX_31~GPIOX_36
            return 0;
        case POR_SDIO_B_ENABLE:
            return 1;
        case POR_SDIO_C_ENABLE:
            return 2;
    }
    return 1;
}



