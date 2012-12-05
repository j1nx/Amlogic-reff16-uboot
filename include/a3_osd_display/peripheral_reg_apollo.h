/*******************************************************************
 *
 *  Copyright C 2005 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: Apollo peripheral registers.
 *
 *  Author: Amlogic Software
 *  Created: 10/17/2008 3:53:53 PM
 *
 *******************************************************************/

#ifndef PERIPHERAL_REG_APOLLO_H
#define PERIPHERAL_REG_APOLLO_H
//[8:4] timebase select .
//
#define PREG_CTLREG0_ADDR             0x2000
//========================================================================
//  JTAG GPIO
//========================================================================
#define PREG_JTAG_GPIO                0x200b
    #define JTAG_GPIO_TESTN_OUTLVL_BIT  20
    #define JTAG_GPIO_TESTN_OE_BIT      16
    #define JTAG_GPIO_TDO_INLVL_BIT     11
    #define JTAG_GPIO_TCK_INLVL_BIT     10
    #define JTAG_GPIO_TDI_INLVL_BIT     9
    #define JTAG_GPIO_TMS_INLVL_BIT     8
    #define JTAG_GPIO_TDO_OUTLVL_BIT    7
    #define JTAG_GPIO_TCK_OUTLVL_BIT    6
    #define JTAG_GPIO_TMS_OUTLVL_BIT    4
    #define JTAG_GPIO_TDO_OE_BIT        3
    #define JTAG_GPIO_TCK_OE_BIT        2
    #define JTAG_GPIO_TDI_OE_BIT        1
    #define JTAG_GPIO_TMS_OE_BIT        0

//========================================================================
//  GPIO registers
//========================================================================
#define PREG_GPIOA_OE                 0x200c
#define PREG_GPIOA_OUTLVL             0x200d
#define PREG_GPIOA_INLVL              0x200e

#define PREG_GPIOB_OE                 0x200f
#define PREG_GPIOB_OUTLVL             0x2010
#define PREG_GPIOB_INLVL              0x2011

#define PREG_GPIOC_OE                 0x2012
#define PREG_GPIOC_OUTLVL             0x2013
#define PREG_GPIOC_INLVL              0x2014

#define PREG_GPIOC2_OE                0x2015
#define PREG_GPIOC2_OUTLVL            0x2016
#define PREG_GPIOC2_INLVL             0x2017

#define PREG_GPIOD_OE                 0x2015
#define PREG_GPIOD_OUTLVL             0x2016
#define PREG_GPIOD_INLVL              0x2017

#define PREG_GPIOD2_OE                0x2018
#define PREG_GPIOD2_OUTLVL            0x2019
#define PREG_GPIOD2_INLVL             0x201a

#define PREG_GPIO_CARD_OE             0x201b
#define PREG_GPIO_CARD_OUTLVL         0x201c
#define PREG_GPIO_CARD_INLVL          0x201d

// ----------------------------
// A9 and L2 chche
// ----------------------------
#define A9_CFG0                                    0x2020
#define A9_CFG1                                    0x2021
#define A9_CFG2                                    0x2022
#define A9_PERIPH_BASE                             0x2023
#define A9_L2_REG_BASE                             0x2024
#define A9_L2_STATUS                               0x2025
#define A9_POR_CFG                                 0x2026

//========================================================================
//  USB Miscellanous control registers
//========================================================================
#define PREI_USB_PHY_REG              0x2100
#define PREI_USB_PHY_A_REG1           0x2101
#define PREI_USB_PHY_B_REG1           0x2102

#define PREI_USB_PHY_A_POR      (1 << 0)
#define PREI_USB_PHY_B_POR      (1 << 1)
//#define PREI_USB_PHY_CLK_EN			(1 << 6) no CLK_EN in A1H
#define PREI_USB_PHY_CLK_SEL    (7 << 5) 
#define PREI_USB_PHY_CLK_GATE 	(1 << 8) 
#define PREI_USB_PHY_B_AHB_RSET     (1 << 11)
#define PREI_USB_PHY_B_CLK_RSET     (1 << 12)
#define PREI_USB_PHY_B_PLL_RSET     (1 << 13)
#define PREI_USB_PHY_A_AHB_RSET     (1 << 17)
#define PREI_USB_PHY_A_CLK_RSET     (1 << 18)
#define PREI_USB_PHY_A_PLL_RSET     (1 << 19)
#define PREI_USB_PHY_A_DRV_VBUS     (1 << 20)
#define PREI_USB_PHY_B_DRV_VBUS			(1 << 21)
#define PREI_USB_PHY_B_CLK_DETECT   (1 << 22)
#define PREI_USB_PHY_CLK_DIV        (0x7f << 24)
#define PREI_USB_PHY_A_CLK_DETECT   (1 << 31)

#define USB_PHY_TUNE_MASK_REFCLKDIV  (3 << 29)
#define USB_PHY_TUNE_MASK_REFCLKSEL  (3 << 27 )
#define USB_PHY_TUNE_MASK_SQRX          (7 << 16 )
#define USB_PHY_TUNE_MASK_TXVREF       (15 << 5)
#define USB_PHY_TUNE_MASK_OTGDISABLE    (1 << 2)
#define USB_PHY_TUNE_MASK_RISETIME  (3 << 9 )
#define USB_PHY_TUNE_MASK_VBUS_THRE (7 << 19)

#define USB_PHY_TUNE_SHIFT_REFCLKDIV  (29)
#define USB_PHY_TUNE_SHIFT_REFCLKSEL  (27)
#define USB_PHY_TUNE_SHIFT_SQRX          (16)
#define USB_PHY_TUNE_SHIFT_TXVREF       (5)
#define USB_PHY_TUNE_SHIFT_OTGDISABLE    (2)
#define USB_PHY_TUNE_SHIFT_RISETIME  (9)
#define USB_PHY_TUNE_SHIFT_VBUS_THRE (19)

