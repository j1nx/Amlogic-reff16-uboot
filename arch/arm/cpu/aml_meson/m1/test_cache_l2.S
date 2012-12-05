
    .global   _get_cache_type
    .global   _get_cache_level
    .global   _get_cache_size_id
    .global   _l2_cache_lockdown 
    .global   _get_cache_size_select
    .global  DataUnifiedCache_test
    .global  L1_icache_bist_test
    .global  L1_dcache_bist_test

@int _get_cache_type(void);
_get_cache_type:
    push {lr}
    mrc p15,0, r0, c0,c0,1 @ Read Cache Type Register
    pop {pc}

@int _get_cache_level(void);
_get_cache_level:
    push {lr}
    mrc p15,1,r0,c0,c0,1  @ read Cache Level ID Register
    pop {pc}

@int _get_cache_size_id(int);
_get_cache_size_id:
    push {lr}
    mrc p15,1,r0,c0,c0,0 @ Cache Size Identification Register
    pop {pc}

_get_cache_size_select:
    push {lr}
    mrc p15,2,r0,c0,c0,0  @ read cache size selection register
    pop  {pc}


CleanDcache:
    push {lr}
    mov r0, #0
    mcr p15,0,r0,c7,c10,0  @ 
    pop  {pc}

InvalidateDcache:
    push {lr}
    mrc p15,0,r0,c7,c6,0  @ 
    mov r0, #0
    mcr p15,0,r0,c7,c6,0  @ 
    pop  {pc}
CleanInvalidateDcache:
    push {lr}
    mov r0, #0
    mcr p15,0,r0,c7,c14,0  @ 
    pop  {pc}


InvalidateIcache:
    push {lr}
    mov r0, #0
    mcr p15,0,r0,c7,c5,0  @ 
    mcr p15,0,r0,c7,c5,6  @ 
    pop  {pc}



@ clean entire unified cache
CleanUnifiedcache:
    push {lr}
    mov r0, #0
    mcr p15,0,r0,c7,c11,0  @ 
    pop  {pc}

InvalidateUnifiedcache:
    push {lr}
    mov r0, #0
    mcr p15,0,r0,c7,c7,0  @ 
    pop  {pc}
CleanInvalidateUnifiedcache:
    push {lr}
    pop  {pc}


DataUnifiedCacheEnable:
    push {lr}
    mrc p15,0,r0,c1,c0,0  @
    orr r0,r0,#4          @ enable Data and unified cache
    mcr p15,0,r0,c1,c0,0  @ 
    pop  {pc}
DataUnifiedCacheDisable:
    push {lr}
    mrc p15,0,r0,c1,c0,0  @
    bic r0,r0,#4          @ disable Data and unified cache
    mcr p15,0,r0,c1,c0,0  @ 
    pop  {pc}

InstructionCacheEnable:
    push {lr}
    mrc p15,0,r0,c1,c0,0  @
    orr r0,r0,#1000          @ enable instruction cache
    mcr p15,0,r0,c1,c0,0  @ 
    pop  {pc}
InstructionCacheDisable:
    push {lr}
    mrc p15,0,r0,c1,c0,0  @
    bic r0,r0,#1000          @ disable instruction cache
    mcr p15,0,r0,c1,c0,0  @ 
    pop  {pc}


MmuEnabling:
    push {lr}
    mrc p15,0,r0,c1,c0,0  @
    orr r0,r0,#1          @ enable mmu
    mcr p15,0,r0,c1,c0,0  @ 
    pop  {pc}
MmuDisabling:
    push {lr}
    mrc p15,0,r0,c1,c0,0  @
    bic r0,r0,#1          @ disable mmu
    mcr p15,0,r0,c1,c0,0  @ 
    pop  {pc}
MpuEnabling:
    push {lr}
    pop  {pc}
MpuDisabling:
    push {lr}
    pop  {pc}

@void _l2_cache_lockdown(void);
_l2_cache_lockdown:
    push {lr}
    
    @ disable irq,fiq
    mrs	r0, cpsr
	orr	r0, r0, #0xc0  @ disable irq,fiq
	msr	cpsr,r0
    pop {pc}



