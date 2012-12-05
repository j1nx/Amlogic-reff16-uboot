/******************************************************************************
 * Copyright Codito Technologies (www.codito.com) Oct 01, 2004
 * 
 * Additional contributions by ARC International, 2005.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *****************************************************************************/

/*
 *  linux/include/asm-arc/io.h
 *
 *  Copyright (C) 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Authors : Amit Bhor
 */

#include <linux/types.h>

#ifndef _ASM_ARC_IO_H
#define _ASM_ARC_IO_H

#define IO_WRITE32(val,addr)  ({__asm__ __volatile__ ("st.di %0,[%1]"::"r"((val)),"r"((addr))); })
#define IO_READ32(addr) 		({unsigned int val=0;__asm__ __volatile__ ("ld.di %0,[%1]":"=&r"((val)):"r"((addr)));val;})

#define IO_WRITE8(val,addr) ({__asm__ __volatile__ ("stb.di %0,[%1]"::"r"((val)),"r"((addr))); })
#define IO_READ8(addr)	({unsigned int val=0;__asm__ __volatile__ ("ldb.di %0,[%1]":"=&r"((val)):"r"((addr)));val;})

#define writeb(val,addr)   ({__asm__ __volatile__ ("stb.di %0,[%1]"::"r"((val)),"r"((addr))); })
#define writew(val,addr)   ({__asm__ __volatile__ ("stw.di %0,[%1]"::"r"((val)),"r"((addr))); })
#define writel(val,addr)   ({__asm__ __volatile__ ("st.di %0,[%1]"::"r"((val)),"r"((addr))); })

#define readb(addr)	       ({unsigned int val=0;__asm__ __volatile__ ("ldb.di %0,[%1]":"=&r"((val)):"r"((addr)));val;})
#define readw(addr)        ({unsigned int val=0; __asm__ __volatile__ ("ldw.di %0,[%1]":"=&r"((val)):"r"((addr)));val;})
#define readl(addr)        ({unsigned int val=0;__asm__ __volatile__ ("ld.di %0,[%1]":"=&r"((val)):"r"((addr)));val;})

#define __raw_readb       readb
#define __raw_readw       readw
#define __raw_readl       readl

#define __raw_writeb      writeb
#define __raw_writew      writew
#define __raw_writel      writel

/* These can't be used, because they don't make any sense like this - PS */
#define memset_io(a,b,c)        memset((void *)(a),(b),(c))
#define memcpy_fromio(a,b,c)    memcpy((a),(void *)(b),(c))
#define memcpy_toio(a,b,c)      memcpy((void *)(a),(b),(c))

#define __io(a)                 (a)
#define __mem_pci(a)            ((unsigned long)(a))
#define __mem_isa(a)            ((unsigned long)(a))

static inline int __raw_readsb(unsigned int addr, void *data, int bytelen) {
  __asm__ __volatile__ ("1:ld.di  r8,[r0];\n"
			"sub.f  r2,r2,1;\n"
                        "bnz.d 1b;\n"
                        "stb.ab  r8,[r1,1];\n"
			: 
			: "r" (addr), "r" (data), "r" (bytelen)
			: "r8" );
  return bytelen;
}

static inline int __raw_readsw(unsigned int addr, void *data, int wordlen) {
  __asm__ __volatile__ ("1:ld.di  r8,[r0];\n"
			"sub.f  r2,r2,1;\n"
                        "bnz.d 1b;\n"
                        "stw.ab  r8,[r1,2];\n"
			: 
			: "r" (addr), "r" (data), "r" (wordlen)
			: "r8" );
  return wordlen;
}

static inline int __raw_readsl(unsigned int addr, void *data, int longlen) {
  __asm__ __volatile__ ("1:ld.di  r8,[r0];\n"
			"sub.f  r2,r2,1;\n"
                        "bnz.d 1b;\n"
                        "st.ab  r8,[r1,4];\n"
			: 
			: "r" (addr), "r" (data), "r" (longlen)
			: "r8" );
  return longlen;
}

