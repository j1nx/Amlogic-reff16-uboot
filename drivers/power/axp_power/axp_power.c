#include <common.h>
#include <asm/arch/gpio.h>
#include <asm/arch/i2c.h>
#include <aml_i2c.h>

#include "axp-mfd.h"

#define AXP_I2C_ADDR 0x34

#define MAX_I2C_BUFF 64

int axp_write(int reg, uint8_t val)
{
	int ret;
	uint8_t buff[2];
    buff[0] = reg;
    buff[1] = val;
    struct i2c_msg msg[] = {
        {
        .addr = AXP_I2C_ADDR,
        .flags = 0,
        .len = 2,
        .buf = buff,
        }
    };

	ret = aml_i2c_xfer(msg, 1);
    if (ret < 0) {
        printf("%s: i2c transfer failed\n", __FUNCTION__);
		return ret;
    }
	return 0;
}


int axp_writes(int reg, int len, uint8_t *val)
{
	int ret;
	uint8_t buff[MAX_I2C_BUFF + 1];
	if(len > MAX_I2C_BUFF)
	{
		printf("%s: i2c len must <= %d\n", __FUNCTION__, MAX_I2C_BUFF);
		return -1;
	}
    buff[0] = reg;
	memcpy((buff+1), val, len);
    struct i2c_msg msg[] = {
        {
        .addr = AXP_I2C_ADDR,
        .flags = 0,
        .len = len + 1,
        .buf = buff,
        }
    };

	ret = aml_i2c_xfer(msg, 1);
    if (ret < 0) {
        printf("%s: i2c transfer failed\n", __FUNCTION__);
		return ret;
    }
	return 0;
}


int axp_read(int reg, uint8_t *val)
{
	int ret;
	struct i2c_msg msgs[] = {
        {
            .addr = AXP_I2C_ADDR,
            .flags = 0,
            .len = 1,
            .buf = &reg,
        },
        {
            .addr = AXP_I2C_ADDR,
            .flags = I2C_M_RD,
            .len = 1,
            .buf = val,
        }
    };
	ret = aml_i2c_xfer(msgs, 2);
    if (ret < 0) {
        printf("%s: i2c transfer failed\n", __FUNCTION__);
		return ret;
    }

    return 0;
}


int axp_reads(int reg, int len, uint8_t *val)
{
	int ret;
	struct i2c_msg msgs[] = {
        {
            .addr = AXP_I2C_ADDR,
            .flags = 0,
            .len = 1,
            .buf = &reg,
        },
        {
            .addr = AXP_I2C_ADDR,
            .flags = I2C_M_RD,
            .len = len,
            .buf = val,
        }
    };
	ret = aml_i2c_xfer(msgs, 2);
    if (ret < 0) {
        printf("%s: i2c transfer failed\n", __FUNCTION__);
		return ret;
    }
    return 0;
}

int axp_set_bits(int reg, uint8_t bit_mask)
{
	uint8_t reg_val;
	int ret = 0;

	ret = axp_read(reg, &reg_val);
	if (ret)
		goto out;

	if ((reg_val & bit_mask) != bit_mask) {
		reg_val |= bit_mask;
		ret = axp_write(reg, reg_val);
	}
out:
	return ret;
}

int axp_clr_bits(int reg, uint8_t bit_mask)
{
	uint8_t reg_val;
	int ret = 0;

	ret = axp_read(reg, &reg_val);
	if (ret)
		goto out;

	if (reg_val & bit_mask) {
		reg_val &= ~bit_mask;
		ret = axp_write(reg, reg_val);
	}
out:
	return ret;
}

int axp_update(int reg, uint8_t val, uint8_t mask)
{
	uint8_t reg_val;
	int ret = 0;

	ret = axp_read(reg, &reg_val);
	if (ret)
		goto out;

	if ((reg_val & mask) != val) {
		reg_val = (reg_val & ~mask) | val;
		ret = axp_write(reg, reg_val);
	}
out:
	return ret;
}

void axp_power_off(void)
{
	uint8_t val;

#if defined (CONFIG_AW_AXP18)
	axp_set_bits(POWER18_ONOFF, 0x80);
#endif

#if defined (CONFIG_AW_AXP19)
	axp_set_bits(POWER19_OFF_CTL, 0x80);
#endif

#if defined (CONFIG_AW_AXP20)
#if 1
	axp_read(POWER20_COULOMB_CTL, &val);
	val &= 0x3f;
	axp_write(POWER20_COULOMB_CTL, val);
	val |= 0x80;
	val &= 0xbf;
	axp_write(POWER20_COULOMB_CTL, val);
#endif

    printf("[axp] send power-off command!\n");
    mdelay(20);
	axp_set_bits(POWER20_OFF_CTL, 0x80);
    mdelay(20);
    printf("[axp] warning!!! axp can't power-off, maybe some error happend!\n");

#endif
}


