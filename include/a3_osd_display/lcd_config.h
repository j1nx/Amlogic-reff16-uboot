#ifndef _LCD_CONIFG_H
#define _LCD_CONIFG_H
//#define LCD_WIDTH       1280
//#define LCD_HEIGHT      768
//#define MAX_WIDTH       1440
//#define MAX_HEIGHT      790
//#define VIDEO_ON_LINE   17
#define wr(reg, val) *(volatile unsigned *)(reg) = (val)
#define rd(reg) (*(volatile unsigned *)(reg))
   #define LCD_GAMMA_VCOM_POL       7
   
   /// GAMMA DATA REVERSE OUTPUT FOLLOWING VCOM
   #define LCD_GAMMA_RVS_OUT        6
   
   /// GAMMA ADDR PORT IS RDY
   #define LCD_ADR_RDY              5
   
   /// GAMMA DATA PORT IS RDY to Write
   #define LCD_WR_RDY               4
   
   /// GAMMA DATA PORT IS RDY to Read
   #define LCD_RD_RDY               3
   
   /// RGB10-->RGB8 using Trancate or Round off
   #define LCD_GAMMA_TR             2

   #define LCD_GAMMA_SET            1
   #define LCD_GAMMA_EN             0
/* for GAMMA_ADDR_PORT */
   /// Host Read/Write
   #define LCD_H_RD                 12
   
   /// Burst Mode
   #define LCD_H_AUTO_INC           11

   #define LCD_H_SEL_R              10
   #define LCD_H_SEL_G              9
   #define LCD_H_SEL_B              8
   
   /// 7:0
   #define LCD_HADR_MSB             7
   
   /// 7:0
   #define LCD_HADR                 0
/* for POL_CNTL_ADDR */
   /// FOR RGB format DVI output
   #define LCD_TCON_VSYNC_SEL_DVI   11
   
   /// FOR RGB format DVI output
   #define LCD_TCON_HSYNC_SEL_DVI   10
   
   /// FOR RGB format DVI output
   #define LCD_TCON_DE_SEL_DVI      9

   #define LCD_CPH3_POL             8
   #define LCD_CPH2_POL             7
   #define LCD_CPH1_POL             6
   #define LCD_TCON_DE_SEL          5
   #define LCD_TCON_VS_SEL          4
   #define LCD_TCON_HS_SEL          3
   #define LCD_DE_POL               2
   #define LCD_VS_POL               1
   #define LCD_HS_POL               0
/* for DITH_CNTL_ADDR */
   #define LCD_DITH10_EN            10
   #define LCD_DITH8_EN             9
   #define LCD_DITH_MD              8
   
   /// 7:4
   #define LCD_DITH10_CNTL_MSB      7
   
   /// 7:4
   #define LCD_DITH10_CNTL          4
   
   /// 3:0
   #define LCD_DITH8_CNTL_MSB       3
   
   /// 3:0
   #define LCD_DITH8_CNTL           0

/* for INV_CNT_ADDR */
   #define LCD_INV_EN               4
   #define LCD_INV_CNT_MSB          3
   #define LCD_INV_CNT              0
/* for TCON_MISC_SEL_ADDR */
   #define LCD_STH2_SEL             12
   #define LCD_STH1_SEL             11
   #define LCD_OEH_SEL              10
   #define LCD_VCOM_SEL             9
   #define LCD_DB_LINE_SW           8
   #define LCD_CPV2_SEL             7
   #define LCD_CPV1_SEL             6
   #define LCD_STV2_SEL             5
   #define LCD_STV1_SEL             4
   #define LCD_OEV_UNITE            3
   #define LCD_OEV3_SEL             2
   #define LCD_OEV2_SEL             1
   #define LCD_OEV1_SEL             0
/* for DUAL_PORT_CNTL_ADDR */
   #define LCD_ANALOG_SEL_CPH3      8
   #define LCD_ANALOG_3PHI_CLK_SEL  7
   #define LCD_LVDS_SEL54           6
   #define LCD_LVDS_SEL27           5
   #define LCD_TTL_SEL              4
   #define LCD_DUAL_PIXEL           3
   #define LCD_PORT_SWP             2
   #define LCD_RGB_SWP              1
   #define LCD_BIT_SWP              0
