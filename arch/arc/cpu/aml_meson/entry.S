
#include <asm/ptrace.h>
.cpu A7
; ---------------------------------------------
    .section .text, "ax",@progbits
;##################### Interrupt Handling ##############################
#define SYMBOL_NAME_LABEL(X) X##:
#define SYMBOL_NAME_STR(X) #X
#define SYMBOL_NAME(X) X
.macro PUSH_G_REG reg
	mov     r0,\reg
	push_s  r0
.endm
.macro POP_G_REG reg
	pop_s   r0
	mov		\reg,r0
.endm 
.macro PUSH_A_REG reg
	lr		r0,[\reg]
	push_s  r0
.endm 
.macro POP_A_REG reg
	pop_s   r0
	sr		r0,[\reg]
.endm 
.macro PUSH_S_REGS
	push_s  %blink
    push_s  %r0
    push_s  %r1
    push_s  %r2
    push_s  %r3
    push_s  %r12
    push_s  %r13
    push_s  %r14
    push_s  %r15
.endm
.macro POP_S_REGS
	pop_s 	%r15
    pop_s 	%r14
    pop_s 	%r13
    pop_s 	%r12
    pop_s 	%r3
    pop_s 	%r2
    pop_s 	%r1
    pop_s 	%r0
    pop_s 	%blink
.endm
.macro PUSH_REGS 
	PUSH_S_REGS
    PUSH_G_REG  %r4
    PUSH_G_REG  %r5
    PUSH_G_REG  %r6
    PUSH_G_REG  %r7
    PUSH_G_REG  %r8
    PUSH_G_REG  %r9
    PUSH_G_REG  %r10
    PUSH_G_REG  %r11
    PUSH_G_REG  %r16
    PUSH_G_REG  %r17
    PUSH_G_REG  %r18
    PUSH_G_REG  %r19
    PUSH_G_REG  %r20
    PUSH_G_REG  %r21
    PUSH_G_REG  %r22
    PUSH_G_REG  %r23
    PUSH_G_REG  %r24
    PUSH_G_REG  %r25
    PUSH_G_REG  %gp
    PUSH_G_REG  %fp

    PUSH_G_REG  lp_count
	PUSH_A_REG	lp_start
    PUSH_A_REG  lp_end
.endm
.macro POP_REGS
	POP_A_REG   lp_end
	POP_A_REG   lp_start
	POP_G_REG   lp_count
	POP_G_REG   fp
	POP_G_REG   gp
	POP_G_REG   r25
	POP_G_REG   r24
	POP_G_REG   r23
	POP_G_REG   r22
	POP_G_REG   r21
	POP_G_REG   r20
	POP_G_REG   r19
	POP_G_REG   r18
	POP_G_REG   r17
	POP_G_REG   r16
	POP_G_REG   r11
	POP_G_REG   r10
	POP_G_REG   r9
	POP_G_REG   r8
	POP_G_REG   r7
	POP_G_REG   r6
	POP_G_REG   r5
	POP_G_REG   r4
	POP_S_REGS
.endm
.macro SAVE_SP 
	mov r0,sp
	sub_s r0,r0,4
	push_s r0
.endm
.macro LOAD_SP 
	pop_s r0
	add_s r0,r0,4
	mov sp,r0
.endm
;	.global	interrupt_vec_def
;	.type	interrupt_vec_def, @function
;SYMBOL_NAME_LABEL(interrupt_vec_def)
.global	handle_int_lv1
.type	handle_int_lv1, @function
.align 4
handle_int_lv1:
    PUSH_REGS
    PUSH_A_REG	bta_l1
    PUSH_G_REG  ilink1
    PUSH_A_REG	status32_l1
    SAVE_SP
    
    bl  SYMBOL_NAME(process_interrupt)
    LOAD_SP
    POP_A_REG	status32_l1
   	POP_G_REG  	ilink1
    POP_A_REG	bta_l1
    POP_REGS
    rtie
.size   handle_int_lv1,. - handle_int_lv1
.global	handle_int_lv2
.type	handle_int_lv2, @function
.align 4
handle_int_lv2:
    PUSH_REGS
    PUSH_A_REG	bta_l2
    PUSH_G_REG  ilink2
    PUSH_A_REG	status32_l2
    SAVE_SP
    bl  SYMBOL_NAME(process_interrupt)
    LOAD_SP
    POP_A_REG	status32_l2
   	POP_G_REG  	ilink2
    POP_A_REG	bta_l2
    POP_REGS
    rtie
