/*******************************************************************
 *
 *  Copyright C 2008 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: APOLLO VIU/VPP/FE2D register domain definitions.
 *
 *  Author: Amlogic Software
 *  Created: 10/21/2008 5:43:51 PM
 *
 *******************************************************************/

#ifndef VIU_REG_APOLLO_H
#define VIU_REG_APOLLO_H

//========================================================================
//  VIU
//========================================================================  
// ----------------------------------------------- 
// CBUS_BASE:  VIU_CBUS_BASE = 0x1a                
// ----------------------------------------------- 
#ifndef VENC_CBUS_BASE
#define VIU_CBUS_BASE   0x1a00
#endif

#define VD1_IF0_GEN_REG     (VIU_CBUS_BASE+0x50)
    #define VDIF_URGENT_BIT              27
    #define VDIF_CHROMA_END_AT_LAST_LINE (1<<26)
    #define VDIF_LUMA_END_AT_LAST_LINE   (1<<25)
    #define VDIF_HOLD_LINES_BIT          19
    #define VDIF_HOLD_LINES_MASK         0x3f
    #define VDIF_LAST_LINE               (1<<18)
    #define VDIF_BUSY                    (1<<17)
    #define VDIF_DEMUX_MODE              (1<<16)
    #define VDIF_DEMUX_MODE_422          (0<<16)
    #define VDIF_DEMUX_MODE_RGB_444      (1<<16)
    #define VDIF_FORMAT_BIT              14
    #define VDIF_FORMAT_MASK             3
    #define VDIF_FORMAT_SPLIT            (0<<14)
    #define VDIF_FORMAT_422              (1<<14)
    #define VDIF_FORMAT_RGB888_YUV444    (2<<14)
    #define VDIF_BURSTSIZE_MASK          3
    #define VDIF_BURSTSIZE_CR_BIT        12
    #define VDIF_BURSTSIZE_CB_BIT        10
    #define VDIF_BURSTSIZE_Y_BIT         8
    #define VDIF_MANULE_START_FRAME      (1<<7)
    #define VDIF_CHRO_RPT_LAST           (1<<6)
    #define VDIF_CHROMA_HZ_AVG           (1<<3)
    #define VDIF_LUMA_HZ_AVG             (1<<2)
    #define VDIF_SEPARATE_EN             (1<<1)
    #define VDIF_ENABLE                  (1<<0)

#define VD1_IF0_CANVAS0     (VIU_CBUS_BASE+0x51)
#define VD1_IF0_CANVAS1     (VIU_CBUS_BASE+0x52)
    #define VDIF_CANVAS_ADDR_MASK        0x7f
    #define VDIF_CANVAS_ADDR2_BIT        16 
    #define VDIF_CANVAS_ADDR1_BIT        8
    #define VDIF_CANVAS_ADDR0_BIT        0
    
#define VD1_IF0_LUMA_X0     (VIU_CBUS_BASE+0x53)
#define VD1_IF0_LUMA_Y0     (VIU_CBUS_BASE+0x54)
#define VD1_IF0_CHROMA_X0   (VIU_CBUS_BASE+0x55)
#define VD1_IF0_CHROMA_Y0   (VIU_CBUS_BASE+0x56)
#define VD1_IF0_LUMA_X1     (VIU_CBUS_BASE+0x57)
#define VD1_IF0_LUMA_Y1     (VIU_CBUS_BASE+0x58)
#define VD1_IF0_CHROMA_X1   (VIU_CBUS_BASE+0x59)
#define VD1_IF0_CHROMA_Y1   (VIU_CBUS_BASE+0x5a)
    #define VDIF_PIC_XYMASK              0x7fff
    #define VDIF_PIC_END_BIT             16
    #define VDIF_PIC_START_BIT           0
    
#define VD1_IF0_RPT_LOOP    (VIU_CBUS_BASE+0x5b)
    #define VDIF_LOOP_MASK              0xff
    #define VDIF_CHROMA_LOOP1_BIT       24
    #define VDIF_LUMA_LOOP1_BIT         16
    #define VDIF_CHROMA_LOOP0_BIT       8
    #define VDIF_LUMA_LOOP0_BIT         0

#define VD1_IF0_LUMA0_RPT_PAT       (VIU_CBUS_BASE+0x5c)
#define VD1_IF0_CHROMA0_RPT_PAT     (VIU_CBUS_BASE+0x5d)
#define VD1_IF0_LUMA1_RPT_PAT       (VIU_CBUS_BASE+0x5e)
#define VD1_IF0_CHROMA1_RPT_PAT     (VIU_CBUS_BASE+0x5f)

#define VD1_IF0_LUMA_PSEL           (VIU_CBUS_BASE+0x60)
    #define VDIF_PSEL_MODE_BIT      26
    #define VDIF_PSEL_MODE_MASK     3
    #define VDIF_PSEL_MODE_PIC0     (0 << 26)
    #define VDIF_PSEL_MODE_PIC1     (1 << 26)
    #define VDIF_PSEL_MODE_PIC01    (2 << 26)
    #define VDIF_PSEL_LAST_LINE1    (1 << 25)
    #define VDIF_PSEL_LAST_LINE0    (1 << 24)
    #define VDIF_PSEL_PAT_BIT       8
    #define VDIF_PSEL_PAT_MASK      0xffff
    #define VDIF_PSEL_LP_MASK       0xf
    #define VDIF_PSEL_LP_START_BIT  4
    #define VDIF_PSEL_LP_END_BIT    0

#define VD1_IF0_CHROMA_PSEL         (VIU_CBUS_BASE+0x61)

