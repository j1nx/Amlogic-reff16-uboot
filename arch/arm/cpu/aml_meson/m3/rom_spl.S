/*
 * aml6236-m nand spl
 * @author jerry.yu
 *
 */
/* These define will be replace with */
#include <config.h>
#include <version.h>
#include <asm/arch/romboot.h>
/*
  .equ TEXT_BASE,0x80000000
  .equ CONFIG_SYS_MALLOC_LEN,0x80000
  .equ CONFIG_STACKSIZE_FIQ,0x1000
  .equ CONFIG_STACKSIZE_IRQ,0x1000
  .equ CONFIG_SYS_GBL_DATA_SIZE,0x1000
  .equ PL310_BASE     , 0xc4200000
 */
.globl _start
_start: b reset
  ldr pc, _undefined_instruction
  ldr pc, _software_interrupt
  ldr pc, _prefetch_abort
  ldr pc, _data_abort
  ldr pc, _not_used
  ldr pc, _irq
  ldr pc, _fiq
__pad:                  .word 0x49001ff0 /* now 16*4=64 */
_undefined_instruction: .word undefined_instruction
_software_interrupt:    .word software_interrupt
_prefetch_abort:        .word prefetch_abort
_data_abort:            .word data_abort
_not_used:              .word not_used
_irq:                   .word irq
_fiq:                   .word fiq


.global _end_vect
_end_vect:
  .balignl 16,0xdeadbeef
_AHB_SRAM_BASE:
  .word AHB_SRAM_BASE
_AHB_SRAM_END:
  .word AHB_SRAM_BASE + READ_SIZE
__TEXT_BASE:
  .word TEXT_BASE
__TEXT_SIZE:
  .word 0x40000
__ROM_STACK_END:
  .word ROM_STACK_END
.globl reset
reset:
  adr r0, _start       		// r0 <- current position of code
  ldr r1, _AHB_SRAM_BASE   	// test if we run from flash or RAM
  cmp r0, r1           		// dont reloc during debug
  beq stack_setup
  ldr r2,_AHB_SRAM_END
relocate_copy:
  ldmia r0!,{r3-r10}
  stmia r1!,{r3-r10}
  cmp r1,r2
  bne relocate_copy
  ldr pc,_AHB_SRAM_BASE
stack_setup:
  ldr r1, target_addr
  sub r1,r1,#4
  adr r0, target_addr
  ldmia r0!,{r3-r6}
  stmia r1!,{r3-r6}
  ldr     sp, __ROM_STACK_END  // STACK_END is defined in memmap

  bl	lowlevel_init
#ifndef AML_DEBUG_ROM
  ldr   r0,__TEXT_BASE
  ldr   r1,__TEXT_SIZE
  bl	relocate_init
  ldr   pc,__TEXT_BASE
#endif
undefined_instruction:
software_interrupt:
prefetch_abort:
data_abort:
not_used:
irq:
fiq:
  ldr pc,_fiq
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

.global __ihw_setting
__ihw_setting:
  .space 0x1b0 - (. - _start)
  .org 0x1b0
.global __magic_word
__magic_word:
  .word MAGIC_WORD1
  .word MAGIC_WORD2
  .word 0
  .word 0
  .word 0xfe0b0101
  .word 0x3ec1e07f
  .word 0xa9e00000
  .word 0x00000075
  .space 0x1fe - (. - _start)
  .short 0xaa55
.global _end_rom_spl
_end_rom_spl:
