/*************************************************************************
 * Copyright ARC International (www.arc.com) 2007-2009
 *
 *  vineetg: Feb 2008
 *
 *      System Event Logging APIs
 *
 ************************************************************************/

#ifndef __ASM_ARC_EVENT_LOG_H
#define __ASM_ARC_EVENT_LOG_H

/*######################################################################
 *
 *    Event Logging API
 *
 *#####################################################################*/

/* This is the main knob which enables Profiling code to be
 * generated at all. If not, all the API calls are #ifdef away
 */
//#define ARC_DBG_EVENT_TIMELINE 1


#define MAX_SNAPS  1024
#define MAX_SNAPS_MASK (MAX_SNAPS-1)

#define SNAP_INTR_IN            1
#define SNAP_INTR_OUT           2
#define SNAP_EXCP_IN            3
#define SNAP_EXCP_OUT           4
#define SNAP_TRAP_IN            5
#define SNAP_TRAP_OUT           6
#define SNAP_PRE_CTXSW_2_U      7
#define SNAP_PRE_CTXSW_2_K      8
#define SNAP_DO_PF_ENTER        9
#define SNAP_DO_PF_EXIT         10
#define SNAP_INTR_OR_EXCP_OUT   11
#define SNAP_UPD_MMU_IN         12
#define SNAP_UPD_MMU_OUT        13
#define SNAP_TLB_FLUSH_ALL      14
#define SNAP_SENTINEL           50
#define SNAP_PROTV_IN           64


#ifndef ARC_DBG_EVENT_TIMELINE // event capture disabled

#ifndef __ASSEMBLY__

#define take_snap(type,extra,ptreg)
#define sort_snaps(halt_after_sort)

#else

.macro TAKE_SNAP_ASM reg_scratch, reg_ptr, type
.endm

#endif

#else // Event capture enabled 

#include <asm/arcregs.h>
#include <asm/ptrace.h>

#ifndef __ASSEMBLY__

typedef struct {

    /*  0 */ char nm[16];
    /* 16 */ unsigned int extra; /* Traps: Sys call num,
                                    Intr: IRQ, Excepn:  
                                  */
    /* 20 */ unsigned int  fault_addr;
    /* 24 */ unsigned int  cause;
    /* 28 */ unsigned int task;
    /* 32 */ unsigned long time;
    /* 36 */ unsigned int  event;
    /* 40 */ unsigned int  sp;
}
timeline_log_t;

void take_snap(int type, unsigned int extra, struct pt_regs *p);
void sort_snaps(int halt_after_sort);

#else /* ASM */

#include <asm/asm-offsets.h>

.macro TAKE_SNAP_ASM reg_scratch, reg_ptr, type

    /* Free up "reg_scratch" reg by temp hauling it to a global var.
     *  When this macro is called, only r9 (reg_ptr)
     *  has been saved by 1st instruction in low level handler. Since we need
     *  at least 2 regs to code this macro, need to free up one more reg.
     *
     *  Using single global "tmp_save_reg" is ok for across all the low
     *  level hamdlers (Intr/Exception/Trap) etc because this macros is 
     *  called so early that Intr can't sneak in Excption handler and 
     *  vice-versa
     */
    st \reg_scratch, [tmp_save_reg]

    ld \reg_ptr, [timeline_ctr]

    ; hack to detect if we are trying to overflow the circular log buffer
    brne \reg_ptr, MAX_SNAPS, 1f
    flag 1
    nop
    nop
    nop

1:
    mpyu  \reg_ptr, \reg_ptr, EVLOG_RECORD_SZ
    add \reg_ptr, timeline_log, \reg_ptr

    ;############ Common data ##########

    ; TIMER1 count in timeline_log[timeline_ctr].time
    lr  \reg_scratch, [ARC_REG_TIMER1_CNT]
    st  \reg_scratch, [\reg_ptr, EVLOG_FIELD_TIME]

    ; current task ptr in timeline_log[timeline_ctr].task
    ld \reg_scratch, [_current_task]
#if 1
    ld \reg_scratch, [\reg_scratch, TASK_TGID]
#endif
    st \reg_scratch, [\reg_ptr, EVLOG_FIELD_TASK]

    ; Type of event (Intr/Excp/Trap etc)
    st \type, [\reg_ptr, EVLOG_FIELD_EVENT_ID]

    ; save SP at time of exception 
    st sp, [\reg_ptr, EVLOG_FIELD_SP]

#if 0
    ; ASID reg
    lr \reg_scratch, [0x409]
    and \reg_scratch, \reg_scratch, 0xFF
    st \reg_scratch, [\reg_ptr, EVLOG_FIELD_EXTRA]
#endif

    st 0, [\reg_ptr, EVLOG_FIELD_EFA]
    st 0, [\reg_ptr, EVLOG_FIELD_EXTRA]

    ;############ Event specific data ##########

    ; save additional info depending on data point
    ;    in timeline_log[timeline_ctr].cause
    ;
    mov \reg_scratch, \type

    /* for Trap, Syscall number  */
    cmp \reg_scratch, SNAP_TRAP_IN
    bnz 3f
    st r8, [\reg_ptr, EVLOG_FIELD_CAUSE]
    j 6f

3: /* for Interrupts, IRQ */
    
    cmp \reg_scratch, SNAP_INTR_IN
    bnz 4f
    lr \reg_scratch, [icause1]
    st \reg_scratch, [\reg_ptr, EVLOG_FIELD_CAUSE]
    j 6f

4: /* For Exceptions (TLB/ProtV etc)*/
    cmp \reg_scratch, SNAP_EXCP_IN
    bnz 6f

    lr \reg_scratch, [ecr]
    st \reg_scratch, [\reg_ptr, EVLOG_FIELD_CAUSE]
    lr \reg_scratch, [efa]
    st \reg_scratch, [\reg_ptr, EVLOG_FIELD_EFA]
    lr \reg_scratch, [eret]
    st \reg_scratch, [\reg_ptr, EVLOG_FIELD_EXTRA]

6:
    /* increment timeline_ctr  with mode on max */
    ld \reg_scratch, [timeline_ctr]
    add \reg_scratch, \reg_scratch, 1
    and \reg_scratch, \reg_scratch, MAX_SNAPS_MASK
    st \reg_scratch, [timeline_ctr]

    /* Restore back orig scratch reg */
    ld \reg_scratch, [tmp_save_reg]
.endm

#endif	/* __ASSEMBLY__ */

#endif /* ARC_DBG_EVENT_TIMELINE */

/*######################################################################
 *
 *    Process fork/exec Logging
 *
 *#####################################################################*/

#ifndef __ASSEMBLY__

//#define CONFIG_DEBUG_ARC_PID 1

#ifndef CONFIG_DEBUG_ARC_PID

#define fork_exec_log(p, c)

#endif /* CONFIG_DEBUG_ARC_PID */

#endif /* __ASSEMBLY__ */

#endif /* __ASM_ARC_EVENT_PROFILE_H */
