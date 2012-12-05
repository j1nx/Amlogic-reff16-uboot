
#include <common.h>
#include <asm/io.h>
#include <asm/cache.h>
#include <asm/arch/am_reg_addr.h>
/******************************************************************************
 * Routine: dram_init
 * Description: sets uboots idea of sdram size
 *****************************************************************************/
int dram_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;
	 /*
    @todo implement this function
    */
    writel(0,P_WATCHDOG_TC);

	gd->bd->bi_dram[0].start = PHYS_MEMORY_START;
	gd->bd->bi_dram[0].size = PHYS_MEMORY_SIZE;

	return 0;
}

/******************************************************************************
 * Routine: cold_copy2sram
 * Description: copy cold code to sram and blanch to it
 *****************************************************************************/
#ifdef CONFIG_COLD_HEART
#include <asm/cache.h>
#include <asm/arch/romboot.h>

int cold_copy2sram(unsigned long *dest, unsigned long *src, unsigned long code_size)
{
	unsigned long *dest_add = dest;
	unsigned long *src_add = src;
	void (*sram_fun)();
	//sram_fun=dest_add+0x200;
	sram_fun=dest_add;

	puts("Enter cold_copy2sram...\n");
	/*detect if need enter cold code*/
	if(0xc01dc0de != (*(volatile unsigned int*)(0xc8100000)))
		return -1;	

	if(code_size > READ_SIZE)
		return -2;

	memcpy(dest_add,src_add,code_size);
	
	/*flush and off cache*/
	dcache_flush();
	icache_invalid();
	dcache_disable();
	icache_disable();

//	return 0;
	
	printf("0x%x	0x%x\n",*(volatile unsigned int*)(dest),*(volatile unsigned int*)(src));
	sram_fun();
}
#endif//#ifdef CONFIG_AML_COLDCODE