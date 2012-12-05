/*
 * Copyright Codito Technologies (www.codito.com) 
 *
 * cpu/arc/cpu.c
 *
 * Copyright (C) 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 * 
 * Authors :Sandeep Patil (sandeep.patil@codito.com)
 * 			Pradeep Sawlani (pradeep.sawlani@codito.com)
 */

#include <command.h>
#include <common.h>
#include <asm/arcregs.h>
#include <asm/arch/am_regs.h>

int do_reset (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	
#ifdef AML_BOOT_SPI  	
	/* if uboot is on flash then boot agian from flash */
	writel(WATCHDOG_ENABLE_MASK, IREG_WATCHDOG_CONTROL0);
#endif	
	WRITE_CBUS_REG(WATCHDOG_RESET, 0);
    WRITE_CBUS_REG(WATCHDOG_TC, 1 << 22 | 100); /*100*10uS=1ms*/
    WRITE_CBUS_REG(WATCHDOG_RESET, 0);
    udelay(10000);/**/
    printf("Chip watchdog reset error!!!please reset it by hardware\n");
    while (1) {
        ;
    }
	return 0;
}
#if 0
void icache_disable(void)
{
	__asm__ __volatile (	"lr  r0,[0x11] \n\t"
				"or  r0,r0,1 \n\t"
				"sr  r0,[0x11] \n\t"
				::
				:"r0");	

}

void icache_enable(void)
{
	__asm__ __volatile__ (
		"lr   r0,[0x11] \n\t"
		"and  r0,r0,~0x01 \n\t"
		"sr   r0,[0x11] \n\t"
		::
		:"r0");
}

int icache_status()
{
	int status = 0;
	__asm__ __volatile__ (
		"lr r0, [0x11] \n\t"
		"and r0,r0,0x01 \n\t"
		"mov %0, r0 \n\t"
		:"=r"(status)::"r0");
	if(status)
		return (0);  //disable
	else
		return (1);  //enable
}

void dcache_disable(void)
{
	/* Flush D cache */
	__asm__ __volatile (	
			"sr  1, [0x47] \n\t"
			::
			);
	/* Disable D cache */
	__asm__ __volatile (	
			"lr  r0,[0x48] \n\t"
			"or  r0,r0,1 \n\t"
			"sr  r0,[0x48] \n\t"
			::
			:"r0");	
}

/* 
 * Perform blocking invalidate, making sure that the flush
 * completes before we continue 
 */
void _invalidate_dcache(void)
{
	/* nop instruction is needed to finish ARC instruction pipeline for previous data fetching */
    __asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
    write_new_aux_reg(ARC_REG_DC_IVDC, ARC_REG_DC_IVDC);
    __asm__ __volatile (" dcache_wait:");
    __asm__ __volatile (" lr %r0, [0x48]");
    __asm__ __volatile (" and.f %r0, %r0, 0x100");
    __asm__ __volatile (" nop");
    __asm__ __volatile (" bnz dcache_wait");
}

void _invalidate_icache(void)
{
	__asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
	__asm__ __volatile (
		"sr 0x10, [0x10] \n\t"
		"lr r0, [0x11] \n\t"
		::
		:"r0"
		);
	__asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
}

void dcache_enable()
{
	__asm__ __volatile (	
			"lr  r0,[0x48] \n\t"
			"and  r0,r0,~0x01 \n\t"
			"sr  r0,[0x48] \n\t"
			::
			:"r0");	
}

int dcache_status()
{
	int status = 0;
	__asm__ __volatile__ (
		"lr r0, [0x48] \n\t"
		"and r0,r0,0x01 \n\t"
		"mov %0, r0 \n\t"
		:"=r"(status)::"r0");
	if(status)
		return (0);
	else
		return (1);
}

void flush_cache (unsigned long dummy1, unsigned long dummy2)
{
	/* flushing data cache */
	write_new_aux_reg(ARC_REG_DC_FLSH, 1);

	/* poll for completion */
	while (read_new_aux_reg(ARC_REG_DC_CTRL) & (1<<8));

	/* Invalidate instruction cache */
	write_new_aux_reg(ARC_REG_IC_IVIC, 1);
	    
	/* read will only comple when invalidation complete */
	read_new_aux_reg(ARC_REG_IC_CTRL);
}	

void _dcache_flush_range(unsigned long startAddr, unsigned long endAddr)
{
    /* DECLARE_CPU_SR; */
    unsigned long start = startAddr;
    unsigned long end = endAddr;

    start &= CACHE_LINE_MASK;

    /* OS_ENTER_CRITICAL(); */

    while (end >= start) {
        write_new_aux_reg(ARC_REG_DC_FLDL, start);
        start = start + CACHE_LINE_LENGTH;
    }
    while (read_new_aux_reg(ARC_REG_DC_CTRL) & ARC_REG_DC_CTRL_FS) ;
    /* OS_EXIT_CRITICAL(); */

    return;
}

void _dcache_inv_range(unsigned long startAddr, unsigned long endAddr)
{
    /* DECLARE_CPU_SR; */
    unsigned long start = startAddr;
    unsigned long end = endAddr;
	unsigned long dc_ctrl;
    start &= CACHE_LINE_MASK;

    /* OS_ENTER_CRITICAL(); */
	dc_ctrl = read_new_aux_reg(ARC_REG_DC_CTRL);
	write_new_aux_reg(ARC_REG_DC_CTRL, dc_ctrl & ~IM_SET_DC_CTRL);
	
    while (end >= start) {
        write_new_aux_reg(ARC_REG_DC_IVDL, start);
        start = start + CACHE_LINE_LENGTH;
    }
    
    /* Accessing the DC_CTRL register to read the SB flag could result
     * in a processor stall if the operation initiated had not completed,
     * however, the result read back after initiating any of the listed
     * functions will reflect the success of that operation.
     */
    write_new_aux_reg(ARC_REG_DC_CTRL, dc_ctrl | IM_SET_DC_CTRL);

    /* OS_EXIT_CRITICAL(); */

    return;
}
#endif
/*
 * disable dcache and icahce
 */
int cpu_init(void)
{
	disable_interrupts();
	return 0;
}

void AV_AHB_bus_invalidate()
{
#ifdef CONFIG_AML_APOLLO
    SET_ISA_REG_MASK(AHB_BRIDGE_CTRL1,(1 << 31));
#endif
}