.size   handle_int_lv2,. - handle_int_lv2   
.global	handle_exception
.type	handle_exception, @function
.align 4
handle_exception:
    PUSH_REGS
    PUSH_A_REG	erbta
    PUSH_A_REG  eret
    PUSH_A_REG	erstatus
    SAVE_SP
    bl  SYMBOL_NAME(process_interrupt)
    LOAD_SP
    POP_A_REG	erstatus
   	POP_A_REG  	eret
    POP_A_REG	erbta
    POP_REGS
    rtie
.size   handle_exception,. - handle_exception

.global	handle_setjmp 
.align  4            
.type	handle_setjmp, @function  
handle_setjmp:
	st sp		,[r0,PT_sp         ]
	
;	st status32 ,[r0,PT_status32   ]
;	st ret		,[r0,PT_ret		  ]
;	st bta		,[r0,PT_bta		  ]
	
	st lp_count,[r0,PT_lp_count   ]
	st fp      ,[r0,PT_fp         ]
	st gp      ,[r0,PT_gp         ]
	st r25     ,[r0,PT_r25        ]
	st r24     ,[r0,PT_r24        ]
	st r23     ,[r0,PT_r23        ]
	st r22     ,[r0,PT_r22        ]
	st r21     ,[r0,PT_r21        ]
	st r20     ,[r0,PT_r20        ]
	st r19     ,[r0,PT_r19        ]
	st r18     ,[r0,PT_r18        ]
	st r17     ,[r0,PT_r17        ]
	st r16     ,[r0,PT_r16        ]
	st r11     ,[r0,PT_r11        ]
	st r10     ,[r0,PT_r10        ]
	st r9      ,[r0,PT_r9         ]
	st r8      ,[r0,PT_r8         ]
	st r7      ,[r0,PT_r7         ]
	st r6      ,[r0,PT_r6         ]
	st r5      ,[r0,PT_r5         ]
	st r4      ,[r0,PT_r4         ]
	st r15     ,[r0,PT_r15        ]
	st r14     ,[r0,PT_r14        ]
	st r13     ,[r0,PT_r13        ]
	st r12     ,[r0,PT_r12        ]
	st r3      ,[r0,PT_r3         ]
	st r2      ,[r0,PT_r2         ]
	st r1      ,[r0,PT_r1         ]
	st r0      ,[r0,PT_r0         ]
	st blink   ,[r0,PT_blink      ]
	lr r1      ,[lp_end]
	st r1  ,[r0,PT_lp_end     ]
	lr r1,[lp_start]
	st r1,[r0,PT_lp_start   ]
	ld r1      ,[r0,PT_r1         ]
	j.d [blink]
	mov_s r0,0
.size   handle_setjmp,. - handle_setjmp	
.global	handle_longjmp  
.align  4           
.type	handle_longjmp, @function  
handle_longjmp:
	ld sp		,[r0,PT_sp         ]
	
;	st status32 ,[r0,PT_status32   ]
;	st ret		,[r0,PT_ret		  ]
;	st bta		,[r0,PT_bta		  ]
	
	ld r2,[r0,PT_lp_count   ]
	mov lp_count,r2
	ld fp      ,[r0,PT_fp         ]
	ld gp      ,[r0,PT_gp         ]
	ld r25     ,[r0,PT_r25        ]
	ld r24     ,[r0,PT_r24        ]
	ld r23     ,[r0,PT_r23        ]
	ld r22     ,[r0,PT_r22        ]
	ld r21     ,[r0,PT_r21        ]
	ld r20     ,[r0,PT_r20        ]
	ld r19     ,[r0,PT_r19        ]
	ld r18     ,[r0,PT_r18        ]
	ld r17     ,[r0,PT_r17        ]
	ld r16     ,[r0,PT_r16        ]
	ld r11     ,[r0,PT_r11        ]
	ld r10     ,[r0,PT_r10        ]
	ld r9      ,[r0,PT_r9         ]
	ld r8      ,[r0,PT_r8         ]
	ld r7      ,[r0,PT_r7         ]
	ld r6      ,[r0,PT_r6         ]
	ld r5      ,[r0,PT_r5         ]
	ld r4      ,[r0,PT_r4         ]
	ld r15     ,[r0,PT_r15        ]
	ld r14     ,[r0,PT_r14        ]
	ld r13     ,[r0,PT_r13        ]
	ld r12     ,[r0,PT_r12        ]
	ld r3      ,[r0,PT_r3         ]
	ld blink   ,[r0,PT_blink      ]
	mov_s   r2,r0
	mov_s	r0,r1
	ld r1  	   ,[r2,PT_lp_end     ]
	sr r1	   ,[lp_end]
	ld r1      ,[r2,PT_lp_start   ]
	sr r1      ,[lp_start]
	
	ld r1      ,[r2,PT_r1         ]
	ld r2      ,[r2,PT_r2         ]
	j 			[blink]
.size   handle_longjmp,. - handle_longjmp		
    


	
