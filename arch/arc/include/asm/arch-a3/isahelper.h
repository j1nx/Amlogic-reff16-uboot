/*******************************************************************
 * 
 *  Copyright C 2008 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: 
 *      ISA register helper defines
 *  Author: Amlogic Software
 *  Created: 10/17/2008 3:31:39 PM
 *
 *******************************************************************/
#ifndef ISAHELPER_H
#define ISAHELPER_H


/********************************************///
//========================================================================
//  Interrupt
//========================================================================
#define IREG_ARC2_GEN_IRQ_STATUS0     0x26a0 
#define IREG_ARC2_GEN_IRQ_CLEAR0      0x26a1
#define IREG_ARC2_GEN_IRQ_MASK0       0x26a2
#define IREG_ARC2_GEN_FIRQ_MASK0      0x26a3
#define IREG_ARC2_GEN_IRQ_STATUS1     0x26a4
#define IREG_ARC2_GEN_IRQ_CLEAR1      0x26a5
#define IREG_ARC2_GEN_IRQ_MASK1       0x26a6
#define IREG_ARC2_GEN_FIRQ_MASK1      0x26a7

/* General IRQ assignments */
    #define IRQ_USBB            (1L<<31)
    #define IRQ_USBA            (1L<<30)
    #define IRQ_TIMER_D         (1L<<29)
    #define IRQ_SDIO            (1L<<28)
    #define IRQ_DVIN            (1L<<27)
    #define IRQ_UART            (1L<<26)
    #define IRQ_FIFO2_FLUSH     (1L<<25)
    #define IRQ_FIFO2_FILL      (1L<<24)
    #define IRQ_DEMUX           (1L<<23)
    #define IRQ_ENCODER         (1L<<22)
    #define IRQ_I2C_MASTER      (1L<<21)
    #define IRQ_BT656           (1L<<20)
    #define IRQ_FIFO_FLUSH      (1L<<19)
    #define IRQ_FIFO_FILL       (1L<<18)
    #define IRQ_ABUF_RD         (1L<<17)
    #define IRQ_ABUF_WR         (1L<<16)
    #define IRQ_IR_DEC          (1L<<15)    /* NEC IR decoder */
    #define IRQ_IDE             (1L<<12)
    #define IRQ_PIT2            (1L<<11)
    #define IRQ_PIT1            (1L<<10)
    #define IRQ_I2C             (1L<<9)
    #define IRQ_ETHERNET        (1L<<8)
    #define IRQ_SPDIF           (1L<<7)
    #define IRQ_PIT3            (1L<<6)
    #define IRQ_DEMUX1          (1L<<5)
    #define IRQ_ATAPI           (1L<<4)
        #define IRQ_IRQ3    IRQ_ATAPI
    #define IRQ_VERTSYNC        (1L<<3)
    #define IRQ_HORIZSYNC       (1L<<2)
    #define IRQ_MAILBOX         (1L<<1)
    #define IRQ_WATCHDOG        (1L<<0)
    
    #define IRQ2_MMC			      (1L<<15)
    #define IRQ2_DEINTERLACER	(1L<<14)
    #define IRQ2_MAILBOX0_INTA	(1L<<13)
    #define IRQ2_MAILBOX1_INTA	(1L<<12)
    #define IRQ2_MAILBOX2_INTA	(1L<<11)
    #define IRQ2_MAILBOX0_INTB	(1L<<10)
    #define IRQ2_MAILBOX1_INTB	(1L<<9)
    #define IRQ2_MAILBOX2_INTB	(1L<<8)
    #define IRQ2_I2S_MCLK		(1L<<6)
    #define IRQ2_SMART_CARD		(1L<<5)
    #define IRQ2_NBLKMV			(1L<<4)
    #define IRQ2_SPDIF			(1L<<3)
    #define IRQ2_NAND			(1L<<2)
    #define IRQ2_VIFF_EMPTY		(1L<<1)
    #define IRQ2_PARSER			(1L<<0)

#define IREG_ARC2_GPIO_IRQ_STATUS     0x2618
#define IREG_ARC2_GPIO_IRQ_CLEAR      0x2619
#define IREG_ARC2_GPIO_IRQ_MASK       0x261a
#define IREG_ARC2_GPIO_FIRQ_MASK      0x261b

#define IREG_ARC2_AMRISC_IRQ_STATUS	  0x261c
#define IREG_ARC2_AMRISC_IRQ_CLEAR	  0x261d
#define IREG_ARC2_AMRISC_IRQ_MASK	  0x261e
#define IREG_ARC2_AMRISC_FIRQ_MASK	  0x261f

