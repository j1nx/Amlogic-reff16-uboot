#include <asm/arch/am_regs.h>
#include <asm/arch/am_reg_addr.h>
#include <asm/arch/pctl.h>
#include <asm/arch/romboot.h>
#include <memtest.h>
#include <config.h>
#include <common.h>
#define JTAG_ENABLE 0x51
extern unsigned char auto_run[4];
extern unsigned cur_char;
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



int get_next_char(void)
{
	int ch;
	if(cur_char<4)
	{
		ch=(int)auto_run[cur_char];
		cur_char++;
	}
	else{
		while(!serial_tstc());
		ch=serial_getc();
	}
	return ch;

}
void caculate_sum(void )
{
	int i;
	unsigned short sum=0;
	unsigned * magic;
	volatile unsigned short *buf=(volatile unsigned short*)AHB_SRAM_BASE;
	for(i=0;i<0x1b0/2;i++)
	{
		sum^=buf[i];
	}

	for(i=256;i<READ_SIZE/2;i++)
	{
		sum^=buf[i];
	}
	buf[0x1b8/2]=sum;
	magic=(unsigned *)&buf[0x1b0/2];
}
int ch2d(int ch)
{
	if(ch>='0'&&ch<='9')
		return ch - '0';
	if(ch>='a'&&ch<='f')
		return ch - 'a' + 10;
	return 0x10;
}
void modify_setting(unsigned short addr,unsigned short val)
{
	unsigned short * p=(unsigned short *)&__hw_setting;
	p[addr]=val;
}
void display_setting(unsigned short addr)
{
	unsigned short * p=(unsigned short *)&__hw_setting;
	serial_puts("\n");
	serial_put_word(p[addr]);
}

unsigned short get_short()
{
	int i,cha,d;
	unsigned short addr=0;
	for(i=0;i<4;i++)
	{
		cha=get_next_char();
		d=ch2d(cha);
		if(d<0x10)
		{
			addr|=d<<((3-i)<<2);
			serial_putc(cha);
		}
		else
			i--;
	}
	return addr;
}
void lowlevel_init(void)
{
   unsigned por_cfg
#if 0
   =READ_CBUS_REG(ASSIST_POR_CONFIG);

   if(((por_cfg&POR_ROM_BOOT_ENABLE)==0))
   {
      // take me as a spi rom boot mode
      romboot_info->por_cfg=POR_ROM_BOOT_ENABLE | POR_INTL_SPI;
      romboot_info->boot_id=0;
   }

   WRITE_CBUS_REG(PAD_PULL_UP_REG0,-1);
   WRITE_CBUS_REG(PAD_PULL_UP_REG1,-1);
   WRITE_CBUS_REG(PAD_PULL_UP_REG2,-1);
   WRITE_CBUS_REG(PAD_PULL_UP_REG3,-1);
#else
	   ;
#endif
   //Adjust 1us timer base
   WRITE_CBUS_REG_BITS(PREG_CTLREG0_ADDR,CONFIG_CRYSTAL_MHZ,4,5);
   unsigned clk;
   if((readl(P_HHI_MPEG_CLK_CNTL)&(1<<8))==0)
	{
		clk=CONFIG_CRYSTAL_MHZ*1000000;
	}else{
		clk=romboot_info->clk81;
	}

  serial_init_with_clk(clk);
  int i;

	unsigned  addr;
	unsigned  val;
	cur_char=0;

	while(1)
	{
		serial_puts("\n>");
		int ch;

		ch=get_next_char();
		serial_putc(ch);
		switch(ch)
		{
//		case 'i':
//		    switch_to_init();
//		    display_error(ddr_phy_data_training(),"\ntrain");
//		    break;
		case 's':
			serial_puts("\nA9Stop");
			wait();
			break;
		case 'A'://Cortex A9
			serial_puts("\nA9JTAG");
			writel(JTAG_ENABLE<<4,P_HHI_JTAG_CONFIG);
			break;
		case 'a'://arc625
			serial_puts("\nARC625 JTAG");
			writel(JTAG_ENABLE<<12|1,P_HHI_JTAG_CONFIG);
			break;
		case 'W'://Wifi ARC
			serial_puts("\nWifi JTAG");

           WRITE_CBUS_REG(HHI_DEMOD_PLL_CNTL, ((4 << 20)|(0<<16)|(0<<15)|(1<<9)|(48<<0)));
           WRITE_CBUS_REG(HHI_WIFI_CLK_CNTL, 0x1);
           WRITE_CBUS_REG(HHI_DEMOD_PLL_CNTL2, READ_CBUS_REG(HHI_DEMOD_PLL_CNTL2)&~((0x1f<<16)|(0x1f<<21)|(0x1f<<26))|((15<<16)|(21<<21)|(10<<26)));
           WRITE_CBUS_REG(HHI_DEMOD_PLL_CNTL3, READ_CBUS_REG(HHI_DEMOD_PLL_CNTL3)&~((0x1f<<16)|(0x1f<<21)|(0x1f<<26))|((5<<16)|(4<<21)|(3<<26)));
           WRITE_CBUS_REG(PERIPHS_PIN_MUX_8, READ_CBUS_REG(PERIPHS_PIN_MUX_8)|(0xffff8000));
           writel(0x1,(volatile unsigned long *)0xc93200a0);
           writel((JTAG_ENABLE<<20)|2,P_HHI_JTAG_CONFIG);
			break;
		case 'w'://quick Wifi ARC  easy
		  serial_puts("\nquick Wifi JTAG ");
		  writel(JTAG_ENABLE<<20|2,P_HHI_JTAG_CONFIG);
		  break;
		case 'M':
		   pll_initial(); //running under default freq now . Before we fixed the PLL stable problem
		   por_cfg=initial_ddr(&__hw_setting.ddr,__hw_setting.ddr.lane);
		   display_error(por_cfg,"\nTrainning");
		   pll_clk_list();
		   por_cfg = memTestDataBus((volatile datum *) PHYS_MEMORY_START);
		   display_error(por_cfg,"\nDataBus");
			por_cfg = memTestAddressBus((volatile datum *) PHYS_MEMORY_START,
					__hw_setting.ddr.lane==0x300?0x8000000:0x10000000);
			display_error(por_cfg,"\nAddr Bus");
			break;
       case 'T':
       	por_cfg=(unsigned)memTestDevice((volatile datum *)PHYS_MEMORY_START,__hw_setting.ddr.lane==0x300?0x8000000:0x10000000);
			display_error(por_cfg,"memDev");
			break;
		case 'S':
			serial_puts("\n3 chars:");
			for(i=0;i<3;i++)
			{
				ch=get_next_char();

				serial_putc(ch);
				auto_run[i]=ch&0xff;
			}
			auto_run[3]=0;
			caculate_sum();
			spi_erase();
			spi_program(AHB_SRAM_BASE,READ_SIZE);
		case 'r':
			setbits_le32(P_WATCHDOG_TC,1<<22);
			break;
		case 'd':
		case 'm':
			addr=get_short();

			if(ch=='d')
			{
				display_setting(addr);
				break;
			}
			val=get_short();

			modify_setting(addr,val);
			break;
		case 'e':
			spi_erase();
			break;

		}

	}
}