/* for LVDS_PACK_CNTL_ADDR */
   #define LCD_LD_CNT_MSB           7
   #define LCD_LD_CNT               5
   #define LCD_PN_SWP               4
   #define LCD_RES                  3
   #define LCD_LVDS_PORT_SWP        2
   #define LCD_PACK_RVS             1
   #define LCD_PACK_LITTLE          0
// lcd config flags
#define LCD_ANALOG              0
#define LCD_DIGITAL             1
#define LCD_SWAP_PbPr           2
#define LVDS_DIGITAL            4
#define LCD_DIGITAL_TTL			5
#define LCD_DIGITAL_LVDS		6
#define LCD_DIGITAL_mLVDS		7

//#ifndef TTL_LCD_CONFIG
typedef struct {
    int channel_num;
    int hv_sel;
    int tcon_1st_hs_addr;
    int tcon_1st_he_addr;
    int tcon_1st_vs_addr;
    int tcon_1st_ve_addr;
    int tcon_2nd_hs_addr;
    int tcon_2nd_he_addr;
    int tcon_2nd_vs_addr;
    int tcon_2nd_ve_addr;
}mlvds_tcon_config_t;
typedef struct {
    unsigned int lvds_prem_ctl;
    unsigned int lvds_swing_ctl;
    unsigned int lvds_vcm_ctl;
    unsigned int lvds_ref_ctl;    
}mlvds_phy_control_t;
typedef struct {
    int mlvds_insert_start;
    int total_line_clk;
    int test_dual_gate;
    int test_bit_num;
    int test_pair_num;
    int set_mlvds_pinmux;
    int phase_select;
    int TL080_phase;
    mlvds_tcon_config_t *mlvds_tcon_config;    //Point to TCON0~7
    mlvds_phy_control_t *mlvds_phy_control;
    int scan_function;
}mlvds_config_t;


//typedef struct {
////	unsigned short  GammaTable[256];
////	unsigned char  gamma_flag;		
//    unsigned short  width;
//    unsigned short  height;
//                    
//    unsigned short  max_width;
//    unsigned short  max_height;
//    unsigned short  video_on_line;
//                    
//    unsigned short  pll_ctrl;
//    unsigned short  clk_ctrl;
//                                        
//    unsigned short  gamma_cntl_port;
//    unsigned short  gamma_vcom_hswitch_addr;
//
//    unsigned short  rgb_base_addr;
//    unsigned short  rgb_coeff_addr;
//    unsigned short  pol_cntl_addr;
//    unsigned short  dith_cntl_addr;    
//                    
////    unsigned short  sth1_hs_addr;
////    unsigned short  sth1_he_addr;
////    unsigned short  sth1_vs_addr;
////    unsigned short  sth1_ve_addr;
////                                
////    unsigned short  sth2_hs_addr;
////    unsigned short  sth2_he_addr;
////    unsigned short  sth2_vs_addr;
////    unsigned short  sth2_ve_addr;
////                    
////    unsigned short  oeh_hs_addr;
////    unsigned short  oeh_he_addr;
////    unsigned short  oeh_vs_addr;
////    unsigned short  oeh_ve_addr;
////                                
////    unsigned short  vcom_hswitch_addr;
////    unsigned short  vcom_vs_addr;
////    unsigned short  vcom_ve_addr;
////                    
////    unsigned short  cpv1_hs_addr;
////    unsigned short  cpv1_he_addr;
////    unsigned short  cpv1_vs_addr;
////    unsigned short  cpv1_ve_addr;
////                                
////    unsigned short  cpv2_hs_addr;
////    unsigned short  cpv2_he_addr;
////    unsigned short  cpv2_vs_addr;
////    unsigned short  cpv2_ve_addr;
////                    
////    unsigned short  stv1_hs_addr;
////    unsigned short  stv1_he_addr;
////    unsigned short  stv1_vs_addr;
////    unsigned short  stv1_ve_addr;
////                                
////    unsigned short  stv2_hs_addr;
////    unsigned short  stv2_he_addr;
////    unsigned short  stv2_vs_addr;
////    unsigned short  stv2_ve_addr;
////                                
////    unsigned short  oev1_hs_addr;
////    unsigned short  oev1_he_addr;
////    unsigned short  oev1_vs_addr;
////    unsigned short  oev1_ve_addr;
////                                
////    unsigned short  oev2_hs_addr;
////    unsigned short  oev2_he_addr;
////    unsigned short  oev2_vs_addr;
////    unsigned short  oev2_ve_addr;
////                                
////    unsigned short  oev3_hs_addr;
////    unsigned short  oev3_he_addr;
////    unsigned short  oev3_vs_addr;
////    unsigned short  oev3_ve_addr;
//                                
//    unsigned short  inv_cnt_addr;                                    
//                    
//    unsigned short  tcon_misc_sel_addr;
//                    
//    unsigned short  dual_port_cntl_addr;
//    
//   unsigned short  flags; 
//        
//    unsigned short screen_width;
//    
//    unsigned short screen_height;
//    mlvds_config_t *mlvds_config;
//} lcdConfig_t;


