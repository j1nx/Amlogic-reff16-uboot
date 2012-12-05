#include <config.h>
#include <common.h>
#include <command.h>

int do_runscript(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong addr = simple_strtoul(argv[1], NULL, 16);
	char* cmd = (char*) addr;
	int ret = 0;
	
	char *line = cmd;
	char *next = cmd;

	while (*next) {
		if (*(next +1) == 0xa) {
			*(next++) = '\0';
		}
		if (*next == '\n') {
			*next = '\0';
			/* run only non-empty commands */
			if (*line) {
				if (!strncmp(line, "#end", 4)) {
					//printf("command end\n");
					break;
				} else if (strncmp(line, "#start", 6)){	
					//printf("** exec: %s ; command len is %s\n", line, strlen(line));
					if (run_command (line, 0) < 0) {
						ret = 1;
						break;
					}
				}
			}
			line = next + 1;
		}
		++next;
	}

	return ret;
}

U_BOOT_CMD(
	runscript,	2,	1,	do_runscript,
	"runscript addr",
);