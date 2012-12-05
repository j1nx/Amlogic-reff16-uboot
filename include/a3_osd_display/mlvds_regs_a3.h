#ifndef __A3_MLVDS_REGS_H
#define __A3_MLVDS_REGS_H

#define MLVDS_TCON0 0
#define MLVDS_TCON1 1
#define MLVDS_TCON2 2
#define MLVDS_TCON3 3
#define MLVDS_TCON4 4
#define MLVDS_TCON5 5
#define MLVDS_TCON6 6
#define MLVDS_TCON7 7

#define Wr(addr, data) *(volatile unsigned long *)(0xc1100000|(addr<<2))=data
#define Rd(addr) *(volatile unsigned long *)(0xc1100000|(addr<<2))

#define PERIPHS_PIN_MUX_0                          0x202c

//the following register function is a little different as before
//but the address is same
//MTCON0-3 is full function, and MTCON4-7 is reduced.
#define MTCON0_1ST_HS_ADDR                         0x1490
#define MTCON0_1ST_HE_ADDR                         0x1491
#define MTCON0_1ST_VS_ADDR                         0x1492
#define MTCON0_1ST_VE_ADDR                         0x1493
#define MTCON0_2ND_HS_ADDR                         0x1494
#define MTCON0_2ND_HE_ADDR                         0x1495
#define MTCON0_2ND_VS_ADDR                         0x1496
#define MTCON0_2ND_VE_ADDR                         0x1497

#define MTCON1_1ST_HS_ADDR                         0x149f
#define MTCON1_1ST_HE_ADDR                         0x14a0
#define MTCON1_1ST_VS_ADDR                         0x14a1
#define MTCON1_1ST_VE_ADDR                         0x14a2
#define MTCON1_2ND_HS_ADDR                         0x14a3
#define MTCON1_2ND_HE_ADDR                         0x14a4
#define MTCON1_2ND_VS_ADDR                         0x14a5
#define MTCON1_2ND_VE_ADDR                         0x14a6

#define MTCON2_1ST_HS_ADDR                         0x14a7
#define MTCON2_1ST_HE_ADDR                         0x14a8
#define MTCON2_1ST_VS_ADDR                         0x14a9
#define MTCON2_1ST_VE_ADDR                         0x14aa
#define MTCON2_2ND_HS_ADDR                         0x14ab
#define MTCON2_2ND_HE_ADDR                         0x14ac
#define MTCON2_2ND_VS_ADDR                         0x14ad
#define MTCON2_2ND_VE_ADDR                         0x14ae

#define MTCON3_1ST_HS_ADDR                         0x14af
#define MTCON3_1ST_HE_ADDR                         0x14b0
#define MTCON3_1ST_VS_ADDR                         0x14b1
#define MTCON3_1ST_VE_ADDR                         0x14b2
#define MTCON3_2ND_HS_ADDR                         0x14b3
#define MTCON3_2ND_HE_ADDR                         0x14b4
#define MTCON3_2ND_VS_ADDR                         0x14b5
#define MTCON3_2ND_VE_ADDR                         0x14b6

#define MTCON4_1ST_HS_ADDR                         0x14d5
#define MTCON4_1ST_HE_ADDR                         0x14d6
#define MTCON4_1ST_VS_ADDR                         0x14d7
#define MTCON4_1ST_VE_ADDR                         0x14d8

#define MTCON5_1ST_HS_ADDR                         0x14d9
#define MTCON5_1ST_HE_ADDR                         0x14da
#define MTCON5_1ST_VS_ADDR                         0x14db
#define MTCON5_1ST_VE_ADDR                         0x14dc

#define MTCON6_1ST_HS_ADDR                         0x1498
#define MTCON6_1ST_HE_ADDR                         0x1499
#define MTCON6_1ST_VS_ADDR                         0x149a
#define MTCON6_1ST_VE_ADDR                         0x149b

#define MTCON7_1ST_HS_ADDR                         0x14b7
#define MTCON7_1ST_HE_ADDR                         0x14b8
#define MTCON7_1ST_VS_ADDR                         0x14b9
#define MTCON7_1ST_VE_ADDR                         0x14ba

#define MLVDS_CONTROL                              0x14c3
   #define     mLVDS_RESERVED  14    // 15:14
   #define     mLVDS_ins_reset  8    // 13:8  // each channel has one bit
   #define     mLVDS_dual_gate  7
   #define     mLVDS_bit_num    6    // 0-6Bits, 1-8Bits
   #define     mLVDS_pair_num   5    // 0-3Pairs, 1-6Pairs
   #define     mLVDS_msb_first  4
   #define     mLVDS_PORT_SWAP  3
   #define     mLVDS_MLSB_SWAP  2
   #define     mLVDS_PN_SWAP    1
   #define     mLVDS_en         0