#define VD1_IF0_DUMMY_PIXEL         (VIU_CBUS_BASE+0x62)
    #define VDIF_DUMMY_MASK         0xff
    #define VDIF_DUMMY_Y_BIT        24
    #define VDIF_DUMMY_R_BIT        24
    #define VDIF_DUMMY_CB_BIT       16
    #define VDIF_DUMMY_G_BIT        16
    #define VDIF_DUMMY_CR_BIT       8
    #define VDIF_DUMMY_B_BIT        8

#define VD1_FMT_CTRL				(VIU_CBUS_BASE+0x68)
	#define HFORMATTER_REPEAT		 (1<<28)
	#define HFORMATTER_BILINEAR		 (0<<28)
	#define HFORMATTER_PHASE_MASK    0xf
	#define HFORMATTER_PHASE_BIT	 24
	#define HFORMATTER_RRT_PIXEL0	 (1<<23)
	#define HFORMATTER_YC_RATIO_1_1	 (0<<21)
	#define HFORMATTER_YC_RATIO_2_1	 (1<<21)
	#define HFORMATTER_YC_RATIO_4_1	 (2<<21)
	#define HFORMATTER_EN			 (1<<20)
	#define VFORMATTER_ALWAYS_RPT	 (1<<19)
	#define VFORMATTER_LUMA_RPTLINE0_DE 	(1<<18)
	#define VFORMATTER_CHROMA_RPTLINE0_DE	(1<<17)
	#define VFORMATTER_RPTLINE0_EN	 (1<<16)
	#define VFORMATTER_SKIPLINE_NUM_MASK	0xf
	#define VFORMATTER_SKIPLINE_NUM_BIT		12
    #define VFORMATTER_INIPHASE_MASK        0xf
    #define VFORMATTER_INIPHASE_BIT         8
	#define VFORMATTER_PHASE_MASK	(0x7f)
	#define VFORMATTER_PHASE_BIT	1
	#define VFORMATTER_EN			(1<<0)

#define VD1_FMT_W					(VIU_CBUS_BASE+0x69)
	#define VD1_FMT_LUMA_WIDTH_MASK		0xfff
	#define VD1_FMT_LUMA_WIDTH_BIT		16
	#define VD1_FMT_CHROMA_WIDTH_MASK	0xfff
	#define VD1_FMT_CHROMA_WIDTH_BIT	0

#define VD2_IF0_GEN_REG             (VIU_CBUS_BASE+0x70)
#define VD2_IF0_CANVAS0             (VIU_CBUS_BASE+0x71)
#define VD2_IF0_CANVAS1             (VIU_CBUS_BASE+0x72)
#define VD2_IF0_LUMA_X0             (VIU_CBUS_BASE+0x73)
#define VD2_IF0_LUMA_Y0             (VIU_CBUS_BASE+0x74)
#define VD2_IF0_CHROMA_X0           (VIU_CBUS_BASE+0x75)
#define VD2_IF0_CHROMA_Y0           (VIU_CBUS_BASE+0x76)
#define VD2_IF0_LUMA_X1             (VIU_CBUS_BASE+0x77)
#define VD2_IF0_LUMA_Y1             (VIU_CBUS_BASE+0x78)
#define VD2_IF0_CHROMA_X1           (VIU_CBUS_BASE+0x79)
#define VD2_IF0_CHROMA_Y1           (VIU_CBUS_BASE+0x7a)
#define VD2_IF0_RPT_LOOP            (VIU_CBUS_BASE+0x7b)
#define VD2_IF0_LUMA0_RPT_PAT       (VIU_CBUS_BASE+0x7c)
#define VD2_IF0_CHROMA0_RPT_PAT     (VIU_CBUS_BASE+0x7d)
#define VD2_IF0_LUMA1_RPT_PAT       (VIU_CBUS_BASE+0x7e)
#define VD2_IF0_CHROMA1_RPT_PAT     (VIU_CBUS_BASE+0x7f)
#define VD2_IF0_LUMA_PSEL           (VIU_CBUS_BASE+0x80)
#define VD2_IF0_CHROMA_PSEL         (VIU_CBUS_BASE+0x81)
#define VD2_IF0_DUMMY_PIXEL         (VIU_CBUS_BASE+0x82)
#define VD2_FMT_CTRL				(VIU_CBUS_BASE+0x88)
#define VD2_FMT_W					(VIU_CBUS_BASE+0x89)

