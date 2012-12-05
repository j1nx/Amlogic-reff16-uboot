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

#include <common.h>
#include <asm/arch/uart.h>
#include <asm/arch/clock.h>



#if (defined CONFIG_BOARD_MBOX_8626M1) || (defined CONFIG_BOARD_GADMEI_6236M)
#define BOARD_UART_PORT UART_PORT_0
#else
#define BOARD_UART_PORT UART_PORT_1
#endif

/*
 * Intialise the serial port with given baudrate
 */
unsigned serial_init_with_clk(unsigned clk)
{
//    unsigned clk;
	writel(0,P_UART_CONTROL(BOARD_UART_PORT));

#ifdef CONFIG_BOARD_MBOX_8626M1
	setbits_le32(P_PERIPHS_PIN_MUX_2,((1<<15)|(1<<11)));
#elif defined CONFIG_BOARD_GADMEI_6236M
	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_7,((1 << 11) | (1 << 8)));
#endif    
#if 0
    if((readl(P_HHI_MPEG_CLK_CNTL)&(1<<8))==0)
	{
		clk=CONFIG_CRYSTAL_MHZ*1000000;
	}
	clk=(clk_util_clk_msr(11)*1000000)/((readl(P_HHI_MPEG_CLK_CNTL)&0x7f)+1);
#endif

	/* baud rate */
    unsigned baud_para=0;
#if 0   //changed by Elvis --- disable parity
    baud_para=clk/(CONFIG_BAUDRATE*4) -1;
    
    clrsetbits_le32(P_UART_CONTROL(BOARD_UART_PORT),UART_CNTL_MASK_BAUD_RATE,baud_para);
	clrsetbits_le32(P_UART_CONTROL(BOARD_UART_PORT),UART_CNTL_MASK_STP_BITS,UART_CNTL_MASK_STP_1BIT);
	clrsetbits_le32(P_UART_CONTROL(BOARD_UART_PORT),
			UART_CNTL_MASK_PRTY_TYPE | UART_CNTL_MASK_PRTY_EN,UART_CNTL_MASK_PRTY_EN|UART_CNTL_MASK_PRTY_EVEN);
	clrsetbits_le32(P_UART_CONTROL(BOARD_UART_PORT),UART_CNTL_MASK_CHAR_LEN,UART_CNTL_MASK_CHAR_8BIT);
	setbits_le32(P_UART_CONTROL(BOARD_UART_PORT),UART_CNTL_MASK_TX_EN | UART_CNTL_MASK_RX_EN);

	setbits_le32(P_UART_CONTROL(BOARD_UART_PORT),UART_CNTL_MASK_RST_TX | UART_CNTL_MASK_RST_RX | UART_CNTL_MASK_CLR_ERR);
	clrbits_le32(P_UART_CONTROL(BOARD_UART_PORT),UART_CNTL_MASK_RST_TX | UART_CNTL_MASK_RST_RX | UART_CNTL_MASK_CLR_ERR);
#endif

	return baud_para;
}

/*
 * Output a single byte to the serial port.
 */
void serial_putc(const char c)
{
	unsigned port_base = BOARD_UART_PORT;
    if (c == '\n') 
        serial_putc('\r');
    
    /* Wait till dataTx register is not full */
    while ((readl(P_UART_STATUS(port_base)) & UART_STAT_MASK_TFIFO_FULL));
    writel(c, P_UART_WFIFO(port_base));
    /* Wait till dataTx register is empty */
}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise 0.
 */
int serial_tstc(void)
{
	unsigned port_base = BOARD_UART_PORT;
	int i;
	i=(readl(P_UART_STATUS(port_base)) & UART_STAT_MASK_RFIFO_CNT);
  return i;

}

/*
 * Read a single byte from the serial port. 
 */
int serial_getc(void)
{
    unsigned char ch;   
    unsigned port_base = BOARD_UART_PORT;
    /* Wait till character is placed in fifo */
  	while((readl(P_UART_STATUS(port_base)) & UART_STAT_MASK_RFIFO_CNT)==0) ;
    
    /* Also check for overflow errors */
    if (readl(P_UART_STATUS(port_base)) & (UART_STAT_MASK_PRTY_ERR | UART_STAT_MASK_FRAM_ERR))
	{
		writel(readl(P_UART_CONTROL(port_base)) |UART_CNTL_MASK_CLR_ERR,P_UART_CONTROL(port_base));//clear errors
		writel(readl(P_UART_CONTROL(port_base)) & (~UART_CNTL_MASK_CLR_ERR),P_UART_CONTROL(port_base));
	}
    
    ch = readl(P_UART_RFIFO(port_base)) & 0x00ff;
    return ((int)ch);

}

void serial_puts(const char *s)
{
    while (*s) {
        serial_putc(*s++);
    }
}


