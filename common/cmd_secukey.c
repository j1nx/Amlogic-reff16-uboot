/*
 * Driver for NAND support, Rick Bronson
 * borrowed heavily from:
 * (c) 1999 Machine Vision Holdings, Inc.
 * (c) 1999, 2000 David Woodhouse <dwmw2@infradead.org>
 *
 * Added 16-bit nand support
 * (C) 2004 Texas Instruments
 */

#include <common.h>
#include <linux/ctype.h>
#include <linux/mtd/mtd.h>
#include <command.h>
#include <watchdog.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <jffs2/jffs2.h>
#include <nand.h>
#include <asm/arch/nand.h>
#include <linux/types.h>
#include <div64.h>
#include <linux/err.h>
int inited=0;
int flag=-1;
extern ssize_t uboot_key_init();
extern ssize_t uboot_get_keylist(char *keyname);
extern ssize_t uboot_key_read(char *keyname, char *keydata);
extern ssize_t uboot_key_write(char *keyname, char *keydata);
extern int nandkey_provider_register();
extern int key_set_version(char *device);
#define debug(fmt,args...) do { printk("[DEBUG]: FILE:%s:%d, FUNC:%s--- "fmt"\n",\
                                                     __FILE__,__LINE__,__func__,## args);} \
                                         while (0)

/* ------------------------------------------------------------------------- */
int do_secukey(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	//
	int i, ret = 0,error,num=0;
	char *cmd;
	char *name;
	char *data;
	char namebuf[20];
	char databuf[4096];
	char listkey[1024];
	memset(namebuf,0,sizeof(namebuf));
	memset(databuf,0,sizeof(databuf));
	memset(listkey,0,sizeof(listkey));
	unsigned long addr;
	/* at least two arguments please */
	if (argc < 2)
		goto usage;
	cmd = argv[1];
	//first read nand key
	if(!inited)
	{
			if (argc!=2)
			{
				printk("wrong command!!\n");
				goto usage;
			} 
			if (!strcmp(cmd,"nand") ) 
			{
				error=uboot_key_init();
				if(error>=0){
					error=nandkey_provider_register();
					if(error>=0){
						 error=key_set_version(cmd);
						 if(error>=0){
							printk("init key ok!!\n");
							inited=1;
							return 0;
						}
					}
				}	
				else
					printk("init error\n");
			}
			else {
				printk("invalid device!!\n");
				return -1;
			}
	}
	else if (inited)
	{
			if(!strcmp(cmd,"list"))
			{
				if (2==argc)
					addr = 0x82000000;
				else if(3==argc)
					addr = (ulong)simple_strtoul(argv[2], NULL, 16);
				else {
					printk("wrong command!!\n");
					goto usage;
				}
				error=uboot_get_keylist(listkey);
				if (error>=0){
					printk("the key name list are:\n%s",listkey);
					for (i=0;i<strlen(listkey);i++){
						if(listkey[i]==0x0a)
							num++;
						}
					flag=num;
					memset((char *)(addr),0,4096);
					*((unsigned int *)addr)=flag;
					flag=-1;
					memcpy((char *)(addr+sizeof(int)),listkey,strlen(listkey)+1);
					return 0;
				}
				else{
					printk("key list error!!check the key  name first!!\n");
					return -1;
				}
			}
			if(!strcmp(cmd,"read"))
			{
				name=argv[2];
				if(4==argc)
					addr = (ulong)simple_strtoul(argv[3], NULL, 16);
				else if(3==argc)
					addr = 0x82000000;
				else{
					printk("wrong command!!\n");
					goto usage; 
				}
				memcpy(namebuf,name,strlen(name));
				error=uboot_key_read(namebuf, databuf);
				if(error>=0){
					char outputdata[2];
					printk("read count=%d\n",error);
					printk("the key name is :%s\n",namebuf);
					printk("the key data is :");
					memset(outputdata,0,2);
					for(i=0;i<error;i++){
						outputdata[0]=databuf[i];
						printk("%c",outputdata[0]);
					}
					printk("\n");
					#if 0
					flag=strlen(databuf)-1;
					*((unsigned int *)addr)=flag;
					#endif
					memset((char *)(addr),0,4096);
					memcpy((char *)(addr),databuf,4096);
					//flag=-1;
					return 0;
				}
				else{
					printk("read error!!\n");
					return -1;
				}
			}
			if(!strcmp(cmd,"write"))
			{
				if (argc!=4)
					goto usage;
				name=argv[2];
				data=argv[3];
				memcpy(namebuf,name,strlen(name));
				memcpy(databuf,data,strlen(data));
				error=uboot_key_write(namebuf, databuf);
				if(error>=0){
					printk("write key ok!!\n");
					return 0;
				}
				else{
					printk("write error!!\n");
					return -1;
				}	
			}
		}
	else
		goto usage ;
usage:
	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(secukey, CONFIG_SYS_MAXARGS, 1, do_secukey,
	"NAND KEY sub-system",
	"list [addr] - show available NAND key name\n"
	"secukey  device - init key in device\n"
	"secukey write keyname data - wirte key data to nand\n"
	"secukey read keyname [addr]- read the key data\n"
);