#ifdef AML_A3
#define DI_PRE_CTRL                                0x1700
#define DI_POST_CTRL                               0x1701
#define DI_POST_SIZE                               0x1702
#define DI_PRE_SIZE                                0x1703
#define DI_EI_CTRL0                                0x1704
#define DI_EI_CTRL1                                0x1705
#define DI_EI_CTRL2                                0x1706
#define DI_NR_CTRL0                                0x1707
#define DI_NR_CTRL1                                0x1708
#define DI_NR_CTRL2                                0x1709
#define DI_NR_CTRL3                                0x170a
#define DI_MTN_CTRL                                0x170b
#define DI_MTN_CTRL1                               0x170c
#define DI_BLEND_CTRL                              0x170d
#define DI_BLEND_CTRL1                             0x170e
#define DI_BLEND_CTRL2                             0x170f
#define DI_BLEND_REG0_X                            0x1710
#define DI_BLEND_REG0_Y                            0x1711
#define DI_BLEND_REG1_X                            0x1712
#define DI_BLEND_REG1_Y                            0x1713
#define DI_BLEND_REG2_X                            0x1714
#define DI_BLEND_REG2_Y                            0x1715
#define DI_BLEND_REG3_X                            0x1716
#define DI_BLEND_REG3_Y                            0x1717
// DEINTERLACE mode check.
#define DI_MC_REG0_X                               0x1720
#define DI_MC_REG0_Y                               0x1721
#define DI_MC_REG1_X                               0x1722
#define DI_MC_REG1_Y                               0x1723
#define DI_MC_REG2_X                               0x1724
#define DI_MC_REG2_Y                               0x1725
#define DI_MC_REG3_X                               0x1726
#define DI_MC_REG3_Y                               0x1727
#define DI_MC_REG4_X                               0x1728
#define DI_MC_REG4_Y                               0x1729
#define DI_MC_32LVL0                               0x172a
#define DI_MC_32LVL1                               0x172b
#define DI_MC_22LVL0                               0x172c
#define DI_MC_22LVL1                               0x172d
#define DI_MC_22LVL2                               0x172e
#define DI_MC_CTRL                                 0x172f
#define DI_INTR_CTRL                               0x1730
#define DI_INFO_ADDR                               0x1731
#define DI_INFO_DATA                               0x1732
#define DI_PRE_HOLD                                0x1733
#define DI_NRWR_X                                  0x17c0
#define DI_NRWR_Y                                  0x17c1
#define DI_NRWR_CTRL                               0x17c2
#define DI_MTNWR_X                                 0x17c3
#define DI_MTNWR_Y                                 0x17c4
#define DI_MTNWR_CTRL                              0x17c5
#define DI_DIWR_X                                  0x17c6
#define DI_DIWR_Y                                  0x17c7
#define DI_DIWR_CTRL                               0x17c8
#define DI_MTNCRD_X                                0x17c9
#define DI_MTNCRD_Y                                0x17ca
#define DI_MTNPRD_X                                0x17cb
#define DI_MTNPRD_Y                                0x17cc
#define DI_MTNRD_CTRL                              0x17cd
#define DI_INP_GEN_REG                             0x17ce
#define DI_INP_CANVAS0                             0x17cf
#define DI_INP_LUMA_X0                             0x17d0
#define DI_INP_LUMA_Y0                             0x17d1
#define DI_INP_CHROMA_X0                           0x17d2
#define DI_INP_CHROMA_Y0                           0x17d3
#define DI_INP_RPT_LOOP                            0x17d4
#define DI_INP_LUMA0_RPT_PAT                       0x17d5
#define DI_INP_CHROMA0_RPT_PAT                     0x17d6
#define DI_INP_DUMMY_PIXEL                         0x17d7
#define DI_INP_LUMA_FIFO_SIZE                      0x17d8
#define DI_INP_RANGE_MAP_Y                         0x17ba
#define DI_INP_RANGE_MAP_CB                        0x17bb
#define DI_INP_RANGE_MAP_CR                        0x17bc
#define DI_INP_FMT_CTRL                            0x17d9
#define DI_INP_FMT_W                               0x17da
#define DI_MEM_GEN_REG                             0x17db
#define DI_MEM_CANVAS0                             0x17dc
#define DI_MEM_LUMA_X0                             0x17dd
#define DI_MEM_LUMA_Y0                             0x17de
#define DI_MEM_CHROMA_X0                           0x17df
#define DI_MEM_CHROMA_Y0                           0x17e0
#define DI_MEM_RPT_LOOP                            0x17e1
#define DI_MEM_LUMA0_RPT_PAT                       0x17e2
#define DI_MEM_CHROMA0_RPT_PAT                     0x17e3
#define DI_MEM_DUMMY_PIXEL                         0x17e4
#define DI_MEM_LUMA_FIFO_SIZE                      0x17e5
#define DI_MEM_RANGE_MAP_Y                         0x17bd
#define DI_MEM_RANGE_MAP_CB                        0x17be
#define DI_MEM_RANGE_MAP_CR                        0x17bf
#define DI_MEM_FMT_CTRL                            0x17e6
#define DI_MEM_FMT_W                               0x17e7
#define DI_IF1_GEN_REG                             0x17e8
#define DI_IF1_CANVAS0                             0x17e9
#define DI_IF1_LUMA_X0                             0x17ea
#define DI_IF1_LUMA_Y0                             0x17eb
#define DI_IF1_CHROMA_X0                           0x17ec
#define DI_IF1_CHROMA_Y0                           0x17ed
#define DI_IF1_RPT_LOOP                            0x17ee
#define DI_IF1_LUMA0_RPT_PAT                       0x17ef
#define DI_IF1_CHROMA0_RPT_PAT                     0x17f0
#define DI_IF1_DUMMY_PIXEL                         0x17f1
#define DI_IF1_LUMA_FIFO_SIZE                      0x17f2
#define DI_IF1_RANGE_MAP_Y                         0x17fc
#define DI_IF1_RANGE_MAP_CB                        0x17fd
#define DI_IF1_RANGE_MAP_CR                        0x17fe
#define DI_IF1_FMT_CTRL                            0x17f3
#define DI_IF1_FMT_W                               0x17f4
#define DI_CHAN2_GEN_REG                           0x17f5
#define DI_CHAN2_CANVAS                            0x17f6
#define DI_CHAN2_LUMA_X                            0x17f7
#define DI_CHAN2_LUMA_Y                            0x17f8
#define DI_CHAN2_RPT_LOOP                          0x17f9
#define DI_CHAN2_LUMA_RPT_PAT                      0x17fa
#define DI_CHAN2_DUMMY_PIXEL                       0x17fb
#define DI_CHAN2_RANGE_MAP_Y                       0x17b9

#else
#define DI_CTRL                   (VIU_CBUS_BASE+0x90)
      // pre field number             30      0: enen, top.   1: odd, bottom. 
      // post field number            29      0: enen, top.   1: odd, bottom. 
      // pre viu link             28
      // post viu link            27
      // pre repeat              26 
      // post_repeat             25 
      // pre_drop_1st            24
      // post_drop_1st           23
      // prepost_link            22
      // mif0_to_vpp_en          21
      // nr memrd_en              20
      // deinterlace result to vpp enable    19
      // deinterlace result write to mem enable 18
      // final mux output enable 17
      // deinterlace blend enable 16
      // current motion read mif enable 15 
      // motion line buffer enable  14
      // edge interpolation enable 13
      // synchronous fifo enable 12
      // line buffer 2 enable 11
      // line buffer 1 enable 10
      // line buf0 enable 9
      // chan2 enable  8
      // hist use pre nr 7 
      // chan2_hist_en 6
      // hist_check_en 5
      // 2:2 check mid comefrom next field 4
      // check222p_en 3
      // check322p_en 2
      // MTN_EN       1
      // NR_EN        0
