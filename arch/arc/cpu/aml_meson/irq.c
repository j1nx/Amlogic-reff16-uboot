/******************************************************************************
 * Copyright ARC International (www.arc.com) 2007-2008
 *
 * Vineetg: Mar 2009 (Supporting 2 levels of Interrupts)
 *  -local_irq_enable shd be cognizant of current IRQ state
 *    It is lot more involved now and thus re-written in "C"
 *  -set_irq_regs in common ISR now always done and not dependent 
 *      on CONFIG_PROFILEas it is used by
 *
 * Vineetg: Jan 2009
 *  -Cosmetic change to display the registered ISR name for an IRQ
 *  -free_irq( ) cleaned up to not have special first-node/other node cases
 *
 * Vineetg: June 17th 2008
 *  -Added set_irq_regs() to top level ISR for profiling 
 *  -Don't Need __cli just before irq_exit(). Intr already disabled
 *  -Disabled compile time ARC_IRQ_DBG
 *
 *****************************************************************************/
/******************************************************************************
 * Copyright Codito Technologies (www.codito.com) Oct 01, 2004
 * 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *****************************************************************************/
 
/*******************************************************************************
* modified history:																						
*
*	modify by zhouzhi for amlogic  isa interrupts;
**	wangjf@amlogic  support  irq share   09/01/16
*      porting to u-boot by ping gao 5/21/09
*
**************************************************************************/



/*
 * arch/arcnommu/kernel/irq.c
 *
 */

#if 0
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/hardirq.h>
#include <linux/irqflags.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#endif /* 0 */
#include <common.h>

#include <asm/system.h>
//#include <asm/signal.h>
#include <asm/errno.h>
#include <asm/arcregs.h>
#include <asm/arch/am_regs.h>
#include <asm/irq.h>

#include <malloc.h>

//FIXME ::
#ifdef CONFIG_DEBUG_KERNEL
#define ARC_IRQ_DBG
#endif
/* table for system interrupt handlers */


struct  irq_node_queue{
int    irq_number;
struct  irq_node_queue *next;
};


static struct irq_node *irq_list[AM_ISA_IRQ_MAX];
static struct irq_node_queue *isa_gen_irq ;
#if defined(CONFIG_AMLOGIC_BOARD_APOLLO)
static struct irq_node_queue *isa_gen1_irq ;
static struct irq_node_queue *isa_gen1_firq ;
#endif
static struct irq_node_queue *isa_gpio_irq;
static struct irq_node_queue *isa_arm_irq;

static  struct irq_node_queue  *isa_gen_firq;
static  struct irq_node_queue  *isa_gpio_firq;
static  struct irq_node_queue  *isa_arm_firq;


/*no PREEMPT in bootloader*/

#define __LOCK(lock) \
  do { } while (0)  // preempt_disable(); __acquire(lock); (void)(lock);
  
#define __UNLOCK(lock) \
        do {  } while (0) //preempt_enable(); __release(lock); (void)(lock);

#define __LOCK_IRQSAVE(lock, flags) \
  do { local_irq_save(flags); __LOCK(lock); } while (0)
  
#define __UNLOCK_IRQRESTORE(lock, flags) \
        do { local_irq_restore(flags); __UNLOCK(lock); } while (0)

#define _spin_lock_irqsave(lock, flags)		__LOCK_IRQSAVE(lock, flags)

#define _spin_unlock_irqrestore(lock, flags)	__UNLOCK_IRQRESTORE(lock, flags)

#define spin_lock_irqsave(lock, flags)	_spin_lock_irqsave(lock, flags)

#define spin_unlock_irqrestore(lock, flags) \
					_spin_unlock_irqrestore(lock, flags)

typedef int	spinlock_t;
/* IRQ status spinlock - enable, disable */
static spinlock_t irq_controller_lock;



#define kmalloc(size, flags)	malloc(size)
#define kzalloc(size, flags)	calloc(size, 1)
#define vmalloc(size)			malloc(size)
#define kfree(ptr)				free(ptr)
#define vfree(ptr)				free(ptr)

#define printk printf

#if 0

/*
 * Per-cpu current frame pointer - the location of the last exception frame on
 * the stack 
 */

 #define DEFINE_PER_CPU(type, name) \
    __typeof__(type) per_cpu__##name

#define per_cpu(var, cpu)			(*((void)(cpu), &per_cpu__##var))
#define __get_cpu_var(var)			per_cpu__##var
#define __raw_get_cpu_var(var)			per_cpu__##var



#define DECLARE_PER_CPU(type, name) extern __typeof__(type) per_cpu__##name