//======================================================
//  Pin Mux registers
//======================================================
#define PREG_PIN_MUX_REG0             0x202c
    #define PINMUX0_GPIOB0_REMOTE           (1<<0)
    #define PINMUX0_GPIOB20_PWM_B           (1<<1)
    #define PINMUX0_GPIOB20_UART_TX         (1<<2)
    #define PINMUX0_GPIOB20_I2CSL_CLK       (1<<3)
    #define PINMUX0_GPIOB20_I2CMS_CLK       (1<<4)
    #define PINMUX0_GPIOB19_PWM_A           (1<<5)
    #define PINMUX0_GPIOB19_UART_RX         (1<<6)
    #define PINMUX0_GPIOB19_I2CSL_DATA      (1<<7)
    #define PINMUX0_GPIOB19_I2CMS_DATA      (1<<8)
    #define PINMUX0_GPIOB18_PWM_B           (1<<9)
    #define PINMUX0_GPIOB18_UART_TX         (1<<10)
    #define PINMUX0_GPIOB18_I2CSL_CLK       (1<<11)
    #define PINMUX0_GPIOB18_I2CMS_CLK       (1<<12)
    #define PINMUX0_GPIOB17_PWM_A           (1<<13)
    #define PINMUX0_GPIOB17_UART_RX         (1<<14)
    #define PINMUX0_GPIOB17_I2CSL_DATA      (1<<15)
    #define PINMUX0_GPIOB17_I2CMS_DATA      (1<<16)
    #define PINMUX0_GPIOB16_PWM_B           (1<<17)
    #define PINMUX0_GPIOB16_UART_TX         (1<<18)
    #define PINMUX0_GPIOB16_I2CSL_CLK       (1<<19)
    #define PINMUX0_GPIOB16_I2CMS_CLK       (1<<20)
    #define PINMUX0_GPIOB15_PWM_A           (1<<21)
    #define PINMUX0_GPIOB15_UART_RX         (1<<22)
    #define PINMUX0_GPIOB15_I2CSL_DATA      (1<<23)
    #define PINMUX0_GPIOB15_I2CMS_DATA      (1<<24)
    #define PINMUX0_GPIOC_DVIN_0_7          (1<<25)
    #define PINMUX0_GPIOC_DVIN_8_15         (1<<26)
    #define PINMUX0_GPIOC_DVIN_16_23        (1<<27)
    #define PINMUX0_GPIOC24_DVIN_VS         (1<<28)
    #define PINMUX0_GPIOC25_DVIN_HS         (1<<29)
    #define PINMUX0_GPIOC24_DVIN_DE         (1<<30)
    #define PINMUX0_GPIOC25_DVIN_CLK        (1<<31)

#define PREG_PIN_MUX_REG1             0x202d
    #define PINMUX1_GPIOC4_FEC_A_D0         (1<<0)
    #define PINMUX1_GPIOC5_11_FEC_D1_7      (1<<1)
    #define PINMUX1_GPIOC0_FEC_A_CLK        (1<<2)
    #define PINMUX1_GPIOC1_FEC_A_SOP        (1<<3)
    #define PINMUX1_GPIOC2_FEC_A_FAIL       (1<<4)
    #define PINMUX1_GPIOC3_FEC_A_VALID      (1<<5)
    #define PINMUX1_GPIOB11_ISO7816_RESET   (1<<6)
    #define PINMUX1_GPIOB12_ISO7816_DETECT  (1<<7)
    #define PINMUX1_GPIOB13_ISO7816_DATA    (1<<8)
    #define PINMUX1_GPIOB14_ISO7816_CLK     (1<<9)
    #define PINMUX1_GPIOC21_ETH_MDIO        (1<<10)
    #define PINMUX1_GPIOC20_ETH_MDC         (1<<11)
    #define PINMUX1_GPIOC19_ETH_RX_CLK50    (1<<12)
    #define PINMUX1_GPIOC18_ETH_RX_ERR      (1<<13)
    #define PINMUX1_GPIOC17_ETH_RX_CRS_DV   (1<<14)
    #define PINMUX1_GPIOC16_ETH_RXD1        (1<<15)
    #define PINMUX1_GPIOC15_ETH_RXD0        (1<<16)
    #define PINMUX1_GPIOC14_ETH_TX_EN       (1<<17)
    #define PINMUX1_GPIOC13_ETH_TXD1        (1<<18)
    #define PINMUX1_GPIOC12_ETH_TXD0        (1<<19)
    #define PINMUX1_CARD25_SDIOC_D3         (1<<20)
    #define PINMUX1_CARD24_SDIOC_D2         (1<<21)
    #define PINMUX1_CARD23_SDIOC_D1         (1<<22)
    #define PINMUX1_CARD22_SDIOC_D0         (1<<23)
    #define PINMUX1_CARD21_SDIOC_CMD        (1<<24)
    #define PINMUX1_CARD20_SDIOC_CLK        (1<<25)
    #define PINMUX1_CARD25_ATAPI_DMACK      (1<<26)
    #define PINMUX1_CARD24_17_ATAPI_DD15_8  (1<<27)
    #define PINMUX1_CARD16_5_ATAPI_DDDA_DIO (1<<28)
    #define PINMUX1_CARD4_ATAPI_DIORDY      (1<<29)
    #define PINMUX1_CARD3_ATAPI_DMARQ       (1<<30)
    #define PINMUX1_CARD2_ATAPI_IRQ         (1<<31)

#define PREG_PIN_MUX_REG2             0x202e
    #define PINMUX2_CARD19_SDIOB_CMD        (1<<0)
    #define PINMUX2_CARD18_SDIOB_CLK        (1<<1)
    #define PINMUX2_CARD16_SDIOB_D3         (1<<2)
    #define PINMUX2_CARD15_SDIOB_D2         (1<<3)
    #define PINMUX2_CARD14_SDIOB_D1         (1<<4)
    #define PINMUX2_CARD13_SDIOB_D0         (1<<5)
    #define PINMUX2_CARD12_SDIOA_D3         (1<<6)
    #define PINMUX2_CARD11_SDIOA_D2         (1<<7)
    #define PINMUX2_CARD10_SDIOA_D1         (1<<8)
    #define PINMUX2_CARD9_SDIOA_D0          (1<<9)
    #define PINMUX2_CARD8_SDIOA_CLK         (1<<10)
    #define PINMUX2_CARD7_SDIOA_CMD         (1<<11)
    #define PINMUX2_CARD5_SDIOB_CLK         (1<<12)
    #define PINMUX2_CARD4_SDIOB_CMD         (1<<13)
    #define PINMUX2_CARD3_SDIOB_CLK         (1<<14)
    #define PINMUX2_CARD2_SDIOB_CMD         (1<<15)
    #define PINMUX2_GPIOC8_15_BT656D0_7     (1<<16)
    #define PINMUX2_CARD1_REMOTE            (1<<17)
    #define PINMUX2_CARD0_SPDIFOUT          (1<<18)
    #define PINMUX2_SPI_CS_SPI_DISABLE      (1<<19)
    #define PINMUX2_GPIOB21_SPI_CS          (1<<20)
    #define PINMUX2_TEST_SPDIF_OUT          (1<<21)
    #define PINMUX2_GPIOC23_BT656_IDQ       (1<<22)
    #define PINMUX2_GPIOC26_BT656_FID       (1<<23)
    #define PINMUX2_GPIOC25_BT656_HS        (1<<24)
    #define PINMUX2_GPIOC24_BT656_VS        (1<<25)
    #define PINMUX2_GPIOC27_BT656_CLK       (1<<26)
    #define PINMUX2_GPIOC0_7_BT656D0_7      (1<<27)
    #define PINMUX2_JTAG_TDO_PWMB           (1<<28)
    #define PINMUX2_JTAG_TDO_UART_TX        (1<<29)
    #define PINMUX2_JTAG_TDO_I2CSL_CLK      (1<<30)
    #define PINMUX2_JTAG_TDO_I2CMS_CLK      (1<<31)