/* AMRISC IRQ assignments */
    #define IRQ_AMRISC_CCPU_INT           (1L<<23)
    #define IRQ_AMRISC_PARSER             (1L<<22)
    #define IRQ_AMRISC_VIFF_EMPTY         (1L<<21)
    #define IRQ_AMRISC_DCAC_DMA           (1L<<20)
    #define IRQ_AMRISC_VENC               (1L<<19)
    #define IRQ_AMRISC_CRC_DONE           (1L<<18)
    #define IRQ_AMRISC_CPU2_STOP          (1L<<17)
    #define IRQ_AMRISC_CPU1_STOP          (1L<<16)
    #define IRQ_AMRISC_GE2D               (1L<<15)  
    #define IRQ_AMRISC_MC_MBDONE          (1L<<14)
    #define IRQ_AMRISC_RV_AI_MB_FINISHED  (1L<<13)
    #define IRQ_AMRISC_SDIO               (1L<<12)
    #define IRQ_AMRISC_IEC958_DDR         (1L<<11)
    #define IRQ_AMRISC_VIU_HSYNC          (1L<<10)
    #define IRQ_AMRISC_MCPU               (1L<<9)
    #define IRQ_AMRISC_VL_CP              (1L<<8)
    #define IRQ_AMRISC_AI_IEC958          (1L<<7)
    #define IRQ_AMRISC_DEINTERLACER       (1L<<6)
    #define IRQ_AMRISC_HI_ABX             (1L<<5) 
    #define IRQ_AMRISC_H2CPAR             (1L<<4) 
    #define IRQ_AMRISC_H2TMR              (1L<<3) 
    #define IRQ_AMRISC_MAC1               (1L<<2) 
    #define IRQ_AMRISC_VIU_VSYNC          (1L<<1) 
    #define IRQ_AMRISC_I2S                (1L<<0) 
#define IREG_GLOBAL_INT_MASK          0x2624
#define IREG_ARC1_GEN_IRQ_STATUS0     0x2690
#define IREG_ARC1_GEN_IRQ_CLEAR0      0x2691
#define IREG_ARC1_GEN_IRQ_MASK0       0x2692
#define IREG_ARC1_GEN_FIRQ_MASK0      0x2693

#define IREG_ARC1_GEN_IRQ_STATUS1     0x2694
#define IREG_ARC1_GEN_IRQ_CLEAR1      0x2695
#define IREG_ARC1_GEN_IRQ_MASK1       0x2696
#define IREG_ARC1_GEN_FIRQ_MASK1      0x2697

#define IREG_ARC1_GPIO_IRQ_STATUS     0x2698
#define IREG_ARC1_GPIO_IRQ_CLEAR      0x2699
#define IREG_ARC1_GPIO_IRQ_MASK       0x269a
#define IREG_ARC1_GPIO_FIRQ_MASK      0x269b

#define IREG_ARC1_AMRISC_IRQ_STATUS	  0x269c
#define IREG_ARC1_AMRISC_IRQ_CLEAR	  0x269d
#define IREG_ARC1_AMRISC_IRQ_MASK	  0x269e
#define IREG_ARC1_AMRISC_FIRQ_MASK	  0x269f


// This is a new register used to control the A1H 2:1 or 1:1 clock controls
#define GLOBAL_INTR_DISABLE                        0x2624


//========================================================================
//  GPIO
//========================================================================

// "IREG_GPIO_INT_EDGE" or "IREG_GPIO_INT_POL"  are Obselete.  The 
// interrupt logic has been replaced with better logic that allows
// any of the 147 digital inputs to be connected to 1 of 8 filter
// interrupt generators:

// #define IREG_GPIO_INT_EDGE            (0x418)       
//     #define EDGE_SEL1_BIT       12
//     #define EDGE_SEL0_BIT       4
//     #define EDGE_EN1_BIT        1
//     #define EDGE_EN0_BIT        0            
// #define IREG_GPIO_INT_POL             (0x41c)

// NEW in Apollo-1H
#define GPIO_INTR_EDGE_POL                         0x2620
#define GPIO_INTR_GPIO_SEL0                        0x2621
#define GPIO_INTR_GPIO_SEL1                        0x2622
#define GPIO_INTR_FILTER_SEL0                      0x2623


//========================================================================
//  BIST
//========================================================================
// "IREG_BIST_DATA" and "IREG_BIST_CTRL" have been replaced
// with more registers.
// #define IREG_BIST_DATA                (0x440)
// #define IREG_BIST_CTRL                (0x444)
//     #define BIST_BTC_DI_BIT     30
//     #define BIST_BTC_MODE_BIT   29
//     #define BIST_BTC_CLK_BIT    28

// New in Apollo-1H
#define ISA_BIST_REG0                              0x2644
#define ISA_BIST_REG1                              0x2645
#define ISA_BIST_REG2                              0x2646
#define ISA_BIST_REG3                              0x2647
//new in A3
#define ISA_BIST_REG4                              0x2648
#define ISA_BIST_REG5                              0x2649

