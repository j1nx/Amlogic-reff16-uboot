/*
 *  Copyright Codito Technologies (www.codito.com) 
 *
 *  cpu/arc/interrupts.c
 *
 *  Copyright (C) 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 * 
 * Authors : Sandeep Patil (sandeep.patil@codito.com)
 * 			 Pradeep Sawlani (pradeep.sawlani@codito.com)
 */
#include <common.h>
#include <asm/arcregs.h> 
#include <asm/arch/isahelper.h>
#include <asm/arch/irqs.h>

//#include <asm/irq.h>
int disable_interrupts(void)
{
#if 1
    
	unsigned int status;
	status = read_new_aux_reg(ARC_REG_STATUS32);
	status &= STATUS_DISABLE_INTERRUPTS;
/*	write_new_aux_reg(ARC_REG_STATUS32,status); */
	__asm__ __volatile__ (
		"FLAG %0"
		:
	       	:"r"(status)
		);
#else
    write_new_aux_reg(AUX_IENABLE,~(3<<3));
#endif		
	return 0;

}

void enable_interrupts(void)
{
	unsigned int status ;
	status = read_new_aux_reg(ARC_REG_STATUS32) | STATUS_E1_MASK | STATUS_E2_MASK;
	__asm__ __volatile__ (
		"FLAG %0"
		:
	       	:"r"(status)
		);
}
#define Jcc_INSTR_hi  0x2020
#define Jcc_INSTR_lo  0x0f80
#define _Interrupt1
#define _Interrupt2
typedef struct {
    unsigned short instruction_hi;
    unsigned short instruction_lo;
    unsigned short target_adr_hi;
    unsigned short target_adr_lo;
} vect_entry_type;
//#define INT_VECTOR_BASE 0x25
#define  MAX_ISA_IRQ_NUM	128
#define VECT_START read_new_aux_reg(AUX_INTR_VEC_BASE)
vect_entry_type vector_table[64] __attribute__ ((aligned (1024))) ;
struct irq_node {
    void               (*handler)(int, void *, struct pt_regs *);
    unsigned long      flags;
	int	disable_depth;
    void               *dev_id;
    const char         *devname;
    struct irq_node    *next;
    unsigned long mask;
    int vertor;
    int irq_number;	
    int hand_num;
};
typedef  struct irq_node irq_node_t;
#define NR_IRQS (sizeof(vector_table)/sizeof(vector_table[0]) - 1)
extern unsigned handle_int_lv2[];
extern unsigned handle_int_lv1[];
extern unsigned handle_exception[];
static irq_node_t node_pool[MAX_ISA_IRQ_NUM];
//common irq 
static irq_node_t* isa_gen_irq[32];
static irq_node_t *isa_gen1_irq[32];
static irq_node_t *isa_gpio_irq[32];
static irq_node_t *isa_arm_irq[32];

static irq_node_t* isa_gen_firq[32];
static irq_node_t *isa_gen1_firq[32];
static irq_node_t *isa_gpio_firq[32];
static irq_node_t *isa_arm_firq[32];



static irq_node_t* isa_irq_list[MAX_ISA_IRQ_NUM];

