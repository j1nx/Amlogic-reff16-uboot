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

static unsigned int TCON_PWM_HE_MAX;
static unsigned int TCON_PWM_VS_MAX;

static unsigned int TCON_PWM_HS1;
static unsigned int TCON_PWM_HE1;
static unsigned int TCON_PWM_HS2;
static unsigned int TCON_PWM_HE2;
static unsigned int TCON_PWM_HS3;
static unsigned int TCON_PWM_HE3;

static unsigned int TCON_PWM_VS1;
static unsigned int TCON_PWM_VS2;
static unsigned int TCON_PWM_VS3;
   
static int ver = 2;

void init_pwm_tcon()
{
    unsigned long arg = simple_strtoul(getenv ("768p"), NULL, 16);
    if(1 == arg){
       TCON_PWM_HE_MAX = 1559;
       TCON_PWM_VS_MAX = 805;
    }else{
       TCON_PWM_HE_MAX = 2199;
       TCON_PWM_VS_MAX = 1126;
    }

    TCON_PWM_HS1 = 0;
    TCON_PWM_HE1 = TCON_PWM_HE_MAX-1;
    TCON_PWM_HS2 = 0;
    TCON_PWM_HE2 = TCON_PWM_HE_MAX-1;
    TCON_PWM_HS3 = 0;
    TCON_PWM_HE3 = TCON_PWM_HE_MAX-1;    

    TCON_PWM_VS1 = 0;
    TCON_PWM_VS2 = TCON_PWM_VS_MAX/3;
    TCON_PWM_VS3 = 2*TCON_PWM_VS_MAX/3;
}

void power_on_backlight(void)
{        
    if(ver == 2)
    {
        clear_mio_mux(2, 1<<17);
        clear_mio_mux(4, 1<<12);
        clear_mio_mux(3, 1<<3);
        clear_mio_mux(7, 1<<20);
        clear_mio_mux(10, 1<<6);

        set_gpio_val(GPIOX_bank_bit32_63(53), GPIOX_bit_bit32_63(53), 0);
        set_gpio_mode(GPIOX_bank_bit32_63(53), GPIOX_bit_bit32_63(53), 0);
    }
    else
    {
        clear_mio_mux(10, 1<<13);
    	clear_mio_mux(4, 1<<5);
    	clear_mio_mux(0, 1<<1);

        set_gpio_val(GPIOB_bank_bit0_8(6), GPIOB_bit_bit0_8(6), 1);
        set_gpio_mode(GPIOB_bank_bit0_8(6), GPIOB_bit_bit0_8(6), GPIO_OUTPUT_MODE);
    }


	//Init Analog pwm pinmux
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_4, (1<<11));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, 1);
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_3, (1<<1));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7, (1<<18));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_10, (1<<6));
	
    SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_1, (1<<24));
    WRITE_CBUS_REG(PWM_MISC_REG_AB, 0x2);
    WRITE_CBUS_REG(PWM_PWM_B, 0);


    //Init Digital pwm pinmux, tcon GPIOB2, GPIOB3,GPIOB4
    SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_0, (1<<4)|(1<<5)|(1<<3));


    WRITE_CBUS_REG_BITS(TCON_MISC_SEL_ADDR, 1, OEH_SEL, 1);
    WRITE_CBUS_REG(OEH_HS_ADDR, TCON_PWM_HS1);
    WRITE_CBUS_REG(OEH_HE_ADDR, TCON_PWM_HE1);
    WRITE_CBUS_REG(OEH_VS_ADDR, TCON_PWM_VS1);

    
    WRITE_CBUS_REG(OEH_VE_ADDR, TCON_PWM_VS2);//180HZ


    WRITE_CBUS_REG_BITS(TCON_MISC_SEL_ADDR, 1, CPV1_SEL, 1);
    WRITE_CBUS_REG(CPV1_HS_ADDR, TCON_PWM_HS2);
    WRITE_CBUS_REG(CPV1_HE_ADDR, TCON_PWM_HE2);
    WRITE_CBUS_REG(CPV1_VS_ADDR, TCON_PWM_VS2);
    WRITE_CBUS_REG(CPV1_VE_ADDR, TCON_PWM_VS2+TCON_PWM_VS2);

	WRITE_CBUS_REG_BITS(TCON_MISC_SEL_ADDR, 1, OEV2_SEL, 1);
	WRITE_CBUS_REG_BITS(TCON_MISC_SEL_ADDR, 1, OEV1_SEL, 1);
	WRITE_CBUS_REG_BITS(TCON_MISC_SEL_ADDR, 1, OEV3_SEL, 1);
	CLEAR_CBUS_REG_MASK(TCON_MISC_SEL_ADDR, 1<<OEV_UNITE);
    WRITE_CBUS_REG(OEV1_HS_ADDR, TCON_PWM_HS3);
    WRITE_CBUS_REG(OEV1_HE_ADDR, TCON_PWM_HE3);
    WRITE_CBUS_REG(OEV1_VS_ADDR, TCON_PWM_VS3);
    WRITE_CBUS_REG(OEV1_VE_ADDR, TCON_PWM_VS3+TCON_PWM_VS2);
}