static inline int __raw_writesb(unsigned int addr, void *data, int bytelen) {
  __asm__ __volatile__ ("1:ldb.ab  r8,[r1,1];\n"
			"sub.f  r2,r2,1;\n"
                        "bnz.d 1b;\n"
                        "st.di  r8,[r0,0];\n"
			: 
			: "r" (addr), "r" (data), "r" (bytelen)
			: "r8" );
  return bytelen;
}

static inline int __raw_writesw(unsigned int addr, void *data, int wordlen) {
   __asm__ __volatile__ ("1:ldw.ab  r8,[r1,2];\n"
			"sub.f  r2,r2,1;\n"
                        "bnz.d 1b;\n"
                        "st.ab.di  r8,[r0,0];\n"
			: 
			: "r" (addr), "r" (data), "r" (wordlen)
			: "r8" );
  return wordlen;
}

static inline int __raw_writesl(unsigned int addr, void *data, int longlen) {
  __asm__ __volatile__ ("1:ld.ab  r8,[r1,4];\n"
			"sub.f  r2,r2,1;\n"
                        "bnz.d 1b;\n"
                        "st.ab.di  r8,[r0,0];\n"
			: 
			: "r" (addr), "r" (data), "r" (longlen)
			: "r8" );
  return longlen;
}

/*
 * Generic virtual read/write
 */
#define __arch_getw(a)          (*(volatile unsigned short *)(a))
#define __arch_putw(v,a)        (*(volatile unsigned short *)(a) = (v))

#define iomem_valid_addr(iomem,sz)      (1)
#define iomem_to_phys(iomem)            (iomem)

#ifdef __io
#define outb(v,p)                       __raw_writeb(v,__io(p))
#define outw(v,p)                       __raw_writew(cpu_to_le16(v),__io(p))
#define outl(v,p)                       __raw_writel(cpu_to_le32(v),__io(p))

#define inb(p)  ({ unsigned int __v = __raw_readb(__io(p)); __v; })
#define inw(p)  ({ unsigned int __v = le16_to_cpu(__raw_readw(__io(p))); __v; })
#define inl(p)  ({ unsigned int __v = le32_to_cpu(__raw_readl(__io(p))); __v; })

#define outsb(p,d,l)                    __raw_writesb(__io(p),d,l)
#define outsw(p,d,l)                    __raw_writesw(__io(p),d,l)
#define outsl(p,d,l)                    __raw_writesl(__io(p),d,l)

#define insb(p,d,l)                     __raw_readsb(__io(p),d,l)
#define insw(p,d,l)                     __raw_readsw(__io(p),d,l)
#define insl(p,d,l)                     __raw_readsl(__io(p),d,l)
#endif

#define outb_p(val,port)                outb((val),(port))
#define outw_p(val,port)                outw((val),(port))
#define outl_p(val,port)                outl((val),(port))
#define inb_p(port)                     inb((port))
#define inw_p(port)                     inw((port))
#define inl_p(port)                     inl((port))

#define outsb_p(port,from,len)          outsb(port,from,len)
#define outsw_p(port,from,len)          outsw(port,from,len)
#define outsl_p(port,from,len)          outsl(port,from,len)
#define insb_p(port,to,len)             insb(port,to,len)
#define insw_p(port,to,len)             insw(port,to,len)
#define insl_p(port,to,len)             insl(port,to,len)
/*
#define readb(addr) (*(volatile unsigned char *) (addr))
#define readw(addr) (*(volatile unsigned short *) (addr))
#define readl(addr) (*(volatile unsigned int *) (addr))
#define writeb(b,addr) (*(volatile unsigned char *) (addr) = (b))
#define writew(b,addr) (*(volatile unsigned short *) (addr) = (b))
#define writel(b,addr) (*(volatile unsigned int *) (addr) = (b))
*/
/*
 * Clear and set bits in one shot. These macros can be used to clear and
 * set multiple bits in a register using a single call. These macros can
 * also be used to set a multiple-bit bit pattern using a mask, by
 * specifying the mask in the 'clear' parameter and the new bit pattern
 * in the 'set' parameter.
 */

