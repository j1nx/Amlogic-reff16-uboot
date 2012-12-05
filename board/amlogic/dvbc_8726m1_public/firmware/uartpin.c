#include <config.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/uart.h>
#include <asm/arch/firm/io.h>
#ifndef FIRMWARE_IN_ONE_FILE
#define STATIC_PREFIX
#else
#define STATIC_PREFIX static
#endif
#if (BOARD_UART_PORT!=UART_PORT_0) && (BOARD_UART_PORT!=UART_PORT_1)
#if CONFIG_AML_DEF_UART==0
#define BOARD_UART_PORT UART_PORT_0
#elif CONFIG_AML_DEF_UART==1
#define BOARD_UART_PORT UART_PORT_1
#else
#error IPL BOARD_UART_PORT configure Error .
#endif
#endif
//#define BOARD_UART_PORT UART_PORT_0
STATIC_PREFIX void serial_hw_init(void)
{
#if    BOARD_UART_PORT== UART_PORT_0
        setbits_le32(P_PERIPHS_PIN_MUX_2,((1<<15)|(1<<11)));

	    //add uart rx pull up for M1 mboxboard  W10(GPIOB_3)
	    WRITE_CBUS_REG( PAD_PULL_UP_REG0, READ_CBUS_REG(PAD_PULL_UP_REG0) & ~(1<<24) );
#else    
//    if(port==UART_B){
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, ((1 << 15) | (1 << 19))); //Audio Out
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_8, ((1 << 8) | (1 << 8))); //Audio In
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, ((1 << 16) | (1 << 20))); //I2C Master B
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, ((1 << 17) | (1 << 21))); //I2C Slave
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6, ((1 << 25) | (1 << 26))); //PWM
        CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_0, (1 << 29)); //JTAG enable
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_0, (1 << 30)); //JTAG disable
        SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_1,((1 << 18) | (1 << 22)));
#endif        
//    }
    ///TODO , add pinmux setting
}
