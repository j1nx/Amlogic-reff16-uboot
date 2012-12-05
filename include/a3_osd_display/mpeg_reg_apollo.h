/*******************************************************************
 *
 *  Copyright C 2008 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: APOLLO MPEG register domain definitions.
 *
 *  Author: Amlogic Software
 *  Created: 10/13/2008 2:40:11 PM
 *
 *******************************************************************/

#ifndef MPEG_REG_APOLLO_H
#define MPEG_REG_APOLLO_H

//========================================================================
//  Global Control Registers                (0x000 - 0x0ff)
//========================================================================
#define RESET_CBUS_BASE 0x1100

#define VERSION_CTRL        (RESET_CBUS_BASE+0x00)
#define RESET0_REGISTER     (RESET_CBUS_BASE+0x01)
    #define RESET_HIU           (1<<0)
    #define RESET_VLD           (1<<1)
    #define RESET_IQIDCT        (1<<2)
    #define RESET_MC            (1<<3)
    #define RESET_VIU           (1<<5)
    #define RESET_AIU           (1<<6)
    #define RESET_VCPU          (1<<7)
    #define RESET_CCPU          (1<<8)
    #define RESET_ASSIST        (1<<11)
    #define RESET_AIFIFO2       (1<<12)
    #define RESET_MDEC          (1<<13)
    #define RESET_VLD_PART      (1<<14)
#define RESET1_REGISTER     (RESET_CBUS_BASE+0x02)
    #define RESET_DEMUXSTB      (1<<1)
    #define RESET_USB           (1<<2)
    #define RESET_DDR           (1<<3)
    #define RESET_BT656IN       (1<<5)
    #define RESET_DVIN          (1<<6)
    #define RESET_AHB_BRIDGE    (1<<7)
    #define RESET_PARSER        (1<<8)
    #define RESET_ARC1          (1<<9)
    #define RESET_DDRPLL        (1<<10)
    #define RESET_ARC2          (1<<11)
#define RESET2_REGISTER     (RESET_CBUS_BASE+0x03)
    #define RESET_AUDIO_IN      (1<<1)
    #define RESET_DBLK          (1<<2)
    #define RESET_PIC_DC        (1<<3)
    #define RESET_PSCALE        (1<<4)
    #define RESET_PNAND         (1<<5)
    #define RESET_GE2D          (1<<6)

//========================================================================
//  Host uP Interface                   (0x100 - 0x2ff)
//========================================================================
#define HIU_CBUS_BASE       0x1000

#define SCR_HIU             (HIU_CBUS_BASE+0x0b)
#define HPG_TIMER           (HIU_CBUS_BASE+0x0f)

#define HARM_ASB_MB0        (HIU_CBUS_BASE+0x30)   // arm to asb mailbox
#define HARM_ASB_MB1        (HIU_CBUS_BASE+0x31)   // arm to asb mailbox
#define HARM_ASB_MB2        (HIU_CBUS_BASE+0x32)   // arm to asb mailbox
#define HARM_ASB_MB3        (HIU_CBUS_BASE+0x33)   // arm to asb mailbox
#define HASB_ARM_MB0        (HIU_CBUS_BASE+0x34)   // asb to arm mailbox
#define HASB_ARM_MB1        (HIU_CBUS_BASE+0x35)   // asb to arm mailbox
#define HASB_ARM_MB2        (HIU_CBUS_BASE+0x36)   // asb to arm mailbox
#define HASB_ARM_MB3        (HIU_CBUS_BASE+0x37)   // asb to arm mailbox
#define HHI_TIMER90K        (HIU_CBUS_BASE+0x3b)   // 90kHz timer divisor

#define HHI_AUD_DAC_CTRL    (HIU_CBUS_BASE+0x44)
#define CBUS_TO_AHB_ADDR    (HIU_CBUS_BASE+0x45)
#define CBUS_TO_AHB_CTRL    (HIU_CBUS_BASE+0x47)
#define CBUS_TO_AHB_WDATA   (HIU_CBUS_BASE+0x48)
#define CBUS_TO_AHB_RDATA   (HIU_CBUS_BASE+0x4a)

// Gated clock enables.  There are 64 enables for the MPEG clocks and 32 enables for other clock domains
#define HHI_GCLK_MPEG0      (HIU_CBUS_BASE+0x50)
#define HHI_GCLK_MPEG1      (HIU_CBUS_BASE+0x51)
#define HHI_GCLK_MPEG2      (HIU_CBUS_BASE+0x52)
#define HHI_GCLK_OTHER      (HIU_CBUS_BASE+0x54)

// PLL Controls
#define HHI_SYS_PLL_CNTL2        (HIU_CBUS_BASE+0x4d)
#define HHI_AUD_PLL_CNTL2        (HIU_CBUS_BASE+0x4e)
#define HHI_VID_PLL_CNTL2        (HIU_CBUS_BASE+0x4f)

#define HHI_SYS_PLL_CNTL3   (HIU_CBUS_BASE+0x56)
#define HHI_AUD_PLL_CNTL3   (HIU_CBUS_BASE+0x57)
#define HHI_VID_PLL_CNTL3   (HIU_CBUS_BASE+0x58)

#define HHI_VID_CLK_DIV     (HIU_CBUS_BASE+0x59) // video clock control
#define MREG_MPEG_PLL_CTRL  (HIU_CBUS_BASE+0x5a) // MPEG PLL control
#define MREG_AUDIO_PLL_CTRL (HIU_CBUS_BASE+0x5b) // Audio PLL control
#define HHI_VID_PLL_CNTL (HIU_CBUS_BASE+0x5c) // Video PLL control

#define MREG_MPEG_CLK_CTRL  (HIU_CBUS_BASE+0x5d) // MPEG clock control
#define MREG_AUDIO_CLK_CTRL (HIU_CBUS_BASE+0x5e) // Audio clock control
#define HHI_VID_CLK_CNTL    (HIU_CBUS_BASE+0x5f) // video clock control
#define HHI_VID_DIVIDER_CNTL  (HIU_CBUS_BASE+0x66) 
#define HHI_SYS_CPU_CLK_CNTL  (HIU_CBUS_BASE+0x67) 

#define HHI_OTHER_PLL_CNTL    (HIU_CBUS_BASE+0x70)
#define HHI_HDMI_CLK_CNTL     (HIU_CBUS_BASE+0x73)
#define HHI_ETH_CLK_CNTL      (HIU_CBUS_BASE+0x76)
#define HHI_HDMI_AFC_CNTL     (HIU_CBUS_BASE+0x7f)

#define HHI_SYS_ARC_AUTO_CLK  (HIU_CBUS_BASE+0x64) // SYS ARC auto clock change
#define HHI_AUD_ARC_AUTO_CLK  (HIU_CBUS_BASE+0x65) // AUD ARC auto clock change

#define ARC_AUTO_CLK_SET(a,b,c) ARC_AUTO_CLK_##a##_BIT=b,ARC_AUTO_CLK_##a##_MASK=c
#define MPEG_CLK_CONTRL(a,b,c)  MPEG_CLK_CONTRL_##a##_BIT=b,MPEG_CLK_CONTRL_##a##_MASK=c

enum{
    /** AUTO CLK0 Settings**/
    ARC_AUTO_CLK_SET(START,15,1),
    ARC_AUTO_CLK_SET(SEL  ,14,1),
    ARC_AUTO_CLK_SET(ENABLE_DELAY,8,0xf),
    ARC_AUTO_CLK_SET(START_DELAY,0,0xff),
    /** AUTO CLK1 Settings**/
    ARC_AUTO_CLK_SET(ENABLE,15+16,1),
    ARC_AUTO_CLK_SET(TIMEBASE,12+16,3),
    ARC_AUTO_CLK_SET(SLEEPCOUNT,15+16,0xff),
    ARC_AUTO_CLK_SET(MODE,2+16,0x3),
    ARC_AUTO_CLK_SET(CLEAR_FIQ,1+16,0x1),
    ARC_AUTO_CLK_SET(CLEAR_IRQ,0+16,0x1),
    /** MPEG CLOCK CONTROL SETTING **/
    MPEG_CLK_CONTRL(ARC_DIV2_SEL,15,1),
    MPEG_CLK_CONTRL(ARC_DIV2_ENABLE,14,1),
    MPEG_CLK_CONTRL(ARC_CLK_GATE,13,1),
    MPEG_CLK_CONTRL(RTC_OSC_EN,9,1)
    
};
#define         AUTO_CLK_MODE_CHANGE_CLK            0
#define         AUTO_CLK_MODE_DELAYED_WAKE          1
#define         AUTO_CLK_MODE_IRQ_DELAYED_WAKE      2
#define         AUTO_CLK_MODE_IRQ_ONLY_WAKE         3


//========================================================================
//  Host uP Interface                   (0x100 - 0x2ff)
//========================================================================
#define PARSER_CBUS_BASE 0x0100

// bit 31:8 -- es_pack_size
// bit 7:6 -- es_type 00-Video, 01-Audio, 10-Subtitle
// bit 5 -- es_write
// bit 4 -- es_passthrough
// bit 3 -- insert_before_es_write
// bit 2 -- discard_cmd
// bit 1 -- search_cmd
// bit 0 -- Parser Busy
#define PARSER_CTRL 	             (PARSER_CBUS_BASE+0x60)
    #define ES_PACK_SIZE_MASK               0xffffff
    #define ES_PACK_SIZE_BIT                8
    #define ES_PACK_SIZE_WID                24
	#define ES_CTRL_WID						8
	#define ES_CTRL_BIT						0
    #define ES_TYPE_MASK                    (3 << 6)
    #define ES_TYPE_VIDEO                   (0 << 6)
    #define ES_TYPE_AUDIO                   (1 << 6)
    #define ES_TYPE_SUBTITLE                (2 << 6)
    #define ES_WRITE                        (1<<5)
    #define ES_PASSTHROUGH                  (1<<4)
    #define ES_INSERT_BEFORE_ES_WRITE       (1<<3)
    #define ES_DISCARD                      (1<<2)
    #define ES_SEARCH                       (1<<1)
    #define ES_PARSER_START                 (1<<0)
    #define ES_PARSER_BUSY                  (1<<0)

// Write
//   bit 31:0 begin address for parser fetch
// Read
//   32 bit current fetch address
#define PARSER_FETCH_ADDR           (PARSER_CBUS_BASE+0x61)

// Write
//   bit 31 circle buffer indicator
//   bit 30 check_buffer_stop_addr
//   bit 29:27 fetch endian
//   bit 26:0 buffer size ( Bytes )
// Special command:
//   when bit 26:0 == 0x0
//     then bit 29    -- stream_passthrough_enable
//          bit 28:27 -- stream_passthrough_type
// Read
//   6:5 cmd_rd_ptr
//   4:3 cmd_wr_ptr
//   2:0 cmd_number
#define PARSER_FETCH_CMD            (PARSER_CBUS_BASE+0x62)
    #define FETCH_CIR_BUF               (1<<31)
    #define FETCH_CHK_BUF_STOP          (1<<30)
    #define FETCH_PASSTHROUGH           (1<<29)
    #define FETCH_PASSTHROUGH_TYPE_MASK (0x3<<27)
    #define FETCH_ENDIAN_MASK           (0x7<<27)
    #define FETCH_BUF_SIZE_MASK         (0x7ffffff)
    #define FETCH_CMD_PTR_MASK          3
    #define FETCH_CMD_RD_PTR_BIT        5
    #define FETCH_CMD_WR_PTR_BIT        3
    #define FETCH_CMD_NUM_MASK          3
    #define FETCH_CMD_NUM_BIT           0

// 31:0 stop_fetch_addr
#define PARSER_FETCH_STOP_ADDR      (PARSER_CBUS_BASE+0x63)

// 31:0 stop_fetch_addr - current_write_addr  // read only
#define PARSER_FETCH_LEVEL          (PARSER_CBUS_BASE+0x64)

// bit 31 - fetch_dma_urgent
// bit 30 - stream_dma_urgent
// bit 29 - force_pfifo_ren
// bit 28 - pfifo_peak_enable
// bit 27:26 -
// bit 25:24 - parser_src_sel (00 - fetch, 01 - aux1, 10 - aux2 11 - aux3)
// bit 23:16 - pfifo_empty_count
// bit 15:12 - max_es_write_cycle (x16x64bits)
// 11:10 - start code width ( 00-8bits, 01-16bits, 10-24bits, 11-32bits )
// 9:8 - pfifo data access width ( 00-8bits, 01-16bits, 10-24bits, 11-32bits )
// 7:0 - empty room for fetch data ( max_fetch_cycle ) x64 bits
#define PARSER_CONFIG               (PARSER_CBUS_BASE+0x65)
    #define PS_CFG_FETCH_DMA_URGENT         (1<<31)
    #define PS_CFG_STREAM_DMA_URGENT        (1<<30)
    #define PS_CFG_FORCE_PFIFO_REN          (1<<29)
    #define PS_CFG_PFIFO_PEAK_EN            (1<<28)
    #define PS_CFG_SRC_SEL_BIT              24
    #define PS_CFG_SRC_SEL_MASK             (3<<PS_CFG_SRC_SEL_BIT)
    #define PS_CFG_SRC_SEL_FETCH            (0<<PS_CFG_SRC_SEL_BIT)
    #define PS_CFG_SRC_SEL_AUX1             (1<<PS_CFG_SRC_SEL_BIT) // from NDMA
    #define PS_CFG_SRC_SEL_AUX2             (2<<PS_CFG_SRC_SEL_BIT)
    #define PS_CFG_SRC_SEL_AUX3             (3<<PS_CFG_SRC_SEL_BIT)
    #define PS_CFG_PFIFO_EMPTY_CNT_BIT      16
    #define PS_CFG_PFIFO_EMPTY_CNT_MASK     0xff
    #define PS_CFG_MAX_ES_WR_CYCLE_BIT      12
    #define PS_CFG_MAX_ES_WR_CYCLE_MASK     0xf
    #define PS_CFG_STARTCODE_WID_MASK       (0x3<<10)
    #define PS_CFG_STARTCODE_WID_8          (0x0<<10)
    #define PS_CFG_STARTCODE_WID_16         (0x1<<10)
    #define PS_CFG_STARTCODE_WID_24         (0x2<<10)
    #define PS_CFG_STARTCODE_WID_32         (0x3<<10)
    #define PS_CFG_PFIFO_ACCESS_WID_MASK    (0x3<<8)
    #define PS_CFG_PFIFO_ACCESS_WID_8       (0x0<<8)
    #define PS_CFG_PFIFO_ACCESS_WID_16      (0x1<<8)
    #define PS_CFG_PFIFO_ACCESS_WID_24      (0x2<<8)
    #define PS_CFG_PFIFO_ACCESS_WID_32      (0x3<<8)
    #define PS_CFG_MAX_FETCH_CYCLE_BIT      0
    #define PS_CFG_MAX_FETCH_CYCLE_MASK     0xff

// bit 24:16 -- pfifo_level
// bit 9:0 -- point to byte address
#define PFIFO_WR_PTR            (PARSER_CBUS_BASE+0x66)
    #define PFIFO_WR_LEVEL_BIT   16
    #define PFIFO_BYTE_ADDR_BIT  0
    #define PFIFO_BYTE_ADDR_MASK 0x3ff

// bit 9:0 -- point to byte address
#define PFIFO_RD_PTR            (PARSER_CBUS_BASE+0x67)

// bit 31:0 -- 8/16/24/32 bits data acording to pfifo_data_width
#define PFIFO_DATA              (PARSER_CBUS_BASE+0x68)

// bit 31:0 -- parser search pattern
#define PARSER_SEARCH_PATTERN   (PARSER_CBUS_BASE+0x69)

// bit 31:0 -- parser search mask ( 0 - disable bit match test )
#define PARSER_SEARCH_MASK      (PARSER_CBUS_BASE+0x6a)

// bit 31:16 -- parser_int_disable_count
// bit 15:8 -- Parser interrupt enable for host cpu
// bit 7:0 -- Parser interrupt enable for Amrisc
#define PARSER_INT_ENABLE       (PARSER_CBUS_BASE+0x6b)
    #define PARSER_INT_DISABLE_CNT_MASK 0xffff
    #define PARSER_INT_DISABLE_CNT_BIT  16
    #define PARSER_INT_HOST_EN_MASK     0xff
    #define PARSER_INT_HOST_EN_BIT      8
    #define PARSER_INT_AMRISC_EN_MASK   0xff
    #define PARSER_INT_AMRISC_EN_BIT    0
	#define PARSER_INT_ALL				0xff
// Bit 7 -- fetch_cmd_finished
// Bit 6:5 -- reserved
// Bit 4 -- parse finished
// Bit 3 -- discard finished
// Bit 2 -- insert zero finished
// Bit 1 -- Action Finished Except Search Start Code
// Bit 0 -- Search match interrupt (StartCode found)
// Read 7:0 int status
// Write will clear (bit based)
#define PARSER_INT_STATUS       (PARSER_CBUS_BASE+0x6c)
    #define PARSER_INTSTAT_FETCH_CMD    (1<<7)
    #define PARSER_INTSTAT_PARSE        (1<<4)
    #define PARSER_INTSTAT_DISCARD      (1<<3)
    #define PARSER_INTSTAT_INSZERO      (1<<2)
    #define PARSER_INTSTAT_ACT_NOSSC    (1<<1)
    #define PARSER_INTSTAT_SC_FOUND     (1<<0)

// 15    - SCR_32_READ_OUT (Read Only)
// 14    - SCR_32_data_valid
// 13    - SCR_32_data
// 12    - SCR_count_en
// 11:0  - SCR90K_DIV
#define PARSER_SCR_CTL          (PARSER_CBUS_BASE+0x6d)
    #define PARSER_SCR_32_RD            (1<<15)
    #define PARSER_SCR_32_DATA_VALID    (1<<14)
    #define PARSER_SCR_32_DATA          (1<<13)
    #define PARSER_SCR_CNT_EN           (1<<12)
    #define PARSER_SCR90K_DIV_WID       12
    #define PARSER_SCR90K_DIV_BIT       0
    #define PARSER_SCR90K_DIV_MASK      0x7ff

// bit 31:0 SCR value
#define PARSER_SCR              (PARSER_CBUS_BASE+0x6e)

// bit 31:24 -- insert_loop_number
// bit 23:21 --
// bit 20:16 -- insert_byte_number
// bit 15:0 -- discard number
#define PARSER_PARAMETER        (PARSER_CBUS_BASE+0x6f)

// bit 31:0 -- insert data // write only
// write to PARSER_CONTROL will reset the write position
// continous write to this address can write upto 16 bytes
#define PARSER_INSERT_DATA      (PARSER_CBUS_BASE+0x70)

// Bit 31:24 -- Reserved Stream_ID
// Bit 23:16 -- Sub Stream_ID
// Bit 15:8  -- Audio Stream_ID
// Bit 7:0   -- Video Stream_ID
#define VAS_STREAM_ID           (PARSER_CBUS_BASE+0x71)

// Bit 31:0 -- VIDEO_DTS
#define VIDEO_DTS               (PARSER_CBUS_BASE+0x72)

// Bit 31:0 -- VIDEO_PTS
#define VIDEO_PTS               (PARSER_CBUS_BASE+0x73)

// Bit 31:0 -- VIDEO_PTS_DTS_WR_PTR
#define VIDEO_PTS_DTS_WR_PTR    (PARSER_CBUS_BASE+0x74)

// Bit 31:0 -- AUDIO_PTS
#define AUDIO_PTS               (PARSER_CBUS_BASE+0x75)

// Bit 31:0 -- AUDIO_PTS_WR_PTR
#define AUDIO_PTS_WR_PTR        (PARSER_CBUS_BASE+0x76)

// bit 31:20 -- stream_es_count ( Read Only)
// bit 19 -- req_pending ( parser ddr_A_fifo ) (Read Only)
// bit 18 -- stream_es_passthrough_enable
// bit 17:16 -- stream_es_passthrough_type
// bit 15:12 -- reserved
// bit 11:9 -- SUB Ees_write Endian
// bit 8 -- SUB using manual read point
// bit 7:5 -- AUDIO Ees_write Endian
// bit 4 -- AUDIO using manual read point
// bit 3:1 -- VIDEO Ees_write Endian
// bit 0 -- VIDEO using manual read point
#define PARSER_ES_CONTROL       (PARSER_CBUS_BASE+0x77)
    #define ES_COUNT_MASK                0xfff
    #define ES_COUNT_BIT                 20
    #define ES_REQ_PENDING               (1<<19)
    #define ES_PASSTHROUGH_EN            (1<<18)
    #define ES_PASSTHROUGH_TYPE_MASK     (3<<16)
    #define ES_PASSTHROUGH_TYPE_VIDEO    (0<<16)
    #define ES_PASSTHROUGH_TYPE_AUDIO    (1<<16)
    #define ES_PASSTHROUGH_TYPE_SUBTITLE (2<<16)
    #define ES_WR_ENDIAN_MASK            (0x7)
    #define ES_SUB_WR_ENDIAN_BIT         9
    #define ES_SUB_MAN_RD_PTR            (1<<8)
    #define ES_AUD_WR_ENDIAN_BIT         5
    #define ES_AUD_MAN_RD_PTR            (1<<4)
    #define ES_VID_WR_ENDIAN_BIT         1
    #define ES_VID_MAN_RD_PTR            (1<<0)

// Bit 31:0 PFIFO_MONITOR
#define PFIFO_MONITOR           (PARSER_CBUS_BASE+0x78)

// Stream buffer DDR interface
// The start pointer into DDR memory is a 32-bit number
// The Start pointer will automatically be truncated to land on
// an 8-byte boundary.  That is, bits [2:0] = 0;
#define PARSER_VIDEO_START_PTR  (PARSER_CBUS_BASE+0x80)
#define PARSER_VIDEO_END_PTR    (PARSER_CBUS_BASE+0x81)
#define PARSER_VIDEO_WP         (PARSER_CBUS_BASE+0x82)
#define PARSER_VIDEO_RP         (PARSER_CBUS_BASE+0x83)   // Manual or VLD read pointer
#define PARSER_VIDEO_HOLE       (PARSER_CBUS_BASE+0x84)

#define PARSER_AUDIO_START_PTR  (PARSER_CBUS_BASE+0x85)
#define PARSER_AUDIO_END_PTR    (PARSER_CBUS_BASE+0x86)
#define PARSER_AUDIO_WP         (PARSER_CBUS_BASE+0x87)
#define PARSER_AUDIO_RP         (PARSER_CBUS_BASE+0x88)   // Manual or AIFIFO read pointer
#define PARSER_AUDIO_HOLE       (PARSER_CBUS_BASE+0x89)

#define PARSER_SUB_START_PTR    (PARSER_CBUS_BASE+0x8a)
#define PARSER_SUB_END_PTR      (PARSER_CBUS_BASE+0x8b)
#define PARSER_SUB_WP           (PARSER_CBUS_BASE+0x8c)
#define PARSER_SUB_RP           (PARSER_CBUS_BASE+0x8d)   // Manual or SUB read pointer
#define PARSER_SUB_HOLE         (PARSER_CBUS_BASE+0x8e)

// -----------------------------------------------  
// CBUS_BASE:  DDR_CBUS_BASE = 0x13                 
// -----------------------------------------------  
#define DDR_CBUS_BASE 0x1300

//  DDR registers
#define DDR_PROT0               (DDR_CBUS_BASE+0x80)
#define DDR_PROT1               (DDR_CBUS_BASE+0x81)
#define DDR_PHY_CTRL            (DDR_CBUS_BASE+0x82)
#define DDR_PHY_DLLCTL          (DDR_CBUS_BASE+0x83)
#define DDR_TIMING_CTL0         (DDR_CBUS_BASE+0x85)
#define DDR_TIMING_CTL1         (DDR_CBUS_BASE+0x86)
#define DDR_TIMING_CTL2         (DDR_CBUS_BASE+0x87)
#define DDR_TIMING_CTL3         (DDR_CBUS_BASE+0x88)
#define DDR_TIMING_CTL4         (DDR_CBUS_BASE+0x89)
#define DDR_TIMING_CTL5         (DDR_CBUS_BASE+0x8a)
#define DDR_TIMING_CTL6         (DDR_CBUS_BASE+0x8b)
#define DDR_TIMING_CTL7         (DDR_CBUS_BASE+0x8c)
#define DDR_MRS                 (DDR_CBUS_BASE+0x8d)
#define DDR_EMRS                (DDR_CBUS_BASE+0x8e)
#define DDR_COMMAND             (DDR_CBUS_BASE+0x8f)
#define DDR_PROT_STATUS         (DDR_CBUS_BASE+0x90)
#define DDR_TOP_CTL0            (DDR_CBUS_BASE+0x91)
#define DDR_TOP_CTL1            (DDR_CBUS_BASE+0x92)
#define DDR_TOP_CTL2            (DDR_CBUS_BASE+0x93)
    #define DDRREQ_AHB          (1<<31)
    #define DDRREQ_ARC2         (1<<30)
    #define DDRREQ_PMEM         (1<<29)
    #define DDRREQ_LMEM         (1<<27)
    #define DDRREQ_AUDIN        (1<<26)
    #define DDRREQ_ARC1         (1<<25)
    #define DDRREQ_DVIN         (1<<24)
    #define DDRREQ_BT656IN      (1<<23)
    #define DDRREQ_PSCALE       (1<<22)
    #define DDRREQ_DI_CHAN2     (1<<21)
    #define DDRREQ_DI_RES       (1<<20)
    #define DDRREQ_PARSER       (1<<19)
    #define DDRREQ_MC_DEBLK     (1<<18)
    #define DDRREQ_OSD2         (1<<17)
    #define DDRREQ_OSD1         (1<<16)
    #define DDRREQ_DI_MR        (1<<15)
    #define DDRREQ_DI_MW        (1<<14)
    #define DDRREQ_DI_NR        (1<<13)
    #define DDRREQ_DI_IF        (1<<12)
    #define DDRREQ_DI_MF        (1<<11)
    #define DDRREQ_DI_CHAN1     (1<<10)
    #define DDRREQ_VPU_V2       (1<<9)
    #define DDRREQ_VPU_V1       (1<<8)
    #define DDRREQ_MPEG_IQIDCT  (1<<7)
    #define DDRREQ_MPEG_VLD     (1<<6)
    #define DDRREQ_AIFIFO       (1<<5)
    #define DDRREQ_IEC958       (1<<4)
    #define DDRREQ_I2S          (1<<3)
    #define DDRREQ_GE2D_WR      (1<<2)
    #define DDRREQ_GE2D_SRC1    (1<<1)
    #define DDRREQ_GE2D_SRC0    (1<<0)
#define DDR_RESET               (DDR_CBUS_BASE+0x95)
#define DDR_200US_BASE          (DDR_CBUS_BASE+0x96)
#define DDR_ARB_BURST_CRL0      (DDR_CBUS_BASE+0xa0)
#define DDR_ARB_BURST_CRL1      (DDR_CBUS_BASE+0xa1)
#define DDR_ARB_BURST_CRL2      (DDR_CBUS_BASE+0xa2)
#define DDR_ARB_BURST_CRL3      (DDR_CBUS_BASE+0xa3)

