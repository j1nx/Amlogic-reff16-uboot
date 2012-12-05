/*******************************************************************
 * 
 *  Copyright C 2005 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: 
 *
 *  Author: Amlogic Software
 *  Created: Tue Nov 15 20:52:43 2005
 *
 *******************************************************************/

#ifndef VENC_REG_APOLLO_H
#define VENC_REG_APOLLO_H

//===========================================================================
// Video Interface Registers    0xa00 - 0xbff
//===========================================================================
// -----------------------------------------------
// CBUS_BASE:  VENC_CBUS_BASE = 0x1b
// -----------------------------------------------
#define VENC_CBUS_BASE  0x1b00

//===========================================================================
// vfifo2vd module is used to convert 4:4:4 vfifo interface (30 bits) to 4:2:2 CCIR601 like (8Bits) interface
// Interlace TV Encoder Input, CCIR601/656 Output will use this interface  
//===========================================================================
// bit 15:8 -- vfifo2vd_vd_sel
//             Output will Repeat {D0, D1, D2, D3}
//             D0 will select by bit[15:14]
//             D1 will select by bit[13:12]
//             D2 will select by bit[11:10]
//             D3 will select by bit[9:8]
//             2-bits sel : 00-Y0, 01-Cb, 10-Y1, 11-Cr
//
// bit 7 -- vfifo2vd_drop
//          During 4:4:4->4:2:2 convert, when set to 1, will drop CbCr of second pixel instead average them.
//
// bit 6:1 -- vfifo2vd_delay 
//            bit[6:5] will delay Y from 0 - 3 pixel
//            bit[4:3] will delay Cb from 0 - 3 pixel
//            bit[2:1] will delay Cr from 0 - 3 pixel
//
// bit 0 -- vfifo2vd_en
//          Enable vfifo2vd interface  
#define VFIFO2VD_CTL                               0x1b58
//
// The following Registers define the display area 
// that is when it will pop data from vfifo 
//
// bit 12:0 -- vfifo2vd_pixel_start 
#define VFIFO2VD_PIXEL_START                       0x1b59
// bit 12:00 -- vfifo2vd_pixel_end   
#define VFIFO2VD_PIXEL_END                         0x1b5a
// bit 10:0 -- vfifo2vd_line_top_start 
#define VFIFO2VD_LINE_TOP_START                    0x1b5b
// bit 10:00 -- vfifo2vd_line_top_end   
#define VFIFO2VD_LINE_TOP_END                      0x1b5c
// bit 10:00 -- vfifo2vd_line_bot_start 
#define VFIFO2VD_LINE_BOT_START                    0x1b5d
// bit 10:00 -- vfifo2vd_line_bot_end   
#define VFIFO2VD_LINE_BOT_END                      0x1b5e
// Route the hsync and vsync signals round the chip. There are three
// sources and users of these signals: VIU, internal video encoder, and
// the pins on the chip. Some muxing is still being done in the VIU. It
// was not moved to the venc module so that the same exact VIU code could
// be used both in Twister and Twister2000.
// Bit 2: venc_sync_source (1=>pins, 0=>viu)
// Bit 1: viu_sync_source (1=>pins, 0=>venc)
// Bit 0: vpins_sync_source (1=>venc, 0=>viu)

#define	VENC_SYNC_ROUTE				(VENC_CBUS_BASE+0x60)
#define VENC_VIDEO_EXSRC			(VENC_CBUS_BASE+0x61)
#define	VENC_DVI_SETTING			(VENC_CBUS_BASE+0x62)
#define VENC_C656_CTRL          	(VENC_CBUS_BASE+0x63)
#define	VENC_UPSAMPLE_CTRL0  		(VENC_CBUS_BASE+0x64)
#define	VENC_UPSAMPLE_CTRL1  		(VENC_CBUS_BASE+0x65)
#define	VENC_UPSAMPLE_CTRL2  		(VENC_CBUS_BASE+0x66)
// Invert control for tcon output
// bit[15:14] -- vsync, hsync, 
// bit[13:0] --  oev3, oev2, cpv2, cph3, cph2, cph1, oeh, vcom, stv2, stv1, cpv1, oev1, sth1, sth2
#define TCON_INVERT_CTL             (VENC_CBUS_BASE+0x67)
#define VENC_VIDEO_PROG_MODE		(VENC_CBUS_BASE+0x68)
#define	VENC_ENCI_LINE				(VENC_CBUS_BASE+0x69)
#define	VENC_ENCI_PIXEL				(VENC_CBUS_BASE+0x6a)
#define	VENC_ENCP_LINE				(VENC_CBUS_BASE+0x6b)
#define	VENC_ENCP_PIXEL				(VENC_CBUS_BASE+0x6c)
#define	VENC_STATA					(VENC_CBUS_BASE+0x6d)
#define	VENC_INTCTRL				(VENC_CBUS_BASE+0x6e)
#define	VENC_INTFLAG				(VENC_CBUS_BASE+0x6f)

