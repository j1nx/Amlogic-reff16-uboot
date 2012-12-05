/*
 * BQ27x00 battery driver
 *
 * Copyright (C) 2008 Rodolfo Giometti <giometti@linux.it>
 * Copyright (C) 2008 Eurotech S.p.A. <info@eurotech.it>
 *
 * Based on a previous work by Copyright (C) 2008 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Author:  elvis yu<elvis.yu@amlogic.com>
 */  
 
#include <asm/arch/pinmux.h>
#include <asm/arch/gpio.h>
#include <asm/arch/i2c.h>
#include <asm/errno.h>
#include <linux/posix_types.h>
#include <linux/unaligned/le_byteshift.h>
#include <aml_i2c.h>
#include <bq27x00_battery.h>

//#define __DBG__
#ifdef __DBG__
#define bq27x00_debug(fmt,args...) do { printf("[DEBUG]: FILE:%s:%d, FUNC:%s--- "fmt"\n",\
                                                     __FILE__,__LINE__,__func__,## args);} \
                                         while (0)
#else
#define bq27x00_debug(fmt,args...)
#endif


#define DRIVER_VERSION			"1.1.0"

#define BQ27x00_REG_TEMP		0x06
#define BQ27x00_REG_VOLT		0x08
#define BQ27x00_REG_AI			0x14
#define BQ27x00_REG_FLAGS		0x0A
#define BQ27x00_REG_TTE			0x16
#define BQ27x00_REG_TTF			0x18
#define BQ27x00_REG_TTECP		0x26

#define BQ27000_REG_RSOC		0x0B /* Relative State-of-Charge */
#define BQ27000_FLAG_CHGS		(1 << 7)

#define BQ27500_REG_SOC			0x2c
#define BQ27500_FLAG_DSC		(1 << 0)
#define BQ27500_FLAG_FC			(1 << 9)


static int polling_count = 0;
static int ac_status = 0;
static int usb_status = 0;
static int new_usb_status = 0;
static int battery_capacity = 100;
static int new_battery_capacity = 100;
static int charge_status = -1;
static int new_charge_status = -1;

/* If the system has several batteries we need a different name for each
 * of them...
 */
static DEFINE_IDR(battery_id);
static DEFINE_MUTEX(battery_mutex);
static struct timer_list polling_timer;
static struct work_struct battery_work;
	
struct bq27x00_device_info;

static enum power_supply_property bq27x00_battery_props[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_CURRENT_NOW,
	POWER_SUPPLY_PROP_CAPACITY,
	POWER_SUPPLY_PROP_TEMP,
	POWER_SUPPLY_PROP_TIME_TO_EMPTY_NOW,
	POWER_SUPPLY_PROP_TIME_TO_EMPTY_AVG,
	POWER_SUPPLY_PROP_TIME_TO_FULL_NOW,
};


static struct bq27x00_battery_pdata *pdata;

static struct bq27x00_device_info *device_info;

static enum power_supply_property ac_power_props[] = {
	POWER_SUPPLY_PROP_ONLINE,
};
static char *ac_supply_list[] = {
	"battery",
};

/*
 * Common code for BQ27x00 devices
 */

static int bq27x00_read(u8 reg, int *rt_value, int b_single,
			struct bq27x00_device_info *di)
{
	return di->read(reg, rt_value, b_single, di);
}

/*
 * Return the battery temperature in tenths of degree Celsius
 * Or < 0 if something fails.
 */
static int bq27x00_battery_temperature(struct bq27x00_device_info *di)
{
	int ret;
	int temp = 0;

	ret = bq27x00_read(BQ27x00_REG_TEMP, &temp, 0, di);
	if (ret) {
		bq27x00_debug("error reading temperature\n");
		return ret;
	}

	if (di->chip == BQ27500)
		return temp - 2731;
	else
		return ((temp >> 2) - 273) * 10;
}

/*
 * Return the battery Voltage in milivolts
 * Or < 0 if something fails.
 */
static int bq27x00_battery_voltage(struct bq27x00_device_info *di)
{
	int ret;
	int volt = 0;

	ret = bq27x00_read(BQ27x00_REG_VOLT, &volt, 0, di);
	if (ret) {
		bq27x00_debug("error reading voltage\n");
		return ret;
	}

	return volt * 1000;
}

/*
 * Return the battery average current
 * Note that current can be negative signed as well
 * Or 0 if something fails.
 */