#if 1
/* VLSI define 
#define DC_CAV_CTRL                                0x13c0
#define DC_CAV_LVL3_GRANT                          0x13c1
#define DC_CAV_LVL3_CTRL0                          0x13c2
#define DC_CAV_LVL3_CTRL1                          0x13c3
#define DC_CAV_LVL3_CTRL2                          0x13c4
#define DC_CAV_LVL3_CTRL3                          0x13c5
#define DC_CAV_LUT_DATAL                           0x13c6
#define DC_CAV_LUT_DATAH                           0x13c7
#define DC_CAV_LUT_ADDR                            0x13c8
#define DC_CAV_LVL3_FLIP                           0x13c9
#define DC_CAV_LVL3_GH                             0x13ca
#define DC_CAV_LVL3_FH                             0x13cb
#define DC_CAV_LVL3_MODE                           0x13cc
#define DC_CAV_LUT_D2L                             0x13cd
#define DC_CAV_LUT_D2H                             0x13ce
*/

#define DDR_CANVAS_CTL               0x1300
    #define LVL3LUT_RANGE_MASK      0x7
    #define LVL3LUT_RANGE_WID       3
    #define LVL3LUT_RANGE_BIT       0
    #define LVL3LUT_MODE_MASK       0x7
    #define LVL3LUT_MODE_WID        3
    #define LVL3LUT_MODE_BIT        3

#define DDR_CANVAS_GRANT         0x1304
    #define ARC_REQUEST_BIT         25

#define DDR_CAV_LVL3_GH            0x1308
  // this is a 32 bit grant regsiter.
  // each bit grant a thread ID for LVL3 use.

#define DDR_CANVAS_DATA_FLIP          0x130c
#define DDR_CAV_LVL3_FH            0x1310
  // this is a 32 bit FLIP regsiter.
  // each bit to define  a thread ID for LVL3 use.

#define DDR_CANVAS_IDX0         0x1314
#define DDR_CANVAS_IDX1         0x1318
#define DDR_CANVAS_IDX2         0x131c
#define DDR_CANVAS_IDX3         0x1320
#define DDR_CANVAS_LUT_DATAL          0x1324
    #define CANVAS_ADDR_LMASK       0x1fffffff
    #define CANVAS_WIDTH_LMASK		0x7
    #define CANVAS_WIDTH_LWID		3
    #define CANVAS_WIDTH_LBIT		29
#define DDR_CANVAS_LUT_DATAH          0x1328
	#define CANVAS_WIDTH_HMASK		0x1ff
	#define CANVAS_WIDTH_HBIT		0
	#define CANVAS_HEIGHT_MASK		0x1fff
	#define CANVAS_HEIGHT_BIT		9
	#define CANVAS_YWRAP			(1<<23)
	#define CANVAS_XWRAP			(1<<22)
    #define CANVAS_ADDR_NOWRAP      0x00
    #define CANVAS_ADDR_WRAPX       0x01
    #define CANVAS_ADDR_WRAPY       0x02
    #define CANVAS_BLKMODE_MASK     3
    #define CANVAS_BLKMODE_BIT      24
    #define CANVAS_BLKMODE_LINEAR   0x00
    #define CANVAS_BLKMODE_32X32    0x01
    #define CANVAS_BLKMODE_64X32    0x02
#define DDR_CANVAS_LUT_ADDR           0x132c
    #define CANVAS_LUT_INDEX_BIT    0
    #define CANVAS_LUT_INDEX_MASK   0x7
    #define CANVAS_LUT_WR_EN        (0x2 << 8)
    #define CANVAS_LUT_RD_EN        (0x1 << 8)
#define DDR_CAV_LVL3_MODE          0x1330

#else

#define DDR_CANVAS_CTL          (DDR_CBUS_BASE+0xc0)
    #define LVL3LUT_RANGE_MASK      0x7
    #define LVL3LUT_RANGE_WID       3
    #define LVL3LUT_RANGE_BIT       0
    #define LVL3LUT_MODE_MASK       0x7
    #define LVL3LUT_MODE_WID        3
    #define LVL3LUT_MODE_BIT        3
#define DDR_CANVAS_GRANT        (DDR_CBUS_BASE+0xc1)
    #define ARC_REQUEST_BIT         25
#define DDR_CANVAS_IDX0         (DDR_CBUS_BASE+0xc2)
#define DDR_CANVAS_IDX1         (DDR_CBUS_BASE+0xc3)
#define DDR_CANVAS_IDX2         (DDR_CBUS_BASE+0xc4)
#define DDR_CANVAS_IDX3         (DDR_CBUS_BASE+0xc5)
#define DDR_CANVAS_LUT_DATAL    (DDR_CBUS_BASE+0xc6)
    #define CANVAS_ADDR_LMASK       0x1fffffff
    #define CANVAS_WIDTH_LMASK		0x7
    #define CANVAS_WIDTH_LWID		3
    #define CANVAS_WIDTH_LBIT		29
    
#define DDR_CANVAS_LUT_DATAH	(DDR_CBUS_BASE+0xc7)
	#define CANVAS_WIDTH_HMASK		0x1ff
	#define CANVAS_WIDTH_HBIT		0
	#define CANVAS_HEIGHT_MASK		0x1fff
	#define CANVAS_HEIGHT_BIT		9
	#define CANVAS_YWRAP			(1<<23)
	#define CANVAS_XWRAP			(1<<22)
    #define CANVAS_BLKMODE_MASK     3
    #define CANVAS_BLKMODE_BIT      24

#define DDR_CANVAS_LUT_ADDR     (DDR_CBUS_BASE+0xc8)
    #define CANVAS_LUT_INDEX_BIT    0
    #define CANVAS_LUT_INDEX_MASK   0x7
    #define CANVAS_LUT_WR_EN        (0x2 << 8)
    #define CANVAS_LUT_RD_EN        (0x1 << 8)
#define DDR_CANVAS_DATA_FLIP    (DDR_CBUS_BASE+0xc9)
#define DDR_CAV_LVL3_GH         (DDR_CBUS_BASE+0xca)
#define DDR_CAV_LVL3_FH         (DDR_CBUS_BASE+0xcb)
#define DDR_CAV_LVL3_MODE       (DDR_CBUS_BASE+0xcc)

#endif
//========================================================================
//  DVIN Registers                  (0x2e0 - 0x2ef)
//========================================================================
#define DVIN_CBUS_BASE 0x1200

// Bit 31:11 Reserved
    #define  VS_TIM_CTRL            10// Bit 10: 9 RW, vs_hs_tim_ctrl
    #define  HS_TIM_CTRL            9
    #define  SAMPLE_CLK_NEG         8// Bit     8 RW, sample_clk_neg
    #define  MODE_422               7// Bit     7 RW, mode_422
    #define  DE_MODE                5  // Bit  6: 5 RW, de_mode
    #define  EXT_FIELD_SEL          4 // Bit     4 RW, ext_field_sel
    #define  FIELD_POL_INV          3  // Bit     3 RW, field_pol_inv, , controls the polarity of input pin FIELD. Internally DVIN regards FIELD as active high signal to indicate odd field
    #define  DE_POL_INV             2    // Bit     2 RW, de_pol_inv                                         
    #define  VS_POL_INV             1  // Bit     1 RW, vs_pol_inv
    #define  HS_POL_INV             0// Bit     0 RW, hs_pol_inv
#define DVIN_FRONT_END_CTRL     (DVIN_CBUS_BASE+0xe0)

// Bit 31:28 Reserved
    #define  HS_LEAD_VS_ODD_MAX     16// Bit 27:16 RW, hs_lead_vs_odd_max
// Bit 15:12 Reserved
    #define  HS_LEAD_VS_ODD_MIN     0// Bit 11: 0 RW, hs_lead_vs_odd_min
#define DVIN_HS_LEAD_VS_ODD     (DVIN_CBUS_BASE+0xe1)

// Bit 31:28 Reserved
    #define  ACTIVE_START_PIX_FO    16// Bit 27:16 RW, active_start_pix_fo
// Bit 15:12 Reserved
    #define  ACTIVE_START_PIX_FE    0// Bit 11: 0 RW, active_start_pix_fe
#define DVIN_ACTIVE_START_PIX   (DVIN_CBUS_BASE+0xe2)

// Bit 31:28 Reserved
    #define  ACTIVE_START_LINE_FO   16// Bit 27:16 RW, active_start_line_fo
// Bit 15:12 Reserved
    #define  ACTIVE_START_LINE_FE   0// Bit 11: 0 RW, active_start_line_fe
#define DVIN_ACTIVE_START_LINE  (DVIN_CBUS_BASE+0xe3)

// Bit 31:28 Reserved
    #define  LINE_WIDTH_M1          0// Bit 27:16 RW, field_height_m1
// Bit 15:12 Reserved
    #define  FIELD_HEIGHT_M1        16// Bit 11: 0 RW, line_width_m1
#define DVIN_DISPLAY_SIZE       (DVIN_CBUS_BASE+0xe4)

// Bit 31:30 Reserved
    #define  MULTIPLE_FIFO_FEND     29// Bit    29 R,  multiple_fifo_fend
    #define  FIFO_OVERFLOW          28// Bit    28 R,  fifo_overflow
    #define  HS_LEAD_VS_CNT         16// Bit 27:16 R,  hs_lead_vs_cnt
// Bit 15: 5 Reserved
    #define  DDR_BURST_SIZE         4// Bit     4 RW, ddr_burst_size -- 0=default,32 per burst; 1=64 per burst
    #define  CLEAR_ERR              3// Bit     3 W,  clear_err (a pulse)
    #define  URGENT                 2// Bit     2 RW, urgent
    #define  FIFO_RESET             1// Bit     1 RW, fifo_reset
    #define  DVIN_ENABLE            0// Bit     0 RW, dvin_enable
#define DVIN_CTRL_STAT         (DVIN_CBUS_BASE+0xe5)

// Bit 31: 7 Reserved
    #define   NXT_TBL_ADDR         0x7f// Bit  6: 0 RW, nxt_tbl_addr
#define DVIN_VCANV_TBL_ADDR    (DVIN_CBUS_BASE+0xe6)

// Bit 31:29 Reserved
    #define  NXT_Y_START_ADDR      16 // Bit 28:16 RW, nxt_y_start_addr
// Bit 15:12 Reserved
    #define  NXT_X_START_ADDR      0 // Bit 11: 0 RW, nxt_x_start_addr
#define DVIN_VCANV_XY_START_ADDR (DVIN_CBUS_BASE+0xe7)

//========================================================================
//  CPU Registers                   (0x300 - 0x3ff)
//========================================================================
// CPU1 registers are 0x30x
#define CPU1_CBUS_BASE 0x300

#define MREG_SP                 (CPU1_CBUS_BASE+0x00)
#define MREG_PSR                (CPU1_CBUS_BASE+0x01)
#define MREG_INT_VEC_BASE       (CPU1_CBUS_BASE+0x02)
#define MREG_CPU_INTR_GRP       (CPU1_CBUS_BASE+0x03)
#define MREG_CPU_INTR_MSK       (CPU1_CBUS_BASE+0x04)
#define MREG_CPU_INTR_REQ       (CPU1_CBUS_BASE+0x05)
#define MREG_PC_P               (CPU1_CBUS_BASE+0x06)
#define MREG_PC_D               (CPU1_CBUS_BASE+0x07)
#define MREG_PC_E               (CPU1_CBUS_BASE+0x08)
#define MREG_PC_W               (CPU1_CBUS_BASE+0x09)
#define MREG_INDEX0_REG         (CPU1_CBUS_BASE+0x0a)
#define MREG_INDEX1_REG         (CPU1_CBUS_BASE+0x0b)
#define MREG_INDEX2_REG         (CPU1_CBUS_BASE+0x0c)
#define MREG_INDEX3_REG         (CPU1_CBUS_BASE+0x0d)
#define MREG_INDEX4_REG         (CPU1_CBUS_BASE+0x0e)
#define MREG_INDEX5_REG         (CPU1_CBUS_BASE+0x0f)
#define MREG_INDEX6_REG         (CPU1_CBUS_BASE+0x10)
#define MREG_INDEX7_REG         (CPU1_CBUS_BASE+0x11)
#define MREG_MIN_REG            (CPU1_CBUS_BASE+0x12)
#define MREG_MAX_REG            (CPU1_CBUS_BASE+0x13)
#define MREG_BREAK0_REG         (CPU1_CBUS_BASE+0x14)
#define MREG_BREAK1_REG         (CPU1_CBUS_BASE+0x15)
#define MREG_BREAK2_REG         (CPU1_CBUS_BASE+0x16)
#define MREG_BREAK3_REG         (CPU1_CBUS_BASE+0x17)
#define MREG_BREAK_TYPE         (CPU1_CBUS_BASE+0x18)
#define MREG_BREAK_CTRL         (CPU1_CBUS_BASE+0x19)
#define MREG_BREAK_STAUTS       (CPU1_CBUS_BASE+0x1a)
#define MREG_DB_ADDR_REG        (CPU1_CBUS_BASE+0x1b)
#define MREG_DB_DATA_REG        (CPU1_CBUS_BASE+0x1c)
#define MREG_DB_CTRL            (CPU1_CBUS_BASE+0x1d)
#define MREG_SFTINT0            (CPU1_CBUS_BASE+0x1e)
#define MREG_SFTINT1            (CPU1_CBUS_BASE+0x1f)

#define MREG_CSP                (CPU1_CBUS_BASE+0x20)
#define MREG_CPSR               (CPU1_CBUS_BASE+0x21)
#define MREG_CINT_VEC_BASE      (CPU1_CBUS_BASE+0x22)
#define MREG_CCPU_INTR_GRP      (CPU1_CBUS_BASE+0x23)
#define MREG_CCPU_INTR_MSK      (CPU1_CBUS_BASE+0x24)
#define MREG_CCPU_INTR_REQ      (CPU1_CBUS_BASE+0x25)
#define MREG_CPC_P              (CPU1_CBUS_BASE+0x26)
#define MREG_CPC_D              (CPU1_CBUS_BASE+0x27)
#define MREG_CPC_E              (CPU1_CBUS_BASE+0x28)
#define MREG_CPC_W              (CPU1_CBUS_BASE+0x29)
#define MREG_CINDEX0_REG        (CPU1_CBUS_BASE+0x2a)
#define MREG_CINDEX1_REG        (CPU1_CBUS_BASE+0x2b)
#define MREG_CINDEX2_REG        (CPU1_CBUS_BASE+0x2c)
#define MREG_CINDEX3_REG        (CPU1_CBUS_BASE+0x2d)
#define MREG_CINDEX4_REG        (CPU1_CBUS_BASE+0x2e)
#define MREG_CINDEX5_REG        (CPU1_CBUS_BASE+0x2f)
#define MREG_CINDEX6_REG        (CPU1_CBUS_BASE+0x30)
#define MREG_CINDEX7_REG        (CPU1_CBUS_BASE+0x31)
#define MREG_CMIN_REG           (CPU1_CBUS_BASE+0x32)
#define MREG_CMAX_REG           (CPU1_CBUS_BASE+0x33)
#define MREG_CBREAK0_REG        (CPU1_CBUS_BASE+0x34)
#define MREG_CBREAK1_REG        (CPU1_CBUS_BASE+0x35)
#define MREG_CBREAK2_REG        (CPU1_CBUS_BASE+0x36)
#define MREG_CBREAK3_REG        (CPU1_CBUS_BASE+0x37)
#define MREG_CBREAK_TYPE        (CPU1_CBUS_BASE+0x38)
#define MREG_CBREAK_CTRL        (CPU1_CBUS_BASE+0x39)
#define MREG_CBREAK_STAUTS      (CPU1_CBUS_BASE+0x3a)
#define MREG_CDB_ADDR_REG       (CPU1_CBUS_BASE+0x3b)
#define MREG_CDB_DATA_REG       (CPU1_CBUS_BASE+0x3c)
#define MREG_CDB_CTRL           (CPU1_CBUS_BASE+0x3d)
#define MREG_CSFTINT0           (CPU1_CBUS_BASE+0x3e)
#define MREG_CSFTINT1           (CPU1_CBUS_BASE+0x3f)
#define MREG_IMEM_DMA_CTRL      (CPU1_CBUS_BASE+0x40)
#define MREG_IMEM_DMA_ADR       (CPU1_CBUS_BASE+0x41)
#define MREG_IMEM_DMA_COUNT     (CPU1_CBUS_BASE+0x42)
#define MREG_LMEM_DMA_CTRL      (CPU1_CBUS_BASE+0x50)
#define MREG_LMEM_DMA_ADR       (CPU1_CBUS_BASE+0x51)
#define MREG_LMEM_DMA_COUNT     (CPU1_CBUS_BASE+0x52)
#define MREG_MAC_CTRL1          (CPU1_CBUS_BASE+0x60)
#define MREG_ACC0REG1           (CPU1_CBUS_BASE+0x61)
#define MREG_ACC1REG1           (CPU1_CBUS_BASE+0x62)

//========================================================================
//LCD DRV     0x480~0x4ef
//=======================================================================
// ----------------------------------------------- 
// CBUS_BASE:  LCD_CBUS_BASE = 0x14                
// ----------------------------------------------- 
#define LCD_CBUS_BASE   0x1400
#define GAMMA_CNTL_PORT            (LCD_CBUS_BASE+0x80) 
   #define  GAMMA_VCOM_POL    7     //RW
   #define  GAMMA_RVS_OUT     6     //RW
   #define  ADR_RDY           5     //Read Only
   #define  WR_RDY            4     //Read Only
   #define  RD_RDY            3     //Read Only
   #define  GAMMA_TR          2     //RW
   #define  GAMMA_SET         1     //RW
   #define  GAMMA_EN          0     //RW
#define GAMMA_DATA_PORT            (LCD_CBUS_BASE+0x81)
#define GAMMA_ADDR_PORT            (LCD_CBUS_BASE+0x82)
   #define  H_RD              12
   #define  H_AUTO_INC        11
   #define  H_SEL_R           10
   #define  H_SEL_G           9
   #define  H_SEL_B           8
   #define  HADR_MSB          7            //7:0
   #define  HADR              0            //7:0
#define GAMMA_VCOM_HSWITCH_ADDR    (LCD_CBUS_BASE+0x83)

#define RGB_BASE_ADDR            (LCD_CBUS_BASE+0x85)
#define RGB_COEFF_ADDR           (LCD_CBUS_BASE+0x86)
#define POL_CNTL_ADDR            (LCD_CBUS_BASE+0x87)
   #define   DCLK_SEL             14    //FOR DCLK OUTPUT
   #define   TCON_VSYNC_SEL_DVI   11    //FOR RGB format DVI output
   #define   TCON_HSYNC_SEL_DVI   10    //FOR RGB format DVI output
   #define   TCON_DE_SEL_DVI      9     //FOR RGB format DVI output
   #define   CPH3_POL         8
   #define   CPH2_POL         7
   #define   CPH1_POL         6
   #define   TCON_DE_SEL      5
   #define   TCON_VS_SEL      4
   #define   TCON_HS_SEL      3
   #define   DE_POL           2
   #define   VS_POL           1
   #define   HS_POL           0

   
#define DITH_CNTL_ADDR            (LCD_CBUS_BASE+0x88)
   #define  DITH10_EN         10
   #define  DITH8_EN          9
   #define  DITH_MD           8
   #define  DITH10_CNTL_MSB   7          //7:4
   #define  DITH10_CNTL       4          //7:4
   #define  DITH8_CNTL_MSB    3          //3:0
   #define  DITH8_CNTL        0          //3:0

#define STH1_HS_ADDR            (LCD_CBUS_BASE+0x90)
#define STH1_HE_ADDR            (LCD_CBUS_BASE+0x91)
#define STH1_VS_ADDR            (LCD_CBUS_BASE+0x92)
#define STH1_VE_ADDR            (LCD_CBUS_BASE+0x93)

#define STH2_HS_ADDR            (LCD_CBUS_BASE+0x94)
#define STH2_HE_ADDR            (LCD_CBUS_BASE+0x95)
#define STH2_VS_ADDR            (LCD_CBUS_BASE+0x96)
#define STH2_VE_ADDR            (LCD_CBUS_BASE+0x97)

#define OEH_HS_ADDR             (LCD_CBUS_BASE+0x98)
#define OEH_HE_ADDR             (LCD_CBUS_BASE+0x99)
#define OEH_VS_ADDR             (LCD_CBUS_BASE+0x9a)
#define OEH_VE_ADDR             (LCD_CBUS_BASE+0x9b)

#define VCOM_HSWITCH_ADDR       (LCD_CBUS_BASE+0x9c)
#define VCOM_VS_ADDR            (LCD_CBUS_BASE+0x9d)
#define VCOM_VE_ADDR            (LCD_CBUS_BASE+0x9e)
#define CPV1_HS_ADDR            (LCD_CBUS_BASE+0x9f)

#define CPV1_HE_ADDR            (LCD_CBUS_BASE+0xa0)
#define CPV1_VS_ADDR            (LCD_CBUS_BASE+0xa1)
#define CPV1_VE_ADDR            (LCD_CBUS_BASE+0xa2)
#define CPV2_HS_ADDR            (LCD_CBUS_BASE+0xa3)

#define CPV2_HE_ADDR            (LCD_CBUS_BASE+0xa4)
#define CPV2_VS_ADDR            (LCD_CBUS_BASE+0xa5)
#define CPV2_VE_ADDR            (LCD_CBUS_BASE+0xa6)
#define STV1_HS_ADDR            (LCD_CBUS_BASE+0xa7)

#define STV1_HE_ADDR            (LCD_CBUS_BASE+0xa8)
#define STV1_VS_ADDR            (LCD_CBUS_BASE+0xa9)
#define STV1_VE_ADDR            (LCD_CBUS_BASE+0xaa)
#define STV2_HS_ADDR            (LCD_CBUS_BASE+0xab)

#define STV2_HE_ADDR            (LCD_CBUS_BASE+0xac)
#define STV2_VS_ADDR            (LCD_CBUS_BASE+0xad)
#define STV2_VE_ADDR            (LCD_CBUS_BASE+0xae)
#define OEV1_HS_ADDR            (LCD_CBUS_BASE+0xaf)

#define OEV1_HE_ADDR            (LCD_CBUS_BASE+0xb0)
#define OEV1_VS_ADDR            (LCD_CBUS_BASE+0xb1)
#define OEV1_VE_ADDR            (LCD_CBUS_BASE+0xb2)
#define OEV2_HS_ADDR            (LCD_CBUS_BASE+0xb3)

#define OEV2_HE_ADDR            (LCD_CBUS_BASE+0xb4)
#define OEV2_VS_ADDR            (LCD_CBUS_BASE+0xb5)
#define OEV2_VE_ADDR            (LCD_CBUS_BASE+0xb6)
#define OEV3_HS_ADDR            (LCD_CBUS_BASE+0xb7)

#define OEV3_HE_ADDR            (LCD_CBUS_BASE+0xb8)
#define OEV3_VS_ADDR            (LCD_CBUS_BASE+0xb9)
#define OEV3_VE_ADDR            (LCD_CBUS_BASE+0xba)

#define LCD_PWR_ADDR            (LCD_CBUS_BASE+0xbb) 
   #define      LCD_VDD        5
   #define      LCD_VBL        4
   #define      LCD_GPI_MSB    3
   #define      LCD_GPIO       0

#define LCD_PWM0_LO_ADDR        (LCD_CBUS_BASE+0xbc) 
#define LCD_PWM0_HI_ADDR        (LCD_CBUS_BASE+0xbd) 
#define LCD_PWM1_LO_ADDR        (LCD_CBUS_BASE+0xbe) 
#define LCD_PWM1_HI_ADDR        (LCD_CBUS_BASE+0xbf) 

#define INV_CNT_ADDR            (LCD_CBUS_BASE+0xc0)
   #define     INV_EN          4
   #define     INV_CNT_MSB     3
   #define     INV_CNT         0

#define TCON_MISC_SEL_ADDR      (LCD_CBUS_BASE+0xc1)
   #define     STH2_SEL        12
   #define     STH1_SEL        11
   #define     OEH_SEL         10
   #define     VCOM_SEL         9
   #define     DB_LINE_SW       8
   #define     CPV2_SEL         7
   #define     CPV1_SEL         6
   #define     STV2_SEL         5
   #define     STV1_SEL         4
   #define     OEV_UNITE        3
   #define     OEV3_SEL         2
   #define     OEV2_SEL         1
   #define     OEV1_SEL         0

#define DUAL_PORT_CNTL_ADDR      (LCD_CBUS_BASE+0xc2)
   #define     OUTPUT_YUV       15
   #define     DUAL_IDF         12   // 14:12
   #define     DUAL_ISF         9    // 11:9
   #define     LCD_ANALOG_SEL_CPH3   8
   #define     LCD_ANALOG_3PHI_CLK_SEL   7
   #define     LCD_LVDS_SEL54   6
   #define     LCD_LVDS_SEL27   5
   #define     LCD_TTL_SEL      4
   #define     DUAL_LVDC_EN     3
   #define     PORT_SWP         2
   #define     RGB_SWP          1
   #define     BIT_SWP          0

#ifdef AML_A3
#else
#define TCON_JITTER0            (LCD_CBUS_BASE+0xc8)
#define TCON_JITTER1            (LCD_CBUS_BASE+0xc9)
#define TCON_JITTER_TAB0        (LCD_CBUS_BASE+0xca)
#define TCON_JITTER_TAB1        (LCD_CBUS_BASE+0xcb)
#define TCON_JITTER_TAB2        (LCD_CBUS_BASE+0xcc)
#define TCON_JITTER_TAB3        (LCD_CBUS_BASE+0xcd)
#define TCON_JITTER_TAB4        (LCD_CBUS_BASE+0xce)
#define TCON_JITTER_TAB5        (LCD_CBUS_BASE+0xcf)
#endif

#define LVDS_PACK_CNTL_ADDR      (LCD_CBUS_BASE+0xd0)
   #define     LD_CNT_MSB       7
   #define     LD_CNT           5
   #define     PN_SWP           4
   #define     RES              3
   #define     LVDS_PORT_SWP    2
   #define     PACK_RVS         1
   #define     PACK_LITTLE      0

#define DE_HS_ADDR                (LCD_CBUS_BASE+0xd1)
#define DE_HE_ADDR                (LCD_CBUS_BASE+0xd2)
#define DE_VS_ADDR                (LCD_CBUS_BASE+0xd3)
#define DE_VE_ADDR                (LCD_CBUS_BASE+0xd4)

#define HSYNC_HS_ADDR             (LCD_CBUS_BASE+0xd5)
#define HSYNC_HE_ADDR             (LCD_CBUS_BASE+0xd6)
#define HSYNC_VS_ADDR             (LCD_CBUS_BASE+0xd7)
#define HSYNC_VE_ADDR             (LCD_CBUS_BASE+0xd8)

