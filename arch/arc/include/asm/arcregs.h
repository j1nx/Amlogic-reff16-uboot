/******************************************************************************
 * Copyright Codito Technologies (www.codito.com) Oct 01, 2004
 * 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *****************************************************************************/

/*
 *  linux/include/asm-arc/arcregs.h
 *
 *  Copyright (C) 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Authors: Amit Bhor ,Ashwin Chaugule
 * Auxiliary register definitions and macros to read and write to them. 
 * Added BCR's reg and masks
 */

#ifndef _ASM_ARC_ARCDEFS_H
#define _ASM_ARC_ARCDEFS_H

/* These are extension BCR's*/
#define ARC_REG_CRC_BCR      0x62
#define ARC_REG_DVFB_BCR     0x64
#define ARC_REG_EXTARITH_BCR 0x65
#define ARC_REG_VECBASE_BCR  0x68
#define ARC_REG_PERIBASE_BCR 0x69
#define ARC_REG_MMU_BCR      0x6f
#define ARC_REG_DCCM_BCR     0x74
#define ARC_REG_TIMERS_BCR   0x75
#define ARC_REG_ICCM_BCR     0x78
#define ARC_REG_XY_MEM_BCR   0x79
#define ARC_REG_MAC_BCR      0x7a
#define ARC_REG_MUL_BCR      0x7b
#define ARC_REG_SWAP_BCR     0x7c
#define ARC_REG_NORM_BCR     0x7d
#define ARC_REG_MIXMAX_BCR   0x7e
#define ARC_REG_BARREL_BCR   0x7f

#define	STATUS_H_SET              0x01		/* Mask for Halt bit */
#define STATUS_DISABLE_INTERRUPTS 0xFFFFFFF9	/* Mask to disable Interrupts  */


/* status32 Bits Positions */
#define STATUS_H_BIT    0
#define STATUS_E1_BIT   1
#define STATUS_E2_BIT   2
#define STATUS_A1_BIT   3
#define STATUS_A2_BIT   4
#define STATUS_AE_BIT   5
#define STATUS_DE_BIT   6
#define STATUS_U_BIT    7
#define STATUS_L_BIT    12

/* These masks correspond to the status word(STATUS_32) bits */
#define STATUS_H_MASK   0x01    /* Mask for Halt bit */
#define STATUS_E1_MASK  0x02    /* Mask for Inerrupt 1 enable */
#define STATUS_E2_MASK  0x04    /* Mask for Inerrupt 2 enable */
#define STATUS_A1_MASK  0x08    /* Interrupt 1 active */
#define STATUS_A2_MASK  0x10    /* Interrupt 2 active */
#define STATUS_AE_MASK  0x20    /* Exception active */
#define STATUS_DE_MASK  0x40    /* PC is in delay slot */
#define STATUS_U_MASK   0x80    /* User/Kernel mode bit */
#define STATUS_L_MASK   0x1000  /* Loop inhibit bit */


#define ARC_REG_STATUS32          0x0A /* status 32 register */ 		
#define ARC_REG_PC                0x6  /* program counter register */

#define ARC_REG_TIMER0_LIMIT      0x23 /* timer 0 limit */
#define ARC_REG_TIMER0_CTRL       0x22 /* timer 0 control */
#define ARC_REG_TIMER0_CNT        0x21 /* timer 0 count */

/* These masks correspond to the exception cause register (ecause) bits */

#define ECAUSE_VECTOR_MASK     0xff0000
#define ECAUSE_CODE_MASK       0x00ff00
#define ECAUSE_PARAMETER_MASK  0x0000ff


/* Auxiliary registers not supported by the assembler */
#define AUX_IDENTITY        4
#define AUX_INTR_VEC_BASE   0x25
#define AUX_IRQ_LEV         0x200   /* interrupt priority level setting */
#define AUX_IRQ_HINT        0x201   /* Aux register for generating software interrupts */
#define AUX_IRQ_LV12        0x43    /* interrupt level register */

