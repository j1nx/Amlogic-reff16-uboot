/*
 * aml6236-m nand spl
 * @author jerry.yu
 *
 */
/* These define will be replace with */
#include <config.h>
#include <version.h>
#include <asm/arcregs.h>
#include <asm/arch/romboot.h>
.globl _start
_start:
	lr			r0,[ARC_REG_PC]
  sub			r0,r0,. - _start
  /*
		Whether I am running in SRAM 
	*/
  mov 	r1,AHB_SRAM_BASE 
  mov   sp,ROM_STACK_END  // setup stack pointer 
  cmp			r0,r1
  bz			sdram_running
  // not equal 
  mov_s 	r2,_end
  sub			r2,r2,_start
  bl  		ipl_memcpy
  j 			sdram_running
sdram_running:
	mov r3,_bssstart
	mov r4,_bssend
	mov r5,0
clr_bss:
	st.ab 	r5,[r3,4]
	cmp r3,r4
	bne clr_bss
  bl	lowlevel_init
#ifndef AML_DEBUG_ROM
  mov   r0,TEXT_BASE
  mov   r1,0x80000 - READ_SIZE
  bl	relocate_init
  j 	TEXT_BASE
#endif  
	.align 4
_AHB_SRAM_BASE:
  .word AHB_SRAM_BASE
_READ_SIZE:
	.word READ_SIZE
_AHB_SRAM_END:
  .word AHB_SRAM_BASE + READ_SIZE
__TEXT_BASE:
  .word TEXT_BASE
__TEXT_SIZE:
  .word 0x80000 - READ_SIZE
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
.global	magic_info
  .type   magic_info, %object
	.size   magic_info, 4
magic_info:
	.word  __magic_word
#if 0	        
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
.global _end_rom_spl
_end_rom_spl:
