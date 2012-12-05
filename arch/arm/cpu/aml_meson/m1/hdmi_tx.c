/*
 * Amlogic M1 
 * frame buffer driver-----------HDMI_TX
 * Copyright (C) 2010 Amlogic, Inc.
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
 */

#include "ubi_uboot.h"
#include "asm/arch-m1/am_regs.h"
#include "asm/arch-m1/vinfo.h"


#include "hdmi_info_global.h"
#include "hdmi_tx_module.h"

#define DEVICE_NAME "amhdmitx"
#define HDMI_TX_COUNT 32
#define HDMI_TX_POOL_NUM  6
#define HDMI_TX_RESOURCE_NUM 4

#ifdef DEBUG
#define pr_dbg(fmt, args...) printk(KERN_DEBUG "amhdmitx: " fmt, ## args)
#else
#define pr_dbg(fmt, args...)
#endif
#define pr_error(fmt, args...) printk(KERN_ERR "amhdmitx: " fmt, ## args)


static hdmitx_dev_t hdmitx_device;

static dev_t hdmitx_id;
static struct class *hdmitx_class;
static struct device *hdmitx_dev;

//static HDMI_TX_INFO_t hdmi_info;
#define INIT_FLAG_VDACOFF        0x1
#define INIT_FLAG_POWERDOWN      0x2

#define INIT_FLAG_NOT_LOAD 0x80

static unsigned char init_flag=0;
static unsigned char init_powermode=0;



//#undef DISABLE_AUDIO

void u_boot_init_hdmi()
{
	 HDMITX_M1B_Init(&hdmitx_device);
    	 hdmitx_device.HWOp.SetupIRQ(&hdmitx_device);

    	if(hdmitx_device.HWOp.Cntl){
        if(init_flag&INIT_FLAG_VDACOFF){
            hdmitx_device.HWOp.Cntl(&hdmitx_device, HDMITX_HWCMD_VDAC_OFF, 0);    
        }
        if(init_powermode&0x80){
            hdmitx_device.HWOp.Cntl(&hdmitx_device, HDMITX_HWCMD_LOWPOWER_SWITCH, init_powermode&0x1f);    
        }
    }

}

static unsigned char* hdmi_log_buf=NULL;
static unsigned int hdmi_log_wr_pos=0;
static unsigned int hdmi_log_rd_pos=0;
static unsigned int hdmi_log_buf_size=0;

static DEFINE_SPINLOCK(hdmi_print_lock);

#define PRINT_TEMP_BUF_SIZE 512
int hdmi_print_buf(char* buf, int len)
{
    unsigned long flags;
    int pos;
    int hdmi_log_rd_pos_;
    if(hdmi_log_buf_size==0)
        return 0;
    
    hdmi_log_rd_pos_=hdmi_log_rd_pos;
    if(hdmi_log_wr_pos>=hdmi_log_rd_pos)
        hdmi_log_rd_pos_+=hdmi_log_buf_size;

    for(pos=0;pos<len && hdmi_log_wr_pos<(hdmi_log_rd_pos_-1);pos++,hdmi_log_wr_pos++){
        if(hdmi_log_wr_pos>=hdmi_log_buf_size)
            hdmi_log_buf[hdmi_log_wr_pos-hdmi_log_buf_size]=buf[pos];
        else
            hdmi_log_buf[hdmi_log_wr_pos]=buf[pos];
    }    
    if(hdmi_log_wr_pos>=hdmi_log_buf_size)
        hdmi_log_wr_pos-=hdmi_log_buf_size;
    return pos;
    
}

int hdmi_print(int printk_flag, const char *fmt, ...)
{
    va_list args;
    int avail = PRINT_TEMP_BUF_SIZE;
    char buf[PRINT_TEMP_BUF_SIZE];
    int pos,len=0;
    if(printk_flag){
        va_start(args, fmt);
	      printf(fmt, args);
        va_end(args);	
    }
    if(hdmi_log_buf_size==0)
        return 0;
        
    va_start(args, fmt);
    //len += vsnprintf(buf+len, avail-len, fmt, args);
    va_end(args);	

    if ((avail-len) <= 0) {
        buf[PRINT_TEMP_BUF_SIZE - 1] = '\0';
    }
    pos = hdmi_print_buf(buf, len);
    //printk("hdmi_print:%d %d\n", hdmi_log_wr_pos, hdmi_log_rd_pos);
	  return pos;
}

static ssize_t show_log(struct device * dev, struct device_attribute *attr, char * buf)
{   
    unsigned long flags;
    int read_size=0;
    if(hdmi_log_buf_size==0)
        return 0;
    //printk("show_log:%d %d\n", hdmi_log_wr_pos, hdmi_log_rd_pos);
    if(hdmi_log_rd_pos<hdmi_log_wr_pos){
        read_size = hdmi_log_wr_pos-hdmi_log_rd_pos;
    }
    else if(hdmi_log_rd_pos>hdmi_log_wr_pos){
        read_size = hdmi_log_buf_size-hdmi_log_rd_pos;
    }
    if(read_size>PAGE_SIZE)
        read_size=PAGE_SIZE;
    if(read_size>0)
        memcpy(buf, hdmi_log_buf+hdmi_log_rd_pos, read_size);
    
    hdmi_log_rd_pos += read_size;  
    if(hdmi_log_rd_pos>=hdmi_log_buf_size)
        hdmi_log_rd_pos = 0;
    return read_size;    
}

static  int  set_disp_mode(const char *mode)
{
    int ret=-1;
    HDMI_Video_Codes_t vic;
    vic = hdmitx_edid_get_VIC(&hdmitx_device, mode, 1);
    hdmitx_device.cur_VIC = HDMI_Unkown;
    ret = hdmitx_set_display(&hdmitx_device, vic);
    if(ret>=0){
        hdmitx_device.cur_VIC = vic;  
    }
    return ret;
}

 int set_disp_mode_auto(int mode)
{
    int ret=-1;
   char  mode_name[][16]=
   {
   	"480i","480i","480p","576i","576i","576p","720p","1080i","1080p","invalid"	
   };
    HDMI_Video_Codes_t vic;
    if(mode>=9)
    {
    	printf("invalid tvout mode \n");
	return -1;
    }
	
    vic = hdmitx_edid_get_VIC(&hdmitx_device,mode_name[mode]/* info->name*/, (hdmitx_device.disp_switch_config==DISP_SWITCH_FORCE)?1:0);
    hdmitx_device.cur_VIC = HDMI_Unkown;
    ret = hdmitx_set_display(&hdmitx_device, vic); //if vic is HDMI_Unkown, hdmitx_set_display will disable HDMI
    if(ret>=0){
        hdmitx_device.cur_VIC = vic;    
    }
    return ret;
}    