#define EXPORT_PER_CPU_SYMBOL(var) EXPORT_SYMBOL(per_cpu__##var)
#define EXPORT_PER_CPU_SYMBOL_GPL(var) EXPORT_SYMBOL_GPL(per_cpu__##var)
DECLARE_PER_CPU(struct pt_regs *, __irq_regs);

static inline struct pt_regs *get_irq_regs(void)
{
	return __get_cpu_var(__irq_regs);
}

static inline struct pt_regs *set_irq_regs(struct pt_regs *new_regs)
{
	struct pt_regs *old_regs, **pp_regs = &__get_cpu_var(__irq_regs);

	old_regs = *pp_regs;
	*pp_regs = new_regs;
	return old_regs;
}


//extern void smp_ipi_init(void);



void arc_irq_init(void)
{
    extern int _int_vec_base_lds;

    /* set the base for the interrupt vector tabe as defined in Linker File
       Interrupts will be enabled in start_kernel
     */
     printk("arc irq init \r\n");
    write_new_aux_reg(AUX_INTR_VEC_BASE, &_int_vec_base_lds);

    /* vineetg: Jan 28th 2008
       Disable all IRQs on CPU side
       We will selectively enable them as devices request for IRQ
     */
    write_new_aux_reg(AUX_IENABLE, 0);
		write_new_aux_reg(AUX_IRQ_LEV, 0);//clear all the interrupts to level 1
#ifdef CONFIG_ARCH_ARC_LV2_INTR
{
    int level_mask = 0;
    /* If any of the peripherals is Level2 Interrupt (high Prio), 
       set it up that way
     */
#ifdef  CONFIG_TIMER_LV2
    level_mask |= 0x8;
#endif

#ifdef  CONFIG_IRQ5_LV2
    level_mask |= 0x20;
#endif

#ifdef  CONFIG_IRQ6_LV2
    level_mask |= 0x40;
#endif

    if (level_mask) {
        printk("setup as level-2 interrupt/s \n");
        write_new_aux_reg(AUX_IRQ_LEV, level_mask);
    }
}
#endif

}

#endif /* 0 */

/****
char *devname = "Timer Interrupt";    
 request_irq(26, timer_handler,IRQ_FLG_LOCK, devname, devname);
 enable_interrupts();
 int timer_handler(int irq, void *b,struct pt_regs * fp)
 {
     static int i;
     i++;
     if(i%1000 == 0)
         printf("interrupt work \n");
 
     return 0;
 }
 
 static struct irq_node timer_intr = { 
     timer_handler, 
     IRQ_FLG_LOCK, 
     0,
     NULL,
     "Timer Interrupt", 
     NULL
 };
 void timeA_init()
 {
     CLEAR_ISA_REG_MASK(IREG_TIMER_BASE, TIMER_A_INPUT_MASK);
     SET_ISA_REG_MASK(IREG_TIMER_BASE, (TIMER_UNIT_1ms << TIMER_A_INPUT_BIT));
     WRITE_ISA_REG(IREG_TIMER_A_COUNT, 0);           
 
// }

 ***/
 
