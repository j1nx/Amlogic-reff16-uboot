#include <asm/arch/gpio.h>
#include <asm/arch/i2c.h>
#include <aml_i2c.h>
#include "shuttle_mcu.h"


unsigned char shmcu_i2c_read(unsigned char reg)
{
    unsigned char val = 0;
    struct i2c_msg msgs[] = {
        {
            .addr = SHMCU_ADDR,
            .flags = 0,
            .len = 1,
            .buf = &reg,
        },
        {
            .addr = SHMCU_ADDR,
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

void shmcu_i2c_write(unsigned char reg, unsigned char val)
{
    unsigned char buff[2];
    buff[0] = reg;
    buff[1] = val;
    struct i2c_msg msg[] = {
        {
        .addr = SHMCU_ADDR,
        .flags = 0,
        .len = 2,
        .buf = buff,
        }
    };

    if (aml_i2c_xfer(msg, 1) < 0) {
        printf("%s: i2c transfer failed\n", __FUNCTION__);
    }
}

int is_ac_online_sh(void)
{
	int adapter;	
	adapter = shmcu_i2c_read(ADAPTER_STATUS);
		
	if (adapter != ADAPTER_OFF){
		printf("Adapter On\n");
		return 1;
	}
	printf("Adapter Off\n");
	return 0;
}

int measure_capacity_battery_sh(void)
{
	int battery;	

	battery = shmcu_i2c_read(BATTERY_STATUS);
	
	if (battery >= HALF_CAPACITY){
		printf("Battery Capacity: %d\n", battery);
		return 1;
	}
	printf("Low Battery Capacity: %d\n", battery);
	return 0;
}
