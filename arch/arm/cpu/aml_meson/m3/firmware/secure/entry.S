#include <config.h>             
    	   
.globl secure_entry_point        	    
secure_entry_point:
		@ Save some stuff on the OS stack (preserve 8 byte alignment)
        push	{r4-r12, lr}		
        @---------------------------------------
        @save A9 registers all mode
        mrs r4, spsr    
        push {r4}   
        
         cps #1      @switch to FIRQ mode
        mov r4,sp   
        mov r5,lr   
        mrs r6,spsr     
        cps #3      @switch to SVC mode
        push {r4,r5,r6}     
        
        cps #1      @switch to FIRQ mode
        mov r4,r8   
        mov r5,r9   
        mov r6,r10  
        mov r7,r11  
        cps #3      @switch to SVC mode
        push {r4,r5,r6,r7}  
        cps #1      @switch to FIRQ mode
        mov r4,r12  
        cps #3      @switch to SVC mode
        push {r4}   
        
        
        cps #2      @switch to IRQ mode
        mov r4,sp   
        mov r5,lr   
        mrs r6,spsr     
        cps #3      @switch to SVC mode
        push {r4,r5,r6}     
        
        cps #6      @switch to monitor user mode
        mov r4,sp   
        mov r5,lr   
        mrs r6,spsr     
        cps #3      @switch to SVC mode
        push {r4,r5,r6}     
        
        cps #6      @switch to FIRQ mode
        mov r4,r8   
        mov r5,r9   
        mov r6,r10  
        mov r7,r11  
        cps #3      @switch to SVC mode
        push {r4,r5,r6,r7}  
        cps #6      @switch to FIRQ mode
        mov r4,r12  
        cps #3      @switch to SVC mode
        push {r4}   
                
        cps #7      @switch to Abort mode
        mov r4,sp   
        mov r5,lr   
        mrs r6,spsr     
        cps #3      @switch to SVC mode
        push {r4,r5,r6}             @ Preserve arguments
        
        cps #0Xb    @switch to undef mode
        mov r4,sp   
        mov r5,lr   
        mrs r6,spsr     
        cps #3      @switch to SVC mode
        push {r4,r5,r6}             @ Preserve arguments
        
        cps #0Xf    @switch to sys mode
        mov r4,sp   
        mov r5,lr   
        mrs r6,spsr     
        cps #3      @switch to SVC mode
        push {r4,r5,r6}             @ Preserve arguments
        
        @ Disable interrupts
        mrs	r7, CPSR    
        cpsid	if    	
		mov	r6, sp    
				
		ldr r4, [sp, #0x9c]
		ldr r5, [sp, #0xa0]
		
		ldr sp, =SECURE_STACK_END				
		 push	{r4-r7}		  @ see comment in appf_reset_entry_point below
		 bl secure_runtime_call

secure_return_to_os:              		 
		  @ retrieve OS stack pointer, TTBR0, TTBCR, DACR
        pop	{r4-r7}     
        
        @ Switch stacks
        mov	sp, r6     
        mov	r6, #0     
        @ Restore interrupts
        msr	CPSR_c, r7
        
               @ Return to OS
        
        @--------------------------------
        @restore A9 registers
        pop {r4,r5,r6}  
        cps #0Xf    @switch to sys mode
        mov sp,r4   
        mov lr,r5   
        msr spsr,r6     
        cps #3      @switch to SVC mode	
        
        pop {r4,r5,r6}  
        cps #0Xb    @switch to undef mode
        mov sp,r4   
        mov lr,r5   
        msr spsr,r6     
        cps #3      @switch to SVC mode	
        
        pop {r4,r5,r6}  
        cps #7      @switch to Abort mode
        mov sp,r4   
        mov lr,r5   
        msr spsr,r6     
        cps #3      @switch to SVC mode	
        
        pop {r4}    
        cps #6      @switch to monitor user mode
        mov r12,r4  
        cps #3      @switch to SVC mode
        
        pop {r4,r5,r6,r7}   
        cps #6      @switch to monitor user mode
        mov r8,r4   
        mov r9,r5   
        mov r10,r6   
        mov r11,r7  
        cps #3      @switch to SVC mode
        
        pop {r4,r5,r6}  
        cps #6      @switch to monitor user mode
        mov sp,r4   
        mov lr,r5   
        msr spsr,r6     
        cps #3      @switch to SVC mode	
        
        pop {r4,r5,r6}  
        cps #2      @switch to IRQ mode
        mov sp,r4   
        mov lr,r5   
        msr spsr,r6     
        cps #3      @switch to SVC mode	
        
        pop {r4}    
        cps #1      @switch to FIRQ mode
        mov r12,r4  
        cps #3      @switch to SVC mode
        
        pop {r4,r5,r6,r7}   
        cps #1      @switch to FIRQ mode
        mov r8,r4   
        mov r9,r5   
        mov r10,r6   
        mov r11,r7  
        cps #3      @switch to SVC mode
        
        pop {r4,r5,r6}  
        cps #1      @switch to FIRQ mode
        mov sp,r4   
        mov lr,r5   
        msr spsr,r6     
        cps #3      @switch to SVC mode	
        
        pop {r4}    
        msr spsr,r4     
        
        pop	{r4-r12, pc}    
        
        .end