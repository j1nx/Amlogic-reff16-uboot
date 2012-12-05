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
/*
 * clear the uart error
 */

/*
 * Sets stop bits
 */
static unsigned port_base_addrs[]={UART_PORT_0,UART_PORT_1};
static void serial_clr_err (int port)
{
    /* write to the register */
	unsigned port_base=port_base_addrs[port];
	if(readl(P_UART_STATUS(port_base))&(UART_STAT_MASK_PRTY_ERR|UART_STAT_MASK_FRAM_ERR))
		writel((readl(P_UART_CONTROL(port_base)) | UART_CNTL_MASK_CLR_ERR), P_UART_CONTROL(port_base));
}
int serial_set_pin_port(int port) __attribute__((weak, alias("__serial_set_pin_port")));

int __serial_set_pin_port(int port)
{	

#ifdef	CONFIG_BOARD_MBOX_8626M1
    if(port==UART_A)
            setbits_le32(P_PERIPHS_PIN_MUX_2,((1<<15)|(1<<11)));
    if(port==UART_B){
            //SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_3,((1 << 30) | (1 << 27)));
            CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, ((1 << 15) | (1 << 19))); //Audio Out
            CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_8, ((1 << 8) | (1 << 8))); //Audio In
            CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, ((1 << 16) | (1 << 20))); //I2C Master B
            CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, ((1 << 17) | (1 << 21))); //I2C Slave
            CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, ((1 << 25) | (1 << 26))); //PWM
            CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_0, (1 << 29)); //JTAG enable
            SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_0, (1 << 30)); //JTAG disable
            *(volatile unsigned*)0xc11084e4 = 0x3196;//0x83196;//UART_B
            SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_1,((1 << 18) | (1 << 22)));
        }
#elif defined CONFIG_BOARD_DPF_6236M_SZ
    if(port==UART_A)
            SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_7,((1 << 11) | (1 << 8)));
    if(port==UART_B)
            SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_3,((1 << 30) | (1 << 27)));
#elif defined CONFIG_BOARD_8726M_MID || defined CONFIG_BOARD_8726M_ARM
    if(port==UART_A)
    {
        *(volatile unsigned*)0xc11084c8 = 0x3196;//0x83196;//UART_A
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, ((1 << 23) | (1 << 24)));
    }
    else if(port==UART_B)
    {
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, ((1 << 11) | (1 << 12)));
        *(volatile unsigned*)0xc11084e4 = 0x3196;//0x83196;//UART_B
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_5, ((1 << 23) | (1 << 24)));
    }
#endif

	return 0;
}

/*
 * Sets baudarate
 */
void serial_setbrg_port (int port)
{
        
    unsigned long baud_para;
    DECLARE_GLOBAL_DATA_PTR;
    unsigned port_base=port_base_addrs[port];
    
    /* baud rate */
    baud_para=get_clk81()/(gd->baudrate*4) -1;
//   baud_para=25000000/(115200*4) -1;

    baud_para &= UART_CNTL_MASK_BAUD_RATE;
    
    /* write to the register */ 
    writel((readl(P_UART_CONTROL(port_base)) & ~UART_CNTL_MASK_BAUD_RATE) | baud_para, P_UART_CONTROL(port_base));
}
//void serial_setbrg()
//{
//	serial_setbrg_port(UART);
//}

/*
 * Sets stop bits
 * input[0]: stop_bits (1, 2)
 */
static void serial_set_stop_port (int port,int stop_bits)
{   
    unsigned long uart_config;
    unsigned port_base=port_base_addrs[port];

    uart_config = readl(P_UART_CONTROL(port_base)) & ~UART_CNTL_MASK_STP_BITS;
    /* stop bits */
    switch(stop_bits)
    {
        case 2:
            uart_config |= UART_CNTL_MASK_STP_2BIT;
            break;
        case 1:
        default:
            uart_config |= UART_CNTL_MASK_STP_1BIT;
            break;      
    }
    
    /* write to the register */
    writel(uart_config, P_UART_CONTROL(port_base));
}

/*
 * Sets parity type
 * input[0]: 1 -- enable parity, 0 -- disable;
 * input[1]: 1 -- odd parity, 0 -- even parity;
 */