#define VSYNC_HS_ADDR             (LCD_CBUS_BASE+0xd9)
#define VSYNC_HE_ADDR             (LCD_CBUS_BASE+0xda)
#define VSYNC_VS_ADDR             (LCD_CBUS_BASE+0xdb)
#define VSYNC_VE_ADDR             (LCD_CBUS_BASE+0xdc)

// bit 8 -- vfifo_mcu_enable
// bit 7 -- halt_vs_de
// bit 6 -- R8G8B8_format
// bit 5 -- R6G6B6_format (round to 6 bits)
// bit 4 -- R5G6B5_format
// bit 3 -- dac_dith_sel
// bit 2 -- lcd_mcu_enable_de     -- ReadOnly
// bit 1 -- lcd_mcu_enable_vsync  -- ReadOnly
// bit 0 -- lcd_mcu_enable
#define LCD_MCU_CTL               (LCD_CBUS_BASE+0xdd)
// ReadOnly 
//   R5G6B5 when R5G6B5_format
//   G8R8   when R8G8B8_format
//   G5R10  Other
#define LCD_MCU_DATA_0            (LCD_CBUS_BASE+0xde)
// ReadOnly
//   G8B8   when R8G8B8_format
//   G5B10  Other
#define LCD_MCU_DATA_1            (LCD_CBUS_BASE+0xdf)

//========================================================================
//  Audio Interface                 (0x500 - 0x5ff)
//========================================================================
// -----------------------------------------------
// CBUS_BASE:  AIU_CBUS_BASE = 0x15               
// -----------------------------------------------
#define AIU_CBUS_BASE 0x1500

// Number of bytes expected per NON-PCM frame, including the header.
// 16 bits.
#define MREG_AIU_958_bpf             (AIU_CBUS_BASE+0x00)

// Value of burst_info used in IEC958 preamble for a NON-PCM frame. See
// ATSC AC-3 spec Annex B. 16 bits.
#define MREG_AIU_958_brst            (AIU_CBUS_BASE+0x01)

// Value of length_code used in IEC958 preamble for an NON-PCM frame. See
// ATSC AC-3 spec Annex B. 16 bits.
#define MREG_AIU_958_length          (AIU_CBUS_BASE+0x02)

// How many subframes to padd around the NON-PCM frame. This will
// be set to 'd3072 if it is a AC-3 audio stream.
// 16 bits
#define MREG_AIU_958_paddsize        (AIU_CBUS_BASE+0x03)
// 16'd3072

// Misc. control settings for IEC958 interface
// Bit 15:14 pcm_sample_ctl, 00-pcm_no_sample, 01-pcm_sample_up, 10-pcm_sample_down, 11-pcm_sample_down_drop
// Bit 13: if true, force each audio data to left or right according to the bit attached with the audio data
//         This bit should be used with Register MREG_AIU_958_force_left(0x505) together
// Bit 12: if true, the U(user data) is from the stream otherwise it is filled by
//                   zero while encoding iec958 frame
// Bit 11   : if true big endian(highword,lowword) otherwise little endian(lowword,highword)
//            for 32bit mode
// Bit 10:8 : shift number for 32 bit mode
// Bit 7  : 32 bit mode turn on while This bit is true and Bit 1 is true
// Bit 6:5 : Specifies output alignment for 16 bit pcm data.
//          00 : dout = {8'b0, din};
//          01 : dout = {4'b0, din, 4'b0};
//          10 : dout = {      din, 8'b0};
// Bit 4  : True if data should be sent out MSB first. LSB first is the
//          default in the spec.
// Bit 3  : True if msb should be extended (only used with 16 bit pcm data.)
// Bit 2  : True if msb of PCM data should be inverted.
// Bit 1  : True if PCM data is 16 bits wide. False if 24 bit or 32bit mode.
// Bit 0  : True if source data is non-PCM data. False if it is PCM data.
#define MREG_AIU_958_misc            (AIU_CBUS_BASE+0x04)
// 14'b0_0_0_000_0_0000001

// A write to this register specifies that the next pcm sample sent out
// of the iec958 interface should go into the _left_ channel.
#define MREG_AIU_958_force_left      (AIU_CBUS_BASE+0x05)


//Read Only
//bit 6:0  how many data discarded in the last dma after one frame data finish transfering to AIU
// should used together with register MREG_AIU_958_dcu_ff_ctrl
#define MREG_AIU_958_discard_num     (AIU_CBUS_BASE+0x06)

//bit 15:8  : A read from this register indicates the IEC958 FIFO count value
//bit 7 :  ai_958_req_size if ture, set to 8 bits interface, used to handle odd frame continous read
//bit 6 :  continue seeking and dont discard the rest data in one dma after frame end
//bit 5 :  if true, byte by byte seeking, otherwise word by word seeking
//bit 4 :  if true, the function for sync head seeking is enabled
//bit 3:2 : IEC958 interrupt mode
// There are two conditions to generate interrupt. The First condition is that one frame data
// have been finished writing into FIFO. The second condition is at least some data of the current
// frame have been read out(refer to Register MREG_AIU_958_ffrdout_thd)
//          00: interrupt is not generated
//          10: interrupt is generated if the first condition is true
//          01: interrupt is generated if the second condition is true
//          11: interrupt is generated if both of the conditions are true
//bit 1: fifo auto disable, High means after one frame data put into the FIFO, the FIFO
//                  will automatically disabled
//bit 0: fifo enable
#define MREG_AIU_958_dcu_ff_ctrl     (AIU_CBUS_BASE+0x07)
// 'b0_0_1_11_1_0

// channel status registers for Left channel
// chstat_l0[15:0] contains bits 15:0 of the channel status word. Note
// that bit zero of the channel status word is sent out first.
// chstat_l1[15:0] contains bits 31:16 of the channel status word.
#define MREG_AIU_958_chstat_l0       (AIU_CBUS_BASE+0x08)
#define MREG_AIU_958_chstat_l1       (AIU_CBUS_BASE+0x09)

// Control register for IEC958 interface
// Bit 9:8 what to do if there is a fifo underrun
//         00 => insert 0x000000
//         01 => insert mute constant as defined below
//         10 => repeat last l/r samples
// Bit 7:5 mute constant
//         000 => 0x000000
//         001 => 0x800000
//         010 => 0x080000
//         011 => 0x008000
//         100 => 0x000001
//         101 => 0x000010
//         110 => 0x000100
// Bit 4   mute left speaker
// Bit 3   mute right speaker
// Bit 2:1 swap channels
//         00 : L R => L R
//         01 : L R => L L
//         10 : L R => R R
//         11 : L R => R L
// Bit 0   Set this bit to hold iec958 interface after the current
//         subframe has been completely transmitted.
#define MREG_AIU_958_ctrl            (AIU_CBUS_BASE+0x0a)
// 10'b00_000_00_00_0

// A write operation to this register will cause one of the output samples
// to be repeated. This can be used to switch the left and the right
// channels.
#define MREG_AIU_958_rpt             (AIU_CBUS_BASE+0x0b)

// Channel swap and mute control register.
// Bit 15: Mute extra left channel
// Bit 14: Mute extra right channel
// Bit 13: Mute center
// Bit 12: Mute subwoofer
// Bit 11: Mute surround left
// Bit 10: Mute surround right
// Bit  9: Mute left
// Bit  8: Mute right
// Bit 7:6 Channel swap for xtra dac
// Bit 5:4 Channel swap for sub/center dac
// Bit 3:2 Channel swap for surround dac
// Bit 1:0 Channel swap for main l/r dac
// Channel swap possibilities:
// 00 : L R => L R
// 01 : L R => L L
// 10 : L R => R R
// 11 : L R => R L
#define MREG_AIU_i2s_mute_swap       (AIU_CBUS_BASE+0x0c)
// 16'b0

// Bit 0 : 0=> single two channel stream
//         1=> four two channel streams
#define MREG_AIU_i2s_source_desc     (AIU_CBUS_BASE+0x0d)

// Median filter control register
// Bit 1:  0=> data is offset binary
//         1=> data is signed
// Bit 0:  enable median filter
#define MREG_AIU_i2s_med_ctrl        (AIU_CBUS_BASE+0x0e)
// 2'b10

// Median filter threshold constant
// 16 bits
#define MREG_AIU_i2s_med_thresh      (AIU_CBUS_BASE+0x0f)
// 0x8000

// Describes dac's connected to I2S interface
// Bit 7:  sign extend sample before downshift.
// Bit 6:4 payload downshift constant
// Bit 3:  mute constant
//         0 => 0x0000000
//         1 => 0x800000
// Bit 2:  send msb first
// Bit 1:0 Size of payload
//         Note that this parameter used to be called "size of dacs"
//         It is now called the payload size since a 24 bit payload can
//         actually be connected to any size of dac. The dac will simply
//         disregard the extra bits.
//         00 => 16 bit, alrclk = aoclk/32
//         01 => 20 bit, alrclk = aoclk/40
//         10 => 24 bit, alrclk = aoclk/48
//         11 => 24 bit, but alrclk = aoclk/64
#define MREG_AIU_i2s_dac_cfg         (AIU_CBUS_BASE+0x10)
// 8'b0_000_1_1_10

// A write to this register will cause the interface to repeat the current
// sample. Can be used to regain synchronization.
// A read from this register indicates that the next sample to be sent
// out of the interface should go into the _left_ channel of the dac.
#define MREG_AIU_i2s_sync            (AIU_CBUS_BASE+0x11)

// Misc regs
// Bit 4 if true, force each audio data to left or right according to the bit attached with the audio data
//         This bit should be used with Register MREG_AIU_i2s_sync(0x511) together
// Bit 3:  Same Audio source for IEC958 and I2s stream 0, both from i2s buffer
// Bit 2:  Set this bit to put i2s interface in hold mode
// Bit 1:0 How to handle underruns
//         00 => send zeros
//         01 => send 0x800000
//         10 => repeat last samples
#define MREG_AIU_i2s_misc            (AIU_CBUS_BASE+0x12)
// 5'b0

// Bit 7:0 Audio output config.
//          2 bits for each dac, 7:6 for dac3, 5:4 for dac2,
//                               3:2 for dac1, 1:0 for dac0
//          For each 2bits: 00: connect channel0-1 to the dac
//                          01: connect channel2-3 to the dac
//                          10: connect channel4-5 to the dac
//                          11: connect channel6-7 to the dac
#define MREG_AIU_i2s_out_cfg         (AIU_CBUS_BASE+0x13)
// 8'b0_000000_0

// A write to this register resets the AIU
//Bit 3 reset slow domain iec958
//Bit 2 soft reset iec958 fast domain
//Bit 1 reset slow domain i2s
//Bit 0 soft reset i2s fast domain
#define MREG_AIU_rst_soft            (AIU_CBUS_BASE+0x15)

// Clock generation control register
// Bit 15: enable_ddr_arb, set low to reset
// Bit 14:13 parser_A_addr_sel  00-A_addr_aififo2, 01-A_addr_iec958, 10-A_addr_aififo, 11-A_addr_i2s
// Bit 12: 958 divisor more, if true, divided by 2, 4, 6, 8
// Bit 11: amclk output divisor
//          0 => dont divide
//          1 => divide by 2
// Bit 10: clock source selection
//        0 => aiclk from pin
//        1 => ai_pll_clk from pll
// Bit 9:8 alrclk skew
//         00 => alrclk transitions at the same time msb is sent
//         01 => alrclk transitions on the cycle before msb is sent
//         10 => alrclk transitions on the cycle after msb is sent
// Bit 7: invert alrclk
// Bit 6: invert aoclk
// Bit 5:4 958 divisor
//        00 => divide by 1
//        01 => divide by 2
//        10 => divide by 3
//        11 => divide by 4
// Bit 3:2 i2s divisor
//         00 => divide by 1
//         01 => divide by 2
//         10 => divide by 4
//         11 => divide by 8
// Bit 1: enable 958 divider
// Bit 0: enable i2s divider
#define MREG_AIU_clk_ctrl            (AIU_CBUS_BASE+0x16)
	#define AIU_CLK_PARSER_HOLE_TARGET_AIFIFO2	0
	#define AIU_CLK_PARSER_HOLE_TARGET_IEC958	1
	#define AIU_CLK_PARSER_HOLE_TARGET_AIFIFO	2
	#define AIU_CLK_PARSER_HOLE_TARGET_I2S   	3
	#define AIU_CLK_PARSER_HOLE_TARGET_BIT		13
	#define AIU_CLK_PARSER_HOLE_TARGET_WID		2
// 13'b0_0_1_01_1_1_10_11_0_0

// Misc settings that determine the type of adc that is
// connected to the AIU. Reverb mode is deleted.
// Bit 12:  selects adc input
// Bit 11:10 adc size
//           00 => 16 bits
//           01 => 18 bits
//           10 => 20 bits
//           11 => 24 bits
// Bit 9:8   adc l/r swap mode
//           00 => stereo
//           01 => send the right adc input to both l and r speakers
//           01 => send the left adc input to both l and r speakers
//           11 => sum the left and right inputs and forward to
//                both speakers
// Bit 7:5 adata/lrclk skew mode
// Bit 4   1=>invert the adc's lrclk (This is the lrclk going _out_
//         of the chip.
// Bit 3   1=>Latch the data on the positive edge of the _internal_
//         aoclk.
// Bit 2   1=>adc data is in signed 2's complement mode
#define MREG_AIU_mix_adccfg          (AIU_CBUS_BASE+0x17)
// 12'b01_00_001_1_0_1_00

// Control register that can be changed dynamically. These control
// signals are synchronized internally.
// Bit 12:  if true, toggle each mixed audio data to left or right channel
// Bit 11:  abuf din left selection, if true, select bit 24 of the data from abuf
//          otherwise select bit 25 of the data from abuf
// Bit 10:9  mix sync select, when music, mic and abuf are mixed togather, the main
//           sync source can be selected
//     00: not sync source
//     01: music data is the main sync source
//     10: abuf input data is the main sync source
//     11: music and abuf togather as the sync source
// Bit 8:  0=> data from abuf is offset binary
//         1=> data from abuf is signed
// Bit 7:6 the source for data from aiu to abuf
//           00 => mic
//           01 => mic saled + abuf scaled
//           10 => mic scaled + abuf scaled + music scaled
//           11 => music
// Bit 5   channel from aiu to abuf is on
// Bit 4   channel from abuf to aiu is on
// Bit 3   mic is on
// Bit 2   music is on
// Bit 1   if true the mixed data are outputed to i2s dac channel,
//          otherwise the mixed data are outputed to IEC958 output
// Bit 0   if true music source for mixing is from i2s buffer,
//          otherwise music source is from iec958 buffer
#define MREG_AIU_mix_ctrl            (AIU_CBUS_BASE+0x18)
// 11'b01_1_01_0_0_0_1_1_1
#define MREG_AIU_clk_ctrl_more   (AIU_CBUS_BASE+0x19)
// A read from this register pops 16 bits of data off the 958
// fifo. A write has no effect.
#define MREG_AIU_958_pop             (AIU_CBUS_BASE+0x1a)


//gain register for mixing
// for each gain,
//           00000: x 0
//           00001: x 1
//           00010: x 2
//           00011: x 3
//           00100: x 4
//           00101: x 5
//           00110: x 6
//          ......
//           01110: x14
//           01111: x15
//           10000: x 0
//           10001: x 1/16
//           10010: x 2/16
//           10011: x 3/16
//           10100: x 4/16
//          ......
//           11110: x 14/16
//           11111: x 15/16
//Bit 14:10 mic gain
//Bit 9:5   abuf gain
//Bit 4:0 music gain
#define MREG_AIU_mix_gain            (AIU_CBUS_BASE+0x1b)
// 15'b00001_00001_00001

//sync head seeking is supported. The  maxinium length of sync head is
//48-bit-wide.(in byte by byte seeking mode, the maximium is 44-bit-wide).
//It is consisted of 3 words (synword1_synword2_syncword3).
//You can configure the sync head pattern by using sync mask(mask1_mask2_mask3).
//For example. AC-3 sync head is a 16-bit word(0b77), so syncword1 is set as 0b77,
//mask1 is 0000(not set), mask2 and mask3 is all masked.
#define MREG_AIU_958_SYNWORD1        (AIU_CBUS_BASE+0x1c)
//0x0b77  //ac-3 sync head
#define MREG_AIU_958_SYNWORD2        (AIU_CBUS_BASE+0x1d)
//0x0000
#define MREG_AIU_958_SYNWORD3        (AIU_CBUS_BASE+0x1e)
//0x0000
#define MREG_AIU_958_SYNWORD1_MASK   (AIU_CBUS_BASE+0x1f)
//0x0000 //16-bit ac-3 sync_head
#define MREG_AIU_958_SYNWORD2_MASK   (AIU_CBUS_BASE+0x20)
//0xffff
#define MREG_AIU_958_SYNWORD3_MASK   (AIU_CBUS_BASE+0x21)
//0xffff

//fifo read-out threshold, one condition to generate interrupt is met after fifo readout counter
//reach this value in a frame, please refer to register MREG_AIU_958_dcu_ff_ctrl
#define MREG_AIU_958_ffrdout_thd     (AIU_CBUS_BASE+0x22)
//0x0004

//For pause burst sequence adding, one pause burst sequence is consist of a serious
// pause burst.
//This register defines the length of each pause burst in a pause burst sequence. The size of the
//preamble(Pa, Pb, Pc, Pd) is not counted, but the size of stuff data is counted.
#define MREG_AIU_958_length_per_pause (AIU_CBUS_BASE+0x23)
//0x0000

//This reigster defines the number of pause burst in a pause burst sequence.
//Bit 15  if true, one pause burst sequence will be added
//Bit 14:0 the number of pause burst in a pause burst sequence
#define MREG_AIU_958_pause_num       (AIU_CBUS_BASE+0x24)
//0x0000

//The first 16-bit in the payload of pause burst sequence (gap_length)
#define MREG_AIU_958_pause_payload   (AIU_CBUS_BASE+0x25)
//0x0000

//For auto pause function, when enabled, pause burst sequence will be automatically added if the data in
// 958 fifo is less than auto_pause threshold and if fifo is disabled.
//Bit 15   if true, auto pause function enable
//Bit 14   pause pack option, just for debugging and adding one option
//Bit 7:0  auto_pause threshold
#define MREG_AIU_958_auto_pause      (AIU_CBUS_BASE+0x26)
//0x0000

//pause burst sequence payload length( = MREG_AIU_958_pause_num * MREG_AIU_958_length_per_pause)
#define MREG_AIU_958_pause_pd_length (AIU_CBUS_BASE+0x27)
//0x0000

#define MREG_AIU_CODEC_DAC_LRCLK_CTRL      (AIU_CBUS_BASE+0x28)

// channel status registers for right channel
// chstat_r0[15:0] contains bits 15:0 of the channel status word. Note
// that bit zero of the channel status word is sent out first.
// chstat_r1[15:0] contains bits 31:16 of the channel status word.
#define MREG_AIU_958_chstat_r0       (AIU_CBUS_BASE+0x30)
#define MREG_AIU_958_chstat_r1       (AIU_CBUS_BASE+0x31)

//Bit 1     if true, turn on Digital output Valid control
//Bit 0     0: output 0, 1: output 1 to the valid bit in audio digital output when Bit 1 is true
#define MREG_AIU_958_valid_ctrl      (AIU_CBUS_BASE+0x32)

//Bit 3     CRC pop aififo2 enable
//Bit 2     writing to this bit to 1 causes CRC module reset
//Bit 1     unused
//Bit 0     writing to this bit to 1 causes AIFIFO2 soft reset
#define MREG_AIU_AIFIFO2_CTRL        (AIU_CBUS_BASE+0x40)
//0x0000

//AIFIFO2 status register
//Bit 4:0       //how many bits left in the first pop register
#define MREG_AIU_AIFIFO2_STATUS      (AIU_CBUS_BASE+0x41)

//Same fucntion as the AIGBIT of AIFIFO in CDROM module
//write to this register how many bits wanna pop,
//and reading this register gets the corresponding bits data
#define MREG_AIU_AIFIFO2_GBIT        (AIU_CBUS_BASE+0x42)

//Same function as the AICLB of AIFIFO in CDROM module
//return the leading zeros by reading this registers
#define MREG_AIU_AIFIFO2_CLB         (AIU_CBUS_BASE+0x43)

//CRC control register, read/write
//Bit 0     CRC caculation start
//Bit 1     CRC core soft reset
//Bit 2     CRC input register clear
//Bit 3     CRC pop data from FIFO enable
//Bit 13:8  CRC polynomial equation order, between 1 to 32
#define MREG_AIU_CRC_CTRL            (AIU_CBUS_BASE+0x44)
//0x1000

//CRC status register, read only
//Bit 7:4   CRC internal shift register bit select, just for debug purpose
//Bit 3     CRC internal shift register data valid, just for debug purpose
//Bit 2     CRC input register data valid
//Bit 1     CRC result, 1: CRC not correct, 0: CRC correct
//Bit 0     CRC state,  1: CRC busy, 0: CRC idle
#define MREG_AIU_CRC_STATUS          (AIU_CBUS_BASE+0x45)

//CRC internal shift register, read only, for debug purpose
#define MREG_AIU_CRC_SHIFT_REG       (AIU_CBUS_BASE+0x46)

//CRC data input register, read/write
#define MREG_AIU_CRC_IREG            (AIU_CBUS_BASE+0x47)
//0x0000

//CRC calculation register high-bit part [31:16], read/write
#define MREG_AIU_CRC_CAL_REG1        (AIU_CBUS_BASE+0x48)
//CRC calculation register low-bit part [15:0], read/write
#define MREG_AIU_CRC_CAL_REG0        (AIU_CBUS_BASE+0x49)
//0x00000000

//CRC polynomial coefficient high-bit part [31:16], read/write
#define MREG_AIU_CRC_POLY_COEF1      (AIU_CBUS_BASE+0x4a)
//CRC polynomial coefficient low-bit part [15:0], read/write
#define MREG_AIU_CRC_POLY_COEF0      (AIU_CBUS_BASE+0x4b)
//0x80050000            default CRC-16

//CRC frame size, high-bit part [19:16], read/write
#define MREG_AIU_CRC_BIT_SIZE1       (AIU_CBUS_BASE+0x4c)
//CRC frame size, low-bit part [15:0],   read/write
#define MREG_AIU_CRC_BIT_SIZE0       (AIU_CBUS_BASE+0x4d)
//0xfffff

//how many bits have been processed right now in the current frame, read only
//high-bit part [19:16]
#define MREG_AIU_CRC_BIT_CNT1        (AIU_CBUS_BASE+0x4e)
//low-bit part [15:0]
#define MREG_AIU_CRC_BIT_CNT0        (AIU_CBUS_BASE+0x4f)

// Used to measure the amclk frequency
#define MREG_AIU_AMCLK_GATE_HI       (AIU_CBUS_BASE+0x50)
#define MREG_AIU_AMCLK_GATE_LO       (AIU_CBUS_BASE+0x51)
#define MREG_AIU_AMCLK_MSR           (AIU_CBUS_BASE+0x52)
#define AIU_AUDAC_CTRL0              (AIU_CBUS_BASE+0x53)             
#define AIU_AUDAC_CTRL1              (AIU_CBUS_BASE+0x54)

// Super Simple Delta Sigma DAC
// bit 15   1 = invert the clock to the analog filter
// bit 14   1 = digital mute
// bit 13   1 = enable analog mute (in the analog block)
// bit 12   1 = enable Right channel 3 in the analog block
// bit 11   1 = enable Left  channel 3 in the analog block
// bit 10   1 = enable Right channel 2 in the analog block
// bit  9   1 = enable Left  channel 2 in the analog block
// bit  8   1 = enable Right channel 1 in the analog block
// bit  7   1 = enable Left  channel 1 in the analog block
// bit  6   1 = enable Right channel 0 in the analog block
// bit  5   1 = enable Left  channel 0 in the analog block
// bit  4:  bit select for serial input data
// bit  3:  invert alrclk used by the delta-sigma DAC
// bit  2:  1 = use serial I2S data.  0 = use parallel audio data
// bits 1:0 Which data to send to the delta-sigma DAC
//              11 = use channel 3 data
//              10 = use channel 2 data
//              01 = use channel 1 data
//              00 = use channel 0 data
#define MREG_AIU_DELTA_SIGMA0        (AIU_CBUS_BASE+0x55)
// Delta Sigma MUTE Value
#define MREG_AIU_DELTA_SIGMA1        (AIU_CBUS_BASE+0x56)

// I2S DDR Interface
// The I2S start pointer into DDR memory is a 32-bit number
#define MREG_AIU_MEM_I2S_START_PTR   (AIU_CBUS_BASE+0x60)
#define MREG_AIU_MEM_I2S_RD_PTR      (AIU_CBUS_BASE+0x61)
#define MREG_AIU_MEM_I2S_END_PTR     (AIU_CBUS_BASE+0x62)
// There are two masks that control how data is read:
// [31:16] IRQ block.
// [15:8] chan_mem_mask.  Each bit indicates which channels exist in memory
// [7:0]  chan_rd_mask.   Each bit indicates which channels are READ from memory
#define MREG_AIU_MEM_I2S_MASKS       (AIU_CBUS_BASE+0x63)
// I2S FIFO Control
// bits [11:10] Select which hardware pointer to use to control the buffer
//              level:
//                  00 = parser
//                  01 = audin_fifo0_wrpt
//                  1x = audin_fifo1_wrpt
// bit  [9]     Use level control: 1 = use buffer level control
// bit  [8]     Read Only.  This bit is 1 when there is data available for reading
// bit  [7]     Read only.  This bit will be high when we're fetching data from the DDR memory
//              To reset this module, set cntl_enable = 0, and then wait for busy = 0.
//              After that you can pulse cntl_init to start over
// bit  [6]     cntl_mode_16bit:Set to 1 for 16 bit storage format in DDR
// bits [5:3]   endian:  see $lib/rtl/ddr_endian.v
// bit  [2]     cntl_empty_en   Set to 1 to enable reading data from the FIFO
// bit  [1]     cntl_fill_en    Set to 1 to enable reading data from DDR memory
// bit  [0]     cntl_init:      After setting the read pointers, sizes, channel masks
//                              and read masks, set this bit to 1 and then to 0
#define MREG_AIU_MEM_I2S_CONTROL     (AIU_CBUS_BASE+0x64)

// IEC958 DDR Interface
// The IEC958 start pointer into DDR memory is a 32-bit number
#define MREG_AIU_MEM_IEC958_START_PTR    (AIU_CBUS_BASE+0x65)
#define MREG_AIU_MEM_IEC958_RD_PTR   (AIU_CBUS_BASE+0x66)
#define MREG_AIU_MEM_IEC958_END_PTR  (AIU_CBUS_BASE+0x67)