static void  enable_isa_interrupt(unsigned int irq,unsigned long flags)
	{
	    // printk("fast irq & slow irq flags=0x%x\r\n",flags);
	     if(irq>=AM_ISA_GEN_IRQ(0) && irq<AM_ISA_GEN_IRQ_MAX())	
	     	{
	     		set_irq_mask((1<<(irq-AM_ISA_GEN_IRQ(0))));
	     		if(flags &IRQ_ISA_FAST)
	     		set_firq_mask((1<<(irq-AM_ISA_GEN_IRQ(0))));			 
	     		
	     	}
	     #if defined(CONFIG_AMLOGIC_BOARD_APOLLO)
	        else if(irq>=AM_ISA_GEN1_IRQ(0) && irq<AM_ISA_GEN1_IRQ_MAX())	
	     	{
			set_irq_mask1(1<<(irq-AM_ISA_GEN1_IRQ(0)));	
			if(flags &IRQ_ISA_FAST)
			set_firq_mask1(1<<(irq-AM_ISA_GEN1_IRQ(0)));	
			 
			
	     	}
	     #endif
	     else if(irq>=AM_ISA_GPIO_IRQ(0) && irq<AM_ISA_GPIO_IRQ_MAX())	
	     	{
			set_gpio_irq_mask(1<<(irq-AM_ISA_GPIO_IRQ(0)));	
			if(flags &IRQ_ISA_FAST)
			set_gpio_firq_mask(1<<(irq-AM_ISA_GPIO_IRQ(0)));	
			 
			
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
	     #if defined(CONFIG_AMLOGIC_BOARD_APOLLO)
	        else if(irq>=AM_ISA_GEN1_IRQ(0) && irq<AM_ISA_GEN1_IRQ_MAX())	
	     	{
			clear_irq_mask1(1<<(irq-AM_ISA_GEN1_IRQ(0)));	
			clear_firq_mask1(1<<(irq-AM_ISA_GEN1_IRQ(0)));	
	     	}
	     #endif
	     else if(irq>=AM_ISA_GPIO_IRQ(0) && irq<AM_ISA_GPIO_IRQ_MAX())
	     	{
	     		clear_gpio_firq_mask( 1<<(irq-AM_ISA_GPIO_IRQ(0)));
	     		clear_gpio_irq_mask( 1<<(irq-AM_ISA_GPIO_IRQ(0)));
	     	}
	     else if(irq>=AM_ISA_AMRISC_IRQ(0) && irq<AM_ISA_AMRISC_IRQ_MAX() )
	     	{
	     		clear_amrisc_firq_mask(1<<(irq-AM_ISA_AMRISC_IRQ(0)));
	     		clear_amrisc_irq_mask(1<<(irq-AM_ISA_AMRISC_IRQ(0)));
	     	}
} 

#define __init

/* initialise the irq table */
void __init init_IRQ(void)
{
	int i;

	for (i = 0; i < AM_ISA_IRQ_MAX; i++) {
		irq_list[i] = NULL;
	}
#ifdef CONFIG_SMP
    smp_ipi_init();
#endif
	isa_gen_irq=NULL;
	#if defined(CONFIG_AMLOGIC_BOARD_APOLLO)
	isa_gen1_irq=NULL;
	isa_gen1_firq=NULL;
	#endif
	isa_gpio_irq=NULL;
	isa_arm_irq=NULL;
	isa_gen_firq=NULL;
	isa_gpio_firq=NULL;
	isa_arm_firq=NULL;

}

/* setup the irq, insert the irq node into the list */
#define ISA_ADD_NODE(mnode,curr,node)		do{\
				if(mnode==NULL){\
					mnode=node;\
				}else\
					{\
					curr = mnode;\
					while (curr->next)	\
					curr = curr->next;\
					curr->next = node;\
				}\
			}while(0)	
int setup_isa_irq(unsigned int irq, struct irq_node *node)
{
	int res=0;
	unsigned long flags;
	struct irq_node_queue * curr;
	struct irq_node_queue *new_queue_item;

	if(irq >=CORE_IRQ && irq<AM_ISA_AMRISC_IRQ_MAX())
	{
		new_queue_item=(struct irq_node_queue *)kmalloc(sizeof(struct irq_node_queue),0);
		new_queue_item->irq_number=irq;
		new_queue_item->next=NULL;
	}
	spin_lock_irqsave(&irq_controller_lock, flags); /* insert atomically */
		if(irq<CORE_IRQ)
			{
			node->mask=1<<irq;
			}
		else if(irq<AM_ISA_GEN_IRQ_MAX())
			{
				if(node->flags &IRQ_ISA_FAST)
				{
					ISA_ADD_NODE(isa_gen_firq,curr,new_queue_item);		
				}
				else
				{
					ISA_ADD_NODE(isa_gen_irq,curr,new_queue_item);	
				}
			node->mask=1<<(irq-CORE_IRQ);
			}
		#if defined(CONFIG_AMLOGIC_BOARD_APOLLO)	
		else if(irq<AM_ISA_GEN1_IRQ_MAX())
			{
				if(node->flags &IRQ_ISA_FAST)
				{
					ISA_ADD_NODE(isa_gen1_firq,curr,new_queue_item);
				}
				else
				{
					ISA_ADD_NODE(isa_gen1_irq,curr,new_queue_item);
				}
			node->mask=1<<(irq-AM_ISA_GEN_IRQ_MAX());
			}
		#endif	
		else if(irq<AM_ISA_GPIO_IRQ_MAX())
			{
				if(node->flags &IRQ_ISA_FAST)
				{
					ISA_ADD_NODE(isa_gpio_firq,curr,new_queue_item);
				}
				else
				{
					ISA_ADD_NODE(isa_gpio_irq,curr,new_queue_item);
				}
				node->mask=1<<(irq-AM_ISA_GEN_IRQ_MAX());
			} 
		else if(irq<AM_ISA_AMRISC_IRQ_MAX())
			{
				if(node->flags &IRQ_ISA_FAST)
				{
					ISA_ADD_NODE(isa_arm_firq,curr,new_queue_item);
				}
				else
				{
					ISA_ADD_NODE(isa_arm_irq,curr,new_queue_item);
				}
				node->mask=1<<(irq-AM_ISA_GPIO_IRQ_MAX());
			}
	spin_unlock_irqrestore(&irq_controller_lock, flags); /* insert atomically */
	return res;	
}


