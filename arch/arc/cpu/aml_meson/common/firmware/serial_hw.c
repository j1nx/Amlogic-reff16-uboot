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
#include <asm/arch/firm/uart.h>
#include <asm/arch/firm/io.h>
#include <asm/arch/firm/config.h>

/*
 * Intialise the serial port with given baudrate
 */
STATIC_PREFIX
unsigned serial_init_firmware(unsigned set)
{
    /* baud rate */
    unsigned baud_para=0;
	writel(set|UART_CNTL_MASK_RST_TX | UART_CNTL_MASK_RST_RX | UART_CNTL_MASK_CLR_ERR
	,P_UART_CONTROL(BOARD_UART_PORT));

    serial_hw_init();

	

    clrbits_le32(P_UART_CONTROL(BOARD_UART_PORT),
	    UART_CNTL_MASK_RST_TX | UART_CNTL_MASK_RST_RX | UART_CNTL_MASK_CLR_ERR);
	return baud_para;
}

/*
 * Output a single byte to the serial port.
 */
STATIC_PREFIX
void serial_putc(const char c)
{
    if (c == '\n') 
    {
        while ((readl(P_UART_STATUS(BOARD_UART_PORT)) & UART_STAT_MASK_TFIFO_FULL));
        writel('\r', P_UART_WFIFO(BOARD_UART_PORT));
        
    }
    
    /* Wait till dataTx register is not full */
    while ((readl(P_UART_STATUS(BOARD_UART_PORT)) & UART_STAT_MASK_TFIFO_FULL));
    writel(c, P_UART_WFIFO(BOARD_UART_PORT));
    /* Wait till dataTx register is empty */
}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise 0.
 */
STATIC_PREFIX
int serial_tstc(void)
{
	return (readl(P_UART_STATUS(BOARD_UART_PORT)) & UART_STAT_MASK_RFIFO_CNT);
}

/*
 * Read a single byte from the serial port. 
 */
STATIC_PREFIX
int serial_getc(void)
{
    unsigned char ch;   
    /* Wait till character is placed in fifo */
  	while((readl(P_UART_STATUS(BOARD_UART_PORT)) & UART_STAT_MASK_RFIFO_CNT)==0) ;
    
    /* Also check for overflow errors */
    if (readl(P_UART_STATUS(BOARD_UART_PORT)) & (UART_STAT_MASK_PRTY_ERR | UART_STAT_MASK_FRAM_ERR))
	{
	    setbits_le32(P_UART_CONTROL(BOARD_UART_PORT),UART_CNTL_MASK_CLR_ERR);
	    clrbits_le32(P_UART_CONTROL(BOARD_UART_PORT),UART_CNTL_MASK_CLR_ERR);
	}
    
    ch = readl(P_UART_RFIFO(BOARD_UART_PORT)) & 0x00ff;
    return ((int)ch);
}


