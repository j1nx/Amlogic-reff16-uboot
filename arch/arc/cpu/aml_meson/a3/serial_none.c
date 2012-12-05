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
 * Sets baudarate
 */
void serial_setbrg_port (int port)
{
}






/*
 * Intialise the serial port with given baudrate
 */
int serial_init_port (int port)
{

    return 0;
}

/*
 * Output a single byte to the serial port.
 */
void serial_putc_port (int port,const char c)
{

}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise 0.
 */
int serial_tstc_port (int port)
{
	return 0;

}

/*
 * Read a single byte from the serial port. 
 */
int serial_getc_port (int port)
{
    
    return ((int)0);

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
	return serial_init_port(DEBUG_PORT);
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
