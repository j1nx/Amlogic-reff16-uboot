/*
 * (C) Copyright 2011
 * 
 */

/* for now: just dummy functions to satisfy the linker */

#include <common.h>

#include <asm/cache.h>
#if __GNUC__ > 0
#define _lr(x) __builtin_arc_lr(x)
#define _sr(a, b) __builtin_arc_sr(a, b)
#endif
void flush_cache(unsigned long dummy1,unsigned long dummy2)
{
    dcache_flush();
    return;
}
void dcache_enable(void)
{
#ifndef CONFIG_SYS_NO_DCACHE
    _sr(_lr(ARC_REG_DC_CTRL)&~ARC_REG_DC_CTRL_DIS, ARC_REG_DC_CTRL);
#endif
}
void dcache_disable(void)
{
    dcache_flush();
#ifndef CONFIG_SYS_NO_DCACHE
    _sr(_lr(ARC_REG_DC_CTRL)|ARC_REG_DC_CTRL_DIS, ARC_REG_DC_CTRL);
#endif
}
void dcache_clean(void)
{
#ifndef CONFIG_SYS_NO_DCACHE
    if(dcache_status())
    {
        /* also flush the locked lines */
        _sr(_lr(ARC_REG_DC_CTRL) | ARC_REG_DC_CTRL_LM, ARC_REG_DC_CTRL);

        _sr(1, ARC_REG_DC_FLSH);
    
        while (_lr(ARC_REG_DC_CTRL) & ARC_REG_DC_CTRL_FS);
    }
#endif

}

void dcache_invalid(void)
{
#ifndef CONFIG_SYS_NO_DCACHE
    if(dcache_status())
    {
        /* also flush the locked lines */
        _sr(_lr(ARC_REG_DC_CTRL) | ARC_REG_DC_CTRL_LM, ARC_REG_DC_CTRL);
        /** invalidate only **/
        _sr(_lr(ARC_REG_DC_CTRL) & ~ARC_REG_DC_CTRL_IM, ARC_REG_DC_CTRL);

        _sr(1, ARC_REG_DC_IVDC);
    
    }
#endif
}

void dcache_flush(void)
{
#ifndef CONFIG_SYS_NO_DCACHE
    if(dcache_status())
    {
        __builtin_arc_nop();
        __builtin_arc_nop();
        __builtin_arc_nop();
        __builtin_arc_nop();
        /* also flush the locked lines */
        _sr(_lr(ARC_REG_DC_CTRL) | ARC_REG_DC_CTRL_LM | ARC_REG_DC_CTRL_IM, ARC_REG_DC_CTRL);
        /** invalidate and flush **/
        _sr(_lr(ARC_REG_DC_CTRL) | ARC_REG_DC_CTRL_IM, ARC_REG_DC_CTRL);

        _sr(1, ARC_REG_DC_IVDC);
        while (_lr(ARC_REG_DC_CTRL) & ARC_REG_DC_CTRL_FS);
        
    }
#endif
}
int dcache_status(void)
{
#ifndef CONFIG_SYS_NO_DCACHE
	return (_lr(ARC_REG_DC_CTRL)&ARC_REG_DC_CTRL_DIS)!=ARC_REG_DC_CTRL_DIS;
#else
    return 0;
#endif
}


void icache_enable(void)
{
#ifndef CONFIG_SYS_NO_ICACHE
    _sr(_lr(ARC_REG_IC_CTRL)&~ARC_REG_IC_CTRL_DC,ARC_REG_IC_CTRL);
    

#endif
}
void icache_disable(void)
{
#ifndef CONFIG_SYS_NO_ICACHE
    icache_invalid();
    _sr(_lr(ARC_REG_IC_CTRL)|ARC_REG_IC_CTRL_DC,ARC_REG_IC_CTRL);
#endif
}

void icache_invalid(void)
{
#ifndef CONFIG_SYS_NO_ICACHE        
    if(icache_status())
    {
        __builtin_arc_nop();
        __builtin_arc_nop();
        __builtin_arc_nop();
        __builtin_arc_nop();
        _sr(1,ARC_REG_IC_IVIC);
        __builtin_arc_nop();
        __builtin_arc_nop();
        __builtin_arc_nop();
        __builtin_arc_nop();
        
    }
#endif    
}
int icache_status(void)
{
#ifndef CONFIG_SYS_NO_ICACHE        
	return (_lr(ARC_REG_IC_CTRL)&ARC_REG_IC_CTRL_DC)!=ARC_REG_IC_CTRL_DC;
#else
    return 0;
#endif	
}

#define CACHE_LINE_SIZE 64
void dcache_flush_line(unsigned addr)
{
#ifndef CONFIG_SYS_NO_DCACHE  
        _sr(addr,ARC_REG_DC_IVDL);
#endif

}
void dcache_clean_line(unsigned addr)
{
#ifndef CONFIG_SYS_NO_DCACHE  
        _sr(addr,ARC_REG_DC_FLDL);
#endif

}
void dcache_inv_line(unsigned addr)
{
#ifndef CONFIG_SYS_NO_DCACHE  
        _sr(addr,ARC_REG_DC_IVDL);    
#endif

}
void dcache_flush_range(unsigned start, unsigned size)
{
    unsigned st,end,i;
    if(!dcache_status())
        return;
    st=start&(~(CACHE_LINE_SIZE-1));
    end=start+size;
    /** invalidate and flush **/
    _sr(_lr(ARC_REG_DC_CTRL) | ARC_REG_DC_CTRL_IM, ARC_REG_DC_CTRL);
    for(i=st;i<end;i+=CACHE_LINE_SIZE)
    {
        dcache_flush_line(i);
    }
    /** wait flush end */
    while (_lr(ARC_REG_DC_CTRL) & ARC_REG_DC_CTRL_FS);
}
void dcache_clean_range(unsigned start,unsigned size)
{
    if(!dcache_status())
        return;
    unsigned st,end,i;
    st=start&(~(CACHE_LINE_SIZE-1));
    
    end=start+size;
    for(i=st;i<end;i+=CACHE_LINE_SIZE)
    {
        dcache_clean_line(i);
    }
    /** wait flush end */
    while (_lr(ARC_REG_DC_CTRL) & ARC_REG_DC_CTRL_FS);
    
}
void dcache_invalid_range(unsigned start, unsigned size)
{
    unsigned st,end,i;
    if(!dcache_status())
        return;
    st=start&(~(CACHE_LINE_SIZE-1));
    end=(start+size)&(~(CACHE_LINE_SIZE-1));
    /** invalidate and flush **/
    _sr(_lr(ARC_REG_DC_CTRL) &(~ ARC_REG_DC_CTRL_IM), ARC_REG_DC_CTRL);
    if(st!=start)
    {
        dcache_clean_line(st);
    }
    //st+=CACHE_LINE_SIZE;
    if(end!=(start+size))
    {
        dcache_clean_line(end);
        end+=CACHE_LINE_SIZE;
    }
  //  end-=CACHE_LINE_SIZE;
    /** wait flush end */
    while (_lr(ARC_REG_DC_CTRL) & ARC_REG_DC_CTRL_FS);
    for(i=st;i<end;i+=CACHE_LINE_SIZE)
    {
        dcache_inv_line(i);
    }
    while (_lr(ARC_REG_DC_CTRL) & ARC_REG_DC_CTRL_FS);
    
}