#define DI_CTRL1                  (VIU_CBUS_BASE+0x91)
      // reset pre modules        31
      // reset post_modules       30
      // reset pre info register  29
      // reset post info register 28
      // post_ hold lines number  11:6
      // pre_ hold lines number   5:0

#define DI_POST_SIZE              (VIU_CBUS_BASE+0x92)
    // HSIZEM1    11:0              
    // VSIZEM1    27:16 
#define DI_PRE_SIZE               (VIU_CBUS_BASE+0x93)
    // HSIZEM1    11:0              
    // VSIZEM1    27:16 
#define DI_EI_CTRL0               (VIU_CBUS_BASE+0x94)
#define DI_EI_CTRL1               (VIU_CBUS_BASE+0x95)
#define DI_EI_CTRL2               (VIU_CBUS_BASE+0x96)
#define DI_NRMTN_CTRL0            (VIU_CBUS_BASE+0x97)
#define DI_NR_CTRL1               (VIU_CBUS_BASE+0x98)
#define DI_BLEND_CTRL             (VIU_CBUS_BASE+0x99)
#define DI_BLEND_REG0_X           (VIU_CBUS_BASE+0x9a)
#define DI_BLEND_REG0_Y           (VIU_CBUS_BASE+0x9b)
#define DI_BLEND_REG1_X           (VIU_CBUS_BASE+0x9c)
#define DI_BLEND_REG1_Y           (VIU_CBUS_BASE+0x9d)
#define DI_BLEND_REG2_X           (VIU_CBUS_BASE+0x9e)
#define DI_BLEND_REG2_Y           (VIU_CBUS_BASE+0x9f)
#define DI_BLEND_REG3_X           (VIU_CBUS_BASE+0xa0)
#define DI_BLEND_REG3_Y           (VIU_CBUS_BASE+0xa1)

// DEINTERLACE mode check.
#define DI_MC_REG0_X              (VIU_CBUS_BASE+0xa2)
#define DI_MC_REG0_Y              (VIU_CBUS_BASE+0xa3)
#define DI_MC_REG1_X              (VIU_CBUS_BASE+0xa4)
#define DI_MC_REG1_Y              (VIU_CBUS_BASE+0xa5)
#define DI_MC_REG2_X              (VIU_CBUS_BASE+0xa6)
#define DI_MC_REG2_Y              (VIU_CBUS_BASE+0xa7)
#define DI_MC_REG3_X              (VIU_CBUS_BASE+0xa8)
#define DI_MC_REG3_Y              (VIU_CBUS_BASE+0xa9)
#define DI_MC_REG4_X              (VIU_CBUS_BASE+0xaa)
#define DI_MC_REG4_Y              (VIU_CBUS_BASE+0xab)
#define DI_MC_32LVL0              (VIU_CBUS_BASE+0xac)
#define DI_MC_32LVL1              (VIU_CBUS_BASE+0xad)
#define DI_MC_22LVL0              (VIU_CBUS_BASE+0xae)
#define DI_MC_22LVL1              (VIU_CBUS_BASE+0xaf)
#define DI_MC_22LVL2              (VIU_CBUS_BASE+0xb0)
#define DI_MC_CTRL                (VIU_CBUS_BASE+0xb1)
#define DI_INTR_CTRL              (VIU_CBUS_BASE+0xb2) 
  // 3                            hist check done
  // 2                            diwr done
  // 1                            mtn done
  // 0                            nr done
#define DI_INFO_ADDR              (VIU_CBUS_BASE+0xb3)
#define DI_INFO_DATA              (VIU_CBUS_BASE+0xb4)
#define DI_PRE_HOLD               (VIU_CBUS_BASE+0xb5)


#define DI_NRWR_X                 (VIU_CBUS_BASE+0xc0)
#define DI_NRWR_Y                 (VIU_CBUS_BASE+0xc1)
#define DI_NRWR_CTRL              (VIU_CBUS_BASE+0xc2)
#define DI_MTNWR_X                (VIU_CBUS_BASE+0xc3)
#define DI_MTNWR_Y                (VIU_CBUS_BASE+0xc4)
   // urgent                 7
   // canvas_index           6:0
