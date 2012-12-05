#include <common.h>
#include <command.h>


static unsigned sd_dect_flag = 0;
static int upgrade_sd_dect(void)
{
	struct mmc *mmc = find_mmc_device(0);
	sd_dect_flag = 1;
	unsigned ret = mmc_init(mmc);
	sd_dect_flag = 0;
	return ret;
}

int auto_upgrade_uboot()
{
    int ret = -1;
	if(!(upgrade_sd_dect()))
	{
		ret=run_command ("mmcinfo", 0);
		if(ret>=0)
		{
			ret=run_command ("fatload mmc 0 81000000 u-boot-aml.bin.ro", 0);
			if(ret>=0)
			{
			    ret=run_command ("sf probe 2;sf erase 0 50000;sf write 81000000 0 50000", 0);
			}
	    }
	}    
    return ret;
}


