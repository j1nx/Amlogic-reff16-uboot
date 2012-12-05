#include <asm/arch/am_regs.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/gpio.h>

static  int powerkey_hold_count = -1; 
int board_ver = 0;

inline int get_powerkey_hold_count(void)
{
    return  powerkey_hold_count;
}

void power_hold(void)
{   
    //power hold
    //printf("power hold\n");

	clear_mio_mux(12, 1<<7);		//disable ENC_6
	clear_mio_mux(0, (1<<9)|(1<<22));		//disable TCON_CPH3 LED_BL_PWM
	clear_mio_mux(2, (1<<31)|(1<<29));		//disable PWM_B DEMOD_RF_PWM
	clear_mio_mux(9, 1<<22);		//disable WIFI Debug

	set_gpio_mode(GPIOA_bank_bit(7), GPIOA_bit_bit0_14(7), GPIO_OUTPUT_MODE);   // OEN = 0 (output) 
	set_gpio_val(GPIOA_bank_bit(7), GPIOA_bit_bit0_14(7), 1);   // Output level = 1 (pull high)
    //*(volatile unsigned long *)(0xC1108030) &= ~(1 << 12);              // OEN = 0 (output) 
    //*(volatile unsigned long *)(0xC1108034) |= (1 << 12);               // Output level = 1 (pull high)
}
void power_unhold(void)
{   
    //power unhold
    //printf("power hold\n");

	clear_mio_mux(12, 1<<7);		//disable ENC_6
	clear_mio_mux(0, (1<<9)|(1<<22));		//disable TCON_CPH3 LED_BL_PWM
	clear_mio_mux(2, (1<<31)|(1<<29));		//disable PWM_B DEMOD_RF_PWM
	clear_mio_mux(9, 1<<22);		//disable WIFI Debug

	set_gpio_mode(GPIOA_bank_bit(7), GPIOA_bit_bit0_14(7), GPIO_OUTPUT_MODE);   // OEN = 0 (output) 
	set_gpio_val(GPIOA_bank_bit(7), GPIOA_bit_bit0_14(7), 0);   // Output level = 0 (pull high)
    //*(volatile unsigned long *)(0xC1108030) &= ~(1 << 12);              // OEN = 0 (output) 
    //*(volatile unsigned long *)(0xC1108034) &= ~(1 << 12);               // Output level = 0 (pull low)
}

static int powerkey_init(void)
{
    if(board_ver == 0){
        WRITE_CBUS_REG(0x21d0/*RTC_ADDR0*/, (READ_CBUS_REG(0x21d0/*RTC_ADDR0*/) &~(1<<11)));
        WRITE_CBUS_REG(0x21d1/*RTC_ADDR0*/, (READ_CBUS_REG(0x21d1/*RTC_ADDR0*/) &~(1<<3)));
    }  
    return 1;
}
int powerkey_scan(void)
{
    if(board_ver == 0)
        return ((READ_CBUS_REG(0x21d1/*RTC_ADDR1*/) >> 2) & 1) ? 0 : 1;
    else
        return (READ_CBUS_REG(ASSIST_HW_REV)&(1<<10))? 0:1;  //GP_INPUT2  bit 10
}



int powerkey_hold(unsigned long hold_time)
{
    unsigned long polling_time = 100, tmp;

    if(powerkey_hold_count == -1)
    {
        powerkey_init();
        
    }

    if(!hold_time)
    {
        tmp = powerkey_scan();
        printf("powerkey: %d\n", tmp);
        if((!tmp) || (powerkey_hold_count < 0))
        {
            powerkey_hold_count = -2;
            return  0;
        }
        else
        {
            return  ++powerkey_hold_count;
        }
    }

    while(hold_time > 0)
    {
        mdelay(polling_time);
        tmp = powerkey_scan();
        //printf("powerkey: %d\n", tmp);
        if(!tmp)  break;
        if(hold_time <= polling_time)
        {
            hold_time = 0;
        }
        else
        {
            hold_time -= polling_time;
        }
    }
    if(hold_time > 0)
    {
        return  0;
    }
    return  1;
}
