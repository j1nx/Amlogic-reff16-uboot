#include <asm/arch/romboot.h>
STATIC_PREFIX
unsigned serial_init_firmware(unsigned set)
{
static unsigned tag=0;
    if(tag)
        return tag;    
    romboot_info->input.mask=CONFIG_JTAG_CONSOLE_INPUT_MASK;
    romboot_info->input.buf=(typeof(romboot_info->output.buf))CONFIG_JTAG_CONSOLE_INPUT_BUF;
    romboot_info->input.wr=0;
    romboot_info->input.rd=0;
	
    romboot_info->output.mask=CONFIG_JTAG_CONSOLE_OUTPUT_MASK;
    romboot_info->output.buf=(typeof(romboot_info->output.buf))CONFIG_JTAG_CONSOLE_OUTPUT_BUF;
    romboot_info->output.wr=0;
    romboot_info->output.rd=0;
    tag=1;
	return tag;
}

/*
 * Output a single byte to the serial port.
 */
STATIC_PREFIX
void serial_putc(const char c)
{
    romboot_info->output.buf[(romboot_info->output.wr++)&CONFIG_JTAG_CONSOLE_OUTPUT_MASK]=c;
    if((romboot_info->output.wr&CONFIG_JTAG_CONSOLE_OUTPUT_MASK)==
        (romboot_info->output.rd&CONFIG_JTAG_CONSOLE_OUTPUT_MASK)
      )
      romboot_info->output.rd++; // remove 1 char 

}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise 0.
 */
STATIC_PREFIX
int serial_tstc(void)
{
	return (romboot_info->input.rd^romboot_info->input.wr)&CONFIG_JTAG_CONSOLE_OUTPUT_MASK;
}

/*
 * Read a single byte from the serial port. 
 */
STATIC_PREFIX
int serial_getc(void)
{
    unsigned char ch;   
    while(serial_tstc()==0);
    ch=romboot_info->input.buf[(romboot_info->input.rd++)&CONFIG_JTAG_CONSOLE_OUTPUT_MASK];
    return ((int)ch);

}




#define serial_put_char(data) serial_puts("0x");serial_put_hex((unsigned)data,8);serial_putc('\n')
#define serial_put_dword(data) serial_puts("0x");serial_put_hex((unsigned)data,32);serial_putc('\n')

#if 0
int drv_jtag_console_init(void)
{
	struct stdio_dev dev;
	int ret;

	memset(&dev, 0x00, sizeof(dev));
	strcpy(dev.name, "jtag");
	dev.flags = DEV_FLAGS_OUTPUT | DEV_FLAGS_INPUT | DEV_FLAGS_SYSTEM;
	dev.putc = jtag_putc;
	dev.puts = jtag_puts;
	dev.tstc = jtag_tstc;
	dev.getc = jtag_getc;

	ret = stdio_register(&dev);
	return (ret == 0 ? 1 : ret);
}
#endif