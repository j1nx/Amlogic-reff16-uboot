
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/io.h>
#include <asm/arch/firm/config.h>
#include <asm/arcregs.h>
#define _lr(x) __builtin_arc_lr(x)
#define _sr(a, b) __builtin_arc_sr(a, b)
void enable_cache()
{
    //dcache;
    _sr(_lr(ARC_REG_DC_CTRL)&~ARC_REG_DC_CTRL_DIS, ARC_REG_DC_CTRL);
    //icache;
    _sr(_lr(ARC_REG_IC_CTRL)&~ARC_REG_IC_CTRL_DC,ARC_REG_IC_CTRL);
}
void disable_cache()
{
    __builtin_arc_nop();
        __builtin_arc_nop();
        __builtin_arc_nop();
        __builtin_arc_nop();
        _sr(_lr(ARC_REG_DC_CTRL) | ARC_REG_DC_CTRL_LM | ARC_REG_DC_CTRL_IM, ARC_REG_DC_CTRL);
        /** invalidate and flush **/
//        _sr(_lr(ARC_REG_DC_CTRL) | ARC_REG_DC_CTRL_IM, ARC_REG_DC_CTRL);

        _sr(1, ARC_REG_DC_IVDC);
        while (_lr(ARC_REG_DC_CTRL) & ARC_REG_DC_CTRL_FS);

        _sr(1,ARC_REG_IC_IVIC);
        __builtin_arc_nop();
        __builtin_arc_nop();
        __builtin_arc_nop();
        __builtin_arc_nop();
    //dcache;
    _sr(_lr(ARC_REG_DC_CTRL)|ARC_REG_DC_CTRL_DIS, ARC_REG_DC_CTRL);
    //icache;
    _sr(_lr(ARC_REG_IC_CTRL)|ARC_REG_IC_CTRL_DC,ARC_REG_IC_CTRL);
}
//#if __CC_ARM
#if 1
void ipl_memcpy(void * dst,void *src_in,int count)
{
    
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
