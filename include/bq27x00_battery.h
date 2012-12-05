/*
 * Common power driver for Amlogic Devices with one or two external
 * power supplies (AC/USB) connected to main and backup batteries,
 * and optional builtin charger.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __BQ27X00_BATTERY_H__
#define __BQ27X00_BATTERY_H__

#include <linux/power_supply.h>


#define AML_POWER_CHARGE_AC  (1 << 0)
#define AML_POWER_CHARGE_USB (1 << 1)

enum bq27x00_chip { BQ27000, BQ27500 };


struct bq27x00_battery_pdata {
	int (*is_ac_online)(void);
	int (*is_usb_online)(void);
	void (*set_charge)(int flags);
	void (*set_bat_off)(void);
	int (*get_charge_status)(void);

	unsigned char addr;
    unsigned int chip;
};

struct bq27x00_device_info {
	int			id;
	struct bq27x00_access_methods	*bus;
	struct power_supply	bat;
	struct power_supply	ac;	
	struct power_supply	usb;	
	enum bq27x00_chip	chip;
	unsigned char addr;
	int (*read)(u8 reg, int *rt_value, int b_single,
		struct bq27x00_device_info *di);
};


//extern int bq27x00_battery_rsoc(struct bq27x00_device_info *di_x);

#endif /* __BQ27X00_BATTERY_H__ */
