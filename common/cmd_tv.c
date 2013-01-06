/*
 * Command for control tv out.
 *
 * Copyright (C) 2011 Amlogic.
 */
#include <common.h>
#include <asm/arch/am_reg_addr.h>
#include <asm/arch/clock.h>


/********************************************************************************************
*
*										TVOUT
*
*********************************************************************************************/

#include <asm/arch-m1/tv_out.h>

static int to_mode(char *mode_name)
{
	if((strcmp(mode_name, "1080P")==0) || (strcmp(mode_name, "1080p")==0))
		return TVOUT_1080P;
	if((strcmp(mode_name, "1080I")==0) || (strcmp(mode_name, "1080i")==0))
		return TVOUT_1080I;
	if((strcmp(mode_name, "720P")==0) || (strcmp(mode_name, "720p")==0))
		return TVOUT_720P;
	if((strcmp(mode_name, "576P")==0) || (strcmp(mode_name, "576p")==0))
		return TVOUT_576P;
	if((strcmp(mode_name, "480P")==0) || (strcmp(mode_name, "480p")==0))
		return TVOUT_480P;
	if((strcmp(mode_name, "576I")==0) || (strcmp(mode_name, "576i")==0))
		return TVOUT_576I;
	if((strcmp(mode_name, "480I")==0) || (strcmp(mode_name, "480i")==0))
		return TVOUT_480I;
    if((strcmp(mode_name, "720p50hz")==0) || (strcmp(mode_name, "720P50HZ")==0))
        return TVOUT_720P50HZ;
    if((strcmp(mode_name, "1080i50hz")==0) || (strcmp(mode_name, "1080I50HZ")==0))
        return TVOUT_1080I50HZ;
    if((strcmp(mode_name, "1080p50hz")==0) || (strcmp(mode_name, "1080P50HZ")==0))
        return TVOUT_1080P50HZ;

	return TVOUT_MAX;
}

static char * to_modestr(int mode)
{
#define CASE_RET(_x_)	case TVOUT_##_x_: return #_x_

	switch(mode)
	{
		CASE_RET(1080P);
		CASE_RET(1080I);
		CASE_RET(720P);
		CASE_RET(576P);
		CASE_RET(480P);
		CASE_RET(576I);
		CASE_RET(480I);
        CASE_RET(720P50HZ);
        CASE_RET(1080I50HZ);
        CASE_RET(1080P50HZ);
	}
	return "UNKNOWN";
}

static int do_tv_out_open(int argc, char *argv[])
{
	int mode;
	int ret;

	if (argc < 2)
		goto usage;

	mode = to_mode(argv[1]);
	if(mode == TVOUT_MAX)
		goto usage;
	
	ret = tv_out_open(mode);
	if (ret!=0) {
		printf("tv %s %s failed\n",argv[0], argv[1]);
		return 1;
	}

	if(mode==TVOUT_1080I50HZ||mode==TVOUT_1080I||mode==TVOUT_576I||mode==TVOUT_480I){
		udelay(3*1000*1000);
		start_dsp();
	}
	return 0;

usage:
	puts("Usage: tv open mode(1080P/1080I/720P/576P/480P/576I/480I)\n");
	return 1;
}

static int do_tv_out_close(int argc, char *argv[])
{
	int ret;

	ret = tv_out_close();
	if (ret!=0) {
		printf("tv %s failed\n", argv[0]);
		return 1;
	}

	return 0;
}

static int do_tv_out_get_info(int argc, char *argv[])
{
	int mode;
	unsigned w,h;
	int ret;

	if (argc < 1)
		goto usage;

	mode = to_mode(argv[1]);
	if(mode == TVOUT_MAX)
		goto usage;

	ret = tv_out_get_info(mode, &w, &h);
	if (ret!=0) {
		printf("tv %s %s failed\n", argv[0], argv[1]);
		return 1;
	}

	printf("[%s]: %u x %u\n", argv[1], w, h);
	return 0;

usage:
	puts("Usage: tv minfo(1080P/1080I/720P/576P/480P/576I/480I)\n");
	return 1;
}

static int do_tv_out_cur_mode(int argc, char *argv[])
{
	int mode;

	mode = tv_out_cur_mode();
	printf("Current mode: %s\n", to_modestr(mode));
	
	return 0;
}

