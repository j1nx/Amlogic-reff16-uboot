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
#ifndef FIRMWARE_IN_ONE_FILE
#define STATIC_PREFIX
#else
#define STATIC_PREFIX static inline
#endif
#ifndef CONFIG_DISABLE_FIRMWARE_OUTPUT

#ifndef CONFIG_AML_MESION_3
#if (BOARD_UART_PORT!=UART_PORT_0) && (BOARD_UART_PORT!=UART_PORT_1)
#if CONFIG_AML_DEF_UART==0
#define BOARD_UART_PORT UART_PORT_0
#elif CONFIG_AML_DEF_UART==1
#define BOARD_UART_PORT UART_PORT_1
#else
#error IPL BOARD_UART_PORT configure Error .
#endif
#endif
#else
#define BOARD_UART_PORT UART_PORT_AO
#endif
/*
 * Intialise the serial port with given baudrate
 */
#if CONFIG_SERIAL_NONE 
STATIC_PREFIX
int serial_tstc(void){
    return 0;
}
STATIC_PREFIX
int serial_getc(void){
    return 0;
}

STATIC_PREFIX
unsigned serial_init_firmware(unsigned set){
    return 0;
}
STATIC_PREFIX
void serial_putc(const char c)
{
    return 0;
}
#else
STATIC_PREFIX
unsigned serial_init_firmware(unsigned set)
{
    /* baud rate */
    unsigned baud_para=0;
//    unsigned clk;
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
//STATIC_PREFIX
void serial_putc(const char c)
{
//	unsigned port_base = BOARD_UART_PORT;
    if (c == '\n') 
    {
//        UART_STAT_MASK_TFIFO_CNT
        while ((readl(P_UART_STATUS(BOARD_UART_PORT)) & UART_STAT_MASK_TFIFO_FULL));
        writel('\r', P_UART_WFIFO(BOARD_UART_PORT));
        
    }
    
    /* Wait till dataTx register is not full */
    while ((readl(P_UART_STATUS(BOARD_UART_PORT)) & UART_STAT_MASK_TFIFO_FULL));
    writel(c, P_UART_WFIFO(BOARD_UART_PORT));
    /* Wait till dataTx register is empty */
}
STATIC_PREFIX 
void serial_wait_tx_empty()
{
    while ((readl(P_UART_STATUS(BOARD_UART_PORT)) & UART_STAT_MASK_TFIFO_EMPTY)==0);
    
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
#endif
//STATIC_PREFIX
void serial_puts(const char *s)
{
    while (*s) {
        serial_putc(*s++);
    }
}
STATIC_PREFIX
void serial_put_hex(unsigned int data,unsigned bitlen)
{
	int i;
//	if(data==0)
//	{
//	    serial_putc(0x30);
//	    return ;
//	}
    for (i=bitlen-4;i>=0;i-=4){
        if((data>>i)==0)
        {
            serial_putc(0x30);
            continue;
        }
 
        unsigned char s = (data>>i)&0xf;
        if (s<10)
            serial_putc(0x30+s);
        else
            serial_putc(0x61+s-10);
        
    }
    
}
#define serial_put_char(data) {unsigned a=(unsigned)data;serial_puts("0x");serial_put_hex(a,8);serial_putc('\n');}
#define serial_put_dword(data) {unsigned a=(unsigned)data;serial_puts("0x");serial_put_hex(a,32);serial_putc('\n');}
#endif
#ifndef CONFIG_AML_MESION_3
void do_exception(unsigned reason,unsigned lr)
{
    serial_puts("Enter Exception:");
    serial_put_dword(reason);
    serial_put_dword(lr);
//    serial_put_dword(testmem(0));
    while(1);
    writel((1<<22)|1000000,P_WATCHDOG_TC);//enable Watchdog
}
#endif