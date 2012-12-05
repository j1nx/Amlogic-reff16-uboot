/*
 * AMLOGIC LVDS controller driver.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the named License,
 * or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Author:  Tim Yao <timyao@amlogic.com>
 *
 */
#include <common.h>
#include <linux/err.h>
#include <asm/arch/am_regs.h>
#include <asm/arch/vinfo.h>
#include <asm/arch/gpio.h>
 
#define BL_MAX_LEVEL 0x100

#define Wr_reg_bits(reg, val, start, len) \
  Wr(reg, (Rd(reg) & ~(((1L<<(len))-1)<<(start))) | ((unsigned int)(val) << (start)))

#define Wr(adr,val)   WRITE_MPEG_REG(adr,val)
#define Rd(adr)       READ_MPEG_REG(adr)

/* config */
static int lvds_bits = 8;
static int jeida = 0;
static int port_reverse = 0;
 int flaga = 0;
 int flagb = 0;
 int flagc = 0;
 int flagd = 0;
/**/
static int cur_lvds_index = 0;
static int lvds_init_flag = 0;

static const vinfo_t lvds_info[] =
{
    {/* VMODE_1080P */
        .name              = "lvds1080p50hz",
        .mode              = VMODE_LVDS_1080P_50HZ,
        .width             = 1920,
        .height            = 1080,
        .field_height      = 1080,
        .aspect_ratio_num  = 16,
        .aspect_ratio_den  = 9,
        .sync_duration_num = 50,
        .sync_duration_den = 1,
    },
    {/* VMODE_1080P */
        .name              = "lvds1080p",
        .mode              = VMODE_LVDS_1080P,
        .width             = 1920,
        .height            = 1080,
        .field_height      = 1080,
        .aspect_ratio_num  = 16,
        .aspect_ratio_den  = 9,
        .sync_duration_num = 60,
        .sync_duration_den = 1,
    },
};