static int do_tv_out_tst(int argc, char *argv[])
{
	int mode;
	int ret;
	char *endp;

	if (argc < 2)
		goto usage;

	mode = simple_strtoul(argv[1], &endp, 0);
	if (*argv[1] == 0 || *endp != 0)
		goto usage;

	ret = tv_out_test(mode);
	if (ret!=0) {
		printf("tv %s %s failed\n",argv[0], argv[1]);
		return 1;
	}

	return 0;

usage:
	puts("Usage: tv tst mode(0-fix,1-colorbar,2-thinline,3-dotgrid)\n");
	return 1;
}

static int do_tv_pll_calc(int argc, char *argv[])
{	
	printf("DDR_PLL_CLK: %uMHz\n", clk_util_clk_msr(DDR_PLL_CLK));
	printf("CTS_A9_CLK: %uMHz\n", clk_util_clk_msr(CTS_A9_CLK));	
	printf("AUD_PLL_CLK: %uMHz\n", clk_util_clk_msr(AUD_PLL_CLK));
	printf("VID_PLL_CLK: %uMHz\n", clk_util_clk_msr(VID_PLL_CLK));
#ifdef CONFIG_AML_MESION_3
	printf("SYS_PLL_CLK: %uMHz\n", clk_util_clk_msr(SYS_PLL_CLK));
	printf("MISC_PLL_CLK: %uMHz\n", clk_util_clk_msr(MISC_PLL_CLK));
	printf("CLK81: %uMHz\n", clk_util_clk_msr(CLK81));
	printf("CTS_ENCP_CLK: %uMHz\n", clk_util_clk_msr(CTS_ENCP_CLK));
	printf("CTS_ENCL_CLK: %uMHz\n", clk_util_clk_msr(CTS_ENCL_CLK));
	printf("CTS_ENCT_CLK: %uMHz\n", clk_util_clk_msr(CTS_ENCT_CLK));
	printf("VID2_PLL_CLK: %uMHz\n", clk_util_clk_msr(VID2_PLL_CLK));
	printf("CTS_AMCLK: %uMHz\n", clk_util_clk_msr(CTS_AMCLK));
	printf("CTS_HDMI_SYS_CLK: %uMHz\n", clk_util_clk_msr(CTS_HDMI_SYS_CLK));
	printf("MOD_AUDIN_AMCLK_I: %uMHz\n", clk_util_clk_msr(MOD_AUDIN_AMCLK_I));
	printf("HDMI_CH3_TMDSCLK: %uMHz\n", clk_util_clk_msr(HDMI_CH3_TMDSCLK));
	printf("LVDS_FIFO_CLK: %uMHz\n", clk_util_clk_msr(LVDS_FIFO_CLK));
	printf("CTS_ENCI_CLK: %uMHz\n", clk_util_clk_msr(CTS_ENCI_CLK));
	printf("CTS_VDAC_CLK0: %uMHz\n", clk_util_clk_msr(CTS_VDAC_CLK0));
	printf("CTS_DDR_CLK: %uMHz\n", clk_util_clk_msr(CTS_DDR_CLK));
	printf("CTS_HDMI_TX_PIXEL_CLK: %uMHz\n", clk_util_clk_msr(CTS_HDMI_TX_PIXEL_CLK));
	printf("CTS_VDAC_CLK1: %uMHz\n", clk_util_clk_msr(CTS_VDAC_CLK1));
#endif
    return 0;
}

static int do_tv_reg_op(int argc, char *argv[])
{
	int mode;
	int ret;
	char *endp;
    unsigned int addr;
    unsigned int val = 0;
	unsigned int count = 0;
    
    if(*argv[1] == 'r') {
        addr = simple_strtoul(argv[3], NULL, 16);
		 if(argc > 4)
            count = simple_strtoul(argv[4], NULL, 16);
		 if(count > 0)
		 {
	        if(*argv[2] == 'c') {
	            printf("read cubs reg 0x%04x: 0x%x\n", addr, count);
				  while(count--)
				  	{
				  	   printf("0x%08x    ", READ_MPEG_REG(addr++));
						val++;
						if(val >= 8)
							{
							val = 0;
							printf("\n");
							}
				  	}
	        }
	        if(*argv[2] == 'h') {
	            printf("read hdmi reg 0x%03x: 0x%x\n", addr, count);
				  while(count--)
				  	{
				  	   printf("0x%08x    ", hdmi_rd_reg(addr++));
						val++;
						if(val >= 8)
							{
							val = 0;
							printf("\n");
							}
				  	}
	        }
		 }
		 else
		 {
	        if(*argv[2] == 'c') {
	            printf("read cubs reg 0x%04x: 0x%08x\n", addr, READ_MPEG_REG(addr));
	        }
	        if(*argv[2] == 'h') {
	            printf("read hdmi reg 0x%03x: 0x%08x\n", addr, hdmi_rd_reg(addr));
	        }
		 }
    }
    
    if(*argv[1] == 'w') {
        addr = simple_strtoul(argv[3], NULL, 16);
        val = simple_strtoul(argv[4], NULL, 16);
        if(*argv[2] == 'c') {
            printf("write cubs reg 0x%04x val 0x%08x\n", addr, WRITE_MPEG_REG(addr, val));
        }
        if(*argv[2] == 'h') {
            printf("write hdmi reg 0x%03x val 0x%08x\n", addr, hdmi_wr_reg(addr, val));
        }
    }

    return 0;
}

