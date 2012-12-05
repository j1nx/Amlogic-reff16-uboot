/* 
 * Copyright Codito Technologies (www.codito.com) 
 *
 *  cpu/arc/start.S
 *
 *  Copyright (C) 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 * 
 * Authors : Sandeep Patil (sandeep.patil@codito.com)
 *           Pradeep Sawlani (pradeep.sawlani@codito.com)
 */

#include <asm/arcregs.h>    /* required for STATUS_H_SET */
#include <config.h>

    /* .text section loaded at TEXT_BASE defined in board/aa3/config.mk */
    .section .text , "ax" , @progbits
    .type _start, @function
    .align 4
    .globl _start



_start:
    /* Disable interrupts */
    flag 0
    /* Initialize Memory */
#ifdef CONFIG_AML_APOLLO
#ifdef CONFIG_AML_A1H

    
;    mov   r10, 0xc11004ec
;    mov   r11, 0x6c5
;    st.di r11, [r10]
;    mov   r10, 0xc11005b4
;    ld.di r11, [r10]
;    and   r11, r11, 0xfffff000
;    or    r11, r11, 0x6c5
;    st.di r11, [r10]

    mov   r10, 0xc1100000+0x2000*4
    ld.di r11,[r10]
    and    r11,r11,(~(0x1f<<4))
    or     r11,r11,((24<<4))
    st.di r11, [r10]
    mov   r10,0xc1100000+0x2650*4
    st.di 0x300,[r10]
    st.di 0,[r10,0x14] 
    mov   r10, 0xc1100000+0x2657*4
    ld.di r11,[r10]
    or    r11,r11,(1<<4)
    st.di r11, [r10]
    
    mov   r10, 0xc1104e44
    #ifdef CONFIG_AML_MEMORY_64MX2
        mov   r11, 0x1f8106
    #endif
    #ifdef CONFIG_AML_MEMORY_32MX2
        mov   r11, 0x1f8105
    #endif
    st.di r11, [r10]
    
    ; ddr cmd fifo size, 0x292
    mov   r10, 0xc1104e48
    mov   r11, 0x10
    st.di r11, [r10]
    ; ddr prot 1
    mov   r10, 0xc1104e04
    mov   r11, 0x700
    st.di r11, [r10]    
    ; timing control 0
    mov   r10, 0xc1104e14
    mov   r11, 0x1304
    st.di r11, [r10]
    ; timing control 1
        mov   r10, 0xc1104e18
    mov   r11, 0x3012
    st.di r11, [r10]
    ; timing control 2
    mov   r10, 0xc1104e1C
    mov   r11, 0x5433
    st.di r11, [r10]
    ; timing control 3
    mov   r10, 0xc1104e20
    mov   r11, 0x0082
    st.di r11, [r10]
    ; timing control 4
    mov   r10, 0xc1104e24
    mov   r11, 0x36b0
    st.di r11, [r10]
    ; timing control 5
    mov   r10, 0xc1104e28
    mov   r11, 0x0960
    st.di r11, [r10]
    ; timing control 6
    mov   r10, 0xc1104e2c
    mov   r11, 0x3421
    st.di r11, [r10]
    ; CAS_latency  bt  burst_length 
    mov   r10, 0xc1104e34
    mov   r11, 0x0033
    st.di r11, [r10]
    ;
    mov   r10, 0xc1104e38
    mov   r11, 0x0000
    st.di r11, [r10]
    ; ddr phy control, (10<<9)|(74)
    mov   r10, 0xc1104e08
    ld.di r11, [r10]    
    and   r11, r11, 0xffff8000
    or    r11, r11, (2<<9)|(15)
    st.di r11, [r10]
    ; DDRCLK_27_200
    mov   r10, 0xc1104e58
    mov   r11, 40000
    st.di r11, [r10]
    
    ; enable ddr clock
    mov   r10, 0xc1104e08
    ld.di r11, [r10]
    or    r11, r11, (1<<15)
    st.di r11, [r10]
    
    mov   r10, 0xc1104e40
pll_lock_wait:    
    ld.di r11, [r10]
    and   r11, r11, 0x8000
    cmp   r11, 0
    bz    pll_lock_wait
    
    ; ddr clock 
    mov   r10, 0xc1104e00
    mov   r11, 0x801f
    st.di r11, [r10]
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
    ;or   r11, r11, 0x00000001
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

#else
    mov   r10, 0xc1100a44
; 32bit
#ifdef CONFIG_AML_BOARD_8626
#ifdef CONFIG_AML_MEMORY_128
    mov   r11,0x1f8106 