//========================================================================
//  Watchdog
//========================================================================
#define IREG_WATCHDOG_CONTROL0        0x2640
    #define WATCHDOG_IRQ_BIT    23
    #define WATCHDOG_ENABLE_BIT 22
#define IREG_WATCHDOG_RESET           0x2641

//========================================================================
//  AHB Arbiter
//========================================================================
#define IREG_AHB_ARBITER              0x2642
    #define AHB_ARB1_PP_BIT         25
    #define AHB_ARB0_PP_BIT         22
    #define AHB_USB0_PP_BIT         19
    #define AHB_USB1_PP_BIT         16
    #define AHB_ETH_PP_BIT          13
    #define AHB_ARC2_PP_BIT         10
    #define AHB_ARC1_PP_BIT         7
    #define AHB_CHANGE_PENDING_BIT  3
    #define AHB_CHANGE_SCHEME_BIT   2
    #define AHB_SCHEME_BIT          0

// New in Apollo-1H.  Used by Henry during ROM boot
#define AHB_ARBDEC_REG                             0x2643

//New in A3
#define AHB_ARBITER2_REG                           0x264a

//========================================================================
//  IDE DMA
//========================================================================
#define IREG_IDE_DMA_REG0             0x2160
    #define IDEDMA_XFER_SIZE_BIT    21
    #define IDEDMA_WAIT_FOR_IRQ_BIT 20
    #define IDEDMA_DMARQ_STALL_BIT  16
    #define IDEDMA_UDMA_PIO_BIT     15
    #define IDEDMA_DMA_BUSY_BIT     14
        #define IDE_DMA_STATUS_MASK       (1<<IDEDMA_DMA_BUSY_BIT)
        #define IDE_DMA_STOPPED           (0<<IDEDMA_DMA_BUSY_BIT)
        #define IDE_DMA_IN_PROGRESS       (1<<IDEDMA_DMA_BUSY_BIT)
    #define IDEDMA_AT_DMARQ_LVL_BIT 12
    #define IDEDMA_IORDY_BIT        11
    #define IDEDMA_IORDY_EN_BIT     10
    #define IDEDMA_DIS_CEL_BIT      9
    #define IDEDMA_CSEL_BIT         8
    #define IDEDMA_HOLD_TIME_BIT    4
    #define IDEDMA_IRQ14_BIT        2
    #define IDEDMA_DMARQ_FULL_BIT   1
    #define IDEDMA_ENABLE_BIT       0
        #define IDE_DMA_START             (1<<IDEDMA_ENABLE_BIT)
        
#define IREG_IDE_DMA_TABLE            0x2161
#define IREG_IDE_DMA_WORD_CNT         0x2162
#define IREG_IDE_DMA_CYCLE_TIME       0x2163
    #define IDEDMA_DMA_CYC3_BIT     24
    #define IDEDMA_DMA_CYC2_BIT     16
    #define IDEDMA_DMA_CYC1_BIT     8
    #define IDEDMA_DMA_CYC0_BIT     0
#define IREG_IDE_DMA_DIORW            0x2164
    #define IDEDMA_DMA_IO3_BIT      24
    #define IDEDMA_DMA_IO2_BIT      16
    #define IDEDMA_DMA_IO1_BIT      8
    #define IDEDMA_DMA_IO0_BIT      0
#define IREG_PIO_TIMING               0x2165
    #define IDEDMA_PIO_ADDR_SETUP_BIT   24
    #define IDEDMA_PIO_IO_ACTTIME_BIT   16
    #define IDEDMA_PIO_DIORDY_SETUP_BIT 12
    #define IDEDMA_PIO_HOLD_TIME_BIT    8
    #define IDEDMA_PIO_CYCLE_TIME_BIT   0
#define IREG_TABLE_ADD_REG            0x2166
#define IREG_IDEDMA_BURST_DELAY       0x2167
#define IREG_IDEDMA_UDMA_TIMING       0x2168
    #define IREG_ULTRA_DMA_TIMING   IREG_IDEDMA_UDMA_TIMING 
    #define IDEDMA_UDMA_TRFS_BIT        23
    #define IDEDMA_UDMA_TENV_BIT        20
    #define IDEDMA_UDMA_TACK_BIT        17
    #define IDEDMA_UDMA_TDVH_BIT        15
    #define IDEDMA_UDMA_TDVS_BIT        10
    #define IDEDMA_UDMA_TRP_BIT         4
    #define IDEDMA_UDMA_TSS_BIT         0
#define IREG_IDEDMA_UDMA_CTRL         0x2169
    #define IREG_ULTRA_DMA_CONTROL  IREG_IDEDMA_UDMA_CTRL   
    #define UDMA_INIT_CRC_BIT           9
    #define UDMA_INIT_CRC_BURST_BIT     8
    #define UDMA_CRC_ADDTION_DATA_BIT   7
    #define UDMA_PIO_DMA_PAUSE_BIT      6
    #define UDMA_CYC_TIME_BIT           0