#define PREG_PIN_MUX_REG3             0x202f
    #define PINMUX3_TEST_I2SOUT_CHAN0       (1<<0)
    #define PINMUX3_GPIOA0_I2SOUT_AMCLK     (1<<1)
    #define PINMUX3_GPIOA1_I2SOUT_AOCLK     (1<<2)
    #define PINMUX3_GPIOA2_I2SOUT_ALRCLK    (1<<3)
    #define PINMUX3_GPIOA3_I2SOUT_CHAN0     (1<<4)
    #define PINMUX3_GPIOA4_I2SOUT_CHAN1     (1<<5)
    #define PINMUX3_GPIOA5_I2SOUT_CHAN2     (1<<6)
    #define PINMUX3_GPIOA6_I2SOUT_CHAN3     (1<<7)
    #define PINMUX3_GPIOA7_I2SIN_CHAN1      (1<<8)
    #define PINMUX3_GPIOA8_I2SIN_CHAN2      (1<<9)
    #define PINMUX3_GPIOA9_I2SIN_CHAN3      (1<<10)
    #define PINMUX3_GPIOA10_I2SIN_CHAN0     (1<<11)
    #define PINMUX3_GPIOA11_I2SIN_AMCLK     (1<<12)
    #define PINMUX3_GPIOA12_I2SIN_AOCLK     (1<<13)
    #define PINMUX3_GPIOA13_I2SIN_ALRCLK    (1<<14)
    #define PINMUX3_GPIOA9_SPDIFIN          (1<<15)
    #define PINMUX3_CARD22_ISO7826_RESET    (1<<16)
    #define PINMUX3_CARD23_ISO7826_DETECT   (1<<17)
    #define PINMUX3_CARD24_ISO7826_DATA     (1<<18)
    #define PINMUX3_CARD25_ISO7826_CLK      (1<<19)
    #define PINMUX3_CARD9_16_NAND_IO0_7     (1<<20)
    #define PINMUX3_CARD8_NAND_CLE          (1<<21)
    #define PINMUX3_CARD7_NAND_ALE          (1<<22)
    #define PINMUX3_GPIOC32_42_CCIR601      (1<<23)
    #define PINMUX3_CARD6_NAND_RDY_BSY      (1<<24)
    #define PINMUX3_CARD5_NAND_WR           (1<<25)
    #define PINMUX3_CARD4_NAND_RD           (1<<26)
    #define PINMUX3_CARD17_NAND_CE          (1<<27)
    #define PINMUX3_JTAGTCK_PWMA            (1<<28)
    #define PINMUX3_JTAGTCK_UART_RX         (1<<29)
    #define PINMUX3_JTAGTCK_I2CSL_DATA      (1<<30)
    #define PINMUX3_JTAGTCK_I2CMS_DATA      (1<<31)

#define PREG_PIN_MUX_REG4             0x2030
    #define PINMUX4_GPIOB1_8_NANDIO_0_7     (1<<0)
    #define PINMUX4_SPI_D_NAND_ALE          (1<<1)
    #define PINMUX4_SPI_C_NAND_CLE          (1<<2)
    #define PINMUX4_SPI_HOLD_NAND_RDY_BSY   (1<<4)
    #define PINMUX4_SPI_W_NAND_WR           (1<<5)
    #define PINMUX4_SPI_Q_NAND_RD           (1<<6)
    #define PINMUX4_GPIOA22_NAND_CE         (1<<7)
    #define PINMUX4_GPIOA21_NAND_CE2        (1<<8)
    #define PINMUX4_GPIOB9_ETH_MDIO         (1<<10)
    #define PINMUX4_GPIOB9_ETH_MDC          (1<<11)
    #define PINMUX4_GPIOB8_ETH_RX_CLK50     (1<<12)
    #define PINMUX4_GPIOB7_ETH_RX_ERR       (1<<13)
    #define PINMUX4_GPIOB6_ETH_RX_CRS_DV    (1<<14)
    #define PINMUX4_GPIOB5_ETH_RXD1         (1<<15)
    #define PINMUX4_GPIOB4_ETH_RXD0         (1<<16)
    #define PINMUX4_GPIOB3_ETH_TX_EN        (1<<17)
    #define PINMUX4_GPIOB2_ETH_TXD1         (1<<18)
    #define PINMUX4_GPIOB1_ETH_TXD0         (1<<19)
    #define PINMUX4_GPIOC34_39_LCD_R2_7     (1<<20)
    #define PINMUX4_GPIOC32_33_LCD_R0_1     (1<<21)
    #define PINMUX4_GPIOC42_47_LCD_G2_7     (1<<22)
    #define PINMUX4_GPIOC40_41_LCD_G0_1     (1<<23)
    #define PINMUX4_GPIOC50_55_LCD_B2_7     (1<<24)
    #define PINMUX4_GPIOC48_49_LCD_B0_1     (1<<25)
    #define PINMUX4_GPIOC28_HDMI_HS         (1<<26)
    #define PINMUX4_GPIOC29_HDMI_VS         (1<<27)
    #define PINMUX4_GPIOC30_HDMI_DE         (1<<28)
    #define PINMUX4_GPIOC31_HDMI_CLK        (1<<29)
    #define PINMUX4_GPIOC32_55_HDMI_SDR0_23 (1<<30)
    #define PINMUX4_GPIOC32_55_HDMI_DDR0_23 (1<<31)

#define PREG_PIN_MUX_REG5             0x2031
    #define PINMUX5_GPIOC30_TCON_OEH1       (1<<0)
    #define PINMUX5_GPIOC30_TCON_OEV1       (1<<1)
    #define PINMUX5_GPIOC29_TCON_STV1       (1<<2)
    #define PINMUX5_GPIOC28_TCON_STH1       (1<<3)
    #define PINMUX5_GPIOC31_TCON_CPH1       (1<<4)
    #define PINMUX5_GPIOC31_TCON_CPH2       (1<<5)
    #define PINMUX5_GPIOC31_TCON_CPH3       (1<<6)
    #define PINMUX5_GPIOC26_TCON_VCOM       (1<<7)
    #define PINMUX5_GPIOC26_TCON_CPV1       (1<<8)
    #define PINMUX5_GPIOC25_TCON_OEV2       (1<<9)
    #define PINMUX5_GPIOC25_TCON_CPV2       (1<<10)
    #define PINMUX5_GPIOC24_TCON_STV2       (1<<11)
    #define PINMUX5_GPIOC24_TCON_CPH2       (1<<12)
    #define PINMUX5_GPIOC23_TCON_STH2       (1<<13)
    #define PINMUX5_GPIOC23_TCON_CPH3       (1<<14)
    #define PINMUX5_GPIOC23_TCON_PWM_VGHL   (1<<15)
    #define PINMUX5_JTAGTDI_PWMB            (1<<18)
    #define PINMUX5_JTAGTDI_UART_TX         (1<<19)
    #define PINMUX5_JTAGTDI_I2CSL_CLK       (1<<20)
    #define PINMUX5_JTAGTDI_I2CMS_CLK       (1<<21)
    #define PINMUX5_JTAGTMS_PWMA            (1<<22)
    #define PINMUX5_JTAGTMS_UART_RX         (1<<23)
    #define PINMUX5_JTAGTMS_I2CSL_DATA      (1<<24)
    #define PINMUX5_JTAGTMS_I2CMS_DATA      (1<<25)
    #define PINMUX5_GPIOA16_FECB_D0         (1<<26)
    #define PINMUX5_GPIOA17_23_FECB_D1_7    (1<<27)
    #define PINMUX5_GPIOA15_FECB_SOP        (1<<28)
    #define PINMUX5_GPIOA14_FECB_CLK        (1<<29)
    #define PINMUX5_GPIOA8_FECB_DVALID      (1<<30)
    #define PINMUX5_GPIOA7_FECB_FAIL        (1<<31)

