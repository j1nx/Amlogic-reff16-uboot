/*
 * AMLOGIC T13 LCD panel driver.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the named License,
 * or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Author:  Jinlin Xia <jinlin.xia@amlogic.com>
 *
 */

#include <common.h>
#include <asm/arch/am_regs.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/gpio.h>
#include <bl_aml.h>

#define ANALOG_BL_MAX_LEVEL    1020

void power_on_backlight(void)
{
	clear_mio_mux(10, 1<<13);
	clear_mio_mux(4, 1<<5);
	set_mio_mux(1, 1<<1);
	clear_mio_mux(0, 1<<1);

    //GPIOX_53
    printf("backlight power on\n");
    set_gpio_val(GPIOX_bank_bit32_63(53), GPIOX_bit_bit32_63(53), 1);
    set_gpio_mode(GPIOX_bank_bit32_63(53), GPIOX_bit_bit32_63(53), GPIO_OUTPUT_MODE);
	  printf("bank:0x%x, bit:0x%x\n", GPIOX_bank_bit32_63(53), GPIOX_bit_bit32_63(53));

	//Init Analog pwm pinmux
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_4, (1<<11));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, 1);
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, (1<<1));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7, (1<<18));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_10, (1<<6));
	
    //SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, (1<<24));
    //WRITE_CBUS_REG(PWM_MISC_REG_AB, 0x2);
    //WRITE_CBUS_REG(PWM_PWM_B, 0);
}

void power_off_backlight(void)
{     
	clear_mio_mux(10, 1<<13);
	clear_mio_mux(4, 1<<5);
	set_mio_mux(1, 1<<1);
	clear_mio_mux(0, 1<<1);
	
    //GPIOX_53
    printf("backlight power off\n");
    set_gpio_val(GPIOX_bank_bit32_63(53), GPIOX_bit_bit32_63(53), 0);
    set_gpio_mode(GPIOX_bank_bit32_63(53), GPIOX_bit_bit32_63(53), GPIO_OUTPUT_MODE);  
	printf("bank:0x%x, bit:0x%x\n", GPIOX_bank_bit32_63(53), GPIOX_bit_bit32_63(53));
}


static unsigned bl_level;
static unsigned aml_8726m_get_bl_level(void)
{
    return bl_level;
}

void aml_8726m_set_bl_level(unsigned level)
{
   unsigned iLevel, iHz, high, low;

    if (level > 0x1ff)
        level = 0x1ff;

    iLevel = level & 0xff;
    iHz = (level & 0x100) >> 8;

    //Analog pwm
    high = iLevel*4;
    low = ANALOG_BL_MAX_LEVEL - high;
    WRITE_CBUS_REG_BITS(PWM_PWM_B, low, 0, 16);
    WRITE_CBUS_REG_BITS(PWM_PWM_B, high, 16, 16);
}

blinfo_t bl_info = 
{
	.max_bl_level	=	255,
	.bl_on	=	power_on_backlight,
	.bl_off	=	power_off_backlight,
	.set_bl_level	=	aml_8726m_set_bl_level,
};