static void serial_set_parity_port(int port,int type)
{
    unsigned long uart_config;
    unsigned port_base=port_base_addrs[port];

    uart_config = readl(P_UART_CONTROL(port_base)) & ~(UART_CNTL_MASK_PRTY_TYPE | UART_CNTL_MASK_PRTY_EN);
#if 0   //changed by Elvis --- disable parity
    uart_config |= UART_CNTL_MASK_PRTY_EN;
    /* parity bits */
    if(type&2)
        uart_config |= UART_CNTL_MASK_PRTY_EN;
    if(type&1)
        uart_config |= UART_CNTL_MASK_PRTY_ODD;
    else
        uart_config |= UART_CNTL_MASK_PRTY_EVEN;
 #endif   
    /* write to the register */
    writel(uart_config, P_UART_CONTROL(port_base));
//    if(port==UART_B)
//    {
//        while(1)
//        serial_puts_port(UART_B,"aaa\n");
//    }
}

/*
 * Sets data length
 * input[0]: Character length [5, 6, 7, 8]
 */
static void serial_set_dlen_port (int port,int data_len)
{
    unsigned long uart_config;
    unsigned port_base=port_base_addrs[port];
    uart_config = readl(P_UART_CONTROL(port_base)) & ~UART_CNTL_MASK_CHAR_LEN;
    /* data bits */
    switch(data_len)
    {
        case 5:
            uart_config |= UART_CNTL_MASK_CHAR_5BIT;
            break;
        case 6:
            uart_config |= UART_CNTL_MASK_CHAR_6BIT;
            break;
        case 7:
            uart_config |= UART_CNTL_MASK_CHAR_7BIT;
            break;
        case 8:
        default:
            uart_config |= UART_CNTL_MASK_CHAR_8BIT;
            break;
    }   
    
    /* write to the register */
    writel(uart_config, P_UART_CONTROL(port_base));
}



/*
 * reset the uart state machine
 */
static void serial_reset_port(int port) {
	unsigned port_base = port_base_addrs[port];
	/* write to the register */
	writel(readl(P_UART_CONTROL(port_base)) | UART_CNTL_MASK_RST_TX | UART_CNTL_MASK_RST_RX | UART_CNTL_MASK_CLR_ERR, P_UART_CONTROL(port_base));
	writel(readl(P_UART_CONTROL(port_base)) & ~(UART_CNTL_MASK_RST_TX | UART_CNTL_MASK_RST_RX | UART_CNTL_MASK_CLR_ERR), P_UART_CONTROL(port_base));
}

/*
 * Intialise the serial port with given baudrate
 */
int serial_init_port (int port)
{
	int ret;
	unsigned port_base = port_base_addrs[port];
//    serial_puts_port(port," ");
    writel(0,P_UART_CONTROL(port_base));
    ret = serial_set_pin_port(port);
    if (ret < 0)
    	return -1;
    
    serial_setbrg_port(port);
#ifndef CONFIG_SERIAL_STP_BITS
#define CONFIG_SERIAL_STP_BITS 1
#endif    
    serial_set_stop_port(port,CONFIG_SERIAL_STP_BITS);
#ifndef CONFIG_SERIAL_PRTY_TYPE
#define CONFIG_SERIAL_PRTY_TYPE 0
#endif

    serial_set_parity_port(port,CONFIG_SERIAL_PRTY_TYPE);
#ifndef CONFIG_SERIAL_CHAR_LEN
#define CONFIG_SERIAL_CHAR_LEN 8
#endif
    serial_set_dlen_port(port,CONFIG_SERIAL_CHAR_LEN);
    writel(readl(P_UART_CONTROL(port_base)) | UART_CNTL_MASK_TX_EN | UART_CNTL_MASK_RX_EN, P_UART_CONTROL(port_base));
    while(!(readl(P_UART_STATUS(port_base)) & UART_STAT_MASK_TFIFO_EMPTY));
    serial_reset_port(port);
    serial_putc_port(port,'\n');
   
    return 0;
}

/*
 * Output a single byte to the serial port.
 */