#define IDE_DMA_THOMSON           (1<<3) /* Enable Thomson DMA Mode */	

#define IDE_PIO_MODE              (2<<4)

#define IDE_DMA_CSEL              (1<<8)
#define IDE_DMA_CSEL_CTRL         (1<<9)
	
#define IDE_DMA_COMPLETED         (0<<14)
#define IDE_DMA_ABORTED           (0<<14)
  

//========================================================================
// 
//========================================================================
#define TRANS_PWMA_REG0                            0x2170
#define TRANS_PWMA_REG1                            0x2171
#define TRANS_PWMA_MUX0                            0x2172
#define TRANS_PWMA_MUX1                            0x2173
#define TRANS_PWMA_MUX2                            0x2174
#define TRANS_PWMA_MUX3                            0x2175
#define TRANS_PWMA_MUX4                            0x2176
#define TRANS_PWMA_MUX5                            0x2177
#define TRANS_PWMB_REG0                            0x2178
#define TRANS_PWMB_REG1                            0x2179
#define TRANS_PWMB_MUX0                            0x217a
#define TRANS_PWMB_MUX1                            0x217b
#define TRANS_PWMB_MUX2                            0x217c
#define TRANS_PWMB_MUX3                            0x217d
#define TRANS_PWMB_MUX4                            0x217e
#define TRANS_PWMB_MUX5                            0x217f

//========================================================================
//  Timer
//========================================================================
#define IREG_TIMER_BASE               0x2650
    #define TIMER_E_INPUT_BIT         8
    #define TIMER_D_INPUT_BIT         6
    #define TIMER_C_INPUT_BIT         4
    #define TIMER_B_INPUT_BIT         2
    #define TIMER_A_INPUT_BIT         0
    #define TIMER_E_INPUT_MASK       (3L << TIMER_E_INPUT_BIT)
    #define TIMER_D_INPUT_MASK       (3L << TIMER_D_INPUT_BIT)
    #define TIMER_C_INPUT_MASK       (3L << TIMER_C_INPUT_BIT)
    #define TIMER_B_INPUT_MASK       (3L << TIMER_B_INPUT_BIT)
    #define TIMER_A_INPUT_MASK       (3L << TIMER_A_INPUT_BIT)
    #define TIMER_UNIT_1us            0
    #define TIMER_UNIT_10us           1
    #define TIMER_UNIT_100us          2
    #define TIMER_UNIT_1ms            3
#define IREG_TIMER_A_COUNT            0x2651
#define IREG_TIMER_B_COUNT            0x2652
#define IREG_TIMER_C_COUNT            0x2653
#define IREG_TIMER_D_COUNT            0x2654
#define IREG_TIMER_E_COUNT            0x2655

//========================================================================
//  Frame buffer
//========================================================================
#define IREG_FRAME_DESC               0x2656
    #define FBUF_BLINK_WR_BIT         26

//========================================================================
//  DDR controls
//========================================================================
#define IREG_DDR_CTRL                 0x2657
	#define CBUS_SLOW_MODE_EN_BIT			  4
    #define DDR_CTL_SYS_URGENT_IRQ_EN_BIT     3
    #define DDR_CTL_SYS_URGENT_FIQ_EN_BIT     2
    #define DDR_CTL_SYS_BVCI_FORCE_URGENT_BIT 1
    #define DDR_CTL_MAPDDR                    0

//========================================================================
//  ARC2 controls
//========================================================================
#define IREG_ARC2_CTRL								0x2659
		// 15:4   aud_arc_remap. aud_arc the lowest 1Mbyte address  remapd to remap adress  
		// 3   when aud_arc irq interrupt, let the aud_arc ddr request to be urgent request.
		// 2   when aud_arc fiq intertupt let the aud_arc ddr requestto be urgent rquest.
		// 1   force aud_arc ddr request always urgent. 
		// 0   enable aud_arc. You have to write this bit to 1 then to 0.  wirte 0 not disable aud_arc. once aud_arc is running it cann't disabled. you have to reset aud_arc to disable it. or use software like brk sleep instruction.  
//========================================================================
//  AHB Bridge controls
//========================================================================
#define AHB_BRIDGE_CTRL0              0x2680
    #define AHB_BRIDGE_URGENT_EN_BIT      21
    #define AHB_BRIDGE_SOFT_RESET_BIT     20
    #define AHB_BRIDGE_REQ_HOLDOFF_BIT    16
    #define AHB_BRIDGE_ARC1_RD_CACHE_BIT  14
    #define AHB_BRIDGE_ARC2_RD_CACHE_BIT  12
    #define AHB_BRIDGE_ETH_RD_CACHE_BIT   10
    #define AHB_BRIDGE_USB1_RD_CACHE_BIT  8
    #define AHB_BRIDGE_USB0_RD_CACHE_BIT  6
    #define AHB_BRIDGE_ARB0_RD_CACHE_BIT  4
    #define AHB_BRIDGE_ARB1_RD_CACHE_BIT  2
