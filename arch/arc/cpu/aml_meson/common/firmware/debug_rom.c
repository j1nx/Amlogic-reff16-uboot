

#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/romboot.h>
#include <asm/arch/firm/timing.h>
#include <memtest.h>
#include <config.h>
#include <asm/arch/firm/io.h>
#include <asm/arch/firm/config.h>
static  char cmd_buf[DEBUGROM_CMD_BUF_SIZE];
STATIC_PREFIX char * get_cmd(void )
{
    
    int i,j;
    char c;
    i=0;
    
    cmd_buf[0]=0;
    i=0;
    serial_putc('>');
    while(1)
    {
        if(serial_tstc()==0)
            continue;
        c=serial_getc();
        j=0;
        switch(c)
        {
        case '\r':
        case '\n':
            serial_putc('\n');
            return &cmd_buf[0];
        case 0x08:
        case 0x7f:
            if(i>0)
            {
                serial_puts("\b \b");
                cmd_buf[i--]=0;
                cmd_buf[i]=0x20;
            }
            continue;
        case 'w':
        case 'r':
        case 'q':
        case ' ':
        case 'P':
        case 'S':
        case 's':
        case 'M':
        case 'B':
        case '"':
        case ';':
            if(i==sizeof(cmd_buf)-1)
                continue;
            serial_putc(c);
            cmd_buf[i++]=c;
            cmd_buf[i]=0;
            continue;
        default:
            if(i==sizeof(cmd_buf)-1)
                continue;
            if((c>='0' && c<='9')||
               (c>='a' && c<='f'))
            {
                serial_putc(c);
                cmd_buf[i++]=c;
                cmd_buf[i]=0;
                continue;
            }
        }
        serial_put_char(c);
        serial_putc('\n');
        serial_putc('>');
        serial_puts(cmd_buf);
    }
    return &cmd_buf[0];
}
STATIC_PREFIX int get_dword(char * str,unsigned * val)
{
    int c,i;
    *val=0;
    for(i=0;i<8&&*str;i++,str++)
    {
        c=*str;
        if((c>='0' && c<='9'))
            c-='0';
        else if((c>='a' && c<='f'))
            c-='a'-10;
        else
            return -1;
        *val=(*val<<4)|c;
    }
    return 0;
}
STATIC_PREFIX void debug_write_reg(int argc, char * argv[])
{
    unsigned  reg;
    unsigned  val;
    if(argc<3)
    {
        serial_puts("FAIL:Wrong write reg command\n");
        return;
    }
    if(get_dword(argv[1],&reg)||get_dword(argv[2],&val))
    {
        serial_puts("FAIL:Wrong reg addr=");
        serial_puts(argv[1]);
        serial_puts(" or val=");
        serial_puts(argv[2]);
        serial_putc('\n');
        return;
    }
    serial_puts("OK:Write ");
    serial_put_hex(reg,32);
    serial_putc('=');
    serial_put_hex(val,32);
    serial_putc('\n');
    writel(val,reg);
}
STATIC_PREFIX void debug_read_reg(int argc, char * argv[])
{
    unsigned  reg;
    if(argc<2)
    {
        serial_puts("FAIL:Wrong read reg command\n");
        return;
    }
    if(get_dword(argv[1],&reg))
    {
        serial_puts("FAIL:Wrong reg addr=");
        serial_puts(argv[1]);
        serial_putc('\n');
        return;
    }
    serial_puts("OK:Read ");
    serial_put_hex(reg,32);
    serial_putc('=');
    serial_put_hex(readl(reg),32);
    serial_putc('\n');
}
STATIC_PREFIX void memory_pll_init(int argc, char * argv[]);
STATIC_PREFIX void show_setting_addr(int argc, char * argv[])
{
    serial_puts("OK:ADDR=");
//    serial_put_hex((unsigned)&__ddr_setting,32);
    serial_putc(' ');
    serial_put_hex((unsigned)&__plls,32);
    serial_putc('\n');
}

STATIC_PREFIX void debugrom_ddr_init(int argc, char * argv[])
{
    
//    int i;
//    ddr_pll_init(&__ddr_setting);
//    ddr_init_test();
    
}
STATIC_PREFIX void caculate_sum(void )
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
STATIC_PREFIX void debugrom_save_to_spi(int argc, char * argv[])
{
    caculate_sum();
    spi_erase();
    spi_program(AHB_SRAM_BASE,READ_SIZE);
}
STATIC_PREFIX void memory_pll_init(int argc, char * argv[])
{
    pll_initial(&__plls); //running under default freq now . Before we fixed the PLL stable problem
    serial_init_firmware(__plls.uart);
	__udelay(1000);//delay 1 ms , wait pll ready
}

