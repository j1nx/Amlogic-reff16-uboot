/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* for now: just dummy functions to satisfy the linker */

#include <common.h>
#include <asm/cache.h>
#include <asm/cache-cp15.h>
#include <asm/cache-l2x0.h>
void flush_cache(unsigned long dummy1,unsigned long dummy2)
{
    dcache_flush();
    return;
}
void dcache_enable(void)
{
#ifndef CONFIG_SYS_NO_DCACHE
    cp15_dcache_enable();
#endif
#ifdef CONFIG_CACHE_L2X0
    l2x0_enable();
#endif
}
void dcache_disable(void)
{
    dcache_flush();
#ifndef CONFIG_SYS_NO_DCACHE
    cp15_dcache_disable();
#endif
#ifdef CONFIG_CACHE_L2X0
    l2x0_disable();
#endif
}
void dcache_clean(void)
{
#ifndef CONFIG_SYS_NO_DCACHE
    if(cp15_dcache_status())
    {
        _clean_dcache();
    }
#endif
#ifdef CONFIG_CACHE_L2X0
    if(l2x0_status())
    {
        l2x0_clean_all();
    }
#endif
}

void dcache_invalid(void)
{
#ifndef CONFIG_SYS_NO_DCACHE
    _invalidate_dcache();
#endif
#ifdef CONFIG_CACHE_L2X0
    l2x0_inv_all();
#endif
}

void dcache_flush(void)
{
#ifndef CONFIG_SYS_NO_DCACHE
    if(cp15_dcache_status())
    {
        _clean_invd_dcache();
    }
#endif
#ifdef CONFIG_CACHE_L2X0
    if(l2x0_status())
    {
        l2x0_clean_inv_all();
    }
#endif
}
int dcache_status(void)
{
#ifndef CONFIG_SYS_NO_DCACHE
	return cp15_dcache_status();
#else
    return 0;
#endif
}


void icache_enable(void)
{
#ifndef CONFIG_SYS_NO_ICACHE
    cp15_icache_enable();

#endif
}
void icache_disable(void)
{
#ifndef CONFIG_SYS_NO_ICACHE
    cp15_icache_disable();
#endif
}

void icache_invalid(void)
{
#ifndef CONFIG_SYS_NO_ICACHE        
    _invalidate_icache();
#endif    
}
int icache_status(void)
{
#ifndef CONFIG_SYS_NO_ICACHE        
	return cp15_icache_status();
#else
    return 0;
#endif	
}

#define CACHE_LINE_SIZE 32
void dcache_flush_line(unsigned addr)
{
#ifndef CONFIG_SYS_NO_DCACHE  
        _clean_invd_dcache_addr(addr);    
#endif
#ifdef CONFIG_CACHE_L2X0
      l2x0_flush_line(addr);  
#endif

}
void dcache_clean_line(unsigned addr)
{
#ifndef CONFIG_SYS_NO_DCACHE  
        _clean_dcache_addr(addr);    
#endif
#ifdef CONFIG_CACHE_L2X0
      l2x0_clean_line(addr);  
#endif

}
void dcache_inv_line(unsigned addr)
{
#ifndef CONFIG_SYS_NO_DCACHE  
        _invalidate_dcache_addr(addr);    
#endif
#ifdef CONFIG_CACHE_L2X0
      l2x0_inv_line(addr);  
#endif

}
void dcache_flush_range(unsigned start, unsigned size)
{
    unsigned st,end,i;
    st=start&(~(CACHE_LINE_SIZE-1));
    end=start+size;
    for(i=st;i<end;i+=CACHE_LINE_SIZE)
    {
        dcache_flush_line(i);
    }
#ifdef CONFIG_CACHE_L2X0
    l2x0_wait_flush();
#endif    
    
}
void dcache_clean_range(unsigned start,unsigned size)
{
    unsigned st,end,i;
    st=start&(~(CACHE_LINE_SIZE-1));
    end=start+size;
    for(i=st;i<end;i+=CACHE_LINE_SIZE)
    {
        dcache_clean_line(i);
    }
#ifdef CONFIG_CACHE_L2X0
    l2x0_wait_clean();
#endif    
    
}
void dcache_invalid_range(unsigned start, unsigned size)
{
    unsigned st,end,i;
    st=start&(~(CACHE_LINE_SIZE-1));
    end=(start+size)&(~(CACHE_LINE_SIZE-1));
    if(st!=start)
    {
        dcache_flush_line(st);
    }
    if(end!=(start+size))
    {
        dcache_flush_line(end);
        end+=CACHE_LINE_SIZE;
    }
#ifdef CONFIG_CACHE_L2X0    
    l2x0_wait_flush();
#endif    
    for(i=st;i<end;i+=CACHE_LINE_SIZE)
    {
        dcache_inv_line(i);
    }
#ifdef CONFIG_CACHE_L2X0    
    l2x0_wait_inv();
#endif    
    
}
