/* 
 * sn7325 i2c interface
 * Copyright (C) 2010 Amlogic, Inc.
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
 * Author:  elvis yu<elvis.yu@amlogic.com>
 */  

#include <asm/arch/pinmux.h>
#include <asm/arch/gpio.h>
#include <asm/arch/i2c.h>
#include <aml_i2c.h>
#include <sn7325.h>


//#define __DBG__
#ifdef __DBG__
#define debug(fmt,args...) do { printf("[DEBUG]: FILE:%s:%d, FUNC:%s--- "fmt"\n",\
                                                     __FILE__,__LINE__,__func__,## args);} \
                                         while (0)
#else
#define debug(fmt,args...)
#endif

void    sn7325_init()
{
    debug("sn7325 reset, address=0x%x", SN7325_ADDR);
    sn7325_pwr_rst();
}

static unsigned char sn7325_i2c_read(unsigned char reg)
{
    unsigned char val = 0;
    struct i2c_msg msgs[] = {
        {
            .addr = SN7325_ADDR,
            .flags = 0,
            .len = 1,
            .buf = &reg,
        },
        {
            .addr = SN7325_ADDR,
            .flags = I2C_M_RD,
            .len = 1,
            .buf = &val,
        }
    };

    if ( aml_i2c_xfer(msgs, 2)< 0) {
        printf("%s: i2c transfer failed\n", __FUNCTION__);
    }

    return val;
}

static void sn7325_i2c_write(unsigned char reg, unsigned char val)
{
    unsigned char buff[2];
    buff[0] = reg;
    buff[1] = val;
    struct i2c_msg msg[] = {
        {
        .addr = SN7325_ADDR,
        .flags = 0,
        .len = 2,
        .buf = buff,
        }
    };

    if (aml_i2c_xfer(msg, 1) < 0) {
        printf("%s: i2c transfer failed\n", __FUNCTION__);
    }
}

inline void sn7325_set_io_dir(unsigned short val)
{
    sn7325_i2c_write(0x4, (val & 0xff));
    sn7325_i2c_write(0x5, (val >> 8));
}

inline unsigned short sn7325_get_io_dir(void)
{
    return  ((sn7325_i2c_read(0x5) << 8) | sn7325_i2c_read(0x4));
}

inline void sn7325_set_output_level(unsigned short val)
{
    sn7325_i2c_write(0x2, (val & 0xff));
    sn7325_i2c_write(0x3, (val >> 8));
}

inline unsigned short  sn7325_get_output_level(unsigned short val)
{
    return  ((sn7325_i2c_read(0x3) << 8) | sn7325_i2c_read(0x2));
}

inline unsigned short sn7325_get_input_level(void)
{
    return  ((sn7325_i2c_read(0x1) << 8) | sn7325_i2c_read(0x0));
}