void serial_putc_port (int port,const char c)
{
	unsigned port_base = port_base_addrs[port];
    if (c == '\n') 
        serial_putc_port(port,'\r');
    
    /* Wait till dataTx register is not full */
    while ((readl(P_UART_STATUS(port_base)) & UART_STAT_MASK_TFIFO_FULL));
 // while(!(readl(P_UART_STATUS(port_base)) & UART_STAT_MASK_TFIFO_EMPTY));  
    writel(c, P_UART_WFIFO(port_base));
    /* Wait till dataTx register is empty */
    while(!(readl(P_UART_STATUS(port_base)) & UART_STAT_MASK_TFIFO_EMPTY));

}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise 0.
 */
int serial_tstc_port (int port)
{
	unsigned port_base = port_base_addrs[port];
	int i;

	i=(readl(P_UART_STATUS(port_base)) & UART_STAT_MASK_RFIFO_CNT);
	return i;

}

/*
 * Read a single byte from the serial port. 
 */
int serial_getc_port (int port)
{
    unsigned char ch;   
    unsigned port_base = port_base_addrs[port];
    /* Wait till character is placed in fifo */
  	while((readl(P_UART_STATUS(port_base)) & UART_STAT_MASK_RFIFO_CNT)==0) ;
  	ch = readl(P_UART_RFIFO(port_base)) & 0x00ff;
    /* Also check for overflow errors */
    if (readl(P_UART_STATUS(port_base)) & (UART_STAT_MASK_PRTY_ERR | UART_STAT_MASK_FRAM_ERR))
    {
    	writel(readl(P_UART_CONTROL(port_base)) |UART_CNTL_MASK_CLR_ERR,P_UART_CONTROL(port_base));//clear errors
        writel(readl(P_UART_CONTROL(port_base)) & (~UART_CNTL_MASK_CLR_ERR),P_UART_CONTROL(port_base));

    }
    

    return ((int)ch);

}

void serial_puts_port (int port,const char *s)
{
    while (*s) {
        serial_putc_port(port,*s++);
    }
}
#if 0
void uart_putc(char c)
{
	serial_putc_port(UART_B,c);
}
void uart_puts(char *s)
{
	serial_puts_port(UART_B,s);
}
int uart_getc(void)
{
	while (!serial_tstc_port(UART_B));
	return serial_getc_port(UART_B);
}
#endif
#if (!defined AML_DEBUG_ROM)
void serial_putdata_port(int port,const char *dat, int count)
{
	int i;
	if(!dat)
		return ;
	for(i=0;i<count;i++)
	{
        serial_putc_port(port,*dat++);
	}
}
#endif
__attribute__((weak))
int serial_init(void){
	return serial_init_port(CONFIG_AML_DEF_UART);
}
__attribute__((weak))
void serial_putc(const char c){
	serial_putc_port(CONFIG_AML_DEF_UART,c);
}
__attribute__((weak))
int serial_tstc(void){
	return serial_tstc_port(CONFIG_AML_DEF_UART);
}
__attribute__((weak))
int serial_getc(void){
	return serial_getc_port(CONFIG_AML_DEF_UART);
}
__attribute__((weak))
void serial_puts(const char * s){
	serial_puts_port(CONFIG_AML_DEF_UART,s);
}
#if (!defined AML_DEBUG_ROM)
__attribute__((weak))
void serial_putdata(const char * dat,int len){
	serial_putdata_port(CONFIG_AML_DEF_UART,dat,len);
}
#endif
__attribute__((weak))
void serial_setbrg(void){
	serial_setbrg_port(CONFIG_AML_DEF_UART);
}

#if CONFIG_AML_DEF_UART
#define DEBUG_PORT 0
#else
#define DEBUG_PORT 1
#endif
#if (!defined AML_DEBUG_ROM)
__attribute__((weak))
int kgdb_serial_init(void){
    int ret=serial_init_port(DEBUG_PORT);
    
	return ret;
}
__attribute__((weak))
int getDebugChar(void){
	return serial_getc_port(DEBUG_PORT);
}
__attribute__((weak))
void putDebugChar(const char c){
	serial_putc_port(DEBUG_PORT,c);
}
__attribute__((weak))
void putDebugStr(const char * s){
	serial_puts_port(DEBUG_PORT,s);
}
#endif
