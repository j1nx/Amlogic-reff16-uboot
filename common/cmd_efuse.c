#include <config.h>
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <asm/arch/am_regs.h>
#include <linux/types.h>

#include "efuse_bch_8.h"

int do_efuse(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int ret = 0 ;
	unsigned char addr[384];
	int i;
	char *op;
	char *s;
	char *end ;
	
	if(argc < 2){
		goto usage;
		return -1;
	}
	
	if(strcmp(argv[1],"read")==0){
		if(strcmp(argv[2],"licence")==0){
			op = efuse_read_usr(1);
			printf("licence is ");
			for(i=0;i<3;i++)
				printf(":%02x",op[i]);
			printf("\n");
		}
		else if(strcmp(argv[2],"mac")==0){
			op = efuse_read_usr(2);
			printf("mac is ");
			for(i=0;i<6;i++)
				printf(":%02x",op[i]);
			printf("\n");
		}
		else if(strcmp(argv[2],"hdcp")==0){
			op = efuse_read_usr(3);
			printf("hdcp is ");
			for(i=0;i<300;i++)
				printf(":%02x",op[i]);
			printf("\n");
		}
		else if(strcmp(argv[2],"usid")==0){
			op = efuse_read_usr(4);
			printf("usid is ");
			for(i=0;i<60;i++)
				printf(":%02x",op[i]);
			printf("\n");
		}
		else {
			printf("arg error.");
			return -1;
		}
	}
	else if(strcmp(argv[1],"write")==0){
		if(argc<4){
			printf("arg error");
			return -1;
		}
		
//		char *endp;
//		unsigned long addr;
//		addr = simple_strtoul(argv[3],endp,16);
//		if(*endp != 0){
//			printf("arg error");
//			return -1;
//		}
		
		memset(addr,0,sizeof(addr));
		
		if(strcmp(argv[2],"licence")==0){
//			s = argv[3];
//			for (i = 0; i < 3; i++) {
//				addr[i] = s ? simple_strtoul(s, &end, 16) : 0;
//				if (s)
//					s = (*end) ? end+1 : end;
//			}
//			if(efuse_write_usr(1, addr)){
//				printf("error:efuse had written.");
//				return -1;
//			}
//			else
//				printf("licence written done.");
			printf("licence write unsupport now.");
		}
		else if(strcmp(argv[2],"mac")==0){
			s = argv[3];
			for (i = 0; i < 6; i++) {
				addr[i] = s ? simple_strtoul(s, &end, 16) : 0;
				if (s)
					s = (*end) ? end+1 : end;
			}
			if(efuse_write_usr(2, addr)){
				printf("error:efuse had written.");
				return -1;
			}
			else 
				printf("mac written done.");
		}
		else if(strcmp(argv[2],"hdcp")==0){
			printf("hdcp write unsupport now.");
//			printf("hdcp written done.");
		}
		else if(strcmp(argv[2],"usid")==0){
			printf("usid write unsupport now.");
//			printf("usid written done.");
		}
		else 
			printf("arg error.");
	}
#ifdef CONFIG_EFUSE_DUMP
	else if(strcmp(argv[1], "dump") == 0){
		op = efuse_dump();		
		printf("Raw efuse data: \n");
		for(i=0; i<384; i++){
			if(i%16 == 0)
				printf("%03x:  ", i);
			printf("%02x ", op[i]);
			if(i%16 == 15)
				printf("\n");
		}	
		printf("efuse raw data dump finish \n");
	}
#endif	
	else 
		printf("arg error.");

usage:
	cmd_usage(cmdtp);
		
	return ret ;
		
}

U_BOOT_CMD(
	efuse,	4,	1,	do_efuse,
	"efuse licence/mac/hdcp/usid read/write or dump raw efuse data commands",
	"[read/write] [licence/mac/hdc/usid] [mem_addr]\n"
	"	   [read/wirte] para read ; write ;\n"
	"				read need not mem_addr;write need\n"
	"				read to get efuse context\n"
	"				write to write efuse\n"
	"	   [mem_addr] usr do \n"
	"efuse [dump]\n"
	"	   dump raw efuse data\n"
);

/****************************************************/