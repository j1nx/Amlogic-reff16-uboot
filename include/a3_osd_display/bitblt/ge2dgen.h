#ifndef GE2DGEN_H
#define GE2DGEN_H

//#if defined(AML_A1H)
#define GE2D_ENDIAN_SHIFT       	24
#define GE2D_ENDIAN_MASK            (0x1 << GE2D_ENDIAN_SHIFT)
#define GE2D_BIG_ENDIAN             (0 << GE2D_ENDIAN_SHIFT)
#define GE2D_LITTLE_ENDIAN          (1 << GE2D_ENDIAN_SHIFT)

#define GE2D_COLOR_MAP_SHIFT        20
#define GE2D_COLOR_MAP_MASK         (0xf << GE2D_COLOR_MAP_SHIFT)
/* 16 bit */
#define GE2D_COLOR_MAP_YUV422		(0 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGB655		(1 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUV655		(1 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGB844		(2 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUV844		(2 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGBA6442     (3 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUVA6442     (3 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGBA4444     (4 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUVA4444     (4 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGB565       (5 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUV565       (5 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_ARGB4444		(6 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_AYUV4444		(6 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_ARGB1555     (7 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_AYUV1555     (7 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGBA4642     (8 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUVA4642     (8 << GE2D_COLOR_MAP_SHIFT)
/* 24 bit */
#define GE2D_COLOR_MAP_RGB888       (0 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUV444       (0 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGBA5658     (1 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUVA5658     (1 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_ARGB8565     (2 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_AYUV8565     (2 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_RGBA6666     (3 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUVA6666     (3 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_ARGB6666     (4 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_AYUV6666     (4 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_BGR888		(5 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_VUY888		(5 << GE2D_COLOR_MAP_SHIFT)
/* 32 bit */
#define GE2D_COLOR_MAP_RGBA8888		(0 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_YUVA8888		(0 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_ARGB8888     (1 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_AYUV8888     (1 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_ABGR8888     (2 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_AVUY8888     (2 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_BGRA8888     (3 << GE2D_COLOR_MAP_SHIFT)
#define GE2D_COLOR_MAP_VUYA8888     (3 << GE2D_COLOR_MAP_SHIFT)
//#endif

/* format code is defined as:
[11] : 1-YUV color space, 0-RGB color space
[10] : compress_range
[9:8]: format
[7:6]: 8bit_mode_sel
[5]  : LUT_EN
[4:3]: PIC_STRUCT
[2]  : SEP_EN
[1:0]: X_YC_RATIO, SRC1_Y_YC_RATIO
*/
#define GE2D_FORMAT_MASK            0x0ffff
#define GE2D_FORMAT_YUV             0x20000
#define GE2D_FORMAT_COMP_RANGE      0x10000

//#if defined(AML_A1H)
#define GE2D_FMT_S8_Y            	0x00000 /* 00_00_0_00_0_00 */
#define GE2D_FMT_S8_CB           	0x00040 /* 00_01_0_00_0_00 */
#define GE2D_FMT_S8_CR           	0x00080 /* 00_10_0_00_0_00 */
#define GE2D_FMT_S8_R            	0x00000 /* 00_00_0_00_0_00 */
#define GE2D_FMT_S8_G            	0x00040 /* 00_01_0_00_0_00 */
#define GE2D_FMT_S8_B            	0x00080 /* 00_10_0_00_0_00 */
#define GE2D_FMT_S8_A            	0x000c0 /* 00_11_0_00_0_00 */
#define GE2D_FMT_S8_LUT          	0x00020 /* 00_00_1_00_0_00 */
#define GE2D_FMT_S16_YUV422      	0x20100 /* 01_00_0_00_0_00 */
#define GE2D_FMT_S16_RGB         	0x00100 /* 01_00_0_00_0_00 */
#define GE2D_FMT_S24_YUV444      	0x20200 /* 10_00_0_00_0_00 */
#define GE2D_FMT_S24_RGB         	0x00200 /* 10_00_0_00_0_00 */
#define GE2D_FMT_S32_YUVA444     	0x20300 /* 11_00_0_00_0_00 */
#define GE2D_FMT_S32_RGBA        	0x00300 /* 11_00_0_00_0_00 */
#define GE2D_FMT_M24_YUV420      	0x20007 /* 00_00_0_00_1_11 */
#define GE2D_FMT_M24_YUV422      	0x20006 /* 00_00_0_00_1_10 */
#define GE2D_FMT_M24_YUV444      	0x20004 /* 00_00_0_00_1_00 */
#define GE2D_FMT_M24_RGB         	0x00004 /* 00_00_0_00_1_00 */
#define GE2D_FMT_M24_YUV420T     	0x20017 /* 00_00_0_10_1_11 */
#define GE2D_FMT_M24_YUV420B     	0x2001f /* 00_00_0_11_1_11 */
#define GE2D_FMT_S16_YUV422T     	0x20110 /* 01_00_0_10_0_00 */
#define GE2D_FMT_S16_YUV422B     	0x20138 /* 01_00_0_11_0_00 */
#define GE2D_FMT_S24_YUV444T     	0x20210 /* 10_00_0_10_0_00 */
#define GE2D_FMT_S24_YUV444B     	0x20218 /* 10_00_0_11_0_00 */

