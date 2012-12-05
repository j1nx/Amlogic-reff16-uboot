// UART.C
//
// Generic software uart written in C, requiring a timer set to 3 times
// the baud rate, and two software read/write pins for the receive and
// transmit functions.
//
// * Received characters are buffered
// * putchar(), getchar(), kbhit() and flush_input_buffer() are available
// * There is a facility for background processing while waiting for input
//
// Colin Gittins, Software Engineer, Halliburton Energy Services
//
// The baud rate can be configured by changing the BAUD_RATE macro as
// follows:
//
// #define BAUD_RATE			19200.0
//
// The function init_uart() must be called before any comms can take place
//
// Interface routines required:
// 1. get_rx_pin_status()
//    Returns 0 or 1 dependent on whether the receive pin is high or low.
// 2. set_tx_pin_high()
//    Sets the transmit pin to the high state.
// 3. set_tx_pin_low()
//    Sets the transmit pin to the low state.
// 4. idle()
//    Background functions to execute while waiting for input.
// 5. timer_set( BAUD_RATE )
//    Sets the timer to 3 times the baud rate.
// 6. set_timer_interrupt( timer_isr )
//    Enables the timer interrupt.
//
// Functions provided:
// 1. void flush_input_buffer( void )
//    Clears the contents of the input buffer.
// 2. char kbhit( void )
//    Tests whether an input character has been received.
// 3. char getchar( void )
//    Reads a character from the input buffer, waiting if necessary.
// 4. void turn_rx_on( void )
//    Turns on the receive function.
// 5. void turn_rx_off( void )
//    Turns off the receive function.
// 6. void putchar( char )
//    Writes a character to the serial port.

STATIC_PREFIX void timer_set(unsigned baud);
STATIC_PREFIX char get_rx_pin_status();
STATIC_PREFIX void set_tx_pin_high();
STATIC_PREFIX void set_tx_pin_low();
STATIC_PREFIX void idle();
STATIC_PREFIX
void set_timer_interrupt(void (* isr)(void));
//#define BAUD_RATE		2400
#define BAUD_RATE		19200
//#define BAUD_RATE		115200
//#define BAUD_RATE       57600
#define IN_BUF_SIZE		256
static unsigned char		qin = 0;
static unsigned char		qout = 0;

#define FALSE 0
#define TRUE 1
static unsigned char		inbuf[IN_BUF_SIZE];

static char 			flag_rx_waiting_for_stop_bit;
static char 			flag_rx_off;
static char 			rx_mask;
static char 			flag_rx_ready;
static char 			flag_tx_ready;
static char 			timer_rx_ctr;
static char 			timer_tx_ctr;
static char 			bits_left_in_rx;
static char 			bits_left_in_tx;
static char 			rx_num_of_bits;
static char 			tx_num_of_bits;
static char			internal_rx_buffer;
static unsigned		internal_tx_buffer;
static char			user_tx_buffer;

void timer_isr(void)
	{
	char			mask, start_bit, flag_in;

// Transmitter Section
	if ( flag_tx_ready )
		{
		if ( --timer_tx_ctr<=0 )
			{
			mask = internal_tx_buffer&1;
			internal_tx_buffer >>= 1;
			if ( mask )
				{
				set_tx_pin_high();
				}
			else
				{
				set_tx_pin_low();
				}
			timer_tx_ctr = 3;
			if ( --bits_left_in_tx<=0 )
				{
				flag_tx_ready = FALSE;
				}
			}
		}
// Receiver Section
	if ( flag_rx_off==FALSE )
		{
		if ( flag_rx_waiting_for_stop_bit )
			{
			if ( --timer_rx_ctr<=0 )
				{
				flag_rx_waiting_for_stop_bit = FALSE;
				flag_rx_ready = FALSE;
				internal_rx_buffer &= 0xFF;
				if ( internal_rx_buffer!=0xC2 )
					{
					inbuf[qin] = internal_rx_buffer;
					if ( ++qin>=IN_BUF_SIZE )
						{
						qin = 0;
						}
					}
				}
			}
		else		// rx_test_busy
			{
			if ( flag_rx_ready==FALSE )
				{
				start_bit = get_rx_pin_status();
// Test for Start Bit
				if ( start_bit==0 )
					{
					flag_rx_ready = TRUE;
					internal_rx_buffer = 0;
					timer_rx_ctr = 4;
					bits_left_in_rx = rx_num_of_bits;
					rx_mask = 1;
					}
				}
			else	// rx_busy
				{
				if ( --timer_rx_ctr<=0 )
					{				// rcv
					timer_rx_ctr = 3;
					flag_in = get_rx_pin_status();
					if ( flag_in )
						{
						internal_rx_buffer |= rx_mask;
						}
					rx_mask <<= 1;
					if ( --bits_left_in_rx<=0 )
						{
						flag_rx_waiting_for_stop_bit = TRUE;
						}
					}
				}
			}
		}
	}
