#include <common.h>
#include <asm/arch/am_reg_addr.h>
int timer_init(void)
{
    /*
        Select Timer D us Base 
        Select Timer E 100 us base
    */
//    WRITE_CBUS_REG_BITS(ISA_TIMER_MUX,4,TIMER_D_INPUT_BIT,4);
	clrsetbits_le32(P_ISA_TIMER_MUX,0x1f<<6,0x10<<6);
    return 0;
}
/*
 * timer without interrupts
 */
void reset_timer(void)
{
    set_timer(0);
}

ulong get_timer(ulong base)
{
    return (READ_CBUS_REG(ISA_TIMERE))-base;
}

void set_timer(ulong t)
{
    WRITE_CBUS_REG(ISA_TIMERE,t*10);
	
}
void __udelay(unsigned long usec)
{
    if(usec&(~0xffff)){
        debug("error in %s: %d\n",__func__ , usec);
        return ;
    }
    WRITE_CBUS_REG(ISA_TIMERD,usec);
    while((READ_CBUS_REG(ISA_TIMERD)&0xffff)>0);
}