#define AHB_REMAP_ADDR01              0x2681
    #define REMAP_ADDR1_ENDIAN_EN_BIT     31
    #define REMAP_ADDR1_ENDIAN_BIT        16
    #define REMAP_ADDR0_ENDIAN_EN_BIT     15
    #define REMAP_ADDR0_ENDIAN_BIT        0
#define AHB_REMAP_ADDR23              0x2682
#define AHB_REMAP_ADDR45              0x2683
#define AHB_REMAP_ADDR67              0x2684
#define AHB_BRIDGE_CTRL1              0x2685
    #define AHB_BRIDGE_INVALIDATE_BIT     31
    #define AHB_BRIDGE_URGENT_THRESH_BIT  8
    #define AHB_BRIDGE_NOWAIT_ARC1_BIT    7
    #define AHB_BRIDGE_NOWAIT_ARC2_BIT    6
    #define AHB_BRIDGE_NOWAIT_ETH_BIT     5
    #define AHB_BRIDGE_NOWAIT_USB1_BIT    4
    #define AHB_BRIDGE_NOWAIT_USB0_BIT    3
    #define AHB_BRIDGE_NOWAIT_ARB0_BIT    2
    #define AHB_BRIDGE_NOWAIT_ARB1_BIT    1

// same as niked3
#define ABUF_WR_CTRL0	                0x2670
#define ABUF_WR_CTRL1	                0x2671
#define ABUF_WR_CTRL2	                0x2672
#define ABUF_WR_CTRL3	                0x2673
#define ABUF_RD_CTRL0	                0x2674
#define ABUF_RD_CTRL1	                0x2675
#define ABUF_RD_CTRL2	                0x2676
#define ABUF_RD_CTRL3	                0x2677
#define ABUF_ARB_CTRL0	              	0x2678
#define ABUF_FIFO_CTRL0	              0x2679

//New in A3
// -----------------------------------------------------------
#define SYS_CPU_0_IRQ_IN0_INTR_STAT                0x2690
#define SYS_CPU_0_IRQ_IN0_INTR_STAT_CLR            0x2691
#define SYS_CPU_0_IRQ_IN0_INTR_MASK                0x2692
#define SYS_CPU_0_IRQ_IN0_INTR_FIRQ_SEL            0x2693
#define SYS_CPU_0_IRQ_IN1_INTR_STAT                0x2694
#define SYS_CPU_0_IRQ_IN1_INTR_STAT_CLR            0x2695
#define SYS_CPU_0_IRQ_IN1_INTR_MASK                0x2696
#define SYS_CPU_0_IRQ_IN1_INTR_FIRQ_SEL            0x2697
#define SYS_CPU_0_IRQ_IN2_INTR_STAT                0x2698
#define SYS_CPU_0_IRQ_IN2_INTR_STAT_CLR            0x2699
#define SYS_CPU_0_IRQ_IN2_INTR_MASK                0x269a
#define SYS_CPU_0_IRQ_IN2_INTR_FIRQ_SEL            0x269b
#define SYS_CPU_0_IRQ_IN3_INTR_STAT                0x269c
#define SYS_CPU_0_IRQ_IN3_INTR_STAT_CLR            0x269d
#define SYS_CPU_0_IRQ_IN3_INTR_MASK                0x269e
#define SYS_CPU_0_IRQ_IN3_INTR_FIRQ_SEL            0x269f
// -----------------------------------------------------------
#define SYS_CPU_1_IRQ_IN0_INTR_STAT                0x26a0
#define SYS_CPU_1_IRQ_IN0_INTR_STAT_CLR            0x26a1
#define SYS_CPU_1_IRQ_IN0_INTR_MASK                0x26a2
#define SYS_CPU_1_IRQ_IN0_INTR_FIRQ_SEL            0x26a3
#define SYS_CPU_1_IRQ_IN1_INTR_STAT                0x26a4
#define SYS_CPU_1_IRQ_IN1_INTR_STAT_CLR            0x26a5
#define SYS_CPU_1_IRQ_IN1_INTR_MASK                0x26a6
#define SYS_CPU_1_IRQ_IN1_INTR_FIRQ_SEL            0x26a7
#define SYS_CPU_1_IRQ_IN2_INTR_STAT                0x26a8
#define SYS_CPU_1_IRQ_IN2_INTR_STAT_CLR            0x26a9
#define SYS_CPU_1_IRQ_IN2_INTR_MASK                0x26aa
#define SYS_CPU_1_IRQ_IN2_INTR_FIRQ_SEL            0x26ab
#define SYS_CPU_1_IRQ_IN3_INTR_STAT                0x26ac
#define SYS_CPU_1_IRQ_IN3_INTR_STAT_CLR            0x26ad
#define SYS_CPU_1_IRQ_IN3_INTR_MASK                0x26ae
#define SYS_CPU_1_IRQ_IN3_INTR_FIRQ_SEL            0x26af
/********************************************///