#define DI_MTNWR_CTRL             (VIU_CBUS_BASE+0xc5)
#define DI_DIWR_X                 (VIU_CBUS_BASE+0xc6)
#define DI_DIWR_Y                 (VIU_CBUS_BASE+0xc7)
#define DI_DIWR_CTRL              (VIU_CBUS_BASE+0xc8)
#define DI_MTNCRD_X               (VIU_CBUS_BASE+0xc9)
#define DI_MTNCRD_Y               (VIU_CBUS_BASE+0xca)
#define DI_MTNPRD_X               (VIU_CBUS_BASE+0xcb)
#define DI_MTNPRD_Y               (VIU_CBUS_BASE+0xcc)
#define DI_MTNRD_CTRL             (VIU_CBUS_BASE+0xcd)
#define DI_INP_GEN_REG            (VIU_CBUS_BASE+0xce)
#define DI_INP_CANVAS0            (VIU_CBUS_BASE+0xcf)
#define DI_INP_LUMA_X0            (VIU_CBUS_BASE+0xd0)
#define DI_INP_LUMA_Y0            (VIU_CBUS_BASE+0xd1)
#define DI_INP_CHROMA_X0          (VIU_CBUS_BASE+0xd2)
#define DI_INP_CHROMA_Y0          (VIU_CBUS_BASE+0xd3)
#define DI_INP_RPT_LOOP           (VIU_CBUS_BASE+0xd4)
#define DI_INP_LUMA0_RPT_PAT      (VIU_CBUS_BASE+0xd5)
#define DI_INP_CHROMA0_RPT_PAT    (VIU_CBUS_BASE+0xd6)
#define DI_INP_DUMMY_PIXEL        (VIU_CBUS_BASE+0xd7)
#define DI_INP_LUMA_FIFO_SIZE     (VIU_CBUS_BASE+0xd8)
#define DI_INP_FMT_CTRL           (VIU_CBUS_BASE+0xd9)
#define DI_INP_FMT_W              (VIU_CBUS_BASE+0xda)
#define DI_MEM_GEN_REG            (VIU_CBUS_BASE+0xdb)
#define DI_MEM_CANVAS0            (VIU_CBUS_BASE+0xdc)
#define DI_MEM_LUMA_X0            (VIU_CBUS_BASE+0xdd)
#define DI_MEM_LUMA_Y0            (VIU_CBUS_BASE+0xde)
#define DI_MEM_CHROMA_X0          (VIU_CBUS_BASE+0xdf)
#define DI_MEM_CHROMA_Y0          (VIU_CBUS_BASE+0xe0)
#define DI_MEM_RPT_LOOP           (VIU_CBUS_BASE+0xe1)
#define DI_MEM_LUMA0_RPT_PAT      (VIU_CBUS_BASE+0xe2)
#define DI_MEM_CHROMA0_RPT_PAT    (VIU_CBUS_BASE+0xe3)
#define DI_MEM_DUMMY_PIXEL        (VIU_CBUS_BASE+0xe4)
#define DI_MEM_LUMA_FIFO_SIZE     (VIU_CBUS_BASE+0xe5)
#define DI_MEM_FMT_CTRL           (VIU_CBUS_BASE+0xe6)
#define DI_MEM_FMT_W              (VIU_CBUS_BASE+0xe7)
#define DI_IF1_GEN_REG            (VIU_CBUS_BASE+0xe8)
#define DI_IF1_CANVAS0            (VIU_CBUS_BASE+0xe9)
#define DI_IF1_LUMA_X0            (VIU_CBUS_BASE+0xea)
#define DI_IF1_LUMA_Y0            (VIU_CBUS_BASE+0xeb)
#define DI_IF1_CHROMA_X0          (VIU_CBUS_BASE+0xec)
#define DI_IF1_CHROMA_Y0          (VIU_CBUS_BASE+0xed)
#define DI_IF1_RPT_LOOP           (VIU_CBUS_BASE+0xee)
#define DI_IF1_LUMA0_RPT_PAT      (VIU_CBUS_BASE+0xef)
#define DI_IF1_CHROMA0_RPT_PAT    (VIU_CBUS_BASE+0xf0)
#define DI_IF1_DUMMY_PIXEL        (VIU_CBUS_BASE+0xf1)
#define DI_IF1_LUMA_FIFO_SIZE     (VIU_CBUS_BASE+0xf2)
#define DI_IF1_FMT_CTRL           (VIU_CBUS_BASE+0xf3)
#define DI_IF1_FMT_W              (VIU_CBUS_BASE+0xf4)
#define DI_CHAN2_GEN_REG          (VIU_CBUS_BASE+0xf5)
#define DI_CHAN2_CANVAS           (VIU_CBUS_BASE+0xf6)
#define DI_CHAN2_LUMA_X           (VIU_CBUS_BASE+0xf7)
#define DI_CHAN2_LUMA_Y           (VIU_CBUS_BASE+0xf8)
#define DI_CHAN2_RPT_LOOP         (VIU_CBUS_BASE+0xf9)
#define DI_CHAN2_LUMA_RPT_PAT     (VIU_CBUS_BASE+0xfa)
#define DI_CHAN2_DUMMY_PIXEL      (VIU_CBUS_BASE+0xfb)
#endif

//========================================================================
//  VPP
//========================================================================       
// -----------------------------------------------  
// CBUS_BASE:  VPP_CBUS_BASE = 0x1d                 
// -----------------------------------------------  
#define VPP_CBUS_BASE   0x1d00

#define VPP_DUMMY_DATA                  (VPP_CBUS_BASE+0x00)
    #define VPP_DUMMY_MASK              0xff
    #define VPP_DUMMY_Y_BIT             16
    #define VPP_DUMMY_Cb_BIT            8
    #define VPP_DUMMY_Cr_BIT            0

#define VPP_LINE_IN_LENGTH              (VPP_CBUS_BASE+0x01)

#define VPP_PIC_IN_HEIGHT               (VPP_CBUS_BASE+0x02)

#define VPP_SCALE_COEF_IDX              (VPP_CBUS_BASE+0x03)
    #define VPP_COEF_IDXINC         (1 << 15)
    #define VPP_COEF_RD_CBUS        (1 << 14)
    #define VPP_COEF_9BIT           (1 << 9)
    #define VPP_COEF_TYPE           (1 << 8)
    #define VPP_COEF_VERT           (0 << 8)
    #define VPP_COEF_HORZ           (1 << 8)
    #define VPP_COEF_INDEX_MASK     0x7f
    #define VPP_COEF_INDEX_BIT      0

#define VPP_SCALE_COEF                  (VPP_CBUS_BASE+0x04)

#define VPP_VSC_REGION12_STARTP         (VPP_CBUS_BASE+0x05)
    #define VPP_REGION_MASK             0xfff
    #define VPP_REGION1_BIT             16
    #define VPP_REGION2_BIT             0

#define VPP_VSC_REGION34_STARTP         (VPP_CBUS_BASE+0x06)
    #define VPP_REGION3_BIT             16
    #define VPP_REGION4_BIT             0

