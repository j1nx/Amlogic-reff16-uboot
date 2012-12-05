/******************************************************************************
 * Copyright ARC International (www.arc.com) 2007-2009
 *
 * Vineetg: March 2009 (Supporting 2 levels of Interrupts)
 *  Stack switching code can no longer reliably rely on the fact that
 *  if we are NOT in user mode, stack is switched to kernel mode.
 *  e.g. L2 IRQ interrupted a L1 ISR which had not yet completed
 *  it's prologue including stack switching from user mode
 *
 * Vineetg: Feb 2009: Changes to stack Switching Macro
 *  The idea is to not have intermediate values in SP
 *    -Rather than using SP as scratcpad in switching code use R9
 *    -R25 safekeeping done before SP changes to kernel mode
 *
 * Vineetg: Aug 28th 2008: Bug #94984
 *  -Zero Overhead Loop Context shd be cleared when entering IRQ/EXcp/Trap
 *   Normally CPU does this automatically, however when doing FAKE rtie,
 *   we also need to explicitly do this. The problem in macros
 *   FAKE_RET_FROM_EXCPN and FAKE_RET_FROM_EXCPN_LOCK_IRQ was that this bit
 *   was being "CLEARED" rather then "SET". Actually "SET" clears ZOL context
 *
 * Vineetg: May 5th 2008
 *  -Modified CALLEE_REG save/restore macros to handle the fact that
 *      r25 contains the kernel current task ptr
 *  - Defined Stack Switching Macro to be reused in all intr/excp hdlrs
 *  - Shaved off 11 instructions from RESTORE_ALL_INT1 by using the
 *      address Write back load ld.ab instead of seperate ld/add instn
 *
 *****************************************************************************/
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
 *  linux/include/asm-arc/entry.h
 *
 *  Copyright (C) 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Authors: Amit Bhor, Sameer Dhavale
 */

#ifndef __ASM_ARC_ENTRY_H
#define __ASM_ARC_ENTRY_H
#include <asm/linkage.h>
#ifdef __ASSEMBLY__

//#include <asm/unistd.h> /* For NR_syscalls defination */
#include <asm/asm-offsets.h>
#include <asm/arcregs.h>
//#include <asm/current.h>
#include <asm/ptrace.h>
//#include <asm/page.h>
//#include <asm/pgtable.h>   // For VMALLOC_START
// disable TAKE_SNAM_ASM
.macro TAKE_SNAP_ASM reg_scratch, reg_ptr, type
.endm

; the ENTRY macro in linux/linkage.h does not work for us ';' is treated 
; as a comment and I could not find a way to put a new line in a #define