void power_off_backlight(void)
{             
    if(ver == 2)
    {
        clear_mio_mux(2, 1<<17);
        clear_mio_mux(4, 1<<12);
        clear_mio_mux(3, 1<<3);
        clear_mio_mux(7, 1<<20);
        clear_mio_mux(10, 1<<6);

        set_gpio_val(GPIOX_bank_bit32_63(53), GPIOX_bit_bit32_63(53), 1);
        set_gpio_mode(GPIOX_bank_bit32_63(53), GPIOX_bit_bit32_63(53), 0);
    }
    else
    {
        clear_mio_mux(10, 1<<13);
    	clear_mio_mux(4, 1<<5);
    	clear_mio_mux(0, 1<<1);

        set_gpio_val(GPIOB_bank_bit0_8(6), GPIOB_bit_bit0_8(6), 0);
        set_gpio_mode(GPIOB_bank_bit0_8(6), GPIOB_bit_bit0_8(6), GPIO_OUTPUT_MODE);  
    }
}

void panel_on()
{
    set_gpio_val(GPIOX_bank_bit32_63(54), GPIOX_bit_bit32_63(54), 1);
    set_gpio_mode(GPIOX_bank_bit32_63(54), GPIOX_bit_bit32_63(54), 0);
}

void panel_off()
{
    set_gpio_val(GPIOX_bank_bit32_63(54), GPIOX_bit_bit32_63(54), 0);
    set_gpio_mode(GPIOX_bank_bit32_63(54), GPIOX_bit_bit32_63(54), 0);
}

static unsigned bl_level;
static unsigned aml_8726m_get_bl_level(void)
{
    return bl_level;
}

void aml_8726m_set_bl_level(unsigned level)
{
   unsigned high, low;

    if (level > 255)
        level = 255;

    //Analog pwm
    high = level*4;
    low = ANALOG_BL_MAX_LEVEL - high;
    WRITE_CBUS_REG_BITS(PWM_PWM_B, low, 0, 16);
    WRITE_CBUS_REG_BITS(PWM_PWM_B, high, 16, 16);

    //Digital pwm
    WRITE_CBUS_REG(OEH_VE_ADDR, (int)((level*TCON_PWM_VS2)/255));
    WRITE_CBUS_REG(CPV1_VE_ADDR, TCON_PWM_VS2 + (int)((level*TCON_PWM_VS2)/255));
    WRITE_CBUS_REG(OEV1_VE_ADDR, TCON_PWM_VS3 + (int)((level*TCON_PWM_VS2)/255));
}

blinfo_t bl_info = 
{
	.max_bl_level	=	255,
	.bl_on	=	power_on_backlight,
	.bl_off	=	power_off_backlight,
	.panel_on = panel_on,
	.panel_off = panel_off,
	.set_bl_level	=	aml_8726m_set_bl_level,
};