// There are two masks that control how data is read:
// [15:8] chan_mem_mask.  Each bit indicates which channels exist in memory
// [7:0]  chan_rd_mask.   Each bit indicates which channels are READ from memory
#define MREG_AIU_MEM_IEC958_MASKS    (AIU_CBUS_BASE+0x68)
// IEC958 FIFO Control
// bit  [31]    A_urgent
// bit  [30]    ch_always_8
// bit  [29:24] rdata_rd_base_begin ( used for select from different channel )
// bit  [23:14] reserved
// bit  [13]    cntl_sim_en
// bit  [12]    cntl_use_level
// bit  [11]    Read only.      This bit will be set to 1 when there is data in the FIFO to process
// bit  [10]    Read only.  This bit will be high when we're fetching data from the DDR memory
//              To reset this module, set cntl_enable = 0, and then wait for busy = 0.
//              After that you can pulse cntl_init to start over
// bit  [9]     cntl_endian_jic Just in case endian.  last minute byte swap of the data out of
//                              the FIFO to the rest of the IEC958 logic
// bit  [8]     mode_raw:       Set this bit to 1 to tell the IEC958 FIFO to read
//                              and process data linearly for raw data.
// bit  [7]     cntl_mode_16bit:Set to 1 for 16 bit storage format in DDR.  Only valid when mode_raw = 0
// bit  [6]     cntl_rd_ddr     Set this bit to read if you want MREG_AIU_MEM_IEC958_RD_PTR and
//                              MREG_AIU_MEM_IEC958_RD_PTR_HIGH to refer to the pointer into DDR memory.
//                              Otherwise, the curr_ptr registers refer to the byte address of the data
//                              at the output of the FIFO to the rest of the IEC958 logic
// bits [5:3]   endian:         see $lib/rtl/ddr_endian.v
// bit  [2]     cntl_empty_en   Set to 1 to enable reading the DDR memory FIFO and filling the pipeline to get-bit
//                              Set cntl_empty_en = cntl_fill_en = 0 when pulsing cntl_init
// bit  [1]     cntl_fill_en    Set to 1 to enable reading data from DDR memory
// bit  [0]     cntl_init:      After setting the read pointers, sizes, channel masks
//                              and read masks, set this bit to 1 and then to 0
#define MREG_AIU_MEM_IEC958_CONTROL      (AIU_CBUS_BASE+0x69)

// AIFIFO2 DDR Interface
// The AIFIFO2 start pointer into DDR memory is a 32-bit number
// The Start pointer will automatically be truncated to land on
// an 8-byte boundary.  That is, bits [2:0] = 0;
#define MREG_AIU_MEM_AIFIFO2_START_PTR   (AIU_CBUS_BASE+0x6a)
// The current pointer points so some location between the START and END
// pointers.  The current pointer is a BYTE pointer.  That is, you can
// point to any BYTE address within the START/END range
#define MREG_AIU_MEM_AIFIFO2_CURR_PTR    (AIU_CBUS_BASE+0x6b)
#define MREG_AIU_MEM_AIFIFO2_END_PTR     (AIU_CBUS_BASE+0x6c)
#define MREG_AIU_MEM_AIFIFO2_BYTES_AVAIL (AIU_CBUS_BASE+0x6d)
// AIFIFO2 FIFO Control
// bit  [15:11] unused
// bit  [10]    use_level       Set this bit to 1 to enable filling of the FIFO controlled by the buffer
//                              level control.  If this bit is 0, then use bit[1] to control the enabling of filling
// bit  [9]     Data Ready.     This bit is set when data can be popped
// bit  [8]     fill busy       This bit will be high when we're fetching data from the DDR memory
//                              To reset this module, set cntl_enable = 0, and then wait for busy = 0.
//                              After that you can pulse cntl_init to start over
// bit  [7]     cntl_endian_jic Just in case endian.  last minute byte swap of the data out of
//                              the FIFO to getbit
// bit  [6]     unused
// bits [5:3]   endian:         see $lib/rtl/ddr_endian.v
// bit  [2]     cntl_empty_en   Set to 1 to enable reading the DDR memory FIFO and filling the pipeline to get-bit
//                              Set cntl_empty_en = cntl_fill_en = 0 when pulsing cntl_init
// bit  [1]     cntl_fill_en    Set to 1 to enable reading data from DDR memory
// bit  [0]     cntl_init:      After setting the read pointers, sizes, channel masks
//                              and read masks, set this bit to 1 and then to 0
//                              NOTE:  You don't need to pulse cntl_init if only the start address is
//                              being changed
#define MREG_AIU_MEM_AIFIFO2_CONTROL     (AIU_CBUS_BASE+0x6e)

// AIFIFO2 Buffer Level Manager
#define MREG_AIU_MEM_AIFIFO2_MAN_WP      (AIU_CBUS_BASE+0x6f) // Manual or Parser write pointer
#define MREG_AIU_MEM_AIFIFO2_MAN_RP      (AIU_CBUS_BASE+0x70)
#define MREG_AIU_MEM_AIFIFO2_LEVEL       (AIU_CBUS_BASE+0x71) // must be low first to shadow low/high 16-bit words
//
// bit  [1]     manual mode     Set to 1 for manual write pointer mode
// bit  [0]     Init            Set high then low after everything has been initialized
#define MREG_AIU_MEM_AIFIFO2_BUF_CNTL    (AIU_CBUS_BASE+0x72)

// I2S Buffer Level Manager
#define MREG_AIU_MEM_I2S_MAN_WP          (AIU_CBUS_BASE+0x73) // Manual or Parser write pointer
#define MREG_AIU_MEM_I2S_MAN_RP          (AIU_CBUS_BASE+0x74)
#define MREG_AIU_MEM_I2S_LEVEL           (AIU_CBUS_BASE+0x75) // must be low first to shadow low/high 16-bit words

//
// bit  [2:1]   mode            0 = parser, 1 for manual write pointer mode, 2 = martin
//                              write pointer #1, 3 = martin write pointer #2
// bit  [0]     Init            Set high then low after everything has been initialized
#define MREG_AIU_MEM_I2S_BUF_CNTL        (AIU_CBUS_BASE+0x76)

#define MREG_AIU_MEM_IEC958_IRQ_LEVEL    (AIU_CBUS_BASE+0x7b)
#define MREG_AIU_MEM_IEC958_MAN_WP       (AIU_CBUS_BASE+0x7c) // Manual or Parser write pointer
#define MREG_AIU_MEM_IEC958_MAN_RP       (AIU_CBUS_BASE+0x7d)
#define MREG_AIU_MEM_IEC958_LEVEL        (AIU_CBUS_BASE+0x7e)
#define MREG_AIU_MEM_IEC958_BUF_CNTL     (AIU_CBUS_BASE+0x7f)

// Bit 3 CRC pop aififo enable
// Bit 2 writing to this bit to 1 causes CRC module reset
// Bit 1 enable aififo
// Bit 0 writing to this bit to 1 causes aififo soft reset
#define MREG_AIU_AIFIFO_CTRL             (AIU_CBUS_BASE+0x80)
//0x0000

// AIFIFO status register
// Bit 13       //aififo request to dcu status
// Bit 12       //dcu select status
// Bit 11:5     //aififo word counter number
// Bit 4:0      //how many bits left in the first pop register
#define MREG_AIU_AIFIFO_STATUS           (AIU_CBUS_BASE+0x81)

// Same fucntion as the AIGBIT of AIFIFO in CDROM module
// write to this register how many bits wanna pop,
// and reading this register gets the corresponding bits data
#define MREG_AIU_AIFIFO_GBIT             (AIU_CBUS_BASE+0x82)

// Same function as the AICLB of AIFIFO in CDROM module
// return the leading zeros by reading this registers
#define MREG_AIU_AIFIFO_CLB              (AIU_CBUS_BASE+0x83)

// AIFIFO DDR Interface
// The AIFIFO start pointer into DDR memory is a 32-bit number
// The Start pointer will automatically be truncated to land on
// an 8-byte boundary.  That is, bits [2:0] = 0;
#define AIU_MEM_AIFIFO_START_PTR         (AIU_CBUS_BASE+0x84)
// The current pointer points so some location between the START and END
// pointers.  The current pointer is a BYTE pointer.  That is, you can
// point to any BYTE address within the START/END range
#define AIU_MEM_AIFIFO_CURR_PTR          (AIU_CBUS_BASE+0x85)
#define AIU_MEM_AIFIFO_END_PTR           (AIU_CBUS_BASE+0x86)
#define AIU_MEM_AIFIFO_BYTES_AVAIL       (AIU_CBUS_BASE+0x87)
// AIFIFO FIFO Control
// bit  [15:11] unused
// bit  [10]    use_level       Set this bit to 1 to enable filling of the FIFO controlled by the buffer
//                              level control.  If this bit is 0, then use bit[1] to control the enabling of filling
// bit  [9]     Data Ready.     This bit is set when data can be popped
// bit  [8]     fill busy       This bit will be high when we're fetching data from the DDR memory
//                              To reset this module, set cntl_enable = 0, and then wait for busy = 0.
//                              After that you can pulse cntl_init to start over
// bit  [7]     cntl_endian_jic Just in case endian.  last minute byte swap of the data out of
//                              the FIFO to getbit
// bit  [6]     unused
// bits [5:3]   endian:         see $lib/rtl/ddr_endian.v
// bit  [2]     cntl_empty_en   Set to 1 to enable reading the DDR memory FIFO and filling the pipeline to get-bit
//                              Set cntl_empty_en = cntl_fill_en = 0 when pulsing cntl_init
// bit  [1]     cntl_fill_en    Set to 1 to enable reading data from DDR memory
// bit  [0]     cntl_init:      After setting the read pointers, sizes, channel masks
//                              and read masks, set this bit to 1 and then to 0
//                              NOTE:  You don't need to pulse cntl_init if only the start address is
//                              being changed
#define AIU_MEM_AIFIFO_CONTROL          (AIU_CBUS_BASE+0x88)

// AIFIFO Buffer Level Manager
#define AIU_MEM_AIFIFO_WP               (AIU_CBUS_BASE+0x89) // Manual or Parser write pointer
#define AIU_MEM_AIFIFO_RP               (AIU_CBUS_BASE+0x8a)
#define AIU_MEM_AIFIFO_LEVEL            (AIU_CBUS_BASE+0x8b) // must be low first to shadow low/high 16-bit words
//
// bit  [1]     manual mode     Set to 1 for manual write pointer mode
// bit  [0]     Init            Set high then low after everything has been initialized
#define AIU_MEM_AIFIFO_BUF_CNTL         (AIU_CBUS_BASE+0x8c)

//========================================================================
//  CDROM Interface                                 (0x600 - 0x6ff)
//========================================================================
// -----------------------------------------------
// CBUS_BASE:  INTERFACE_CBUS_BASE = 0x17               
// -----------------------------------------------
#define INTERFACE_CBUS_BASE 0x1700

#define AIADR   (INTERFACE_CBUS_BASE+0x38)
#define AICSR   (INTERFACE_CBUS_BASE+0x39)
#define AIDAT   (INTERFACE_CBUS_BASE+0x3a)  // Cpu writes to the AIFIFO
#define AIGBIT  (INTERFACE_CBUS_BASE+0x3b)  // Cpu writes getbit amount to this
#define AICLB   (INTERFACE_CBUS_BASE+0x3c)

// -----------------------------------------------
// CBUS_BASE:  STB_CBUS_BASE = 0x16
// -----------------------------------------------
#define STB_CBUS_BASE 0x1600
//#define SECOND_DEMUX_OFFSET_0  0x50
//#define SECOND_DEMUX_OFFSET_1  0x50
//#define THIRD_DEMUX_OFFSET_0   0xa0

/* address from 0x650-0x66f is used for stb_demux_0 */
#define MREG_STB_HD0                      (STB_CBUS_BASE+0x80)
#define MREG_STB_HD1                      (STB_CBUS_BASE+0x81) // Header Reg 1
#define MREG_STB_SHD0                     (STB_CBUS_BASE+0x82) // Sub-Header Reg 0
#define MREG_STB_SHD1                     (STB_CBUS_BASE+0x83) // Sub-Header Reg 1
#define MREG_STB_SYND                     (STB_CBUS_BASE+0x84) // Syndrome 0
#define MREG_STB_ECDCT                    (STB_CBUS_BASE+0x85) // ECC&EDC control reg (cdr_creg)
#define MREG_STB_ECDSTAT                  (STB_CBUS_BASE+0x86) // Location etc
#define MREG_STB_CTR0                     (STB_CBUS_BASE+0x87) // Syndrome etc
#define MREG_STB_CTR1                     (STB_CBUS_BASE+0x88) //
#define MREG_STB_CTR2                     (STB_CBUS_BASE+0x89) //
#define MREG_STB_STAT0                    (STB_CBUS_BASE+0x8a) //
#define MREG_STB_INT                      (STB_CBUS_BASE+0x8b) // Interrupt requests & masks reg
#define MREG_STB_TCTR0                    (STB_CBUS_BASE+0x8c) //
#define MREG_STB_TSTAT0                   (STB_CBUS_BASE+0x8d) //
#define MREG_STB_TSTAT1                   (STB_CBUS_BASE+0x8e) //

//========================================================================
//  STB Registers Start
//========================================================================
// -----------------------------------------------
//#define STB_CBUS_BASE           0x1600
// -----------------------------------------------
// There are two instantiations under one CBUS slave.  Each CBUS slave can support
// 256 registers.  Each demux is allocated 128 registers so set the offset in
// the middle
// Copy this define but don't add a base address
#define DEMUX_1_OFFSET         0x00
#define DEMUX_2_OFFSET         0x50
#define DEMUX_3_OFFSET         0xa0
//========================================================================
//  STB TOP Registers				    (8'hf0 - 8'hf7)
//========================================================================
// bit 22 -- invert fec_error for S2P1
// bit 21 -- invert fec_data for S2P1
// bit 20 -- invert fec_sync for S2P1
// bit 19 -- invert fec_valid for S2P1
// bit 18 -- invert fec_clk for S2P1
// bit 17:16 -- fec_s_sel for S2P1  00 - select TS0, 01 -- select TS1, 10 -- select TS2, 11 - reserved
// Bit 15 -- enable_des_pl_clk
// Bit 14:13 -- reserved
// Bit 12:10 -- ts_out_select, 0-TS0, 1-TS1, 2-TS2, 3,4-Reserved, 5-S2P1, 6-S2P0, 7-File
// bit 9:8 -- des_i_sel 00 -- select_fec_0, 01 -- select_fec_1, 10 -- select_fec_2, 11 - reserved
// bit 7 -- enable_des_pl
// bit 6 -- invert fec_error for S2P0
// bit 5 -- invert fec_data for S2P0
// bit 4 -- invert fec_sync for S2P0
// bit 3 -- invert fec_valid for S2P0
// bit 2 -- invert fec_clk for S2P0
// bit 1:0 -- fec_s_sel for S2P0  00 - select TS0, 01 -- select TS1, 10 -- select TS2, 11 - reserved
#define STB_TOP_CONFIG          (STB_CBUS_BASE + 0xf0) // 0x16f0
    /*----------- bit define -----------*/
    #define INVERT_S2P1_FEC_ERROR       22
    #define INVERT_S2P1_FEC_DATA        21
    #define INVERT_S2P1_FEC_SYNC        20
    #define INVERT_S2P1_FEC_VALID       19
    #define INVERT_S2P1_FEC_CLK         18
    #define S2P1_FEC_SERIAL_SEL         16
    #define ENABLE_DES_PL_CLK           15
    #define TS_OUTPUT_SOURCE            10
    #define DES_INPUT_SEL               8
    #define ENABLE_DES_PL               7
    #define INVERT_S2P0_FEC_ERROR       6
    #define INVERT_S2P0_FEC_DATA        5
    #define INVERT_S2P0_FEC_SYNC        4
    #define INVERT_S2P0_FEC_VALID       3
    #define INVERT_S2P0_FEC_CLK         2
    #define S2P0_FEC_SERIAL_SEL         0

// 15:8 -- TS_package_length_sub_1 (default : 187)
//  7:0 -- fec_sync_byte (default : 0x47)
#define TS_TOP_CONFIG           (STB_CBUS_BASE + 0xf1) // 0x16f1
    /*----------- bit define -----------*/
    #define TS_PACKAGE_LENGTH_SUB_1     8
    #define FEC_DEFAULT_SYNC_BYTE       0

// Bit 23:16 -- file_m2ts_skip_bytes
// Bit 15:8 -- des_out_dly
// Bit 7:6 -- transport_scrambling_control_odd // should be 3
// Bit 5 -- ts_hiu_enable
// Bit 4:0 -- fec_clk_div
#define TS_FILE_CONFIG          (STB_CBUS_BASE + 0xf2) // 0x16f2
    /*----------- bit define -----------*/
    #define FILE_M2TS_SKIP_BYTES        16
    #define DES_OUT_DLY                 8
    #define TRANSPORT_SCRAMBLING_CONTROL_ODD 6
    #define TS_HIU_ENABLE               5
    #define FEC_FILE_CLK_DIV            0

// Bit 13:8 -- des ts pl state   -- Read Only
// Bit 3:0 PID index to 8 PID to get key-set
// auto increse after TS_PL_PID_DATA read/write
#define TS_PL_PID_INDEX         (STB_CBUS_BASE + 0xf3) // 0x16f3
    /*----------- bit define -----------*/
    #define DES_TS_PL_STATE             8

// Bit 13 -- PID match disble
// Bit 12:0 -- PID
#define TS_PL_PID_DATA          (STB_CBUS_BASE + 0xf4) // 0x16f4
    /*----------- bit define -----------*/
    #define PID_MATCH_DISABLE_HIGH       29
    #define PID_MATCH_HIGH               16
    #define PID_MATCH_DISABLE_LOW        13
    #define PID_MATCH_LOW                0

#define COMM_DESC_KEY0          (STB_CBUS_BASE + 0xf5) // 0x16f5 // Common descrambler key (key bits[63:32])
#define COMM_DESC_KEY1          (STB_CBUS_BASE + 0xf5) // 0x16f6 // Common descrambler key (key bits[31:0])
#define COMM_DESC_KEY_RW        (STB_CBUS_BASE + 0xf7) // 0x16f7 // bits[3:0] point to the address to write the key {COMM_DESC_KEY1, COMM_DESC_KEY0}
                                                                 // Writing this register writes the key to RAM

//========================================================================
//  Multiple STB Registers				    (8'h00 - 8'h45)
//========================================================================
// STB registers are 8'h0x
// Bit 15:0 -- version number : 0x0002  (v0.01)
#define STB_VERSION             (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x00)  // 0x1600 // read only
#define STB_VERSION_2           (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x00)  // 0x1650 // read only
#define STB_VERSION_3           (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x00)  // 0x16a0 // read only

#define STB_TEST_REG            (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x01)  // 0x1601
#define STB_TEST_REG_2          (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x01)  // 0x1651
#define STB_TEST_REG_3          (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x01)  // 0x16a1

// Bit 15 -- fec_core_select 1 - select descramble output
// Bit 14:12 -  fec_select 0-TS0, 1-TS1, 2-TS2, 3,4-Reserved, 5-S2P1, 6-S2P0, 7-File 
// Bit 11 -- FEC_CLK
// Bit 10 -- SOP
// Bit 9 -- D_VALID
// Bit 8 -- D_FAIL
// Bit 7:0 -- D_DATA 7:0
#define FEC_INPUT_CONTROL       (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x02)  // 0x1602
#define FEC_INPUT_CONTROL_2     (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x02)  // 0x1652
#define FEC_INPUT_CONTROL_3     (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x02)  // 0x16a2
    /*----------- bit define -----------*/
    #define FEC_CORE_SEL               15
    #define FEC_SEL                    12
    #define FEC_INPUT_FEC_CLK          11
    #define FEC_INPUT_SOP              10
    #define FEC_INPUT_D_VALID           9
    #define FEC_INPUT_D_FAIL            8

#define FEC_INPUT_DATA          (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x03)  // 0x1603 // read only
#define FEC_INPUT_DATA_2        (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x03)  // 0x1653 // read only
#define FEC_INPUT_DATA_3        (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x03)  // 0x16a3 // read only

// bit 31 -- enable_free_clk_fec_data_valid
// bit 30 -- enable_free_clk_stb_reg
// bit 29:21 -- reserved
// bit 20 (bit 4) -- Bypass use recoder path
// bit 19 (bit 3) -- clear_PID_continuity_counter_valid
// bit 18 (bit 2) -- Disable Splicing
// bit 17 (bit 1) -- Insert PES_STRONG_SYNC in Audio PES
// bit 16 (bit 0) -- Insert PES_STRONG_SYNC in Video PES
// Bit 15 - do not trust section length
// Bit 14 - om cmd push even zero
// Bit 13 - reserved
// Bit 12 - SUB, OTHER PES interrupt at beginning of PES
// Bit 11 - discard_av_package -- for ts_recorder use only 
// Bit 10 - ts_recorder_select  0:after PID filter 1:before PID filter
// Bit 9 - ts_recorder_enable
// Bit 8 - (table_id == 0xff) means section_end
// Bit 7 - do not send uncomplete section
// Bit 6 - do not discard duplicate package
// Bit 5 - search SOP when trasport_error_indicator
// Bit 4 - stb demux enable
// Bit 3 - do not reset state machine on SOP
// Bit 2 - search SOP when error happened ( when ignore_fail_n_sop, will have this case)
// Bit 1 - do not use SOP input ( check FEC sync byte instead )
// Bit 0 - ignore fec_error bit when non sop ( check error on SOP only)
#define DEMUX_CONTROL           (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x04)  // 0x1604
#define DEMUX_CONTROL_2         (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x04)  // 0x1654
#define DEMUX_CONTROL_3         (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x04)  // 0x16a4
    /*----------- bit define -----------*/
    #define ENABLE_FREE_CLK_FEC_DATA_VALID          31
    #define ENABLE_FREE_CLK_STB_REG                 30
    #define BYPASS_USE_RECODER_PATH                 20
    #define CLEAR_PID_CONTINUITY_COUNTER_VALID      19
    #define DISABLE_SPLICING                        18
    #define INSERT_AUDIO_PES_STRONG_SYNC            17
    #define INSERT_VIDEO_PES_STRONG_SYNC            16
    #define SECTION_LENGTH_UNTRUSTY                 15
    #define OM_CMD_PUSH_EVEN_ZERO                   14
    #define OTHER_INT_AT_PES_BEGINING               12
    #define DISCARD_AV_PACKAGE                      11
    #define TS_RECORDER_SELECT                      10
    #define TS_RECORDER_ENABLE                      9
    #define SECTION_END_WITH_TABLE_ID               8
    #define SEND_COMPLETE_SECTION_ONLY              7
    #define KEEP_DUPLICATE_PACKAGE                  6
    #define SEACH_SOP_ON_TRANSPORT_ERROR            5
    #define STB_DEMUX_ENABLE                        4
    #define NO_RESET_ON_SOP                         3
    #define SEARCH_SOP_ON_ERROR                     2
    #define NOT_USE_OF_SOP_INPUT                    1
    #define IGNORE_NONSOP_FEC_ERROR                 0

// bit 15:8 demux package length - 1 ( default : 187 )
// bit 7:0 default is 0x47
#define FEC_SYNC_BYTE           (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x05)  // 0x1605
#define FEC_SYNC_BYTE_2         (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x05)  // 0x1655
#define FEC_SYNC_BYTE_3         (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x05)  // 0x16a5

/******************************************************************************************************
FM Memory Usage :
0-15 (32 PID filter target) ---- 15:13-PID type 12:0-PID target or force data
(force data : 1 will mask corespoding bit, 0 will disable this PID filter channel)
advanced setting -- bit 7:0
                    bit 7 -- PID bit 12:11 compare result force  
                    bit 6 -- PID bit 10:9 compare result force  
                    bit 5 -- PID bit 8:7 compare result force  
                    bit 4 -- PID bit 6:5 compare result force  
                    bit 3 -- PID bit 4:3 compare result force  
                    bit 2 -- PID bit 2 compare result force  
                    bit 1 -- PID bit 1 compare result force  
                    bit 0 -- PID bit 0 compare result force  
16-255(15x32 Section filter target) 
For first byte : Table_ID
---- 15-Mask High 4-bits 
     14-Mask Low 4-bits 
     13-disable_PID_check 
     12:8-PIDindex 
     7:0-section target (always EQ)
For rest of bytes :
---- 15-Mask 14-EQ/NE 13-disable_PID_check 12:8-PIDindex 7:0-section target (or force data)
advanced setting -- bit 7:0 force compare result
******************************************************************************************************/
    /*----------- bit define -----------*/
    #define PID_TYPE                                13
    #define PID_TARGET                              0

    #define SECTION_FIRSTBYTE_MASKHIGH              15
    #define SECTION_FIRSTBYTE_MASKLOW               14
    #define SECTION_FIRSTBYTE_DISABLE_PID_CHECK     13
    #define SECTION_FIRSTBYTE_PID_INDEX             8
    #define SECTION_TARGET                          0

    #define SECTION_RESTBYTE_MASK                   15
    #define SECTION_RESTBYTE_MASK_EQ                14
    #define SECTION_RESTBYTE_DISABLE_PID_CHECK      13
    #define SECTION_RESTBYTE_PID_INDEX              8

// bit 31:16 -- filter memory write data hi[31:16]
// bit 15:0 -- filter memory write data low [15:0]
#define FM_WR_DATA              (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x06)  // 0x1606
#define FM_WR_DATA_2            (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x06)  // 0x1656
#define FM_WR_DATA_3            (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x06)  // 0x16a6
    /*----------- bit define -----------*/
    #define FM_WR_DATA_HI                           16

// bit 31:24 -- advanced setting hi
// bit 23:16 -- advanced setting low
// bit 15 -- filter memory write data request
// bit 7:0 -- filter memory write addr
#define FM_WR_ADDR              (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x07)  // 0x1607
#define FM_WR_ADDR_2            (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x07)  // 0x1657
#define FM_WR_ADDR_3            (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x07)  // 0x16a7
    /*----------- bit define -----------*/
    #define FM_ADVANCED_SETTING_HI                  24
    #define FM_ADVANCED_SETTING_LO                  16
    #define FM_WR_DATA_REQUEST                      15

// bit 13:8 demux state  -- read only
// bit 7:4 -- maxnum section filter compare address
// bit 3:0 -- maxnum PID filter compare address
#define MAX_FM_COMP_ADDR        (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x08)  // 0x1608
#define MAX_FM_COMP_ADDR_2      (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x08)  // 0x1658
#define MAX_FM_COMP_ADDR_3      (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x08)  // 0x16a8
    /*----------- bit define -----------*/
    #define DEMUX_STATE                             8
    #define MAX_FM_SECTION_FILTER_COMP_ADDR         4

// bit 15 - transport_error_indicator
// bit 14 - payload_unit_start_indicator
// bit 13 - transport_priority
// bit 12:0 - PID
#define TS_HEAD_0               (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x09)  // 0x1609
#define TS_HEAD_0_2             (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x09)  // 0x1659
#define TS_HEAD_0_3             (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x09)  // 0x16a9
    /*----------- bit define -----------*/
    #define TRANSPORT_ERROR_INDICATOR               15
    #define PAYLOAD_UNIT_START_INDICATOR            14
    #define TRANSPORT_PRIORITY                      13

