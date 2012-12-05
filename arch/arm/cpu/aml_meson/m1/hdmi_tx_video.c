#if 0
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <linux/major.h>
#include <linux/platform_device.h>
#include <linux/mutex.h>
#include <linux/cdev.h>
#endif
#include "ubi_uboot.h"
#include "asm/arch-m1/am_regs.h"
#include "asm/arch-m1/vinfo.h"

#include "hdmi_tx_module.h"
#include "hdmi_info_global.h"

static Hdmi_tx_video_para_t hdmi_tx_video_params[] = 
{
    { 
        .VIC            = HDMI_640x480p60,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_4_3,
        .cc             = CC_ITU601,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_480p60,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_4_3,
        .cc             = CC_ITU601,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_480p60_16x9,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU601,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_720p60,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
#ifdef DOUBLE_CLK_720P_1080I
        .repeat_time    = HDMI_2_TIMES_REPEAT,
#else
        .repeat_time    = NO_REPEAT,
#endif        
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU709,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_1080i60,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
#ifdef DOUBLE_CLK_720P_1080I
        .repeat_time    = HDMI_2_TIMES_REPEAT,
#else
        .repeat_time    = NO_REPEAT,
#endif        
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU709,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_480i60,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = HDMI_2_TIMES_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_4_3,
        .cc             = CC_ITU601,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_480i60_16x9,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = HDMI_2_TIMES_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU601,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_1440x480p60,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_4_3,
        .cc             = CC_ITU601,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_1080p60,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU709,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_576p50,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_4_3,
        .cc             = CC_ITU601,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_576p50_16x9,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU601,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_720p50,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU709,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_1080i50,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU709,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_576i50,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = HDMI_2_TIMES_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_4_3,
        .cc             = CC_ITU601,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_576i50_16x9,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = HDMI_2_TIMES_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU601,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_1080p50,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU709,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_1080p24,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU709,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_1080p25,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU709,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
    { 
        .VIC            = HDMI_1080p30,
        .color_prefer   = COLOR_SPACE_RGB444,
        .color_depth    = COLOR_24BIT,
        .bar_info       = B_BAR_VERT_HORIZ,
        .repeat_time    = NO_REPEAT,
        .aspect_ratio   = TV_ASPECT_RATIO_16_9,
        .cc             = CC_ITU709,
        .ss             = SS_SCAN_UNDER,   
        .sc             = SC_SCALE_HORIZ_VERT,
    },
};

static Hdmi_tx_video_para_t *hdmi_get_video_param(HDMI_Video_Codes_t VideoCode)
{
    Hdmi_tx_video_para_t * video_param=NULL;
	  int  i,count=ARRAY_SIZE(hdmi_tx_video_params);
    for(i=0;i<count;i++){
        if(hdmi_tx_video_params[i].VIC == VideoCode){
            break;    
        }
    }    
    if(i<count){
        video_param = &(hdmi_tx_video_params[i]);        
    }
    return video_param;
}    

static void hdmi_tx_construct_avi_packet(Hdmi_tx_video_para_t *video_param, char* AVI_DB)
{
    unsigned char color, bar_info, aspect_ratio, cc, ss, sc, ec;
    ss = video_param->ss;
    bar_info = video_param->bar_info;
    if(video_param->color == COLOR_SPACE_YUV444){
        color = 2;
    }
    else if(video_param->color == COLOR_SPACE_YUV422){
        color = 1;
    }
    else{ //(video_param->color == COLOR_SPACE_RGB444)
        color = 0;
    }
    AVI_DB[0] = (ss) | (bar_info << 2) | (1<<4) | (color << 5);
    //AVI_DB[0] = (1<<4) | (color << 5);

    aspect_ratio = video_param->aspect_ratio;
    cc = video_param->cc;
    AVI_DB[1] = (aspect_ratio) | (aspect_ratio << 4) | (cc << 6);
    //AVI_DB[1] = 8 | (aspect_ratio << 4) | (cc << 6);

    sc = video_param->sc;
    if(video_param->cc == CC_XVYCC601)
        ec = 0;
    else if(video_param->cc == CC_XVYCC709)
        ec = 1;
    else
        ec = 3;
    AVI_DB[2] = (sc) | (ec << 4);
    //AVI_DB[2] = 0;

    AVI_DB[3] = video_param->VIC;

    AVI_DB[4] = video_param->repeat_time;
}

/************************************
*    hdmitx protocol level interface
*************************************/

void hdmitx_init_parameters(HDMI_TX_INFO_t *info)
{
    memset(info, 0, sizeof(HDMI_TX_INFO_t));
    
    info->video_out_changing_flag = 1;
    
    info->audio_flag = 1;
    info->audio_info.type = CT_REFER_TO_STREAM;
    info->audio_info.format = AF_I2S;
    info->audio_info.fs = FS_44K1;
    info->audio_info.ss = SS_16BITS;
    info->audio_info.channels = CC_2CH;
    info->audio_info.audio_mclk = MCLK_256_Fs;
    info->audio_out_changing_flag = 1;
    
    info->auto_hdcp_ri_flag = 1;     // If == 1, turn on Auto Ri Checking
    info->hw_sha_calculator_flag = 1;    // If  == 1, use the HW SHA calculator, otherwise, use SW SHA calculator

}

int hdmitx_set_display(hdmitx_dev_t* hdmitx_device, HDMI_Video_Codes_t VideoCode)
{
    Hdmi_tx_video_para_t *param;
    int i,ret=-1;
    unsigned char AVI_DB[32];
    unsigned char AVI_HB[32];
    AVI_HB[0] = TYPE_AVI_INFOFRAMES ; 
    AVI_HB[1] = AVI_INFOFRAMES_VERSION ; 
    AVI_HB[2] = AVI_INFOFRAMES_LENGTH ; 
    for(i=0;i<32;i++){
        AVI_DB[i]=0;
    }

    param = hdmi_get_video_param(VideoCode);
    if(param){
        param->color = param->color_prefer;
        if(param->color == COLOR_SPACE_YUV444 &&
            (hdmitx_device->RXCap.native_Mode&0x20) == 0){
            param->color = COLOR_SPACE_YUV422;        
        }
        if(param->color == COLOR_SPACE_YUV422 &&
            (hdmitx_device->RXCap.native_Mode&0x10) == 0){
            param->color = COLOR_SPACE_RGB444;        
        }
        if(hdmitx_device->HWOp.SetDispMode(param)>=0){
    
            hdmi_tx_construct_avi_packet(param, AVI_DB);
    
            hdmitx_device->HWOp.SetPacket(HDMI_PACKET_AVI, AVI_DB, AVI_HB);
            ret = 0;
        }
    }
    else{
        hdmitx_device->HWOp.SetDispMode(NULL); //disable HDMI    
    }
    return ret;

}

int hdmi_set_3d(hdmitx_dev_t* hdmitx_device, int type, unsigned int param)
{
    int i;
    unsigned char VEN_DB[6];
    unsigned char VEN_HB[3];
    VEN_HB[0] = 0x81 ; 
    VEN_HB[1] = 0x01 ; 
    VEN_HB[2] = 0x6 ; 
    if(type==0xf){
        hdmitx_device->HWOp.SetPacket(HDMI_PACKET_VEND, NULL, VEN_HB);
    }
    else{
        for(i=0;i<0x6;i++){
            VEN_DB[i]=0;
        }
        VEN_DB[0]=0x03;
        VEN_DB[1]=0x0c;
        VEN_DB[2]=0x00;
        
        VEN_DB[3]=0x40;
        VEN_DB[4]=type<<4;
        VEN_DB[5]=param<<4;    
        hdmitx_device->HWOp.SetPacket(HDMI_PACKET_VEND, VEN_DB, VEN_HB);
    }  
    return 0;          

}    


