#include <asm/arcregs.h>
#include <config.h>
.section .text , "ax" , @progbits
#define AHB_SDRAM_BASE 0xc1001000
#define AUX_IENABLE		0x40c
#define AUX_IRQ_LEV		0x200
#define C_ROM_BOOT_DEBUG_1 (AHB_SDRAM_BASE + 0xc04)
	.type boot_start, @function
	.globl	boot_start
	.globl  init_copy_end
	.globl  next_step
	.globl  _nand_read_ipl
	.globl  _end_ipl
	.globl  _spi_read
	.globl  _sd_read_ipl
	.globl  start_nomal
	.globl  zero_start
boot_start:
    ; enable ddr auto initialization 
    mov   r10, 0xc1104e3c
    ld.di r11, [r10]
    bset  r11, r11, 0
    st.di r11, [r10]
    ; enable DDR request 
    mov   r10, 0xc1104e4c
    mov   r11, 0xffffffff
    st.di r11, [r10]
    ; mapping
	;mov   r10, 0xc1000578
    ;ld.di r11, [r10]
    ;or	  r11, r11, 0x00000001
    ;st.di r11, [r10]
    ; config arc cpu clk 156Mhz
    mov   r10, 0xc1104168
    mov   r11, 0xc4e
    st.di r11, [r10]
    mov   r10, 0xc1104174
    mov   r11, 0x3980
    st.di r11, [r10]
    
    mov r10,0xc1100000+0x2262*4
    mov r11,0xea525
    st.di  r11, [r10]   
    mov r10,0xc1100000+0x2263*4
    mov r11,0xf0280100
    st.di  r11, [r10]

#ifdef CONFIG_NAND_ROMBOOT
	lr   r0,[0x11]
    and  r0,r0,~0x01
    sr   r0,[0x11]
    
	mov r1, AHB_SDRAM_BASE
	mov lp_count,0x300 
	mov r2,_start
	lp init_copy_end
	ld.ab %r3,[%r1,4]
	st.ab %r3,[%r2,4]
init_copy_end:
	sr 0x10, [0x10]
    lr r3, [0x11]
    nop
    nop
    nop
	j			next_step
next_step:
	
;	sr	0xff, [0x40c]	
;    lr	r10, [0x200]
;    and	r10, r10, ~(1<<5)
;    or	r10, r10, (1<<6)
;    sr	r10, [0x200]
 												; initialize frame pointer
	mov r1,_start+0xc00
	mov r10,_start
	sub r2,__data_end,r10
	add r2,r2,3
	and r2,r2,(~0x3)
	asr r2,r2,2

	ld.di r0,[C_ROM_BOOT_DEBUG_1]
	and	r5,r0,0x100f ; 0x100f
	cmp	r5,0x1000 ;0x1000		
	beq _spi_read

	and	r5,r0,0x20f0 ; 0x100f
	cmp	r5,0x2000 ;0x1000		
	beq _nand_read_ipl    			
	
	and	r5,r0,0x4f00 ; 0x100f
	cmp	r5,0x4000 ;0x1000		
	beq _sd_read_ipl

	b zero_start

_spi_read:
	mov %lp_count,%r2
	mov %r1,0xc1800c00
	mov %r2,_start+0xc00
	lp start_nomal
	ld.ab %r3,[%r1,4]
	st.ab %r3,[%r2,4]
start_nomal:
	b _end_ipl

_nand_read_ipl:
;Nand read function void _nand_read(unsigned target_addr,unsigned size)
	mov sp,__stack_end
	bl	_nand_read
	b 	_end_ipl

_sd_read_ipl:
;SD read function void _sd_read(unsigned target_addr,unsigned size)
	mov sp,__stack_end
	bl _sd_read
	b 	_end_ipl

_end_ipl:	
	/* Clear bss */
	mov_s r2, __bss_start
	mov_s r3, __bss_end
_clear_bss:
	st.ab 0,[r2,4]
	brlt  r2,r3,_clear_bss
	mov sp,__stack_end		; initialize stack pointer
;	sub	sp, sp, 16			; allocate expected call frame
;	mov fp, 0

#else
/* Initial Justice , Whether it is start from flash */
_current:
	/*
		r1 : src
		r2 : dest
		r3 : count
		r4 : acturally text_start
		r5 : target text_start 
		r6 : offset(temp)
	*/

	mov r1,pcl
	mov r5,__text_start
	sub r6,_current,r5
	sub r4,r1,r6
	
	cmp r4,r5
	jeq   bypass_copy
	
	/* Relocate text and data section */
	mov r2,__text_start
	mov r3,__text_end
	sub r3,r3, r2
	asr r3,r3,2
	sub r6,r2,r5
	mov lp_count,r3
	add r1,r4,r6
	lp   text_copy_end
	ld.ab %r3,[%r1,4]
	st.ab %r3,[%r2,4]
text_copy_end:
	mov r2,__data_start
	mov r3,__data_end
	sub r3,r3, r2
	asr r3,r3,2
	sub r6,r2,r5
	mov lp_count,r3
	add r1,r4,r6
	lp  bypass_copy
	ld.ab %r3,[%r1,4]
	st.ab %r3,[%r2,4]
bypass_copy:
    sr 1,[0x4b] ;flush dcache 
22: lr r1,[0x48];wait flush end
    and r0,r1,0x100 
    cmp r0,0 
    jnz 22b; 
    nop; 
    nop 
    nop 
    sr 0x10, [0x10];invalidate icache 
    lr r3, [0x11]
    nop
    nop
    nop
    j   fill_bss

/* This code will be executed from ram */	
	.globl fill_bass
fill_bss:
	/* Clear bss */
	mov_s r2, __bss_start
	mov_s r3, __bss_end

_clear_bss:
	st.ab 0,[r2,4]
	brlt  r2,r3,_clear_bss
	
	/* setup stack pointer */
	mov sp,__stack_end
#endif
	
	sr	0xff, [AUX_IENABLE]	
    lr	r10, [AUX_IRQ_LEV]
    and	r10, r10, ~(1<<5)
    or	r10, r10, (1<<6)
    sr	r10, [AUX_IRQ_LEV]
    
    /* enable dcache */
    lr   r0,[0x48]
    and  r0,r0,~0x01
    sr   r0,[0x48]
    jl start_arcboot
    
zero_start:
	sleep