//--------- Video test configuration
#define VENC_VIDEO_TST_EN           (VENC_CBUS_BASE+0x70)
#define VENC_VIDEO_TST_MDSEL        (VENC_CBUS_BASE+0x71)
#define VENC_VIDEO_TST_Y            (VENC_CBUS_BASE+0x72)
#define VENC_VIDEO_TST_CB           (VENC_CBUS_BASE+0x73)
#define VENC_VIDEO_TST_CR           (VENC_CBUS_BASE+0x74)
#define VENC_VIDEO_TST_CLRBAR_STRT  (VENC_CBUS_BASE+0x75)
#define VENC_VIDEO_TST_CLRBAR_WIDTH (VENC_CBUS_BASE+0x76)
#define VENC_VIDEO_TST_VDCNT_STSET  (VENC_CBUS_BASE+0x77)

#define	VENC_VDAC_DACSEL0			(VENC_CBUS_BASE+0x78)
#define	VENC_VDAC_DACSEL1			(VENC_CBUS_BASE+0x79)
#define	VENC_VDAC_DACSEL2			(VENC_CBUS_BASE+0x7a)
#define	VENC_VDAC_DACSEL3			(VENC_CBUS_BASE+0x7b)
#define	VENC_VDAC_DACSEL4			(VENC_CBUS_BASE+0x7c)
#define	VENC_VDAC_DACSEL5			(VENC_CBUS_BASE+0x7d)
#define	VENC_VDAC_SETTING			(VENC_CBUS_BASE+0x7e)
#define	VENC_VDAC_TST_VAL			(VENC_CBUS_BASE+0x7f)

