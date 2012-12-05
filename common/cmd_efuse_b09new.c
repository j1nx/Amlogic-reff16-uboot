#include <config.h>
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <asm/arch/am_regs.h>
#include <linux/types.h>

#include "efuse_bch_8.h"

int do_efuse_new(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int ret = 0 ;
	unsigned char ch;
	char title[16];
	unsigned char value[128];
	unsigned char addr[384];
	int i, j, k, tag, count;
	char *op;
	char *s;
	char *end ;
	
	if(argc < 2){
		goto usage;
		return -1;
	}
	
	if(strcmp(argv[1],"R")==0){
		if(strcmp(argv[2],"L")==0){
			op = efuse_read_usr(1);
			printf("licence is ");
			for(i=0;i<3;i++)
				printf(":%02x",op[i]);
			printf("\n");
		}
		else if(strcmp(argv[2],"M")==0){
			op = efuse_read_usr(2);
			printf("mac is ");
			for(i=0;i<6;i++)
				printf(":%02x",op[i]);
			printf("\n");
		}
		else if(strcmp(argv[2],"H")==0){
			op = efuse_read_usr(3);
			printf("hdcp is ");
			for(i=0;i<300;i++)
				printf(":%02x",op[i]);
			printf("\n");
		}
		else if(strcmp(argv[2],"U")==0){
			op = efuse_read_usr(4);

			if((op[0]==7)&&(op[1]==0)&&(op[2]==1)&&(op[3]==3)&&(op[4]==0)&&(op[5]==2)){
				printf("usid is ");
				for(i=0;i<20;i++)
					printf(":%x",op[i]);
				printf("\n");
				}
			else{
				op=efuse_read_usr_workaround(4);
				if((op[0]==7)&&(op[1]==0)&&(op[2]==1)&&(op[3]==3)&&(op[4]==0)&&(op[5]==2)){
			printf("usid is ");
			for(i=0;i<20;i++)
				printf(":%x",op[i]);
			printf("\n");
		}
		else {
					printf("read usid error\n");
					printf("error usid is ");
					for(i=0;i<20;i++)
						printf(":%x",op[i]);
					printf("\n");
					return -1;
				}
				}
			
		}
		else {
			printf("arg error.");
			return -1;
		}
	}
	else if(strcmp(argv[1],"W")==0){
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
		else if(strcmp(argv[2],"M")==0){
			int lenth;
			char char_tmp_buf[3];
			
			s = argv[3];
			#if 1
			lenth = strlen(s);
			
			if(lenth != 12)
			{
				printf("Mac lenth is not right curlen=%d\n", lenth);
				printf("mac written failed.\n");
			}
			else
			{
				int error_flag = 0;
				for (i = 0; i <lenth; i++)
				{
					if(s[i]<'0' || s[i]>'f')
					{
						printf("invalid charactor\n");
						error_flag = 1;
						break;
					}
				}
				if(error_flag == 0)
				{
					for (i = 0; i < 6; i++) {
						char_tmp_buf[0] = s[2*i];
						char_tmp_buf[1] = s[2*i+1];
						char_tmp_buf[2] = 0;
						addr[i] = simple_strtoul(char_tmp_buf, &end, 16);
						printf("mac burn%02d %02x\n", i, addr[i]);
					}
					
					if(efuse_write_usr(2, addr)){
						printf("error:efuse had written.");
						return -1;
					}
					else 
						printf("mac written done.");
					
				}
			}
			#else
			for (i = 0; i < 6; i++) {
				addr[i] = s ? simple_strtoul(s, &end, 16) : 0;
				printf("mac burn%02d %d\n", i, addr[i]);
				if (s)
					s = (*end) ? end+1 : end;
			}
			if(efuse_write_usr(2, addr)){
				printf("error:efuse had written.");
				return -1;
			}
			else 
				printf("mac written done.");
			#endif
			
		}
		else if(strcmp(argv[2],"hdcp")==0){
			printf("hdcp write unsupport now.");
//			printf("hdcp written done.");
		}
		else if(strcmp(argv[2],"U")==0){
			int lenth;
			char tmp_buf[2];
			
			s = argv[3];
			lenth = strlen(s);
			
			if(lenth>20 || lenth<=0)
			{
				printf("usid lenth is not right curlen=%d\n", lenth);
				printf("usid written failed.\n");
			}
			else
			{
				int error_flag = 0;
				for (i = 0; i <lenth; i++)
				{
					if(s[i]<'0' || s[i]>'f')
					{
						printf("invalid charactor\n");
						error_flag = 1;
						break;
					}
				}
				if (error_flag == 0)
				{
					for (i = 0; i < 20; i++) {
						tmp_buf[0] = s[i];
						tmp_buf[1] = 0;
						addr[i] = simple_strtoul(tmp_buf, &end, 16);
						printf("usid burn%02d %x\n", i, addr[i]);
					}
					if(efuse_write_usr(4, addr)){
						printf("error:efuse had written.");
						return -1;
					}
					else {
						printf("usid written done.");
					}
				}
			}
		}
		else if(strcmp(argv[2],"custom")==0){
			s = argv[3];
			printf("string is %s\n", s);
			if(s) {				
				i = 0;
				j = 0;
				tag = 0;
				count = 0;
				ch = s[i];
				while(ch != 0) {
					if((ch != ':') && (tag == 0)) {
						title[j] = ch;
						i++;
						j++;
						ch = s[i];
					}
					else if(ch == ':') {
						printf("title is %s\n", title);
						tag = 1;
						i++;
						j = 0;
						ch = s[i];
					}
					else if(ch == ';'){
						if(strcmp(title,"sn")==0){
							if(efuse_write_usr(4, value)){
								printf("error:efuse had written.");
								puts("ERROR -");
								return -1;
							}
							else {
								printf("usid written done.");
							}
							printf("sn:%s\n", value);
						}
						else if(strcmp(title,"mac")==0){
							printf("value:%s\n", value);
							printf("mac:");
							op = value;
							for (k = 0; k < 6; k++) {
								addr[k] = op ? simple_strtoul(op, &end, 16) : 0;
								printf("%02x", addr[k]);
								if (op)
									op = (*end) ? end+1 : end;
							}
							printf("\n");
							if(efuse_write_usr(2, addr)){
								printf("error:efuse had written.");
								puts("ERROR -");
								return -1;
							}
							else {
								printf("mac written done.");
							}
						}
						memset(title,0,sizeof(title));
						memset(value,0,sizeof(value));
						tag = 0;
						i++;
						j = 0;
						ch = s[i];
					}
					else {
						if(strcmp(title,"sn")==0){
							value[j] = ch;
							i++;
							j++;
							ch = s[i];
						}
						else if(strcmp(title,"mac")==0){
							value[j] = ch;
							i++;
							j++;
							ch = s[i];
							count++;
							if(count >= 2) {
								count = 0;
								value[j] = ':';
								j++;
							}
						}
					}
				}
				puts("PASS");
			}
			else {
				puts("ERROR -");
			}
		}
		else 
			printf("arg error.");
	}
	else 
		printf("arg error.");

usage:
	cmd_usage(cmdtp);
		
	return ret ;
		
}

U_BOOT_CMD(
	EFUSE,	4,	1,	do_efuse_new,
	"EFUSE licence/mac/hdcp/usid read/write commands",
	"EFUSE [R/W] [L-licence/M-mac/H-hdcp/U-usid] [mem_addr]\n"
	"	   [read/wirte] para read ; write ;\n"
	"				read need not mem_addr;write need\n"
	"				read to get efuse context\n"
	"				write to write efuse\n"
	"	   [mem_addr] usr do \n"
);

/****************************************************/