static void setvect(int vector,unsigned target)
{
    volatile vect_entry_type *vtab = (volatile vect_entry_type *)VECT_START;
    unsigned long target_as_int = (unsigned long)target;
	vtab[vector].instruction_hi = Jcc_INSTR_hi;
	vtab[vector].instruction_lo = Jcc_INSTR_lo;
	vtab[vector].target_adr_hi = target_as_int >> 16;
	vtab[vector].target_adr_lo = target_as_int & 0xffff;
}
#define PROCESS_ISA_NODE( mnode,node,status)\
do{int i=0;\
	while(i<32)\
	{\
		node=mnode[i],i++;\
		if((node)&&(status & node_pool[node->irq_number].mask)&&(node_pool[node->irq_number].handler))\
		{\
			node_pool[node->irq_number].handler(node->irq_number,NULL,NULL);\
		}\
	}	\
}while(0) 
void process_isa_interrupt(void *args)
{
	unsigned long status,mask;
	irq_node_t *node=NULL;

	
	status=read_irq_status();
	mask=get_irq_mask();
	if((status=status & mask)!=0)
		{
		clear_irq_status(status);
		PROCESS_ISA_NODE( isa_gen_irq,node,status);
		}
	status=read_irq_status1();
	mask=get_irq_mask1();
	if((status=status & mask)!=0)
		{
		clear_irq_status1(status);
		PROCESS_ISA_NODE( isa_gen1_irq,node,status);
		}
	status=read_gpio_irq_status();
	mask=get_gpio_irq_mask();
	if((status=status & mask)!=0)
		{
		clear_gpio_irq_status(status);
		PROCESS_ISA_NODE( isa_gpio_irq,node,status);
		}
	status=read_amrisc_irq_status();
	mask=get_amrisc_irq_mask();
	if((status=status & mask)!=0)
		{
		clear_amrisc_irq_status(status);
		PROCESS_ISA_NODE( isa_arm_irq,node,status);
		}
	return;
}
void process_isa_interrupt_fast(void *args)
{
	
}
int  init_interrupt(void)
{
   int i,addr;
   write_new_aux_reg(AUX_INTR_VEC_BASE,(unsigned)&vector_table[0]);
   write_new_aux_reg(AUX_IENABLE,0x0);//0xff disable all interrupt , will open in irq.c
   write_new_aux_reg(AUX_IRQ_LEV,0x0);
   setvect(0,(unsigned long)handle_exception);
   setvect(1,(unsigned long)handle_int_lv2);
   setvect(2,(unsigned long)handle_exception);
   for(i=3;i<0x20;i++)
   {
        setvect(i,(unsigned long)handle_int_lv1);
   }
   setvect(6,(unsigned long)handle_int_lv2); //handle fast irq
   for(i=0x20;i<0x40;i++)
        setvect(i,(unsigned long)handle_exception);
   irq_install_handler(5,process_isa_interrupt, NULL);
   irq_install_handler(6,process_isa_interrupt_fast,NULL);

   memset(isa_gen_irq,0,sizeof(int)*32);
   memset(isa_gen1_irq,0,sizeof(int)*32);
   memset(isa_gpio_irq,0,sizeof(int)*32);
   memset(isa_arm_irq,0,sizeof(int)*32);
   memset(isa_gen_firq,0,sizeof(int)*32);
   memset(isa_gen1_firq,0,sizeof(int)*32);
   memset(isa_gpio_firq,0,sizeof(int)*32);
   memset(isa_arm_firq,0,sizeof(int)*32);
   return 0;
}
void set_int_level(int vector, int level)
{
     unsigned level_table=read_new_aux_reg(AUX_IRQ_LEV);
     if(level)
     {
        setvect(vector,(unsigned long)handle_int_lv2);
        write_new_aux_reg(AUX_IRQ_LEV,level_table|(1<<vector));
     }
     else
     {
        setvect(vector,(unsigned long)handle_int_lv1);
        write_new_aux_reg(AUX_IRQ_LEV,level_table&(~(1<<vector)));
     }
     
}
void _setvect1(int vector, _Interrupt1 void (*target)(void)) {
    unsigned long temp;
    
    if(vector<3 || vector>31)
        return;
    if(vector!=1)
    {
        temp=target?target:handle_int_lv1;
        set_int_level(vector,0);
    }else{
        temp=target?target:handle_int_lv2;
    }
    setvect(vector, (unsigned long) temp);

}
void _setvect2(int vector, _Interrupt2 void (*target)(void)) {
   unsigned long temp;
    
    if(vector<3 || vector>31)
        return;
    if(vector!=1)
        set_int_level(vector,1);
    
    temp=target?target:handle_int_lv2;
    setvect(vector, (unsigned long)temp);
}
void _setexception(int vector, _Interrupt2 void (*target)(void)) {
    if(vector>2 && vector <31)
        return;
    if(vector==1)
        return;
    if(target)
        setvect(vector, (unsigned long) target);
    else
        setvect(vector, (unsigned long) handle_exception);
}
struct interrupt_action {
	interrupt_handler_t *handler;
	void *arg;
};
static struct interrupt_action  irq_vecs[64]; 
void process_interrupt(struct pt_regs * pt)
{
    int vec_num=0;
    int status=read_new_aux_reg(ARC_REG_STATUS32);
    if((status&(STATUS_A1_MASK|STATUS_A2_MASK|STATUS_AE_MASK))==0)
        while(1); // wrong status 
    if(status&STATUS_A1_MASK)
        vec_num=(read_new_aux_reg(AUX_ICAUSE1))&0x1f;
    if(status&STATUS_A2_MASK)
        vec_num=(read_new_aux_reg(AUX_ICAUSE2))&0x1f;
    if(status&STATUS_AE_MASK)
        vec_num=(read_new_aux_reg(ARC_REG_ECR)>>16)&0xff;
    if(irq_vecs[vec_num].handler==0)
        while(1); // wrong status , func not set
    void * arg=irq_vecs[vec_num].arg?irq_vecs[vec_num].arg:pt;
    irq_vecs[vec_num].handler(arg);
    return ;
}
/************************************************************************/
/*
 * Install and free an interrupt handler
 */