#define VPP_VSC_REGION4_ENDP            (VPP_CBUS_BASE+0x07)

#define VPP_VSC_START_PHASE_STEP        (VPP_CBUS_BASE+0x08)

#define VPP_VSC_RGN0_START_PHASE_SLOPE  (VPP_CBUS_BASE+0x09)
#define VPP_VSC_RGN1_START_PHASE_SLOPE  (VPP_CBUS_BASE+0x0a)
#define VPP_VSC_RGN3_START_PHASE_SLOPE  (VPP_CBUS_BASE+0x0b)
#define VPP_VSC_RGN4_START_PHASE_SLOPE  (VPP_CBUS_BASE+0x0c)

#define VPP_VSC_PHASE_CTRL              (VPP_CBUS_BASE+0x0d)
    #define VPP_PHASECTL_TYPE           (1<<16)
    #define VPP_PHASECTL_TYPE_PROG      (0<<16)
    #define VPP_PHASECTL_TYPE_INTERLACE (1<<16)
    #define VPP_PHASECTL_VSL0B          (1<<15)
    #define VPP_PHASECTL_INIRPTNUM_MASK 0x3
    #define VPP_PHASECTL_INIRPTNUM_WID  2
    #define VPP_PHASECTL_INIRPTNUMB_BIT 13
    #define VPP_PHASECTL_INIRCVNUM_MASK 0xf
    #define VPP_PHASECTL_INIRCVNUM_WID  5
    #define VPP_PHASECTL_INIRCVNUMB_BIT 8
    #define VPP_PHASECTL_VSL0T          (1<<7)
    #define VPP_PHASECTL_INIRPTNUMT_BIT 5
    #define VPP_PHASECTL_INIRCVNUMT_BIT 0

#define VPP_VSC_INI_PHASE               (VPP_CBUS_BASE+0x0e)
    #define VPP_VSC_PHASE_MASK          0xffff
    #define VPP_VSC_PHASEB_BIT          16
    #define VPP_VSC_PHASET_BIT          0

#define VPP_HSC_REGION12_STARTP         (VPP_CBUS_BASE+0x10)
    #define VPP_HSC_RGN_MASK            0xfff
    #define VPP_HSC_RGN1_BIT            16
    #define VPP_HSC_RGN2_BIT            0

#define VPP_HSC_REGION34_STARTP         (VPP_CBUS_BASE+0x11)
    #define VPP_HSC_RGN3_BIT            16
    #define VPP_HSC_RGN4_BIT            0

#define VPP_HSC_REGION4_ENDP            (VPP_CBUS_BASE+0x12)

#define VPP_HSC_START_PHASE_STEP        (VPP_CBUS_BASE+0x13)

#define VPP_HSC_REGION0_PHASE_SLOPE     (VPP_CBUS_BASE+0x14)
#define VPP_HSC_REGION1_PHASE_SLOPE     (VPP_CBUS_BASE+0x15)
#define VPP_HSC_REGION3_PHASE_SLOPE     (VPP_CBUS_BASE+0x16)
#define VPP_HSC_REGION4_PHASE_SLOPE     (VPP_CBUS_BASE+0x17)

#define VPP_HSC_PHASE_CTRL              (VPP_CBUS_BASE+0x18)
    #define VPP_HSC_INIRPT_NUM_MASK     0x3
    #define VPP_HSC_INIRPT_NUM_BIT      21
    #define VPP_HSC_INIRCV_NUM_MASK     0xf
    #define VPP_HSC_INIRCV_NUM_BIT      16
    #define VPP_HSC_TOP_INI_PHASE_WID   16
    #define VPP_HSC_TOP_INI_PHASE_BIT   0
    
#define VPP_SC_MISC                     (VPP_CBUS_BASE+0x19)
    #define VPP_SC_PREHORZ_EN           (1 << 20)
    #define VPP_SC_PREVERT_EN           (1 << 19)
    #define VPP_SC_VERT_EN              (1 << 18)
    #define VPP_SC_HORZ_EN              (1 << 17)
    #define VPP_SC_TOP_EN               (1 << 16)
    #define VPP_SC_V1OUT_EN             (1 << 15)
    #define VPP_SC_RGN14_HNOLINEAR      (1 << 12)
    #define VPP_SC_BANK_LENGTH_WID      3
    #define VPP_SC_BANK_LENGTH_MASK     0x7
    #define VPP_SC_HBANK_LENGTH_BIT     8
    #define VPP_SC_RGN14_VNOLINEAR      (1 << 4)
    #define VPP_SC_VBANK_LENGTH_BIT     0

#define VPP_PREBLEND_VD1_H_START_END    (VPP_CBUS_BASE+0x1a)
#define VPP_PREBLEND_VD1_V_START_END    (VPP_CBUS_BASE+0x1b)
#define VPP_POSTBLEND_VD1_H_START_END   (VPP_CBUS_BASE+0x1c)
#define VPP_POSTBLEND_VD1_V_START_END   (VPP_CBUS_BASE+0x1d)
#define VPP_BLEND_VD2_H_START_END       (VPP_CBUS_BASE+0x1e)
#define VPP_BLEND_VD2_V_START_END       (VPP_CBUS_BASE+0x1f)
    #define VPP_VD_SIZE_MASK            0xfff
    #define VPP_VD1_START_BIT           16
    #define VPP_VD1_END_BIT             0

#define VPP_PREBLEND_HSIZE              (VPP_CBUS_BASE+0x20)
#define VPP_POSTBLEND_HSIZE             (VPP_CBUS_BASE+0x21)

#define VPP_HOLD_LINES                  (VPP_CBUS_BASE+0x22)

