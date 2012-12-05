#ifndef _SECURE_REGISTERS_H_
#define _SECURE_REGISTERS_H_

#include <config.h>

/*******************************************************************
  * register address BASE definition
 *******************************************************************/
#define IO_EFUSE_PHY_BASE		0xc1100000
#define IO_CBUS_PHY_BASE		0xc1100000

#define IO_EFUSE_BASE		0xf1100000
#define IO_CBUS_BASE		0xf1100000


//#define CBUS_REG_OFFSET(reg) ((reg) << 2)
//#define CBUS_REG_ADDR(reg)	 (IO_CBUS_BASE + CBUS_REG_OFFSET(reg))

/*******************************************************************
  * EFUSE registers offset
 *******************************************************************/
#define EFUSE_CNTL0                                (0x2158<<2)
#define EFUSE_CNTL1                                (0x2159<<2)
#define EFUSE_CNTL2                                (0x215a<<2)
#define EFUSE_CNTL3                                (0x215b<<2)
#define EFUSE_CNTL4                                (0x215c<<2)

/*******************************************************************
  * TIMERE registers address
 *******************************************************************/
#define ISA_TIMERE    0x2655
#define TIMERE_SUB(timea,timeb) (((timea<<8)-(timeb<<8))>>8)

#endif