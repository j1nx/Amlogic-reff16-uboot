#ifndef _GE2D_H_
#define _GE2D_H_

#define MAX_GE2D_CMD 32

#define GE2D_STATE_IDLE          0
#define GE2D_STATE_RUNNING       1
#define GE2D_STATE_CLEANUP       2

#define RELEASE_SRC1_CANVAS 0x01
#define RELEASE_SRC2_CANVAS 0x02
#define RELEASE_SRC1_BUFFER 0x04
#define RELEASE_SRC2_BUFFER 0x08
#define RELEASE_CB          0x10
#define RELEASE_REQUIRED    0x1f

#define START_FLAG          0x20
#define RELEASE_FLAG        0x40
#define FINISH_FLAG         0x80

#define FORMAT_8BIT_COMPONENT   0
    #define COMPONENT_Y_OR_R      0
    #define COMPONENT_Cb_OR_G     1
    #define COMPONENT_Cr_OR_B     2
    #define COMPONENT_ALPHA       3
#define FORMAT_422_YUV          1
#define FORMAT_444_YUV_OR_RGB   2
#define FORMAT_YUVA_OR_RGBA     3

#define FILL_MODE_BOUNDARY_PIXEL    0
#define FILL_MODE_DEFAULT_COLOR     1

#define OPERATION_ADD           0    //Cd = Cs*Fs+Cd*Fd
#define OPERATION_SUB           1    //Cd = Cs*Fs-Cd*Fd
#define OPERATION_REVERSE_SUB   2    //Cd = Cd*Fd-Cs*Fs
#define OPERATION_MIN           3    //Cd = Min(Cd*Fd,Cs*Fs)
#define OPERATION_MAX           4    //Cd = Max(Cd*Fd,Cs*Fs)
#define OPERATION_LOGIC         5

#define COLOR_FACTOR_ZERO                     0
#define COLOR_FACTOR_ONE                      1
#define COLOR_FACTOR_SRC_COLOR                2
#define COLOR_FACTOR_ONE_MINUS_SRC_COLOR      3
#define COLOR_FACTOR_DST_COLOR                4
#define COLOR_FACTOR_ONE_MINUS_DST_COLOR      5
#define COLOR_FACTOR_SRC_ALPHA                6
#define COLOR_FACTOR_ONE_MINUS_SRC_ALPHA      7
#define COLOR_FACTOR_DST_ALPHA                8
#define COLOR_FACTOR_ONE_MINUS_DST_ALPHA      9
#define COLOR_FACTOR_CONST_COLOR              10
#define COLOR_FACTOR_ONE_MINUS_CONST_COLOR    11
#define COLOR_FACTOR_CONST_ALPHA              12
#define COLOR_FACTOR_ONE_MINUS_CONST_ALPHA    13
#define COLOR_FACTOR_SRC_ALPHA_SATURATE       14

#define ALPHA_FACTOR_ZERO                     0
#define ALPHA_FACTOR_ONE                      1
#define ALPHA_FACTOR_SRC_ALPHA                2
#define ALPHA_FACTOR_ONE_MINUS_SRC_ALPHA      3
#define ALPHA_FACTOR_DST_ALPHA                4
#define ALPHA_FACTOR_ONE_MINUS_DST_ALPHA      5
#define ALPHA_FACTOR_CONST_ALPHA              6
#define ALPHA_FACTOR_ONE_MINUS_CONST_ALPHA    7

#define LOGIC_OPERATION_CLEAR       0
#define LOGIC_OPERATION_COPY        1
#define LOGIC_OPERATION_NOOP        2
#define LOGIC_OPERATION_SET         3
#define LOGIC_OPERATION_COPY_INVERT 4
#define LOGIC_OPERATION_INVERT      5
#define LOGIC_OPERATION_AND_REVERSE 6
#define LOGIC_OPERATION_OR_REVERSE  7
#define LOGIC_OPERATION_AND         8
#define LOGIC_OPERATION_OR          9
#define LOGIC_OPERATION_NAND        10
#define LOGIC_OPERATION_NOR         11
#define LOGIC_OPERATION_XOR         12
#define LOGIC_OPERATION_EQUIV       13
#define LOGIC_OPERATION_AND_INVERT  14
#define LOGIC_OPERATION_OR_INVERT   15 