#define PREG_PIN_MUX_REG6             0x2032
    #define PINMUX6_SPI_W_DISABLE           (1<<0)
    #define PINMUX6_SPI_Q_DISABLE           (1<<1)
    #define PINMUX6_SPI_D_DISABLE           (1<<2)
    #define PINMUX6_SPI_C_DISABLE           (1<<3)
    #define PINMUX6_SPI_HOLD_DISABLE        (1<<4)
    #define PINMUX6_GPIOC11_ETH_RX_CLK50    (1<<5)
    #define PINMUX6_GPIOC10_ETH_RX_ERR      (1<<6)
    #define PINMUX6_GPIOC9_ETH_RX_CRS_DV    (1<<7)
    #define PINMUX6_GPIOC8_ETH_RXD1         (1<<8)
    #define PINMUX6_GPIOC7_ETH_RXD0         (1<<9)
    #define PINMUX6_GPIOC6_ETH_TX_EN        (1<<10)
    #define PINMUX6_GPIOC5_ETH_DATA1        (1<<11)
    #define PINMUX6_GPIOC4_ETH_DATA0        (1<<12)
    #define PINMUX6_GPIOC3_ETH_MDIO         (1<<13)
    #define PINMUX6_GPIOC2_ETH_MDC          (1<<14)
    #define PINMUX6_GPIOC32_40_CCIR656      (1<<15)
    #define PINMUX6_GPIOD_3_I2SOUT_ALRCLK   (1<<16)
    #define PINMUX6_GPIOD_2_I2SOUT_AOCLK    (1<<17)
    #define PINMUX6_GPIOD_1_I2SOUT_AMCLK    (1<<18)
    #define PINMUX6_GPIOD_0_I2SOUT_CH0      (1<<19)
    #define PINMUX6_GPIOA13_BT656_CLK       (1<<20)
    #define PINMUX6_GPIOA13_FEC_B_VALID     (1<<21)
    #define PINMUX6_GPIOA12_FEC_B_FAIL      (1<<22)
    #define PINMUX6_GPIOA12_BT656_FID       (1<<23)
    #define PINMUX6_GPIOA11_BT656_IDQ       (1<<24)
    #define PINMUX6_GPIOA15_BT656_HS        (1<<25)
    #define PINMUX6_GPIOA15_BT656_VS        (1<<26)
    #define PINMUX6_GPIOA16_23_BT656_DATA   (1<<27)
    #define PINMUX6_GPIOA23_ETH_MDIO        (1<<28)
    #define PINMUX6_GPIOA22_ETH_MDC         (1<<29)
    #define PINMUX6_GPIOA21_ETH_RX_CLK50    (1<<30)
    #define PINMUX6_GPIOA21_ETH_RX_ERR      (1<<31)

#define PREG_PIN_MUX_REG7             0x2033
    #define PINMUX7_GPIOD3_PWMB             (1<<0)
    #define PINMUX7_GPIOD3_UART_TX          (1<<1)
    #define PINMUX7_GPIOD3_I2CSL_CLK        (1<<2)
    #define PINMUX7_GPIOD3_I2CMS_CLK        (1<<3)
    #define PINMUX7_GPIOD2_PWMA             (1<<4)
    #define PINMUX7_GPIOD2_UART_RX          (1<<5)
    #define PINMUX7_GPIOD2_I2CSL_DATA       (1<<6)
    #define PINMUX7_GPIOD2_I2CMS_DATA       (1<<7)
    #define PINMUX7_GPIOD1_PWMB             (1<<8)
    #define PINMUX7_GPIOD1_UART_TX          (1<<9)
    #define PINMUX7_GPIOD1_I2CSL_CLK        (1<<10)
    #define PINMUX7_GPIOD1_I2CMS_CLK        (1<<11)
    #define PINMUX7_GPIOD0_PWMA             (1<<12)
    #define PINMUX7_GPIOD0_UART_RX          (1<<13)
    #define PINMUX7_GPIOD0_I2CSL_DATA       (1<<14)
    #define PINMUX7_GPIOD0_I2CMS_DATA       (1<<15)
    #define PINMUX7_GPIOC2_DDR_DIV16_OUT    (1<<19)
    #define PINMUX7_GPIOC1_DDR_DIV8_OUT     (1<<20)
    #define PINMUX7_GPIOC0_DDR_PLL_LOCK     (1<<21)
    #define PINMUX7_GPIOA19_ETH_CRS_DV      (1<<26)
    #define PINMUX7_GPIOA18_ETH_RXD1        (1<<27)
    #define PINMUX7_GPIOA17_ETH_RXD0        (1<<28)
    #define PINMUX7_GPIOA16_ETH_EX_EN       (1<<29)
    #define PINMUX7_GPIOA15_ETH_TXD1        (1<<30)
    #define PINMUX7_GPIOA14_ETH_TXD0        (1<<31)

#define PREG_PIN_MUX_REG8             0x2034
    #define PINMUX8_GPIOA16_23_NAND_DATA0_7 (1<<0)
    #define PINMUX8_GPIOD13_ETH_RX_CLK50    (1<<1)
    #define PINMUX8_GPIOD12_ETH_RX_ERR      (1<<2)
    #define PINMUX8_GPIOD11_ETH_RX_CRS_DV   (1<<3)
    #define PINMUX8_GPIOD10_ETH_TXD1        (1<<4)
    #define PINMUX8_GPIOD9_ETH_TXD0         (1<<5)
    #define PINMUX8_GPIOD8_ETH_TX_EN        (1<<6)
    #define PINMUX8_GPIOD7_ETH_TXD1         (1<<7)
    #define PINMUX8_GPIOD6_ETH_TXD0         (1<<8)
    #define PINMUX8_GPIOD5_ETH_MDIO         (1<<9)
    #define PINMUX8_GPIOD4_ETH_MDC          (1<<10)
    #define PINMUX8_CARD18_ENC17            (1<<11)
    #define PINMUX8_CARD19_ENC16            (1<<12)
    #define PINMUX8_CARD20_ENC15            (1<<13)
    #define PINMUX8_CARD21_ENC14            (1<<14)
    #define PINMUX8_CARD22_ENC13            (1<<15)
    #define PINMUX8_CARD23_ENC12            (1<<16)
    #define PINMUX8_CARD24_ENC11            (1<<17)
    #define PINMUX8_CARD25_ENC10            (1<<18)
    #define PINMUX8_GPIOD13_ENC9            (1<<19)
    #define PINMUX8_GPIOD12_ENC8            (1<<20)
    #define PINMUX8_GPIOD11_ENC7            (1<<21)
    #define PINMUX8_GPIOD10_ENC6            (1<<22)
    #define PINMUX8_GPIOD9_ENC5             (1<<23)
    #define PINMUX8_GPIOD8_ENC4             (1<<24)
    #define PINMUX8_GPIOD7_ENC3             (1<<25)
    #define PINMUX8_GPIOD6_ENC2             (1<<26)
    #define PINMUX8_GPIOD5_ENC1             (1<<27)
    #define PINMUX8_GPIOD4_ENC0             (1<<28)
    #define PINMUX8_GPIOC19_TCON_CPH3       (1<<29)
    #define PINMUX8_GPIOC20_TCON_CPH2       (1<<30)
    #define PINMUX8_GPIOC21_TCON_OEV3       (1<<31)

