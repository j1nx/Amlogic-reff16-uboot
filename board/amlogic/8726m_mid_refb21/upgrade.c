#include <linux/types.h>
#include <upgrade.h>
#include <common.h>
#include <command.h>

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
            sprintf(str, "fatexist mmc 0:%d %s", (i + 1), UBOOTPATH);
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
                        sprintf(str, "fatload mmc 0:%d ${loadaddr} %s", (i + 1), UBOOTPATH);
                        UPGRADE_DPRINT("command:    %s\n", str);
                        run_command (str, 0);

                        run_command ("nand rom_protect off", 0);
#ifdef  NAND_CLEAN
                        run_command ("nand scrub_detect", 0);	
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