#define ISABASE                      0xc1100000

#define WRITE_ISA_REG(reg, val) *(volatile unsigned *)(ISABASE + ((reg)<<2)) = (val)
#define READ_ISA_REG(reg) (*(volatile unsigned *)(ISABASE + ((reg)<<2)))

#define CLEAR_ISA_REG_MASK(reg, mask) WRITE_ISA_REG(reg, (READ_ISA_REG(reg)&(~mask)))
#define SET_ISA_REG_MASK(reg, mask)   WRITE_ISA_REG(reg, (READ_ISA_REG(reg)|(mask)))

/* DMA Routines **********************************************/

#define ide_dma_started()                                               \
    ((READ_ISA_REG(IREG_IDE_DMA_REG0)&IDE_DMA_START) == IDE_DMA_START)

#define ide_dma_is_busy()                                               \
    ((READ_ISA_REG(IREG_IDE_DMA_REG0)&IDE_DMA_STATUS_MASK) == IDE_DMA_IN_PROGRESS)

#define set_ide_dma_address(addr)                       \
    WRITE_ISA_REG(IREG_IDE_DMA_TABLE, (unsigned) addr)

#define read_next_ide_dma_address()                       \
    READ_ISA_REG(IREG_IDE_DMA_TABLE)

#define add_ide_dma_table(num)                       \
    WRITE_ISA_REG(IREG_TABLE_ADD_REG,num)

#define read_ide_dma_table_remain()                       \
    READ_ISA_REG(IREG_TABLE_ADD_REG)

#define halt_ide_dma() \
    CLEAR_ISA_REG_MASK(IREG_IDE_DMA_REG0, IDE_DMA_START)

#define start_ide_dma()                                 \
    SET_ISA_REG_MASK(IREG_IDE_DMA_REG0, IDE_DMA_START)

/* Interrupt routines ******************************************/
#define set_irq_mask(mask)                      \
    SET_ISA_REG_MASK(IREG_ARC1_GEN_IRQ_MASK0, mask)
#define set_irq_mask1(mask)                      \
    SET_ISA_REG_MASK(IREG_ARC1_GEN_IRQ_MASK1, mask)

#define set_arc2_irq_mask(mask)                      \
    SET_ISA_REG_MASK(IREG_ARC2_GEN_IRQ_MASK0, mask)
#define set_arc2_irq_mask1(mask)                      \
    SET_ISA_REG_MASK(IREG_ARC2_GEN_IRQ_MASK1, mask)
    
#define get_irq_mask()                          \
    READ_ISA_REG(IREG_ARC1_GEN_IRQ_MASK0)
#define get_irq_mask1()                          \
    READ_ISA_REG(IREG_ARC1_GEN_IRQ_MASK1)

#define get_arc2_irq_mask()                          \
    READ_ISA_REG(IREG_ARC2_GEN_IRQ_MASK0)
#define get_arc2_irq_mask1()                          \
    READ_ISA_REG(IREG_ARC2_GEN_IRQ_MASK1)
    
#define clear_irq_mask(mask)                    \
    CLEAR_ISA_REG_MASK(IREG_ARC1_GEN_IRQ_MASK0, mask)
#define clear_irq_mask1(mask)                    \
    CLEAR_ISA_REG_MASK(IREG_ARC1_GEN_IRQ_MASK1, mask)

#define clear_arc2_irq_mask(mask)                    \
    CLEAR_ISA_REG_MASK(IREG_ARC2_GEN_IRQ_MASK0, mask)
#define clear_arc2_irq_mask1(mask)                    \
    CLEAR_ISA_REG_MASK(IREG_ARC2_GEN_IRQ_MASK1, mask)
    
#define set_firq_mask(mask)                     \
    SET_ISA_REG_MASK(IREG_ARC1_GEN_FIRQ_MASK0, mask)
#define set_firq_mask1(mask)                     \
    SET_ISA_REG_MASK(IREG_ARC1_GEN_FIRQ_MASK1, mask)

#define set_arc2_firq_mask(mask)                     \
    SET_ISA_REG_MASK(IREG_ARC2_GEN_FIRQ_MASK0, mask)
#define set_arc2_firq_mask1(mask)                     \
    SET_ISA_REG_MASK(IREG_ARC2_GEN_FIRQ_MASK1, mask)
    
#define get_firq_mask()                     \
    READ_ISA_REG(IREG_ARC1_GEN_FIRQ_MASK0)