#define PREG_PIN_MUX_REG9                          0x2035
#define PREG_PIN_MUX_REG10                         0x2036
#define PREG_PIN_MUX_REG11                         0x2037
#define PREG_PIN_MUX_REG12                         0x2038
// ----------------------------
// Pad conntrols
// ----------------------------
#define PAD_PULL_UP_REG0                           0x203b
#define PAD_PULL_UP_REG1                           0x203c
#define PAD_PULL_UP_REG2                           0x203d
#define PAD_PULL_UP_REG3                           0x203e

//======================================================
//  PWM registers
//======================================================
#define PWM_A_DUTY_CYCLE              0x2154
    #define PWM_HIGH_BIT        16
    #define PWM_LOW_BIT         0
#define PWM_B_DUTY_CYCLE              0x2155
#define PWM_DS_SELECT                 0x2156
    #define PWM_DS_B_EN_BIT     3
    #define PWM_DS_A_EN_BIT     2
    #define PWM_B_EN_BIT        1
    #define PWM_A_EN_BIT        0
#define DS_A_B                        0x2157
    #define PWM_DS_B_VAL_BIT    16
    #define PWM_DS_A_VAL_BIT    0

// ---------------------------
// EFUSE (4)
// ----------------------------
#define EFUSE_CNTL0                                0x2158
#define EFUSE_CNTL1                                0x2159
#define EFUSE_CNTL2                                0x215a
#define EFUSE_CNTL3                                0x215b
#define EFUSE_CNTL4                                0x215c

//======================================================
//  Smart card registers registers
//======================================================
#define PREG_SMARTCARD_REG0           0x2110
#define PREG_SMARTCARD_REG1           0x2111
#define PREG_SMARTCARD_REG2           0x2112
#define PREG_SMARTCARD_STATUS         0x2113
#define PREG_SMARTCARD_INTR           0x2114
#define PREG_SMARTCARD_REG5           0x2115
#define PREG_SMARTCARD_REG6           0x2116
#define PREG_SMARTCARD_FIFO           0x2117

//======================================================
//  IR remote control registers
//======================================================
#define PREG_IR_DEC_LEADER_ACTIVE     0x2120
    #define IR_MAX_ACTIVE_BIT   16
    #define IR_MIN_ACTIVE_BIT   0
#define PREG_IR_DEC_LEADER_IDLE       0x2121
    #define IR_MAX_IDLE_BIT     16
    #define IR_MIN_IDLE_BIT     0
#define PREG_IR_DEC_REPEAT_IDLE       0x2122
    #define IR_MAX_REPEAT_BIT   16
    #define IR_MIN_REPEAT_BIT   0
#define PREG_IR_DEC_BIT0_TIME         0x2123
    #define IR_MAX_BIT0_BIT     16
    #define IR_MIN_BIT0_BIT     0
#define PREG_IR_DEC_BASE_GEN          0x2124
    #define IR_FILTER_COUNT_BIT 28
    #define IR_MAX_FRAME_BIT    12
    #define IR_BASE_RATE_BIT    0
#define PREG_IR_DEC_FRAME_DATA        0x2125
#define PREG_IR_DEC_FRAME_STATUS      0x2126
    #define IR_BIT_MATCH_EN_BIT 30
    #define IR_MAX_BIT1_BIT     20
    #define IR_MIN_BIT1_BIT     10
    #define IR_DEC_STATUS_MASK         (3<<4)
    #define IR_STATUS_PRT_ERR          (3<<4)
    #define IR_STATUS_LEAD_ERR         (2<<4)
    #define IR_STATUS_TIMEOUT          (1<<4)
    #define IR_STATUS_DECODE_OK        (0<<4)
    #define IR_STATUS_MASK             (0x0f)
    #define IR_STATUS_FRAME_DATA_VALID (1<<3)
    #define IR_STATUS_DATA_CODE_ERROR  (1<<2)
    #define IR_STATUS_CUSTOM_ERROR     (1<<1)
    #define IR_STATUS_REPEAT_KEY       (1<<0)
#define PREG_IR_DEC_CONTROL        0x2127
    #define IR_CONTROL_HOLD_LAST_KEY   (1<<6)
    #define IR_CONTROL_RESET           (1<<0)

//======================================================
//  UART registers
//======================================================
#define UART_WR_FIFO                  0x2130
#define UART_RD_FIFO                  0x2131
#define UART_MODE                     0x2132
// bit 21:20 -- Character length:  00 = 8 bits, 01 = 7 bits, 10 = 6 bits, 11 = 5 bits
// bit 17:16 -- Stop bit length:  00 = 1 bit, 01 = 2 bits
// bit 11:0  -- Baud rate:  This value sets the baud rate by dividing the MPEG system clock.
//              The baud rate is set according to the following equation:
//              Baud rate = MPEG System clock/(4*(value+1)) .  For example: 9600 = 129.6Mhz/(4*(3358+1))
    #define invert_rts_bit              31
    #define mask_error_bit              30 // set to 1 to mask errors
    #define invert_cts_bit              29
    #define transmit_byte_interrupt_bit 28 // set to 1 to enable the generation an interrupt whenever a byte is read from the transmit FIFO
    #define receive_byte_interrupt_bit  27 // set to 1 to enable the generation an interrupt whenever a byte is written to the receive FIFO
    #define invert_tx_pin_bit           26 // set to 1 to inver the TX pin
    #define invert_rx_pin_bit           25 // set to 1 to inver the RX pin
    #define clear_error_bit             24
    #define reset_receive_bit           23
    #define reset_transmit_bit          22
    #define character_length_bit        20
    #define parity_enable_bit           19
    #define parity_type_bit             18
    #define stop_bit_length_bit         16
    #define two_wire_mode_bit           15
#define UART_STATUS                   0x2133
// bit  8:0 -- Data depth in the RFIFO
// bit 14:8 -- Data depth in the WFIFO
    #define rfifo_data_depth_bit         0
    #define wfifo_data_depth_bit         8
    #define parity_error_bit            16
    #define frame_error_bit             17
    #define wfifo_overflow_error_bit    18
    #define rfifo_full_bit              19
    #define rfifo_empty_bit             20
    #define wfifo_full_bit              21
    #define wfifo_empty_bit             22
    #define cts_level_bit               23
#define UART_IRQ_CTRL                 0x2134
    #define xmit_irq_cnt_bit            7
    #define recv_irq_cnt_bit            0

//======================================================
//  Random number generator
//======================================================
#define PREG_RAND64_LO                0x2040
#define PREG_RAND64_HI                0x2041

//======================================================
//  I2C Slave/Master module
//======================================================
#define PREG_I2C_SL_CONTROL           0x2150
#define PREG_I2C_SL_SEND              0x2151
#define PREG_I2C_SL_RECV              0x2152