/*
//===========================================================================
// Video Interface Registers    0xb00 - 0xb5f
//===========================================================================
*/
#define	ENCI_ADDR_START			(VENC_CBUS_BASE+0x00)
#define	ENCI_ADDR_END			(VENC_CBUS_BASE+0x5f)
#define	ENCI_VIDEO_MODE			(VENC_CBUS_BASE+0x00)
#define	ENCI_VIDEO_MODE_ADV		(VENC_CBUS_BASE+0x01)
#define	ENCI_VIDEO_FSC_ADJ		(VENC_CBUS_BASE+0x02)
#define	ENCI_VIDEO_BRIGHT		(VENC_CBUS_BASE+0x03)
#define	ENCI_VIDEO_CONT			(VENC_CBUS_BASE+0x04)
#define	ENCI_VIDEO_SAT			(VENC_CBUS_BASE+0x05)
#define	ENCI_VIDEO_HUE			(VENC_CBUS_BASE+0x06)
#define	ENCI_VIDEO_SCH			(VENC_CBUS_BASE+0x07)
#define	ENCI_SYNC_MODE			(VENC_CBUS_BASE+0x08)
#define	ENCI_SYNC_CTRL			(VENC_CBUS_BASE+0x09)
#define	ENCI_SYNC_HSO_BEGIN		(VENC_CBUS_BASE+0x0a)
#define	ENCI_SYNC_HSO_END		(VENC_CBUS_BASE+0x0b)
#define	ENCI_SYNC_VSO_EVN		(VENC_CBUS_BASE+0x0c)
#define	ENCI_SYNC_VSO_ODD		(VENC_CBUS_BASE+0x0d)
#define	ENCI_SYNC_VSO_EVNLN		(VENC_CBUS_BASE+0x0e)
#define	ENCI_SYNC_VSO_ODDLN		(VENC_CBUS_BASE+0x0f)
#define	ENCI_SYNC_HOFFST		(VENC_CBUS_BASE+0x10)
#define	ENCI_SYNC_VOFFST		(VENC_CBUS_BASE+0x11)
#define	ENCI_SYNC_ADJ			(VENC_CBUS_BASE+0x12)
#define	ENCI_RGB_SETTING		(VENC_CBUS_BASE+0x13)
#define ENCI_DE_H_BEGIN         (VENC_CBUS_BASE+0x16)
#define ENCI_DE_H_END           (VENC_CBUS_BASE+0x17)
#define ENCI_DE_V_BEGIN_EVEN    (VENC_CBUS_BASE+0x18)
#define ENCI_DE_V_END_EVEN      (VENC_CBUS_BASE+0x19)
#define ENCI_DE_V_BEGIN_ODD     (VENC_CBUS_BASE+0x1a)
#define ENCI_DE_V_END_ODD       (VENC_CBUS_BASE+0x1b)
#define	ENCI_VBI_SETTING		(VENC_CBUS_BASE+0x20)
#define	ENCI_VBI_CCDT_EVN		(VENC_CBUS_BASE+0x21)
#define	ENCI_VBI_CCDT_ODD		(VENC_CBUS_BASE+0x22)
#define	ENCI_VBI_CC525_LN		(VENC_CBUS_BASE+0x23)
#define	ENCI_VBI_CC625_LN		(VENC_CBUS_BASE+0x24)
#define	ENCI_VBI_WSSDT			(VENC_CBUS_BASE+0x25)
#define	ENCI_VBI_WSS_LN			(VENC_CBUS_BASE+0x26)
#define	ENCI_VBI_CGMSDT_L		(VENC_CBUS_BASE+0x27)
#define	ENCI_VBI_CGMSDT_H		(VENC_CBUS_BASE+0x28)
#define	ENCI_VBI_CGMS_LN		(VENC_CBUS_BASE+0x29)
#define	ENCI_VBI_TTX_HTIME		(VENC_CBUS_BASE+0x2a)
#define	ENCI_VBI_TTX_LN   		(VENC_CBUS_BASE+0x2b)
#define	ENCI_VBI_TTXDT0   		(VENC_CBUS_BASE+0x2c)
#define	ENCI_VBI_TTXDT1   		(VENC_CBUS_BASE+0x2d)
#define	ENCI_VBI_TTXDT2   		(VENC_CBUS_BASE+0x2e)
#define	ENCI_VBI_TTXDT3 	   	(VENC_CBUS_BASE+0x2f)
#define	ENCI_MACV_N0			(VENC_CBUS_BASE+0x30)
#define	ENCI_MACV_N1			(VENC_CBUS_BASE+0x31)
#define	ENCI_MACV_N2			(VENC_CBUS_BASE+0x32)
#define	ENCI_MACV_N3			(VENC_CBUS_BASE+0x33)
#define	ENCI_MACV_N4			(VENC_CBUS_BASE+0x34)
#define	ENCI_MACV_N5			(VENC_CBUS_BASE+0x35)
#define	ENCI_MACV_N6			(VENC_CBUS_BASE+0x36)
#define	ENCI_MACV_N7			(VENC_CBUS_BASE+0x37)
#define	ENCI_MACV_N8			(VENC_CBUS_BASE+0x38)
#define	ENCI_MACV_N9			(VENC_CBUS_BASE+0x39)
#define	ENCI_MACV_N10			(VENC_CBUS_BASE+0x3a)
#define	ENCI_MACV_N11			(VENC_CBUS_BASE+0x3b)
#define	ENCI_MACV_N12			(VENC_CBUS_BASE+0x3c)
#define	ENCI_MACV_N13			(VENC_CBUS_BASE+0x3d)
#define	ENCI_MACV_N14			(VENC_CBUS_BASE+0x3e)
#define	ENCI_MACV_N15			(VENC_CBUS_BASE+0x3f)
#define	ENCI_MACV_N16			(VENC_CBUS_BASE+0x40)
#define	ENCI_MACV_N17			(VENC_CBUS_BASE+0x41)
#define	ENCI_MACV_N18			(VENC_CBUS_BASE+0x42)
#define	ENCI_MACV_N19			(VENC_CBUS_BASE+0x43)
#define	ENCI_MACV_N20			(VENC_CBUS_BASE+0x44)
#define	ENCI_MACV_N21			(VENC_CBUS_BASE+0x45)
#define	ENCI_MACV_N22			(VENC_CBUS_BASE+0x46)
#define	ENCI_DBG_PX_RST			(VENC_CBUS_BASE+0x48)
#define	ENCI_DBG_FLDLN_RST		(VENC_CBUS_BASE+0x49)
#define	ENCI_DBG_PX_INT			(VENC_CBUS_BASE+0x4a)
#define	ENCI_DBG_FLDLN_INT		(VENC_CBUS_BASE+0x4b)
#define	ENCI_DBG_MAXPX			(VENC_CBUS_BASE+0x4c)
#define	ENCI_DBG_MAXLN			(VENC_CBUS_BASE+0x4d)
#define ENCI_MACV_MAX_AMP		(VENC_CBUS_BASE+0x50)
#define ENCI_MACV_PULSE_LO		(VENC_CBUS_BASE+0x51)
#define ENCI_MACV_PULSE_HI		(VENC_CBUS_BASE+0x52)
#define ENCI_MACV_BKP_MAX		(VENC_CBUS_BASE+0x53)
#define ENCI_CFILT_CTRL			(VENC_CBUS_BASE+0x54)
#define ENCI_CFILT7				(VENC_CBUS_BASE+0x55)
#define ENCI_YC_DELAY			(VENC_CBUS_BASE+0x56)
#define ENCI_VIDEO_EN			(VENC_CBUS_BASE+0x57)


