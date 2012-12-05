#include <config.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/uart.h>
#include <asm/arch/firm/io.h>
#ifndef FIRMWARE_IN_ONE_FILE
#define STATIC_PREFIX
#else
#define STATIC_PREFIX static
#endif
#define BOARD_UART_PORT UART_PORT_1
STATIC_PREFIX void serial_hw_init(void)
{
    setbits_le32(P_PERIPHS_PIN_MUX_2,((1<<29)|(1<<30)));
    ///TODO , add pinmux setting
}
