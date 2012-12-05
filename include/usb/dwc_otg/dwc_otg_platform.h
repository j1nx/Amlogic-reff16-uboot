/*
 * Copyright (c) 2011 Amlogic (Shanghai), Inc.
 * Tianhui.Wang <tianhui.wang@amlogic.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#ifndef __DWC_OTG_PLATFORM_H__
#define __DWC_OTG_PLATFORM_H__

/* The following macro is the base or offset addr of dwc-otg controller regs */

#define DWC_REG_BASE	CONFIG_USBPORT_BASE


#define DWC_REG_GOTGCTL	0x000 /** OTG Control and Status Register.  <i>Offset: 000h</i> */
#define DWC_REG_GOTGINT	0x004 /** OTG Interrupt Register.	 <i>Offset: 004h</i> */
#define DWC_REG_GAHBCFG	0x008 /**Core AHB Configuration Register.	 <i>Offset: 008h</i> */
#define DWC_REG_GUSBCFG	0x00C /**Core USB Configuration Register.	 <i>Offset: 00Ch</i> */
#define DWC_REG_GRSTCTL	0x010 /**Core Reset Register.	 <i>Offset: 010h</i> */
#define DWC_REG_GINTSTS	0x014 /**Core Interrupt Register.	 <i>Offset: 014h</i> */
#define DWC_REG_GINTMSK	0x018 /**Core Interrupt Mask Register.  <i>Offset: 018h</i> */
#define DWC_REG_GRXSTSR	0x01C /**Receive Status Queue Read Register (Read Only).	<i>Offset: 01Ch</i> */
#define DWC_REG_GRXSTSP	0x020 /**Receive Status Queue Read & POP Register (Read Only).  <i>Offset: 020h</i>*/
#define DWC_REG_GRXFSIZ	0x024 /**Receive FIFO Size Register.	<i>Offset: 024h</i> */
#define DWC_REG_GNPTXFSIZ	0x028 /**Non Periodic Transmit FIFO Size Register.  <i>Offset: 028h</i> */
#define DWC_REG_GNPTXSTS	0x02C /**Non Periodic Transmit FIFO/Queue Status Register (Read  * Only). <i>Offset: 02Ch</i> */
#define DWC_REG_GI2CCTL	0x030 /**I2C Access Register.	 <i>Offset: 030h</i> */
#define DWC_REG_GPVNDCTL	0x034 /**PHY Vendor Control Register.	 <i>Offset: 034h</i> */
#define DWC_REG_GGPIO		0x038 /**General Purpose Input/Output Register.  <i>Offset: 038h</i> */
#define DWC_REG_GUID		0x03C /**User ID Register.  <i>Offset: 03Ch</i> */
#define DWC_REG_GSNPSID	0x040 /**Synopsys ID Register (Read Only).  <i>Offset: 040h</i> */
#define DWC_REG_GHWCFG1	0x044 /**User HW Config1 Register (Read Only).  <i>Offset: 044h</i> */
#define DWC_REG_GHWCFG2	0x048 /**User HW Config2 Register (Read Only).  <i>Offset: 048h</i> */
#define DWC_REG_GHWCFG3	0x04C /**User HW Config3 Register (Read Only).  <i>Offset: 04Ch</i> */
#define DWC_REG_GHWCFG4	0x050 /**User HW Config4 Register (Read Only).  <i>Offset: 050h</i>*/
#define DWC_REG_HPTXFSIZ	0x100 /** Host Periodic Transmit FIFO Size Register. <i>Offset: 100h</i> */

/** Device Periodic Transmit FIFO#n Register if dedicated fifos are disabled, 
		otherwise Device Transmit FIFO#n Register. 
	 * <i>Offset: 104h + (FIFO_Number-1)*04h, 1 <= FIFO Number <= 15 (1<=n<=15).</i> */
#define DWC_REG_DPTXFSIZ_DIEPTXF		0x104

