#include <asm/arch/irqs.h>

#define isb() __asm__ __volatile__ ("isb" : : : "memory")
#define dsb() __asm__ __volatile__ ("dsb" : : : "memory")
#define dmb() __asm__ __volatile__ ("dmb" : : : "memory")

/* enable IRQ interrupts */
void enable_interrupts (void)
{
	unsigned long temp;
	__asm__ __volatile__("mrs %0, cpsr\n"
			     "bic %0, %0, #0x80\n"
			     "msr cpsr_c, %0"
			     : "=r" (temp)
			     :
			     : "memory");
}


/*
 * disable IRQ/FIQ interrupts
 * returns true if interrupts had been enabled before we disabled them
 */
int disable_interrupts (void)
{
	unsigned long old,temp;
	__asm__ __volatile__("mrs %0, cpsr\n"
			     "orr %1, %0, #0xc0\n"
			     "msr cpsr_c, %1"
			     : "=r" (old), "=r" (temp)
			     :
			     : "memory");
	return (old & 0x80) == 0;
}

/***********************************************************************
 * IRQ
 **********************************************************************/

/* Enable interrupt */
static void meson_unmask_irq(unsigned int irq)
{
	unsigned int mask;

	if (irq >= NR_IRQS)
		return;

	mask = 1 << IRQ_BIT(irq);

	SET_CBUS_REG_MASK(IRQ_MASK_REG(irq), mask);
	
	dsb();
}

/* Disable interrupt */
static void meson_mask_irq(unsigned int irq)
{
	unsigned int mask;

	if (irq >= NR_IRQS)
		return;

	mask = 1 << IRQ_BIT(irq);

	CLEAR_CBUS_REG_MASK(IRQ_MASK_REG(irq), mask);
	
	dsb();
}

/* Clear interrupt */
static void meson_ack_irq(unsigned int irq)
{
	unsigned int mask;

	if (irq >= NR_IRQS)
		return;

	mask = 1 << IRQ_BIT(irq);

	WRITE_CBUS_REG(IRQ_CLR_REG(irq), mask);
	
	dsb();
}

/* ARM Interrupt Controller Initialization */
void meson_init_irq(void)
{
	unsigned i;

	/* Disable all interrupt requests */
	WRITE_CBUS_REG(A9_0_IRQ_IN0_INTR_MASK, 0);
	WRITE_CBUS_REG(A9_0_IRQ_IN1_INTR_MASK, 0);
	WRITE_CBUS_REG(A9_0_IRQ_IN2_INTR_MASK, 0);
	WRITE_CBUS_REG(A9_0_IRQ_IN3_INTR_MASK, 0);

	/* Clear all interrupts */
	WRITE_CBUS_REG(A9_0_IRQ_IN0_INTR_STAT_CLR, ~0);
	WRITE_CBUS_REG(A9_0_IRQ_IN1_INTR_STAT_CLR, ~0);
	WRITE_CBUS_REG(A9_0_IRQ_IN2_INTR_STAT_CLR, ~0);
	WRITE_CBUS_REG(A9_0_IRQ_IN3_INTR_STAT_CLR, ~0);

	/* Set all interrupts to IRQ */
	WRITE_CBUS_REG(A9_0_IRQ_IN0_INTR_FIRQ_SEL, 0);
	WRITE_CBUS_REG(A9_0_IRQ_IN1_INTR_FIRQ_SEL, 0);
	WRITE_CBUS_REG(A9_0_IRQ_IN2_INTR_FIRQ_SEL, 0);
	WRITE_CBUS_REG(A9_0_IRQ_IN3_INTR_FIRQ_SEL, 0);
}

struct _irq_handler {
	void                *m_data;
	void (*m_func)( void *data);
};

typedef void (interrupt_handler_t)(void *);

static struct _irq_handler IRQ_HANDLER[NR_IRQS];

static void default_isr(void *data)
{
	serial_puts("default_isr():  ");
	serial_put_char((int)data);
}

//void do_irq (struct pt_regs *pt_regs)
void do_irq (struct pt_regs *pt_regs)
{
	int i;
	unsigned long intr_stat;
	unsigned char irq;
	disable_interrupts();
	serial_puts("\ndo_irq...\n");

	for(i=0; i<4; i++)
	{
		intr_stat = READ_CBUS_REG(A9_0_IRQ_IN0_INTR_STAT + 4*i);
		if(intr_stat)
		{
			for(irq=0; irq<32; irq++)
			{
				if(intr_stat & (1<<irq))
				{
					irq += 32*i;
					break;
				}
			}
		}
	}
	if (irq >= NR_IRQS)
	{
		serial_puts("interrupt number out of range\n");
		enable_interrupts();
		return;
	}
	else
	{
		serial_put_char(irq);
	}

	meson_ack_irq(irq);
	enable_interrupts();

	IRQ_HANDLER[irq].m_func(IRQ_HANDLER[irq].m_data);
}

void irq_install_handler (int irq, interrupt_handler_t handle_irq, void *data)
{
	if (irq >= NR_IRQS || !handle_irq)
		return;
	disable_interrupts();
	IRQ_HANDLER[irq].m_data = data;
	IRQ_HANDLER[irq].m_func = handle_irq;
	meson_unmask_irq(irq);
	enable_interrupts();
}

int arch_interrupt_init (void)
{
	int i;
	disable_interrupts();
	meson_init_irq();
	
	/* install default interrupt handlers */
	for (i = 0; i < NR_IRQS; i++)
	{
		IRQ_HANDLER[i].m_data = (void *)i;
		IRQ_HANDLER[i].m_func = default_isr;
	}

	enable_interrupts();
	return (0);
}