#define ISA_FREE_NODE(mnode,curr,node,irq,dev_id) do{\
				node = curr = mnode;\
				if (curr->irq_number == irq ) {\
					mnode = curr->next;\
					goto out;\
					}\
				while (curr) {\
					node = curr;\
					curr = curr->next;\
					if (curr->irq_number == irq ) {\
						node->next = curr->next;\
						goto out;\
					}\
				}	\
				curr=NULL;/*no node found*/\
				}while(0)
				
int free_isa_irq(unsigned int irq, void *dev_id,unsigned long irq_type)
{
	unsigned long flags;
	struct irq_node_queue *curr;
	struct irq_node_queue *node;
	disable_isa_interrupt(irq);

	
	spin_lock_irqsave(&irq_controller_lock, flags);
		if(irq<CORE_IRQ)
			{
			;//nothing
			}
		else if(irq<AM_ISA_GEN_IRQ_MAX())
			{
				
				if(irq_type&IRQ_ISA_FAST)
				{
					ISA_FREE_NODE(isa_gen_firq,curr,node,irq,dev_id);
				}
				else
				{
					ISA_FREE_NODE(isa_gen_irq,curr,node,irq,dev_id);
				}
			}
		#if defined(CONFIG_AMLOGIC_BOARD_APOLLO)	
		else if(irq<AM_ISA_GEN1_IRQ_MAX())
			{
				
				if(irq_type&IRQ_ISA_FAST)
				{
					ISA_FREE_NODE(isa_gen1_firq,curr,node,irq,dev_id);
				}
				else
				{
					ISA_FREE_NODE(isa_gen1_irq,curr,node,irq,dev_id);
				}
			}
		#endif	
		else if(irq<AM_ISA_GPIO_IRQ_MAX())
			{
				if(irq_type&IRQ_ISA_FAST)
				{
					ISA_FREE_NODE(isa_gpio_firq,curr,node,irq,dev_id);
					
				}
				else
				{
					ISA_FREE_NODE(isa_gpio_irq,curr,node,irq,dev_id);
					
				}
				
			} 
		else if(irq<AM_ISA_AMRISC_IRQ_MAX())
			{
				if(irq_type&IRQ_ISA_FAST)
				{
					ISA_FREE_NODE(isa_arm_firq,curr,node,irq,dev_id);
					
				}
				else
				{
					ISA_FREE_NODE(isa_arm_irq,curr,node,irq,dev_id);
					
				}
				
				
			}
	out:	
		if(curr)	kfree(curr);
		disable_irq(irq);
		spin_unlock_irqrestore(&irq_controller_lock, flags);
		return 0;
}
int setup_arc_irq(unsigned int irq, struct irq_node *node)
{
	unsigned long flags;
	struct irq_node *curr;

	if (!(irq_list[irq])) {
		irq_list[irq] = node;
		setup_isa_irq(irq,node);
		return 0;
	}

	if (!(irq_list[irq]->flags & IRQ_FLG_STD)) {
		if (irq_list[irq]->flags & IRQ_FLG_LOCK) {
			printk("%s: IRQ %d from %s is not replaceable\n",
			       __FUNCTION__, irq, irq_list[irq]->devname);
			return -EBUSY;
		}
		if (node->flags & IRQ_FLG_REPLACE) {
			printk("%s: %s can't replace IRQ %d from %s\n",
			       __FUNCTION__, node->devname, irq,
			       irq_list[irq]->devname);
			return -EBUSY;
		}
	}

    /* Add the ISR to link-list of ISRs per IRQ */

    spin_lock_irqsave(&irq_controller_lock, flags); /* insert atomically */

	curr = irq_list[irq];
	node->mask=curr->mask;
	while (curr->next)
		curr = curr->next;
	curr->next = node;
	
    spin_unlock_irqrestore(&irq_controller_lock, flags);
	return 0;
}