#define MLVDS_RESET_PATTERN_HI                     0x14c4
#define MLVDS_RESET_PATTERN_LO                     0x14c5
   #define     mLVDS_reset_pattern  0 // Bit 47:16
#define MLVDS_RESET_PATTERN_EXT                    0x14c6
   #define     mLVDS_reset_pattern_ext  0 // Bit 15:0
#define MLVDS_CONFIG_HI                            0x14c7
#define MLVDS_CONFIG_LO                            0x14c8
   #define     mLVDS_reset_offset         29 // Bit 31:29 
   #define     mLVDS_reset_length         23 // Bit 28:23
   #define     mLVDS_config_reserved      19 // Bit 22:19
   #define     mLVDS_data_write_toggle    18
   #define     mLVDS_data_write_ini       17
   #define     mLVDS_data_latch_1_toggle  16
   #define     mLVDS_data_latch_1_ini     15
   #define     mLVDS_data_latch_0_toggle  14
   #define     mLVDS_data_latch_0_ini     13
   #define     mLVDS_reset_1_select       12 // 0 - same as reset_0, 1 - 1 clock delay of reset_0
   #define     mLVDS_reset_start           0 // Bit 11:0
#define TCON_DOUBLE_CTL                            0x14c9
   #define     tcon_double_ini          8 // Bit 7:0
   #define     tcon_double_inv          0 // Bit 7:0
#define TCON_PATTERN_HI                            0x14ca
#define TCON_PATTERN_LO                            0x14cb
   #define     tcon_pattern_loop_data     16 // Bit 15:0
   #define     tcon_pattern_loop_start    12 // Bit 3:0
   #define     tcon_pattern_loop_end       8 // Bit 3:0
   #define     tcon_pattern_enable         0 // Bit 7:0
#define TCON_CONTROL_HI                            0x14cc
#define TCON_CONTROL_LO                            0x14cd
   #define     tcon_pclk_enable           26 // Bit 5:0 (enable pclk on TCON channel 7 to 2)
   #define     tcon_pclk_div              24 // Bit 1:0 (control phy clok divide 2,4,6,8)
   #define     tcon_delay                  0 // Bit 23:0 (3 bit for each channel)
#define LVDS_BLANK_DATA_HI                         0x14ce
#define LVDS_BLANK_DATA_LO                         0x14cf
   #define     LVDS_blank_data_reserved 30  // 31:30
   #define     LVDS_blank_data_r        20  // 29:20
   #define     LVDS_blank_data_g        10  // 19:10
   #define     LVDS_blank_data_b         0  //  9:0
// #define LVDS_PACK_CNTL_ADDR                        0x14d0
   // #define     LVDS_USE_TCON    7
   // #define     LVDS_DUAL        6
   // #define     PN_SWP           5
   // #define     LSB_FIRST        4
   // #define     LVDS_RESV        3
   // #define     ODD_EVEN_SWP     2
   // #define     LVDS_REPACK      0
// #define DE_HS_ADDR                                 0x14d1
// #define DE_HE_ADDR                                 0x14d2
// #define DE_VS_ADDR                                 0x14d3
// #define DE_VE_ADDR                                 0x14d4
// #define HSYNC_HS_ADDR                              0x14d5
// #define HSYNC_HE_ADDR                              0x14d6
// #define HSYNC_VS_ADDR                              0x14d7
// #define HSYNC_VE_ADDR                              0x14d8
// #define VSYNC_HS_ADDR                              0x14d9
// #define VSYNC_HE_ADDR                              0x14da
// #define VSYNC_VS_ADDR                              0x14db
// #define VSYNC_VE_ADDR                              0x14dc
// bit 8 -- vfifo_mcu_enable
// bit 7 -- halt_vs_de
// bit 6 -- R8G8B8_format
// bit 5 -- R6G6B6_format (round to 6 bits)
// bit 4 -- R5G6B5_format
// bit 3 -- dac_dith_sel
// bit 2 -- lcd_mcu_enable_de     -- ReadOnly
// bit 1 -- lcd_mcu_enable_vsync  -- ReadOnly
// bit 0 -- lcd_mcu_enable
// #define LCD_MCU_CTL                                0x14dd
// ReadOnly 
//   R5G6B5 when R5G6B5_format
//   G8R8   when R8G8B8_format
//   G5R10  Other
// #define LCD_MCU_DATA_0                             0x14de
// ReadOnly
//   G8B8   when R8G8B8_format
//   G5B10  Other
// #define LCD_MCU_DATA_1                             0x14df
// LVDS
#define AML_A3
#ifdef AML_A3
#define ENCP_VIDEO_RGBIN_CTRL                      0x1bbd

