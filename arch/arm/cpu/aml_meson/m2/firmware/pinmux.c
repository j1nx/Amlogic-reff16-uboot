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

//STATIC_PREFIX void serial_hw_init(void)
//{
//    setbits_le32(P_PERIPHS_PIN_MUX_2,((1<<15)|(1<<11)));
//    ///TODO , add pinmux setting
//}
STATIC_PREFIX void spi_pinmux_init()
{
    // disable NAND
    clrbits_le32(P_PERIPHS_PIN_MUX_5,0x7fff);
    //enable spie
    setbits_le32(P_PERIPHS_PIN_MUX_2,0x3f<<7);

}
STATIC_PREFIX void nf_pinmux_init()
{
    // enable NAND
    setbits_le32(P_PERIPHS_PIN_MUX_5,0x7fff);
    //disable spie
    clrbits_le32(P_PERIPHS_PIN_MUX_2,0x3f<<7);
}
static inline unsigned enable_sdio(unsigned por_config)
{
    unsigned SD_boot_type=2;
    register int card_en_sel = (por_config>>3)&0x1;
    // disbale  NAND
    clrbits_le32(P_PERIPHS_PIN_MUX_5,0x7fff);
    //disable spie
    clrbits_le32(P_PERIPHS_PIN_MUX_2,0x3f<<7);
    switch((por_config>>8)&0x3)
    {
        case 0x3://SDIOA,GPIOX_31~GPIOX_36
            clrbits_le32(P_PREG_EGPIO_EN_N,(0x1<<31));
            clrbits_le32(P_PREG_EGPIO_O,(0x1<<31));
            clrbits_le32(P_PREG_FGPIO_EN_N,0x1f);
            clrbits_le32(P_PREG_FGPIO_O,0x1f);
            
            
            if (card_en_sel)
            {
                clrbits_le32(P_PREG_EGPIO_EN_N,(0x1<<8));
                clrbits_le32(P_PREG_EGPIO_O,(0x1<<8));
            
                
            }
            else
            {
                clrbits_le32(P_PREG_FGPIO_EN_N,(0x1<<25));
                clrbits_le32(P_PREG_FGPIO_O,(0x1<<25));
            }
            setbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<10);
            SD_boot_type=0;
            break;
        case 0x1://SDIOB,GPIOX_37~GPIOX_42
            clrbits_le32(P_PREG_FGPIO_EN_N,(0x3f<<5));
            clrbits_le32(P_PREG_FGPIO_O,(0x3f<<5));
            
            
            if (card_en_sel)
            {
                clrbits_le32(P_PREG_EGPIO_EN_N,(0x1<<8));
                clrbits_le32(P_PREG_EGPIO_O,(0x1<<8));
            
                
            }
            else
            {
                clrbits_le32(P_PREG_FGPIO_EN_N,(0x1<<25));
                clrbits_le32(P_PREG_FGPIO_O,(0x1<<25));
            }
            setbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<22);
            SD_boot_type=1;
            break;
        default://SDIOC, GPIOX_17~GPIOX_22, iNAND or eMMC, don't need to send CARD_EN
            clrbits_le32(P_PREG_EGPIO_EN_N,(0xff<<15));
            clrsetbits_le32(P_PREG_EGPIO_O,(0xff<<15),(0x3<<15));
            
            setbits_le32(P_PERIPHS_PIN_MUX_2,0x3f);
            SD_boot_type=2;
            break;
    }
    return SD_boot_type;
}
static inline void disable_sdio(unsigned por_config)
{
    register int card_en_sel = (por_config>>3)&0x1;
    if (card_en_sel)
    {
                clrbits_le32(P_PREG_EGPIO_EN_N,(0x1<<8));
                setbits_le32(P_PREG_EGPIO_O,(0x1<<8));
            
                
    }
    else
   {
        clrbits_le32(P_PREG_FGPIO_EN_N,(0x1<<25));
        setbits_le32(P_PREG_FGPIO_O,(0x1<<25));
   }

    switch((por_config>>8)&0x3)
    {
        case 0x3://SDIOA,GPIOX_31~GPIOX_36
            clrbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<10);
            break;
        case 0x1://SDIOB,GPIOX_37~GPIOX_42
            
            
            clrbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<22);
            break;
        default://SDIOC, GPIOX_17~GPIOX_22, iNAND or eMMC, don't need to send CARD_EN
            
            break;
    }
}

STATIC_PREFIX int sdio_get_port(unsigned por_config)
{
    switch((por_config>>8)&0x3)
    {
        case 0x3://SDIOA,GPIOX_31~GPIOX_36
            return 0;
        case 0x1:
            return 1;
    }
    return 2;
}



