#include <config.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/uart.h>
#include <asm/arch/firm/io.h>
#ifndef FIRMWARE_IN_ONE_FILE
#define STATIC_PREFIX
#else
#define STATIC_PREFIX static
#endif

STATIC_PREFIX void serial_hw_init(void)
{
/*    
    if(BOARD_UART_PORT==UART_A)
    {
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, ((1 << 23) | (1 << 24)));
    }
    else if(BOARD_UART_PORT==UART_B)
    {
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, ((1 << 11) | (1 << 12)));
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_5, ((1 << 23) | (1 << 24)));
    }
*/
    //GPIOAO_0==tx,GPIOAO_1==rx
    setbits_le32(P_AO_RTI_PIN_MUX_REG,3<<11);
}