#else
    mov   r11,0x1f8105
#endif
#else
    mov   r11, 0x1f0185
#endif
    st.di r11, [r10]
    
    ; timing control 0
    mov   r10, 0xc1100a14
    mov   r11, 0x1304
    st.di r11, [r10]
    ; timing control 1
    mov   r10, 0xc1100a18
    mov   r11, 0x3011
    st.di r11, [r10]
    ; timing control 2
    mov   r10, 0xc1100a1C
    mov   r11, 0x5433
    st.di r11, [r10]
    ; timing control 3
    mov   r10, 0xc1100a20
    mov   r11, 0x0082
    st.di r11, [r10]
    ; timing control 4
    mov   r10, 0xc1100a24
    mov   r11, 0x36b0
    st.di r11, [r10]
    ; timing control 5
    mov   r10, 0xc1100a28
    mov   r11, 0x0960
    st.di r11, [r10]
    ; timing control 6
    mov   r10, 0xc1100a2c
    mov   r11, 0x3421
    st.di r11, [r10]
    ; CAS_latency  bt  burst_length 
    mov   r10, 0xc1100a34
    mov   r11, 0x0033
    st.di r11, [r10]
    ;
    mov   r10, 0xc1100a38
    mov   r11, 0x0000
    st.di r11, [r10]
    ; ddr phy control, (10<<9)|(74)
    mov   r10, 0xc1100a08
    ld.di r11, [r10]    
    and   r11, r11, 0xffff8000
    ; 160M
    ;or    r11, r11, ( 1<<9)|( 6)
    ; 180M
    ;or    r11, r11, ( 3<<9)|(20)
    ; 200M
    or    r11, r11, 0x0144a
    st.di r11, [r10]
    ; DDRCLK_27_200
    mov   r10, 0xc1100a58
    ; 160M
    ;mov   r11, 32400
    ; 180M
    ;mov   r11, 36000
    ; 200M
    mov   r11, 40000
    st.di r11, [r10]
    
    ; enable ddr clock
    mov   r10, 0xc1100a08
    ld.di r11, [r10]
    or    r11, r11, (1<<15)
    st.di r11, [r10]
    
    mov   r10, 0xc1100a40
pll_lock_wait:    
    ld.di r11, [r10]
    and   r11, r11, 0x8000
    cmp   r11, 0
    bz    pll_lock_wait
    
    ; ddr clock 
    mov   r10, 0xc1100a00
    ; 32bit
    mov   r11, 0x801f
    st.di r11, [r10]
    ; enable ddr auto initialization 
    mov   r10, 0xc1100a3c
    ld.di r11, [r10]
    bset  r11, r11, 0
    st.di r11, [r10]
    ; enable DDR request 
    mov   r10, 0xc1100a4c
    mov   r11, 0xffffffff
    st.di r11, [r10]
#endif
#endif  
#ifdef CONFIG_AML_NIKE
    mov   r10, 0x01100568
    mov   r11, 0x00000320
    st.di r11, [r10]
    
    mov   r10, 0x01100574
    mov   r11, 0x00000180
    add   r11, r11, 1
    st.di r11, [r10]
    ; DRAM_MASK_0
    mov     r10, 0x01101d10
    mov     r11, 0x8000
    st.di r11, [r10]
    
    ; DRAM_MASK_1
    mov     r10, 0x01101d14
    mov     r11, 0x3b0e
    st.di r11, [r10]
    
    ; DRAM_CTRL_REG1
    mov     r10, 0x01101c00
    mov     r11, 0x0500
    st.di r11, [r10]
    mov     r10, 0x01000590
    mov     r11, 0x88110
    st.di r11, [r10]
#endif
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
    sr 0x10, [0x10]
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
    /* enable dcache */
    lr   r0,[0x48]
    and  r0,r0,~0x01
    sr   r0,[0x48]
    jl start_arcboot

    /* Interrupt service routines */
res_service :   
mem_service :
instr_service   :
handle_interrupt:
EV_MachineCheck :

EV_TLBMissI :
EV_TLBMissD :
EV_TLBProtV :
EV_PrivilegeV   :
EV_Trap     :
EV_Extension    :
reserved    :
            FLAG STATUS_H_SET
            NOP
            NOP
            NOP 
.globl _TEXT_BASE
_TEXT_BASE:
    .word TEXT_BASE
    
.globl _bss_start
_bss_start:
    .word __bss_start

.globl _bss_end
_bss_end:
    .word __bss_end

.globl _arcboot_start
_arcboot_start:
    .word _start        