static int bq27x00_battery_current(struct bq27x00_device_info *di)
{
	int ret;
	int curr = 0;
	int flags = 0;

	ret = bq27x00_read(BQ27x00_REG_AI, &curr, 0, di);
	if (ret) {
		bq27x00_debug("error reading current\n");
		return 0;
	}

	if (di->chip == BQ27500) {
		/* bq27500 returns signed value */
		curr = (int)(s16)curr;
	} else {
		ret = bq27x00_read(BQ27x00_REG_FLAGS, &flags, 0, di);
		if (ret < 0) {
			bq27x00_debug("error reading flags\n");
			return 0;
		}
		if (flags & BQ27000_FLAG_CHGS) {
			bq27x00_debug("negative current!\n");
			curr = -curr;
		}
	}

	return curr * 1000;
}

/*
 * Return the battery Relative State-of-Charge
 * Or < 0 if something fails.
 */
static int bq27x00_battery_rsoc(struct bq27x00_device_info *di)
{
	int ret;
	int rsoc = 0;

	if (di->chip == BQ27500)
		ret = bq27x00_read(BQ27500_REG_SOC, &rsoc, 0, di);
	else
		ret = bq27x00_read(BQ27000_REG_RSOC, &rsoc, 1, di);
	if (ret) {
		bq27x00_debug("error reading relative State-of-Charge\n");
		return ret;
	}

	return rsoc;
}
static int bq27x00_battery_status(struct bq27x00_device_info *di)
{
	int flags = 0;
	int status;
	int ret;

	ret = bq27x00_read(BQ27x00_REG_FLAGS, &flags, 0, di);
	if (ret < 0) {
		//dev_err(di->dev, "error reading flags\n");
		return ret;
	}

	if (di->chip == BQ27500) {
		if (flags & BQ27500_FLAG_FC)
			status = POWER_SUPPLY_STATUS_FULL;
		else if (flags & BQ27500_FLAG_DSC)
			status = POWER_SUPPLY_STATUS_DISCHARGING;
		else
			status = POWER_SUPPLY_STATUS_CHARGING;
	} else {
	    if(pdata->is_ac_online()){
    		if (flags & BQ27000_FLAG_CHGS)
    			status = POWER_SUPPLY_STATUS_CHARGING;
    		else
    		    status = POWER_SUPPLY_STATUS_FULL;
		}
		else
			status = POWER_SUPPLY_STATUS_DISCHARGING;		    
	}

	return status;
}

/*
 * Read a time register.
 * Return < 0 if something fails.
 */
static int bq27x00_battery_time(struct bq27x00_device_info *di, int reg,
				union power_supply_propval *val)
{
	int tval = 0;
	int ret;

	ret = bq27x00_read(reg, &tval, 0, di);
	if (ret) {
		bq27x00_debug("error reading register %02x\n", reg);
		return ret;
	}

	if (tval == 65535)
		return -ENODATA;

	val->intval = tval * 60;
	return 0;
}

#define to_bq27x00_device_info(x) container_of((x), \
				struct bq27x00_device_info, bat);
static int ac_power_get_property(struct power_supply *psy,
		enum power_supply_property psp, union power_supply_propval *val)
{
	int retval = 0;
	
    if (psy->type == POWER_SUPPLY_TYPE_MAINS)
        val->intval = pdata->is_ac_online ? pdata->is_ac_online() : 0;
    else
        val->intval = pdata->is_usb_online ? pdata->is_usb_online() : 0;

	return retval;
}

static int bq27x00_battery_get_property(struct power_supply *psy,
					enum power_supply_property psp,
					union power_supply_propval *val)
{
	int ret = 0;
	struct bq27x00_device_info *di = to_bq27x00_device_info(psy);