#define DST_CLIP_MODE_INSIDE    0
#define DST_CLIP_MODE_OUTSIDE   1

#define FILTER_TYPE_BICUBIC     1
#define FILTER_TYPE_BILINEAR    2
#define FILTER_TYPE_TRIANGLE    3

#define MATRIX_YCC_TO_RGB               1
#define MATRIX_RGB_TO_YCC               2
#define MATRIX_FULL_RANGE_YCC_TO_RGB    3

typedef struct {
    unsigned char     interrupt_ctrl;
    
    unsigned char     dp_onoff_mode;
    unsigned char     vfmt_onoff_en;
    unsigned int      dp_on_cnt;
    unsigned int      dp_off_cnt;
} ge2d_gen_t;


typedef struct {
    unsigned char     urgent_en;
    unsigned char     ddr_burst_size_y;
    unsigned char     ddr_burst_size_cb;
    unsigned char     ddr_burst_size_cr;
	unsigned		  canaddr;
    unsigned char     x_yc_ratio;
    unsigned char     y_yc_ratio;
    unsigned char     sep_en;
    unsigned char     format;
//#if defined(AML_A1H)
	unsigned char     endian;
	unsigned char     color_map;
//#endif    
    unsigned char     mode_8b_sel;
    unsigned char     lut_en;
    unsigned int      def_color;
    unsigned int      format_all;
} ge2d_src1_data_t;

typedef struct {
    int               clipx_start;
    int               clipx_end;
    int               clipy_start;
    int               clipy_end;
    unsigned char     clipx_start_ex;
    unsigned char     clipx_end_ex;
    unsigned char     clipy_start_ex;
    unsigned char     clipy_end_ex;
    unsigned char     pic_struct;
    unsigned char     fill_mode; //bit1 for outside alpha , bit0 for color data
    unsigned int      outside_alpha;
    unsigned char     chfmt_rpt_pix;
    unsigned char     cvfmt_rpt_pix;
} ge2d_src1_gen_t;

typedef struct {
    unsigned char     urgent_en;
    unsigned char     ddr_burst_size;
    unsigned char     src2_canaddr;
    unsigned char     src2_format;
//#if defined(AML_A1H)
	unsigned char     src2_endian;
	unsigned char     src2_color_map;
//#endif    
    unsigned char     src2_mode_8b_sel;
    unsigned int      src2_def_color;

    unsigned char     dst_canaddr;
    unsigned char     dst_format;
//#if defined(AML_A1H)
	unsigned char     dst_endian;
	unsigned char     dst_color_map;
//#endif    
    unsigned char     dst_mode_8b_sel;

    unsigned int      src2_format_all;
    unsigned int      dst_format_all;
} ge2d_src2_dst_data_t;


typedef struct {
    int               src2_clipx_start;
    int               src2_clipx_end;
    int               src2_clipy_start;
    int               src2_clipy_end;
    unsigned char     src2_pic_struct;
    unsigned char     src2_fill_mode; //bit1 for outside alpha , bit0 for color data
    unsigned int      src2_outside_alpha;

    int               dst_clipx_start;
    int               dst_clipx_end;
    int               dst_clipy_start;
    int               dst_clipy_end;
    unsigned char     dst_clip_mode;
    unsigned char     dst_pic_struct;
} ge2d_src2_dst_gen_t;


typedef struct {
    //scaler related
    unsigned char     src1_vsc_bank_length;
    unsigned char     src1_vsc_phase0_always_en;
    unsigned char     src1_hsc_bank_length;
    unsigned char     src1_hsc_phase0_always_en;
    unsigned char     src1_hsc_rpt_ctrl;  //1bit, 0: using minus, 1: using repeat data
    unsigned char     src1_vsc_rpt_ctrl;  //1bit, 0: using minus  1: using repeat data
    unsigned char     src1_hsc_nearest_en; 
    unsigned char     src1_vsc_nearest_en; 
    
    //matrix related
    unsigned char     use_matrix_default;
    unsigned char     conv_matrix_en;
    unsigned char     matrix_sat_in_en;     
    unsigned char     matrix_minus_16_ctrl; //3bit 
    unsigned char     matrix_sign_ctrl;     //3bit 
    int               matrix_offset[3];
    int               matrix_coef[9];
    
    unsigned char     src1_gb_alpha;
    unsigned int      alu_const_color;

    unsigned char     src1_key_en;
    unsigned char     src2_key_en;
    unsigned char     src1_key_mode;
    unsigned char     src2_key_mode;
    unsigned int      src1_key;
    unsigned int      src2_key;
    unsigned int      src1_key_mask;
    unsigned int      src2_key_mask;
    unsigned char     bitmask_en;
    unsigned char     bytemask_only;
    unsigned int      bitmask;

} ge2d_dp_gen_t;

