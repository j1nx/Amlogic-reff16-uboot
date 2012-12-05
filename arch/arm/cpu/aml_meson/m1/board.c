
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