static int do_tv(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	const char *cmd;

	cmd = argv[1];

	if (strcmp(cmd, "open") == 0)
		return do_tv_out_open(argc - 1, argv + 1);
	if (strcmp(cmd, "close") == 0)
		return do_tv_out_close(argc - 1, argv + 1);
	if (strcmp(cmd, "minfo") == 0)
		return do_tv_out_get_info(argc - 1, argv + 1);
	if (strcmp(cmd, "mode") == 0)
		return do_tv_out_cur_mode(argc - 1, argv + 1);
	if (strcmp(cmd, "tst") == 0)
		return do_tv_out_tst(argc - 1, argv + 1);
	if (strcmp(cmd, "pll") == 0)
		return do_tv_pll_calc(argc - 1, argv + 1);
	if (strcmp(cmd, "reg") == 0)
		return do_tv_reg_op(argc - 1, argv + 1);

	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(
	tv,	8,	0,	do_tv,
	"tv sub-system",
	"open mode(1080P/1080I/720P/576P/480P/576I/480I)\n"
	"			- open the tv out\n"
	"tv close		- close the tv out\n"
	"tv minfo(1080P/1080I/720P/576P/480P/576I/480I)\n"
	"			- get mode info \n"
	"tv mode		- get current mode\n"
	"tv tst mode(0-fix,1-colorbar,2-thinline,3-dotgrid)\n"
	"                    - test tv output\n"
	"tv pll         - view important pll\n"
	"tv reg r/w c/h addr [val]  - READ/WRITE CBUS/HDMI regs\n"
);

#include <config.h>

#ifdef M3_DSP_VSYNC_INTERRUPT

#include <asm/arch/dsp_state.h>

static int dsp_machine=0;

static int tv_check_dsp_status(int argc, char *argv[])
{
	unsigned int dsp_status=0;
	int count;

	printf("check func:argc:%d\n",argc);
	if (argc < 2)
		goto usage;

	dsp_machine = 0;
	for(count=0;count<1000;count++)
	{
		dsp_status = read_reg(P_AO_RTI_STATUS_REG0);
		if(dsp_status == DSP_CURRENT_RUN)
		{
			dsp_machine = 1;
			printf("dsp is runing\n");
			break;
		}
		else if(dsp_status == DSP_REQUST_START)
		{	//continue;
		}
		else
		{
			break;
		}
	}
	if(dsp_status != DSP_CURRENT_RUN)
	{
		printf("dsp is not start\n");
	}
	return 0;

usage:
	return 1;
}

static int tv_request_stop_dsp(int argc, char *argv[])
{
	unsigned int dsp_status=0;
	if(dsp_machine == 1)
	{
		write_reg(P_AO_RTI_STATUS_REG0, DSP_REQUST_STOP);
		printf("request stop dsp\n");
		while(1)
		{
			dsp_status = read_reg(P_AO_RTI_STATUS_REG0);
			if(dsp_status == DSP_CURRENT_SLEEP)
			{
				write_reg(P_AO_RTI_STATUS_REG0, DSP_CURRENT_END);
				printf("dsp was sleep\n");
				break;
			}
		}
		return 0;
	}
	printf("dsp is not using\n");
	return 1;
}

static int do_tvdsp(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	const char *cmd;
	cmd = argv[1];

	printf("test tvdsp\n");
	printf("%s\n",cmd);
	if (strcmp(cmd, "check") == 0)
		return tv_check_dsp_status(argc,argv);
	if (strcmp(cmd, "stop") == 0)
		return tv_request_stop_dsp(argc,argv);

	cmd_usage(cmdtp);
	return 1;
}

U_BOOT_CMD(
	tvdsp, 8, 0, do_tvdsp,
	"tv sub-system",
	"check dsp\n"
	"                - check dsp(arc) is running\n"
	"tvdsp stop      - request stop dsp(arc)\n"
);
#endif  //M3_DSP_VSYNC_INTERRUPT


