/*
 * (C) Copyright 2009
 * Marvell Semiconductor <www.marvell.com>
 * Written-by: Prafulla Wadaskar <prafulla@marvell.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#ifndef _ASM_CACHE_H
#define _ASM_CACHE_H

#include <asm/system.h>
#include "arcregs.h"

void flush_cache(unsigned long dummy1, unsigned long dummy2);
void dcache_enable(void);
void dcache_disable(void);
void dcache_clean(void);
void dcache_invalid(void);
void dcache_flush(void);
int dcache_status(void);
void icache_enable(void);
void icache_disable(void);
void icache_invalid(void);
int icache_status(void);
void dcache_flush_line(unsigned addr);
void dcache_clean_line(unsigned addr);
void dcache_inv_line(unsigned addr);
void dcache_flush_range(unsigned start, unsigned size);
void dcache_clean_range(unsigned start, unsigned size);
void dcache_invalid_range(unsigned start, unsigned size);

#endif /* _ASM_CACHE_H */