int request_irq(unsigned int irq, irq_handler_t handler,
		unsigned long flags, const char *devname, void *dev_id)
{
	struct irq_node *node;
	int retval;
	int   i;
	struct irq_node  *curr;
	
	if (irq < 0 || irq >= AM_ISA_IRQ_MAX) {
		printk("%s: Requesting for unknown IRQ %d from %s\n",
		       __FUNCTION__, irq, devname);
		return -ENXIO;
	}

	node = (struct irq_node *)kmalloc(sizeof(struct irq_node), GFP_KERNEL);
	if (!node)
		return -ENOMEM;

	//discard it if dev_id is NULL
	if(dev_id==NULL)   return -1;
	
	for (i=0;i<AM_ISA_IRQ_MAX;i++)
	{
		curr = irq_list[i];
		while(curr)
		{
			if(curr->dev_id==dev_id ) return -1;
			curr = curr->next;
		}
	}
	
	/* Sameer: Lots of stuff is now moved to linux/interrupt.h
	   The whole interrupt subsystem needs a deep look and
	   new implementation */
	node->handler = handler;
	node->flags = flags;
	node->dev_id = dev_id;
	node->devname = devname;
	node->disable_depth=1;//is disabled
	node->next = NULL;
	node->vertor=irq;
	node->hand_num=0;
	/* insert the new irq registered into the irq list */
	retval = setup_arc_irq(irq, node);
	if (retval)
		kfree(node);
	enable_irq(irq);
	return retval;
}

/* free an irq node for the irq list */

void free_irq(unsigned int irq, void *dev_id)
{
	unsigned long flags;
	struct irq_node *curr, *node;
	unsigned long  irq_type;


	
	if (irq < 0 || irq >= AM_ISA_IRQ_MAX) {
		printk("%s: Unknown IRQ %d\n", __FUNCTION__, irq);
		return;
	}
	irq_type=irq_list[irq]->flags;
	spin_lock_irqsave(&irq_controller_lock, flags); /* delete atomically */

	/* this case is if the first ndoe for that irq number itself has 
	   to be freed */
	node = curr = irq_list[irq];
	if (curr->dev_id == dev_id) {

		irq_list[irq] = node->next;
		kfree(curr);
    goto out;
	}

	while (node) {
		if (curr->dev_id == dev_id) {
			node->next = curr->next;
			kfree(curr);
            goto out;
		}
		node = curr;
		curr = curr->next;
	}
    goto bad_out;


//	printk("%s: tried to remove invalid interrupt\n", __FUNCTION__);

out:
	if(irq_list[irq]==NULL)		free_isa_irq(irq,dev_id,irq_type);	
bad_out:	
	spin_unlock_irqrestore(&irq_controller_lock, flags);
	
}



#define PROCESS_ISA_NODE(irq, mnode,node,fp,status)\
do{\
	node=mnode;\
	while(node)\
	{	\
		if(status & irq_list[node->irq_number]->mask)\
		{\
			struct irq_node *i_node; \
			i_node=irq_list[node->irq_number ];\
			while (i_node) {\
				i_node->handler(irq, i_node->dev_id, fp);\
				i_node->hand_num++;\
				i_node = i_node->next;\
			}\
		}\
		node=node->next;\
	}	\
}while(0) 



/* handle the irq */
void process_isa_interrupt(unsigned int irq, struct pt_regs *fp)
{
	unsigned long status,mask;
	struct irq_node_queue *node=NULL;
	
	status=read_irq_status();
	mask=get_irq_mask();
	if((status=status & mask)!=0)
		{
		clear_irq_status(status);
		PROCESS_ISA_NODE(irq, isa_gen_irq,node,fp,status);
		}
	#if defined(CONFIG_AMLOGIC_BOARD_APOLLO)	
	status=read_irq_status1();
	mask=get_irq_mask1();
	if((status=status & mask)!=0)
		{
		clear_irq_status1(status);
		PROCESS_ISA_NODE(irq, isa_gen1_irq,node,fp,status);
		}
	#endif	
	status=read_gpio_irq_status();
	mask=get_gpio_irq_mask();
	if((status=status & mask)!=0)
		{
		clear_gpio_irq_status(status);
		PROCESS_ISA_NODE(irq, isa_gpio_irq,node,fp,status);
		}
	status=read_amrisc_irq_status();
	mask=get_amrisc_irq_mask();
	if((status=status & mask)!=0)
		{
		clear_amrisc_irq_status(status);
		PROCESS_ISA_NODE(irq, isa_arm_irq,node,fp,status);
		}
	return;
}
/* handle the irq */
void process_isa_interrupt_fast(unsigned int irq, struct pt_regs *fp)
{
	unsigned long status,mask;
	struct irq_node_queue *node=NULL;
	status=read_irq_status();
	mask=get_firq_mask();
	if((status=status & mask)!=0)
		{
		clear_irq_status(status);
		PROCESS_ISA_NODE(irq, isa_gen_firq,node,fp,status);
		}
	#if defined(CONFIG_AMLOGIC_BOARD_APOLLO)	
	status=read_irq_status1();
	mask=get_firq_mask1();
	if((status=status & mask)!=0)
		{
		clear_irq_status1(status);
		PROCESS_ISA_NODE(irq, isa_gen1_firq,node,fp,status);
		
		}
	#endif
	status=read_gpio_irq_status();
	mask=get_gpio_firq_mask();
	if((status=status & mask)!=0)
		{
		clear_gpio_irq_status(status);
		PROCESS_ISA_NODE(irq, isa_gpio_firq,node,fp,status);
		
		}
	status=read_amrisc_irq_status();
	mask=get_amrisc_firq_mask();
	if((status=status & mask)!=0)
		{
		clear_amrisc_irq_status(status);
		PROCESS_ISA_NODE(irq, isa_arm_firq,node,fp,status);
		
		}
	return;
}


