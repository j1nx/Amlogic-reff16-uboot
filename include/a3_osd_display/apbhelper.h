/*******************************************************************
 * 
 *  Copyright C 2008 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: 
 *      Peripheral register helper defines
 *  Author: Amlogic Software
 *  Created: 10/17/2008 3:51:02 PM
 *
 *******************************************************************/
#ifndef AHBHELPER_H
#define AHBHELPER_H

#include "volatile_read.h"
#define AHB_BASE_ADDR               0xc9000000

#define WRITE_AHB_REG(reg, val) *(volatile unsigned *)(AHB_BASE_ADDR + (reg)) = (val)
#define READ_AHB_REG(reg) (READ_VOLATILE_UINT32(AHB_BASE_ADDR + (reg)))

#define CLEAR_AHB_REG_MASK(reg, mask) WRITE_AHB_REG(reg, (READ_AHB_REG(reg)&(~(mask))))
#define SET_AHB_REG_MASK(reg, mask)   WRITE_AHB_REG(reg, (READ_AHB_REG(reg)|(mask)))


#define WRITE_APB_REG(reg, val) \
    *(volatile unsigned *)(APB_BASE_ADDR + reg) = (val)

#define READ_APB_REG(reg) \
    (READ_VOLATILE_UINT32(APB_BASE_ADDR + reg))

#define WRITE_APB_REG_BITS(reg, val, start, len) \
    WRITE_APB_REG(reg,	(READ_APB_REG(reg) & ~(((1L<<(len))-1)<<(start))) | ((unsigned)((val)&((1L<<(len))-1)) << (start)))
#define READ_APB_REG_BITS(reg, start, len) \
    ((READ_APB_REG(reg) >> (start)) & ((1L<<(len))-1))


#endif /* AHBHELPER_H */