void irq_install_handler (int vec, interrupt_handler_t * handler, void *arg)
{
	if ((vec < 0) || (vec > NR_IRQS)) {
		printf ("irq_install_handler: wrong interrupt vector %d\n",
			vec);
		return;
	}

	irq_vecs[vec].handler = handler;
	irq_vecs[vec].arg = arg;
}

void irq_free_handler (int vec)
{
	if ((vec < 0) || (vec > NR_IRQS)) {
		return;
	}

	irq_vecs[vec].handler = NULL;
	irq_vecs[vec].arg = NULL;
}
static int setup_isa_irq(unsigned int irq, irq_node_t *node)
{
	int res=0;
	unsigned long flags;
	struct irq_node_queue * curr;
	struct irq_node_queue *new_queue_item;

	if(irq <CORE_IRQ || irq >AM_ISA_AMRISC_IRQ_MAX())
	{
		return -1;
	}
	
	if(irq<AM_ISA_GEN_IRQ_MAX())
	{
		if(node->flags &IRQ_ISA_FAST)
		{
			isa_gen_firq[irq-CORE_IRQ]=node;
		}
		else
		{
			isa_gen_irq[irq-CORE_IRQ]=node;	
		}
		node->mask=1<<(irq-CORE_IRQ);
	}
	else if(irq<AM_ISA_GEN1_IRQ_MAX())
	{
		if(node->flags &IRQ_ISA_FAST)
		{
			isa_gen1_firq[irq-AM_ISA_GEN_IRQ_MAX()]=node;
		}
		else
		{
			isa_gen1_irq[irq-AM_ISA_GEN_IRQ_MAX()]=node;	
		}
		node->mask=1<<(irq-AM_ISA_GEN_IRQ_MAX());
	}
	else if(irq<AM_ISA_GEN2_IRQ_MAX())
	{
		if(node->flags &IRQ_ISA_FAST)
		{
			isa_gpio_firq[irq-AM_ISA_GEN1_IRQ_MAX()]=node;
		}
		else
		{
			isa_gpio_irq[irq-AM_ISA_GEN1_IRQ_MAX()]=node;	
		}
		node->mask=1<<(irq-AM_ISA_GEN1_IRQ_MAX());
	} 
	else if(irq<AM_ISA_AMRISC_IRQ_MAX())
	{
		if(node->flags &IRQ_ISA_FAST)
		{
			isa_arm_firq[irq-AM_ISA_GEN2_IRQ_MAX()]=node;
		}
		else
		{
			isa_arm_irq[irq-AM_ISA_GEN2_IRQ_MAX()]=node;	
		}
		node->mask=1<<(irq-AM_ISA_GEN2_IRQ_MAX());
	}
	return res;	
}
static void  enable_isa_interrupt(unsigned int irq,unsigned long flags)
{
	    // printk("fast irq & slow irq flags=0x%x\r\n",flags);
	     if(irq>=AM_ISA_GEN_IRQ(0) && irq<AM_ISA_GEN_IRQ_MAX())	
	     	{
	     		set_irq_mask((1<<(irq-AM_ISA_GEN_IRQ(0))));
	     		if(flags &IRQ_ISA_FAST)
	     		set_firq_mask((1<<(irq-AM_ISA_GEN_IRQ(0))));			 
	     		
	     	}
	        else if(irq>=AM_ISA_GEN1_IRQ(0) && irq<AM_ISA_GEN1_IRQ_MAX())	
	     	{
			set_irq_mask1(1<<(irq-AM_ISA_GEN1_IRQ(0)));	
			if(flags &IRQ_ISA_FAST)
			set_firq_mask1(1<<(irq-AM_ISA_GEN1_IRQ(0)));	
			 
			
	     	}
	     else if(irq>=AM_ISA_GEN2_IRQ(0) && irq<AM_ISA_GEN2_IRQ_MAX())	
	     	{
			set_gpio_irq_mask(1<<(irq-AM_ISA_GEN2_IRQ(0)));	
			if(flags &IRQ_ISA_FAST)
			set_gpio_firq_mask(1<<(irq-AM_ISA_GEN2_IRQ(0)));	
			 
			
	     	}
	     else if(irq>=AM_ISA_AMRISC_IRQ(0) && irq<AM_ISA_AMRISC_IRQ_MAX())	
	     	{

			set_amrisc_irq_mask(1<<(irq-AM_ISA_AMRISC_IRQ(0)));
			if(flags &IRQ_ISA_FAST)	
			set_amrisc_firq_mask(1<<(irq-AM_ISA_AMRISC_IRQ(0)));
			
	     	}

}
static void disable_isa_interrupt(int irq)  
{
	     if(irq>=AM_ISA_GEN_IRQ(0) && irq<AM_ISA_GEN_IRQ_MAX() )	
	     	{
	     		clear_firq_mask(1<<(irq-AM_ISA_GEN_IRQ(0)));
	     		clear_irq_mask(1<<(irq-AM_ISA_GEN_IRQ(0)));
	     	}

	        else if(irq>=AM_ISA_GEN1_IRQ(0) && irq<AM_ISA_GEN1_IRQ_MAX())	
	     	{
			clear_irq_mask1(1<<(irq-AM_ISA_GEN1_IRQ(0)));	
			clear_firq_mask1(1<<(irq-AM_ISA_GEN1_IRQ(0)));	
	        }
	     else if(irq>=AM_ISA_GEN2_IRQ(0) && irq<AM_ISA_GEN2_IRQ_MAX())
	     	{
	     		clear_gpio_firq_mask( 1<<(irq-AM_ISA_GEN2_IRQ(0)));
	     		clear_gpio_irq_mask( 1<<(irq-AM_ISA_GEN2_IRQ(0)));
	     	}
	     else if(irq>=AM_ISA_AMRISC_IRQ(0) && irq<AM_ISA_AMRISC_IRQ_MAX() )
	     	{
	     		clear_amrisc_firq_mask(1<<(irq-AM_ISA_AMRISC_IRQ(0)));
	     		clear_amrisc_irq_mask(1<<(irq-AM_ISA_AMRISC_IRQ(0)));
	     	}
} 

static int free_isa_irq(unsigned int irq)
{
    //unsigned long flags;
    disable_isa_interrupt(irq);
    return 0;
}
int request_irq(unsigned int irq, 
       interrupt_handler_t  handle,
        unsigned long flags, const char *name, void *dev_id)
{
  	irq_node_t *node=&node_pool[irq];
    	int retval=0;

    	node->handler = handle;
	node->flags = flags;
	node->irq_number=irq;


	/* insert the new irq registered into the irq list */

	retval = setup_isa_irq(irq,node);
	if(irq < CORE_IRQ)
	{
	    	unmask_interrupt((1<<irq));
	}
	else
	{
		    int intbit = 1<<5;
			if(node->flags &IRQ_ISA_FAST)
				intbit |= 1<<6;
		    unmask_interrupt(intbit);	
			enable_isa_interrupt(irq,node->flags);
	}	
	return retval;
}
void free_irq(unsigned int irq, void *dev_id)
{
	irq_node_t *node=&node_pool[irq];

	node->handler=NULL;
	node->flags=0;
	node->irq_number=0;

	free_isa_irq(irq);
	return;
}
