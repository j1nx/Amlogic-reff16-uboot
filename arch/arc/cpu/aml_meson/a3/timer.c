#include <common.h>
#include <asm/arch/am_reg_addr.h>
int timer_init(void)
{
    /*
        Select Timer D us Base 
        Select Timer E 100 us base
    */
	clrsetbits_le32(P_ISA_TIMER_MUX,0x7<<8,0x1<<8);
    return 0;
}
/*
 * timer without interrupts
 */
void reset_timer(void)
{
    set_timer(0);
}

ulong get_utimer(ulong base)
{
    return READ_CBUS_REG(ISA_TIMERE)-base;
}
ulong get_timer(ulong base)
{
	return get_utimer(base*1000)/1000;
}

void set_timer(ulong t)
{
    WRITE_CBUS_REG(ISA_TIMERE,t);
}
void __udelay(unsigned long usec)
{
    unsigned long base=get_utimer(0);
    while(get_utimer(base)<usec);
    
}