@int DataUnifiedCache_test(void);
@int DataUnifiedCache_test(int fill_value,int modify_value);
DataUnifiedCache_test:
    push {r4,r5,r6,r7,r8,lr}
    mov r6,r0
    mov r7,r1
    @ diable irq,fiq
    mrs	r0, cpsr
    mov r8,r0
	orr	r0, r0, #0xc0  @ disable irq,fiq
	msr	cpsr,r0
    
    @init L2-cache
    bl cache_l2x0_enable
    
    @invalidate icache 
    @bl InvalidateIcache
    bl _invalidate_icache
    
    
    @ clean cache and unified cache
    @bl CleanDcache
    @bl InvalidateDcache
    bl _clean_dcache
    
    @bl CleanUnifiedcache
    bl cache_l2x0_clean_all
    
    bl cache_l2x0_disable
    @bl DataUnifiedCacheDisable

    @disable icache
    @bl InstructionCacheDisable
    
    ldr r4,_start_no_cache_mem
    ldr r5,_cache_size
    @ldr r2,_data_5555
    mov r2,r6
    mov r3,#0
fill_1:
    str r2,[r4],#4
    add r3,r3,#4
    cmp r5,r3
    bne fill_1
    
    ldr r4,_start_cache_mem

    @enable data and unified cache
    bl cache_l2x0_enable
    @bl DataUnifiedCacheEnable
    
    @mov r5,#0x1000
    ldr r5,_cache_size
    lsr r5,r5,#5
    mov r1,#0
fill_cache:
    ldr r0,[r4],#32
    add r1,r1,#1
    cmp r1,r5
    bne fill_cache
    
    ldr r4,_start_cache_mem
    ldr r5,_cache_size
    mov r1,#0
    @ldr r2,_data_aaaa
    mov r2,r7
modify_cache:
    str r2,[r4],#4
    add r1,r1,#4
    cmp r1,r5
    bne modify_cache
    
    mov r0,r0
    @bl CleanDcache
    @bl CleanUnifiedcache
    bl _clean_dcache
    bl cache_l2x0_clean_all
    
    bl cache_l2x0_disable
    @bl DataUnifiedCacheDisable

    ldr r4,_start_no_cache_mem
    ldr r5,_cache_size
    mov r1,#0
    @ldr r2,_data_aaaa
    mov r2,r7
    mov r0,#0
cmp_ok:
    ldr r3,[r4],#4
    cmp r2,r3
    bne error
    add r1,r1,#4
    cmp r1,r5
    bne cmp_ok
    beq exit
error:
    mov r0,#1  @ cache is error
    sub r1,r4,#4  @ error position
exit:
    msr	cpsr,r8
    pop {r4,r5,r6,r7,r8,pc}
   
@ function: L1_icache_bist_test
@ parameter:
@ this function uses bist control interface operates at a speed 
@ "slower" than the A9 clock, A9 is issuing the writes need to 
@ wait a really long time between each 0x2644 write. wait clock 
@ value don't reduce.

@int L1_icache_bist_test(int *pattern_addr,int pattern_count);
L1_icache_bist_test:
    push {r4,r5,r6,r7,r8,lr}
    mov r4,r0
    mov r5,r1
    
    @invalide icache
    bl _invalidate_icache
    
    @save status of icache 
    mrc p15,0,r0,c1,c0,0  @get status
    mov r6,r0
    @disable icache
    bic r0,r0,#1000          @ disable instruction cache
    mcr p15,0,r0,c1,c0,0  @ 
    
    @ clean dcache
    @bl _clean_dcache
    @bl _invalidate_dcache
    @bl _clean_invd_dcache
    
    @Wait for BIST to start
    ldr r3,bist_cache_result_addr
s_bist_i:
    ldr r2,[r3]
    cmp r2,#0
    bne s_bist_i
    
    mov r0,r4
    mov r1,#0
    @ldr r3,bist_cache_timing_addr
