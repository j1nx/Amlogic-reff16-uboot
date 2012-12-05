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
//        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, ((1 << 11) | (1 << 12)));
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, ((1 << 27) | (1 << 30)));
}
