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
STATIC_PREFIX
void serial_puts(const char *s);
#if CONFIG_SERIAL_HWUART
#include "serial_hw.c"
void process_interrupt(struct pt_regs * pt){}
#endif


#if CONFIG_SERIAL_SWUART
#include "serial_sw.c"
#endif

#if CONFIG_SERIAL_NONE
void process_interrupt(struct pt_regs * pt){}
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
#endif


STATIC_PREFIX
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
#define serial_put_char(data) serial_puts("0x");serial_put_hex((unsigned)data,8);serial_putc('\n')
#define serial_put_dword(data) serial_puts("0x");serial_put_hex((unsigned)data,32);serial_putc('\n')


