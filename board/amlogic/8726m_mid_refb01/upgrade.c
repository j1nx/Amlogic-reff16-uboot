#include <linux/types.h>
#include <upgrade.h>
#include <common.h>
#include <command.h>
#include <nand.h>
#include <asm/arch/nand.h>
#include <linux/err.h>

//#define NAND_CLEAN

int upgrade_bootloader(void)
{
    int	i = 0, j = 0, retry = 2;
    char	str[128];
    unsigned long   size, size1;
    char    *filepath;
    
    printf("u-boot upgrading...\n");
    if(run_command ("mmcinfo", 0))
    {
        UPGRADE_DPRINT("##	ERROR: SD card not find!!!\n");
    }
    else
    {
        UPGRADE_DPRINT("Find SD card!!!\n");

        for(i = 0; i < SCAN_MMC_PARTITION; i++)
        {
            sprintf(str, "fatexist mmc 0:%d ${bootloader_path}", (i + 1));
            UPGRADE_DPRINT("command:    %s\n", str);
            if(!run_command (str, 0))
            {
                size = simple_strtoul (getenv ("bootloader_filesize"), NULL, 16);
                size1 = simple_strtoul (getenv ("filesize"), NULL, 16);
                //if(size != size1)
                {
                    UPGRADE_DPRINT("bootloader_filesize:%d   !=  filesize:%d\n", size, size1);
                    while(retry-- > 0)
                    {
                        sprintf(str, "fatload mmc 0:%d ${loadaddr} ${bootloader_path}", (i + 1));
                        UPGRADE_DPRINT("command:    %s\n", str);
                        run_command (str, 0);

                        run_command ("nand rom_protect off", 0);
#ifdef  NAND_CLEAN
                        int partition_num = simple_strtoul(getenv ("partnum"), NULL, 16);;
                        for(j = 0; j < partition_num; j++)
                        {
                            sprintf(str, "p%dpath", j);
                            filepath = getenv (str);
                            sprintf(str, "fatexist mmc 0:%d %s", (i + 1), filepath);
                            UPGRADE_DPRINT("command:    %s\n", str);
                            if(run_command (str, 0))
                            {
                                break;
                            }
                        }
                        if(j >= (partition_num - 1))
                        {
                            run_command ("nand scrub", 0);	
                        }
#endif				
                        run_command ("nand rom_write ${loadaddr} ${bootloader_start} ${bootloader_size}", 0);
                        run_command ("nand rom_protect on", 0);

                        size = simple_strtoul (getenv ("filesize"), NULL, 16);

                        sprintf(str, "set bootloader_filesize 0x%x", size);
                        run_command (str, 0);

                        UPGRADE_DPRINT("bootloader upgrade successful!\n");
                        run_command ("save", 0);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

static int upgrade_erase_nftl_part()
{
	char	str[128];
	nand_erase_options_t opts;
	int error, nand_scrub_flag = 0, i, blk_num;
	size_t pagesize, blocksize, len;
	loff_t nftl_erase_addr;
	struct mtd_info *mtd = nand_info[nand_curr_device];
	if (mtd == NULL) {
		printk("mtd name err: %s\n", mtd->name);
		return -1;
	}

	pagesize = mtd->writesize;
	blocksize = mtd->erasesize;
	len = pagesize;
	u_char temp_buf[pagesize];
	nftl_erase_addr = 0;
	blk_num = mtd->size / mtd->erasesize;
	printk("nftl_erase_addr %llx blks: %d\n", nftl_erase_addr, blk_num);

	for (i=0; i<blk_num; i++) {
		memset(temp_buf, 0xff, 16);
		error = nand_read(mtd, nftl_erase_addr + i*blocksize, &len, temp_buf);
		if ((error) && (error != -EUCLEAN)) {
			printf("read data from nand error: %d\n",error);
			//continue;
		}
		if(!strncmp(temp_buf, "amlnftl", 7) || !strncmp((temp_buf + pagesize / 2), "amlnftl", 7)) {
			nand_scrub_flag = 1;
			break;
		}
	}

	printk("nand_scrub_flag: %d\n", nand_scrub_flag);
	if (nand_scrub_flag) {
		i = 0;

		memset(&opts, 0, sizeof(opts));
		opts.offset = 0;
		opts.length = mtd->size;
		opts.quiet = 1;
		opts.scrub = 1;
		nand_erase_opts(mtd, &opts);

		sprintf(str, "fatload mmc 0:%d ${loadaddr} ${bootloader_path}", (i + 1));
		UPGRADE_DPRINT("command:    %s\n", str);
		run_command (str, 0);

		run_command ("nand rom_protect off", 0);
		run_command ("nand rom_write ${loadaddr} ${bootloader_start} ${bootloader_size}", 0);
		run_command ("nand rom_protect on", 0);
	}
	return 0;
}

int upgrade_env(void)
{
    int	i = 0, partition_num = 0;
    char	str[128];
    char    *filepath;
    
    printf("environment upgrading...\n");
    if(run_command ("mmcinfo", 0))
    {
        UPGRADE_DPRINT("##	ERROR: SD card not find!!!\n");
    }
    else
    {
        UPGRADE_DPRINT("Find SD card!!!\n");
        
		//if (upgrade_erase_nftl_part())
			//UPGRADE_DPRINT("NFTL erase failed!!!\n"); 

        for(i = 0; i < SCAN_MMC_PARTITION; i++)
        {
            sprintf(str, "fatexist mmc 0:%d ${env_path}", (i + 1));
            UPGRADE_DPRINT("command:    %s\n", str);
            if(!run_command (str, 0))
            {
                sprintf(str, "fatload mmc 0:%d ${loadaddr} ${env_path}", (i + 1));
                run_command (str, 0);
                run_command ("loadenv ${loadaddr}", 0);
                run_command ("save", 0);
                return  1;
            }
        }
    }
    return  0;
}

#ifndef	CONFIG_UBI_SUPPORT
int upgrade_partition(void)
{
    int	i = 0, j = 0, ret = 0, partition_num = 0;
    char    upgrade_status_list[16];
    char	str[128];
    unsigned long   size, size1;
    char    *filepath;
    
    printf("partition upgrading...\n");
    if(run_command ("mmcinfo", 0))
    {
        UPGRADE_DPRINT("##	ERROR: SD card not find!!!\n");
    }
    else
    {
        UPGRADE_DPRINT("Find SD card!!!\n");
        memset(upgrade_status_list, 0, 16);
        partition_num = simple_strtoul(getenv ("partnum"), NULL, 16);

        for(i = 0; i < SCAN_MMC_PARTITION; i++)
        {
            for(j = 0; j < partition_num; j++)
            {
                if(!upgrade_status_list[j])
                {
                    sprintf(str, "p%dpath", j);
                    filepath = getenv (str);
                    sprintf(str, "fatexist mmc 0:%d %s", (i + 1), filepath);
                    UPGRADE_DPRINT("command:    %s\n", str);
                    if(!run_command (str, 0))
                    {
                        sprintf(str, "p%dfilesize", j);
                        size = simple_strtoul (getenv (str), NULL, 16);
                        size1 = simple_strtoul (getenv ("filesize"), NULL, 16);
                        //if(size != size1)
                        {
                            sprintf(str, "fatload mmc 0:%d ${loadaddr} ${p%dpath}", (i + 1), j);
                            UPGRADE_DPRINT("command:    %s\n", str);
                            run_command (str, 0);

                            sprintf(str, "nand erase ${p%dstart} ${p%dsize}", j, j);
                            run_command(str, 0);

                            sprintf(str, "nand write ${loadaddr} ${p%dstart} ${p%dsize}", j, j);
                            run_command (str, 0);
                            
                            sprintf(str, "set p%dfilesize ${filesize}", j);
                            run_command (str, 0);
                            upgrade_status_list[j] = 1;
                        }
                    }
                }
            }
        }
    }
    for(j = 0; j < partition_num; j++)
    {
        if(upgrade_status_list[j])
        {
            UPGRADE_DPRINT("p% upgrade successful!\n", j);
            ret = 1;
        }
    }
    if(ret)
    {
        run_command ("save", 0);
    }
    return ret;
}
#endif

void into_recovery(void)
{
    int	i = 0, j = 0, ret = 0;
    char	str[128];
    
    printf("Recovery Start...\n");
    run_command ("nand read ${logo_name} 84100000 0 ${logo_size}", 0);
    if(!run_command ("mmcinfo", 0))
    {
        for(i = 0; i < SCAN_MMC_PARTITION; i++)
        {
            sprintf(str, "fatexist mmc 0:%d ${recovery_path}", (i + 1));
            if(!run_command (str, 0))
            {
                UPGRADE_DPRINT("recovery in SD Card!!!\n");
                sprintf(str, "fatload mmc 0:%d ${loadaddr} ${recovery_path}", (i + 1));
                run_command (str, 0);
                run_command ("bootm", 0);
            }
        }
    }

recovery_in_nand:
    UPGRADE_DPRINT("recovery in nand!!!\n");
    run_command ("nand read ${recovery_name} ${loadaddr} 0 ${recovery_size}", 0);

    run_command ("bootm", 0);
}