// bit 7:6 transport_scrambling_control
// bit 5:4 adaptation_field_control
// bit 3:0 continuity_counter
#define TS_HEAD_1               (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x0a)  // 0x160a
#define TS_HEAD_1_2             (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x0a)  // 0x165a
#define TS_HEAD_1_3             (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x0a)  // 0x16aa
    /*----------- bit define -----------*/
    #define TRANSPORT_SCRAMBLING_CONTROL            6
    #define ADAPTATION_FIELD_CONTROL                4

// bit 15:12 -- om_cmd_count (read only)
// bit  11:9 -- overflow_count // bit  11:9 -- om_cmd_wr_ptr (read only)
// bit   8:6 -- om_overwrite_count // bit   8:6 -- om_cmd_rd_ptr (read only)
// bit   5:3 -- type_stb_om_w_rd (read only)
// bit     2 -- unit_start_stb_om_w_rd (read only)
// bit     1 -- om_cmd_overflow (read only)
// bit     0 -- om_cmd_pending (read) 
// bit     0 -- om_cmd_read_finished (write)
#define OM_CMD_STATUS           (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x0b)  // 0x160b
#define OM_CMD_STATUS_2         (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x0b)  // 0x165b
#define OM_CMD_STATUS_3         (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x0b)  // 0x16ab
    /*----------- bit define -----------*/
    #define OM_CMD_COUNT                            12
    #define OM_OVERFLOW_COUNT                       9
    #define OM_OVERWRITE_COUNT                      6
    #define TYPE_STB_OM_W_RD                        3
    #define UNIT_START_STB_OM_W_RD                  2
    #define OM_CMD_OVERFLOW                         1

// bit 15:9 -- count_stb_om_w_rd  (read only)
// bit 8:0 -- start_stb_om_wa_rd (read only)
#define OM_CMD_DATA             (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x0c)  // 0x160c
#define OM_CMD_DATA_2           (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x0c)  // 0x165c
#define OM_CMD_DATA_3           (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x0c)  // 0x16ac
    /*----------- bit define -----------*/
    #define COUNT_STB_OM_W_RD                       9

// bit 11:0 -- offset for section data
#define OM_CMD_DATA2            (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x0d)  // 0x160d
#define OM_CMD_DATA2_2          (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x0d)  // 0x165d
#define OM_CMD_DATA2_3          (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x0d)  // 0x16ad

// bit 31:16 -- base address for section buffer group 0 (*0x400 to get real address)
// bit 15:0 -- base address for section buffer group 1 (*0x400 to get real address)
#define SEC_BUFF_01_START       (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x0e)  // 0x160e
#define SEC_BUFF_01_START_2     (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x0e)  // 0x165e
#define SEC_BUFF_01_START_3     (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x0e)  // 0x16ae
    /*----------- bit define -----------*/
    #define SEC_BUFF_0_BASE_ADDR                    16

// bit 31:16 -- base address for section buffer group 2 (*0x400 to get real address)
// bit 15:0 -- base address for section buffer group 3 (*0x400 to get real address)
#define SEC_BUFF_23_START       (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x0f)  // 0x160f
#define SEC_BUFF_23_START_2     (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x0f)  // 0x165f
#define SEC_BUFF_23_START_3     (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x0f)  // 0x16af
    /*----------- bit define -----------*/
    #define SEC_BUFF_2_BASE_ADDR                    16

// bit 15:12 -- section buffer size for group 3
// bit 11:8 -- section buffer size for group 2
// bit 7:4 -- section buffer size for group 1
// bit 3:0 -- section buffer size for group 0 (bit used,  for example, 10 means 1K)
#define SEC_BUFF_SIZE           (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x10)  // 0x1610
#define SEC_BUFF_SIZE_2         (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x10)  // 0x1660
#define SEC_BUFF_SIZE_3         (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x10)  // 0x16b0
    /*----------- bit define -----------*/
    #define SEC_BUFF_3_SIZE                        12
    #define SEC_BUFF_2_SIZE                         8
    #define SEC_BUFF_1_SIZE                         4

// section buffer busy status for buff 31:0 ( Read Only )
#define SEC_BUFF_BUSY           (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x11)  // 0x1611
#define SEC_BUFF_BUSY_2         (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x11)  // 0x1661
#define SEC_BUFF_BUSY_3         (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x11)  // 0x16b1

// section buffer write status for buff 31:0 -- Read
// clear buffer status ( buff READY and BUSY ) -- write
#define SEC_BUFF_READY          (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x12)  // 0x1612
#define SEC_BUFF_READY_2        (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x13)  // 0x1662
#define SEC_BUFF_READY_3        (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x13)  // 0x16b2

// bit 15 -- section_reset_busy (Read Only)
// bit 14 -- output_section_buffer_valid
// bit 12:8 -- SEC_BUFFER_NUMBER for the INDEX buffer  Read_Only
// bit 4:0 -- SEC_BUFFER_INDEX   RW
#define SEC_BUFF_NUMBER         (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x13)  // 0x1613
#define SEC_BUFF_NUMBER_2       (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x13)  // 0x1663
#define SEC_BUFF_NUMBER_3       (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x13)  // 0x16b3
    /*----------- bit define -----------*/
    #define SECTION_RESET_BUSY                      15
    #define OUTPUT_SECTION_BUFFER_VALID             14
    #define INDEXED_SEC_BUFF_NUMBER                 8

// bit 9:5 -- BYPASS PID number
// bit 4:0 -- PCR PID number
#define ASSIGN_PID_NUMBER       (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x14)  // 0x1614
#define ASSIGN_PID_NUMBER_2     (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x14)  // 0x1664
#define ASSIGN_PID_NUMBER_3     (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x14)  // 0x16b4
    /*----------- bit define -----------*/
    #define BYPASS_PID_NUMBER                       5

// bit 15:0 -- stream_id filter bit enable
// bit 7:0 -- stream_id filter target
#define VIDEO_STREAM_ID         (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x15)  // 0x1615
#define VIDEO_STREAM_ID_2       (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x15)  // 0x1665
#define VIDEO_STREAM_ID_3       (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x15)  // 0x16b5

#define AUDIO_STREAM_ID         (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x16)  // 0x1616
#define AUDIO_STREAM_ID_2       (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x16)  // 0x1666
#define AUDIO_STREAM_ID_3       (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x16)  // 0x16b6

#define SUB_STREAM_ID           (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x17)  // 0x1617
#define SUB_STREAM_ID_2         (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x17)  // 0x1667
#define SUB_STREAM_ID_3         (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x17)  // 0x16b7

#define OTHER_STREAM_ID         (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x18)  // 0x1618
#define OTHER_STREAM_ID_2       (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x18)  // 0x1668
#define OTHER_STREAM_ID_3       (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x18)  // 0x16b8

// bit 12 -- PCR_EN
// bit 11:0 -- PCR90K_DIV
#define PCR90K_CTL              (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x19)  // 0x1619
#define PCR90K_CTL_2            (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x19)  // 0x1669
#define PCR90K_CTL_3            (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x19)  // 0x16b9
    /*----------- bit define -----------*/
    #define PCR_EN                     12
    
// bit 15:0 -- PCR[31:0] R/W
#define PCR_DEMUX               (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x1a)  // 0x161a
#define PCR_DEMUX_2             (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x1a)  // 0x166a
#define PCR_DEMUX_3             (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x1a)  // 0x16ba

// bit 15:0 -- VPTS[31:0] R/W
#define VIDEO_PTS_DEMUX         (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x1b)  // 0x161b
#define VIDEO_PTS_DEMUX_2       (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x1b)  // 0x166b
#define VIDEO_PTS_DEMUX_3       (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x1b)  // 0x16bb

// bit 15:0 -- VDTS[31:0] R/W
#define VIDEO_DTS_DEMUX         (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x1c)  // 0x161c
#define VIDEO_DTS_DEMUX_2       (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x1c)  // 0x166c
#define VIDEO_DTS_DEMUX_3       (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x1c)  // 0x16bc

// bit 15:0 -- APTS[31:0] R/W
#define AUDIO_PTS_DEMUX         (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x1d)  // 0x161d
#define AUDIO_PTS_DEMUX_2       (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x1d)  // 0x166d
#define AUDIO_PTS_DEMUX_3       (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x1d)  // 0x16bd

// bit 15:0 -- SPTS[31:0] R/W
#define SUB_PTS_DEMUX           (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x1e)  // 0x161e
#define SUB_PTS_DEMUX_2         (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x1e)  // 0x166e
#define SUB_PTS_DEMUX_3         (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x1e)  // 0x16be

// read -- status, write 1 clear status
// bit 15 -- SUB_PTS[32]
// bit 14 -- AUDIO_PTS[32]
// bit 13 -- VIDEO_DTS[32]
// bit 12 -- VIDEO_PTS[32]
// bit 3 -- sub_pts_ready
// bit 2 -- audio_pts_ready
// bit 1 -- video_dts_ready
// bit 0 -- video_pts_ready
#define STB_PTS_DTS_STATUS      (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x1f)  // 0x161f
#define STB_PTS_DTS_STATUS_2    (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x1f)  // 0x166f
#define STB_PTS_DTS_STATUS_3    (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x1f)  // 0x16bf
    /*----------- bit define -----------*/
    #define SUB_PTS_BIT32              15
    #define AUDIO_PTS_BIT32            14
    #define VIDEO_DTS_BIT32            13
    #define VIDEO_PTS_BIT32            12
    #define SUB_PTS_READY              3
    #define AUDIO_PTS_READY            2
    #define VIDEO_DTS_READY            1
    #define VIDEO_PTS_READY            0

// bit 3:0 --
//  0 -- adaptation_field_length[7:0], adaption_field_byte_1[7:0] 
//  1 -- stream_id[7:0], pes_header_bytes_left[7:0]
//  2 -- pes_package_bytes_left[15:0]
//  3 -- pes_ctr_byte[7:0], pes_flag_byte[7:0]
#define STB_DEBUG_INDEX         (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x20)  // 0x1620
#define STB_DEBUG_INDEX_2       (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x20)  // 0x1670
#define STB_DEBUG_INDEX_3       (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x20)  // 0x16c0

// read only
#define STB_DEBUG_DATA_OUT      (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x21)  // 0x1621
#define STB_DEBUG_DATA_OUT_2    (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x21)  // 0x1671
#define STB_DEBUG_DATA_OUT_3    (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x21)  // 0x16c1

// default : 0x807f
// bit 15:9  -- MAX OM DMA COUNT  (default: 0x40) 
// bit 8:0   -- LAST ADDR OF OM ADDR (default: 127)
#define STB_OM_CTL              (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x22)  // 0x1622
#define STB_OM_CTL_2            (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x22)  // 0x1672
#define STB_OM_CTL_3            (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x22)  // 0x16c2
    /*----------- bit define -----------*/
    #define MAX_OM_DMA_COUNT           9
    #define LAST_OM_ADDR               0

// 15:0  WRITE 1 CLEAR to clear interrupt source
// 9 -- splicing_point
// 8 -- other_PES_int
// 7 -- sub_PES_int 
// 6 -- discontinuity
// 5 -- duplicated_pack_found
// 4 -- New PDTS ready
// 3 -- om_cmd_buffer ready for access
// 2 -- section buffer ready 
// 1 -- transport_error_indicator
// 0 -- TS ERROR PIN 
#define STB_INT_STATUS          (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x23)  // 0x1623
#define STB_INT_STATUS_2        (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x23)  // 0x1673
#define STB_INT_STATUS_3        (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x23)  // 0x16c3
    /*----------- bit define -----------*/
    #define PCR_READY                  11
    #define AUDIO_SPLICING_POINT       10
    #define VIDEO_SPLICING_POINT       9
    #define OTHER_PES_READY            8
    #define SUB_PES_READY              7
    #define DIS_CONTINUITY_PACKET      6
    #define DUPLICATED_PACKET          5
    #define NEW_PDTS_READY             4
    #define OM_CMD_READ_PENDING        3
    #define SECTION_BUFFER_READY       2
    #define TS_ERROR_PACKAGE           1
    #define TS_ERROR_PIN               0

// When Bit 31 - 1 write will indicate all type use sepertate endian (Write Only)
// When Bit 31 - 0 write will indicate all type else use Bit 8:6
// Bit 23:21 - demux om write endian control for OTHER_PES_PACKET
// Bit 20:18 - demux om write endian control for SCR_ONLY_PACKET
// Bit 17:15 - demux om write endian control for SUB_PACKET
// Bit 14:12 - demux om write endian control for AUDIO_PACKET
// Bit 11:9  - demux om write endian control for VIDEO_PACKET
// Bit 8:6 - demux om write endian control for else
// Bit 5:3 - demux om write endian control for bypass
// Bit 2:0 - demux om write endian control for section
#define DEMUX_ENDIAN            (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x24)  // 0x1624
#define DEMUX_ENDIAN_2          (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x24)  // 0x1674
#define DEMUX_ENDIAN_3          (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x24)  // 0x16c4
    /*----------- bit define -----------*/
    #define SEPERATE_ENDIAN            31
    #define OTHER_PES_ENDIAN           21
    #define SCR_ENDIAN                 18
    #define SUB_ENDIAN                 15
    #define AUDIO_ENDIAN               12
    #define VIDEO_ENDIAN               9
    #define OTHER_ENDIAN               6
    #define BYPASS_ENDIAN              3
    #define SECTION_ENDIAN             0

// Bit 7 -- use hi_bsf interface
#define TS_HIU_CTL              (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x25)  // 0x1625
#define TS_HIU_CTL_2            (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x25)  // 0x1675
#define TS_HIU_CTL_3            (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x25)  // 0x16c5
    /*----------- bit define -----------*/
    #define LAST_BURST_THRESHOLD       8
    #define USE_HI_BSF_INTERFACE       7

// bit 15:0 -- base address for section buffer start (*0x10000 to get real base)
#define SEC_BUFF_BASE           (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x26)  // 0x1626
#define SEC_BUFF_BASE_2         (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x26)  // 0x1676
#define SEC_BUFF_BASE_3         (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x26)  // 0x16c6

// bit 11 -- mask bit for OTHER_PES_AHB_DMA_EN
// bit 10 -- mask bit for SUB_AHB_DMA_EN
// bit 9 -- mask bit for BYPASS_AHB_DMA_EN
// bit 8 -- mask bit for SECTION_AHB_DMA_EN
// bit 7 -- mask bit for recoder stream
// bit 6:0 -- mask bit for each type
#define DEMUX_MEM_REQ_EN        (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x27)  // 0x1627
#define DEMUX_MEM_REQ_EN_2      (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x27)  // 0x1677
#define DEMUX_MEM_REQ_EN_3      (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x27)  // 0x16c7
    /*----------- bit define -----------*/
    #define OTHER_PES_AHB_DMA_EN       11
    #define SUB_AHB_DMA_EN             10
    #define BYPASS_AHB_DMA_EN          9
    #define SECTION_AHB_DMA_EN         8
    #define RECORDER_STREAM            7
    #define OTHER_PES_PACKET           6
    #define SCR_ONLY_PACKET            5
    #define BYPASS_PACKET              4
    #define SECTION_PACKET             3
    #define SUB_PACKET                 2
    #define AUDIO_PACKET               1
    #define VIDEO_PACKET               0

// bit 31:0 -- vb_wr_ptr for video PDTS 
#define VIDEO_PDTS_WR_PTR       (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x28)  // 0x1628
#define VIDEO_PDTS_WR_PTR_2     (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x28)  // 0x1678
#define VIDEO_PDTS_WR_PTR_3     (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x28)  // 0x16c8

// bit 31:0 -- ab_wr_ptr for audio PDTS
#define AUDIO_PDTS_WR_PTR       (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x29)  // 0x1629
#define AUDIO_PDTS_WR_PTR_2     (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x29)  // 0x1679
#define AUDIO_PDTS_WR_PTR_3     (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x29)  // 0x16c9

// bit 20:0 -- SB_WRITE_PTR (sb_wr_ptr << 3 == byte write position)
#define SUB_WR_PTR              (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x2a)  // 0x162a
#define SUB_WR_PTR_2            (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x2a)  // 0x167a
#define SUB_WR_PTR_3            (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x2a)  // 0x16ca

// bit 19:0 -- SB_START (sb_start << 12 == byte address);
#define SB_START                (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x2b)  // 0x162b
#define SB_START_2              (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x2b)  // 0x167b
#define SB_START_3              (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x2b)  // 0x16cb

// bit 20:0 -- SB_SIZE (sb_size << 3 == byte size, 16M maximun)
#define SB_LAST_ADDR            (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x2c)  // 0x162c
#define SB_LAST_ADDR_2          (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x2c)  // 0x167c
#define SB_LAST_ADDR_3          (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x2c)  // 0x16cc

// bit 31:0 -- sb_wr_ptr for sub PES
#define SB_PES_WRITE_PTR        (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x2d)  // 0x162d
#define SB_PES_WRITE_PTR_2      (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x2d)  // 0x167d
#define SB_PES_WRITE_PTR_3      (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x2d)  // 0x16cd

// bit 31:16 -- ob_wr_ptr for other PES
// bit 20:0 -- OB_WRITE_PTR (ob_wr_ptr << 3 == byte write position)
#define OTHER_WR_PTR            (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x2e)  // 0x162e
#define OTHER_WR_PTR_2          (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x2e)  // 0x167e
#define OTHER_WR_PTR_3          (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x2e)  // 0x16ce

// bit 19:0 -- OB_START (ob_start << 12 == byte address);
#define OB_START                (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x2f)  // 0x162f
#define OB_START_2              (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x2f)  // 0x167f
#define OB_START_3              (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x2f)  // 0x16cf

// bit 20:0 -- OB_SIZE (ob_size << 3 == byte size, 16M maximun)
#define OB_LAST_ADDR            (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x30)  // 0x1630
#define OB_LAST_ADDR_2          (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x30)  // 0x1680
#define OB_LAST_ADDR_3          (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x30)  // 0x16d0

// bit 31:0 -- ob_wr_ptr for sub PES
#define OB_PES_WRITE_PTR        (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x31)  // 0x1631
#define OB_PES_WRITE_PTR_2      (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x31)  // 0x1681
#define OB_PES_WRITE_PTR_3      (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x31)  // 0x16d1

// 15:0  DEMUX interrupt MASK
// 11 -- PCR_READY
// 10 -- audio_splicing_point
// 9 -- video_splicing_point
// 8 -- other_PES_int
// 7 -- sub_PES_int 
// 6 -- discontinuity
// 5 -- duplicated_pack_found
// 4 -- New PDTS ready
// 3 -- om_cmd_buffer ready for access
// 2 -- section buffer ready 
// 1 -- transport_error_indicator
// 0 -- TS ERROR PIN
#define STB_INT_MASK            (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x32)  // 0x1632
#define STB_INT_MASK_2          (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x32)  // 0x1682
#define STB_INT_MASK_3          (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x32)  // 0x16d2

// 31:16 VIDEO PID filter data
//15 -- splicing VIDEO PID change enable 
//14:10 -- VIDEO PID FILTER ADDRESS
// 9 -- PES splicing active (Read Only) 
// 8 -- splicing active (Read Only)
// 7:0  splicing countdown (Read Only)
#define VIDEO_SPLICING_CTL      (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x33)  // 0x1633
#define VIDEO_SPLICING_CTL_2    (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x33)  // 0x1683
#define VIDEO_SPLICING_CTL_3    (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x33)  // 0x16d3
    /*----------- bit define -----------*/
    #define VIDEO_PID_FILTER_DATA      16
    #define VIDEO_SPLICING_PID_CHANGE_ENABLE       15
    #define VIDEO_PID_FILTER_ADDRESS   10
    #define VIDEO_PES_SPLICING_ACTIVE  9
    #define VIDEO_SPLICING_ACTIVE      8


// 31:16 AUDIO PID filter data
//15 -- splicing AUDIO PID change enable 
//14:10 -- AUDIO PID FILTER ADDRESS
// 9 -- PES splicing active (Read Only) 
// 8 -- splicing active (Read Only)
// 7:0  splicing countdown (Read Only)
#define AUDIO_SPLICING_CTL      (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x34)  // 0x1634
#define AUDIO_SPLICING_CTL_2    (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x34)  // 0x1684
#define AUDIO_SPLICING_CTL_3    (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x34)  // 0x16d4
    /*----------- bit define -----------*/
    #define AUDIO_PID_FILTER_DATA      16
    #define AUDIO_SPLICING_PID_CHANGE_ENABLE       15
    #define AUDIO_PID_FILTER_ADDRESS   10
    #define AUDIO_PES_SPLICING_ACTIVE  9
    #define AUDIO_SPLICING_ACTIVE      8

// 23:16 M2TS_SKIP_BYTES
// 15:8 LAST TS PACKAGE BYTE COUNT (Read Only)
// 7:0  PACKAGE BYTE COUNT (Read Only)
#define TS_PACKAGE_BYTE_COUNT   (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x35)  // 0x1635
#define TS_PACKAGE_BYTE_COUNT_2 (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x35)  // 0x1685
#define TS_PACKAGE_BYTE_COUNT_3 (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x35)  // 0x16d5
    /*----------- bit define -----------*/
    #define M2TS_SKIP_BYTES            16
    #define LAST_TS_PACKAGE_BYTE_COUNT 8

// 15:0 2 bytes strong sync add to PES
#define PES_STRONG_SYNC         (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x36)  // 0x1636
#define PES_STRONG_SYNC_2       (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x36)  // 0x1686
#define PES_STRONG_SYNC_3       (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x36)  // 0x16d6

// bit 15 -- stb_om_ren
// bit 14:11 -- reserved
// bit  10:0 -- OM_DATA_RD_ADDR
#define OM_DATA_RD_ADDR         (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x37)  // 0x1637
#define OM_DATA_RD_ADDR_2       (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x37)  // 0x1687
#define OM_DATA_RD_ADDR_3       (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x37)  // 0x16d7
    /*----------- bit define -----------*/
    #define STB_OM_REN                 15

// bit 15:0 -- OM_DATA_RD
#define OM_DATA_RD              (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x38)  // 0x1638
#define OM_DATA_RD_2            (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x38)  // 0x1688
#define OM_DATA_RD_3            (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x38)  // 0x16d8

// AUTO STOP SETTING for 32 channels
// 4-bits per channel
// when write
// bit 3 -- set section active
// bit 2:0 -- auto stop after count (0 means never stop)
// when read
// bit 3 -- current active status (1 - active, 0 - stopped )
// bit 2:0 -- count down to auto stop
// section 31:24
#define SECTION_AUTO_STOP_3     (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x39)  // 0x1639
#define SECTION_AUTO_STOP_3_2   (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x39)  // 0x1689
#define SECTION_AUTO_STOP_3_3   (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x39)  // 0x16d9
// section 23:16
#define SECTION_AUTO_STOP_2     (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x3a)  // 0x163a
#define SECTION_AUTO_STOP_2_2   (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x3a)  // 0x168a
#define SECTION_AUTO_STOP_2_3   (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x3a)  // 0x16da
// section 15:8
#define SECTION_AUTO_STOP_1     (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x3b)  // 0x163b
#define SECTION_AUTO_STOP_1_2   (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x3b)  // 0x168b
#define SECTION_AUTO_STOP_1_3   (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x3b)  // 0x16db
// section 7:0
#define SECTION_AUTO_STOP_0     (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x3c)  // 0x163c
#define SECTION_AUTO_STOP_0_2   (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x3c)  // 0x168c
#define SECTION_AUTO_STOP_0_3   (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x3c)  // 0x16dc

// bit 31:0 reset channel status - each bit reset each channel 
// read -- 32 channel status 
#define DEMUX_CHANNEL_RESET     (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x3d)  // 0x163d
#define DEMUX_CHANNEL_RESET_2   (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x3d)  // 0x168d
#define DEMUX_CHANNEL_RESET_3   (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x3d)  // 0x16dd

#define DEMUX_SCRAMBLING_STATE     (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x3e)  // 0x163e
#define DEMUX_SCRAMBLING_STATE_2   (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x3e)  // 0x168e
#define DEMUX_SCRAMBLING_STATE_3   (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x3e)  // 0x16de

#define DEMUX_CHANNEL_ACTIVITY     (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x3f)  // 0x163f
#define DEMUX_CHANNEL_ACTIVITY_2   (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x3f)  // 0x168f
#define DEMUX_CHANNEL_ACTIVITY_3   (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x3f)  // 0x16df

// bit 4 -- video_stamp_use_dts
// bit 3 -- audio_stamp_sync_1_en 
// bit 2 -- audio_stamp_insert_en 
// bit 1 -- video_stamp_sync_1_en 
// bit 0 -- video_stamp_insert_en
#define DEMUX_STAMP_CTL         (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x40)  // 0x1640
#define DEMUX_STAMP_CTL_2       (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x40)  // 0x1690
#define DEMUX_STAMP_CTL_3       (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x40)  // 0x16e0

#define DEMUX_VIDEO_STAMP_SYNC_0   (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x41)  // 0x1641
#define DEMUX_VIDEO_STAMP_SYNC_0_2 (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x41)  // 0x1691
#define DEMUX_VIDEO_STAMP_SYNC_0_3 (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x41)  // 0x16e1

#define DEMUX_VIDEO_STAMP_SYNC_1   (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x42)  // 0x1642
#define DEMUX_VIDEO_STAMP_SYNC_1_2 (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x42)  // 0x1692
#define DEMUX_VIDEO_STAMP_SYNC_1_3 (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x42)  // 0x16e2

#define DEMUX_AUDIO_STAMP_SYNC_0   (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x43)  // 0x1643
#define DEMUX_AUDIO_STAMP_SYNC_0_2 (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x43)  // 0x1693
#define DEMUX_AUDIO_STAMP_SYNC_0_3 (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x43)  // 0x16e3

#define DEMUX_AUDIO_STAMP_SYNC_1   (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x44)  // 0x1644
#define DEMUX_AUDIO_STAMP_SYNC_1_2 (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x44)  // 0x1694
#define DEMUX_AUDIO_STAMP_SYNC_1_3 (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x44)  // 0x16e4

// Write : Bit[4:0] secter filter number for reset
// Read  : select according to output_section_buffer_valid : 
//         per bit per section buffer valid status
//         or section_buffer_ignore
#define DEMUX_SECTION_RESET     (STB_CBUS_BASE + DEMUX_1_OFFSET + 0x45)  // 0x1645
#define DEMUX_SECTION_RESET_2   (STB_CBUS_BASE + DEMUX_2_OFFSET + 0x45)  // 0x1695
#define DEMUX_SECTION_RESET_3   (STB_CBUS_BASE + DEMUX_3_OFFSET + 0x45)  // 0x16e5

//========================================================================
//  STB Registers End
//========================================================================


//========================================================================
//  GE2D Registers                  (0x8a0 - 0x8d6)
//========================================================================
// -----------------------------------------------  
// CBUS_BASE:  GE2D_CBUS_BASE = 0x18                
// -----------------------------------------------  
#define GE2D_CBUS_BASE 0x1800