STATIC_PREFIX
void init_uart( void )
	{
	flag_tx_ready = FALSE;
	flag_rx_ready = FALSE;
	flag_rx_waiting_for_stop_bit = FALSE;
	flag_rx_off = FALSE;
	rx_num_of_bits = 10;
	tx_num_of_bits = 10;

	set_tx_pin_low();

	timer_set( BAUD_RATE );
	set_timer_interrupt( timer_isr ); 	// Enable timer interrupt
	}
STATIC_PREFIX
char _getchar( void )
	{
	char		ch;

	do
		{
		while ( qout==qin )
			{
			idle();
			}
		ch = inbuf[qout] & 0xFF;
		if ( ++qout>=IN_BUF_SIZE )
			{
			qout = 0;
			}
		}
	while ( ch==0x0A || ch==0xC2 );
	return( ch );
	}
STATIC_PREFIX
void _putchar( char ch )
	{
	while ( flag_tx_ready );
	user_tx_buffer = ch;

// invoke_UART_transmit
	timer_tx_ctr = 3;
	bits_left_in_tx = tx_num_of_bits;
	internal_tx_buffer = (user_tx_buffer<<1) | 0x200;
	flag_tx_ready = TRUE;
	}
STATIC_PREFIX
void flush_input_buffer( void )
	{
	qin = 0;
	qout = 0;
	}
STATIC_PREFIX
char kbhit( void )
	{
	return( qin!=qout );
	}
STATIC_PREFIX
void turn_rx_on( void )
	{
	flag_rx_off = FALSE;
	}
STATIC_PREFIX
void turn_rx_off( void )
	{
	flag_rx_off = TRUE;
	}
#include "interrupts.c"	
static void (* timer)(void)=NULL;
static unsigned max_timer=0;
_Interrupt2 void timer_fiq(void)
{
    write_new_aux_reg(ARC_REG_TIMER1_CNT,0);
    write_new_aux_reg(ARC_REG_TIMER0_CTRL,0);
    write_new_aux_reg(ARC_REG_TIMER0_CTRL,3);
    if(timer)
        timer();
    max_timer=max_timer>read_new_aux_reg(ARC_REG_TIMER1_CNT)?max_timer:read_new_aux_reg(ARC_REG_TIMER1_CNT);
}
STATIC_PREFIX
void timer_set(unsigned baud)
{
    unsigned limit;
    limit=600000000/(3* baud);
    write_new_aux_reg(ARC_REG_TIMER0_LIMIT,limit);
    write_new_aux_reg(ARC_REG_TIMER0_CNT,0);
    write_new_aux_reg(ARC_REG_TIMER0_CTRL,3);
}
STATIC_PREFIX
void set_timer_interrupt(void (* isr)(void))
{
    timer=isr;
}
#define USE_JTAG_PIN 1
STATIC_PREFIX
char get_rx_pin_status()
{
#ifdef USE_JTAG_PIN    
    return (READ_CBUS_REG(0x200e)>>14)&1;
#else
    return (READ_CBUS_REG(0x2014)>>15)&1;
#endif
}
STATIC_PREFIX
void set_tx_pin_high()
{
    
    
#ifdef USE_JTAG_PIN    
    WRITE_CBUS_REG(0x200d,(0x1<<17)|READ_CBUS_REG(0x200d));
#else
    WRITE_CBUS_REG(0x2013,1<<14);
#endif
}
STATIC_PREFIX
void set_tx_pin_low()
{
    
#ifdef USE_JTAG_PIN        
    WRITE_CBUS_REG(0x200d,(~(0x1<<17))&READ_CBUS_REG(0x200d));
#else
    WRITE_CBUS_REG(0x2013,0<<14);
#endif
}
STATIC_PREFIX
void idle()
{
    asm("sleep");
}
STATIC_PREFIX
int serial_tstc(void){
    return kbhit();
}
STATIC_PREFIX
int serial_getc(void){
    return _getchar();
}
STATIC_PREFIX
void serial_putc(const char c)
{
    if (c == '\n')
        _putchar('\r');
    return _putchar(c);
}
extern void ( timer0_service)(void);
STATIC_PREFIX
unsigned serial_init_firmware(unsigned set){
#ifdef USE_JTAG_PIN
    clrsetbits_le32(P_PERIPHS_PIN_MUX_1,3,2);
    clrbits_le32(P_PERIPHS_PIN_MUX_5,3<<11);
    clrbits_le32(P_PERIPHS_PIN_MUX_0,1<<11);
    clrbits_le32(P_PERIPHS_PIN_MUX_1,0xff<<10);
    WRITE_CBUS_REG(0x200c,1<<14);
#else    
    clrbits_le32(P_PERIPHS_PIN_MUX_4,1<<11);
    clrbits_le32(P_PERIPHS_PIN_MUX_7,3<<6);
    WRITE_CBUS_REG(0x2012,1<<15);
#endif
    romboot_info->getc=serial_getc;
    romboot_info->putc=serial_putc;
    romboot_info->tstc=serial_tstc;
    init_interrupt();
    set_int_level(3,1);
    _setvect2(3,&timer0_service);
    init_uart();
    enable_interrupts();
    return 0;
}