#define ARC_REG_ECR           0x403
#define AUX_ICAUSE1           0x40a
#define AUX_ICAUSE2           0x40b

#define AUX_IENABLE         0x40c   /* Known to the assembler as auxienable, so don't really need this */
#define AUX_ITRIGGER        0x40d
#define AUX_IRQ_PULSE_CANCEL  0x415
/* Privileged MMU auxiliary register definitions */
#define ARC_REG_TLBPD0      0x405
#define ARC_REG_TLBPD1      0x406
#define ARC_REG_TLBINDEX    0x407   
#define ARC_REG_TLBCOMMAND  0x408
#define ARC_REG_PID         0x409
#define ARC_REG_SCRATCH_DATA0   0x418

/* Instruction cache related Auxiliary registers */
#define ARC_REG_I_CACHE_BUILD_REG 0x77
#define ARC_REG_IC_IVIC     0x10
#define ARC_REG_IC_CTRL     0x11
#define ARC_REG_IC_IVIL     0x19
/* Data cache related Auxiliary registers */
#define ARC_REG_D_CACHE_BUILD_REG 0x72
#define ARC_REG_DC_IVDC     0x47
#define ARC_REG_DC_CTRL     0x48
#define ARC_REG_DC_IVDL     0x4A
#define ARC_REG_DC_FLSH     0x4B
#define ARC_REG_DC_FLDL     0x4C
#define IM_SET_DC_CTRL 		0x40

/* Timer related Aux registers */
#define ARC_REG_TIMER0_LIMIT 0x23 /* timer 0 limit */
#define ARC_REG_TIMER0_CTRL  0x22 /* timer 0 control */
#define ARC_REG_TIMER0_CNT   0x21 /* timer 0 count */
#define ARC_REG_TIMER1_LIMIT 0x102 /* timer 1 limit */
#define ARC_REG_TIMER1_CTRL  0x101 /* timer 1 control */
#define ARC_REG_TIMER1_CNT   0x100 /* timer 1 count */

#ifdef CONFIG_ARC700_SSRAM
/* Synchronous SRAM controller registers */
#define SSRAM_CTRL_BASE 0xc0fc7000  /* Base of the controller registers */
#define SSRAM_CTRL_ID       SSRAM_CTRL_BASE + 0x0
#define SSRAM_CTRL_B0CNFR1  SSRAM_CTRL_BASE + 0x2
#define SSRAM_CTRL_B0CNFR2  SSRAM_CTRL_BASE + 0x3
#define SSRAM_CTRL_B1CNFR1  SSRAM_CTRL_BASE + 0x6
#define SSRAM_CTRL_B1CNFR2  SSRAM_CTRL_BASE + 0x7
#define SSRAM_CTRL_B2CNFR1  SSRAM_CTRL_BASE + 0xa
#define SSRAM_CTRL_B2CNFR2  SSRAM_CTRL_BASE + 0xb
#endif /* CONFIG_ARC700_SSRAM */

#define TLBPD1_MASK 0xffffe1fc /* Mask of bits to be written to the TLBPD1 */

#ifdef CONFIG_ARCH_ARC800

#define ARC_AUX_IDU_REG_CMD     0x2000
#define ARC_AUX_IDU_REG_PARAM   0x2001

#define ARC_AUX_XTL_REG_CMD     0x2002
#define ARC_AUX_XTL_REG_PARAM   0x2003

#define ARC_REG_MP_BCR          0x2021

#define ARC_XTL_REG_SYNTAX_PARAM_PC     1   /* Left shift by 1 */
#define ARC_XTL_REG_SYNTAX_CMD_CPU_ID   8   /* Left shift by 8 */

#define ARC_XTL_CMD_WRITE_PC    0x04
#define ARC_XTL_CMD_CLEAR_HALT  0x02