#define get_firq_mask1()                     \
    READ_ISA_REG(IREG_ARC1_GEN_FIRQ_MASK1)

#define get_arc2_firq_mask()                     \
    READ_ISA_REG(IREG_ARC2_GEN_FIRQ_MASK0)
#define get_arc2_firq_mask1()                     \
    READ_ISA_REG(IREG_ARC2_GEN_FIRQ_MASK1)
    
#define clear_firq_mask(mask)                       \
    CLEAR_ISA_REG_MASK(IREG_ARC1_GEN_FIRQ_MASK0, mask)
#define clear_firq_mask1(mask)                       \
    CLEAR_ISA_REG_MASK(IREG_ARC1_GEN_FIRQ_MASK1, mask)

#define clear_arc2_firq_mask(mask)                       \
    CLEAR_ISA_REG_MASK(IREG_ARC2_GEN_FIRQ_MASK0, mask)
#define clear_arc2_firq_mask1(mask)                       \
    CLEAR_ISA_REG_MASK(IREG_ARC2_GEN_FIRQ_MASK1, mask)
    
#define read_irq_status()                       \
    READ_ISA_REG(IREG_ARC1_GEN_IRQ_STATUS0)
#define read_irq_status1()                       \
    READ_ISA_REG(IREG_ARC1_GEN_IRQ_STATUS1)

#define read_arc2_irq_status()                       \
    READ_ISA_REG(IREG_ARC2_GEN_IRQ_STATUS0)
#define read_arc2_irq_status1()                       \
    READ_ISA_REG(IREG_ARC2_GEN_IRQ_STATUS1)
    
#define clear_irq_status(mask)                  \
    WRITE_ISA_REG(IREG_ARC1_GEN_IRQ_CLEAR0, mask)
#define clear_irq_status1(mask)                  \
    WRITE_ISA_REG(IREG_ARC1_GEN_IRQ_CLEAR1, mask)

#define clear_arc2_irq_status(mask)                  \
    WRITE_ISA_REG(IREG_ARC2_GEN_IRQ_CLEAR0, mask)
#define clear_arc2_irq_status1(mask)                  \
    WRITE_ISA_REG(IREG_ARC2_GEN_IRQ_CLEAR1, mask)
    
/* AMRISC Interrupt routines ***********************************/
#define set_amrisc_irq_mask(mask)                   \
    SET_ISA_REG_MASK(IREG_ARC1_AMRISC_IRQ_MASK, mask)
#define get_amrisc_irq_mask()                   \
    READ_ISA_REG(IREG_ARC1_AMRISC_IRQ_MASK)
#define clear_amrisc_irq_mask(mask)                 \
    CLEAR_ISA_REG_MASK(IREG_ARC1_AMRISC_IRQ_MASK, mask)
#define set_amrisc_firq_mask(mask)                  \
    SET_ISA_REG_MASK(IREG_ARC1_AMRISC_FIRQ_MASK, mask)
#define get_amrisc_firq_mask()                  \
    READ_ISA_REG(IREG_ARC1_AMRISC_FIRQ_MASK)
#define clear_amrisc_firq_mask(mask)                    \
    CLEAR_ISA_REG_MASK(IREG_ARC1_AMRISC_FIRQ_MASK, mask)
#define read_amrisc_irq_status()                \
    READ_ISA_REG(IREG_ARC1_AMRISC_IRQ_STATUS)
#define clear_amrisc_irq_status(mask)           \
    WRITE_ISA_REG(IREG_ARC1_AMRISC_IRQ_CLEAR, mask)

#define set_arc2_amrisc_irq_mask(mask)                   \
    SET_ISA_REG_MASK(IREG_ARC2_AMRISC_IRQ_MASK, mask)
#define get_arc2_amrisc_irq_mask()                   \
    READ_ISA_REG(IREG_ARC2_AMRISC_IRQ_MASK)
#define clear_arc2_amrisc_irq_mask(mask)                 \
    CLEAR_ISA_REG_MASK(IREG_ARC2_AMRISC_IRQ_MASK, mask)
#define set_arc2_amrisc_firq_mask(mask)                  \
    SET_ISA_REG_MASK(IREG_ARC2_AMRISC_FIRQ_MASK, mask)
#define get_arc2_amrisc_firq_mask()                  \
    READ_ISA_REG(IREG_ARC2_AMRISC_FIRQ_MASK)
#define clear_arc2_amrisc_firq_mask(mask)                    \
    CLEAR_ISA_REG_MASK(IREG_ARC2_AMRISC_FIRQ_MASK, mask)
#define read_arc2_amrisc_irq_status()                \
    READ_ISA_REG(IREG_ARC2_AMRISC_IRQ_STATUS)
#define clear_arc2_amrisc_irq_status(mask)           \
    WRITE_ISA_REG(IREG_ARC2_AMRISC_IRQ_CLEAR, mask)
    