	switch (psp) {
	case POWER_SUPPLY_PROP_STATUS:
        val->intval = bq27x00_battery_status(di);
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
	case POWER_SUPPLY_PROP_PRESENT:
		val->intval = bq27x00_battery_voltage(di);
		if (psp == POWER_SUPPLY_PROP_PRESENT)
			val->intval = val->intval <= 0 ? 0 : 1;
		break;
	case POWER_SUPPLY_PROP_CURRENT_NOW:
		val->intval = bq27x00_battery_current(di);
		break;
	case POWER_SUPPLY_PROP_CAPACITY:
		val->intval = bq27x00_battery_rsoc(di);
		break;
	case POWER_SUPPLY_PROP_TEMP:
		val->intval = bq27x00_battery_temperature(di);
		break;
	case POWER_SUPPLY_PROP_TIME_TO_EMPTY_NOW:
		ret = bq27x00_battery_time(di, BQ27x00_REG_TTE, val);
		break;
	case POWER_SUPPLY_PROP_TIME_TO_EMPTY_AVG:
		ret = bq27x00_battery_time(di, BQ27x00_REG_TTECP, val);
		break;
	case POWER_SUPPLY_PROP_TIME_TO_FULL_NOW:
		ret = bq27x00_battery_time(di, BQ27x00_REG_TTF, val);
		break;
	default:
		return -EINVAL;
	}

	return ret;
}


static void bq27x00_powersupply_init(struct bq27x00_device_info *di)
{
	di->bat.type = POWER_SUPPLY_TYPE_BATTERY;
	di->bat.properties = bq27x00_battery_props;
	di->bat.num_properties = ARRAY_SIZE(bq27x00_battery_props);
	di->bat.get_property = bq27x00_battery_get_property;
	di->bat.external_power_changed = NULL;

    di->ac.name = "ac";
	di->ac.type = POWER_SUPPLY_TYPE_MAINS;
	di->ac.supplied_to = ac_supply_list,
	di->ac.num_supplicants = ARRAY_SIZE(ac_supply_list),
	di->ac.properties = ac_power_props;
	di->ac.num_properties = ARRAY_SIZE(ac_power_props);
	di->ac.get_property = ac_power_get_property;

    di->usb.name = "usb";
	di->usb.type = POWER_SUPPLY_TYPE_USB;
	di->usb.supplied_to = ac_supply_list,
	di->usb.num_supplicants = ARRAY_SIZE(ac_supply_list),
	di->usb.properties = ac_power_props;
	di->usb.num_properties = ARRAY_SIZE(ac_power_props);
	di->usb.get_property = ac_power_get_property;
	
}

/*
 * i2c specific code
 */

static int bq27x00_read_i2c(u8 reg, int *rt_value, int b_single,
			struct bq27x00_device_info *di)
{
	struct i2c_msg msg[1];
	unsigned char data[2];
	int err;

	if (!di)
		return -ENODEV;
	
	msg->addr = di->addr;
	msg->flags = 0;
	msg->len = 1;
	msg->buf = data;

	data[0] = reg;
	err = aml_i2c_xfer(msg, 1);

	if (err >= 0) {
		if (!b_single)
			msg->len = 2;
		else
			msg->len = 1;

		msg->flags = I2C_M_RD;
		err = aml_i2c_xfer(msg, 1);
		if (err >= 0) {
			if (!b_single)
				*rt_value = get_unaligned_le16(data);
			else
				*rt_value = data[0];

			return 0;
		}
	}
	return err;
}

static void battery_work_func(struct work_struct *work)
{
}

static ssize_t store_powerhold(struct class *class, 
			struct class_attribute *attr,	const char *buf, size_t count)
{
	if(buf[0] == 'y'){
    printf("system off\n");    
        if(pdata->set_bat_off)
        pdata->set_bat_off();
    }
	return count;
}

struct bq27x00_device_info *bq27x00_battery_probe(struct bq27x00_battery_pdata *bq27x00_pdata)
{
	struct bq27x00_device_info *di;
	int num;
	int retval = 0;

	if (bq27x00_pdata->set_charge) {
		bq27x00_pdata->set_charge(0);
        printf("set slow charge\n");
	} 

	di = malloc(sizeof(*di));
	memset(di, 0, sizeof(*di));
	if (!di) {
		bq27x00_debug("failed to allocate device info data\n");
		retval = -ENOMEM;
		goto batt_failed;
	}
	di->id = num;
	di->chip = bq27x00_pdata->chip;

	di->bat.name = "BQ27x00";
	di->read = &bq27x00_read_i2c;
	di->addr = bq27x00_pdata->addr;

	bq27x00_powersupply_init(di);
	
	bq27x00_debug("support ver. %s enabled\n", DRIVER_VERSION);

	return di;

batt_failed:
	free(di);
	printf("[%s]:ERROR=%d.\n", __FUNCTION__, retval);
	return NULL;
}

static int bq27x00_battery_remove(struct bq27x00_device_info *di)
{
	free(di);

	return 0;
}