.macro ARC_ENTRY name
  .globl SYMBOL_NAME(#name) 
  .align 4      
  SYMBOL_NAME_LABEL(#name)
.endm  



/*-------------------------------------------------------------- 
 * Save caller saved registers (scratch registers) ( r0 - r12 )
 * Registers are pushed / popped in the order defined in struct ptregs
 * in asm/ptrace.h
 *-------------------------------------------------------------*/
.macro  SAVE_CALLER_SAVED
    st.a    r0, [sp, -4]
    st.a    r1, [sp, -4]
    st.a    r2, [sp, -4]
    st.a    r3, [sp, -4]
    st.a    r4, [sp, -4]
    st.a    r5, [sp, -4]
    st.a    r6, [sp, -4]
    st.a    r7, [sp, -4]
    st.a    r8, [sp, -4]
    st.a    r9, [sp, -4]
    st.a    r10, [sp, -4]
    st.a    r11, [sp, -4]
    st.a    r12, [sp, -4]
.endm

/*-------------------------------------------------------------- 
 * Restore caller saved registers (scratch registers)
 *-------------------------------------------------------------*/
.macro RESTORE_CALLER_SAVED
    ld.ab   r12, [sp, 4]
    ld.ab   r11, [sp, 4]
    ld.ab   r10, [sp, 4]
    ld.ab   r9, [sp, 4]
    ld.ab   r8, [sp, 4]
    ld.ab   r7, [sp, 4]
    ld.ab   r6, [sp, 4]
    ld.ab   r5, [sp, 4]
    ld.ab   r4, [sp, 4]
    ld.ab   r3, [sp, 4]
    ld.ab   r2, [sp, 4]
    ld.ab   r1, [sp, 4]
    ld.ab   r0, [sp, 4]
.endm


/*-------------------------------------------------------------- 
* Save callee saved registers (non scratch registers) ( r16 - r23 )
 * This is the actual context saved when a task gets scheduled out.
 * sp points to the next free element on the stack at exit of this macro.
 * Registers are pushed / popped in the order defined in struct callee_regs
 * in asm/ptrace.h
 *-------------------------------------------------------------*/
.macro SAVE_CALLEE_SAVED
    st.a    r13, [sp, -4]
    st.a    r14, [sp, -4]
    st.a    r15, [sp, -4]
    st.a    r16, [sp, -4]
    st.a    r17, [sp, -4]
    st.a    r18, [sp, -4]
    st.a    r19, [sp, -4]
    st.a    r20, [sp, -4]
    st.a    r21, [sp, -4]
    st.a    r22, [sp, -4]
    st.a    r23, [sp, -4]
    st.a    r24, [sp, -4]

#ifdef CONFIG_ARCH_ARC_CURR_IN_REG
    ; Retrieve orig r25 and save it on stack
    ld      r12, [r25, TASK_THREAD + THREAD_USER_R25]
    st.a    r12, [sp, -4]
#else
    st.a    r25, [sp, -4]
#endif

    /* move sp to next free entry */
    sub sp, sp, 4
.endm

/*-------------------------------------------------------------- 
 * Restore callee saved registers (non scratch registers)
 *-------------------------------------------------------------*/
.macro RESTORE_CALLEE_SAVED
    add     sp, sp, 4

#ifdef CONFIG_ARCH_ARC_CURR_IN_REG

    /* This is not needed as current->thread.r25 always had user r25*/
#if 0
    ; Get saved r25 from stack and copy in current->thread.user_r25
    ld.ab   r12, [sp, 4]    
        ; Cant do this hence following 3 lines
        ;st      r12, [r25, TASK_THREAD + 20]
    add  r25, r25,  TASK_THREAD + 20
    st   r12, [r25]
    sub  r25, r25,  TASK_THREAD + 20
#endif

    ; Just skip the r25 placeholder
    add     sp, sp, 4
    
#else
    ld.ab   r25, [sp, 4]
#endif

    ld.ab   r24, [sp, 4]    
    ld.ab   r23, [sp, 4]    
    ld.ab   r22, [sp, 4]    
    ld.ab   r21, [sp, 4]    
    ld.ab   r20, [sp, 4]    
    ld.ab   r19, [sp, 4]    
    ld.ab   r18, [sp, 4]    
    ld.ab   r17, [sp, 4]    
    ld.ab   r16, [sp, 4]    
    ld.ab   r15, [sp, 4]    
    ld.ab   r14, [sp, 4]    
    ld.ab   r13, [sp, 4]    
    
.endm

/*-------------------------------------------------------------- 
 * Super FAST Restore callee saved regs by simply re-adjusting SP
 *-------------------------------------------------------------*/
.macro RESTORE_CALLEE_SAVED_FAST
    add     sp, sp, 14 * 4
.endm

/*-------------------------------------------------------------- 
 * Restore User mode r25 saved in task_struct->thread.user_r25
 *-------------------------------------------------------------*/
.macro RESTORE_USER_R25
    ld  r25, [r25, TASK_THREAD + THREAD_USER_R25]
.endm

/*--------------------------------------------------------------
 * Switch to Kernel Mode stack if SP points to User Mode stack
 *
 * This macro
 *  -if event happened in USER mode proceeds to switch stack.
 *    (in other words, would _NOT_ switch stack if event happened
 *      in kernel mode)
 *  -For that calculates the new value of SP (kernel mode stack)
 *
 * Note (Assumption on ENTRY)
 *   Requires r9 to be loaded with the pre-intr/pre-excp status32
 *
 * Note: Special case handling for L2 ISR  
 *-------------------------------------------------------------*/

.macro SWITCH_TO_KERNEL_STK

    /* Which mode (user/kernel) was the system intr/excpn occured */
    and.f   0, r9, STATUS_U_MASK

    /* If in User mode, proceed to switch stack */
    bnz  88f

    /* OK we were already in kernel mode when this event happened, thus can
     * assume SP is kernle mode SP. _NO_ need to do any stack switching */

#ifdef  CONFIG_ARCH_ARC_LV2_INTR
    /* However....
     * If Level 2 Interrupts enabled, we may end up with a corner case where
     * although L2 ISR happened in Kernel mode, stack is still not switched
     * e.g. L2 IRQ interrupted a L1 ISR which had not yet completed
     * it's prologue including stack switching from user mode
     *
     * Thus this extra instance of switching stack...
     */ 
    brlo sp, VMALLOC_START, 88f

#endif

    /* Save Pre Intr/Exception Kernel SP on kernel stack 
     *  This is not strictly required when in Kernel mode. 
     *  We can skip this part here. However at the end, when restoring
     *  Regs back, we also need to make a check that for kernel mode
     *  SP is not restored. This however equires additional code, which
     *  in turn requires free Reg(s) to write the code itself, which we 
     *  dont have since rest of the reg file has been restored to 
     *  PRE-INTR/EXCP values
     */
    st.a    sp, [sp, -4]    

    b 77f

88: /*------Intr/Ecxp happened in user mode, "switch" stack ------ */

    /* Get the "Current" task */
    GET_CURR_TASK_ON_CPU   r9

    /* If current task pointer cached in R25, time to 
     *  -safekeep USER R25 in task->thread_struct->user_r25
     *  -load R25 with current task ptr
     */
#ifdef CONFIG_ARCH_ARC_CURR_IN_REG

    // Can't use this Single instruction becoz offset > 512
    //st      r25, [r9, TASK_THREAD + THREAD_USER_R25]

    add     r9, r9, TASK_THREAD
    st      r25, [r9, THREAD_USER_R25]
    sub     r9, r9, TASK_THREAD

    mov     r25, r9
#endif

    /* Get task->thread_info (this is essentially start of a PAGE) */
    ld  r9, [r9, TASK_THREAD_INFO]

    /* Go to end of page pointed to by task->thread_info.
     *  This is start of THE kernel stack (grows upwards remember)
     */
    add r9, r9, (THREAD_SIZE - 4)

    /* Save Pre Intr/Exception User SP on kernel stack */
    st.a    sp, [r9, -4]

    /* set SP to point to kernel mode stack */
    mov sp, r9

77: /* ----- Stack Switched to kernel Mode, Now save REG FILE ----- */

.endm

/*------------------------------------------------------------
 * "FAKE" a rtie to return from CPU Exception context
 * This is to re-enable Exceptions within exception
 * Look at EV_ProtV to see how this is actually used
 *-------------------------------------------------------------*/
/* TODO: maybe we dont need the Intr lockout and get rid of this */
.macro FAKE_RET_FROM_EXCPN_LOCK_IRQ  reg

    ld  \reg, [sp, PT_status32]
    and \reg, \reg, ~(STATUS_U_MASK|STATUS_DE_MASK|STATUS_E1_MASK|STATUS_E2_MASK)
    or  \reg, \reg, STATUS_L_MASK
    sr  \reg, [erstatus]
    mov \reg, 55f
    sr  \reg, [eret]

    rtie
55:
.endm

.macro FAKE_RET_FROM_EXCPN  reg

    ld  \reg, [sp, PT_status32]
    and \reg, \reg, ~(STATUS_U_MASK|STATUS_DE_MASK)
    or  \reg, \reg, STATUS_L_MASK
    sr  \reg, [erstatus]
    mov \reg, 55f
    sr  \reg, [eret]

    rtie
55:
.endm
/*-------------------------------------------------------------- 
 * Save all registers used by Exceptions (TLB Miss, Prot-V, Mem err etc)
 * Requires SP to be already switched to kernel mode Stack
 * sp points to the next free element on the stack at exit of this macro.
 * Registers are pushed / popped in the order defined in struct ptregs
 * in asm/ptrace.h
 *-------------------------------------------------------------*/
.macro SAVE_ALL_SYS

    /* restore original r9 , saved in ex_saved_reg1
     * It will be saved on stack in macro: SAVE_CALLER_SAVED
     */
    ld  r9, [SYMBOL_NAME(ex_saved_reg1)]

    /* now we are ready to save the remaining context */
    st.a    NR_syscalls + 1, [sp, -4]   /* positive value in orig_r8 */
    st.a    r0, [sp, -4]    /* orig_r0 */
    SAVE_CALLER_SAVED
    st.a    r26, [sp, -4]   /* gp */
    st.a    fp, [sp, - 4]
    st.a    blink, [sp, -4]
    lr  r9, [eret]
    st.a    r9, [sp, -4]
    lr  r9, [erstatus]
    st.a    r9, [sp, -4]
    st.a    lp_count, [sp, -4]
    lr  r9, [lp_end]
    st.a    r9, [sp, -4]
    lr  r9, [lp_start]
    st.a    r9, [sp, -4]
    lr  r9, [erbta]
    st.a    r9, [sp, -4]
        /* move sp to next free entry */
    sub sp, sp, 4
.endm

/*-------------------------------------------------------------- 
 * Save all registers used by system calls.
 *-------------------------------------------------------------*/
.macro SAVE_ALL_TRAP
    /* retsore original r9 , saved in ex_saved_reg1 */
    ld  r9, [SYMBOL_NAME(ex_saved_reg1)]

    /* now we are ready to save the remaining context */
    st.a    r8, [sp, -4]    /* orig_r8 */   
    st.a    r0, [sp, -4]    /* orig_r0 */
    SAVE_CALLER_SAVED
    st.a    r26, [sp, -4]   /* gp */
    st.a    fp, [sp, - 4]
    st.a    blink, [sp, -4]
    lr  r9, [eret]
    st.a    r9, [sp, -4]
    lr  r9, [erstatus]
    st.a    r9, [sp, -4]
    st.a    lp_count, [sp, -4]
    lr  r9, [lp_end]
    st.a    r9, [sp, -4]
    lr  r9, [lp_start]
    st.a    r9, [sp, -4]
    lr  r9, [erbta]
    st.a    r9, [sp, -4]
        /* move sp to next free entry */
    sub sp, sp, 4
.endm

/*-------------------------------------------------------------- 
 * Restore all registers used by system call or Exceptions
 * SP should always be pointing to the next free stack element
 * when entering this macro. 
 *
 * NOTE:
 *
 * It is recommended that lp_count/ilink1/ilink2 not be used as a dest reg
 * for memory load operations. If used in that way interrupts are deffered 
 * by hardware and that is not good. 
 *-------------------------------------------------------------*/
.macro RESTORE_ALL_SYS

    /* ignore free entry */
    add sp, sp, 4

    ld.ab   r9, [sp, 4]
    sr  r9, [erbta] 
    ld.ab   r9, [sp, 4]
    sr  r9, [lp_start]
    ld.ab   r9, [sp, 4]
    sr  r9, [lp_end]
    ld.ab   r9, [sp, 4]
    mov lp_count, r9
    ld.ab   r9, [sp, 4]
    sr  r9, [erstatus]
    ld.ab   r9, [sp, 4]
    sr  r9, [eret]
    ld.ab   blink, [sp, 4]
    ld.ab   fp, [sp, 4] 
    ld.ab   r26, [sp, 4]    /* gp */
    RESTORE_CALLER_SAVED

    /* ignoring orig_r0 and orig_r8 */
    ld  sp, [sp, 8] /* restore original sp */
.endm


/*-------------------------------------------------------------- 
 * Save all registers used by interrupt handlers.
 *-------------------------------------------------------------*/
.macro SAVE_ALL_INT1

    /* restore original r9 , saved in int1_saved_reg 
     * It will be saved on stack in macro: SAVE_CALLER_SAVED
     */
#ifdef CONFIG_SMP
    lr  r9, [ARC_REG_SCRATCH_DATA0]
#else
    ld  r9, [SYMBOL_NAME(int1_saved_reg)]
#endif

    /* now we are ready to save the remaining context :) */
    st.a    -1, [sp, -4]    /* orig_r8, -1 for interuppt level one */   
    st.a    -1, [sp, -4]    /* orig_r0 , -1 for interrpts */
    SAVE_CALLER_SAVED
    st.a    r26, [sp, -4]   /* gp */
    st.a    fp, [sp, - 4]
    st.a    blink, [sp, -4]
    st.a    ilink1, [sp, -4]
    lr  r9, [status32_l1]
    st.a    r9, [sp, -4]
    st.a    lp_count, [sp, -4]
    lr  r9, [lp_end]
    st.a    r9, [sp, -4]
    lr  r9, [lp_start]
    st.a    r9, [sp, -4]
    lr  r9, [bta_l1]
    st.a    r9, [sp, -4]
        /* move sp to next free entry */
    sub sp, sp, 4
.endm

.macro SAVE_ALL_INT2

    /* TODO-vineetg: SMP we can't use global nor can we use
     *   SCRATCH0 as we do for int1 because while int1 is using
     *   it, int2 can come
     */
    /* retsore original r9 , saved in sys_saved_r9 */
    ld  r9, [SYMBOL_NAME(int2_saved_reg)]

    /* now we are ready to save the remaining context :) */
    st.a    -2, [sp, -4]    /* orig_r8, -2 for interrupt level 2 */ 
    st.a    -1, [sp, -4]    /* orig_r0 , -1 for interrpts */
    SAVE_CALLER_SAVED
    st.a    r26, [sp, -4]   /* gp */
    st.a    fp, [sp, - 4]
    st.a    blink, [sp, -4]
    st.a    ilink2, [sp, -4]
    lr  r9, [status32_l2]
    st.a    r9, [sp, -4]
    st.a    lp_count, [sp, -4]
    lr  r9, [lp_end]
    st.a    r9, [sp, -4]
    lr  r9, [lp_start]
    st.a    r9, [sp, -4]
    lr  r9, [bta_l2]
    st.a    r9, [sp, -4]
        /* move sp to next free entry */
    sub sp, sp, 4
.endm

/*-------------------------------------------------------------- 
 * Restore all registers used by interrupt handlers.
 *
 * NOTE:
 *
 * It is recommended that lp_count/ilink1/ilink2 not be used as a dest reg
 * for memory load operations. If used in that way interrupts are deffered 
 * by hardware and that is not good. 
 *-------------------------------------------------------------*/

.macro RESTORE_ALL_INT1
    /* ignore free entry */
    add sp, sp, 4
    ld.ab   r9, [sp, 4]
    sr  r9, [bta_l1]
    ld.ab   r9, [sp, 4]
    sr  r9, [lp_start]
    ld.ab   r9, [sp, 4]
    sr  r9, [lp_end]
    ld.ab   r9, [sp, 4]
    mov lp_count, r9
    ld.ab   r9, [sp, 4]
    sr  r9, [status32_l1]
    ld.ab   r9, [sp, 4]
    mov ilink1, r9
    ld.ab   blink, [sp, 4]
    ld.ab   fp, [sp, 4] 
    ld.ab   r26, [sp, 4]    /* gp */
    RESTORE_CALLER_SAVED
    
    /* ignoring orig_r0 and orig_r8 */
    ld  sp, [sp, 8] /* restore original sp */
.endm

.macro RESTORE_ALL_INT2
    /* ignore free entry */
    add sp, sp, 4
    ld.ab   r9, [sp, 4]
    sr  r9, [bta_l2]
    ld.ab   r9, [sp, 4]
    sr  r9, [lp_start]
    ld.ab   r9, [sp, 4]
    sr  r9, [lp_end]
    ld.ab   r9, [sp, 4]
    mov lp_count, r9
    ld.ab   r9, [sp, 4]
    sr  r9, [status32_l2]
    ld.ab   r9, [sp, 4]
    mov ilink2, r9
    ld.ab   blink, [sp, 4]
    ld.ab   fp, [sp, 4] 
    ld.ab   r26, [sp, 4]    /* gp */
    RESTORE_CALLER_SAVED

    /* ignoring orig_r0 and orig_r8 */
    ld  sp, [sp, 8] /* restore original sp */
    
.endm


/*----------------------------------------------------
    vineetg, Dec 30th 2008
    Helper Macros to access the current_task for 
    switching to kernel mode stack
----------------------------------------------------*/

#ifdef CONFIG_SMP

/* Get CPU-ID of this core */
.macro  GET_CPU_ID  reg
    lr  \reg, [identity]
    lsr \reg, \reg, 8
    and \reg, \reg, 0xFF
.endm

/*------------------------------------------------- 
 * Get current running task on this CPU 
 * 1. Determine curr CPU id.
 * 2. Use it to index into _current_task[ ]
 
.macro  GET_CURR_TASK_ON_CPU_SLOW    reg
    GET_CPU_ID  \reg
    lsl \reg, \reg, 2
    ld  \reg, [_current_task, \reg]
.endm
 */

/* Using the Address Scaling mode to avoid extra instrn
 * Note this wont work with GCC 3.4 Assembler
 * REQuires src oper to be Reg
 */
.macro  GET_CURR_TASK_ON_CPU   reg
    GET_CPU_ID  \reg
    ld.as  \reg, [_current_task, \reg]
.endm

/*------------------------------------------------- 
 * Save a new task as the "current" task on this CPU 
 * 1. Determine curr CPU id.
 * 2. Use it to index into _current_task[ ]
 */

/* Ideal implementation I wanted, using Address Scaled Indexing
 * so offset in array calculated within st instn
 * GCC 4.2 Assembler doesnt allow this
 * This is because it is not allowed by ABI so no blame on Assmebler
 
.macro  PUT_CURR_TASK_ON_CPU_IDEAL    out_reg, tmp_reg
    GET_CPU_ID  \tmp_reg
    st.as  \out_reg, [_current_task, \tmp_reg]
.endm
 */

.macro  PUT_CURR_TASK_ON_CPU    out_reg, tmp_reg
    GET_CPU_ID  \tmp_reg
    lsl \tmp_reg, \tmp_reg, 2
    add \tmp_reg, \tmp_reg, _current_task
    st  \out_reg, [\tmp_reg]
#ifdef CONFIG_ARCH_ARC_CURR_IN_REG
    mov r25, \out_reg
#endif

.endm


#else   /* Uniprocessor implementation of macros */

.macro  GET_CURR_TASK_ON_CPU    reg
    ld  \reg, [_current_task]
.endm

.macro  PUT_CURR_TASK_ON_CPU    out_reg, tmp_reg
    st  \out_reg, [_current_task]
#ifdef CONFIG_ARCH_ARC_CURR_IN_REG
    mov r25, \out_reg
#endif
.endm

#endif /* SMP / UNI */

/* ------------------------------------------------------------------
 *   Get the Ptr to some field of Current Task at @off in task struct
 *       -Out Reg is specified by Caller
 *       -r9 is used as scratchpad so called must be aware
 *       -Uses r25 for Current task ptr if that is enabled
 */

#ifdef CONFIG_ARCH_ARC_CURR_IN_REG

.macro GET_CURR_TASK_FIELD_PTR  off,  reg
    add \reg, r25, \off
.endm

#else

.macro GET_CURR_TASK_FIELD_PTR  off,  reg
    GET_CURR_TASK_ON_CPU  r9
    add \reg, r9, \off
.endm

#endif


/* ------------------------------------------------------------------
 * If someone doent want to show diagnostic information on user faults
 * we make ret_from_user_faulst to simply, ret_from_exception
 *
 * Otherwise ret_from_user_faults, shows more diagnostic information
 */
#ifndef CONFIG_ARC_USER_FAULTS_DBG
#define ret_from_user_faults    ret_from_exception
#endif

#endif  /* __ASSEMBLY__ */

#endif  /* __ASM_ARC_ENTRY_H */
