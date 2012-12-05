#ifndef AML_A3_HARD_I2C_H
#define AML_A3_HARD_I2C_H
#include <a3_osd_display/volatile_read.h>
typedef struct
{
	unsigned char       device_id;
	unsigned char       id_mask;
	unsigned char       i2c_flag;    
	unsigned            max_addr;
	unsigned            delay;  
	 unsigned            requency;  
 
}hard_i2c_para_t;

extern hard_i2c_para_t hard_i2c_para;
extern void Init_hard_I2c(void);
extern int hard_i2c_write_0( unsigned char *rw_buff, unsigned int rw_bytes, unsigned int rw_addr);


#define PERI_BASE_ADDR                 0xc1100000

#define WRITE_PERI_REG(reg, val) *(volatile unsigned *)(PERI_BASE_ADDR + ((reg)<<2)) = (val)
#define READ_PERI_REG(reg) (READ_VOLATILE_UINT32(PERI_BASE_ADDR + ((reg)<<2)))

#define CLEAR_PERI_REG_MASK(reg, mask) WRITE_PERI_REG(reg, (READ_PERI_REG(reg)&(~(mask))))
#define SET_PERI_REG_MASK(reg, mask)   WRITE_PERI_REG(reg, (READ_PERI_REG(reg)|(mask)))

#define PREG_I2C_SL_CONTROL           0x2150
#define PREG_I2C_SL_SEND              0x2151
#define PREG_I2C_SL_RECV              0x2152

#define PREG_I2C_SL_REG1              ** new **             0x2153  // NEW FILTER


#define PREG_I2C_MS_CTRL              0x2148
    #define I2C_M_MANUAL_SDA_I        26
    #define I2C_M_MANUAL_SCL_I        25
    #define I2C_M_MANUAL_SDA_O        24
    #define I2C_M_MANUAL_SCL_O        23
    #define I2C_M_MANUAL_EN           22
    #define I2C_M_DELAY_MSB           21
    #define I2C_M_DELAY_LSB           12
    #define I2C_M_DATA_CNT_MSB        11
    #define I2C_M_DATA_CNT_LSB        8
    #define I2C_M_CURR_TOKEN_MSB      7
    #define I2C_M_CURR_TOKEN_LSB      4
    #define I2C_M_ERROR               3
    #define I2C_M_STATUS              2
    #define I2C_M_ACK_IGNORE          1
    #define I2C_M_START               0
#define  PREG_I2C_MS_SL_ADDR          0x2149
#define  PREG_I2C_MS_TOKEN_LIST_REG0  0x214a
#define  PREG_I2C_MS_TOKEN_LIST_REG1  0x214b
#define  PREG_I2C_MS_TOKEN_WDATA_REG0 0x214c
#define  PREG_I2C_MS_TOKEN_WDATA_REG1 0x214d
#define  PREG_I2C_MS_TOKEN_RDATA_REG0 0x214e
#define  PREG_I2C_MS_TOKEN_RDATA_REG1 0x214f
    #define  I2C_END                  0x0
    #define  I2C_START                0x1
    #define  I2C_SLAVE_ADDR_WRITE     0x2
    #define  I2C_SLAVE_ADDR_READ      0x3
    #define  I2C_DATA                 0x4
    #define  I2C_DATA_LAST            0x5
    #define  I2C_STOP                 0x6


#endif

