#include <common.h>
#include <environment.h>
#include <mmc.h>

DECLARE_GLOBAL_DATA_PTR;

/* references to names in env_common.c */
extern uchar default_environment[];

char * env_name_spec = "eMMC";
env_t *env_ptr;

static struct spi_flash *env_flash;

uchar env_get_char_spec(int index)
{
	return *((uchar *)(gd->env_addr + index));
}

int saveenv(void)
{
    struct mmc *mmc;
    int dev_num;
	u64 cnt = CONFIG_ENV_SIZE;
	u64 blk = CONFIG_ENV_OFFSET;
    dev_num = CONFIG_ENV_DEVICE_ID;
    mmc = find_mmc_device(dev_num);
	if (!mmc)
	{
		puts("No eMMC device! Environment eMMC not initialized!\n");
		return -1;
	}

	if ((cnt % 512) || (blk % 512)) {
	    printf("addr notalign 0x%llx or byte notalign 0x%llx",blk,cnt);
		return -1;
	}
	mmc_init(mmc);
    blk >>= 9;
    cnt >>= 9;
	if(cnt == mmc->block_dev.block_write(dev_num, blk, cnt, (const void *)env_ptr))
    {
        printf("emmc save env ok\n");
    }
    else
    {
        printf("emmc save env fail\n");
		return -1;
    }
	return 0;
}


void env_relocate_spec(void)
{
    struct mmc *mmc;
    int dev_num;
	u64 cnt = CONFIG_ENV_SIZE;
	u64 blk = CONFIG_ENV_OFFSET;

    dev_num = CONFIG_ENV_DEVICE_ID;
    mmc = find_mmc_device(dev_num);
	if (!mmc)
	{
		printf("No eMMC device!\n");
		return;
	}
	if ((cnt % 512) || (blk % 512)) {
	    printf("addr notalign 0x%llx or byte notalign 0x%llx",blk,cnt);
	    return ;
	}
	mmc_init(mmc);
    blk >>= 9;
    cnt >>= 9;
	if(cnt == mmc->block_dev.block_read(dev_num, blk, cnt, (const void *)env_ptr))
	{
	    printf("emmc read env ok\n");
	}
	else
    {
        printf("emmc read env fail\n");
        return ;
    }
    if (crc32(0, env_ptr->data, ENV_SIZE) != env_ptr->crc)
    {
        goto crc_err;
    }
    
    return ;
crc_err:
	puts("*** Warning - bad CRC, using default environment\n\n");
    set_default_env();
}

int env_init(void)
{
	/* eMMC isn't usable before relocation */
	gd->env_addr = (ulong)&default_environment[0];
	gd->env_valid = 1;

	return 0;
}