/* back compatible defines */
#define GE2D_FORMAT_S8_Y            GE2D_FMT_S8_Y            
#define GE2D_FORMAT_S8_CB           GE2D_FMT_S8_CB           
#define GE2D_FORMAT_S8_CR           GE2D_FMT_S8_CR           
#define GE2D_FORMAT_S8_R            GE2D_FMT_S8_R            
#define GE2D_FORMAT_S8_G            GE2D_FMT_S8_G            
#define GE2D_FORMAT_S8_B            GE2D_FMT_S8_B            
#define GE2D_FORMAT_S8_A            GE2D_FMT_S8_A            
#define GE2D_FORMAT_S8_LUT          GE2D_FMT_S8_LUT          
#define GE2D_FORMAT_S16_YUV422      (GE2D_FMT_S16_YUV422  | GE2D_COLOR_MAP_YUV422)
#define GE2D_FORMAT_S16_RGB         (GE2D_FMT_S16_RGB     | GE2D_COLOR_MAP_RGB655)  
#define GE2D_FORMAT_S24_YUV444      (GE2D_FMT_S24_YUV444  | GE2D_COLOR_MAP_YUV444) 
#define GE2D_FORMAT_S24_RGB         (GE2D_FMT_S24_RGB     | GE2D_COLOR_MAP_RGB888)   
#define GE2D_FORMAT_S32_YUVA444     (GE2D_FMT_S32_YUVA444 | GE2D_COLOR_MAP_YUVA4444)   
#define GE2D_FORMAT_S32_RGBA        (GE2D_FMT_S32_RGBA    | GE2D_COLOR_MAP_RGBA8888) 
#define GE2D_FORMAT_M24_YUV420      GE2D_FMT_M24_YUV420    
#define GE2D_FORMAT_M24_YUV422      GE2D_FMT_M24_YUV422
#define GE2D_FORMAT_M24_YUV444      GE2D_FMT_M24_YUV444
#define GE2D_FORMAT_M24_RGB         GE2D_FMT_M24_RGB
#define GE2D_FORMAT_M24_YUV420T     GE2D_FMT_M24_YUV420T
#define GE2D_FORMAT_M24_YUV420B     GE2D_FMT_M24_YUV420B
#define GE2D_FORMAT_S16_YUV422T     (GE2D_FMT_S16_YUV422T | GE2D_COLOR_MAP_YUV422)
#define GE2D_FORMAT_S16_YUV422B     (GE2D_FMT_S16_YUV422B | GE2D_COLOR_MAP_YUV422)   
#define GE2D_FORMAT_S24_YUV444T     (GE2D_FMT_S24_YUV444T | GE2D_COLOR_MAP_YUV444)   
#define GE2D_FORMAT_S24_YUV444B     (GE2D_FMT_S24_YUV444B | GE2D_COLOR_MAP_YUV444)   
//#else
//#define GE2D_FORMAT_MASK            0x0ffff
//#define GE2D_FORMAT_YUV             0x20000
//#define GE2D_FORMAT_COMP_RANGE      0x10000
//#define GE2D_FORMAT_S8_Y            0x00000 /* 00_00_0_00_0_00 */
//#define GE2D_FORMAT_S8_CB           0x00040 /* 00_01_0_00_0_00 */
//#define GE2D_FORMAT_S8_CR           0x00080 /* 00_10_0_00_0_00 */
//#define GE2D_FORMAT_S8_R            0x00000 /* 00_00_0_00_0_00 */
//#define GE2D_FORMAT_S8_G            0x00040 /* 00_01_0_00_0_00 */
//#define GE2D_FORMAT_S8_B            0x00080 /* 00_10_0_00_0_00 */
//#define GE2D_FORMAT_S8_A            0x000c0 /* 00_11_0_00_0_00 */
//#define GE2D_FORMAT_S8_LUT          0x00020 /* 00_00_1_00_0_00 */
//#define GE2D_FORMAT_S16_YUV422      0x20100 /* 01_00_0_00_0_00 */
//#define GE2D_FORMAT_S16_RGB         0x00100 /* 01_00_0_00_0_00 */
//#define GE2D_FORMAT_S24_YUV444      0x20200 /* 10_00_0_00_0_00 */
//#define GE2D_FORMAT_S24_RGB         0x00200 /* 10_00_0_00_0_00 */
//#define GE2D_FORMAT_S32_YUVA444     0x20300 /* 11_00_0_00_0_00 */
//#define GE2D_FORMAT_S32_RGBA        0x00300 /* 11_00_0_00_0_00 */
//#define GE2D_FORMAT_M24_YUV420      0x20007 /* 00_00_0_00_1_11 */
//#define GE2D_FORMAT_M24_YUV422      0x20006 /* 00_00_0_00_1_10 */
//#define GE2D_FORMAT_M24_YUV444      0x20004 /* 00_00_0_00_1_00 */
//#define GE2D_FORMAT_M24_RGB         0x00004 /* 00_00_0_00_1_00 */
//#define GE2D_FORMAT_M24_YUV420T     0x20017 /* 00_00_0_10_1_11 */
//#define GE2D_FORMAT_M24_YUV420B     0x2001f /* 00_00_0_11_1_11 */
//#define GE2D_FORMAT_S16_YUV422T     0x20110 /* 01_00_0_10_0_00 */
//#define GE2D_FORMAT_S16_YUV422B     0x20138 /* 01_00_0_11_0_00 */
//#define GE2D_FORMAT_S24_YUV444T     0x20210 /* 10_00_0_10_0_00 */
//#define GE2D_FORMAT_S24_YUV444B     0x20218 /* 10_00_0_11_0_00 */
//#endif

