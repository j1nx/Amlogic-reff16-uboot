#include <config.h>
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <asm/arch/am_regs.h>
#include <linux/types.h>
#include <efuse_m3.h>

#include "efuse_bch_8.h"

#define EFUSE_WRITE 0
#define EFUSE_READ 1
#define EFUSE_DUMP 2

int do_efuse(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret = 0 ;
	int i;
	char addr[EFUSE_BYTES];
	char *title;
	char *op;	
	char *s;
	char *end;
	efuseinfo_item_t info;
	int action = -1;	
		
	if(argc < 2){
		cmd_usage(cmdtp);
		return -1;
	}
	
	if(strncmp(argv[1], "read", 4) == 0)
		action=EFUSE_READ;
	else if(strncmp(argv[1], "write", 5) == 0)
		action=EFUSE_WRITE;
	else if(strcmp(argv[1], "dump") == 0)
		action=EFUSE_DUMP;
	/*else if(strcmp(argv[1], "version") == 0)
		action = EFUSE_VERSION;*/
	else{
		printf("%s arg error\n", argv[1]);
		return -1;
	}
				
	// efuse dump
	if(action == EFUSE_DUMP){
		op = efuse_dump();		
		printf("Raw efuse data: \n");
		for(i=0; i<EFUSE_BYTES; i++){
			if(i%16 == 0)
				printf("%03x:  ", i);
			printf("%02x ", op[i]);
			if(i%16 == 15)
				printf("\n");
		}	
		printf("efuse raw data dump finish \n");
	}
	
	// efuse version
	/*else if(action == EFUSE_VERSION){
		if(argc<3){
				printf("arg count error\n");
				return -1;
		}
		efuse_getinfo_version(&info);
		memset(addr, 0, sizeof(addr));	
		s=argv[2];
		for(i=0; i<info.data_len; i++){
			addr[i] = s ? simple_strtoul(s, &end, 16) : 0;
			if (s)
				s = (*end) ? end+1 : end;
		}
		for(i=1; i<info.data_len; i++){			
			if(addr[i] != 0)
				break;
		}		
		if(i==info.data_len){
			printf("err: efuse need select version and machid at the same time.\n");
			return -1;
		}
			
		if(efuse_write_usr(&info, addr)){
			printf("error: efuse version has been selected.\n");
			return -1;
		}
		else
			printf("efuse version select done.\n");		
	}*/
		

	// efuse read
	else if(action == EFUSE_READ){
		title = argv[2];
		if(efuse_getinfo(title, &info) < 0)		
			return -1;
		
		memset(addr, 0, EFUSE_BYTES);
		efuse_read_usr(addr, info.data_len, (loff_t *)&info.offset);		
		printf("%s is: ", title);
		for(i=0; i<(info.data_len); i++)
			printf(":%02x", addr[i]);
		printf("\n");
	}
	
	// efuse write
	else if(action==EFUSE_WRITE){		
		if(argc<4){
			printf("arg count error\n");
			return -1;
		}
		title = argv[2];
		if(efuse_getinfo(title, &info) < 0)
			return -1;		
		if(!(info.we)){
			printf("%s write unsupport now. \n", title);
			return -1;
		}
		
		memset(addr, 0, sizeof(addr));	
		s=argv[3];
		for(i=0; i<info.data_len; i++){
			addr[i] = s ? simple_strtoul(s, &end, 16) : 0;
			if (s)
				s = (*end) ? end+1 : end;
		}
		
		if(*s){
			printf("error: The wriiten data length is too large.\n");
			return -1;
		}
		
		if(efuse_write_usr(addr, info.data_len, (loff_t*)&info.offset)<0){
			printf("error: efuse write fail.\n");
			return -1;
		}
		else
			printf("%s written done.\n", info.title);					
	}

	else{
		printf("arg error\n");
		return -1;	
	}
		
	return ret ;		
}

U_BOOT_CMD(
	efuse,	4,	1,	do_efuse,
	"efuse version/licence/mac/hdcp/usid read/write or dump raw efuse data commands",
	"[read/write] [version/licence/mac/hdc/usid] [mem_addr]\n"
	"	   [read/wirte] para read ; write ;\n"
	"				read need not mem_addr;write need\n"
	"				read to get efuse context\n"
	"				write to write efuse\n"
	"	   [mem_addr] usr do \n"
	"efuse [dump]\n"
	"	   dump raw efuse data\n"	
);

/****************************************************/