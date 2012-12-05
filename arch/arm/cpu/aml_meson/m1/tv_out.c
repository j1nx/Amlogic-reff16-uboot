
#include <common.h>

#define __typesafe_io
#include <asm/arch/io.h>

#include "tvregs.h"
#include "tv_out.h"

static int tvmode = -1;
static int used_audio_pll=-1;
unsigned int system_serial_low=0xA;

static int chip_version_init(void )
{
	char  *version_map;
	unsigned int version;

	/*0x49800000*/
	/*system_serial_low=,1,2,3,4,5 for REVA,B,C*/
	version_map=(char *)0x49800000;
	version = *(volatile unsigned int *)&version_map[0x1a0];
	switch(version)
	{
		case 0xe7e0c653:
			system_serial_low=0xA;
			break;
		case 0xe5832fe0:
			system_serial_low=0xB;
			break;	
		default:/*changed?*/
			system_serial_low=0xC;
	}
	printf("chip version=%x, serial:0x%X\n",version, system_serial_low);
 	return 0;
}

static unsigned long get_xtal_clock(void)
{
	unsigned long clk;
	
	clk=READ_CBUS_REG_BITS(PREG_CTLREG0_ADDR,4,5);
	clk=clk*1000*1000;
	return clk;
}
typedef  enum{
	INTERALCE_COMPONENT=0,
	CVBS_SVIDEO,
	PROGRESSIVE,
	SIGNAL_SET_MAX
}video_signal_set_t;
typedef enum {
    VIDEO_SIGNAL_TYPE_INTERLACE_Y = 0, /**< Interlace Y signal */
    VIDEO_SIGNAL_TYPE_CVBS,            /**< CVBS signal */
    VIDEO_SIGNAL_TYPE_SVIDEO_LUMA,     /**< S-Video luma signal */
    VIDEO_SIGNAL_TYPE_SVIDEO_CHROMA,   /**< S-Video chroma signal */
    VIDEO_SIGNAL_TYPE_INTERLACE_PB,    /**< Interlace Pb signal */
    VIDEO_SIGNAL_TYPE_INTERLACE_PR,    /**< Interlace Pr signal */
    VIDEO_SIGNAL_TYPE_INTERLACE_R,     /**< Interlace R signal */
    VIDEO_SIGNAL_TYPE_INTERLACE_G,     /**< Interlace G signal */
    VIDEO_SIGNAL_TYPE_INTERLACE_B,     /**< Interlace B signal */
    VIDEO_SIGNAL_TYPE_PROGRESSIVE_Y,   /**< Progressive Y signal */
    VIDEO_SIGNAL_TYPE_PROGRESSIVE_PB,  /**< Progressive Pb signal */
    VIDEO_SIGNAL_TYPE_PROGRESSIVE_PR,  /**< Progressive Pr signal */
    VIDEO_SIGNAL_TYPE_PROGEESSIVE_R,   /**< Progressive R signal */
    VIDEO_SIGNAL_TYPE_PROGEESSIVE_G,   /**< Progressive G signal */
    VIDEO_SIGNAL_TYPE_PROGEESSIVE_B,   /**< Progressive B signal */
    VIDEO_SIGNAL_TYPE_MAX
} video_signal_type_t;


#define  SET_VDAC(index,val)   (WRITE_MPEG_REG((index+VENC_VDAC_DACSEL0),val))
static const unsigned int  signal_set[SIGNAL_SET_MAX][3]=
{
	{	VIDEO_SIGNAL_TYPE_INTERLACE_Y,     // component interlace
		VIDEO_SIGNAL_TYPE_INTERLACE_PB,
		VIDEO_SIGNAL_TYPE_INTERLACE_PR,
	},
	{
		VIDEO_SIGNAL_TYPE_CVBS,            	//cvbs&svideo
		VIDEO_SIGNAL_TYPE_SVIDEO_LUMA,    
    	VIDEO_SIGNAL_TYPE_SVIDEO_CHROMA,   
	},
	{	VIDEO_SIGNAL_TYPE_PROGRESSIVE_Y,     //progressive.
		VIDEO_SIGNAL_TYPE_PROGRESSIVE_PB,
		VIDEO_SIGNAL_TYPE_PROGRESSIVE_PR,
	},
};
static  const  char*   signal_table[]={
	"INTERLACE_Y ", /**< Interlace Y signal */
    	"CVBS",            /**< CVBS signal */
    	"SVIDEO_LUMA",     /**< S-Video luma signal */
    	"SVIDEO_CHROMA",   /**< S-Video chroma signal */
    	"INTERLACE_PB",    /**< Interlace Pb signal */
    	"INTERLACE_PR",    /**< Interlace Pr signal */
    	"INTERLACE_R",     /**< Interlace R signal */
         "INTERLACE_G",     /**< Interlace G signal */
         "INTERLACE_B",     /**< Interlace B signal */
         "PROGRESSIVE_Y",   /**< Progressive Y signal */
         "PROGRESSIVE_PB",  /**< Progressive Pb signal */
         "PROGRESSIVE_PR",  /**< Progressive Pr signal */
         "PROGEESSIVE_R",   /**< Progressive R signal */
         "PROGEESSIVE_G",   /**< Progressive G signal */
         "PROGEESSIVE_B",   /**< Progressive B signal */
		
	};
