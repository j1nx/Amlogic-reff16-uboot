#include <config.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/uart.h>
#include <asm/arch/firm/io.h>
#ifndef FIRMWARE_IN_ONE_FILE
#define STATIC_PREFIX
#else
#define STATIC_PREFIX static
#endif
//#define BOARD_UART_PORT UART_PORT_0
#ifndef BOARD_UART_PORT
#define BOARD_UART_PORT CONFIG_AML_DEF_UART
#endif
STATIC_PREFIX void serial_hw_init(void)
{
    if(BOARD_UART_PORT==UART_A)
    {
//        *(volatile unsigned*)0xc11084c8 = 0x3196;//0x83196;//UART_A
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, ((1 << 23) | (1 << 24)));
    }
    else if(BOARD_UART_PORT==UART_B)
    {
        #if 0
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, ((1 << 11) | (1 << 12)));
//        *(volatile unsigned*)0xc11084e4 = 0x3196;//0x83196;//UART_B
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_5, ((1 << 23) | (1 << 24)));
        
        #else
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_4, ((1 << 10) | (1 << 1)));
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, ((1 << 25) | (1 << 28)| (1 << 26)| (1 << 29)));
       
//        *(volatile unsigned*)0xc11084e4 = 0x3196;//0x83196;//UART_B
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, ((1 << 27) | (1 << 30)));
        #endif
    }
}