//Bit 31, destination bytemask only if destination bitmask is enable
//Bit 30, destination bitmask enable
//Bit 29, source2 key  enable
//Bit 28, source2 key  mode, 0: mask data when match, 1: mask data when unmatch
//Bit 27, source1 key  enable
//Bit 26, source1 key  mode, 0: mask data when match, 1: mask data when unmatch
//Bit 25:24, dst 8bit mode component selection, 
//            00: select Y(R), 01: Cb(G), 10: Cr(B), 11: Alpha
//Bit 23 dst clip mode, 0: write inside clip window, 1: write outside clip window
//Bit 21:20  dst format,  00: 8bit, 01:reserved, 10:24bit(RGB/YCbCr) 11: 32bit(RGBA/YCBCRA)
//Bit 18:17  src2 format, 00: 8bit, 01:reserved, 10:24bit(RGB/YCbCr) 11: 32bit(RGBA/YCBCRA)
//Bit 16:15, src2 8bit mode component selection, 
//            00: select Y(R), 01: Cb(G), 10: Cr(B), 11: Alpha
//Bit 14     src2 fill mode, 0: repeat data, 1: fill default color
//Bit 13:12  src2 picture struct, 00: frame, 10: even, 11: odd
//Bit 11     src1 x direction yc ration, 0: 1:1, 1: 2:1
//Bit 10     src1 y direction yc ration, 0: 1:1, 1: 2:1
//Bit 8:7    src1 format, 00: 8bit, 01:4:2:2 store togather, 10:24bit(RGB/YCbCr) 11: 32bit(RGBA/YCBCRA)
//Bit 6:5,   src1  8bit mode component selection, 
//            00: select Y(R), 01: Cb(G), 10: Cr(B), 11: Alpha
//Bit 4      src1 fill mode, 0: repeat data, 1: fill default color
//Bit 3      src1 lookup table enable
//Bit 2:1    src1 picture struct, 00: frame, 10: even, 11: odd
//Bit 0      src1 separate buffer enable
#define GE2D_GEN_CTRL0                  (GE2D_CBUS_BASE+0xa0)  //32'h0
//Bit 31, soft rst
//bit 25:24, interrupt control, if bit[0] true, generate interrupt when one command done,
//                              if bit[1] true, generate interrupt when ge2d change from busy to not busy
//Bit 23:22 src2 burst size control
//Bit 21:16 src1 burst size control, 5:4, yfifo, 3:2, cbfifo, 1:0, crfifo
//          each 2bit, 00: 24 64bitword, 01: 32 64bitword, 10: 48 64bitwords, 11: 64 64bitwords  
//Bit 15:14, dst picture struct, 00: frame, 10:top, 11: bottom
//Bit 13:12, bit 13 if true, force read src1, bit 12 if true, force read src2
//Bit 10, src1 request urgent enable
//Bit 9,  src2 request urgent enable
//Bit 8,  dst request urgent enable
//Bit 7:0 src1 global alpha
#define GE2D_GEN_CTRL1                  (GE2D_CBUS_BASE+0xa1)   //32'h00ff0000
#define GE2D_GEN_CTRL2                  (GE2D_CBUS_BASE+0xa2)
//Bit 9     if true, all src2 data use default color
//Bit 8     if true, all src1 data use default color
//Bit 7     if true, dst x/y swap 
//Bit 6     if true, dst x direction reversely read
//Bit 5     if true, dst y direction reversely read
//Bit 4     if true, src2 x direction reversely read
//Bit 3     if true, src2 y direction reversely read
//Bit 2     if true, src1 x direction reversely read
//Bit 1     if true, src1 y direction reversely read
//Bit 0     cmd write
#define GE2D_CMD_CTRL                   (GE2D_CBUS_BASE+0xa3) //'h0
//Read only
//Bit 16:7  ge2d_dp status, for debug only
//Bit 6     read src2 cmd ready
//Bit 5     read src2 cmd ready
//Bit 4     pre dpcmd ready
//Bit 3     ge2d dpcmd ready
//Bit 2     ge2d buffer command valid
//Bit 1     ge2d current command valid
//Bit 0     ge2d busy
#define GE2D_STATUS0                    (GE2D_CBUS_BASE+0xa4) 
//
//Read only
// Bit 29:16 ge2d_dst_status, for debug only
// Bit    15 ge2d_rd_src2 core.fifo_empty
// Bit    14 ge2d_rd_src2 core.fifo_overflow
// Bit 13:12 ge2d_rd_src2 core.req_st
// Bit    11 ge2d_rd_src2 cmd_if.cmd_err, true if cmd_format=1
// Bit    10 ge2d_rd_src2 cmd_if.cmd_st, 0=IDLE state, 1=BUSY state
// Bit     9 ge2d_rd_src1 luma_core(chroma_core).fifo_empty
// Bit     8 ge2d_rd_src1 luma_core(chroma_core).fifo_overflow
// Bit  7: 6 ge2d_rd_src1 chroma_core.req_st_cr
// Bit  5: 4 ge2d_rd_src1 chroma_core.req_st_cb
// Bit  3: 2 ge2d_rd_src1 luma_core.req_st_y
// Bit     1 ge2d_rd_src1 cmd_if.stat_read_window_err, 1=reading/clipping window setting exceed limit
// Bit     0 ge2d_rd_src1 cmd_if.cmd_st, 0=IDLE state, 1=BUSY state
#define GE2D_STATUS1                    (GE2D_CBUS_BASE+0xa5) 
//SRC1 default clolor
//{Y,Cb,Cr,A}/{R,G,B,A}
#define GE2D_SRC1_DEF_COLOR             (GE2D_CBUS_BASE+0xa6)   //32'h00808000 
//Bit 31, SRC1 clip x start extra, if true, one more data is read for chroma
//Bit 28:16, SRC1 clip x start
//Bit 15, SRC1 clip x end extra, if true, one more data is read for chroma
//Bit 12:0, SRC1 clip x end
#define GE2D_SRC1_CLIPX_START_END       (GE2D_CBUS_BASE+0xa7)  //32'h0000_1fff 
//Bit 31, SRC1 clip y start extra, if true, one more data is read for chroma
//Bit 28:16, SRC1 clip y start
//Bit 15, SRC1 clip y end extra, if true, one more data is read for chroma
//Bit 12:0, SRC1 clip y end
#define GE2D_SRC1_CLIPY_START_END       (GE2D_CBUS_BASE+0xa8)  //32'h0000_1fff 
//Bit 30:24, SRC1 canvas address0
//Bit 22:16, SRC1 canvas address1
//Bit 14:8, SRC1 canvas address2
#define GE2D_SRC1_CANVAS                (GE2D_CBUS_BASE+0xa9) //'h0 
//Bit 31, SRC1 x start extra bit1, if true, one more chroma data is read for x even start chroma data when y/c ratio = 2
//             or x even/odd start chroma extra data when y/c ratio = 1
//Bit 30, SRC1 x start extra bit0, if true, one more chroma data is read for x odd start chroma data when y/c ratio = 2
//Bit 29:16, SRC1 x start, signed data
//Bit 15, SRC1 x end extra bit1, if true, one more chroma data is read for x odd end chroma data when y/c ratio = 2
//             or x even/odd end chroma extra data when y/c ratio = 1
//Bit 14, SRC1 x end extra bit0, if true, one more chroma data is read for x even end chroma data when y/c ratio = 2
//Bit 13:0, SRC1 x end, signed data
#define GE2D_SRC1_X_START_END           (GE2D_CBUS_BASE+0xaa) //'h0 
//Bit 31, SRC1 y start extra, if true, one more chroma data is read for y even start chroma data when y/c ratio = 2
//             or y even/odd start chroma extra data when y/c ratio = 1
//Bit 30, SRC1 y start extra, if true, one more chroma data is read for x odd start chroma data when y/c ratio = 2
//Bit 29:16, SRC1 y start
//Bit 15, SRC1 y end extra bit1, if true, one more chroma data is read for y odd end chroma data when y/c ratio = 2
//             or y even/odd end chroma extra data when y/c ratio = 1
//Bit 14, SRC1 y end extra bit0, if true, one more chroma data is read for y even end chroma data when y/c ratio = 2
//Bit 13:0, SRC1 y end
#define GE2D_SRC1_Y_START_END           (GE2D_CBUS_BASE+0xab)  //'h0 
// Bit 31: 9 Reserved
// Bit     8 RW, 0 = Write LUT, 1 = Read LUT
// Bit  7: 0 RW, lut_addr
#define GE2D_SRC1_LUT_ADDR              (GE2D_CBUS_BASE+0xac)
// Bit 31:24 RW, Y or R
// Bit 23:16 RW, Cb or G
// Bit 15: 8 RW, Cr or B
// Bit  7: 0 RW, Alpha
#define GE2D_SRC1_LUT_DAT               (GE2D_CBUS_BASE+0xad)
//Bit 19, if true, horizontal formatter using repeat to get the pixel, otherwise using interpolation
//Bit 18, horizontal formatter en
//Bit 17, if true, vertical formatter using repeat to get the pixel, otherwise using interpolation
//Bit 16, vertical formatter en
//Bit 15:8 X direction chroma phase,  
//          [7:4] for x direction even start/end chroma phase when y/c ratio = 2
//                or start/end even/odd chroma phase  when y/c ratio = 1 
//          [3:0] for x direction odd start/end chroma phase only when y/c ration = 2
//Bit 7:0  Y direction chroma phase, 
//          [7:4] for y direction even start/end chroma phase when y/c ratio = 2
//          or start/end even/odd chroma phase  when y/c ratio = 1 
//          [3:0] for y direction odd start/end chroma phase only when y/c ration = 2 
#define GE2D_SRC1_FMT_CTRL              (GE2D_CBUS_BASE+0xae)  //'h0
//SRC2 default clolor
//{Y,Cb,Cr,A}/{R,G,B,A}
#define GE2D_SRC2_DEF_COLOR             (GE2D_CBUS_BASE+0xaf)  //32'h00808000 
//Bit 28:16, SRC2 clip x start
//Bit 12:0, SRC2 clip x end
#define GE2D_SRC2_CLIPX_START_END       (GE2D_CBUS_BASE+0xb0)  //32'h0000_1fff 
//Bit 28:16, SRC2 clip y start
//Bit 12:0, SRC2 clip y end
#define GE2D_SRC2_CLIPY_START_END       (GE2D_CBUS_BASE+0xb1)  //32'h0000_1fff 
//Bit 28:16, SRC2 x start
//Bit 12:0, SRC2 x end
#define GE2D_SRC2_X_START_END           (GE2D_CBUS_BASE+0xb2)  //'h0 
//Bit 28:16, SRC2 y start
//Bit 12:0, SRC2 y end
#define GE2D_SRC2_Y_START_END           (GE2D_CBUS_BASE+0xb3)  //'h0 
//Bit 28:16, DST clip x start
//Bit 12:0, DST clip x end
#define GE2D_DST_CLIPX_START_END        (GE2D_CBUS_BASE+0xb4)  //32'h0000_1fff 
//
//Bit 28:16, DST clip y start
//Bit 12:0, DST clip y end
#define GE2D_DST_CLIPY_START_END        (GE2D_CBUS_BASE+0xb5)  //32'h0000_1fff
//Bit 28:16, DST x start
//Bit 12:0, DST x end
#define GE2D_DST_X_START_END            (GE2D_CBUS_BASE+0xb6) //'h0 
//
//Bit 28:16, DST y start
//Bit 12:0, DST y end
#define GE2D_DST_Y_START_END            (GE2D_CBUS_BASE+0xb7) //'h0 

//Bit 14:8 SRC2 canvas address
//Bit 6:0 DST canvas address
#define GE2D_SRC2_DST_CANVAS            (GE2D_CBUS_BASE+0xb8) //'h0
//vertical scaler phase step
//Bit 28:0,  5.24 format
#define GE2D_VSC_START_PHASE_STEP       (GE2D_CBUS_BASE+0xb9) //29'h01000000 

//phase slope 
//Bit 24:0, bit 24 signed bit
#define GE2D_VSC_PHASE_SLOPE            (GE2D_CBUS_BASE+0xba) //'h0 

//Bit 30:29, vertical repeat line0 number 
//Bit 23:0, vertical scaler initial phase
#define GE2D_VSC_INI_CTRL               (GE2D_CBUS_BASE+0xbb) //'h0

//horizontal scaler phase step
//Bit 28:0,  5.24 format
#define GE2D_HSC_START_PHASE_STEP       (GE2D_CBUS_BASE+0xbc) //29'h01000000 

//phase slope 
//Bit 24:0, bit 24 signed bit
#define GE2D_HSC_PHASE_SLOPE            (GE2D_CBUS_BASE+0xbd) //'h0

//Bit 30:29, horizontal repeat line0 number 
//Bit 23:0, horizontal scaler initial phase
#define GE2D_HSC_INI_CTRL               (GE2D_CBUS_BASE+0xbe) //'h0

//Bit 31:24, advance number in this round, if horizontal scaler is working on dividing mode
//Bit 23:0, horizontal scaler advance phase in this round, if horizontal scaler is working on dividing mode 
#define GE2D_HSC_ADV_CTRL               (GE2D_CBUS_BASE+0xbf) //'h0

//Bit 30, vertical nearest mode enable, must set vt_bank_length = 4
//Bit 29, horizontal nearest mode enable, must set hz_bank_length = 4 
//Bit 28, horizontal scaler dividing mode enable
//Bit 27:15, horizontal dividing length, if bit 25 is enable
//Bit 14, pre horizontal scaler enable 
//Bit 13, pre vertical scale enable
//Bit 12, vertical scale enable
//Bit 11, horizontal scaler enable
//Bit 9, if true, treat horizontal repeat line number(GE2D_HSC_INI_CTRL bit 30:29) as repeating line, 
//        otherwise using treat horizontal repeat line number as minus line number. 
//Bit 8, if true, treat vertical repeat line number(GE2D_VSC_INI_CTRL bit 30:29) as repeating line, 
//        otherwise using treat vertical repeat line number as minus line number. 
//Bit 7, if true, always use phase0 in vertical scaler
//Bit 6:4, vertical scaler bank length
//Bit 3, if true, always use phase0 in horizontal scaler
//Bit 2:0, horizontal scaler bank length
#define GE2D_SC_MISC_CTRL               (GE2D_CBUS_BASE+0xc0) //'h00000044

//Read only
//vertical scaler next round integer pixel pointer, signed data
//Bit 13:0
#define GE2D_VSC_NRND_POINT             (GE2D_CBUS_BASE+0xc1)

//Read only
//vertical scaler next round phase
//bit 23:0
#define GE2D_VSC_NRND_PHASE             (GE2D_CBUS_BASE+0xc2) 

//Read only
//horizontal scaler next round integer pixel pointer, signed data
//Bit 13:0
#define GE2D_HSC_NRND_POINT             (GE2D_CBUS_BASE+0xc3) 

//Read only
//horizontal scaler next round phase
//bit 23:0
#define GE2D_HSC_NRND_PHASE             (GE2D_CBUS_BASE+0xc4) 

#define GE2D_MATRIX_PRE_OFFSET			(GE2D_CBUS_BASE+0xc5) 
//Bit 28:16 coef00
//Bit 12:0  coef01
#define GE2D_MATRIX_COEF00_01           (GE2D_CBUS_BASE+0xc6)     //'h0 

//Bit 28:16 coef02
//Bit 12:0  coef10
#define GE2D_MATRIX_COEF02_10           (GE2D_CBUS_BASE+0xc7)     //'h0 

//Bit 28:16 coef11
//Bit 12:0  coef12
#define GE2D_MATRIX_COEF11_12           (GE2D_CBUS_BASE+0xc8)     //'h0

//Bit 28:16 coef20
//Bit 12:0  coef21
#define GE2D_MATRIX_COEF20_21           (GE2D_CBUS_BASE+0xc9)     //'h0 

//Bit 28:16 coef22
//Bit 7    input y/cb/cr saturation enable
//Bit 0    conversion matrix enable
#define GE2D_MATRIX_COEF22_CTRL         (GE2D_CBUS_BASE+0xca)     //'h0 

//Bit 28:20, offset0
//Bit 18:10, offset1 
//Bit 8:0,   offset2
#define GE2D_MATRIX_OFFSET              (GE2D_CBUS_BASE+0xcb)     //'h0

//Bit 26:25, SRC1 color multiplier alpha selection
//           if 00, Cs = Csr
//           if 01, Cs = Csr * Asr * Ag (if source is not premultiplied)
//           if 10, Cs = Csr * Ag (if source is premultipied)
//Bit 24    SRC2 color multiplier alpha selection 
//          if 0, no multiplier, Cd = Cdr,  otherwise, Cd = Cdr * Ad.   
//Bit 22:12 ALU color operation
//          bit10:8 Blending Mode Parameter
//            3'b000: ADD               Cs*Fs + Cd*Fd
//            3'b001: SUBTRACT          Cs*Fs - Cd*Fd
//            3'b010: REVERSE SUBTRACT  Cd*Fd - Cs*Fs
//            3'b011: MIN               min(Cs*Fs, Cd*Fd)
//            3'b100: MAX               max(Cs*Fs, Cd*Fd)
//            3'b101: LOGIC OP          Cs op Cd
//          bit7:4 Source Color Blending Factor CFs
//            4'b0000: ZERO                        0
//            4'b0001: ONE                         1
//            4'b0010: SRC_COLOR                   Cs(RGBs)
//            4'b0011: ONE_MINUS_SRC_COLOR         1 - Cs(RGBs)
//            4'b0100: DST_COLOR                   Cd(RGBd)
//            4'b0101: ONE_MINUS_DST_COLOR         1 - Cd(RGBd)
//            4'b0110: SRC_ALPHA                   As
//            4'b0111: ONE_MINUS_SRC_ALPHA         1 - As
//            4'b1000: DST_ALPHA                   Ad
//            4'b1001: ONE_MINUS_DST_ALPHA         1 - Ad
//            4'b1010: CONST_COLOR                 Cc(RGBc)
//            4'b1011: ONE_MINUS_CONST_COLOR       1 - Cc(RGBc)
//            4'b1100: CONST_ALPHA                 Ac
//            4'b1101: ONE_MINUS_CONST_ALPHA       1 - Ac
//            4'b1110: SRC_ALPHA_SATURATE          min(As,1-Ad)
//          bit3:0 dest Color Blending Factor CFd, when bit10:8 != LOGIC OP
//            4'b0000: ZERO                        0
//            4'b0001: ONE                         1
//            4'b0010: SRC_COLOR                   Cs(RGBs)
//            4'b0011: ONE_MINUS_SRC_COLOR         1 - Cs(RGBs)
//            4'b0100: DST_COLOR                   Cd(RGBd)
//            4'b0101: ONE_MINUS_DST_COLOR         1 - Cd(RGBd)
//            4'b0110: SRC_ALPHA                   As
//            4'b0111: ONE_MINUS_SRC_ALPHA         1 - As
//            4'b1000: DST_ALPHA                   Ad
//            4'b1001: ONE_MINUS_DST_ALPHA         1 - Ad
//            4'b1010: CONST_COLOR                 Cc(RGBc)
//            4'b1011: ONE_MINUS_CONST_COLOR       1 - Cc(RGBc)
//            4'b1100: CONST_ALPHA                 Ac
//            4'b1101: ONE_MINUS_CONST_ALPHA       1 - Ac
//            4'b1110: SRC_ALPHA_SATURATE          min(As,1-Ad)
//          bit3:0 logic operations, when bit10:8 == LOGIC OP
//            4'b0000: CLEAR                       0
//            4'b0001: COPY                        s
//            4'b0010: NOOP                        d
//            4'b0011: SET                         1
//            4'b0100: COPY_INVERT                 ~s
//            4'b0101: INVERT                      ~d
//            4'b0110: AND_REVERSE                 s & ~d
//            4'b0111: OR_REVERSE                  s | ~d
//            4'b1000: AND                         s & d
//            4'b1001: OR                          s | d
//            4'b1010: NAND                        ~(s & d)
//            4'b1011: NOR                         ~(s | d)
//            4'b1100: XOR                         s ^ d
//            4'b1101: EQUIV                       ~(s ^ d)
//            4'b1110: AND_INVERTED                ~s & d
//            4'b1111: OR_INVERTED                 ~s | d
//Bit 10:0  ALU alpha operation
//            bit10:8 Blending Equation Math Operation
//              3'b000: ADD               As*Fs + Ad*Fd
//              3'b001: SUBTRACT          As*Fs - Ad*Fd
//              3'b010: REVERSE SUBTRACT  Ad*Fd - As*Fs
//              3'b011: MIN               min(As*Fs, Ad*Fd)
//              3'b100: MAX               max(As*Fs, Ad*Fd)
//              3'b101: LOGIC OP          As op Ad
//            bit7:4 Source alpha Blending Factor AFs
//              4'b0000                       0
//              4'b0001                       1
//              4'b0010                       As
//              4'b0011                       1 - As
//              4'b0100                       Ad
//              4'b0101                       1 - Ad
//              4'b0110                       Ac
//              4'b0111                       1 - Ac
//               ....                         reserved
//            bit3:0 Destination alpha Blending Factor AFd, when bit10:8 != LOGIC OP
//              4'b0000                       0
//              4'b0001                       1
//              4'b0010                       As
//              4'b0011                       1 - As
//              4'b0100                       Ad
//              4'b0101                       1 - Ad
//              4'b0110                       Ac
//              4'b0111                       1 - Ac
//               ....                         reserved
//            bit3:0 logic operations, when bit10:8 == LOGIC OP
//              4'b0000: CLEAR                       0
//              4'b0001: COPY                        s
//              4'b0010: NOOP                        d
//              4'b0011: SET                         1
//              4'b0100: COPY_INVERT                 ~s
//              4'b0101: INVERT                      ~d
//              4'b0110: AND_REVERSE                 s & ~d
//              4'b0111: OR_REVERSE                  s | ~d
//              4'b1000: AND                         s & d
//              4'b1001: OR                          s | d
//              4'b1010: NAND                        ~(s & d)
//              4'b1011: NOR                         ~(s | d)
//              4'b1100: XOR                         s ^ d
//              4'b1101: EQUIV                       ~(s ^ d)
//              4'b1110: AND_INVERTED                ~s & d
//              4'b1111: OR_INVERTED                 ~s | d
#define GE2D_ALU_OP_CTRL                (GE2D_CBUS_BASE+0xcc)     //32'h00010010 

//bit 31:0 (RGBA,YCBCRA)
#define GE2D_ALU_CONST_COLOR            (GE2D_CBUS_BASE+0xcd)     //32'h00808000 

//SRC1 Key
//31:0 
#define GE2D_SRC1_KEY                   (GE2D_CBUS_BASE+0xce)     //'h0 

//SRC1 Key Mask
//31:0 
#define GE2D_SRC1_KEY_MASK              (GE2D_CBUS_BASE+0xcf)     //'h0 

//SRC2 Key
//31:0 
#define GE2D_SRC2_KEY                   (GE2D_CBUS_BASE+0xd0)     //'h0

//SRC2 Key Mask
//31:0 
#define GE2D_SRC2_KEY_MASK              (GE2D_CBUS_BASE+0xd1)     //'h0

//Destination Bit Mask
//31:0 
#define GE2D_DST_BITMASK                (GE2D_CBUS_BASE+0xd2)     //'h0 

//Bit 31    DP onoff mode, 0: on_counter means how many pixels will output before ge2d turns off
//                         1: on_counter means how many clocks will ge2d turn on before ge2d turns off
//Bit 30:16     DP on counter
//Bit 15        0: vd_format doesnt have onoff mode, 1: vd format has onoff mode
//Bit 14:0      DP off counter
#define GE2D_DP_ONOFF_CTRL              (GE2D_CBUS_BASE+0xd3)     //'h0


//Because there are many coefficients used in the vertical filter and horizontal filters,
//indirect access the coefficients of vertical filter and horizontal filter is used.
//For vertical filter, there are 33x4 coefficients 
//For horizontal filter, there are 33x4 coefficients
//Bit 15	index increment, if bit9 == 1  then (0: index increase 1, 1: index increase 2) else (index increase 2)	
//Bit 14	1: read coef through cbus enable, just for debug purpose in case when we wanna check the coef in ram in correct or not
//Bit 9     if true, use 9bit resolution coef, other use 8bit resolution coef
//Bit 8	    type of index, 0: vertical coef
//						   1: horizontal coef
//Bit 6:0 	coef index
#define GE2D_SCALE_COEF_IDX					(GE2D_CBUS_BASE+0xd4)	 //16'h0

//coefficients for vertical filter and horizontal filter
#define GE2D_SCALE_COEF							(GE2D_CBUS_BASE+0xd5)	 //32'h0

//Bit 24    src2 alpha fill mode, 0: repeat innner alpha, 1: fill src2 outside alpha
//Bit 24    src2 alpha fill mode: together with GE2D_GEN_CTRL0[4](fill_mode), define what alpha values are used
//                                for the area outside the clipping window. As below:
//                                fill_mode=0, alpha_fill_mode=0 : use inner alpha, (or default_alpha if src data have no alpha values);
//                                fill_mode=0, alpha_fill_mode=1 : use outside_alpha;
//                                fill_mode=1, alpha_fill_mode=0 : use default_alpha;
//                                fill_mode=1, alpha_fill_mode=1 : use outside_alpha.
//Bit 23:16 src2 outside alpha
//Bit 8     src1 alpha fill mode, refer to src2 alpha fill mode above. 
//Bit 7:0   src1 outside alpha
#define GE2D_SRC_OUTSIDE_ALPHA	    (GE2D_CBUS_BASE+0xd6)	 //32'h0

// synopsys translate_off


//========================================================================
//  MDEC Registers                  (0x900 - 0x9ff)
//========================================================================
// -----------------------------------------------
// CBUS_BASE:  MDEC_CBUS_BASE = 0x09
// -----------------------------------------------
#define MDEC_CBUS_BASE 0x900

#define MREG_MC_CTRL_REG        (MDEC_CBUS_BASE+0x00)
    #define MC_ENABLE       0x0001
    //#define MC_RESET      0x0002
    #define SKIP_MB         0x0004

#define MREG_MC_MB_INFO         (MDEC_CBUS_BASE+0x01)
    #define INTRA_MB        0x0001
    #define BWD_PRED        0x0004
    #define FWD_PRED        0x0008
    #define FLD_MOT         0x0100
    #define FRM_16x8_MOT    0x0200
    #define DUAL_PRM_MOT    0x0300
    #define FRM_DCT         0x0000
    #define FLD_DCT         0x0400
    #define I_PIC           0x0001
    #define P_PIC           0x0002
    #define B_PIC           0x0003
    #define FLD_PIC         0x0000
    #define FRM_PIC         0x0100



