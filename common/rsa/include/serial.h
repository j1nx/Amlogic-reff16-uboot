#include "romboot.h"

#define UART_PORT_0     CBUS_REG_ADDR(UART0_WFIFO)
#define UART_PORT_1     CBUS_REG_ADDR(UART1_WFIFO)
#define UART_PORT_2     CBUS_REG_ADDR(UART2_WFIFO)
#define P_AO_UART_WFIFO                 (0xc81004c0)
#define P_AO_RTI_PIN_MUX_REG	        (0xc8100014)
#define UART_WFIFO      (0<<2)
#define UART_RFIFO      (1<<2)
#define UART_CONTROL    (2<<2)
#define UART_STATUS     (3<<2)
#define UART_MISC       (4<<2)
#define UART_PORT_CONS P_AO_UART_WFIFO
#define UART_CLK_SRC CLK_CLK81


#define P_UART(uart_base,reg)    	(uart_base+reg)
#define P_UART_WFIFO(uart_base)   	P_UART(uart_base,UART_WFIFO)
#define P_UART_RFIFO(uart_base)   	P_UART(uart_base,UART_RFIFO)

#define P_UART_CONTROL(uart_base)    P_UART(uart_base,UART_CONTROL)
    #define UART_CNTL_MASK_BAUD_RATE                (0xfff)
    #define UART_CNTL_MASK_TX_EN                    (1<<12)
    #define UART_CNTL_MASK_RX_EN                    (1<<13)
    #define UART_CNTL_MASK_2WIRE                    (1<<15)
    #define UART_CNTL_MASK_STP_BITS                 (3<<16)
    #define UART_CNTL_MASK_STP_1BIT                 (0<<16)
    #define UART_CNTL_MASK_STP_2BIT                 (1<<16)
    #define UART_CNTL_MASK_PRTY_EVEN                (0<<18)
    #define UART_CNTL_MASK_PRTY_ODD                 (1<<18)
    #define UART_CNTL_MASK_PRTY_TYPE                (1<<18)
    #define UART_CNTL_MASK_PRTY_EN                  (1<<19)
    #define UART_CNTL_MASK_CHAR_LEN                 (3<<20)
    #define UART_CNTL_MASK_CHAR_8BIT                (0<<20)
    #define UART_CNTL_MASK_CHAR_7BIT                (1<<20)
    #define UART_CNTL_MASK_CHAR_6BIT                (2<<20)
    #define UART_CNTL_MASK_CHAR_5BIT                (3<<20)
    #define UART_CNTL_MASK_RST_TX                   (1<<22)
    #define UART_CNTL_MASK_RST_RX                   (1<<23)
    #define UART_CNTL_MASK_CLR_ERR                  (1<<24)
    #define UART_CNTL_MASK_INV_RX                   (1<<25)
    #define UART_CNTL_MASK_INV_TX                   (1<<26)
    #define UART_CNTL_MASK_RINT_EN                  (1<<27)
    #define UART_CNTL_MASK_TINT_EN                  (1<<28)
    #define UART_CNTL_MASK_INV_CTS                  (1<<29)
    #define UART_CNTL_MASK_MASK_ERR                 (1<<30)
    #define UART_CNTL_MASK_INV_RTS                  (1<<31)
#define P_UART_STATUS(uart_base)  P_UART(uart_base,UART_STATUS )
    #define UART_STAT_MASK_RFIFO_CNT                (0x3f<<0)
    #define UART_STAT_MASK_TFIFO_CNT                (0x3f<<8)
    #define UART_STAT_MASK_PRTY_ERR                 (1<<16)
    #define UART_STAT_MASK_FRAM_ERR                 (1<<17)
    #define UART_STAT_MASK_WFULL_ERR                (1<<18)
    #define UART_STAT_MASK_RFIFO_FULL               (1<<19)
    #define UART_STAT_MASK_RFIFO_EMPTY              (1<<20)
    #define UART_STAT_MASK_TFIFO_FULL               (1<<21)
    #define UART_STAT_MASK_TFIFO_EMPTY              (1<<22)
#define P_UART_MISC(uart_base)    P_UART(uart_base,UART_MISC)
#define writel(val,reg) (*((volatile unsigned *)reg))=(val)
#define readl(reg)		(*((volatile unsigned *)reg))
#define setbits_le32(reg,val)	(*((volatile unsigned *)reg))|=val
#define clrbits_le32(reg,val)	(*((volatile unsigned *)reg))&=(~val)
#define clrsetbits_le32(reg,clr,set)	{unsigned __v=readl(reg);__v&=(~clr);__v|=set;writel(__v,reg);}


extern void serial_set_pin_port(unsigned port_base);
extern void serial_init(unsigned set);
extern int serial_getc(void);
extern void serial_puts(const char *s);
extern void send_ident_version(void);
extern int ser_write_mem(serial_pack_head_t * pack);
extern int ser_function(serial_pack_head_t * pack);
extern int ser_unlock(serial_pack_head_t * pack);
extern int serial_boot(void);
extern int serial_break(void);
extern int ser_ready(serial_pack_head_t * pack);