#if 0

/* handle the irq */
void process_interrupt_all(unsigned int irq, struct pt_regs *fp)
{
    struct pt_regs *old = set_irq_regs(fp);
struct irq_node *node;
#ifdef	ARC_IRQ_DBG
	int proc_cnt = 0;
#endif
#ifdef	ARC_IRQ_DBG	 
	register unsigned long sp asm ("sp");
	unsigned long ss=sp;
	if((ss-(unsigned  int)current_thread_info())<0x200)
	{
		printk(KERN_ERR "[WORRING],The stack size is more smoller \n");
		printk("	Some date maybe over writed later...");
		printk("	Current Task is [%s]\n",get_current()->comm);
		printk("	Stack botton is 0x%x\n",(unsigned int)((unsigned int)current_thread_info()+sizeof(struct thread_info)));
		printk("	Current stack pointer is 0x%x\n",(unsigned int)ss);
	}
#endif	
	//irq_enter();

	if (irq >= 0 && irq < CORE_IRQ) {
		/* Sameer: kstat also undergone changes for SMP support. So
		   we need to handle it in a different way. */
		/* kstat.irqs[0][irq]++; */
		switch (irq)
		{
			case  5:
				 
				process_isa_interrupt(irq,fp);
				#ifdef	ARC_IRQ_DBG
					proc_cnt++;
				#endif
				break;
			case 6:
				 process_isa_interrupt_fast(irq,fp);
				#ifdef	ARC_IRQ_DBG
					proc_cnt++;
				#endif
				break;
					
			default :
				node = irq_list[irq];
				/* call all the ISR's in the list for that interrupt source */
				while (node) {
				node->handler(irq, node->dev_id, fp);
				node->hand_num++;
				node = node->next;
				#ifdef	ARC_IRQ_DBG
					proc_cnt++;
				#endif
				}
				break;
		}
		
	} else
		panic("Can't process interrupt vector %u\n", irq);
#ifdef	ARC_IRQ_DBG
	if (!proc_cnt)
        printk(KERN_ERR "Spurious interrupt : irq no %u on cpu %u", irq, 
                    smp_processor_id());
#endif

    /* vineetg: June 17th 2008
     * Don't see a need for this.
     * STILL NEED TO VERIFY
     */
    //__cli();
	//irq_exit();

    set_irq_regs(old);
	return;
}



int get_irq_list(char *buf)
{
	int i;
	struct irq_node *node;
	char *p = buf;

	for (i = 0; i < AM_ISA_IRQ_MAX; i++) {
		node = irq_list[i];
		if (!node)
			continue;
		p += sprintf(p, "%3d: %10u ", i, kstat_irqs(i));
		p += sprintf(p, "  %s", node->devname);
		for (node = node->next; node; node = node->next) {
			p += sprintf(p, "  %s", node->devname);
		}
		*p++ = '\n';
	}
	return (p - buf);

}
#endif /* 0 */

/* FIXME: implement if necessary */
unsigned long probe_irq_on(void)
{
printk("show probe_irq_on\n");
	return 0;
}

/* FIXME: implement if necessary */
int probe_irq_off(unsigned long irqs)
{
printk("show probe_irq_off\n");
	return 0;
}

/* FIXME: implement if necessary */
void init_irq_proc(void)
{
	printk("show init_irq_proc\n");
	/* place holder */
}