/*
//===========================================================================
// Video Encoder 480p Registers    0xb80 - 0xbbf
//===========================================================================
*/

#define ENCP_ADDR_START      			(VENC_CBUS_BASE+0x80)
#define ENCP_ADDR_END        			(VENC_CBUS_BASE+0xdf)
//-------- Video basic setting
#define ENCP_VIDEO_EN					(VENC_CBUS_BASE+0x80)
#define ENCP_VIDEO_SYNC_MODE			(VENC_CBUS_BASE+0x81)
#define ENCP_MACV_EN					(VENC_CBUS_BASE+0x82)
#define ENCP_VIDEO_Y_SCL				(VENC_CBUS_BASE+0x83)
#define ENCP_VIDEO_PB_SCL				(VENC_CBUS_BASE+0x84)
#define ENCP_VIDEO_PR_SCL				(VENC_CBUS_BASE+0x85)
#define ENCP_VIDEO_SYNC_SCL				(VENC_CBUS_BASE+0x86)
#define ENCP_VIDEO_MACV_SCL				(VENC_CBUS_BASE+0x87)
#define ENCP_VIDEO_Y_OFFST   			(VENC_CBUS_BASE+0x88)
#define ENCP_VIDEO_PB_OFFST  			(VENC_CBUS_BASE+0x89)
#define ENCP_VIDEO_PR_OFFST  			(VENC_CBUS_BASE+0x8a)
#define ENCP_VIDEO_SYNC_OFFST			(VENC_CBUS_BASE+0x8b)
#define ENCP_VIDEO_MACV_OFFST			(VENC_CBUS_BASE+0x8c)
//----- Video mode
#define ENCP_VIDEO_MODE					(VENC_CBUS_BASE+0x8d) //<
#define ENCP_VIDEO_MODE_ADV				(VENC_CBUS_BASE+0x8e) //<
//--------------- Debug pins
#define	ENCP_DBG_PX_RST					(VENC_CBUS_BASE+0x90)
#define	ENCP_DBG_LN_RST					(VENC_CBUS_BASE+0x91)
#define	ENCP_DBG_PX_INT					(VENC_CBUS_BASE+0x92)
#define	ENCP_DBG_LN_INT					(VENC_CBUS_BASE+0x93)
//----------- Video Advanced setting
#define ENCP_VIDEO_YFP1_HTIME			(VENC_CBUS_BASE+0x94)
#define ENCP_VIDEO_YFP2_HTIME			(VENC_CBUS_BASE+0x95)
#define ENCP_VIDEO_YC_DLY				(VENC_CBUS_BASE+0x96)
#define ENCP_VIDEO_MAX_PXCNT			(VENC_CBUS_BASE+0x97)
#define ENCP_VIDEO_HSPULS_BEGIN			(VENC_CBUS_BASE+0x98)
#define ENCP_VIDEO_HSPULS_END			(VENC_CBUS_BASE+0x99)
#define ENCP_VIDEO_HSPULS_SWITCH		(VENC_CBUS_BASE+0x9a) //<
#define ENCP_VIDEO_VSPULS_BEGIN			(VENC_CBUS_BASE+0x9b)	//<
#define ENCP_VIDEO_VSPULS_END			(VENC_CBUS_BASE+0x9c) //<
#define ENCP_VIDEO_VSPULS_BLINE			(VENC_CBUS_BASE+0x9d)
#define ENCP_VIDEO_VSPULS_ELINE			(VENC_CBUS_BASE+0x9e)
#define ENCP_VIDEO_EQPULS_BEGIN			(VENC_CBUS_BASE+0x9f) //<
#define ENCP_VIDEO_EQPULS_END			(VENC_CBUS_BASE+0xa0) //<
#define ENCP_VIDEO_EQPULS_BLINE			(VENC_CBUS_BASE+0xa1) //<
#define ENCP_VIDEO_EQPULS_ELINE			(VENC_CBUS_BASE+0xa2) //<
#define ENCP_VIDEO_HAVON_END			(VENC_CBUS_BASE+0xa3)
#define ENCP_VIDEO_HAVON_BEGIN			(VENC_CBUS_BASE+0xa4)
#define ENCP_VIDEO_VAVON_ELINE			(VENC_CBUS_BASE+0xaf)
#define ENCP_VIDEO_VAVON_BLINE			(VENC_CBUS_BASE+0xa6)
#define ENCP_VIDEO_HSO_BEGIN			(VENC_CBUS_BASE+0xa7)
#define ENCP_VIDEO_HSO_END				(VENC_CBUS_BASE+0xa8)
#define ENCP_VIDEO_VSO_BEGIN			(VENC_CBUS_BASE+0xa9)
#define ENCP_VIDEO_VSO_END				(VENC_CBUS_BASE+0xaa)
#define ENCP_VIDEO_VSO_BLINE			(VENC_CBUS_BASE+0xab)
#define ENCP_VIDEO_VSO_ELINE			(VENC_CBUS_BASE+0xac)
#define ENCP_VIDEO_SYNC_WAVE_CURVE		(VENC_CBUS_BASE+0xad)
#define ENCP_VIDEO_MAX_LNCNT			(VENC_CBUS_BASE+0xae)
#define ENCP_VIDEO_SY_VAL				(VENC_CBUS_BASE+0xb0) //<
#define ENCP_VIDEO_SY2_VAL				(VENC_CBUS_BASE+0xb1) //<
#define ENCP_VIDEO_BLANKY_VAL			(VENC_CBUS_BASE+0xb2)
#define ENCP_VIDEO_BLANKPB_VAL			(VENC_CBUS_BASE+0xb3)
#define ENCP_VIDEO_BLANKPR_VAL			(VENC_CBUS_BASE+0xb4)
#define ENCP_VIDEO_HOFFST				(VENC_CBUS_BASE+0xb5)
#define ENCP_VIDEO_VOFFST				(VENC_CBUS_BASE+0xb6)
#define	ENCP_VIDEO_RGB_CTRL				(VENC_CBUS_BASE+0xb7)
#define	ENCP_VIDEO_FILT_CTRL			(VENC_CBUS_BASE+0xb8)
#define ENCP_VIDEO_OFLD_VPEQ_OFST		(VENC_CBUS_BASE+0xb9) //<
#define ENCP_VIDEO_OFLD_VOAV_OFST		(VENC_CBUS_BASE+0xba) //<
//------------------Macrovision advanced setting
#define ENCP_MACV_BLANKY_VAL			(VENC_CBUS_BASE+0xc0)
#define ENCP_MACV_MAXY_VAL				(VENC_CBUS_BASE+0xc1)
#define ENCP_MACV_1ST_PSSYNC_STRT		(VENC_CBUS_BASE+0xc2)
#define ENCP_MACV_PSSYNC_STRT			(VENC_CBUS_BASE+0xc3)
#define ENCP_MACV_AGC_STRT				(VENC_CBUS_BASE+0xc4)
#define ENCP_MACV_AGC_END				(VENC_CBUS_BASE+0xc5)
#define ENCP_MACV_WAVE_END				(VENC_CBUS_BASE+0xc6)
#define ENCP_MACV_STRTLINE				(VENC_CBUS_BASE+0xc7)
#define ENCP_MACV_ENDLINE				(VENC_CBUS_BASE+0xc8)
#define ENCP_MACV_TS_CNT_MAX_L			(VENC_CBUS_BASE+0xc9)
#define ENCP_MACV_TS_CNT_MAX_H			(VENC_CBUS_BASE+0xca)
#define ENCP_MACV_TIME_DOWN				(VENC_CBUS_BASE+0xcb)
#define ENCP_MACV_TIME_LO				(VENC_CBUS_BASE+0xcc)
#define ENCP_MACV_TIME_UP				(VENC_CBUS_BASE+0xcd)
#define ENCP_MACV_TIME_RST				(VENC_CBUS_BASE+0xce)
//---------------- VBI control -------------------
#define ENCP_VBI_CTRL					(VENC_CBUS_BASE+0xd0) //<
#define ENCP_VBI_SETTING				(VENC_CBUS_BASE+0xd1) //<
#define ENCP_VBI_BEGIN					(VENC_CBUS_BASE+0xd2) //<
#define ENCP_VBI_WIDTH					(VENC_CBUS_BASE+0xd3) //<
#define ENCP_VBI_HVAL					(VENC_CBUS_BASE+0xd4) //<
#define ENCP_VBI_DATA0					(VENC_CBUS_BASE+0xd5) //<
#define ENCP_VBI_DATA1					(VENC_CBUS_BASE+0xd6) //<