typedef struct {
    int              src1_x_start;
    int              src1_y_start;
    int              src1_x_end;
    int              src1_y_end;
   
    unsigned char    src1_x_rev;
    unsigned char    src1_y_rev;    
    unsigned char    src1_fill_color_en;
     
    int              src2_x_start;
    int              src2_y_start;
    int              src2_x_end;
    int              src2_y_end;
    unsigned char    src2_x_rev;
    unsigned char    src2_y_rev;
    unsigned char    src2_fill_color_en;

    int              dst_x_start;
    int              dst_y_start;
    int              dst_x_end;
    int              dst_y_end;
    unsigned char    dst_xy_swap;
    unsigned char    dst_x_rev;
    unsigned char    dst_y_rev;

    int              sc_prehsc_en;
    int              sc_prevsc_en;
    int              sc_hsc_en;
    int              sc_vsc_en;
    int              vsc_phase_step;
    int              vsc_phase_slope;
    unsigned char    vsc_rpt_l0_num;
    int              vsc_ini_phase;
    int              hsc_phase_step;
    int              hsc_phase_slope;
    unsigned char    hsc_rpt_p0_num;
    int              hsc_ini_phase;
    unsigned char    hsc_div_en; 
    unsigned char    hsc_div_length;
    int              hsc_adv_num;
    int              hsc_adv_phase;

    unsigned char    src1_cmult_asel;
    unsigned char    src2_cmult_asel;
    
    unsigned char    color_blend_mode;
    unsigned char    color_src_blend_factor;        
    unsigned char    color_dst_blend_factor;        
    unsigned char    color_logic_op;
    
    unsigned char    alpha_blend_mode;
    unsigned char    alpha_src_blend_factor;        
    unsigned char    alpha_dst_blend_factor;        
    unsigned char    alpha_logic_op;
    
    int (*cmd_cb)(unsigned);
    unsigned int     cmd_cb_param;
    unsigned int     src1_buffer;
    unsigned int     src2_buffer;
    unsigned char    release_flag;
    unsigned char    wait_done_flag;
} ge2d_cmd_t;

typedef struct {
    ge2d_gen_t            gen;
    ge2d_src1_data_t      src1_data;
    ge2d_src1_gen_t       src1_gen;
    ge2d_src2_dst_data_t  src2_dst_data;
    ge2d_src2_dst_gen_t   src2_dst_gen;
    ge2d_dp_gen_t         dp_gen;
    unsigned              v_scale_coef_type; 
    unsigned              h_scale_coef_type; 
    unsigned              update_flag;
} ge2d_config_t;

extern void ge2d_set_src1_data(ge2d_src1_data_t *cfg);

extern void ge2d_set_src1_gen(ge2d_src1_gen_t *cfg);

extern void ge2d_set_src2_dst_data(ge2d_src2_dst_data_t *cfg);

extern void ge2d_set_src2_dst_gen(ge2d_src2_dst_gen_t *cfg);

extern void ge2d_set_dp_gen(ge2d_dp_gen_t *cfg);

extern void ge2d_set_cmd(ge2d_cmd_t *cfg);

extern void ge2d_wait_done(void);

extern void ge2d_set_src1_scale_coef(unsigned v_filt_type, unsigned h_filt_type);

extern void ge2d_set_gen(ge2d_gen_t * cfg);

extern void ge2d_soft_rst(void);

extern int ge2d_is_busy (void);

extern int ge2d_cmd_fifo_full(void);

#endif