STATIC_PREFIX void debugrom_set_start(int argc, char * argv[])
{
    char * p;
    p=argv[1];
    ipl_memcpy(&init_script[0],p,sizeof(init_script));
    init_script[sizeof(init_script)-1]=0;
}

STATIC_PREFIX char * debugrom_startup(void)
{
    static int cur =0;
    int ret;
    ret=cur;
    if(cur>sizeof(init_script))
        return NULL;
    if(cur==0)
    {
        ipl_memcpy(&cmd_buf[0],&init_script[0],sizeof(init_script));
        serial_puts(&cmd_buf[0]);serial_putc('\n');
    }
    for(;cur!=';'&&cmd_buf[cur]!=0;cur++);
    if(cur==';')cmd_buf[cur++]=0;
    return &cmd_buf[ret];
}
STATIC_PREFIX int run_cmd(char * cmd)
{
    int argc;
    char * argv[4];
    char * str;
    for(argc=0,str=cmd;argc<4;argc++)
    {
        while(*str==0x20)
            str++;
        if(*str==0)
            break;
        if(*str=='"')
        {
            argv[argc]=++str;
            while(*str!='"'&&*str!=0)
                str++;
            if(*str==0)
                break;
            *str++=0;
            continue;
        }
        argv[argc]=str;
        while(*str!=0x20&&*str!=0)
            str++;
        if(*str==0)
        {
            argc++;
            break;
        }
        *str++=0;
    }
    if(argc==0)
        return 1;
    switch(argv[0][0])
    {
    case 'w':
        debug_write_reg(argc,argv);
        break;
    case 'r':
        debug_read_reg(argc,argv);
        break;
#if (defined AML_DEBUGROM)||(CONFIG_ENABLE_SPL_MORE_CMD)
    case 'P':
        memory_pll_init(argc,argv);
        break;
    case 'S':
        show_setting_addr(argc,argv);
        break;
    case 'M':
        debugrom_ddr_init(argc,argv);
        break;
    case 'B':
        debugrom_set_start(argc,argv);
        break;
#endif 
#if (defined AML_DEBUGROM)
    case 's':
        debugrom_save_to_spi(argc,argv);
        break;
#endif    
    case 'q':
        return 0;
    }
    return 1;
}
STATIC_PREFIX void debug_rom(char * file, int line)
{
#if CONFIG_SERIAL_NONE
#else
    serial_puts("Enter Debugrom mode at ");
    serial_puts(file);
    serial_putc(':');
    serial_put_dword(line);
#if (defined AML_DEBUGROM)||(CONFIG_ENABLE_SPL_MORE_CMD) 
    
    char * cmd=debugrom_startup();
    while(cmd&&*cmd)
    {
        run_cmd(cmd);
        cmd=debugrom_startup();
    }
#endif    
    while(run_cmd(get_cmd()))
    {
        ;
    }
#endif    
}
#ifdef AML_DEBUGROM

void lowlevel_init(void* cur,void * target)
{
	if(cur != target) //r0!=r1
	{
	    //running from spi
    	// take me as a spi rom boot mode
//		romboot_info->por_cfg = POR_INTL_SPI |
//		    (READ_CBUS_REG(ASSIST_POR_CONFIG)&(~POR_INTL_CFG_MASK));
//		romboot_info->boot_id = 0;//boot from spi
	}
	//Adjust 1us timer base
	WRITE_CBUS_REG_BITS(PREG_CTLREG0_ADDR,CONFIG_CRYSTAL_MHZ,4,5);
	/*
        Select TimerE 1 us base
    */
	clrsetbits_le32(P_ISA_TIMER_MUX,0x7<<8,0x1<<8);
	serial_init_firmware((__plls.uart&(~0xfff))|(CONFIG_CRYSTAL_MHZ*1000000/(115200*4)));
}
void relocate_init(unsigned __TEXT_BASE,unsigned __TEXT_SIZE)
{
#if CONFIG_SERIAL_NONE
#else    
    while(1)
        debug_rom(__FILE__,__LINE__);
#endif        

}
#endif
