#ifndef _A1H_16PP_H
#define _A1H_16PP_H
//#include<a3_osd_display/mpeg_reg_apollo.h>
//#include<a3_osd_display/mpeghelper.h>
//#include<a3_osd_display/venc_reg_apollo.h>
//#include<a3_osd_display/viu_reg_apollo.h>
//#include<a3_osd_display/apb_reg_a3.h>
//#include<a3_osd_display/apbhelper.h>
//#include<a3_osd_display/isa_reg_apollo.h>
//#include<a3_osd_display/isahelper.h>
//#include<a3_osd_display/mlvds_regs_a3.h>
//#include<a3_osd_display/peripheral_reg_apollo.h>
#define CANVAS_LVL3_MODE_2_28BIT            0
#define CANVAS_LVL3_MODE_2_27BIT_4_26BIT    1
#define CANVAS_LVL3_MODE_1_28BIT_8_25BIT    2
#define FLAG_OUTPUT_VALID           0x01

#define MAX_LVL3_LUT_NUM 16
#define CANVAS_ADDR_NOWRAP  0x00
#define CANVAS_BLKMODE_32X32    0x01
#define MAX_CANVAS_NUM  2
#define POST_BLEND  0
#define PRE_BLEND   1
#define BITBLT_PARAM_DRAW_RECT              2
#define CANVAS_LVL3_MODE_16_25BIT           5
#define OSD_LAYER_HW_LAYER0 0
#define OSD_PALETTE_TYPE_NONE   3
#define FLAG_H_SCALE                0x08
#define FLAG_V_SCALE    			0x10


extern unsigned LCD_WIDTH;
extern unsigned LCD_HEIGHT;
//#define endian_convert(c) ( ((c&0xff00)>>8) | ((c&0xff)<<8) )
#define color_convert(c)  (endian_convert(c))
#define endian_convert(c) ( ((c&0xff)<<24) | ((c&0xff00)<<8) | ((c&0xff0000)>>8) | (c>>24) )
inline void normal_draw_pixel(unsigned short x, unsigned short y, unsigned color);
int af_osd_layer_create1(void);
void canvas_release1(int index);
void osd_draw_string(char *text, unsigned short x, unsigned short y,unsigned color);
int osd_layer0_16bpp_create(unsigned short w, unsigned short h);
void osd_layer0_16bpp_pixel(unsigned short x, unsigned short y, unsigned color, unsigned params);
typedef struct {
    int index;
    int map;
    unsigned addr;
    unsigned shift;
} osd_canvas_t;

typedef struct canvas_s {
    unsigned long   addr;   /* canvas start address */
    unsigned        width;  /* canvas width, aligned at 8 bytes boundary */
    unsigned        height; /* canvas height, aligned at 8 bytes boundary */
    unsigned        wrap;   /* canvas wrap flag */
    unsigned        blkmode;/* canvas block mode */
    int             count;  /* canvas usage count */
} canvas_t; 

typedef struct osd_area{
    struct osd_area *parent;
    unsigned char layer;
    unsigned char type;
    unsigned char flags;
    unsigned char palette_type;
    unsigned short x;
    unsigned short y;
    unsigned short w;
    unsigned short h;
    unsigned short x_show;
    unsigned short y_show;
    unsigned short w_show;
    unsigned short h_show;
    unsigned buf_size;
    unsigned buf_even_start;
    unsigned buf_odd_start;
    unsigned char *palette;
    unsigned char *palette_mask;
    unsigned short target_alpha;
    unsigned short current_alpha;
    unsigned short delta_alpha;
    unsigned buf_even_offscr;
    unsigned header_size;
		int ge2d_handle;
		osd_canvas_t canvas;
    osd_canvas_t canvas_offscr;
} osd_area_t;


extern unsigned y_shift;
extern unsigned canvas_addr;


typedef struct osd_layer{
    unsigned w;
    unsigned h;
    unsigned char parent;
    unsigned char num_area;
    unsigned char flags;
    unsigned char mode;
    unsigned even_addr;
    unsigned odd_addr;
    unsigned buf_even_start;
    unsigned buf_even_end;
    unsigned buf_odd_start;
    unsigned buf_odd_end;
    osd_area_t *area[2];
    unsigned blend;
} osd_layer_t;
typedef struct _lcd_info
{
	unsigned lcd_width;
	unsigned lcd_height;
	unsigned width_position;//the x position on lcd
	unsigned hight_position;//the Y position on lcd
}lcd_info;

typedef struct _osd_display_info
{
	unsigned osd_address;//address where osd display
	unsigned bmp_load_addr_start;//the start address where bmp picture read from sd 
	unsigned rgb_data_addr_start;//the start address where rbg data stored after convert bmp picture,such as rgba or rgb655
	unsigned bmp_in_nand_addr_start;//the start address where the bmp rgb data write on nand.refer to the AVOS/bsp/nand_start.c
	unsigned bmp_nand_rw_size;//the size on nand one bmp picture write or read
	unsigned display_logo_num;//the number of logos to display
}osd_display_info;

extern lcd_info lcd_plat_info;
extern void lcd_power_on(void);
extern osd_display_info osd_display_plat_info;
extern void power_on_backlight(void);
void DisableVideoLayer1(void);
int show_bmp(osd_area_t *area,unsigned bmp_address);
#endif

