/*
 * Amlogic M1 
 * frame buffer driver-----------HDMI_TX
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
 */

#include "asm/arch-m1/am_regs.h"
#include "hdmi_tx_reg.h"

unsigned long hdmi_rd_reg(unsigned long addr)
{
    unsigned long data;
    WRITE_APB_REG(HDMI_ADDR_PORT, addr);
    WRITE_APB_REG(HDMI_ADDR_PORT, addr);
    
    data = READ_APB_REG(HDMI_DATA_PORT);
    
    return (data);
}


void hdmi_wr_only_reg(unsigned long addr, unsigned long data)
{
    WRITE_APB_REG(HDMI_ADDR_PORT, addr);
    WRITE_APB_REG(HDMI_ADDR_PORT, addr);
    
    WRITE_APB_REG(HDMI_DATA_PORT, data);
}

void hdmi_wr_reg(unsigned long addr, unsigned long data)
{
    unsigned long rd_data;
    
    WRITE_APB_REG(HDMI_ADDR_PORT, addr);
    WRITE_APB_REG(HDMI_ADDR_PORT, addr);
    
    WRITE_APB_REG(HDMI_DATA_PORT, data);
    rd_data = hdmi_rd_reg (addr);
    if (rd_data != data) 
    {
        //printk("hdmi_wr_reg(%x,%x) fails to write: %x\n",addr, data, rd_data);
       //while(1){};      
    }
}



