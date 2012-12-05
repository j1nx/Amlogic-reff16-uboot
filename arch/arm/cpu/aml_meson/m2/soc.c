#include <asm/arch/am_regs.h>
#include <asm/arch/am_reg_addr.h>
#include <memtest.h>
#include <config.h>
#include <common.h>
void reset_cpu(ulong addr)
{
    /*
    @todo implement this function
    */
    setbits_le32(P_WATCHDOG_TC,1<<22);
    while(1);
}