#define PREG_I2C_SL_REG1              ** new **             0x2153  // NEW FILTER


//#define PREG_I2C_MS_CTRL              0x2140
//    #define I2C_M_MANUAL_SDA_I        26
//    #define I2C_M_MANUAL_SCL_I        25
//    #define I2C_M_MANUAL_SDA_O        24
//    #define I2C_M_MANUAL_SCL_O        23
//    #define I2C_M_MANUAL_EN           22
//    #define I2C_M_DELAY_MSB           21
//    #define I2C_M_DELAY_LSB           12
//    #define I2C_M_DATA_CNT_MSB        11
//    #define I2C_M_DATA_CNT_LSB        8
//    #define I2C_M_CURR_TOKEN_MSB      7
//    #define I2C_M_CURR_TOKEN_LSB      4
//    #define I2C_M_ERROR               3
//    #define I2C_M_STATUS              2
//    #define I2C_M_ACK_IGNORE          1
//    #define I2C_M_START               0
//#define  PREG_I2C_MS_SL_ADDR          0x2141
//#define  PREG_I2C_MS_TOKEN_LIST_REG0  0x2142
//#define  PREG_I2C_MS_TOKEN_LIST_REG1  0x2143
//#define  PREG_I2C_MS_TOKEN_WDATA_REG0 0x2144
//#define  PREG_I2C_MS_TOKEN_WDATA_REG1 0x2145
//#define  PREG_I2C_MS_TOKEN_RDATA_REG0 0x2146
//#define  PREG_I2C_MS_TOKEN_RDATA_REG1 0x2147
//    #define  I2C_END                  0x0
//    #define  I2C_START                0x1
//    #define  I2C_SLAVE_ADDR_WRITE     0x2
//    #define  I2C_SLAVE_ADDR_READ      0x3
//    #define  I2C_DATA                 0x4
//    #define  I2C_DATA_LAST            0x5
//    #define  I2C_STOP                 0x6

#define PREG_I2C_M1_MS_CTRL                        0x2148
#define PREG_I2C_M1_MS_SL_ADDR                     0x2149
#define PREG_I2C_M1_MS_TOKEN_LIST_REG0             0x214a
#define PREG_I2C_M1_MS_TOKEN_LIST_REG1             0x214b
#define PREG_I2C_M1_MS_TOKEN_WDATA_REG0            0x214c
#define PREG_I2C_M1_MS_TOKEN_WDATA_REG1            0x214d
#define PREG_I2C_M1_MS_TOKEN_RDATA_REG0            0x214e
#define PREG_I2C_M1_MS_TOKEN_RDATA_REG1            0x214f


//======================================================
//  NDMA
//======================================================
#define PREG_NDMA_CONTROL_REG       0x2270
    #define NDMA_IN_PROGRESS          (1<<26)
    #define NDMA_PERIOD_INT_ENABLE    (1<<15)
    #define NDMA_START                (1<<14)
    #define NDMA_DELAY_MASK           (0x3fff)
#define PREG_NDMA_TABLE_POINTER     0x2271
#define PREG_NMDA_TABLE_DESC_ADD    0x2272
#define PREG_NDMA_TDES_KEY_LO       0x2273
#define PREG_NDMA_TDES_KEY_HI       0x2274
#define PREG_NDMA_TDES_CONTROL      0x2275
#define PREG_NDMA_AES_CONTROL       0x2276
#define PREG_NDMA_AES_RK_FIFO       0x2277
#define PREG_NDMA_CRC_OUT           0x2278

#define NDMA_THREAD_REG                            0x2279
#define NDMA_THREAD_TABLE_START0                   0x2280
#define NDMA_THREAD_TABLE_CURR0                    0x2281
#define NDMA_THREAD_TABLE_END0                     0x2282
#define NDMA_THREAD_TABLE_START1                   0x2283
#define NDMA_THREAD_TABLE_CURR1                    0x2284
#define NDMA_THREAD_TABLE_END1                     0x2285
#define NDMA_THREAD_TABLE_START2                   0x2286
#define NDMA_THREAD_TABLE_CURR2                    0x2287
#define NDMA_THREAD_TABLE_END2                     0x2288
#define NDMA_THREAD_TABLE_START3                   0x2289
#define NDMA_THREAD_TABLE_CURR3                    0x228a
#define NDMA_THREAD_TABLE_END3                     0x228b
#define NDMA_CNTL_REG1                             0x228c


//========================================================================
// SDIO interface
//========================================================================
// -----------------------------------------------     
// CBUS_BASE:  PERIPHS3_CBUS_BASE = 0x23               
// -----------------------------------------------     
#define CMD_ARGUMENT                0x2308
#define CMD_SEND                    0x2309
#define SDIO_CONFIG                 0x230a
#define SDIO_STATUS_IRQ             0x230b
#define SDIO_IRQ_CONFIG             0x230c
#define SDIO_MULT_CONFIG            0x230d
#define SDIO_M_ADDR                 0x230e
#define SDIO_EXTENSION              0x230f

//======================================================
//  ASYNC FIFO
//======================================================
#define ASYNC_FIFO1_REG0            0x2310
#define ASYNC_FIFO1_REG1            0x2311
    #define ASYNC_FIFO_FLUSH_STATUS     31
    #define ASYNC_FIFO_ERR              30
    #define ASYNC_FIFO_FIFO_EMPTY       29
    #define ASYNC_FIFO_TO_HIU           24
    #define ASYNC_FIFO_FLUSH            23
    #define ASYNC_FIFO_RESET            22
    #define ASYNC_FIFO_WRAP_EN          21
    #define ASYNC_FIFO_FLUSH_EN         20
    #define ASYNC_FIFO_RESIDUAL_MSB     19
    #define ASYNC_FIFO_RESIDUAL_LSB     15
    #define ASYNC_FIFO_FLUSH_CNT_MSB    14
    #define ASYNC_FIFO_FLUSH_CNT_LSB    0
#define ASYNC_FIFO1_REG2            0x2312
    #define ASYNC_FIFO_FIFO_FULL        26
    #define ASYNC_FIFO_FILL_STATUS      25
    #define ASYNC_FIFO_SOURCE_MSB       24
    #define ASYNC_FIFO_SOURCE_LSB       23
    #define ASYNC_FIFO_ENDIAN_MSB       22
    #define ASYNC_FIFO_ENDIAN_LSB       21
    #define ASYNC_FIFO_FILL_EN          20
    #define ASYNC_FIFO_FILL_CNT_MSB     19
    #define ASYNC_FIFO_FILL_CNT_LSB     0
#define ASYNC_FIFO1_REG3            0x2313
    #define ASYNC_FLUSH_SIZE_IRQ_MSB    15
    #define ASYNC_FLUSH_SIZE_IRQ_LSB    0

#define ASYNC_FIFO2_REG0            0x2314
#define ASYNC_FIFO2_REG1            0x2315
#define ASYNC_FIFO2_REG2            0x2316
#define ASYNC_FIFO2_REG3            0x2317