#define VPP_BLEND_ONECOLOR_CTRL         (VPP_CBUS_BASE+0x23)
    #define VPP_PREBLEND_1COLOR         (1 << 25)
    #define VPP_POSTBLEND_1COLOR        (1 << 24)
    #define VPP_1COLOR_MASK             0xff
    #define VPP_1COLOR_Y_BIT            16
    #define VPP_1COLOR_CB_BIT           8
    #define VPP_1COLOR_CR_BIT           0
    
#define VPP_PREBLEND_CUR_XY             (VPP_CBUS_BASE+0x24)
    #define VPP_CURXY_MASK              0xfff
    #define VPP_PREBLEND_CURX_BIT       16    
    #define VPP_PREBLEND_CURY_BIT       0 

#define VPP_POSTBLEND_CUR_XY            (VPP_CBUS_BASE+0x25)

#define VPP_MISC                        (VPP_CBUS_BASE+0x26)
    #define VPP_VD2_ALPHA_WID           9
    #define VPP_VD2_ALPHA_MASK          0x1ff
    #define VPP_VD2_ALPHA_BIT           18
    #define VPP_OSD2_PREBLEND           (1 << 17)
    #define VPP_OSD1_PREBLEND           (1 << 16)
    #define VPP_VD2_PREBLEND            (1 << 15)
    #define VPP_VD1_PREBLEND            (1 << 14)
    #define VPP_OSD2_POSTBLEND          (1 << 13)
    #define VPP_OSD1_POSTBLEND          (1 << 12)
    #define VPP_VD2_POSTBLEND           (1 << 11)
    #define VPP_VD1_POSTBLEND           (1 << 10)
    #define VPP_OSD1_ALPHA              (1 << 9)
    #define VPP_OSD2_ALPHA              (1 << 8)
    #define VPP_POSTBLEND_EN            (1 << 7)
    #define VPP_PREBLEND_EN             (1 << 6)
    #define VPP_PRE_FG_SEL_MASK         (1 << 5)
    #define VPP_PRE_FG_OSD2             (1 << 5)
    #define VPP_PRE_FG_OSD1             (0 << 5)
    #define VPP_POST_FG_SEL_MASK        (1 << 4)
    #define VPP_POST_FG_OSD2            (1 << 4)
    #define VPP_POST_FG_OSD1            (0 << 4)
    #define VPP_FIFO_RESET_DE           (1 << 2)
    #define VPP_OUT_SATURATE            (1 << 0)
    
#define VPP_OFIFO_SIZE                  (VPP_CBUS_BASE+0x27)
    #define VPP_OFIFO_LINELEN_MASK      0xfff
    #define VPP_OFIFO_LINELEN_BIT       20
    #define VPP_INV_VS                  (1 << 19)
    #define VPP_INV_HS                  (1 << 18)
    #define VPP_FORCE_FIELD_EN          (1 << 17)
    #define VPP_FORCE_FIELD_TYPE_MASK   (1 << 16)
    #define VPP_FORCE_FIELD_TOP         (0 << 16)
    #define VPP_FORCE_FIELD_BOTTOM      (1 << 16)
    #define VPP_FOURCE_GO_FIELD         (1 << 15)
    #define VPP_FOURCE_GO_LINE          (1 << 14)
    #define VPP_OFIFO_SIZE_WID          12
    #define VPP_OFIFO_SIZE_MASK         0xfff
    #define VPP_OFIFO_SIZE_BIT          0

#define VPP_OFIFO_STATUS                (VPP_CBUS_BASE+0x28)
    #define VPP_SOFIFO_CNT_MASK         0x3ff
    #define VPP_SOFIFO_CNT_BIT          17
    #define VPP_AFIFO_CNT_MASK          0x1ff
    #define VPP_AFIFO_CNT_BIT           12
    #define VPP_OFIFO_CNT_MASK          0xfff
    #define VPP_OFIFO_CNT_BIT           0

#define VPP_SMOKE_CTRL                  (VPP_CBUS_BASE+0x29)
    #define VPP_POSTSMOKE3EN_POSTVD2DE  (1 << 5)
    #define VPP_PRESMOKE3EN_PREVD2DE    (1 << 4)
    #define VPP_POSTSMOKE2EN_POSTVD2DE  (1 << 3)
    #define VPP_PRESMOKE2EN_PREVD2DE    (1 << 2)
    #define VPP_POSTSMOKE1EN_POSTVD2DE  (1 << 1)
    #define VPP_PRESMOKE1EN_PREVD2DE    (1 << 0)

#define VPP_SMOKE1_VAL                  (VPP_CBUS_BASE+0x2a)
#define VPP_SMOKE2_VAL                  (VPP_CBUS_BASE+0x2b)
#define VPP_SMOKE3_VAL                  (VPP_CBUS_BASE+0x2c)
    #define VPP_SMOKE_VAL_MASK          0xff
    #define VPP_SMOKE_Y_BIT             24
    #define VPP_SMOKE_CB_BIT            16
    #define VPP_SMOKE_CR_BIT            8
    #define VPP_SMOKE_ALPHA_BIT         0

#define VPP_SMOKE1_H_START_END          (VPP_CBUS_BASE+0x2d)
#define VPP_SMOKE1_V_START_END          (VPP_CBUS_BASE+0x2e)
#define VPP_SMOKE2_H_START_END          (VPP_CBUS_BASE+0x2f)
#define VPP_SMOKE2_V_START_END          (VPP_CBUS_BASE+0x30)
#define VPP_SMOKE3_H_START_END          (VPP_CBUS_BASE+0x31)
#define VPP_SMOKE3_V_START_END          (VPP_CBUS_BASE+0x32)
    #define VPP_SMOKE_SE_MASK           0xfff
    #define VPP_SMOKE_START_BIT         16
    #define VPP_SMOKE_END_BIT           0