#if 0
/* Sameer: Empty stub. Need our implentation here. */
int show_interrupts(struct seq_file *p, void *v)
{

 	int i = *(loff_t *) v, cpu; 
	struct irq_node* node; 
 	unsigned long flags; 

 	if (i == 0) { 
 		char cpuname[12]; 
 		seq_printf(p, "    "); 
		for_each_present_cpu(cpu) { 
			sprintf(cpuname, "CPU%d", cpu); 
		seq_printf(p, " %10s", cpuname); 
		} 
 		seq_putc(p, '\n'); 
 	} 
	local_irq_save(flags);
 	if (i < NR_IRQS) {
		node = irq_list[i]; 
		while(node!=NULL)
		{
			
			seq_printf(p, "%3d: ", i); 
			seq_printf(p, "%10u ", node->hand_num);
			seq_printf(p, "  %s", node->devname); 
			seq_printf(p, "  %s", irq_list[i]->disable_depth>0?"disable":"enable"); 
			seq_putc(p, '\n');
			node=node->next;
		}
	} else if (i == NR_IRQS) { 
	/*
		for(i=0;i<NR_IRQS;i++)
			{
			node=irq_list[i];
			if(node)
				{
					seq_printf(p, "%3d: %10u %10s", i,node->hand_num,node->devname?node->devname:"-"); 
					seq_putc(p, '\n'); 
				}
			}
			*/
 	} 
	 local_irq_restore(flags);
return 0;
}
#endif /* 0 */
/**
 *		disable_irq - disable an irq and wait for completion
 *		@irq: Interrupt to disable
 *
 *		Disable the selected interrupt line.  We do this lazily.
 *
 *		This function may be called from IRQ context.
 */
void disable_irq(unsigned int irq)
{
	struct irq_node *node;
	unsigned long flags;
	unsigned long intbit;
	//unsigned long reg;

	if ((irq >= 0 && irq < CORE_IRQ) && (node = irq_list[irq])) {
		spin_lock_irqsave(&irq_controller_lock, flags);
		if (!node->disable_depth++) {
			intbit = (1<<irq);
			mask_interrupt(intbit);
		}
		spin_unlock_irqrestore(&irq_controller_lock, flags);
	}
	else if((irq >= CORE_IRQ && irq < AM_ISA_IRQ_MAX) && (node = irq_list[irq]))
		{
			spin_lock_irqsave(&irq_controller_lock, flags);
			if (!node->disable_depth++) {
				disable_isa_interrupt(irq);
			}
			spin_unlock_irqrestore(&irq_controller_lock, flags);
		
		}
}

EXPORT_SYMBOL(disable_irq);
/**
 *		enable_irq - enable interrupt handling on an irq
 *		@irq: Interrupt to enable
 *
 *		Re-enables the processing of interrupts on this IRQ line.
 *		Note that this may call the interrupt handler, so you may
 *		get unexpected results if you hold IRQs disabled.
 *
 *		This function may be called from IRQ context.
 */
void enable_irq(unsigned int irq)
{
	struct irq_node *node;
	unsigned long flags;
	unsigned long intbit;
	//unsigned long reg;
	if ((irq >= 0 && irq < CORE_IRQ) && (node = irq_list[irq])) {
 		spin_lock_irqsave(&irq_controller_lock, flags);
		if (!node->disable_depth) {
			printk("enable_irq(%u) unbalanced from 0x%p\n", irq,
						 __builtin_return_address(0));
		} else if (!--node->disable_depth) {
			intbit = 1<<irq;
			unmask_interrupt(intbit);			
		}
		spin_unlock_irqrestore(&irq_controller_lock, flags);
	}
	else if((irq >= CORE_IRQ && irq < AM_ISA_IRQ_MAX) && (node = irq_list[irq]))
		{
			spin_lock_irqsave(&irq_controller_lock, flags);
			if (!node->disable_depth) {
				printk("enable_irq(%u) unbalanced from 0x%p\n", irq,
							 __builtin_return_address(0));
			} else if (!--node->disable_depth) {
				intbit = 1<<5;
				if(node->flags &IRQ_ISA_FAST)
					intbit |= 1<<6;
				unmask_interrupt(intbit);	
				enable_isa_interrupt(irq,node->flags);
			}
			spin_unlock_irqrestore(&irq_controller_lock, flags);
		}
	else
		{
		printk("can't enable irq (%d),node=%p\n",irq,irq_list[irq]);
		}
}

EXPORT_SYMBOL(enable_irq);

#if 0


int get_hw_config_num_irq()
{
    uint32_t val = read_new_aux_reg(ARC_REG_VECBASE_BCR);

    switch(val & 0x03)
    {
        case 0: return 16;
        case 1: return 32;
        case 2: return 8;
        default: return 0;
    }

    return 0;
}