#define out_arch(type,endian,a,v)	__raw_write##type(cpu_to_##endian(v),a)
#define in_arch(type,endian,a)		endian##_to_cpu(__raw_read##type(a))

#define out_le32(a,v)	out_arch(l,le32,a,v)
#define out_le16(a,v)	out_arch(w,le16,a,v)

#define in_le32(a)	in_arch(l,le32,a)
#define in_le16(a)	in_arch(w,le16,a)

#define out_be32(a,v)	out_arch(l,be32,a,v)
#define out_be16(a,v)	out_arch(w,be16,a,v)

#define in_be32(a)	in_arch(l,be32,a)
#define in_be16(a)	in_arch(w,be16,a)

#define out_8(a,v)	__raw_writeb(v,a)
#define in_8(a)		__raw_readb(a)

#define clrbits(type, addr, clear) \
	out_##type((addr), in_##type(addr) & ~(clear))

#define setbits(type, addr, set) \
	out_##type((addr), in_##type(addr) | (set))

#define clrsetbits(type, addr, clear, set) \
	out_##type((addr), (in_##type(addr) & ~(clear)) | (set))

#define clrbits_be32(addr, clear) clrbits(be32, addr, clear)
#define setbits_be32(addr, set) setbits(be32, addr, set)
#define clrsetbits_be32(addr, clear, set) clrsetbits(be32, addr, clear, set)

#define clrbits_le32(addr, clear) clrbits(le32, addr, clear)
#define setbits_le32(addr, set) setbits(le32, addr, set)
#define clrsetbits_le32(addr, clear, set) clrsetbits(le32, addr, clear, set)

#define clrbits_be16(addr, clear) clrbits(be16, addr, clear)
#define setbits_be16(addr, set) setbits(be16, addr, set)
#define clrsetbits_be16(addr, clear, set) clrsetbits(be16, addr, clear, set)

#define clrbits_le16(addr, clear) clrbits(le16, addr, clear)
#define setbits_le16(addr, set) setbits(le16, addr, set)
#define clrsetbits_le16(addr, clear, set) clrsetbits(le16, addr, clear, set)

#define clrbits_8(addr, clear) clrbits(8, addr, clear)
#define setbits_8(addr, set) setbits(8, addr, set)
#define clrsetbits_8(addr, clear, set) clrsetbits(8, addr, clear, set)
/*
 * Given a physical address and a length, return a virtual address
 * that can be used to access the memory range with the caching
 * properties specified by "flags".
 */
#define MAP_NOCACHE	(0)
#define MAP_WRCOMBINE	(0)
#define MAP_WRBACK	(0)
#define MAP_WRTHROUGH	(0)

static inline void *
map_physmem(phys_addr_t paddr, unsigned long len, unsigned long flags)
{
	return (void *)paddr;
}

/*
 * Take down a mapping set up by map_physmem().
 */
static inline void unmap_physmem(void *vaddr, unsigned long flags)
{

}



extern void *__ioremap(unsigned long physaddr, unsigned long size, int cacheflag);
extern void *ioremap(unsigned long physaddr, unsigned long size);
extern void *ioremap_nocache(unsigned long physaddr, unsigned long size);
extern void __iounmap(void *addr, unsigned long size);
extern void iounmap(void *addr);

/*
 * IO bus memory addresses are also 1:1 with the physical address
 */
#define virt_to_bus virt_to_phys
#define bus_to_virt phys_to_virt


/* Written to pacify arch indepeandant code.
 * Not used by ARC I/O
 */

/* #define inb(x) 0 */
/* #define outb(x,y) do { } while(0) */

#define _inb inb
#define _outb outb

#define IO_SPACE_LIMIT	        0xffff

#define IOMAP_FULL_CACHING		  0
#define IOMAP_NOCACHE_SER		    1
#define IOMAP_NOCACHE_NONSER		2
#define IOMAP_WRITETHROUGH		  3

#endif	/* _ASM_ARC_IO_H */