//======================================================
//  NAND Registers
//======================================================
// Back to CBUS registers 
#define PNAND_CONFIG_REG            0x2180  //0x340                   
#define PNAND_GEN_TIMING            0x2181  //0x344                   
#define PNAND_DMA_START_ADDR        0x2182  //0x348                   
#define PNAND_DMA_XFER_COUNT        0x2183  //0x34c                   
#define PNAND_DMA_CNTL              0x2184  //0x350                   
#define PNAND_DMA_COL_PAGE          0x2185  //0x354                   
#define PNAND_DMA_BURST             0x2186  //0x358                   
#define PNAND_USER_3_0              0x2187  //0x35c                   
#define PNAND_USER_7_4              0x2188  //0x360                   
#define PNAND_ECC2                  0x2189  //0x364                   
#define PNAND_USER_11_8             0x2189  //0x360                   
#define PNAND_USER_15_12            0x218a  //0x360                   
#define PNAND_ERR_LOC               0x218b  //0x36c                   
#define PNAND_ERROR_FIFO_POINT      0x218b  //0x36c                   
#define PNAND_ERROR_FIFO_DATA       0x218b  //0x36c                   
#define PNAND_ERR_CNT0              0x218c  //0x370                   
#define PNAND_ERR_CNT1              0x218d  //0x374                   
#define PNAND_ERR_UNC               0x218e  //0x378                   
#define PNAND_USER_3_0_2ND          0x218f  //** new ** 2nd user bytes
#define PNAND_USER_7_4_2ND          0x2190  //** new ** 2nd user bytes
#define PNAND_USER_11_8_2ND         0x2192  //** new ** 2nd user bytes
#define PNAND_USER_15_12_2ND        0x2193  //** new ** 2nd user bytes

//======================================================
// PWM / DS C/D
//======================================================
#define PWM_PWM_C                                  0x2194
#define PWM_PWM_D                                  0x2195
#define PWM_MISC_REG_CD                            0x2196
#define PWM_DELTA_SIGMA_CD                         0x2197
//======================================================
// SAR ADC (16)
//======================================================
#define SAR_ADC_REG0                               0x21a0
#define SAR_ADC_CHAN_LIST                          0x21a1
#define SAR_ADC_AVG_CNTL                           0x21a2
#define SAR_ADC_REG3                               0x21a3
#define SAR_ADC_DELAY                              0x21a4
#define SAR_ADC_LAST_RD                            0x21a5
#define SAR_ADC_FIFO_RD                            0x21a6
#define SAR_ADC_AUX_SW                             0x21a7
#define SAR_ADC_CHAN_10_SW                         0x21a8
#define SAR_ADC_DETECT_IDLE_SW                     0x21a9
#define SAR_ADC_DELTA_10                           0x21aa

//========================================================================
//SPI FLASH REG     12'h500~12'h53f
//=======================================================================
// for SPI FLASH interface.
#define PREG_SPI_FLASH_CMD        0x2320
   #define SPI_FLASH_READ    					31
   #define SPI_FLASH_WREN    					30
   #define SPI_FLASH_WRDI    					29
   #define SPI_FLASH_RDID    					28
   #define SPI_FLASH_RDSR    					27
   #define SPI_FLASH_WRSR    					26
   #define SPI_FLASH_PP      					25
   #define SPI_FLASH_SE      					24
   #define SPI_FLASH_BE      					23
   #define SPI_FLASH_CE      					22
   #define SPI_FLASH_DP      					21
   #define SPI_FLASH_RES     					20
   #define SPI_HPM           					19
   #define SPI_FLASH_USR     					18
   #define SPI_FLASH_USR_ADDR 				15
   #define SPI_FLASH_USR_DUMMY 				14
   #define SPI_FLASH_USR_DIN   				13
   #define SPI_FLASH_USR_DOUT   			12
   #define SPI_FLASH_USR_DUMMY_BLEN   10
   #define SPI_FLASH_USR_CMD     			0

#define PREG_SPI_FLASH_ADDR       0x2321
   #define SPI_FLASH_BYTES_LEN 				24
   #define SPI_FLASH_ADDR_START 			0
   
#define PREG_SPI_FLASH_CTRL              0x2322
   #define SPI_ENABLE_AHB    					(1<<17)
   #define SPI_SST_AAI       					(1<<16)
   #define SPI_RES_RID       					(1<<15)
   #define SPI_FREAD_DUAL    					(1<<14)
   #define SPI_READ_READ_EN  					(1<<13)
   #define SPI_CLK_DIV0      					(1<<12)
   #define SPI_CLKCNT_N      					(1<<8 )
   #define SPI_CLKCNT_H      					(1<<4 )
   #define SPI_CLKCNT_L      					(1<<0 )

#define PREG_SPI_FLASH_CTRL1      0x2323
#define PREG_SPI_FLASH_STATUS     0x2324
#define PREG_SPI_FLASH_CTRL2      0x2325
#define SPI_FLASH_CLOCK                            0x2326
#define SPI_FLASH_USER                             0x2327
#define SPI_FLASH_USER1                            0x2328
#define SPI_FLASH_USER2                            0x2329
#define SPI_FLASH_USER3                            0x232a
#define SPI_FLASH_USER4                            0x232b
#define SPI_FLASH_SLAVE                            0x232c
#define SPI_FLASH_SLAVE1                           0x232d
#define SPI_FLASH_SLAVE2                           0x232e
#define SPI_FLASH_SLAVE3                           0x232f
#define PREG_SPI_FLASH_C0         0x2330

#define PREG_SPI_FLASH_C1         0x2331
#define PREG_SPI_FLASH_C2         0x2332
#define PREG_SPI_FLASH_C3         0x2333
#define PREG_SPI_FLASH_C4         0x2334
#define PREG_SPI_FLASH_C5         0x2335
#define PREG_SPI_FLASH_C6         0x2336
#define PREG_SPI_FLASH_C7         0x2337
#define SPI_FLASH_B8                               0x2338
#define SPI_FLASH_B9                               0x2339
#define SPI_FLASH_B10                              0x233a
#define SPI_FLASH_B11                              0x233b
#define SPI_FLASH_B12                              0x233c
#define SPI_FLASH_B13                              0x233d
#define SPI_FLASH_B14                              0x233e
#define SPI_FLASH_B15                              0x233f
// ----------------------------
// SPI #2
// ----------------------------
#define SPI2_FLASH_CMD                             0x2360
#define SPI2_FLASH_ADDR                            0x2361
#define SPI2_FLASH_CTRL                            0x2362
#define SPI2_FLASH_CTRL1                           0x2363
#define SPI2_FLASH_STATUS                          0x2364
#define SPI2_FLASH_CTRL2                           0x2365
#define SPI2_FLASH_CLOCK                           0x2366
#define SPI2_FLASH_USER                            0x2367
#define SPI2_FLASH_USER1                           0x2368
#define SPI2_FLASH_USER2                           0x2369
#define SPI2_FLASH_USER3                           0x236a
#define SPI2_FLASH_USER4                           0x236b
#define SPI2_FLASH_SLAVE                           0x236c
#define SPI2_FLASH_SLAVE1                          0x236d
#define SPI2_FLASH_SLAVE2                          0x236e
#define SPI2_FLASH_SLAVE3                          0x236f
#define SPI2_FLASH_C0                              0x2370
#define SPI2_FLASH_C1                              0x2371
#define SPI2_FLASH_C2                              0x2372
#define SPI2_FLASH_C3                              0x2373
#define SPI2_FLASH_C4                              0x2374
#define SPI2_FLASH_C5                              0x2375
#define SPI2_FLASH_C6                              0x2376
#define SPI2_FLASH_C7                              0x2377
#define SPI2_FLASH_B8                              0x2378
#define SPI2_FLASH_B9                              0x2379
#define SPI2_FLASH_B10                             0x237a
#define SPI2_FLASH_B11                             0x237b
#define SPI2_FLASH_B12                             0x237c
#define SPI2_FLASH_B13                             0x237d
#define SPI2_FLASH_B14                             0x237e
#define SPI2_FLASH_B15                             0x237f


