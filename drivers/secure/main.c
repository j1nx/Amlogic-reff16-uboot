#include <linux/string.h>
#include "aes.h"

//uboot command
#include <common.h>

void show_help(void)
{
	printf("format : aeschk <sub cmd> <in> <out> <len>\n");
	printf("sub cmd:\n");
	printf("\t%s: encode <in> by aes algorithm\n", SUBCMD_AES_ENC);
	printf("\t%s: decode <in> by aes algorithm\n", SUBCMD_AES_DEC);

}

static int do_hisun(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char* szSubCmd;
	int nret = 1;
	if (argc != 5)
	{
		show_help();
		return 0;
	}

	szSubCmd = argv[1];

	nret = do_aes_process(argc,argv);

	if (nret)
	{
		show_help();
		printf("Error: AES enc-dec fail!\n");
	}

	return nret;
}

U_BOOT_CMD(
	aeschk,	5,	1,	do_hisun,
	"aeschk AES algorithm check sub-system",
	"	aeschk aes-enc in out len"
	"	aeschk aes-dec in out len"
	"	note: in/out/len are all HEX mode"
);