static inline void lvds_enable_vsync_interrupt(void)
{
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

static const vinfo_t *lvds_get_current_info(void)
{
    return &(lvds_info[cur_lvds_index]);
}

#define REG_LVDS_PHY_CLK_CNTL       0x14ef

#define LVDS_GEN_CNTL               0x14e0
#define REG_LVDS_PHY_CNTL0          0x14e1
#define REG_LVDS_PHY_CNTL1          0x14e2
#define REG_LVDS_PHY_CNTL2          0x14e3
#define REG_LVDS_PHY_CNTL3          0x14e4
#define REG_LVDS_PHY_CNTL4          0x14e5
#define REG_LVDS_PHY_CNTL5          0x14e6
#define REG_LVDS_PHY_CNTL6          0x14f1
#define REG_LVDS_PHY_CNTL7          0x14f2
#define REG_LVDS_PHY_CNTL8          0x14f3

#define LVDS_MDR_PU    REG_LVDS_PHY_CNTL4
#define LVDS_COMMON    REG_LVDS_PHY_CNTL3
void init_lvds_phy(void)
{
    unsigned tmp_add_data;
    Wr(REG_LVDS_PHY_CNTL4, Rd(REG_LVDS_PHY_CNTL4)|0xfff);

    Wr(REG_LVDS_PHY_CNTL3, 0x3f);

    tmp_add_data  = 0;
    tmp_add_data |= 0xf<<0; //LVDS_PREM_CTL<3:0>=<1111>
    tmp_add_data |= 0x3 << 4; //LVDS_SWING_CTL<3:0>=<0011>
    tmp_add_data |= 0x2<<8 ; //LVDS_VCM_CTL<2:0>=<010>
    tmp_add_data |= 0xf<<11; //LVDS_REFCTL<4:0>=<01111>

    Wr(REG_LVDS_PHY_CNTL5, tmp_add_data);

    Wr(REG_LVDS_PHY_CNTL0,0xfff);
    Wr(REG_LVDS_PHY_CNTL1,0xfff);
    Wr(REG_LVDS_PHY_CNTL2,0xfff);

    Wr(REG_LVDS_PHY_CNTL6,0xcccc);
    Wr(REG_LVDS_PHY_CNTL7,0xcccc);
    Wr(REG_LVDS_PHY_CNTL8,0xcccc);
}


static void vpp_set_matrix_ycbcr2rgb (int vd1_or_vd2_or_post, int mode)
{
   if (vd1_or_vd2_or_post == 0) //vd1
   {
      Wr_reg_bits (VPP_MATRIX_CTRL, 1, 5, 1);
      Wr_reg_bits (VPP_MATRIX_CTRL, 1, 8, 2);
   }
   else if (vd1_or_vd2_or_post == 1) //vd2
   {
      Wr_reg_bits (VPP_MATRIX_CTRL, 1, 4, 1);
      Wr_reg_bits (VPP_MATRIX_CTRL, 2, 8, 2);
   }
   else
   {
      Wr_reg_bits (VPP_MATRIX_CTRL, 1, 0, 1);
      Wr_reg_bits (VPP_MATRIX_CTRL, 0, 8, 2);
      if (mode == 0)
      {
        Wr_reg_bits(VPP_MATRIX_CTRL, 1, 1, 2);
      }
      else if (mode == 1)
      {
        Wr_reg_bits(VPP_MATRIX_CTRL, 0, 1, 2);
      }
   }

   if (mode == 0) //ycbcr not full range, 601 conversion
   {
        Wr(VPP_MATRIX_PRE_OFFSET0_1, 0x7c00600);
        Wr(VPP_MATRIX_PRE_OFFSET2, 0x0600);

        //1.164     0       1.596
        //1.164   -0.392    -0.813
        //1.164   2.017     0
        Wr(VPP_MATRIX_COEF00_01, (0x4a8 << 16) |
                            0);
        Wr(VPP_MATRIX_COEF02_10, (0x662 << 16) |
                            0x4a8);
        Wr(VPP_MATRIX_COEF11_12, (0x1e6f << 16) |
                            0x1cbf);
        Wr(VPP_MATRIX_COEF20_21, (0x4a8 << 16) |
                            0x811);
        Wr(VPP_MATRIX_COEF22, 0x0);
        Wr(VPP_MATRIX_OFFSET0_1, 0x0);
        Wr(VPP_MATRIX_OFFSET2, 0x0);
   }
   else if (mode == 1) //ycbcr full range, 601 conversion
   {

        Wr(VPP_MATRIX_PRE_OFFSET0_1, 0x0000600);
        Wr(VPP_MATRIX_PRE_OFFSET2, 0x0600);

        //1     0       1.402
        //1   -0.34414  -0.71414
        //1   1.772     0
        Wr(VPP_MATRIX_COEF00_01, (0x400 << 16) |
                            0);
        Wr(VPP_MATRIX_COEF02_10, (0x59c << 16) |
                            0x400);
        Wr(VPP_MATRIX_COEF11_12, (0x1ea0 << 16) |
                            0x1d25);
        Wr(VPP_MATRIX_COEF20_21, (0x400 << 16) |
                            0x717);
        Wr(VPP_MATRIX_COEF22, 0x0);
        Wr(VPP_MATRIX_OFFSET0_1, 0x0);
        Wr(VPP_MATRIX_OFFSET2, 0x0);
   }
}

// -----------------------------------------
// clk_util_lvds_set_clk_div()
// -----------------------------------------
// This task is used to setup the LVDS dividers.
//
// Dual Pixel Mode
//    clk_util_lvds_set_clk_div(  2,          // unsigned long   divn_sel,        // select divide by 3.5
//                                7,          // unsigned long   divn_tcnt,       // ignored
//                                0 );        // unsigned long   div2_en,         // divide by 1 for LVDS
//
// Single Pixel Mode
//    clk_util_lvds_set_clk_div(  1,          // unsigned long   divn_sel,        // select divide by N
//                                7,          // unsigned long   divn_tcnt,       // divide by 7
//                                0 );        // unsigned long   div2_en,         // divide by 1 for LVDS
static void    clk_util_lvds_set_clk_div(  unsigned long   divn_sel,
                                    unsigned long   divn_tcnt,
                                    unsigned long   div2_en  )
{
    // assign          lvds_div_phy_clk_en     = tst_lvds_tmode ? 1'b1         : phy_clk_cntl[10];
    // assign          lvds_div_div2_sel       = tst_lvds_tmode ? atest_i[5]   : phy_clk_cntl[9];
    // assign          lvds_div_sel            = tst_lvds_tmode ? atest_i[7:6] : phy_clk_cntl[8:7];
    // assign          lvds_div_tcnt           = tst_lvds_tmode ? 3'd6         : phy_clk_cntl[6:4];
    // If dividing by 1, just select the divide by 1 path
    if( divn_tcnt == 1 ) { divn_sel = 0; }

    Wr( REG_LVDS_PHY_CLK_CNTL, ((Rd(REG_LVDS_PHY_CLK_CNTL) & ~((0x3 << 7) | (1 << 9) | (0x7 << 4))) | ((1 << 10) | (divn_sel << 7) | (div2_en << 9) | (((divn_tcnt-1)&0x7) << 4))) );
}

int set_tv_enc_1080p(void )
{
    Wr(ENCI_VIDEO_EN, 0);                 // Enable Interlace video encoder
    Wr(ENCP_VIDEO_EN, 0);                 // Enable Interlace video encoder
    Wr(ENCP_VIDEO_MODE,             0x0040 | (1<<14)); // Enable Hsync and equalization pulse switch in center; bit[14] cfg_de_v = 1
    Wr(ENCP_VIDEO_MODE_ADV,         0x0008); // Sampling rate: 1
    Wr(ENCP_VIDEO_YFP1_HTIME,       140);
    Wr(ENCP_VIDEO_YFP2_HTIME,       2060);

    Wr(ENCP_VIDEO_MAX_PXCNT,        2199);
    Wr(ENCP_VIDEO_HSPULS_BEGIN,     2156);
    Wr(ENCP_VIDEO_HSPULS_END,       44);
    Wr(ENCP_VIDEO_HSPULS_SWITCH,    44);
    Wr(ENCP_VIDEO_VSPULS_BEGIN,     140);
    Wr(ENCP_VIDEO_VSPULS_END,       2059);
    Wr(ENCP_VIDEO_VSPULS_BLINE,     0);
    Wr(ENCP_VIDEO_VSPULS_ELINE,     4);

    Wr(ENCP_VIDEO_HAVON_BEGIN,      148);
    Wr(ENCP_VIDEO_HAVON_END,        2067);
    Wr(ENCP_VIDEO_VAVON_BLINE,      41);
    Wr(ENCP_VIDEO_VAVON_ELINE,      1120);

    Wr(ENCP_VIDEO_HSO_BEGIN,        44);
    Wr(ENCP_VIDEO_HSO_END,          2156);
    Wr(ENCP_VIDEO_VSO_BEGIN,        2100);
    Wr(ENCP_VIDEO_VSO_END,          2164);

    Wr(ENCP_VIDEO_VSO_BLINE,        3);
    Wr(ENCP_VIDEO_VSO_ELINE,        5);
    if(lvds_info[cur_lvds_index].mode == VMODE_LVDS_1080P_50HZ)
        Wr(ENCP_VIDEO_MAX_LNCNT,        1349);
    else
        Wr(ENCP_VIDEO_MAX_LNCNT,        1124);

    Wr(ENCP_VIDEO_FILT_CTRL,        0x1000); //bypass filter

    Wr(VENC_VDAC_DACSEL0,           0x9);
    Wr(VENC_VDAC_DACSEL1,           0xa);
    Wr(VENC_VDAC_DACSEL2,           0xb);
    Wr(VENC_VDAC_DACSEL3,           0xc);
    Wr(VENC_VDAC_DACSEL4,           0xd);
    Wr(VENC_VDAC_DACSEL5,           0xe);

    //Wr(ENCP_VIDEO_EN, 1); // Enable it in test.c
   return 0;
}


static void init_lvds(void)
{
    unsigned char lvds_bits_cfg = 1; // 0:10bits, 1:8bits, 2:6bits, 3:4bits.
    unsigned int  dithering_cfg = 0;

    switch(lvds_bits){
        case 10:
            lvds_bits_cfg = 0;
            dithering_cfg = 0;
            break;
        case 8:
            lvds_bits_cfg = 1;
            dithering_cfg = 0x400;
            break;
        case 6:
            lvds_bits_cfg = 2;
            dithering_cfg = 0x600;
            break;
        case 4:
            lvds_bits_cfg = 3;
            dithering_cfg = 0;
            break;
    }
    clk_util_lvds_set_clk_div(  2,          // unsigned long   divn_sel,        // select divide by 3.5 (0 = div1, 1=divN,2 = div3.5)
                                7,          // unsigned long   divn_tcnt,       // ignored
                                0 );        // unsigned long   div2_en,         // divide by 1 for LVDS

    //Wr( REG_LVDS_PHY_CNTL0, 0xffff );

    // wire            fifo_en                 = tst_lvds_tmode ? atest_i[3]   : gen_cntl[3];
    // wire            fifo_wr_bist_gate       = tst_lvds_tmode ? 1'b1         : gen_cntl[2];
    // wire    [1:0]   fifo_wr_mode            =                                 gen_cntl[1:0];
    //Wr( LVDS_GEN_CNTL, (Rd(LVDS_GEN_CNTL) | (3 << 0)) );    //use dv_continuous as the fifo_wr_en
    // enable the FIFO and use dv_continuous as the fifo_wr_en
    Wr( LVDS_GEN_CNTL, (Rd(LVDS_GEN_CNTL) | (1 << 3) | (3<< 0)));

    set_tv_enc_1080p();

    vpp_set_matrix_ycbcr2rgb(2, 0);
    Wr(ENCP_VIDEO_RGBIN_CTRL, 3);
    Wr(RGB_BASE_ADDR, 0);
    Wr(RGB_COEFF_ADDR, 0x400);


    //Wr(ENCP_VIDEO_EN,       1);

    Wr(LVDS_PACK_CNTL_ADDR,
       ( jeida? 0:1 ) | // repack
       ( port_reverse?(0<<2):(1<<2) ) | // odd_even reverse
       ( 0<<3 ) | // reserve
       ( 0<<4 ) | // lsb first
       ( 0<<5 ) | // pn swap
       ( 1<<6 ) | // dual port
       ( 0<<7 ) | // use tcon control
       ( lvds_bits_cfg<<8) | // 0:10bits, 1:8bits, 2:6bits, 3:4bits.
       ( 0<<10 ) | // 0:R, 1:G, 2:B, 3:0
       ( 1<<12 ) |
       ( 2<<14 ));

    Wr(POL_CNTL_ADDR,  (1 << DCLK_SEL) |
        //(0x1 << HS_POL) |
       (0x1 << VS_POL)
    );

    Wr(DITH_CNTL_ADDR, dithering_cfg);
}

// -----------------------------------------------------------------
// clk_util_set_video_clock()
//
// This function sets the "master clock" in the video clock
// module $clk_rst_tst/rtl/crt_video
//
// wire            cntl_sclk_n     = hi_video_clk_cntl[10];
// wire    [1:0]   cntl_vclk3_mux  = hi_video_clk_cntl[9:8];
// wire    [1:0]   cntl_vclk2_mux  = hi_video_clk_cntl[7:6];
// wire    [1:0]   cntl_vclk1_mux  = hi_video_clk_cntl[5:4];
// wire    [3:0]   cntl_xd         = hi_video_clk_cntl[3:0];
static void clk_util_set_video_clock( unsigned long hiu_reg, unsigned long xd )
{
    // switch video clock to sclk_n and enable the PLL
    Wr( HHI_VID_CLK_CNTL, Rd(HHI_VID_CLK_CNTL) & ~(1 << 10) );   // Switch to the SCLK
    // delay 2uS to allow the sync mux to switch over
    Wr( ISA_TIMERE, 0); while( Rd(ISA_TIMERE) < 2 ) {}
    // Bring out of reset but keep bypassed to allow to stablize
    //Wr( HHI_VID_PLL_CNTL, (0 << 15) | (0 << 14) | (hiu_reg & 0x3FFF) );
    Wr( HHI_VID_PLL_CNTL, hiu_reg );
    // Setup external divider in $clk_rst_tst/rtl/crt_video.v
    // wire    [3:0]   cntl_div_by_1       = hi_vid_clk_cntl[3:0];     // for LVDS we can divide by 1 (pll direct)
    // wire            cntl_vclk_gate      = hi_vid_clk_cntl[0];       // needed to mux divide by 1
    //
    // wire    [7:0]   cntl_xd             = hi_vid_clk_div[7:0];
    if( xd == 1 ) {
        // Set divide by 1 in crt_video
        Wr( HHI_VID_CLK_CNTL, Rd(HHI_VID_CLK_CNTL) | (0xF << 0) );
    } else {
        Wr( HHI_VID_CLK_CNTL, (Rd(HHI_VID_CLK_CNTL) & ~(0xF << 0)) );               // disable divide by 1 control
    }
    Wr( HHI_VID_CLK_DIV, (Rd(HHI_VID_CLK_DIV) & ~(0xFF << 0)) | (xd-1) );   // setup the XD divider value
    // delay 5uS
    Wr( ISA_TIMERE, 0); while( Rd(ISA_TIMERE) < 5 ) {}
    Wr( HHI_VID_CLK_CNTL, Rd(HHI_VID_CLK_CNTL) | (1 << 10) );   // Switch to the PLL
    // delay 2uS
    Wr( ISA_TIMERE, 0); while( Rd(ISA_TIMERE) < 2 ) {}
}

static int lvds_set_current_vmode(vmode_t mode)
{
    int i, j, count = ARRAY_SIZE(lvds_info);
    long m = 10, n = 10;
    for(i=0;i<count;i++){
        if(lvds_info[i].mode==mode){
            cur_lvds_index = i;
            break;
        }
    }
    if(i == count)
        return -EINVAL;
    /* to do */
    if (lvds_init_flag == 0) {
        /*for (j=0; j<m; j++)
            udelay(1000);*/
        clk_util_set_video_clock( 0x00000641, 1 );  // divide by 1
        init_lvds_phy();
        init_lvds();
        Wr(ENCP_VIDEO_EN,       1);
        Wr(VPP_HOLD_LINES, 0x08080808);

        lvds_enable_vsync_interrupt();
        WRITE_MPEG_REG(VPP_POSTBLEND_H_SIZE, lvds_info[0].width);
        /*for (j=0; j<n; j++)
            udelay(1000);*/
        lvds_init_flag = 1;
    }
    return 0;
}

static const vinfo_t *get_valid_vinfo(char  *mode)
{
    int  i,count=ARRAY_SIZE(lvds_info);

    for(i=0;i<count;i++)
    {
        if(strncmp(lvds_info[i].name,mode,strlen(lvds_info[i].name))==0)
        {
            return &lvds_info[i];
        }
    }
    return NULL;
}

static vmode_t lvds_validate_vmode(char *mode)
{
    const vinfo_t *info = get_valid_vinfo(mode);

    if (info)
        return info->mode;

    return VMODE_MAX;
}

static int lvds_vmode_is_supported(vmode_t mode)
{
    int  i,count=ARRAY_SIZE(lvds_info);

    for(i=0;i<count;i++)
    {
        if(lvds_info[i].mode==mode)
        {
            return true;
        }
    }
    return false;

}

static int lvds_module_disable(vmode_t cur_vmod)
{
    return 0;
}

#ifdef  CONFIG_PM
static int lvds_suspend(void)
{
    int j;
    long m = 250, n= 200;

    if (pdata->lvds_backlight_power_off)
        pdata->lvds_backlight_power_off();

    for (j=0; j<n; j++)
        udelay(1000);

    Wr(REG_LVDS_PHY_CNTL4, Rd(REG_LVDS_PHY_CNTL4)&(~0xfff));//LVDS_MDR_PU
    Wr(REG_LVDS_PHY_CNTL5,  Rd(REG_LVDS_PHY_CNTL5)&(~(0xc<<11))); //LVDS_REFCTL<3:2> = 0
    Wr(REG_LVDS_PHY_CNTL0,0); //LVDS_PDN_A*=0, LVDS_PDN_B*=0

    lvds_init_flag = 0;

    for (j=0; j<m; j++)
        udelay(1000);

    if (pdata->lvds_panel_power_off)
        pdata->lvds_panel_power_off();
    printk("vout lvds suspend\n");

    return 0;
}
static int lvds_resume(void)
{
    int j;
    int m = 200, n = 300;

    if (pdata->lvds_panel_power_on)
        pdata->lvds_panel_power_on();

    for (j=0; j<n; j++)
        udelay(1000);

    Wr(REG_LVDS_PHY_CNTL4, Rd(REG_LVDS_PHY_CNTL4)|0xfff); //LVDS_MDR_PU
    Wr(REG_LVDS_PHY_CNTL5,  Rd(REG_LVDS_PHY_CNTL5)|(0xf<<11)); //LVDS_REFCTL<4:0>=<01111>
    Wr(REG_LVDS_PHY_CNTL0,0xfff); //LVDS_PDN_A*=1, LVDS_PDN_B*=1

    for (j=0; j<m; j++)
        udelay(1000);

    if (pdata->lvds_backlight_power_on)
        pdata->lvds_backlight_power_on();
    printk("vout lvds resume\n");

    return 0;
}
#endif

void u_boot_init_lvds()
{
    int j;
    int m = 10, n = 34;
		unsigned long reverse = simple_strtoul(getenv ("port_reverse"), NULL, 16);
		if(reverse == 1){
			port_reverse = 1;
			}
    //GPIOX_54 LVDS power off
    set_gpio_val(GPIOX_bank_bit32_63(54), GPIOX_bit_bit32_63(54), 0);
    set_gpio_mode(GPIOX_bank_bit32_63(54), GPIOX_bit_bit32_63(54), 0);

    //GPIOX_54 LVDS power on
    set_gpio_val(GPIOX_bank_bit32_63(54), GPIOX_bit_bit32_63(54), 1);
    set_gpio_mode(GPIOX_bank_bit32_63(54), GPIOX_bit_bit32_63(54), 0);

    
    for (j=0; j<n; j++)
        udelay(1000);

    Wr(REG_LVDS_PHY_CNTL4, Rd(REG_LVDS_PHY_CNTL4)|0xfff); //LVDS_MDR_PU
    Wr(REG_LVDS_PHY_CNTL5,  Rd(REG_LVDS_PHY_CNTL5)|(0xf<<11)); //LVDS_REFCTL<4:0>=<01111>
    Wr(REG_LVDS_PHY_CNTL0,0xfff); //LVDS_PDN_A*=1, LVDS_PDN_B*=1

    /*for (j=0; j<m; j++)
        udelay(1000);*/

    //GPIOB_6 Backlight power on
    set_gpio_val(GPIOB_bank_bit0_7(6), GPIOB_bank_bit0_7(6), 0);
    set_gpio_mode(GPIOB_bank_bit0_7(6), GPIOB_bank_bit0_7(6), 0);

    lvds_set_current_vmode(VMODE_LVDS_1080P);
    //pwm
	set_gpio_val(GPIOX_bank_bit32_63(55), GPIOX_bit_bit32_63(55), 0);
	set_gpio_mode(GPIOX_bank_bit32_63(55), GPIOX_bit_bit32_63(55), GPIO_OUTPUT_MODE);
			
	set_gpio_val(GPIOB_bank_bit0_8(2), GPIOB_bit_bit0_8(2), 1);
	set_gpio_mode(GPIOB_bank_bit0_8(2), GPIOB_bit_bit0_8(2), 0);
			
	set_gpio_val(GPIOB_bank_bit0_8(3), GPIOB_bit_bit0_8(3), 1);
	set_gpio_mode(GPIOB_bank_bit0_8(3), GPIOB_bit_bit0_8(3), 0);	    
}

