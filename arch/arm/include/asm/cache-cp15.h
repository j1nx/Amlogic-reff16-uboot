#ifndef _CACHE_CP15_H_
#define _CACHE_CP15_H_
#include "system.h"
void cp15_icache_enable(void);
void cp15_icache_disable(void);
int cp15_icache_status(void);
void cp15_dcache_enable(void);
void cp15_dcache_disable(void);
int cp15_dcache_status(void);
void _invalidate_dcache(void);
void _clean_dcache(void);
void _clean_invd_dcache(void);
void _clean_dcache_addr(unsigned long addr);
void _invalidate_dcache_addr2(unsigned long addr);
void _invalidate_dcache_addr(unsigned long addr);
void _clean_invd_dcache_addr(unsigned long addr);
void _invalidate_icache(void);

#endif