void ge2dgen_src(int ge2d_handle,
                 unsigned canvas_addr,
                 unsigned format);

void ge2dgen_src_yuv(int ge2d_handle,
                 unsigned canvas_addr0,unsigned canvas_addr1,unsigned canvas_addr2,
                 unsigned format);

void ge2dgen_post_release_src1buf(int ge2d_handle, unsigned buffer);

void ge2dgen_post_release_src1canvas(int ge2d_handle);

void ge2dgen_post_release_src2buf(int ge2d_handle, unsigned buffer);

void ge2dgen_post_release_src2canvas(int ge2d_handle);

void ge2dgen_src2(int ge2d_handle,
                  unsigned canvas_addr,
                  unsigned format);

void ge2dgen_src2_clip(int ge2d_handle,
                      int x, int y, int w, int h);

void ge2dgen_rendering_dir(int ge2d_handle,
                           int src1_xrev,
                           int src1_yrev,
                           int dst_xrev,
                           int dst_yrev,
                           int dst_xy_swap);

void ge2dgen_dst(int ge2d_handle,
                 unsigned canvas_addr,
                 unsigned format);

void ge2dgen_src_clip(int ge2d_handle,
                      int x, int y, int w, int h);

void ge2dgen_src_key(int ge2d_handle,
                     int en, int key, int keymask);

void ge2dgent_src_gbalpha(int ge2d_handle,
                          unsigned char alpha);

void ge2dgen_src_color(int ge2d_handle,
                       unsigned color);

void ge2dgent_rendering_dir(int ge2d_handle,
                            int src_x_dir, int src_y_dir,
                            int dst_x_dir, int dst_y_dir);

void ge2dgen_src2(int ge2d_handle, unsigned canvas_addr, unsigned format);

void ge2dgen_dst_clip(int ge2d_handle,
                      int x, int y, int w, int h, int mode);

void ge2dgent_src2_clip(int ge2d_handle,
                        int x, int y, int w, int h);

void ge2dgen_cb(int ge2d_handle, int (*cmd_cb)(unsigned), unsigned param);

void ge2dgen_const_color(int ge2d_handle, unsigned color);

#endif /* GE2DGEN_H */

