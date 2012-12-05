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
#include <asm/arch/romboot.h>
/*
 * clear the uart error
 */


int serial_set_pin_port(int port) __attribute__((weak, alias("__serial_set_pin_port")));

int __serial_set_pin_port(int port)
{	
	return 0;
}

/*
 * Sets baudarate
 */
void serial_setbrg_port (int port)
{
    serial_puts("Unsupported\n");
        
}




int serial_init(void){
    enable_interrupts();
	return 0;
}
void serial_putc(const char c){
	romboot_info->putc(c);
}

int serial_tstc(void){
	return romboot_info->tstc();
}

int serial_getc(void){
	return romboot_info->getc();
}
void serial_puts(const char * s){
    while(*s)
    {
        serial_putc(*s++);
    }
}
void serial_setbrg(void){
	serial_setbrg_port(CONFIG_AML_DEF_UART);
}

