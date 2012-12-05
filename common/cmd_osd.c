/*
 * Command for control osd.
 *
 * Copyright (C) 2011 Amlogic.
 */
#include <common.h>

/********************************************************************************************
*
*										OSD
*
*********************************************************************************************/
#include <asm/arch/osd.h>

extern int osd_width;
extern int osd_height;

static int do_osd_open(int argc, char *argv[])
{
	unsigned int addr, type;
	unsigned int w, h,  osd;
	char *endp;

	if (argc < 5)
		goto usage;

	type = simple_strtoul(argv[1], &endp, 0);
	if (*argv[1] == 0 || *endp != 0)
		goto usage;

	w = simple_strtoul(argv[2], &endp, 0);
	if (*argv[2] == 0 || *endp != 0)
		goto usage;
	h = simple_strtoul(argv[3], &endp, 0);
	if (*argv[3] == 0 || *endp != 0)
		goto usage;
	osd = simple_strtoul(argv[4], &endp, 0);
	if (*argv[4] == 0 || *endp != 0)
		goto usage;

	printf("osd : %u (%d,%d) osd%d\n", type, w,h,osd);

	osd_init_hw();
	
	osd_setup(0,
                    0,
                    w,
                    h,
                    w,
                    h*2 ,
                    0,
                    0,
                   w - 1,
                    h - 1,
                    FB_ADDR,
                    &default_color_format_array[type],
                    osd);

	return 0;

usage:
	puts("Usage: osd open <type> <w h> osd \n");
	return 1;
}

static int do_osd_close(int argc, char *argv[])
{
	return 0;
}

static int do_osd_enable(int argc, char *argv[], int enable)
{
	//enable_dsp();
	return 0;
}

static int do_fill_osd(int argc, char *argv[])
{
	int i,j;
	char *out = FB_ADDR;
	int color=0;
	char *endp;
	int x=0,y=0,w=osd_width,h=osd_height;
	if(argc<2)
		return -1;
	color = simple_strtoul(argv[1], 0, 16);
	if(argc>2)
		x=simple_strtoul(argv[2], 0, 0);
	if(argc>3)
		y=simple_strtoul(argv[3], 0, 0);	
	if(argc>4)
		w=simple_strtoul(argv[4], 0, 0);	
	if(argc>5)
		h=simple_strtoul(argv[5], 0, 0);	
	printf("fill osd  with color r(%d)g(%d)b(%d)\n",(color>>16)&0xff,(color>>8)&0xff,color &0xff);
	out+=(y*osd_width+x)*3;
	for (i=0;i<h ;i++)
	{
		for(j=0;j<w;j++)
		{
			*(out+j*3)=color &0xff;
			*(out+j*3+1)=(color>>8)&0xff;
			*(out+j*3+2)=(color>>16)&0xff;
		}
		out+=osd_width*3;
	}
	flush_cache(FB_ADDR, osd_width*osd_height*3);
	return 0;

	
}


static int do_osd(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	const char *cmd;

	cmd = argv[1];

	if (strcmp(cmd, "open") == 0)
		return do_osd_open(argc - 1, argv + 1);
	if (strcmp(cmd, "close") == 0)
		return do_osd_close(argc - 1, argv + 1);
	if (strcmp(cmd, "enable") == 0)
		return do_osd_enable(argc - 1, argv + 1, 1);
	if (strcmp(cmd, "disable") == 0)
		return do_osd_enable(argc - 1, argv + 1, 0);
	if (strcmp(cmd, "fill") == 0)
		return do_fill_osd(argc - 1, argv + 1);

	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(
	osd,	8,	0,	do_osd,
	"osd sub-system",
	"open <type> <w h> osd\n"
	"			- open an osd layer\n"
	"osd close	- close the osd layer\n"
	"osd enable	- enable the osd layer\n"
	"osd disable	- disable the osd layer\n"
	"osd fill color	- fill osd background\n");


