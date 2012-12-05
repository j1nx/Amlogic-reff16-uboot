/*
 * cold heart 
 * author: elvis.yu
 * email: elvis.yu@amlogic.com
 */
/* These define will be replace with */
#include <config.h>
#include <version.h>
#include <asm/arch/romboot.h>
#include <asm/arch/firm/regs.h>
//#include <asm/arch/entry-macro.S>

.globl _start
_start: 
	b	reset
	ldr	pc,_undefined_instruction
	ldr	pc,_software_interrupt
	ldr	pc,_prefetch_abort
	ldr	pc,_data_abort
	ldr	pc,_not_used
	ldr	pc,_irq
	ldr	pc,_fiq

_undefined_instruction: .word undefined_instruction
_software_interrupt:	.word software_interrupt
_prefetch_abort:	.word prefetch_abort
_data_abort:		.word data_abort
_not_used:		.word not_used
_irq:			.word irq
_fiq:			.word fiq
_pad:			.word 0x12345678 /* now 16*4=64 */


/* IRQ stack memory (calculated at run-time) */
.globl IRQ_STACK_START
IRQ_STACK_START:
	.word	ROM_IRQ_STACK

/* IRQ stack memory (calculated at run-time) */
.globl FIQ_STACK_START
FIQ_STACK_START:
	.word ROM_FIQ_STACK

reset:
	/*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr,r0
  ldr     sp, __ROM_STACK_END  // setup stack pointer 
	/*
		Whether I am running in SRAM 
      */
  adr r0, _start       		// r0 <- current position of code
  ldr r1, _AHB_SRAM_BASE  // test if we run from flash or RAM
  cmp r0, r1           		// dont reloc during debug
  beq sdram_running				// skip relocate 
  ldr r2,_READ_SIZE
  bl  ipl_memcpy
  add pc,r1,#( sdram_running - _start )
sdram_running:
	ldr r3,__BSS_START
	ldr r4,__BSS_END
	mov r5,#0
clr_bss:
	cmp r3,r4
	beq clr_bss_end
	str r5,[r3]
	add r3,r3,#4
	b 	clr_bss
clr_bss_end:

	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000	@ clear bits 13 (--V-)
	mcr	p15, 0, r0, c1, c0, 0
	adr r0, _start
  mcr p15,0,r0,c12,c0,0
  bl	freezing_main
#ifndef AML_DEBUG_ROM
  ldr   r0,__TEXT_BASE
  ldr   r1,__TEXT_SIZE
  #bl	relocate_init
  ldr   pc,__TEXT_BASE
#endif  

_AHB_SRAM_BASE:          //‘⁄<soc/romboot.h> ∂®“Â
  .word AHB_SRAM_BASE
_READ_SIZE:
	.word READ_SIZE
_AHB_SRAM_END:
  .word AHB_SRAM_BASE + READ_SIZE
  
__TEXT_BASE:
  .word TEXT_BASE
__TEXT_SIZE:
  .word 0x60000 - READ_SIZE   //uboot.bin  ≥ﬂ¥Á
  
__ROM_STACK_END:
  .word ROM_STACK_END
  
__BSS_START:
	.word _bssstart
__BSS_END:
	.word _bssend
	
  .global	romboot_info
  .type   romboot_info, %object
	.size   romboot_info, 4
romboot_info:
        .word   C_ROM_BOOT_DEBUG
	
#if 1	        
	.global	ipl_memcpy
	.type	ipl_memcpy, %function
ipl_memcpy:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	str	r4, [sp, #-4]!
	movs	r2, r2, lsr #2
	beq	.L4
	mov	ip, #0
	mov	r3, ip
.L3:
	mov	ip, ip, asl #2
	ldr	r4, [r1, ip]
	str	r4, [r0, ip]
	add	r3, r3, #1
	mov	ip, r3
	cmp	r3, r2
	bcc	.L3
.L4:
	ldmfd	sp!, {r4}
	bx	lr
#endif	

.global wait
wait:
	ldr pc,target_addr
target_addr:
	.word  0x49001ff4
test_func:
	dmb
	isb
	ldr pc,target_addr
	
.global _end_wait
_end_wait:

.global _end_rom_spl
_end_rom_spl:

/*
 *************************************************************************
 *
 * Interrupt handling
 *
 *************************************************************************
 */
@
@ IRQ stack frame.
@
#define S_FRAME_SIZE	72

#define S_OLD_R0	68
#define S_PSR		64
#define S_PC		60
#define S_LR		56
#define S_SP		52

#define S_IP		48
#define S_FP		44
#define S_R10		40
#define S_R9		36
#define S_R8		32
#define S_R7		28
#define S_R6		24
#define S_R5		20
#define S_R4		16
#define S_R3		12
#define S_R2		8
#define S_R1		4
#define S_R0		0

#define MODE_SVC 0x13
#define I_BIT	 0x80

.macro	irq_save_user_regs
sub sp, sp, #S_FRAME_SIZE
stmia	sp, {r0 - r12}			@ Calling r0-r12
add r8, sp, #S_PC			@ !! R8 NEEDS to be saved !!
					@ a reserved stack spot would
					@ be good.
stmdb	r8, {sp, lr}^			@ Calling SP, LR
str lr, [r8, #0]			@ Save calling PC
mrs r6, spsr
str r6, [r8, #4]			@ Save CPSR
str r0, [r8, #8]			@ Save OLD_R0
mov r0, sp
.endm

.macro	irq_restore_user_regs
ldmia	sp, {r0 - lr}^			@ Calling r0 - lr
mov r0, r0
ldr lr, [sp, #S_PC] 		@ Get PC
add sp, sp, #S_FRAME_SIZE
subs	pc, lr, #4			@ return & move spsr_svc into
					@ cpsr
.endm


.macro get_irq_stack			@ setup IRQ stack
ldr sp, IRQ_STACK_START
.endm

.macro get_fiq_stack			@ setup FIQ stack
ldr sp, FIQ_STACK_START
.endm

	/*
	 * Interrupt handling.	Preserves r7, r8, r9
	 */
	.macro	irq_handler
	get_irqnr_preamble r5, lr
	get_irqnr_and_base r0, r6, r5, lr
	bl do_irq	
	.endm





undefined_instruction: 
	mov r0,#1
	b fail_sleep
software_interrupt:	
	mov r0,#2
	b fail_sleep
prefetch_abort:	
	mov r0,#3
	b fail_sleep
data_abort:
	mov r0,#4
	b fail_sleep		
	
not_used:		
	mov r0,#5
	b fail_sleep
irq:	
	get_irq_stack
	irq_save_user_regs
	@irq_handler
	bl	do_irq
	irq_restore_user_regs		
fiq:
	mov r0,#7
	b fail_sleep	
fail_sleep:
	ldr     sp, =0x49002000  // setup stack pointer 			
	bl do_exception
	wfi