//ccir_656
#define C656_HS_ST         				(VENC_CBUS_BASE+0xe0)
#define C656_HS_ED         				(VENC_CBUS_BASE+0xe1)
#define C656_VS_LNST_E     				(VENC_CBUS_BASE+0xe2)
#define C656_VS_LNST_O     				(VENC_CBUS_BASE+0xe3)
#define C656_VS_LNED_E     				(VENC_CBUS_BASE+0xe4)
#define C656_VS_LNED_O     				(VENC_CBUS_BASE+0xe5)
#define C656_FS_LNST       				(VENC_CBUS_BASE+0xe6)
#define C656_FS_LNED       				(VENC_CBUS_BASE+0xe7)




//===========================================================================
// Venc Registers (Cont.)    0xc00 - 0xcff (VENC registers 0xc00 - 0xcef; RDMA registers 0xcf0 - 0xcff)
//===========================================================================
// -----------------------------------------------
// CBUS_BASE:  VENC2_CBUS_BASE = 0x1c
// -----------------------------------------------
// Program video control signals from ENCI core to DVI/HDMI interface
#define ENCI_DVI_HSO_BEGIN                         0x1c00
#define ENCI_DVI_HSO_END                           0x1c01
#define ENCI_DVI_VSO_BLINE_EVN                     0x1c02
#define ENCI_DVI_VSO_BLINE_ODD                     0x1c03
#define ENCI_DVI_VSO_ELINE_EVN                     0x1c04
#define ENCI_DVI_VSO_ELINE_ODD                     0x1c05
#define ENCI_DVI_VSO_BEGIN_EVN                     0x1c06
#define ENCI_DVI_VSO_BEGIN_ODD                     0x1c07
#define ENCI_DVI_VSO_END_EVN                       0x1c08
#define ENCI_DVI_VSO_END_ODD                       0x1c09
// Define cmpt and cvbs cb/cr delay after ENCI chroma filters
// Bit 15:12 RW, enci_cb_cvbs_dly_sel. 0=no delay; 1~6=delay by 1~6 clk; 7~15 reserved.
// Bit 11: 8 RW, enci_cr_cvbs_dly_sel. 0=no delay; 1~6=delay by 1~6 clk; 7~15 reserved.
// Bit  7: 4 RW, enci_cb_cmpt_dly_sel. 0=no delay; 1~6=delay by 1~6 clk; 7~15 reserved.
// Bit  3: 0 RW, enci_cr_cmpt_dly_sel. 0=no delay; 1~6=delay by 1~6 clk; 7~15 reserved.
#define ENCI_CFILT_CTRL2                           0x1c0a
// Program video control signals from ENCP core to DVI/HDMI interface
#define ENCP_DVI_HSO_BEGIN                         0x1c40
#define ENCP_DVI_HSO_END                           0x1c41
#define ENCP_DVI_VSO_BLINE_EVN                     0x1c42
#define ENCP_DVI_VSO_BLINE_ODD                     0x1c43
#define ENCP_DVI_VSO_ELINE_EVN                     0x1c44
#define ENCP_DVI_VSO_ELINE_ODD                     0x1c45
#define ENCP_DVI_VSO_BEGIN_EVN                     0x1c46
#define ENCP_DVI_VSO_BEGIN_ODD                     0x1c47
#define ENCP_DVI_VSO_END_EVN                       0x1c48
#define ENCP_DVI_VSO_END_ODD                       0x1c49
#define ENCP_DE_H_BEGIN                            0x1c4a
#define ENCP_DE_H_END                              0x1c4b
#define ENCP_DE_V_BEGIN_EVEN                       0x1c4c
#define ENCP_DE_V_END_EVEN                         0x1c4d
#define ENCP_DE_V_BEGIN_ODD                        0x1c4e
#define ENCP_DE_V_END_ODD                          0x1c4f
#define ENCP_MAX_LINE_SWITCH_POINT                 0x1c50
// bit    3 cfg_encp_lcd_scaler_bypass. 1=Do not scale LCD input data;
//                                      0=Scale LCD input data to y [16*4,235*4], c [16*4,240*4].
// bit    2 cfg_encp_vadj_scaler_bypass. 1=Do not scale data to enc480p_vadj;
//                                       0=Scale enc480p_vadj input data to y [16*4,235*4], c [16*4,240*4].
// bit    1 cfg_vfifo2vd_out_scaler_bypass. 1=Do not scale vfifo2vd's output vdata;
//                                          0=Scale vfifo2vd's output vdata to y [16,235], c [16,240].
// bit    0 cfg_vfifo_din_full_range. 1=Data from viu fifo is full range [0,1023];
//                                    0=Data from viu fifo is y [16*4,235*4], c [16*4,240*4].
#define VENC_VFIFO2VD_CTL2                         0x1c80
// bit 15:1 Reserved.
// bit    0 cfg_int_dvi_sel_rgb. Applicable for using on-chip hdmi tx module only. This bit controls correct bit-mapping from
//          Venc to hdmi_tx depending on whether YCbCr or RGB mode.
//                               1=Map data bit from Venc to hdmi_tx for RGB mode;
//                               0=Default. Map data bit from Venc to hdmi_tx for YCbCr mode.
#define VENC_DVI_SETTING_MORE                      0x1c81
//Bit 0   filter_en
#define VENC_VDAC_DAC0_FILT_CTRL0                  0x1c88
//dout = ((din + din_d2) * coef1 + (din_d1 * coef0) + 32) >> 6
//Bit 15:8, coef1,
//Bit 7:0, coef0, 
#define VENC_VDAC_DAC0_FILT_CTRL1                  0x1c89
//Bit 0   filter_en
#define VENC_VDAC_DAC1_FILT_CTRL0                  0x1c8a
//dout = ((din + din_d2) * coef1 + (din_d1 * coef0) + 32) >> 6
//Bit 15:8, coef1,
//Bit 7:0, coef0, 
#define VENC_VDAC_DAC1_FILT_CTRL1                  0x1c8b
//Bit 0   filter_en
#define VENC_VDAC_DAC2_FILT_CTRL0                  0x1c8c
//dout = ((din + din_d2) * coef1 + (din_d1 * coef0) + 32) >> 6
//Bit 15:8, coef1,
//Bit 7:0, coef0, 
#define VENC_VDAC_DAC2_FILT_CTRL1                  0x1c8d
//Bit 0   filter_en
#define VENC_VDAC_DAC3_FILT_CTRL0                  0x1c8e
//dout = ((din + din_d2) * coef1 + (din_d1 * coef0) + 32) >> 6
//Bit 15:8, coef1,
//Bit 7:0, coef0, 
#define VENC_VDAC_DAC3_FILT_CTRL1                  0x1c8f
//===========================================================================
// RDMA Registers       0xcf0 - 0xcff
// Bit 31: 0 RW AHB start address for manual start DMA
#define RDMA_AHB_START_ADDR_MAN                    0x1cf0
// Bit 31: 0 RW AHB end address for manual start DMA
#define RDMA_AHB_END_ADDR_MAN                      0x1cf1
// Bit 31: 0 RW AHB start address for auto start source 1
#define RDMA_AHB_START_ADDR_1                      0x1cf2
// Bit 31: 0 RW AHB end address for auto start source 1
#define RDMA_AHB_END_ADDR_1                        0x1cf3
// Bit 31: 0 RW AHB start address for auto start source 2
#define RDMA_AHB_START_ADDR_2                      0x1cf4
// Bit 31: 0 RW AHB end address for auto start source 2
#define RDMA_AHB_END_ADDR_2                        0x1cf5
// Bit 31: 0 RW AHB start address for auto start source 3
#define RDMA_AHB_START_ADDR_3                      0x1cf6
// Bit 31: 0 RW AHB end address for auto start source 3
#define RDMA_AHB_END_ADDR_3                        0x1cf7
// Auto start DMA control:
// Bit 31:24 RW ctrl_enable_int_3. Interrupt inputs enable mask for source 3.
// Bit 23:16 RW ctrl_enable_int_2. Interrupt inputs enable mask for source 2.
// Bit 15: 8 RW ctrl_enable_int_1. Interrupt inputs enable mask for source 1.
// Bit     7 RW ctrl_cbus_write_3. Register read/write mode for auto-start 3. 1=Register write; 0=Register read.
// Bit     6 RW ctrl_cbus_write_3. Register read/write mode for auto-start 2. 1=Register write; 0=Register read.
// Bit     5 RW ctrl_cbus_write_3. Register read/write mode for auto-start 1. 1=Register write; 0=Register read.
// Bit     4 R  Rsrv.
// Bit     3 RW ctrl_cbus_addr_incr_3. 1=Incremental register access for auto-start 3; 0=Non-incremental (individual) register access.
// Bit     2 RW ctrl_cbus_addr_incr_2. 1=Incremental register access for auto-start 2; 0=Non-incremental (individual) register access.
// Bit     1 RW ctrl_cbus_addr_incr_1. 1=Incremental register access for auto-start 1; 0=Non-incremental (individual) register access.
// Bit     0 R  Rsrv.
#define RDMA_ACCESS_AUTO                           0x1cf8
// Manual start DMA control:
// Bit 31: 3 R  Rsrv.
// Bit     2 RW ctrl_cbus_write_man. Register read/write mode for manual-start. 1=Register write; 0=Register read.
// Bit     1 RW ctrl_cbus_addr_incr_man. 1=Incremental register access for manual-start; 0=Non-incremental (individual) register access.
// Bit     0 W  ctrl_start_man. Write 1 to this bit to manual-start DMA. This bit always read back 0.
#define RDMA_ACCESS_MAN                            0x1cf9
// RDMA general control:
// Bit 31: 6 R  Rsrv.
// Bit  5: 4 RW ctrl_ahb_wr_burst_size. 0=ABH write request burst size 16;
//                                      1=ABH write request burst size 24;
//                                      2=ABH write request burst size 32;
//                                      3=ABH write request burst size 48.
// Bit  3: 2 RW ctrl_ahb_rd_burst_size. 0=ABH read request burst size 16;
//                                      1=ABH read request burst size 24;
//                                      2=ABH read request burst size 32;
//                                      3=ABH read request burst size 48.
// Bit     1 RW ctrl_sw_reset. 1=Reset RDMA logics except register.
// Bit     0 RW ctrl_free_clk_enable. 0=Default, Enable clock gating. 1=No clock gating, enable free clock.
#define RDMA_CTRL                                  0x1cfa
// Read only.
// Bit 31:24 R  Rsrv.
// Bit 23:17 R  ahb_wrfifo_cnt. FIFO for buffering CBus read data to be sent to AHB
// Bit 16:10 R  ahb_rdfifo_cnt. FIFO for buffering data read from AHB.
// Bit  9: 8 R  ahb_req_st. =0 -- Idle; !=0 -- AHB interfacing ongoing.
// Bit  7: 4 R  curr_req. Latest requests that is being/been serviced. E.g. 0000=Idle; 0010=Latest serviced request is Req 1.
// Bit  3: 0 R  req_latch. Requests that are yet to be serviced. E.g. 0000=No request; 0001=Req 0 waiting; 1100=Req 2 and 3 waiting.
#define RDMA_STATUS                                0x1cfb

#endif /* VENC_REG_APOLLO_H */
