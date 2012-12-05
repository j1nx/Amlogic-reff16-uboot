#include <config.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/uart.h>
#include <asm/arch/firm/io.h>
#include <asm/arch/firm/config.h>


STATIC_PREFIX void serial_hw_init(void)
{
        clrbits_le32(P_PERIPHS_PIN_MUX_4,1<<11);
        clrbits_le32(P_PERIPHS_PIN_MUX_7,((1 << 6) | (1 << 7)));
        clrbits_le32(0xc1100000+0x2012*4,((1 << 14) | (1 << 15)));
        clrbits_le32(0xc1100000+0x2013*4,((1 << 14) | (1 << 15)));
        int i;
        for(i=0;i<10000;i++);
        setbits_le32(P_PERIPHS_PIN_MUX_7,((1 << 6) | (1 << 7)));
}
