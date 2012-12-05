/*******************************************************************
 * 
 *  Copyright C 2008 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: 
 *      MPEG register helper defines
 *  Author: Amlogic Software
 *  Created: 10/17/2008 4:01:17 PM
 *
 *******************************************************************/
#ifndef MPEGHELPER_H
#define MPEGHELPER_H

#include "volatile_read.h"
#define MPEG_BASE_ADDR               0xC1100000
#define WRITE_MPEG_REG(reg, val) \
    *(volatile unsigned *)(MPEG_BASE_ADDR + ((reg)<<2)) = (val)

#define READ_MPEG_REG(reg) \
    (READ_VOLATILE_UINT32(MPEG_BASE_ADDR + ((reg)<<2)))

#define WRITE_MPEG_REG_BITS(reg, val, start, len) \
    WRITE_MPEG_REG(reg,	(READ_MPEG_REG(reg) & ~(((1L<<(len))-1)<<(start))) | ((unsigned)((val)&((1L<<(len))-1)) << (start)))
#define READ_MPEG_REG_BITS(reg, start, len) \
    ((READ_MPEG_REG(reg) >> (start)) & ((1L<<(len))-1))

#define CLEAR_MPEG_REG_MASK(reg, mask) WRITE_MPEG_REG(reg, (READ_MPEG_REG(reg)&(~(mask))))
#define SET_MPEG_REG_MASK(reg, mask)   WRITE_MPEG_REG(reg, (READ_MPEG_REG(reg)|(mask)))

#define VLD_POWER_WORKAROUND()	WRITE_MPEG_REG(MREG_POWER_CTL_VLD, FORCE_VIFF_RD)
#define RESET_WORKAROUND()	do {int dummy = READ_MPEG_REG(VERSION_CTRL);} while (0)
#define SET_MPEG_REG_MASK_VAL(reg,mask,val) WRITE_MPEG_REG(reg, ((READ_MPEG_REG(reg)&(~(mask)))|((val)&(mask))))


#endif /* MPEGHELPER_H */
