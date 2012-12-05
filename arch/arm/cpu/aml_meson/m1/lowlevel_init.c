#include <asm/arch/am_regs.h>
#include <asm/arch/am_reg_addr.h>
#include <asm/arch/pctl.h>
#include <asm/arch/romboot.h>
#include <memtest.h>
#include <config.h>
#include <common.h>
extern void wait(void);
void serial_put_dword(unsigned int data)
{
	int i;
    serial_puts("0x");
    for (i=0;i<8;i++){
        unsigned char s = data>>28;
        if (s<10)
            serial_putc(0x30+s);
        else
            serial_putc(0x61+s-10);
        data<<=4;
    }
}
void serial_put_word(unsigned short data)
{
	int i;
    serial_puts("0x");
    for (i=0;i<4;i++){
        unsigned char s = data>>12;
        if (s<10)
            serial_putc(0x30+s);
        else
            serial_putc(0x61+s-10);
        data<<=4;
    }
}

static  void display_error(unsigned por_cfg,char * hint)
{
	serial_puts(hint);
	if(por_cfg)
	{
		serial_puts("fail");
		serial_put_dword(por_cfg);


	}else{
		serial_puts("pass");
	}
	serial_puts("\n");

}
int lowlevel_init(void)
{
	  unsigned por_cfg = READ_CBUS_REG(ASSIST_POR_CONFIG);
	if (((por_cfg & POR_ROM_BOOT_ENABLE) == 0)) {
		// take me as a spi rom boot mode
		romboot_info->por_cfg = POR_ROM_BOOT_ENABLE | POR_INTL_SPI;
		romboot_info->boot_id = 0;
	}

	WRITE_CBUS_REG(PAD_PULL_UP_REG0,-1);
	WRITE_CBUS_REG(PAD_PULL_UP_REG1,-1);
	WRITE_CBUS_REG(PAD_PULL_UP_REG2,-1);
	WRITE_CBUS_REG(PAD_PULL_UP_REG3,-1);

	//changed by Elvis, add uart rx pull up
	//pull up LINUX_RX(B15--->GPIOE_19) reg (7422y v1.pdf)
	//GPIOE_19(M1-Apps v25- 2010-07-19-BGA372_297-6.xls serach B15)
	WRITE_CBUS_REG( PAD_PULL_UP_REG3, READ_CBUS_REG(PAD_PULL_UP_REG3) & ~(1<<2) );	// Meson-pull-up-down_table.xlsx ( GPIOE_19 is PAD_PULL_UP_REG3---0x203e 2bits)
	
	//Adjust 1us timer base
	WRITE_CBUS_REG_BITS(PREG_CTLREG0_ADDR,CONFIG_CRYSTAL_MHZ,4,5);
	pll_initial(); //running under default freq now . Before we fixed the PLL stable problem

	unsigned clk;
	if ((readl(P_HHI_MPEG_CLK_CNTL) & (1 << 8)) == 0) {
		clk = CONFIG_CRYSTAL_MHZ * 1000000;
	} else {
		clk = romboot_info->clk81;
	}
	unsigned memory_size;
	
	memory_size = PHYS_MEMORY_SIZE;
	serial_init_with_clk(clk);
	writel((1<<22)|1000000,P_WATCHDOG_TC);
	do {
		unsigned ret1, ret2, ret3, ret4;
		ret1 = ret2 = ret3 = ret4 = 0;
		por_cfg = 0;
		ret1 = initial_ddr(&__hw_setting.ddr, __hw_setting.ddr.lane);
		display_error(ret1, "DDr Init");

#ifdef CONFIG_MEM_TEST
		ret2=memTestDevice((volatile datum *)PHYS_MEMORY_START,memory_size);
		display_error(ret2,"TestDevice");
		por_cfg=ret2;
#endif
		ret3 = memTestDataBus((volatile datum *) PHYS_MEMORY_START);
		display_error(ret3, "TestDataBus");
		ret4 = memTestAddressBus((volatile datum *) PHYS_MEMORY_START,
				memory_size);

		display_error(ret4, "TestAddressBus");

		por_cfg = ret1 | ret2 | ret3 | ret4;

	} while (por_cfg != 0);
	writel(0,P_WATCHDOG_RESET);
	return 0;

}