Li1:
    ldr r3,bist_cache_timing_addr
    ldr r2,[r0],#4
    str r2,[r3,#0]
    add r1,r1,#1
    
    @wait clock 
    mov r7,#0
i_wait_clock:
    ldr r3,bist_dummy_write_addr
    ldr r2,bist_dummy_data
    str r2,[r3]
    ldr r3,bist_dummy_write_addr
    ldr r2,bist_dummy_data
    str r2,[r3]
    add r7,r7,#1
    cmp r7,#0x10000
    bne i_wait_clock
    
    cmp r1,r5
    bne Li1
    
    @Wait for BIST to finish
    ldr r3,bist_cache_result_addr
e_bist_i:
    ldr r2,[r3]
    cmp r2,#0
    beq e_bist_i
    
    ldr r3,bist_cache_timing_addr
    mov r2,#3
    str r2,[r3,#0]
    
    @wait clock
    mov r7,#0
i_wait_clock_2:
    ldr r3,bist_dummy_write_addr
    ldr r2,bist_dummy_data
    str r2,[r3]
    ldr r3,bist_dummy_write_addr
    ldr r2,bist_dummy_data
    str r2,[r3]
    add r7,r7,#1
    cmp r7,#0x10000
    bne i_wait_clock_2
    
    ldr r3,bist_cache_result_addr
    ldr r2,[r3]
    and r2,r2,#0x20000
    cmp r2,#0x20000
    moveq r0,#1   @ icache error
    movne r0,#0   @ icache ok
    
    @restore status of icache
    mcr p15,0,r6,c1,c0,0  @ 
    
    pop {r4,r5,r6,r7,r8,pc}

@int L1_dcache_bist_test(int *pattern_addr,int pattern_count);
L1_dcache_bist_test:
    push {r4,r5,r6,r7,r8,lr}
    mov r4,r0
    mov r5,r1
    
    @invalide icache
    @bl _invalidate_icache
    
    @save status of icache 
    @mrc p15,0,r0,c1,c0,0  @get status
    @mov r6,r0
    @disable icache
    @bic r0,r0,#1000          @ disable instruction cache
    @mcr p15,0,r0,c1,c0,0  @ 
    
    @ clean dcache
    @bl _clean_dcache
    @bl _invalidate_dcache
    @bl _clean_invd_dcache
    
    @Wait for BIST to start
    ldr r3,bist_cache_result_addr
s_bist_d:
    ldr r2,[r3]
    cmp r2,#0
    bne s_bist_d
    
    mov r0,r4
    mov r1,#0
    @ldr r3,bist_cache_timing_addr
Ld1:
    ldr r3,bist_cache_timing_addr
    ldr r2,[r0],#4
    str r2,[r3,#0]
    add r1,r1,#1
    
    @wait clock 
    mov r7,#0
d_wait_clock:
    ldr r3,bist_dummy_write_addr
    ldr r2,bist_dummy_data
    str r2,[r3]
    ldr r3,bist_dummy_write_addr
    ldr r2,bist_dummy_data
    str r2,[r3]
    add r7,r7,#1
    cmp r7,#0x10000
    bne d_wait_clock
    
    cmp r1,r5
    bne Ld1
    
    @Wait for BIST to finish
    ldr r3,bist_cache_result_addr
e_bist_d:
    ldr r2,[r3]
    cmp r2,#0
    beq e_bist_d

    ldr r3,bist_cache_timing_addr
    mov r2,#3
    str r2,[r3,#0]
    
    @wait clock
    mov r7,#0
d_wait_clock_2:
    ldr r3,bist_dummy_write_addr
    ldr r2,bist_dummy_data
    str r2,[r3]
    ldr r3,bist_dummy_write_addr
    ldr r2,bist_dummy_data
    str r2,[r3]
    add r7,r7,#1
    cmp r7,#0x10000
    bne d_wait_clock_2
    
    @read back result
    ldr r3,bist_cache_result_addr
    ldr r2,[r3]
    and r2,r2,#0x4000
    cmp r2,#0x4000
    moveq r0,#1   @ dcache error
    movne r0,#0   @ dcache ok
    
    @restore status of icache
    @mcr p15,0,r6,c1,c0,0  @ 
    
    pop {r4,r5,r6,r7,r8,pc}




bist_cache_timing_addr:
    .word 0xC1109910
bist_cache_result_addr:
    .word 0xC1109914
bist_cache_read_result_delay:
    .word 0x1000
bist_dummy_write_addr:
    .word 0xC1107d3c
bist_dummy_data:
    .word 0xdeadbeef


_start_no_cache_mem:
    .word 0xa0000000
    
_start_cache_mem:
    .word 0x80000000
_cache_size:
#    .word 0x20000
    .word 0x40000
_data_5555:
    .word 0x55555555
_data_aaaa:
    .word 0xaaaaaaaa




