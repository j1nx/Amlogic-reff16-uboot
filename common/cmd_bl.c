/*
 * Common LCD routines for supported CPUs
 *
 * (C) Copyright 2001-2002
 * Wolfgang Denk, DENX Software Engineering -- wd@denx.de
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Author:  Jinlin Xia <jinlin.xia@amlogic.com>
 *
 */

/************************************************************************/
/* ** HEADER FILES							*/
/************************************************************************/

//#define DEBUG

#include <config.h>
#include <common.h>
#include <command.h>
#include <stdarg.h>
#include <linux/types.h>
#include <stdio_dev.h>
#include <bl_aml.h>

extern blinfo_t bl_info;

static int do_bl_on(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	bl_info.bl_on();
	return 0;
}

static int do_bl_off(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	bl_info.bl_off();
	return 0;
}

static int do_pl_on(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
    bl_info.panel_on();
    return 0;
}

static int do_pl_off(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
    bl_info.panel_off();
    return 0;
}

static int do_bl_set(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	int level;
	if(argc < 2){
		cmd_usage(cmdtp);
		return 1;
	}
	else{
		level = simple_strtoul(argv[1], NULL, 10);
		if(level > bl_info.max_bl_level){
			cmd_usage(cmdtp);
			return 1;
		}
		bl_info.set_bl_level(level);
		return 0;
	}
}

static cmd_tbl_t cmd_bl_sub[] = {
	U_BOOT_CMD_MKENT(on , 2, 0, do_bl_on, "", ""),
	U_BOOT_CMD_MKENT(off, 2, 0, do_bl_off, "", ""),
	U_BOOT_CMD_MKENT(set, 3, 0, do_bl_set, "", ""),
	U_BOOT_CMD_MKENT(panelon, 3, 0, do_pl_on, "", ""),
	U_BOOT_CMD_MKENT(paneloff, 3, 0, do_pl_off, "", ""),
};

/*
 * Subroutine:  do_backlight
 *
 * Description: Handler for 'backlight' command..
 *
 * Inputs:	argv[1] contains the subcommand
 *
 * Return:      None
 *
 */
static int do_backlight(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	cmd_tbl_t *c;

	/* Strip off leading 'bmp' command argument */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_bl_sub[0], ARRAY_SIZE(cmd_bl_sub));

	if (c) {
		return  c->cmd(cmdtp, flag, argc, argv);
	} else {
		cmd_usage(cmdtp);
		return 1;
	}
}

U_BOOT_CMD(
	bl,	5,	1,	do_backlight,
	"manipulate backlight function",
	"on		- power on backlight\n"
	"bl off		- power off backlight\n"
	"bl panelon		-panel power on\n"
	"bl paneloff		-panel power off\n"
	"bl set <backlight level>	- set backlight\n"
);

/************************************************************************/
/************************************************************************/

