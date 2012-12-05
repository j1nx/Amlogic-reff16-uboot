#include <common.h>
#include <stdio_dev.h>
#include <asm/arch/romboot.h>






/*
 * Output a single byte to the serial port.
 */
static
void  jtag_putc(const char c)
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
static
int  jtag_tstc(void)
{
	return (romboot_info->input.rd^romboot_info->input.wr)&CONFIG_JTAG_CONSOLE_OUTPUT_MASK;
}

/*
 * Read a single byte from the serial port.
 */
static
int  jtag_getc(void)
{
    unsigned char ch;
    while( jtag_tstc()==0);
    ch=romboot_info->input.buf[(romboot_info->input.rd++)&CONFIG_JTAG_CONSOLE_OUTPUT_MASK];
    return ((int)ch);

}


static
void  jtag_puts(const char *s)
{
    while (*s) {
         jtag_putc(*s++);
    }
}




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