//#else
typedef struct {
    unsigned short  width;
    unsigned short  height;                    
    unsigned short  max_width;
    unsigned short  max_height;
		unsigned short  video_on_pixel;
    unsigned short  video_on_line;
                    
    unsigned 		pll_ctrl;
    unsigned short	clk_ctrl;
                    
    unsigned short  gamma_cntl_port;
    unsigned short  gamma_vcom_hswitch_addr;

    unsigned short  rgb_base_addr;
    unsigned short  rgb_coeff_addr;
    unsigned short  pol_cntl_addr;
    unsigned short  dith_cntl_addr;     
    
    unsigned short  sth1_hs_addr;
    unsigned short  sth1_he_addr;
    unsigned short  sth1_vs_addr;
    unsigned short  sth1_ve_addr;
/*                                
    unsigned short  sth2_hs_addr;
    unsigned short  sth2_he_addr;
    unsigned short  sth2_vs_addr;
    unsigned short  sth2_ve_addr;
 */                   
    unsigned short  oeh_hs_addr;
    unsigned short  oeh_he_addr;
    unsigned short  oeh_vs_addr;
    unsigned short  oeh_ve_addr;
                                
    unsigned short  vcom_hswitch_addr;
    unsigned short  vcom_vs_addr;
    unsigned short  vcom_ve_addr;
                    
    unsigned short  cpv1_hs_addr;
    unsigned short  cpv1_he_addr;
    unsigned short  cpv1_vs_addr;
    unsigned short  cpv1_ve_addr;
/*                                
    unsigned short  cpv2_hs_addr;
    unsigned short  cpv2_he_addr;
    unsigned short  cpv2_vs_addr;
    unsigned short  cpv2_ve_addr;
 */                   
    unsigned short  stv1_hs_addr;
    unsigned short  stv1_he_addr;
    unsigned short  stv1_vs_addr;
    unsigned short  stv1_ve_addr;
/*                                
    unsigned short  stv2_hs_addr;
    unsigned short  stv2_he_addr;
    unsigned short  stv2_vs_addr;
    unsigned short  stv2_ve_addr;
 */                               
    unsigned short  oev1_hs_addr;
    unsigned short  oev1_he_addr;
    unsigned short  oev1_vs_addr;
    unsigned short  oev1_ve_addr;
/*                                
    unsigned short  oev2_hs_addr;
    unsigned short  oev2_he_addr;
    unsigned short  oev2_vs_addr;
    unsigned short  oev2_ve_addr;
                                
    unsigned short  oev3_hs_addr;
    unsigned short  oev3_he_addr;
    unsigned short  oev3_vs_addr;
    unsigned short  oev3_ve_addr;
*/                                
    unsigned short  inv_cnt_addr;                                    
                    
    unsigned short  tcon_misc_sel_addr;
                    
    unsigned short  dual_port_cntl_addr;
    
    unsigned short  flags;  
    
    unsigned short screen_width;
    
    unsigned short screen_height;

    mlvds_config_t *mlvds_config;
    
} lcdConfig_t;
//#endif
typedef struct {
	lcdConfig_t * lcd;
	void (* power_on_lcd)(void);
	void (* power_off_lcd)(void);
	void (* power_on_backlight)(void);
	void (* power_off_backlight)(void);
//	void (* enable_RGB_data_output)(void);
//	void (* disable_RGB_data_output)(void);
}LCDDesc;
extern LCDDesc lcd_platform_config;
extern lcdConfig_t *get_lcd_config1(void);
void osd_display(void);
extern lcdConfig_t *get_lcd_config1(void);
extern  int bmp_logo_display(unsigned long addr, int x, int y);
extern void set_lcd_display(LCDDesc *conf);
void generate_gamma_table_dlCD(void);
void disable_i2c_pinmux(void); 
#endif