#define VPP_SCO_FIFO_CTRL               (VPP_CBUS_BASE+0x33)
    #define VPP_SCO_FIFO_LEN_MASK       0xfff
    #define VPP_SCO_FIFO_LEN_BIT        16
    #define VPP_SCO_FIFO_SIZE_MASK      0x3ff
    #define VPP_SCO_FIFO_SIZE_BIT       0

#define VPP_VADJ_CTRL                   (VPP_CBUS_BASE+0x40)
    #define VPP_VADJ2_BLMINUS_EN        (1 << 3)
    #define VPP_VADJ2_EN                (1 << 2)
    #define VPP_VADJ1_BLMINUS_EN        (1 << 1)
    #define VPP_VADJ1_EN                (1 << 0)

#define VPP_VADJ1_Y                     (VPP_CBUS_BASE+0x41)
    #define VPP_VADJ_COEF_MASK          0xff
    #define VPP_VADJ_BR_BIT             8
    #define VPP_VADJ_CT_BIT             0

#define VPP_VADJ1_MA_MB                 (VPP_CBUS_BASE+0x42)
    #define VPP_VADJ_MA_BIT             16
    #define VPP_VADJ_MB_BIT             0

#define VPP_VADJ1_MC_MD                 (VPP_CBUS_BASE+0x43)
    #define VPP_VADJ_MC_BIT             16
    #define VPP_VADJ_MD_BIT             0

#define VPP_VADJ2_Y                     (VPP_CBUS_BASE+0x44)
#define VPP_VADJ2_MA_MB                 (VPP_CBUS_BASE+0x45)
#define VPP_VADJ2_MC_MD                 (VPP_CBUS_BASE+0x46)

#define VPP_HSHARP_CTRL                 (VPP_CBUS_BASE+0x50)
    #define VPP_CHROMA_HSHARP_SB_SEL    (1 << 2)
    #define VPP_CHROMA_HSHARP_SHARP     (0 << 2)
    #define VPP_CHROMA_HSHARP_BLUR      (1 << 2)
    #define VPP_LUMA_HSHARP_SB_SEL      (1 << 1)
    #define VPP_LUMA_HSHARP_SHARP       (0 << 1)
    #define VPP_LUMA_HSHARP_BLUR        (1 << 1)
    #define VPP_HSHARP_EN               (1 << 0)

#define VPP_HSHARP_LUMA_THRESH01        (VPP_CBUS_BASE+0x51)
    #define VPP_HSHARP_THRESH_MASK      0x7ff
    #define VPP_HSHARP_THRESH0_BIT      16
    #define VPP_HSHARP_THRESH1_BIT      0

#define VPP_HSHARP_LUMA_THRESH23        (VPP_CBUS_BASE+0x52)
    #define VPP_HSHARP_THRESH2_BIT      16
    #define VPP_HSHARP_THRESH3_BIT      0

#define VPP_HSHARP_CHROMA_THRESH01      (VPP_CBUS_BASE+0x53)
#define VPP_HSHARP_CHROMA_THRESH23      (VPP_CBUS_BASE+0x54)

#define VPP_HSHARP_LUMA_GAIN            (VPP_CBUS_BASE+0x55)
#define VPP_HSHARP_CHROMA_GAIN          (VPP_CBUS_BASE+0x56)
    #define VPP_GAIN_MASK               0xff
    #define VPP_GAIN2_BIT               16
    #define VPP_GAIN1_BIT               8
    #define VPP_GAIN0_BIT               0

#define VPP_MAT_CTRL                    (VPP_CBUS_BASE+0x5f)
    #define VPP_MAT_SEL_MASK            (3 << 8)
    #define VPP_MAT_SEL_BIT             8
	#define VPP_MAT_VD0_MAT_EN          (1 << 5)
	#define VPP_MAT_VD1_MAT_EN          (1 << 4)
	#define VPP_MAT_OUTSAT_EN_BIT       (1 << 2)
	#define VPP_MAT_INSAT_EN_BIT        (1 << 1)
    #define VPP_MAT_EN                  (1 << 0)
    
#define VPP_MAT_COEF00_01               (VPP_CBUS_BASE+0x60)
    #define VPP_MAT_COEF_MASK           0xfff
    #define VPP_MAT_COEF00_BIT          16
    #define VPP_MAT_COEF01_BIT          0

#define VPP_MAT_COEF02_10               (VPP_CBUS_BASE+0x61)
    #define VPP_MAT_COEF02_BIT          16
    #define VPP_MAT_COEF10_BIT          0

#define VPP_MAT_COEF11_12               (VPP_CBUS_BASE+0x62)
    #define VPP_MAT_COEF11_BIT          16
    #define VPP_MAT_COEF12_BIT          0

#define VPP_MAT_COEF20_21               (VPP_CBUS_BASE+0x63)
    #define VPP_MAT_COEF20_BIT          16
    #define VPP_MAT_COEF21_BIT          0

#define VPP_MAT_COEF22                  (VPP_CBUS_BASE+0x64)

#define VPP_MAT_OFFSET0_1               (VPP_CBUS_BASE+0x65)
    #define VPP_MAT_OFFSET_MASK         0x7ff
    #define VPP_MAT_OFFSET0_BIT         16
    #define VPP_MAT_OFFSET1_BIT         0

#define VPP_MAT_OFFSET2                 (VPP_CBUS_BASE+0x66)

#define VPP_MAT_PRE_OFFSET0_1			(VPP_CBUS_BASE+0x67)
#define VPP_MAT_PRE_OFFSET2 			(VPP_CBUS_BASE+0x68)

#define VPP_DUMMY_DATA1					(VPP_CBUS_BASE+0x69)

#define VPP_CHROMA_ADDR_PORT			(VPP_CBUS_BASE+0x70)
#define VPP_CHROMA_DATA_PORT			(VPP_CBUS_BASE+0x71)

#endif /* VIU_REG_APOLLO_H */
