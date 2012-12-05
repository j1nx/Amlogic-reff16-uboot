#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <asm/arch/romboot.h>
#include <asm/arch/irqs.h>
#include <memtest.h>
#include <config.h>
#include <asm/arch/firm/io.h>

int chip_reset(void)
{
#ifdef AML_BOOT_SPI  	
	/* if uboot is on flash then boot agian from flash */
	writel(WATCHDOG_ENABLE_MASK, IREG_WATCHDOG_CONTROL0);
#endif	
	WRITE_CBUS_REG(WATCHDOG_RESET, 0);
    WRITE_CBUS_REG(WATCHDOG_TC, 1 << 22 | 100); /*100*10uS=1ms*/
    WRITE_CBUS_REG(WATCHDOG_RESET, 0);
    __udelay(10000);/**/
    serial_puts("Chip watchdog reset error!!!please reset it by hardware\n");
    while (1) ;
	return 0;
}

void freezing_main(void)
{
    serial_puts("\nfreezing...\n");

	init_custom_trigger();

	arch_interrupt_init();

	serial_puts("\nIRQ...\n");

	//irq_install_handler(INT_REMOTE, ir_remote_irq_handler, NULL);     // enable remote interrupt

	//while(1);
	
	//writel((1<<22)|100000,P_WATCHDOG_TC);//enable Watchdog 1 seconds
	//Adjust 1us timer base
	WRITE_CBUS_REG_BITS(PREG_CTLREG0_ADDR,CONFIG_CRYSTAL_MHZ,4,5);
	/*
        Select TimerE 1 us base
    */
	clrsetbits_le32(P_ISA_TIMER_MUX,0x7<<8,0x1<<8);

	cooling();

	chip_reset();
}