//120120
void  change_vdac_setting(unsigned int  vdec_setting,int  mode)
{
	unsigned  int  signal_set_index=0;
	unsigned int  idx=0,bit=5,i;
	switch(mode )
	{
		case TVOUT_480I:
		case TVOUT_576I:
		signal_set_index=0;
		bit=5;
		break;
		//case TVOUT_480CVBS:
		//case TVOUT_576CVBS:
		//signal_set_index=1;	
		//bit=2;
		//break;
		default :
		signal_set_index=2;
		bit=5;
		break;
	}
	for(i=0;i<3;i++)
	{
		idx=vdec_setting>>(bit<<2)&0xf;
		printf("dac index:%d ,signal:%s\n",idx,signal_table[signal_set[signal_set_index][i]]);
		SET_VDAC(idx,signal_set[signal_set_index][i]);
		if(signal_set[signal_set_index][i] == VIDEO_SIGNAL_TYPE_INTERLACE_Y) {
          SET_VDAC(idx,(signal_set[signal_set_index][i] | 0xf000));
        }
		bit--;
	}
	
}


static int tvoutc_setclk(int mode)
{
	const  reg_t *sd,*hd;
	int xtal;

	if(used_audio_pll==-1)
		used_audio_pll=(system_serial_low==0xA)?1:0;
	
	if(used_audio_pll)
	{
		printf("TEST:used audio pll for video out for test!!\n");
		sd=tvreg_aclk_sd;
		hd=tvreg_aclk_hd;
	}
	else
	{
		printf("used Video pll for video out!!\n");
		sd=tvreg_vclk_sd;
		hd=tvreg_vclk_hd;
	}
	xtal=get_xtal_clock();
	xtal=xtal/1000000;
	if(xtal>=24 && xtal <=25)/*current only support 24,25*/
	{
		xtal-=24;
	}
	else
	{
		printf("UNsupport xtal setting for vidoe xtal=%d,default to 24M\n",xtal);	
		xtal=0;
	}
	switch(mode)
	{
		case TVOUT_480I:
		case TVOUT_480CVBS:
		case TVOUT_480P:
		case TVOUT_576I:
		case TVOUT_576CVBS:
		case TVOUT_576P:
			  setreg(&sd[xtal]);
			  //clk_set_rate(clk,540);
			  break;
		case TVOUT_720P:
		case TVOUT_1080I:
		case TVOUT_1080P:
			  setreg(&hd[xtal]);
			  if(xtal == 1)
			  {
				WRITE_MPEG_REG(HHI_VID_CLK_DIV, 4);
			  }
			 // clk_set_rate(clk,297);
			  break;
		default:
			printf("unsupport tv mode,video clk is not set!!\n");	
	}

	return 0;
}

static void enable_vsync_interrupt(void)
{
	
	if(used_audio_pll)
	{
		/* M1 chip test only, use audio PLL as video clock source */
		SET_CBUS_REG_MASK(HHI_MPEG_CLK_CNTL, 1<<11);
	}
	else
	{
	      /* M1 REVB , Video PLL bug fixed */
	        CLEAR_CBUS_REG_MASK(HHI_MPEG_CLK_CNTL, 1<<11);
	}
	
    if (READ_MPEG_REG(ENCP_VIDEO_EN) & 1) {
        WRITE_MPEG_REG(VENC_INTCTRL, 0x200);

        while ((READ_MPEG_REG(VENC_INTFLAG) & 0x200) == 0) {
            u32 line1, line2;

            line1 = line2 = READ_MPEG_REG(VENC_ENCP_LINE);

            while (line1 >= line2) {
                line2 = line1;
                line1 = READ_MPEG_REG(VENC_ENCP_LINE);
            }

            READ_MPEG_REG(VENC_INTFLAG);
            READ_MPEG_REG(VENC_INTFLAG);
            READ_MPEG_REG(VENC_INTFLAG);
            READ_MPEG_REG(VENC_INTFLAG);
            if (READ_MPEG_REG(VENC_INTFLAG) & 0x200) {
                break;
            }

            WRITE_MPEG_REG(ENCP_VIDEO_EN, 0);
            READ_MPEG_REG(VENC_INTFLAG);
            READ_MPEG_REG(VENC_INTFLAG);
            READ_MPEG_REG(VENC_INTFLAG);
            READ_MPEG_REG(VENC_INTFLAG);
            READ_MPEG_REG(VENC_INTFLAG);

            WRITE_MPEG_REG(ENCP_VIDEO_EN, 1);
            READ_MPEG_REG(VENC_INTFLAG);
            READ_MPEG_REG(VENC_INTFLAG);
            READ_MPEG_REG(VENC_INTFLAG);
            READ_MPEG_REG(VENC_INTFLAG);
            READ_MPEG_REG(VENC_INTFLAG);
        }
    }
    else{
        WRITE_MPEG_REG(VENC_INTCTRL, 0x2);
    }
}