#endif

// Profiling AUX regs.


#define ARC_PCT_CONTROL         0x255
#define ARC_HWP_CTRL            0xc0fcb018 //Periphbase + b000 + 18
#define PR_CTRL_EN              (1<<0)
#define ARC_PR_ID                   0xc0fcb000

/* Instruction cache Control Auxiliary register Mask */
#define ARC_REG_IC_CTRL_DC   (0x01)
#define ARC_REG_IC_CTRL_SB   (0x08) /* Last cache operation succeeded */ 
#define ARC_REG_IC_CTRL_FS   (0x10) /* Flush icache */
/* Data cache Control Auxiliary register Mask */
#define ARC_REG_DC_CTRL_DIS  (1<<0) /* Data cache control disable */
#define ARC_REG_DC_CTRL_IM   (1<<6) /* Flush dirty dcache line */
#define ARC_REG_DC_CTRL_LM   (1<<7) /* Flush locked dcache */
#define ARC_REG_DC_CTRL_FS   (1<<8)
#define ARC_REG_DC_CTRL_SB   (1<<2) /* Success of last cache operation */
#define CACHE_LINE_LENGTH    (0x40)
#define CACHE_LINE_MASK      (0xffffffc0) 


/* Inline macros for reading, writing into auxiliary registers	*/

#ifndef __ASSEMBLY__

#define read_new_aux_reg(reg)                                   \
({ unsigned int __ret;                                          \
    __asm__ __volatile__("lr    %0, [%1]":"=r"(__ret):"i"(reg));\
    __ret;                                                      \
})
        
/* Write to an auxiliary register */

#define write_new_aux_reg(reg, val)                             \
({                                                              \
    __asm__ __volatile__ ("sr   %0, [%1]"::"r"(val),"i"(reg));  \
})

/**************************************************************** 
 * Register Layouts using bitfields so that we dont have to write
 * bit fiddling ourselves; the compiler can do that for us
 */
struct cpuinfo_arc_processor {
    unsigned int
        family: 8,
        cpu_id: 8,
        chip_id:16;
};

struct cpuinfo_arc_mmu {
    unsigned int   
        num_dTLB        :8,
        num_iTLB        :8,
        entries_per_way :4,
        num_ways        :4,
        ver             :8;
};

#define EXTN_SWAP_VALID     0x1
#define EXTN_NORM_VALID     0x2
#define EXTN_MINMAX_VALID   0x2
#define EXTN_BARREL_VALID   0x2

struct cpuinfo_arc_extn {
    unsigned int

        /* Prog Ref Manual */
        swap:1, norm:2, minmax:2, barrel:2, mul:2, ext_arith:2,

        mac_mul:8,          /* DSP Options Ref Manual */
        crc:1,              /* DSP-LIB Ref Manual */
        dccm:1, iccm:1,
        dvfb:1,             /* Dual Viterbi Butterfly Instrn: 
                               Exotic but not supported by 700
                             */
        padding:9;
};

struct cpuinfo_arc_extn_xymem {
    unsigned int   ver:8,
                    bank_sz:4, num_banks:4,
                    ram_org:2;
};

struct bcr_cache {
    unsigned long ver:8, type:4, sz:4, line_len:4, pad:12;
};


#ifdef CONFIG_ARCH_ARC800
struct cpuinfo_arc800 {
    unsigned int ver:8, scu:1, idu:1, sdu:1, padding:5, mp_arch:16;
};
#endif
 

#endif  /* __ASEMBLY__ */
/* Build Registers */
#define ARC_REG_IDENTITY        4
#define ARC_REG_DEBUG           5
#define ARC_REG_AP_BUILD        0x76
/* Action Point Registers */
#define ARC_REG_AP_AMV(i)   (0x220+i*3)
#define ARC_REG_AP_AMM(i)   (0x221+i*3)
#define ARC_REG_AP_AC(i)    (0x222+i*3)