#define MREG_MC_PIC_INFO        (MDEC_CBUS_BASE+0x02)
#define MREG_MC_HALF_PEL_ONE    (MDEC_CBUS_BASE+0x03)
#define MREG_MC_HALF_PEL_TWO    (MDEC_CBUS_BASE+0x04)
#define MREG_POWER_CTL_MC       (MDEC_CBUS_BASE+0x05)
#define MREG_MC_CMD             (MDEC_CBUS_BASE+0x06)
#define MREG_MC_CTRL            (MDEC_CBUS_BASE+0x07)
#define MREG_MC_PIC_W           (MDEC_CBUS_BASE+0x08)
#define MREG_MC_PIC_H           (MDEC_CBUS_BASE+0x09)
#define MREG_MC_STATUS          (MDEC_CBUS_BASE+0x0a)
#define MREG_MC_RST             (MDEC_CBUS_BASE+0x0b)
#define MREG_MC_MIX_RATIO0      (MDEC_CBUS_BASE+0x0c)
#define MREG_MC_MIX_RATIO1      (MDEC_CBUS_BASE+0x0d)
#define MREG_MC_DP_MB_XY        (MDEC_CBUS_BASE+0x0e)     //read only
#define MREG_MC_OM_MB_XY        (MDEC_CBUS_BASE+0x0f)     //read only
#define PSCALE_RST              (MDEC_CBUS_BASE+0x10)
#define PSCALE_CTRL             (MDEC_CBUS_BASE+0x11)
#define PSCALE_PICI_W           (MDEC_CBUS_BASE+0x12)
#define PSCALE_PICI_H           (MDEC_CBUS_BASE+0x13)
#define PSCALE_PICO_W           (MDEC_CBUS_BASE+0x14)
#define PSCALE_PICO_H           (MDEC_CBUS_BASE+0x15)
#define PSCALE_PICO_START_X     (MDEC_CBUS_BASE+0x16)
#define PSCALE_PICO_START_Y     (MDEC_CBUS_BASE+0x17)
#define PSCALE_DUMMY            (MDEC_CBUS_BASE+0x18)
#define PSCALE_FILT0_COEF0      (MDEC_CBUS_BASE+0x19)
#define PSCALE_FILT0_COEF1      (MDEC_CBUS_BASE+0x1a)
#define PSCALE_CMD_CTRL         (MDEC_CBUS_BASE+0x1b)
#define PSCALE_CMD_BLK_X        (MDEC_CBUS_BASE+0x1c)
#define PSCALE_CMD_BLK_Y        (MDEC_CBUS_BASE+0x1d)
#define PSCALE_STATUS           (MDEC_CBUS_BASE+0x1e)
#define PSCALE_BMEM_ADDR        (MDEC_CBUS_BASE+0x1f)
#define PSCALE_BMEM_DAT         (MDEC_CBUS_BASE+0x20)
#define PSCALE_DRAM_BUF_CTRL    (MDEC_CBUS_BASE+0x21)
#define PSCALE_MCMD_CTRL        (MDEC_CBUS_BASE+0x22)
#define PSCALE_MCMD_XSIZE       (MDEC_CBUS_BASE+0x23)
#define PSCALE_MCMD_YSIZE       (MDEC_CBUS_BASE+0x24)
#define PSCALE_RBUF_START_BLKX  (MDEC_CBUS_BASE+0x25)
#define PSCALE_RBUF_START_BLKY  (MDEC_CBUS_BASE+0x26)
//#define PSCALE_RBUF_MB_WIDTH  (MDEC_CBUS_BASE+0x27)
#define PSCALE_PICO_SHIFT_XY    (MDEC_CBUS_BASE+0x28)
#define PSCALE_CTRL1            (MDEC_CBUS_BASE+0x29)
//Bit 15, wmask enable
//Bit 14:13, filt0 srckey_less,
//Bit 12:11, filt1 srckey_less, in the case of the interpolated data is equal distance to
//key data and normal data, 00: select normal data, 01: select right data, 10: select key data
//Bit 10:9, srckey mode, 00: equal, 01: less than or equal, 10: great than or equal
//Bit 8, src key enable
//Bit 7:0, y src key
#define PSCALE_SRCKEY_CTRL0     (MDEC_CBUS_BASE+0x2a)
//Bit 15:8, cb src key
//Bit 7:0, cr src key
#define PSCALE_SRCKEY_CTRL1     (MDEC_CBUS_BASE+0x2b)
//Bit 22:16 canvas_rd_addr2
//Bit 14:8 canvas_rd_addr1
//Bit 6:0 canvas_rd_addr1
#define PSCALE_CANVAS_RD_ADDR   (MDEC_CBUS_BASE+0x2c)
//Bit 22:16 canvas_wr_addr2
//Bit 14:8 canvas_wr_addr1
//Bit 6:0 canvas_wr_addr1
#define PSCALE_CANVAS_WR_ADDR   (MDEC_CBUS_BASE+0x2d)

#define MDEC_PIC_W              (MDEC_CBUS_BASE+0x8c)
#define MDEC_PIC_H              (MDEC_CBUS_BASE+0x8d)
#define MDEC_PIC_DC_CTRL        (MDEC_CBUS_BASE+0x8e)
#define MDEC_PIC_DC_STATUS      (MDEC_CBUS_BASE+0x8f)
#define ANC00_CANVAS_ADDR		(MDEC_CBUS_BASE+0x90)
#define ANC01_CANVAS_ADDR		(MDEC_CBUS_BASE+0x91)
#define ANC02_CANVAS_ADDR		(MDEC_CBUS_BASE+0x92)
#define ANC03_CANVAS_ADDR		(MDEC_CBUS_BASE+0x93)
#define ANC04_CANVAS_ADDR		(MDEC_CBUS_BASE+0x94)
#define ANC05_CANVAS_ADDR		(MDEC_CBUS_BASE+0x95)
#define ANC06_CANVAS_ADDR		(MDEC_CBUS_BASE+0x96)
#define ANC07_CANVAS_ADDR		(MDEC_CBUS_BASE+0x97)
#define ANC08_CANVAS_ADDR		(MDEC_CBUS_BASE+0x98)
#define ANC09_CANVAS_ADDR		(MDEC_CBUS_BASE+0x99)
#define ANC10_CANVAS_ADDR		(MDEC_CBUS_BASE+0x9a)
#define ANC11_CANVAS_ADDR		(MDEC_CBUS_BASE+0x9b)
#define ANC12_CANVAS_ADDR		(MDEC_CBUS_BASE+0x9c)
#define ANC13_CANVAS_ADDR		(MDEC_CBUS_BASE+0x9d)
#define ANC14_CANVAS_ADDR		(MDEC_CBUS_BASE+0x9e)
#define ANC15_CANVAS_ADDR		(MDEC_CBUS_BASE+0x9f)
#define ANC16_CANVAS_ADDR		(MDEC_CBUS_BASE+0xa0)
#define ANC17_CANVAS_ADDR		(MDEC_CBUS_BASE+0xa1)
#define ANC18_CANVAS_ADDR		(MDEC_CBUS_BASE+0xa2)
#define ANC19_CANVAS_ADDR		(MDEC_CBUS_BASE+0xa3)
#define ANC20_CANVAS_ADDR		(MDEC_CBUS_BASE+0xa4)
#define ANC21_CANVAS_ADDR		(MDEC_CBUS_BASE+0xa5)
#define ANC22_CANVAS_ADDR		(MDEC_CBUS_BASE+0xa6)
#define ANC23_CANVAS_ADDR		(MDEC_CBUS_BASE+0xa7)
#define ANC24_CANVAS_ADDR		(MDEC_CBUS_BASE+0xa8)
#define ANC25_CANVAS_ADDR		(MDEC_CBUS_BASE+0xa9)
#define ANC26_CANVAS_ADDR		(MDEC_CBUS_BASE+0xaa)
#define ANC27_CANVAS_ADDR		(MDEC_CBUS_BASE+0xab)
#define ANC28_CANVAS_ADDR		(MDEC_CBUS_BASE+0xac)
#define ANC29_CANVAS_ADDR		(MDEC_CBUS_BASE+0xad)
#define ANC30_CANVAS_ADDR		(MDEC_CBUS_BASE+0xae)
#define ANC31_CANVAS_ADDR		(MDEC_CBUS_BASE+0xaf)
#define DBKR_CANVAS_ADDR        (MDEC_CBUS_BASE+0xb0)
#define DBKW_CANVAS_ADDR        (MDEC_CBUS_BASE+0xb1)
#define REC_CANVAS_ADDR         (MDEC_CBUS_BASE+0xb2)

#define AV_SCRATCH_0        	(MDEC_CBUS_BASE+0xc0)
#define AV_SCRATCH_1        	(MDEC_CBUS_BASE+0xc1)
#define AV_SCRATCH_2        	(MDEC_CBUS_BASE+0xc2)
#define AV_SCRATCH_3        	(MDEC_CBUS_BASE+0xc3)
#define AV_SCRATCH_4        	(MDEC_CBUS_BASE+0xc4)
#define AV_SCRATCH_5        	(MDEC_CBUS_BASE+0xc5)
#define AV_SCRATCH_6        	(MDEC_CBUS_BASE+0xc6)
#define AV_SCRATCH_7        	(MDEC_CBUS_BASE+0xc7)
#define AV_SCRATCH_8        	(MDEC_CBUS_BASE+0xc8)
#define AV_SCRATCH_9        	(MDEC_CBUS_BASE+0xc9)
#define AV_SCRATCH_a        	(MDEC_CBUS_BASE+0xca)
#define AV_SCRATCH_b        	(MDEC_CBUS_BASE+0xcb)
#define AV_SCRATCH_c        	(MDEC_CBUS_BASE+0xcc)
#define AV_SCRATCH_d        	(MDEC_CBUS_BASE+0xcd)
#define AV_SCRATCH_e        	(MDEC_CBUS_BASE+0xce)
#define AV_SCRATCH_f        	(MDEC_CBUS_BASE+0xcf)

//========================================================================
// DBLK Register:  0x950 - 0x97f 
//========================================================================
#define MREG_DBLK_RST			(MDEC_CBUS_BASE+0x50)
#define MREG_DBLK_CTRL          (MDEC_CBUS_BASE+0x51)
#define MREG_DBLK_MB_WID_HEIGHT (MDEC_CBUS_BASE+0x52)
#define MREG_DBLK_STATUS        (MDEC_CBUS_BASE+0x53)
#define MREG_DBLK_CMD_CTRL      (MDEC_CBUS_BASE+0x54)
#define MREG_DBLK_MB_XY         (MDEC_CBUS_BASE+0x55)
#define MREG_DBLK_QP            (MDEC_CBUS_BASE+0x56)
#define MREG_DBLK_Y_BHFILT      (MDEC_CBUS_BASE+0x57)
#define MREG_DBLK_Y_BHFILT_HIGH (MDEC_CBUS_BASE+0x58)
#define MREG_DBLK_Y_BVFILT      (MDEC_CBUS_BASE+0x59)
#define MREG_DBLK_CB_BFILT      (MDEC_CBUS_BASE+0x5a)
#define MREG_DBLK_CR_BFILT      (MDEC_CBUS_BASE+0x5b)
#define MREG_DBLK_Y_HFILT       (MDEC_CBUS_BASE+0x5c)
#define MREG_DBLK_Y_HFILT_HIGH  (MDEC_CBUS_BASE+0x5d)
#define MREG_DBLK_Y_VFILT       (MDEC_CBUS_BASE+0x5e)
#define MREG_DBLK_CB_FILT       (MDEC_CBUS_BASE+0x5f)
#define MREG_DBLK_CR_FILT       (MDEC_CBUS_BASE+0x60)
#define MREG_DBLK_BETAX_QP_SEL  (MDEC_CBUS_BASE+0x61)
#define MREG_DBLK_CLIP_CTRL0    (MDEC_CBUS_BASE+0x62)
#define MREG_DBLK_CLIP_CTRL1    (MDEC_CBUS_BASE+0x63)
#define MREG_DBLK_CLIP_CTRL2    (MDEC_CBUS_BASE+0x64)
#define MREG_DBLK_CLIP_CTRL3    (MDEC_CBUS_BASE+0x65)
#define MREG_DBLK_CLIP_CTRL4    (MDEC_CBUS_BASE+0x66)
#define MREG_DBLK_CLIP_CTRL5    (MDEC_CBUS_BASE+0x67)
#define MREG_DBLK_CLIP_CTRL6    (MDEC_CBUS_BASE+0x68)
#define MREG_DBLK_CLIP_CTRL7    (MDEC_CBUS_BASE+0x69)
#define MREG_DBLK_CLIP_CTRL8    (MDEC_CBUS_BASE+0x6a)
// bit 15:0
#define MREG_DBLK_CBPY          (MDEC_CBUS_BASE+0x70)
// bit 11:8 -- deblk_cbpy_bottom
// bit 7:4 -- deblk_cbpy_left
// bit 3:0 -- deblk_cbpy_top
#define MREG_DBLK_CBPY_ADJ      (MDEC_CBUS_BASE+0x71)
// bit 7:0 -- deblk_cbpc
#define MREG_DBLK_CBPC          (MDEC_CBUS_BASE+0x72)
// bit 15 -- bottom_mb
// bit 14 -- left_mb
// bit 13 -- top_mb
// bit 12 -- reserved
// bit 11:8 -- deblk_cbpc_bottom
// bit 7:4 -- deblk_cbpc_left
// bit 3:0 -- deblk_cbpc_top
#define MREG_DBLK_CBPC_ADJ      (MDEC_CBUS_BASE+0x73)
// bit 15:8 -- deblk_hmvd   -- {left_1, left_0, below_1, below_0, block3-0} 
// bit 7:0 -- deblk_vmvd    -- {top_1, top_0, below_1, below_0, block3-0} 
#define MREG_DBLK_VHMVD         (MDEC_CBUS_BASE+0x74)
// bit 13:12 -- right_vmvd
// bit 11 -- right_above_vmvd
// bit 10 -- left_below_hmvd
// bit 9 -- disable_dblk_luma  
// bit 8 -- disable_dblk_chroma
// bit 7 -- bBelowRefDiff
// bit 6 -- bLeftRefDiff
// bit 5 -- bAboveRefDiff
// bit 4 -- reserved
// bit 3 -- s_below
// bit 2 -- s_left
// bit 1 -- s_above
// bit 0 -- s 
#define MREG_DBLK_STRONG        (MDEC_CBUS_BASE+0x75)
// bit 14:10 -- PQUANT 
// bit 9:5 -- left_PQUANT 
// bit 4:0 -- top_PQUANT 
#define MREG_DBLK_RV8_QUANT     (MDEC_CBUS_BASE+0x76)

// -----------------------------------------------   
// CBUS_BASE:  VIU_CBUS_BASE = 0x1a                  
// -----------------------------------------------   
#define VIU_CBUS_BASE 0x1a00

//========================================================================
//  OSD Registers				    (0xa10 - 0xa4c)
//========================================================================


// Bit 31:29 Reserved for status signals
// Bit    28 R, test_rd_dsr
// Bit    27 R, osd_done
// Bit 26:24 R, osd_blk_mode
// Bit 23:22 R, osd_blk_ptr
// Bit    21 R, osd_enable
//
// Bit 20:12 RW, global_alpha
// Bit    11 RW, test_rd_en
// Bit 10: 9 Reserved for control signals
// Bit  8: 5 RW, ctrl_mtch_y
// Bit     4 RW, ctrl_422to444
// Bit  3: 0 RW, osd_blk_enable. Bit 0 to enable block 0: 1=enable, 0=disable;
//                               Bit 1 to enable block 1, and so on.
#define VIU_OSD1_CTRL_STAT          (VIU_CBUS_BASE+0x10)

// Bit 31: 9 Reserved
// Bit     8 RW, 0 = Write LUT, 1 = Read LUT
// Bit  7: 0 RW, lut_addr
#define VIU_OSD1_COLOR_ADDR         (VIU_CBUS_BASE+0x11)

// Bit 31:24 RW, Y or R
// Bit 23:16 RW, Cb or G
// Bit 15: 8 RW, Cr or B
// Bit  7: 0 RW, Alpha
#define VIU_OSD1_COLOR              (VIU_CBUS_BASE+0x12)

// Bit 27:16	R/W 	0	V_END. Display vertical co-ordinate.
// Bit 11-0		R/W 	0	V_START. Display vertical co-ordinate.
#define VIU_OSD1_BLK0_CFG_W4		(VIU_CBUS_BASE+0x13)
#define VIU_OSD1_BLK1_CFG_W4		(VIU_CBUS_BASE+0x14)
#define VIU_OSD1_BLK2_CFG_W4		(VIU_CBUS_BASE+0x15)
#define VIU_OSD1_BLK3_CFG_W4		(VIU_CBUS_BASE+0x16)

// Bit 31:24 RW, Y or R
// Bit 23:16 RW, Cb or G
// Bit 15: 8 RW, Cr or B
// Bit  7: 0 RW, Alpha
#define VIU_OSD1_TCOLOR_AG0         (VIU_CBUS_BASE+0x17)
#define VIU_OSD1_TCOLOR_AG1         (VIU_CBUS_BASE+0x18)
#define VIU_OSD1_TCOLOR_AG2         (VIU_CBUS_BASE+0x19)
#define VIU_OSD1_TCOLOR_AG3         (VIU_CBUS_BASE+0x1a)

// Bit 31:26 RW, y_start[5:0]
// Bit 25:13 RW, x_end
// Bit 12: 0 RW, y_end
#define VIU_OSD1_BLK0_CFG_W0        (VIU_CBUS_BASE+0x1b) 
    #define  OSD_INTERLACE_BIT          0
    #define  OSD_INTERLACE_WID          2
    #define  OSD_INTERLACE_DISABLE      (0<<OSD_INTERLACE_BIT)
    #define  OSD_INTERLACE_ODD          (3<<OSD_INTERLACE_BIT)
    #define  OSD_INTERLACE_EVEN         (2<<OSD_INTERLACE_BIT)
#define VIU_OSD1_BLK0_CFG_W1        (VIU_CBUS_BASE+0x1c) 
#define VIU_OSD1_BLK0_CFG_W2        (VIU_CBUS_BASE+0x1d) 
#define VIU_OSD1_BLK0_CFG_W3        (VIU_CBUS_BASE+0x1e)

// Bit 31:27 RW, v_end[4:0]
// Bit 26:20 RW, tbl_addr
// Bit 19: 7 RW, x_start
// Bit  6: 0 RW, y_start[12:6]
#define VIU_OSD1_BLK1_CFG_W0        (VIU_CBUS_BASE+0x1f)
#define VIU_OSD1_BLK1_CFG_W1        (VIU_CBUS_BASE+0x20)
#define VIU_OSD1_BLK1_CFG_W2        (VIU_CBUS_BASE+0x21)
#define VIU_OSD1_BLK1_CFG_W3        (VIU_CBUS_BASE+0x22)

// Bit    31 RW, h_start[0]
// Bit 30:19 RW, v_start
// Bit 18: 7 RW, h_end
// Bit  6: 0 RW, v_end[11:5]
#define VIU_OSD1_BLK2_CFG_W0        (VIU_CBUS_BASE+0x23)
#define VIU_OSD1_BLK2_CFG_W1        (VIU_CBUS_BASE+0x24)
#define VIU_OSD1_BLK2_CFG_W2        (VIU_CBUS_BASE+0x25)
#define VIU_OSD1_BLK2_CFG_W3        (VIU_CBUS_BASE+0x26)

// Bit 31:23 Reserved
// Bit    22 RW, rpt_y
// Bit 21:20 RW, interp_ctrl. 0x=No interpolation; 10=Interpolate with previous
//                            pixel; 11=Interpolate with the average value
//                            between previous and next pixel.
// Bit 19:17 RW, osd_blk_mode
// Bit    16 RW, rgb_en
// Bit    15 RW, tc_alpha_en
// Bit 14:13 RW, color_matrix
// Bit    12 RW, interlace_en
// Bit    11 RW, interlace_sel_odd
// Bit 10: 0 RW, h_start[11:1]
#define VIU_OSD1_BLK3_CFG_W0        (VIU_CBUS_BASE+0x27)
#define VIU_OSD1_BLK3_CFG_W1        (VIU_CBUS_BASE+0x28)
#define VIU_OSD1_BLK3_CFG_W2        (VIU_CBUS_BASE+0x29)
#define VIU_OSD1_BLK3_CFG_W3        (VIU_CBUS_BASE+0x2a)

// Bit 31:30 Reserved
// Bit 29:21 R, fifo_count
// Bit 20:19 R, fifo_st. 0=IDLE, 1=FILL, 2=ABORT 
// Bit    18 R, fifo_overflow
//
// Bit 17:12 RW, fifo_depth_val, max value=32: set actual fifo depth to fifo_depth_val*8.
// Bit 11:10 RW, burst_len_sel. 0=24(default), 1=32, 2=48, 3=64.
// Bit  9: 5 RW, hold_fifo_lines
// Bit     4 RW, clear_err: one pulse to clear fifo_overflow
// Bit     3 RW, fifo_sync_rst
// Bit  2: 1 RW, endian
// Bit     0 RW, urgent
#define VIU_OSD1_FIFO_CTRL_STAT     (VIU_CBUS_BASE+0x2b)

// Bit 31:24 R, Y or R
// Bit 23:16 R, Cb or G
// Bit 15: 8 R, Cr or B
// Bit  7: 0 R, Output Alpha[8:1]
#define VIU_OSD1_TEST_RDDATA        (VIU_CBUS_BASE+0x2c)

// Bit 2	R/W 	0	ALPHA_9B_MODE: Define how to expand 8-bit alpha value to 9-bit.
// 					1 = The formula is (Alpha < 128) ? Alpha : Alpha + 1;
// 					2 = The formula is (Alpha ==   0) ? Alpha : Alpha + 1.
// Bit 1	R/W 	0	ALPHA_EXPAND_MODE.
//					1 = Expand alpha value to 8-bit by padding LSBs with MSBs. E.g. If the input is 5¡¯b11000, the output is expanded to 8¡¯b11000110;
//					0 = If input alpha value is all 1, then expand the value to 8-bit by padding LSBs with 1; otherwise, pad LSBs with 0.
// Bit 0	R/W 	0	COLOR_EXPAND_MODE.
//					1 = Expand the color components to 8-bit by padding LSBs with MSBs. E.g. If the input is 5¡¯b11000, the output is expanded to 8¡¯b11000110;
//					0 = Expand the color components to 8-bit by padding LSBs with 0.


#define VIU_OSD1_CTRL_STAT2         (VIU_CBUS_BASE+0x2d)

//------------------------------------------------------------------------------
// OSD2 registers
//------------------------------------------------------------------------------
// Bit    31 Reserved
// Bit    30 RW, enable_free_clk: 1=use free-running clock to drive logics;
//                                0=use gated clock for low power.
// Bit    29 R, test_rd_dsr
// Bit    28 R, osd_done
// Bit 27:24 R, osd_blk_mode
// Bit 23:22 R, osd_blk_ptr
// Bit    21 R, osd_enable
//
// Bit 20:12 RW, global_alpha
// Bit    11 RW, test_rd_en
// Bit    10 RW, hl2_en
// Bit     9 RW, hl1_en
// Bit  8: 5 RW, ctrl_mtch_y
// Bit     4 RW, ctrl_422to444
// Bit  3: 0 RW, osd_blk_enable. Bit 0 to enable block 0: 1=enable, 0=disable;
//                               Bit 1 to enable block 1, and so on.
#define VIU_OSD2_CTRL_STAT                         0x1a30
// Bit 31: 3 Reserved
// Bit     2 RW, alpha_9b_mode
// Bit     1 RW, alpha_expand_mode
// Bit     0 RW, color_expand_mode
#define VIU_OSD2_CTRL_STAT2                        0x1a4d
// Bit 31: 9 Reserved
// Bit     8 RW, 0 = Write LUT, 1 = Read LUT
// Bit  7: 0 RW, lut_addr
#define VIU_OSD2_COLOR_ADDR                        0x1a31
// Bit 31:24 RW, Y or R
// Bit 23:16 RW, Cb or G
// Bit 15: 8 RW, Cr or B
// Bit  7: 0 RW, Alpha
#define VIU_OSD2_COLOR                             0x1a32
// Bit 31:28 Reserved
// Bit 27:16 RW, hl[1-2]_h/v_start
// Bit 15:12 Reserved
// Bit 11: 0 RW, hl[1-2]_h/v_end
#define VIU_OSD2_HL1_H_START_END                   0x1a33
#define VIU_OSD2_HL1_V_START_END                   0x1a34
#define VIU_OSD2_HL2_H_START_END                   0x1a35
#define VIU_OSD2_HL2_V_START_END                   0x1a36
// Bit 31:24 RW, Y or R
// Bit 23:16 RW, Cb or G
// Bit 15: 8 RW, Cr or B
// Bit  7: 0 RW, Alpha
#define VIU_OSD2_TCOLOR_AG0                        0x1a37
#define VIU_OSD2_TCOLOR_AG1                        0x1a38
#define VIU_OSD2_TCOLOR_AG2                        0x1a39
#define VIU_OSD2_TCOLOR_AG3                        0x1a3a
// Bit 31:24 Reserved
// Bit 23:16 RW, tbl_addr
// Bit    15 RW, little_endian: 0=big endian, 1=little endian
// Bit    14 RW, rpt_y
// Bit 13:12 RW, interp_ctrl. 0x=No interpolation; 10=Interpolate with previous
//                            pixel; 11=Interpolate with the average value
//                            between previous and next pixel.
// Bit 11: 8 RW, osd_blk_mode
// Bit     7 RW, rgb_en
// Bit     6 RW, tc_alpha_en
// Bit  5: 2 RW, color_matrix
// Bit     1 RW, interlace_en
// Bit     0 RW, interlace_sel_odd
#define VIU_OSD2_BLK0_CFG_W0                       0x1a3b
#define VIU_OSD2_BLK1_CFG_W0                       0x1a3f
#define VIU_OSD2_BLK2_CFG_W0                       0x1a43
#define VIU_OSD2_BLK3_CFG_W0                       0x1a47
// Bit 31:29 Reserved
// Bit 28:16 RW, x_end
// Bit 15:13 Reserved
// Bit 12: 0 RW, x_start
#define VIU_OSD2_BLK0_CFG_W1                       0x1a3c
#define VIU_OSD2_BLK1_CFG_W1                       0x1a40
#define VIU_OSD2_BLK2_CFG_W1                       0x1a44
#define VIU_OSD2_BLK3_CFG_W1                       0x1a48
// Bit 31:29 Reserved
// Bit 28:16 RW, y_end
// Bit 15:13 Reserved
// Bit 12: 0 RW, y_start
#define VIU_OSD2_BLK0_CFG_W2                       0x1a3d
#define VIU_OSD2_BLK1_CFG_W2                       0x1a41
#define VIU_OSD2_BLK2_CFG_W2                       0x1a45
#define VIU_OSD2_BLK3_CFG_W2                       0x1a49
// Bit 31:28 Reserved
// Bit 27:16 RW, h_end
// Bit 15:12 Reserved
// Bit 11: 0 RW, h_start
#define VIU_OSD2_BLK0_CFG_W3                       0x1a3e
#define VIU_OSD2_BLK1_CFG_W3                       0x1a42
#define VIU_OSD2_BLK2_CFG_W3                       0x1a46
#define VIU_OSD2_BLK3_CFG_W3                       0x1a4a
// Bit 31:28 Reserved
// Bit 27:16 RW, v_end
// Bit 15:12 Reserved
// Bit 11: 0 RW, v_start
#define VIU_OSD2_BLK0_CFG_W4                       0x1a64
#define VIU_OSD2_BLK1_CFG_W4                       0x1a65
#define VIU_OSD2_BLK2_CFG_W4                       0x1a66
#define VIU_OSD2_BLK3_CFG_W4                       0x1a67
// Bit    31 Reserved
// Bit    30 RW, byte_swap: In addition to endian control, further define
//               whether to swap upper and lower byte within a 16-bit mem word.
//               0=No swap; 1=Swap data[15:0] to be {data[7:0], data[15:8]}
// Bit 29:21 R,  fifo_count
// Bit 20:19 R,  fifo_st. 0=IDLE, 1=FILL, 2=ABORT 
// Bit    18 R,  fifo_overflow
//
// Bit 17:12 RW, fifo_depth_val, max value=32: set actual fifo depth to fifo_depth_val*8.
// Bit 11:10 RW, burst_len_sel. 0=24(default), 1=32, 2=48, 3=64.
// Bit  9: 5 RW, hold_fifo_lines
// Bit     4 RW, clear_err: one pulse to clear fifo_overflow
// Bit     3 RW, fifo_sync_rst
// Bit  2: 1 RW, endian
// Bit     0 RW, urgent
#define VIU_OSD2_FIFO_CTRL_STAT                    0x1a4b
// Bit 31:24 R, Y or R
// Bit 23:16 R, Cb or G
// Bit 15: 8 R, Cr or B
// Bit  7: 0 R, Output Alpha[8:1]
#define VIU_OSD2_TEST_RDDATA                       0x1a4c

