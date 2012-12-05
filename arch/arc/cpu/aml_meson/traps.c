/*
 * linux/arch/ppc/kernel/traps.c
 *
 * Copyright (C) 1995-1996  Gary Thomas (gdt@linuxppc.org)
 *
 * Modified by Cort Dougan (cort@cs.nmt.edu)
 * and Paul Mackerras (paulus@cs.anu.edu.au)
 *
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * This file handles the architecture-dependent parts of hardware exceptions
 */

#include <common.h>
#include <command.h>
#include <asm/processor.h>
#include <asm/arcregs.h>

#if defined(CONFIG_CMD_KGDB)
int (*debugger_exception_handler)(struct pt_regs *) = 0;
#endif


/* Returns 0 if exception not found and fixup otherwise.  */
extern unsigned long search_exception_table(unsigned long);


/*
 * Trap & Exception support
 */

void
print_backtrace(unsigned long *sp)
{
	int cnt = 0;
	unsigned long i;

	printf("Call backtrace: ");
//	while (sp) {
//		if ((uint)sp > END_OF_MEM)
//			break;
//
//		i = sp[1];
//		if (cnt++ % 7 == 0)
//			printf("\n");
//		printf("%08lX ", i);
//		if (cnt > 32) break;
//		sp = (unsigned long *)*sp;
//	}
	printf("\n");
}

void show_regs(struct pt_regs * regs)
{
	int i;

//	printf("NIP: %08lX XER: %08lX LR: %08lX REGS: %p TRAP: %04lx DAR: %08lX\n",
//	       regs->nip, regs->xer, regs->link, regs, regs->trap, regs->dar);
//	printf("MSR: %08lx EE: %01x PR: %01x FP: %01x ME: %01x IR/DR: %01x%01x\n",
//	       regs->msr, regs->msr&MSR_EE ? 1 : 0, regs->msr&MSR_PR ? 1 : 0,
//	       regs->msr & MSR_FP ? 1 : 0,regs->msr&MSR_ME ? 1 : 0,
//	       regs->msr&MSR_IR ? 1 : 0,
//	       regs->msr&MSR_DR ? 1 : 0);
//
//	printf("\n");
//	for (i = 0;  i < 32;  i++) {
//		if ((i % 8) == 0)
//		{
//			printf("GPR%02d: ", i);
//		}
//
//		printf("%08lX ", regs->gpr[i]);
//		if ((i % 8) == 7)
//		{
//			printf("\n");
//		}
//	}
}


void
_exception(int signr, struct pt_regs *regs)
{
	show_regs(regs);
//	print_backtrace((unsigned long *)regs->gpr[1]);
//	panic("Exception in kernel pc %lx signal %d",regs->nip,signr);
}

void
MachineCheckException(int vecnum,struct pt_regs *regs)
{
	unsigned long fixup;


#if defined(CONFIG_CMD_KGDB)
	if (debugger_exception_handler && (*debugger_exception_handler)(regs))
		return;
#endif

	printf("Machine check in kernel mode.\n");
	printf("Caused by (from msr): ");
	printf("regs %p ",regs);
	panic("machine check");
}






void
UnknownException(int vecnum,struct pt_regs *regs)
{
#if defined(CONFIG_CMD_KGDB)
	if (debugger_exception_handler && (*debugger_exception_handler)(regs))
		return;
#endif
//	printf("Bad trap at PC: %lx, SR: %lx, vector=%lx\n",
//	       regs->nip, regs->msr, regs->trap);
	_exception(0, regs);
}
static int trap_switch_break=0;
static int trap_jmp_buf[256];
void
PrivilegeVException(int vecnum,struct pt_regs *regs)
{
#if defined(CONFIG_CMD_KGDB)

	if (debugger_exception_handler && (*debugger_exception_handler)(regs))
		return;
#endif
	show_regs(regs);
	panic("Software Emulation Exception");
}
void
TrapException(int vecnum,struct pt_regs *regs)
{
#if defined(CONFIG_CMD_KGDB)
	if (debugger_exception_handler && (*debugger_exception_handler)(regs))
		return;
#endif
//	printf("Bad trap at PC: %lx, SR: %lx, vector=%lx\n",
//	       regs->nip, regs->msr, regs->trap);
	_exception(0, regs);
}
#if defined(CONFIG_CMD_KGDB)
void
Timer1Int(int vecnum,struct pt_regs *regs)
{
    
    if (debugger_exception_handler && (*debugger_exception_handler)(regs)){
        write_new_aux_reg(ARC_REG_TIMER1_CTRL ,0xb);
        
		return;
	}
	write_new_aux_reg(ARC_REG_TIMER1_CTRL ,0xb);
}
#endif
void traps_init(void)
{
#if defined(CONFIG_CMD_KGDB)
    int i;
    irq_install_handler(0     ,(interrupt_handler_t *)UnknownException,NULL);
    irq_install_handler    (1     ,(interrupt_handler_t *)UnknownException,NULL);
    irq_install_handler(2     ,(interrupt_handler_t *)UnknownException,NULL);
    irq_install_handler(0x20  ,(interrupt_handler_t *)MachineCheckException,NULL);
    irq_install_handler(0x21  ,(interrupt_handler_t *)UnknownException,NULL);
    irq_install_handler(0x22  ,(interrupt_handler_t *)UnknownException,NULL);
    irq_install_handler(0x23  ,(interrupt_handler_t *)UnknownException,NULL);
    irq_install_handler(0x24  ,(interrupt_handler_t *)PrivilegeVException,NULL);
    irq_install_handler(0x25  ,(interrupt_handler_t *)TrapException,NULL);
    for(i=0x26;i<0x40;i++)
        irq_install_handler(i ,(interrupt_handler_t *)UnknownException,NULL);
    set_int_level(4,1);
    irq_install_handler(4     ,(interrupt_handler_t *)Timer1Int,NULL);
    write_new_aux_reg(ARC_REG_TIMER1_LIMIT,0x20000000);
    write_new_aux_reg(ARC_REG_TIMER1_CNT,0);
    write_new_aux_reg(ARC_REG_TIMER1_CTRL,3);
	write_new_aux_reg(AUX_IENABLE,read_new_aux_reg(AUX_IENABLE)|(1<<4));	
	  

    enable_interrupts();
#endif
}
