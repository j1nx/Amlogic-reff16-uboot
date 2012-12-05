
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/io.h>
#ifndef FIRMWARE_IN_ONE_FILE
#define STATIC_PREFIX
#else
#define STATIC_PREFIX static inline
#endif
//#if __CC_ARM
#if 0
void ipl_memcpy(void * dst,void *src_in,int count)
{
    int i;
    unsigned * dest=(unsigned*)dst;
    unsigned * src=(unsigned*)src_in;
    count>>=2;
    while(--count>=0)
    {
        dest[count]=src[count];
    }
}
#endif


STATIC_PREFIX
unsigned get_timer(unsigned base)
{
    return TIMERE_SUB(TIMERE_GET(),base);
}
STATIC_PREFIX
void __udelay(unsigned long usec)
{
    unsigned long base=TIMERE_GET();
    while(TIMERE_SUB(TIMERE_GET(),base)<usec);
    
}