#define HHI_MPEG_CLK_CNTL                          0x105d

#define PERIPHS_PIN_MUX_5                          0x2031
    
/********************
mLVDS

*********************/
#define REG_LVDS_PHY_CLK_CNTL                      0x14ef

#define LVDS_GEN_CNTL                              0x14e0
#define REG_LVDS_PHY_CNTL0                         0x14e1
#define REG_LVDS_PHY_CNTL1                         0x14e2
#define REG_LVDS_PHY_CNTL2                         0x14e3
#define REG_LVDS_PHY_CNTL3                         0x14e4
#define REG_LVDS_PHY_CNTL4                         0x14e5
#define REG_LVDS_PHY_CNTL5                         0x14e6
#define REG_LVDS_PHY_CNTL6                         0x14f1
#define REG_LVDS_PHY_CNTL7                         0x14f2
#define REG_LVDS_PHY_CNTL8                         0x14f3

#define LVDS_MDR_PU    REG_LVDS_PHY_CNTL4
#define LVDS_COMMON    REG_LVDS_PHY_CNTL3

#endif

#define LVDS_GEN_CNTL                              0x14e0
#define LVDS_PHY_CNTL0                             0x14e1
#define LVDS_PHY_CNTL1                             0x14e2
#define LVDS_PHY_CNTL2                             0x14e3
#define LVDS_PHY_CNTL3                             0x14e4
#define LVDS_PHY_CNTL4                             0x14e5
#define LVDS_PHY_CNTL5                             0x14e6
#define LVDS_SRG_TEST                              0x14e8
#define LVDS_BIST_MUX0                             0x14e9
#define LVDS_BIST_MUX1                             0x14ea
#define LVDS_BIST_FIXED0                           0x14eb
#define LVDS_BIST_FIXED1                           0x14ec
#define LVDS_BIST_CNTL0                            0x14ed
#define LVDS_CLKB_CLKA                             0x14ee
#define LVDS_PHY_CLK_CNTL                          0x14ef
#define LVDS_SER_EN                                0x14f0
#define LVDS_PHY_CNTL6                             0x14f1
#define LVDS_PHY_CNTL7                             0x14f2
#define LVDS_PHY_CNTL8                             0x14f3
#define MLVDS_CLK_CTL_HI                           0x14f4
#define MLVDS_CLK_CTL_LO                           0x14f5
   #define     mlvds_clk_pattern_reserved 31 // Bit 31
   #define     mpclk_dly                  28 // Bit 2:0 
   #define     mpclk_div                  26 // Bit 1:0 (control phy clok divide 2,4,6,8)
   #define     use_mpclk                  25 // Bit 0
   #define     mlvds_clk_half_delay       24 // Bit 0
   #define     mlvds_clk_pattern           0 // Bit 23:0
#define MLVDS_DUAL_GATE_WR_START                   0x14f6
   #define     mlvds_dual_gate_wr_start    0 // Bit 11:0
#define MLVDS_DUAL_GATE_WR_END                     0x14f7
   #define     mlvds_dual_gate_wr_end      0 // Bit 11:0
#define MLVDS_DUAL_GATE_RD_START                   0x14f8
   #define     mlvds_dual_gate_rd_start    0 // Bit 11:0
#define MLVDS_DUAL_GATE_RD_END                     0x14f9
   #define     mlvds_dual_gate_rd_end      0 // Bit 11:0
#define MLVDS_SECOND_RESET_CTL                     0x14fa
   #define     mLVDS_2ND_reset_start       0 // Bit 11:0
#define MLVDS_DUAL_GATE_CTL_HI                     0x14fb
#define MLVDS_DUAL_GATE_CTL_LO                     0x14fc
   #define     mlvds_tcon_field_en        24 // Bit 7:0
   #define     mlvds_dual_gate_reserved   20 // Bit 3:0
   #define     mlvds_scan_mode_odd        16 // Bit 3:0
   #define     mlvds_scan_mode_even       12 // Bit 3:0
   #define     mlvds_scan_mode_start_line  0 // Bit 11:0 
#endif

