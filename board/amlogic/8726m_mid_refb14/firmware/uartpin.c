#include <config.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/uart.h>
#include <asm/arch/firm/io.h>
#ifndef FIRMWARE_IN_ONE_FILE
#define STATIC_PREFIX
#else
#define STATIC_PREFIX static
#endif
#ifndef BOARD_UART_PORT
#define BOARD_UART_PORT CONFIG_AML_DEF_UART
#endif
STATIC_PREFIX void serial_hw_init(void)
{
    if(BOARD_UART_PORT==UART_A)
    {
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_7, ((1 << 18) | (1 << 19)));
    }
    else if(BOARD_UART_PORT==UART_B)
    {
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_4, (1 << 10)); //disable HDMI pin
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_4, (1 << 1)); //disable TCON pin
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, ((1 << 28)|(1 << 25))); //disable I2C Master B pin
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, ((1 << 29)|(1 << 26))); //disable I2C Slave pin
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7, ((1 << 4)|(1 << 5))); //disable TS Out pin
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_9, ((1 << 13)|(1 << 14))); //disable WiFi Debug pin
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_10, ((1 << 13)|(1 << 14))); //disable Demod Debug pin
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, (1 << 27));
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, (1 << 30));
    }
}
