#include <config.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/uart.h>
#include <asm/arch/firm/io.h>
#ifndef FIRMWARE_IN_ONE_FILE
#define STATIC_PREFIX
#else
#define STATIC_PREFIX static
#endif
#if CONFIG_AML_DEF_UART==UART_B
#define BOARD_UART_PORT UART_PORT_1
#endif
#if CONFIG_AML_DEF_UART==UART_A
#define BOARD_UART_PORT UART_PORT_0
#endif

STATIC_PREFIX void serial_hw_init(void)
{
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, ((1 << 4) | (1 << 5)));
}