/* GPIO Interrupt routines ****************************************/
#define set_gpio_irq_mask(mask)                 \
    SET_ISA_REG_MASK(IREG_ARC1_GPIO_IRQ_MASK, mask)
#define get_gpio_irq_mask()                     \
    READ_ISA_REG(IREG_ARC1_GPIO_IRQ_MASK)
#define clear_gpio_irq_mask(mask)                   \
    CLEAR_ISA_REG_MASK(IREG_ARC1_GPIO_IRQ_MASK, mask)
#define set_gpio_firq_mask(mask)                \
    SET_ISA_REG_MASK(IREG_ARC1_GPIO_FIRQ_MASK, mask)
#define get_gpio_firq_mask()                    \
    READ_ISA_REG(IREG_ARC1_GPIO_FIRQ_MASK)
#define clear_gpio_firq_mask(mask)                  \
    CLEAR_ISA_REG_MASK(IREG_ARC1_GPIO_FIRQ_MASK, mask)
#define read_gpio_irq_status()                  \
    READ_ISA_REG(IREG_ARC1_GPIO_IRQ_STATUS)
#define clear_gpio_irq_status(mask)             \
    WRITE_ISA_REG(IREG_ARC1_GPIO_IRQ_CLEAR, mask)

#define set_arc2_gpio_irq_mask(mask)                 \
    SET_ISA_REG_MASK(IREG_ARC2_GPIO_IRQ_MASK, mask)
#define get_arc2_gpio_irq_mask()                     \
    READ_ISA_REG(IREG_ARC2_GPIO_IRQ_MASK)
#define clear_arc2_gpio_irq_mask(mask)                   \
    CLEAR_ISA_REG_MASK(IREG_ARC2_GPIO_IRQ_MASK, mask)
#define set_arc2_gpio_firq_mask(mask)                \
    SET_ISA_REG_MASK(IREG_ARC2_GPIO_FIRQ_MASK, mask)
#define get_arc2_gpio_firq_mask()                    \
    READ_ISA_REG(IREG_ARC2_GPIO_FIRQ_MASK)
#define clear_arc2_gpio_firq_mask(mask)                  \
    CLEAR_ISA_REG_MASK(IREG_ARC2_GPIO_FIRQ_MASK, mask)
#define read_arc2_gpio_irq_status()                  \
    READ_ISA_REG(IREG_ARC2_GPIO_IRQ_STATUS)
#define clear_arc2_gpio_irq_status(mask)             \
    WRITE_ISA_REG(IREG_ARC2_GPIO_IRQ_CLEAR, mask)
    
/* DMA Routines **********************************************/

#define ide_dma_started()                                               \
    ((READ_ISA_REG(IREG_IDE_DMA_REG0)&IDE_DMA_START) == IDE_DMA_START)

#define ide_dma_is_busy()                                               \
    ((READ_ISA_REG(IREG_IDE_DMA_REG0)&IDE_DMA_STATUS_MASK) == IDE_DMA_IN_PROGRESS)

#define set_ide_dma_address(addr)                       \
    WRITE_ISA_REG(IREG_IDE_DMA_TABLE, (unsigned) addr)

#define read_next_ide_dma_address()                       \
    READ_ISA_REG(IREG_IDE_DMA_TABLE)

#define add_ide_dma_table(num)                       \
    WRITE_ISA_REG(IREG_TABLE_ADD_REG,num)

#define read_ide_dma_table_remain()                       \
    READ_ISA_REG(IREG_TABLE_ADD_REG)

#define halt_ide_dma() \
    CLEAR_ISA_REG_MASK(IREG_IDE_DMA_REG0, IDE_DMA_START)

#define start_ide_dma()                                 \
    SET_ISA_REG_MASK(IREG_IDE_DMA_REG0, IDE_DMA_START)
    
#define enable_viu_with_dcu2()                                 \
    SET_ISA_REG_MASK(IREG_SDRAM_CTRL, SC0_FLAG_VIU_EN)
#define disable_viu_with_dcu2()                                    \
    CLEAR_ISA_REG_MASK(IREG_SDRAM_CTRL, SC0_FLAG_VIU_EN)

#define map_eeprom_to_sdram()                           \
    SET_ISA_REG_MASK(IREG_SDRAM_CTRL, SC0_FLAG_SDRAM)
#define map_eeprom_not_to_sdram()                           \
    CLEAR_ISA_REG_MASK(IREG_SDRAM_CTRL, SC0_FLAG_SDRAM)
    

/* RTC routines ***************************************************/
#define PRECISE_CUR_TIME   READ_ISA_REG(IREG_TIMER_C_COUNT)
#define set_timer_base(n) WRITE_ISA_REG(IREG_TIMER_BASE, n) 
#define read_timer_base() READ_ISA_REG(IREG_TIMER_BASE)

#endif /* ISAHELPER_H */