//-------------------------------------------------------------------------
// BT655 in
//------------------------------------------------------------------------- 
#define BT_CTRL 				             0x2240
	#define BT_SOFT_RESET    	    31      // Soft reset
	#define BT_XCLK27_EN_BIT		  12      // 1 : xclk27 is input. 	0 : xclk27 is output.
	#define BT_INT_MASK_BIT			  11      // 1 : MASK the error interrupt.
	#define BT_IDQ_EN_BIT			    10		// 1 : enable use IDQ port.
	#define BT_FID_EN_BIT			    9		// 1 : enable use FID port.
	#define BT_CLK27_SEL_BIT		  8 		// 1 : external xclk27		0 : internal clk27.
	#define BT_CLK27_PHASE_BIT		7		// 1 : no inverted			0 : inverted.
	#define BT_ACE_MODE_BIT			  6		// 1 : auto cover error by hardware.
	#define BT_SLICE_MODE_BIT		  5		// 1 : no ancillay flag		0 : with ancillay flag.
	#define BT_FMT_MODE_BIT			  4       // 1 : ntsc 				0 : pal.
	#define BT_REF_MODE_BIT			  3       // 1 : from bit stream. 	0 : from ports.
	#define BT_MODE_BIT				    2       // 1 : BT656 model      	0 : SAA7118 mode.
	#define BT_NFIELD_BIT		      1		// 1 : enable.
	#define BT_EN_BIT				      0		// 1 : enable.

// The following 3 registers : low 16 bits for field0; high 16bits for field1.
#define BT_VBI_START					    0x2241
#define BT_VBI_END    				        0x2242
#define BT_FIELD_START				        0x2243

// video line control low 16 bits for hoffset. high 16 bits for hsize.
#define BT_LINECTRL					        0x2244

// the following 2 registes : low 16 bits for field0; high 16bits for field1.
#define BT_VIDEO_START				        0x2245
#define BT_VIDEO_END					    0x2246

// VBI line ancilary data control for field0
#define BT_SLICE_LINE0				        0x2247
// VBI line ancilary data control for field1
#define BT_SLICE_LINE1				        0x2248

#define BT_PORT_CTRL        			    0x2249
#define BT_SWAP_CTRL					    0x224a
      #define BT_CB0_POSITION    	    28
      #define BT_Y0_POSITION    	    24
      #define BT_CR0_POSITION    	    20
      #define BT_Y1_POSITION    	    16
      #define BT_CB2_POSITION    	    12
      #define BT_Y2_POSITION    	    8
      #define BT_CR2_POSITION    	    4
      #define BT_Y3_POSITION    	    0
#define BT_ANCISADR                                0x224b
#define BT_ANCIEADR                                0x224c
#define BT_AFIFO_CTRL                              0x224d
#define BT_601_CTRL0                               0x224e
#define BT_601_CTRL1                               0x224f
#define BT_601_CTRL2                               0x2250
#define BT_601_CTRL3                               0x2251
#define BT_FIELD_LUMA                              0x2252
#define BT_RAW_CTRL                                0x2253
#define BT_STATUS					    0x2254
     #define BT_SOFT_INT_FLAG    	    28
     #define BT_NTSC_OR_PAL       	    5
     #define BT_FIELD_STATUS    	    4
     #define BT_PRE_FIELD_STATUS    	    3
     #define BT_SKIP_CURRENT_FIELD    	    2
     #define BT_PRE_SKIP_FIELD    	    1
     #define BT_ERROR_STATUS    	    0
#define BT_INT_CTRL                                0x2255
#define BT_ANCI_STATUS         		    0x2256
#define BT_VLINE_STATUS        		    0x2257
#define BT_AFIFO_PTR                               0x2258
#define BT_JPEGBYTENUM                             0x2259
#define BT_ERR_CNT                                 0x225a
#define BT_JPEG_STATUS0                            0x225b
#define BT_JPEG_STATUS1                            0x225c
#define BT656_ADDR_END                             0x225f

#if 0
//not exist in A3
// DMA start address.
#define BT_ANCI_SADR					0x224e
#define BT_ANCI_OVADR					0x224f

#define BT_VFIFO_CTRL					0x2250
	#define BT_FIFO_FILL_EN    	    31
	#define BT_FIFO_FLUSH_EN    	    30
	#define BT_BURST_64_CYCLES    	    29
	#define BT_URGENT_REQUEST    	    28

#define BT_VFIFO_CTRL1					0x2251
	#define BT_CANVAS_START_POINT_IN_HEIGHT    	    16
	#define BT_CANVAS_START_POINT_IN_WIDTH    	    0
	
#define BT_AFIFO_CTR					0x2252
	#define BT_FILL_EN    	    3
	#define BT_FLUSH_EN    	    2
	#define BT_URGENT_EN        0

#define BT_FIELD_LUMA					0x2253
#endif

// new feature for apollo, audio input
#define SPDIF_MODE						0x2200
#define SPDIF_CLOCK					    0x2201
#define SPDIF_CHAN_A_STATUS			    0x2202
#define SPDIF_CHAN_B_STATUS			    0x2203
#define SPDIF_STATUS					0x2204
#define SPDIF_PCPD						0x2205

#define AUDIN_CTRL						0x2210

#define AUDIN_FIFO0_START				0x2220
#define AUDIN_FIFO0_END				    0x2221
#define AUDIN_FIFO0_WP					0x2222
#define AUDIN_FIFO0_INTP				0x2223
#define AUDIN_FIFO0_RP					0x2224
#define AUDIN_FIFO0_CTRL				0x2225
#define AUDIN_FIFO0_EMPTY_LEVEL0		0x2226
#define AUDIN_FIFO0_EMPTY_LEVEL1		0x2227
#define AUDIN_FIFO0_EMPTY_LEVEL2		0x2228

#define AUDIN_FIFO1_START				0x2229
#define AUDIN_FIFO1_END				    0x222a
#define AUDIN_FIFO1_WP					0x222b
#define AUDIN_FIFO1_INTP				0x222c
#define AUDIN_FIFO1_RP					0x222d
#define AUDIN_FIFO1_CTRL				0x222e
#define AUDIN_FIFO1_EMPTY_LEVEL0		0x222f
#define AUDIN_FIFO1_EMPTY_LEVEL1		0x2230
#define AUDIN_FIFO1_EMPTY_LEVEL2		0x2231
#define AUDIN_INT						0x2232

#endif /* PERIPHERAL_REG_APOLLO_H */