/* Enable interrupts.
 * 1. Explicitly called to re-enable interrupts
 * 2. Implicitly called from spin_unlock_irq, write_unlock_irq etc
 *    which maybe in hard ISR itself
 *
 * Semantics of this function change depending on where it is called from:
 *
 * -If called from hard-ISR, it must not invert interrupt priorities
 *  e.g. suppose TIMER is high priority (Level 2) IRQ
 *    Time hard-ISR, timer_interrupt( ) calls spin_unlock_irq several times.
 *    Here local_irq_enable( ) shd not re-enable lower priority interrupts 
 * -If called from soft-ISR, it must re-enable all interrupts   
 *    soft ISR are low prioity jobs which can be very slow, thus all IRQs
 *    must be enabled while they run. 
 *    Now hardware context wise we may still be in L2 ISR (not done rtie)
 *    still we must re-enable both L1 and L2 IRQs
 *  Another twist is prev scenario with flow being
 *     L1 ISR ==> interrupted by L2 ISR  ==> L2 soft ISR
 *     here we must not re-enable Ll as prev Ll Interrupt's h/w context will get 
 *     over-written (this is deficiency in ARC700 Interrupt mechanism)
 */

#ifdef CONFIG_ARCH_ARC_LV2_INTR     // Complex version for 2 levels of Intr

void local_irq_enable(void) {

    unsigned long flags;
    local_save_flags(flags);

    /* Allow both L1 and L2 at the onset */
    flags |= (STATUS_E1_MASK | STATUS_E2_MASK);

    /* Called from hard ISR (between irq_enter and irq_exit) */
    if ( in_irq() ) {

        /* If in L2 ISR, don't re-enable any further IRQs as this can cause 
         * IRQ priorities to get upside down.
         * L1 can be taken while in L2 hard ISR which is wron in theory ans
         * can also cause the dreaded L1-L2-L1 scenario
         */
        if ( flags & STATUS_A2_MASK ) {
            flags &= ~(STATUS_E1_MASK | STATUS_E2_MASK);
        }

        /* Even if in L1 ISR, allowe Higher prio L2 IRQs */
        else if ( flags & STATUS_A1_MASK ) {
            flags &= ~(STATUS_E1_MASK);
        }
    }

    /* called from soft IRQ, ideally we want to re-enable all levels */

    else if ( in_softirq() ) {

        /* However if this is case of L1 interrupted by L2,
         * re-enabling both may cause whaco L1-L2-L1 scenario
         * because ARC700 allows level 1 to interrupt an active L2 ISR
         * Thus we disable both
         * However some code, executing in soft ISR wants some IRQs to be
         * enabled so we re-enable L2 only
         *
         * How do we determine L1 intr by L2
         *  -A2 is set (means in L2 ISR)
         *  -E1 is set in this ISR's pt_regs->status32 which is
         *      saved copy of status32_l2 when l2 ISR happened
         */
        struct pt_regs *pt = get_irq_regs();
        if ( (flags & STATUS_A2_MASK ) && pt &&
              (pt->status32 & STATUS_A1_MASK ) ) {
            //flags &= ~(STATUS_E1_MASK | STATUS_E2_MASK);
            flags &= ~(STATUS_E1_MASK);
        }
    }

    local_irq_restore(flags);
}

#else  /* ! CONFIG_ARCH_ARC_LV2_INTR */
    
 /* Simpler version for only 1 level of interrupt
  * Here we only Worry about Level 1 Bits 
  */

void local_irq_enable(void) {

    unsigned long flags;
    local_save_flags(flags);
    flags |= (STATUS_E1_MASK | STATUS_E2_MASK);

    /* If called from hard ISR (between irq_enter and irq_exit) 
     * don't allow Level 1. In Soft ISR we allow further Level 1s
     */

    if ( in_irq() ) {
        flags &= ~(STATUS_E1_MASK | STATUS_E2_MASK);
    }

    local_irq_restore(flags);
}
#endif

EXPORT_SYMBOL(local_irq_enable);

/************************   DEBUG   ********************************
 * Monitoring a Variable every IRQ entry/exit
 * Low Level ISR can code to dump the contents of a variable
 * This can for e.g. be used to figure out the whether the @var
 * got clobbered during ISR or between ISRs (pure kernel mode)
 *
 * The macro itself can be switched on/off at runtime using a toggle
 * @irq_inspect_on
 */
extern void raw_printk5(const char *str, uint n1, uint n2, uint n3, uint n4);

int irq_inspect_on = 1;   // toggle to switch on/off at runtime

/* Function called from level ISR */
void print_var_on_irq(int irq, int in_or_out, uint addr, uint val)
{
    raw_printk5("IRQ \n", irq, in_or_out, addr, val);
}

#endif /* 0 */