//========================================================================
//  VLD Registers				    (0xc00 - 0xd00)
//========================================================================
// -----------------------------------------------
// CBUS_BASE:  VLD_CBUS_BASE = 0x0c
// -----------------------------------------------
#define VLD_CBUS_BASE 0x0c00

#define MREG_VLD_STATUS_CTRL    (VLD_CBUS_BASE+0x00)
//
// bit 10 -- use_old_shift_en
// bit 9 -- output_mv_not_pmv
// bit 8:5 -- force_zigzag
// bit 4 -- force_zigzag_en
// bit 3 -- disable_viff_anempty_int
// bit 2 -- disable_m2_ac_coeff_one_cycle
// bit 1 -- forced_reset  force reset pmv
// bit 0 -- mpeg_type  0:mpeg1 1: mpeg2
#define MREG_MPEG1_2_REG        (VLD_CBUS_BASE+0x01)
#define MREG_F_CODE_REG         (VLD_CBUS_BASE+0x02)
#define MREG_PIC_HEAD_INFO      (VLD_CBUS_BASE+0x03)
#define MREG_SLICE_VER_POS_PIC_TYPE  (VLD_CBUS_BASE+0x04)
#define MREG_QP_VALUE_REG       (VLD_CBUS_BASE+0x05)
#define MREG_MBA_INC            (VLD_CBUS_BASE+0x06)
#define MREG_MB_MOTION_MODE     (VLD_CBUS_BASE+0x07)
//#define PACKET_BYTE_COUNT   (VLD_CBUS_BASE+0x08)
// bit 15 -- force_search_startcode_en
// bit 14 -- int_cpu_when_error (before do anything)
// bit 13 -- vld_error_reset
// bit 12 -- return_on_slice_header
// bit 6 -- jpeg_ff00_en 
// bit 5:0 -- vld_power_ctl
#define MREG_POWER_CTL_VLD      (VLD_CBUS_BASE+0x08)
	#define FORCE_VIFF_RD		(1<<4)
#define MREG_MB_WIDTH	        (VLD_CBUS_BASE+0x09)
#define MREG_SLICE_QP           (VLD_CBUS_BASE+0x0a)
#define MREG_MB_X_MB_Y          (VLD_CBUS_BASE+0x0b)   /* current MBX and MBY                  */

#define MREG_FST_FOR_MV_X       (VLD_CBUS_BASE+0x10)
#define MREG_FST_FOR_MV_Y       (VLD_CBUS_BASE+0x11)
#define MREG_SCD_FOR_MV_X       (VLD_CBUS_BASE+0x12)
#define MREG_SCD_FOR_MV_Y       (VLD_CBUS_BASE+0x13)
#define MREG_FST_BAK_MV_X       (VLD_CBUS_BASE+0x14)
#define MREG_FST_BAK_MV_Y       (VLD_CBUS_BASE+0x15)
#define MREG_SCD_BAK_MV_X       (VLD_CBUS_BASE+0x16)
#define MREG_SCD_BAK_MV_Y       (VLD_CBUS_BASE+0x17)
#define MREG_HALF_PEL_ONE       (VLD_CBUS_BASE+0x18)
#define MREG_HALF_PEL_TWO       (VLD_CBUS_BASE+0x19)

#define MREG_VIFF_BIT_CNT       (VLD_CBUS_BASE+0x1a)
#define MREG_BYTE_ALIGN_PEAK_HI (VLD_CBUS_BASE+0x1b) 
#define MREG_BYTE_ALIGN_PEAK_LO (VLD_CBUS_BASE+0x1c)

#define MREG_PMV1_X             (VLD_CBUS_BASE+0x20)
#define MREG_PMV1_Y             (VLD_CBUS_BASE+0x21)
#define MREG_PMV2_X             (VLD_CBUS_BASE+0x22)
#define MREG_PMV2_Y             (VLD_CBUS_BASE+0x23)
#define MREG_PMV3_X             (VLD_CBUS_BASE+0x24)
#define MREG_PMV3_Y             (VLD_CBUS_BASE+0x25)
#define MREG_PMV4_X             (VLD_CBUS_BASE+0x26)
#define MREG_PMV4_Y             (VLD_CBUS_BASE+0x27)
/*
  bit 15:8 -- rv_aic_vlc_index
  bit  7:0 -- m4_table_select
*/
#define MREG_M4_TABLE_SELECT    (VLD_CBUS_BASE+0x28)  /* Write Only */ 
#define MREG_M4_TABLE_OUTPUT    (VLD_CBUS_BASE+0x28)  /* Read Only */
/* 
  bit 0 -- MPEG4_EN 
  bit 2:1 -- MPEG4_TYPE : 00-DIVX5 01-DIVX311 02-WMV9 
  bit 3 -- reserved
  bit 4 -- byte_aligned_zero_23 (Read Only)
  bit 5 -- zero_23 (Read Only)
  bit 6 -- force_byte_align_en
  bit 7 -- Reserved
  bit 8 -- jpeg_en
  bit 9 -- startcode_ff
  bit 10 -- discard_hiff
  bit 11 -- real_en
  bit 12 -- real_rv8
  bit 13 -- h264_en        
  bit 14 -- resync_marker  // read only
  bit 15 -- pre_resync_marker // read only
*/
#define MREG_M4_CONTROL_REG     (VLD_CBUS_BASE+0x29)  /* RW */
/* 
 For RealVideo 
  bit 4:0 - block_number
 Else
  bit 2:0 - block_number
  // DIVX311
  bit 3 - ac_intra_lum_00 select
  bit 4 - ac_intra_lum_10 select
  bit 5 - ac_intra_lum_11 select
  bit 6 - ac_inter_or_chr_00 select
  bit 7 - ac_inter_or_chr_10 select
  bit 8 - ac_inter_or_chr_11 select
  bit 9 - swap_voff_wa
  // Mpeg4
  bit 10 - dc_intra_ac
*/
#define MREG_BLOCK_NUM          (VLD_CBUS_BASE+0x2a)  /* Write-Only */
// for RealVideo use bit 23:0
// for Mpeg2/4 use bit 5:0
#define MREG_PATTERN_CODE       (VLD_CBUS_BASE+0x2b)  /* Write-Only */
/*
  bit 0 -- mb_intra 
  bit 1 -- mb_pattern 
  bit 2 -- mb_m_bwd 
  bit 3 -- mb_m_fwd 
  bit 4 -- mb_quant 
*/
#define MREG_MB_INFO            (VLD_CBUS_BASE+0x2c)  /* Write-Only */
/* 
   bit 12:0 -- 13 bit DC_PRED value 
   bit 13   -- pred_direction 0 - Left 1 - Top
   bit 14   -- acpred_flag
   bit 15   -- 13-bit used 
*/
#define MREG_DC_PRED            (VLD_CBUS_BASE+0x2d)  /* RW */
#define MREG_VLD_ERROR_MASK     (VLD_CBUS_BASE+0x2e)  /* Write-Only */
// Bit 28:16 -- 13 bit DC_PRED_V value
// Bit 12:0  -- 13 bit DC_PRED_U value
#define MREG_DC_PRED_C          (VLD_CBUS_BASE+0x2f)  /* RW */

#define MREG_LAST_SLICE_MV_ADDR (VLD_CBUS_BASE+0x30)
#define MREG_LAST_MVX           (VLD_CBUS_BASE+0x31)
#define MREG_LAST_MVY           (VLD_CBUS_BASE+0x32)

// VIFIFO DDR Interface
// The VIFIFO start pointer into DDR memory is a 32-bit number
// The Start pointer will automatically be truncated to land on 
// an 8-byte boundary.  That is, bits [2:0] = 0;
#define VLD_MEM_VIFIFO_START_PTR    (VLD_CBUS_BASE+0x40)
// The current pointer points so some location between the START and END 
// pointers.  The current pointer is a BYTE pointer.  That is, you can 
// point to any BYTE address within the START/END range
#define VLD_MEM_VIFIFO_CURR_PTR     (VLD_CBUS_BASE+0x41)
#define VLD_MEM_VIFIFO_END_PTR      (VLD_CBUS_BASE+0x42)
#define VLD_MEM_VIFIFO_BYTES_AVAIL  (VLD_CBUS_BASE+0x43)
// VIFIFO FIFO Control
// bit  [31:24] viff_empty_int_enable_cpu[7:0] 
// bit  [23:16] viff_empty_int_enable_amrisc[7:0] 
//   -bit 23 Video BUFFER < 0x400 Bytes
//   -bit 22 Video BUFFER < 0x200 Bytes
//   -bit 21 Video BUFFER < 0x100 Bytes
//   -bit 20 Video BUFFER < 0x80 Bytes
//   -bit 19 Video BUFFER < 0x40 Bytes
//   -bit 18 Video BUFFER < 0x20 Bytes
//   -bit 17 vififo  < 16 double words
//   -bit 16 vififo  < 8 double words
// bit  [15:13] unused
// bit  [12]    A_urgent
// bit  [11]    transfer_length 0 - 32x64 Bits per request, 1 - 16x64 Bits per request
// bit  [10]    use_level       Set this bit to 1 to enable filling of the FIFO controlled by the buffer
//                              level control.  If this bit is 0, then use bit[1] to control the enabling of filling
// bit  [9]     Data Ready.     This bit is set when data can be popped
// bit  [8]     fill busy       This bit will be high when we're fetching data from the DDR memory
//                              To reset this module, set cntl_enable = 0, and then wait for busy = 0. 
//                              After that you can pulse cntl_init to start over
// bit  [7]     unused
// bit  [6]     unused
// bits [5:3]   endian:         see $lib/rtl/ddr_endian.v
// bit  [2]     cntl_empty_en   Set to 1 to enable reading the DDR memory FIFO
//                              Set cntl_empty_en = cntl_fill_en = 0 when pulsing cntl_init
// bit  [1]     cntl_fill_en    Set to 1 to enable reading data from DDR memory
// bit  [0]     cntl_init:      After setting the read pointers, sizes, channel masks
//                              and read masks, set this bit to 1 and then to 0
//                              NOTE:  You don't need to pulse cntl_init if only the start address is
//                              being changed
#define VLD_MEM_VIFIFO_CONTROL      (VLD_CBUS_BASE+0x44)
    #define MEM_LEVEL_CNT_BIT       18
    #define MEM_FIFO_CNT_BIT        16
    #define MEM_CTRL_AURGENT		(1<<12)
	#define MEM_FILL_ON_LEVEL		(1<<10)
	#define MEM_CTRL_EMPTY_EN		(1<<2)
	#define MEM_CTRL_FILL_EN		(1<<1)
	#define MEM_CTRL_INIT        	(1<<0)
// --------------------------------------------
// VIFIFO Buffer Level Manager
// --------------------------------------------
#define VLD_MEM_VIFIFO_WP           (VLD_CBUS_BASE+0x45) // Manual or Parser write pointer
#define VLD_MEM_VIFIFO_RP           (VLD_CBUS_BASE+0x46)
#define VLD_MEM_VIFIFO_LEVEL        (VLD_CBUS_BASE+0x47) // must be low first to shadow low/high 16-bit words
//
// bit  [1]     manual mode     Set to 1 for manual write pointer mode
// bit  [0]     Init            Set high then low after everything has been initialized
#define VLD_MEM_VIFIFO_BUF_CNTL     (VLD_CBUS_BASE+0x48)
	#define MEM_BUFCTRL_MANUAL		(1<<1)
	#define MEM_BUFCTRL_INIT		(1<<0)

//========================================================================
//  IQ/IDCT Registers				    (0xe00 - 0xeff)
//========================================================================    
// -----------------------------------------------  
// CBUS_BASE:  IQIDCT_CBUS_BASE = 0x0e              
// -----------------------------------------------  
#define IQIDCT_CBUS_BASE 0x0e00

#define VCOP_CTRL_REG               (IQIDCT_CBUS_BASE+0x00)
    //#define IQIDCT_RESET	    0x0001	    // Bit 0
    #define QM_WEN		        0x0002	    // Bit 1
    #define IQIDCT_ENABLE	    0x0004	    // Bit 2
    #define INTRA_QM	        0x0008	    // Bit 3	0 = Use default; 1 = use loaded
    #define NINTRA_QM	        0x0010	    // Bit 4	0 = Use default; 1 = use loaded
#define QP_CTRL_REG                 (IQIDCT_CBUS_BASE+0x01)
    #define INTRA_MODE	        0x0080	    // Bit 7
    #define FLD_DCT_TYPE	    0x0100	    // Bit 8	0 = Frame DCT; 1 = field DCT
#define INTRA_QUANT_MATRIX          (IQIDCT_CBUS_BASE+0x02)
#define NON_I_QUANT_MATRIX          (IQIDCT_CBUS_BASE+0x03)
#define DC_SCALER                   (IQIDCT_CBUS_BASE+0x04)
#define DC_AC_CTRL                  (IQIDCT_CBUS_BASE+0x05)
#define DC_AC_SCALE_MUL             (IQIDCT_CBUS_BASE+0x06)
#define DC_AC_SCALE_DIV             (IQIDCT_CBUS_BASE+0x07)
#define DC_AC_SCALE_RESULT          (IQIDCT_CBUS_BASE+0x06)
#define RESERVED_E07                (IQIDCT_CBUS_BASE+0x07)
#define POWER_CTL_IQIDCT            (IQIDCT_CBUS_BASE+0x08)
#define RV_AI_Y_X                   (IQIDCT_CBUS_BASE+0x09)
#define RV_AI_U_X                   (IQIDCT_CBUS_BASE+0x0a)
#define RV_AI_V_X                   (IQIDCT_CBUS_BASE+0x0b)
// bit 15:0 will count up when rv_ai_mb finished when non zero
// and rv_ai_mb_finished_int will be generate when this is not zero
#define RV_AI_MB_COUNT              (IQIDCT_CBUS_BASE+0x0c)

//========================================================================
//  RM DE_BLOCK Registers				    (0xe10 - 0xeff)
//========================================================================
#define DEBLK_CMD           (IQIDCT_CBUS_BASE+0x10)
#define DEBLK_STATUS        (IQIDCT_CBUS_BASE+0x11)
// bit 31 -- convas_enable
// bit 30:24 -- convas_x_count ( 8 pixels / 64 bits )
// bit 23:16 -- convas_y_count
// bit 15   -- dcac_dma_busy
// bit 14   -- dcac_dma_urgent
// bit 13:8 -- dcac_dma_count
// bit 7    -- dcac_dma_rw
// bit 6    -- dcac_skip_read_mode
// bit 5:0  -- dcac_dma_offset
#define DCAC_DMA_CTRL       (IQIDCT_CBUS_BASE+0x12)
// when (convas_enable == 0 )
//   bit 31:0 dcac_dma_addr
// when (convas_enable == 1 )
//   bit 31:25 canvas_addr (7 bits)
//   bit 24:12 convas_y_start ( 13 btis )
//   bit 11:0  convas_x_start ( 12 btis )
#define DCAC_DMA_ADDRESS    (IQIDCT_CBUS_BASE+0x13)
// bit 7:0 -- dcac_cpu_addr
#define DCAC_CPU_ADDRESS    (IQIDCT_CBUS_BASE+0x14)
// bit 31:0 -- dcac_cpu_data
#define DCAC_CPU_DATA       (IQIDCT_CBUS_BASE+0x15)

//========================================================================
//  DSP Co-Processor Registers			    (0xe00 - 0xfff)
//========================================================================
#define COPROCESSOR_CBUS_BASE 0xf00

#define AUDIO_COP_CTL1      (COPROCESSOR_CBUS_BASE+0x00)  // r & w; r-> 6'h00,bsmod[2:0],acmod[2:0],lfeon,nfchans[2:0]
#define AUDIO_COP_CTL2      (COPROCESSOR_CBUS_BASE+0x01)  // r & w; r-> frame_size[12:0],dsurmod[1:0]
#define OPERAND_M_CTL       (COPROCESSOR_CBUS_BASE+0x02)  
#define OPERAND1_ADDR       (COPROCESSOR_CBUS_BASE+0x03)
#define OPERAND2_ADDR       (COPROCESSOR_CBUS_BASE+0x04)
#define RESULT_M_CTL        (COPROCESSOR_CBUS_BASE+0x05)
#define RESULT1_ADDR        (COPROCESSOR_CBUS_BASE+0x06)
#define RESULT2_ADDR        (COPROCESSOR_CBUS_BASE+0x07)
#define ADD_SHFT_CTL        (COPROCESSOR_CBUS_BASE+0x08)
#define OPERAND_ONE_H       (COPROCESSOR_CBUS_BASE+0x09)
#define OPERAND_ONE_L       (COPROCESSOR_CBUS_BASE+0x0a)
#define OPERAND_TWO_H       (COPROCESSOR_CBUS_BASE+0x0b)
#define OPERAND_TWO_L       (COPROCESSOR_CBUS_BASE+0x0c)
#define RESULT_H            (COPROCESSOR_CBUS_BASE+0x0d)
#define RESULT_M            (COPROCESSOR_CBUS_BASE+0x0e)   // r & w; ac3_reg_14
#define RESULT_L            (COPROCESSOR_CBUS_BASE+0x0f)
#define WMEM_R_PTR          (COPROCESSOR_CBUS_BASE+0x10)   // r & w ; endmant0 &1
#define WMEM_W_PTR          (COPROCESSOR_CBUS_BASE+0x11)
#define AUDIO_LAYER         (COPROCESSOR_CBUS_BASE+0x20)
#define AC3_DECODING        (COPROCESSOR_CBUS_BASE+0x21)
#define AC3_DYNAMIC         (COPROCESSOR_CBUS_BASE+0x22)
#define AC3_MELODY          (COPROCESSOR_CBUS_BASE+0x23)
#define AC3_VOCAL           (COPROCESSOR_CBUS_BASE+0x24)

//======================================
//  CPU Assist module
//======================================
// -----------------------------------------------
// CBUS_BASE:  ASSIST_CBUS_BASE = 0x1f            
// -----------------------------------------------
#define ASSIST_CBUS_BASE    0x1f00
#define MREG_ASSIST_AMR1_INT0	    (ASSIST_CBUS_BASE+0x25)
#define MREG_ASSIST_AMR1_INT1	    (ASSIST_CBUS_BASE+0x26)
#define MREG_ASSIST_AMR1_INT2	    (ASSIST_CBUS_BASE+0x27)
#define MREG_ASSIST_AMR1_INT3	    (ASSIST_CBUS_BASE+0x28)
#define MREG_ASSIST_AMR1_INT4	    (ASSIST_CBUS_BASE+0x29)
#define MREG_ASSIST_AMR1_INT5	    (ASSIST_CBUS_BASE+0x2a)
#define MREG_ASSIST_AMR1_INT6	    (ASSIST_CBUS_BASE+0x2b)
#define MREG_ASSIST_AMR1_INT7	    (ASSIST_CBUS_BASE+0x2c)
#define MREG_ASSIST_AMR1_INT8	    (ASSIST_CBUS_BASE+0x2d)
#define MREG_ASSIST_AMR1_INT9	    (ASSIST_CBUS_BASE+0x2e)
#define MREG_ASSIST_AMR1_INTA	    (ASSIST_CBUS_BASE+0x2f)
#define MREG_ASSIST_AMR1_INTB	    (ASSIST_CBUS_BASE+0x30)
#define MREG_ASSIST_AMR1_INTC	    (ASSIST_CBUS_BASE+0x31)
#define MREG_ASSIST_AMR1_INTD	    (ASSIST_CBUS_BASE+0x32)
#define MREG_ASSIST_AMR1_INTE	    (ASSIST_CBUS_BASE+0x33)
#define MREG_ASSIST_AMR1_INTF	    (ASSIST_CBUS_BASE+0x34)
#define MREG_ASSIST_AMR2_INT0       (ASSIST_CBUS_BASE+0x35)
#define MREG_ASSIST_AMR2_INT1       (ASSIST_CBUS_BASE+0x36)
#define MREG_ASSIST_AMR2_INT2       (ASSIST_CBUS_BASE+0x37)
#define MREG_ASSIST_AMR2_INT3       (ASSIST_CBUS_BASE+0x38)
#define MREG_ASSIST_AMR2_INT4       (ASSIST_CBUS_BASE+0x39)
#define MREG_ASSIST_AMR2_INT5       (ASSIST_CBUS_BASE+0x3a)
#define MREG_ASSIST_AMR2_INT6       (ASSIST_CBUS_BASE+0x3b)
#define MREG_ASSIST_AMR2_INT7       (ASSIST_CBUS_BASE+0x3c)
#define MREG_ASSIST_AMR2_INT8       (ASSIST_CBUS_BASE+0x3d)
#define MREG_ASSIST_AMR2_INT9       (ASSIST_CBUS_BASE+0x3e)
#define MREG_ASSIST_AMR2_INTA       (ASSIST_CBUS_BASE+0x3f)
#define MREG_ASSIST_AMR2_INTB       (ASSIST_CBUS_BASE+0x40)
#define MREG_ASSIST_AMR2_INTC       (ASSIST_CBUS_BASE+0x41)
#define MREG_ASSIST_AMR2_INTD       (ASSIST_CBUS_BASE+0x42)
#define MREG_ASSIST_AMR2_INTE       (ASSIST_CBUS_BASE+0x43)
#define MREG_ASSIST_AMR2_INTF       (ASSIST_CBUS_BASE+0x44)
#define ASSIST_AMR_MBOX1_INT    (ASSIST_CBUS_BASE+0x4d)
#define ASSIST_AMR_MBOX2_INT    (ASSIST_CBUS_BASE+0x4e)
#define ASSIST_AMR_SCRATCH0     (ASSIST_CBUS_BASE+0x4f)
#define ASSIST_AMR_SCRATCH1     (ASSIST_CBUS_BASE+0x50)
#define ASSIST_AMR_SCRATCH2     (ASSIST_CBUS_BASE+0x51)
#define ASSIST_AMR_SCRATCH3     (ASSIST_CBUS_BASE+0x52)
#define ASSIST_HW_REV           (ASSIST_CBUS_BASE+0x53)
#define ASSIST_CBUS_ARB         (ASSIST_CBUS_BASE+0x54)
#define ASSIST_POR_CONFIG       (ASSIST_CBUS_BASE+0x55)
#define ASSIST_SPARE16_REG1     (ASSIST_CBUS_BASE+0x56) 
#define ASSIST_SPARE16_REG2     (ASSIST_CBUS_BASE+0x57)
#define ASSIST_SPARE8_REG1      (ASSIST_CBUS_BASE+0x58)
#define ASSIST_SPARE8_REG2      (ASSIST_CBUS_BASE+0x59)
#define ASSIST_SPARE8_REG3      (ASSIST_CBUS_BASE+0x5a)
#define AC3_CTRL_REG1           (ASSIST_CBUS_BASE+0x5b)
#define AC3_CTRL_REG2           (ASSIST_CBUS_BASE+0x5c)
#define AC3_CTRL_REG3           (ASSIST_CBUS_BASE+0x5d)
#define AC3_CTRL_REG4           (ASSIST_CBUS_BASE+0x5e)
    #define MREG_AUDIO_CTRL_REG1    AC3_CTRL_REG1
    #define MREG_AUDIO_CTRL_REG2    AC3_CTRL_REG2
    #define MREG_AUDIO_CTRL_REG3    AC3_CTRL_REG3
    #define MREG_AUDIO_CTRL_REG4    AC3_CTRL_REG4
    
#define ASSIST_PMEM_SPLIT       (ASSIST_CBUS_BASE+0x5f)
#define TIMER0_LO               (ASSIST_CBUS_BASE+0x60)
#define TIMER0_HI               (ASSIST_CBUS_BASE+0x61)
#define TIMER1_LO               (ASSIST_CBUS_BASE+0x62)
#define TIMER1_HI               (ASSIST_CBUS_BASE+0x63)
#define DMA_INT					(ASSIST_CBUS_BASE+0x64)
#define DMA_INT_MSK				(ASSIST_CBUS_BASE+0x65)
#define DMA_INT2				(ASSIST_CBUS_BASE+0x66)
#define DMA_INT_MSK2			(ASSIST_CBUS_BASE+0x67)
#define ASSIST_MBOX0_IRQ_REG    (ASSIST_CBUS_BASE+0x70)
#define ASSIST_MBOX0_CLR_REG    (ASSIST_CBUS_BASE+0x71)
#define ASSIST_MBOX0_MASK       (ASSIST_CBUS_BASE+0x72)
#define ASSIST_MBOX0_FIQ_SEL    (ASSIST_CBUS_BASE+0x73)
#define ASSIST_MBOX1_IRQ_REG    (ASSIST_CBUS_BASE+0x74)
#define ASSIST_MBOX1_CLR_REG    (ASSIST_CBUS_BASE+0x75)
#define ASSIST_MBOX1_MASK       (ASSIST_CBUS_BASE+0x76)
#define ASSIST_MBOX1_FIQ_SEL    (ASSIST_CBUS_BASE+0x77)
#define ASSIST_MBOX2_IRQ_REG    (ASSIST_CBUS_BASE+0x78)
#define ASSIST_MBOX2_CLR_REG    (ASSIST_CBUS_BASE+0x79)
#define ASSIST_MBOX2_MASK       (ASSIST_CBUS_BASE+0x7a)
#define ASSIST_MBOX2_FIQ_SEL    (ASSIST_CBUS_BASE+0x7b)

#endif /* MPEG_REG_APOLLO_H */