#define DWC_REG_DCFG	0x800 /** Device Configuration Register. <i>Offset 800h</i> */
#define DWC_REG_DCTL	0x804 /** Device Control Register. <i>Offset: 804h</i> */
#define DWC_REG_DSTS	0x808 /** Device Status Register (Read Only). <i>Offset: 808h</i> */
#define DWC_REG_DIEPMSK	0x810 /** Device IN Endpoint Common Interrupt Mask Register. <i>Offset: 810h</i> */
#define DWC_REG_DOEPMSK	0x814 /** Device OUT Endpoint Common Interrupt Mask Register. <i>Offset: 814h</i> */
#define DWC_REG_DAINT	0x818 /** Device All Endpoints Interrupt Register.  <i>Offset: 818h</i> */
#define DWC_REG_DAINTMSK	0x81C /** Device All Endpoints Interrupt Mask Register.  <i>Offset: 81Ch</i> */
#define DWC_REG_DTKNQR1	0x820 /** Device IN Token Queue Read Register-1 (Read Only). <i>Offset: 820h</i> */
#define DWC_REG_DTKNQR2	0x824 /** Device IN Token Queue Read Register-2 (Read Only). <i>Offset: 824h</i> */ 
#define DWC_REG_DVBUGDIS	0x828 /** Device VBUS	 discharge Register.  <i>Offset: 828h</i> */
#define DWC_REG_DVBUSPULSE	0x82C /** Device VBUS Pulse Register.	 <i>Offset: 82Ch</i> */
#define DWC_REG_DTKNQR3	0x830
	/** Device IN Token Queue Read Register-3 (Read Only). /
	 *	Device Thresholding control register (Read/Write)
	 * <i>Offset: 830h</i> */ 
#define DWC_REG_DTKNQR4	0x834
	/** Device IN Token Queue Read Register-4 (Read Only). /
	 *	Device IN EPs empty Inr. Mask Register (Read/Write)
	 * <i>Offset: 834h</i> */ 
#define DWC_REG_PCGCCTL	0xE00 /** Power and Clock Gating Control Register */

#define DWC_EP_REGS_OFFSET	0x00
#define DWC_EP_INTR_OFFSET	0x08
#define DWC_EP_TSIZE_OFFSET	0x10
#define DWC_EP_DMA_OFFSET		0x14

#define DWC_IN_EP_REG_START 0x900
#define DWC_OUT_EP_REG_START 0xB00

#define DWC_REG_IN_EP_REG(x)	(DWC_IN_EP_REG_START + x*0x20 + DWC_EP_REGS_OFFSET)
#define DWC_REG_IN_EP_INTR(x)	(DWC_IN_EP_REG_START + x*0x20 + DWC_EP_INTR_OFFSET)
#define DWC_REG_IN_EP_TSIZE(x)	(DWC_IN_EP_REG_START + x*0x20 + DWC_EP_TSIZE_OFFSET)
#define DWC_REG_IN_EP_DMA(x)	(DWC_IN_EP_REG_START + x*0x20 + DWC_EP_DMA_OFFSET)

#define DWC_REG_OUT_EP_REG(x)		(DWC_OUT_EP_REG_START + x*0x20 + DWC_EP_REGS_OFFSET)
#define DWC_REG_OUT_EP_INTR(x)	(DWC_OUT_EP_REG_START + x*0x20 + DWC_EP_INTR_OFFSET)
#define DWC_REG_OUT_EP_TSIZE(x)	(DWC_OUT_EP_REG_START + x*0x20 + DWC_EP_TSIZE_OFFSET)
#define DWC_REG_OUT_EP_DMA(x)	(DWC_OUT_EP_REG_START + x*0x20 + DWC_EP_DMA_OFFSET)


#define DWC_REG_DATA_FIFO_START 0x1000

/* operate the regs */
//#include <asm/arch-m1/firm/io.h>

#define WRITE_PERI_REG(reg, val)	writel(v, PERI_BASE_ADDR + ((reg)<<2)))
#define READ_PERI_REG(reg)		readl(PERI_BASE_ADDR + ((reg)<<2)))

#define CLEAR_PERIPHS_REG_BITS(reg, mask) WRITE_PERI_REG(reg, (READ_PERI_REG(reg)&(~(mask))))
#define SET_PERIPHS_REG_BITS(reg, mask)   WRITE_PERI_REG(reg, (READ_PERI_REG(reg)|(mask)))

#define dwc_write_reg32(x, v)		writel(v, x + DWC_REG_BASE)
#define dwc_read_reg32(x)		readl(x + DWC_REG_BASE)
#define dwc_modify_reg32(x, c, s) 	writel(((dwc_read_reg32(x) & ~c) |s), (x + DWC_REG_BASE))

/*memory buffer oprations*/
#define get_unaligned_16(ptr)				(((__u8 *)ptr)[0] | (((__u8 *)ptr)[1]<<8))
#define get_unaligned_32(ptr)				(((__u8 *)ptr)[0] | (((__u8 *)ptr)[1]<<8) | (((__u8 *)ptr)[2]<<16) | (((__u8 *)ptr)[3]<<24))
#define get_unaligned(ptr)				(((__u8 *)ptr)[0] | (((__u8 *)ptr)[1]<<8) | (((__u8 *)ptr)[2]<<16) | (((__u8 *)ptr)[3]<<24))

#endif