#define ARC_REG_AP_AMV0     ARC_REG_AP_AMV(0)
#define ARC_REG_AP_AMV1     ARC_REG_AP_AMV(1)
#define ARC_REG_AP_AMV2     ARC_REG_AP_AMV(2)
#define ARC_REG_AP_AMV3     ARC_REG_AP_AMV(3)
#define ARC_REG_AP_AMV4     ARC_REG_AP_AMV(4)
#define ARC_REG_AP_AMV5     ARC_REG_AP_AMV(5)
#define ARC_REG_AP_AMV6     ARC_REG_AP_AMV(6)
#define ARC_REG_AP_AMV7     ARC_REG_AP_AMV(7)

#define ARC_REG_AP_AMM0     ARC_REG_AP_AMM(0)
#define ARC_REG_AP_AMM1     ARC_REG_AP_AMM(1)
#define ARC_REG_AP_AMM2     ARC_REG_AP_AMM(2)
#define ARC_REG_AP_AMM3     ARC_REG_AP_AMM(3)
#define ARC_REG_AP_AMM4     ARC_REG_AP_AMM(4)
#define ARC_REG_AP_AMM5     ARC_REG_AP_AMM(5)
#define ARC_REG_AP_AMM6     ARC_REG_AP_AMM(6)
#define ARC_REG_AP_AMM7     ARC_REG_AP_AMM(7)

#define ARC_REG_AP_AC0     ARC_REG_AP_AC(0)
#define ARC_REG_AP_AC1     ARC_REG_AP_AC(1)
#define ARC_REG_AP_AC2     ARC_REG_AP_AC(2)
#define ARC_REG_AP_AC3     ARC_REG_AP_AC(3)
#define ARC_REG_AP_AC4     ARC_REG_AP_AC(4)
#define ARC_REG_AP_AC5     ARC_REG_AP_AC(5)
#define ARC_REG_AP_AC6     ARC_REG_AP_AC(6)
#define ARC_REG_AP_AC7     ARC_REG_AP_AC(7)

/** AP_BUILD Register 0x5 set details**/
#define ARC_REG_ABTM    (0xf<<8)
#define ARC_REG_ABVM    (0xff)

/** AP_ACx set details **/
#define ARC_REG_AP_AC_ATOff (0)
#define ARC_REG_AP_AC_TTOff (4)
#define ARC_REG_AP_AC_MOff  (6)
#define ARC_REG_AP_AC_POff  (7)
#define ARC_REG_AP_AC_AAOff (8)
#define ARC_REG_AP_AC_QOff  (9)

#define ARC_REG_AP_AC_ATM   (0xf)
#define ARC_REG_AP_AC_TTM   (3<<4)
#define ARC_REG_AP_AC_MM    (1<<6)
#define ARC_REG_AP_AC_PM    (1<<7)
#define ARC_REG_AP_AC_AAM   (1<<8)
#define ARC_REG_AP_AC_QM    (1<<9)

/** AP_ACx AT set details **/
#define AP_Inst_Addr        0
#define AP_Inst_data        1
#define AP_LDST_Addr        2
#define AP_LDST_data        3
#define AP_REG_Addr         4
#define AP_REG_data         5
#define AP_Ext_Para0        6
#define AP_Ext_Para1        7

/** AP_ACx TT type ***/
#define AP_TT_Disable       0
#define AP_TT_WTrigger      1
#define AP_TT_RTrigger      2
#define AP_TT_WRTrigger     3

/** AP_ACx M Type **/
#define AP_M_In_Trigger     0
#define AP_M_OUT_Trigger    1

/** AP_ACx P Select **/
#define AP_P_Disable        0
#define AP_P_Enable         1

/** AP_ACx AA **/
#define AP_AA_Halt          0
#define AP_AA_Exception     1




#endif  /* _ASM_ARC_ARCDEFS_H */


