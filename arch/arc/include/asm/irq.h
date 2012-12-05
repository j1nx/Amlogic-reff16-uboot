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
 * include/asm-arc/irq.h
 *
 * Amit Shah: initial write from asm-armnommu/
 */

#ifndef __ASM_ARC_IRQ_H
#define __ASM_ARC_IRQ_H

#include <asm/ptrace.h>
#include <asm/arch/irqs.h>
#ifndef irq_cannonicalize
#define irq_cannonicalize(i)	(i)
#endif

#ifndef NR_IRQS

#ifdef CONFIG_SMP
#define NR_IRQS AM_ISA_IRQ_MAX
#else
#define NR_IRQS AM_ISA_IRQ_MAX
#endif

#endif

#define TIMER0_INT       3
#define TIMER1_INT       4

#define VUART_INT       5
#define VUART1_INT      10
#define VUART2_INT      11

#define INTERRUPT_6     6
#define INTERRUPT_7     7
#define INTERRUPT_8     8
#define INTERRUPT_9     9
#define INTERRUPT_10   10
#define INTERRUPT_12   12
#define INTERRUPT_13   13
#define INTERRUPT_14   14
#define INTERRUPT_15   15

#ifdef CONFIG_SMP
#define INTERRUPT_16    16
#define INTERRUPT_17    17
#define INTERRUPT_18    18
#define INTERRUPT_19    19
#define INTERRUPT_20    20
#define INTERRUPT_21    21
#define INTERRUPT_22    22
#define INTERRUPT_23    23
#define INTERRUPT_24    24
#define INTERRUPT_25    25
#define INTERRUPT_26    26
#define INTERRUPT_27    27
#define INTERRUPT_28    28
#define INTERRUPT_29    29
#define INTERRUPT_30    30
#define INTERRUPT_31    31

#define IDU_INTERRUPT_0 INTERRUPT_16
#endif


#define IRQ_FLG_LOCK    (0x0001)        /* handler is not replaceable   */
#define IRQ_FLG_REPLACE (0x0002)        /* replace existing handler     */
#define IRQ_FLG_STD     (0x8000)        /* internally used              */

// the same as SA_INTERRUPT,IRQF_DISABLED
#define  IRQ_ISA_FAST  (0x00000020)

#ifndef __ASSEMBLY__

typedef void (*irq_handler_t)(int, void *, struct pt_regs *);


struct irq_node {
    void               (*handler)(int, void *, struct pt_regs *);
    unsigned long      flags;
	int	disable_depth;
    void               *dev_id;
    const char         *devname;
    struct irq_node    *next;
    unsigned long mask;
    int vertor;
    int hand_num;
}; 

int setup_arc_irq(unsigned int irq, struct irq_node *node);

#define disable_irq_nosync(i) disable_irq(i)

extern void disable_irq(unsigned int);
extern void enable_irq(unsigned int);
extern int  get_hw_config_num_irq(void);
extern void init_IRQ(void);
extern void process_isa_interrupt(unsigned int irq, struct pt_regs *fp);
extern void process_isa_interrupt_fast(unsigned int irq, struct pt_regs *fp);
#endif

#endif

