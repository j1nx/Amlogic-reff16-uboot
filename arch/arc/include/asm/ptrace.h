/*
 * Copyright Codito Technologies (www.codito.com) 
 *
 *  include/asm-arc/ptrace.h
 *
 *  Copyright (C) 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * 
 */
#ifndef __ASM_ARC_PTRACE_H
#define __ASM_ARC_PTRACE_H

#include <linux/config.h>
#include <asm/debug.h>
#include <asm/arcregs.h>

#ifndef __ASSEMBLY__

/* this struct defines the way the registers are stored on the
   stack during a system call. */

struct pt_regs {
    long    sp          ;
    long	status32    ;	    /* status32_l1, status32_l2, erstatus */
	long	ret         ;		/* ilink1, ilink2 or eret*/
	long	bta         ;		/* bta_l1, bta_l2, erbta */
	long   lp_end       ; 
	long   lp_start     ;
	long   lp_count     ;
	long   fp           ;
	long   gp           ;
	long   r25          ;
	long   r24          ;
	long   r23          ;
	long   r22          ;
	long   r21          ;
	long   r20          ;
	long   r19          ;
	long   r18          ;
	long   r17          ;
	long   r16          ;
	long   r11          ;
	long   r10          ;
	long   r9           ;
	long   r8           ;
	long   r7           ;
	long   r6           ;
	long   r5           ;
	long   r4           ; 
	long    r15         ;
    long    r14         ;
    long    r13         ;
    long    r12         ;
    long    r3          ;
    long    r2          ;
    long    r1          ;
    long    r0          ;
    long    blink       ;
};

#endif /* !__ASSEMBLY__ */
/* Offsets of various elements(registers) of struct pt_regs on the stack.
 * The offsets starts with 4 because sp points to next free element on stack
 */
#define PT_sp              0x00
#define PT_status32        0x04 
#define PT_ret		       0x08
#define PT_bta		       0x0C
#define PT_lp_end          0x10
#define PT_lp_start        0x14
#define PT_lp_count        0x18
#define PT_fp              0x1C
#define PT_gp              0x20
#define PT_r25             0x24
#define PT_r24             0x28
#define PT_r23             0x2C
#define PT_r22             0x30
#define PT_r21             0x34
#define PT_r20             0x38
#define PT_r19             0x3C
#define PT_r18             0x40
#define PT_r17             0x44
#define PT_r16             0x48
#define PT_r11             0x4C
#define PT_r10             0x50
#define PT_r9              0x54
#define PT_r8              0x58
#define PT_r7              0x5C
#define PT_r6              0x60
#define PT_r5              0x64
#define PT_r4              0x68
#define PT_r15             0x6C
#define PT_r14             0x70
#define PT_r13             0x74
#define PT_r12             0x78
#define PT_r3              0x7C
#define PT_r2              0x80
#define PT_r1              0x84
#define PT_r0              0x88
#define PT_blink           0x8C

#endif /* __ASM_PTRACE_H */