int tv_out_open(int mode)
{
    const  reg_t *s;

    if (TVOUT_VALID(mode))
    {
        tvmode = mode;

        s = tvregsTab[mode];

        while (MREG_END_MARKER != s->reg)
            setreg(s++);

	chip_version_init();
	
	tvoutc_setclk(mode);
	enable_vsync_interrupt();

        WRITE_MPEG_REG(VPP_POSTBLEND_H_SIZE, tvinfoTab[mode].xres);

	change_vdac_setting(0x120120, mode);

	set_disp_mode_auto(mode );
        return 0;
    }

    return -1;
}

int tv_out_close(void)
{

    if (TVOUT_VALID(tvmode))
    {
        /* VENC_VDAC_SETTING */
        WRITE_MPEG_REG(VENC_VDAC_SETTING, 0xff);

        return 0;
    }

    return -1;
}

int tv_out_cur_mode(void)
{
    return tvmode;
}

int tv_out_get_info(int mode, unsigned *width, unsigned *height)
{
    if (TVOUT_VALID(mode))
    {
        *width = tvinfoTab[mode].xres;
        *height = tvinfoTab[mode].yres;

        return 0;
    }

    return -1;
}

enum tv_test_mode{
	TV_TST_DISABLE,
	TV_TST_FIX,
	TV_TST_COLORBAR,
	TV_TST_THINLINE,
	TV_TST_DOTGRID,
	TV_TST_MAX
};

#define TVTST_VALID(x) ((x)>=0 && (x)<TV_TST_MAX)
static const  reg_t tvregs_dis_tst[]={
	{VENC_VIDEO_TST_EN,	                                0},
	{MREG_END_MARKER,                                   0}
};
static const  reg_t tvregs_fix[]={
	{VENC_VIDEO_TST_MDSEL,                          0},
	{VENC_VIDEO_TST_Y,                              512},
	{VENC_VIDEO_TST_CR,                            512},
	{VENC_VIDEO_TST_CB,                            512},
	{VENC_VIDEO_TST_EN,	                                1},
	{MREG_END_MARKER,                                   0}
};
static const  reg_t tvregs_colorbar[]={
	{VENC_VIDEO_TST_MDSEL,                          1},
	{VENC_VIDEO_TST_CLRBAR_STRT,             74},
	{VENC_VIDEO_TST_CLRBAR_WIDTH,         180},
	{VENC_VIDEO_TST_EN,	                                1},
	{MREG_END_MARKER,                                   0}
};
static const  reg_t tvregs_thinline[]={
	{VENC_VIDEO_TST_MDSEL,                          2},
	{VENC_VIDEO_TST_Y,                              512},
	{VENC_VIDEO_TST_CR,                            512},
	{VENC_VIDEO_TST_CB,                            512},
	{VENC_VIDEO_TST_EN,	                                1},
	{MREG_END_MARKER,                                   0}
};
static const  reg_t tvregs_dotgrid[]={
	{VENC_VIDEO_TST_MDSEL,                          3},
	{VENC_VIDEO_TST_Y,                              512},
	{VENC_VIDEO_TST_CR,                            512},
	{VENC_VIDEO_TST_CB,                            512},
	{VENC_VIDEO_TST_EN,	                                1},
	{MREG_END_MARKER,                                   0}
};

static const reg_t *tvregsTab_tst[] = {
    tvregs_dis_tst,
    tvregs_fix,
    tvregs_colorbar,
    tvregs_thinline,
    tvregs_dotgrid
};

int tv_out_test(int mode)
{
    const  reg_t *s;

    if (TVTST_VALID(mode))
    {
	s = tvregsTab_tst[mode];

        while (MREG_END_MARKER != s->reg)
            setreg(s++);

        return 0;
    }

    return -1;
}

