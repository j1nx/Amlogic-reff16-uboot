/*
 * ADS7846 based touchscreen and sensor driver
 *
 * Copyright (c) 2005 David Brownell
 * Copyright (c) 2006 Nokia Corporation
 * Various changes: Imre Deak <imre.deak@nokia.com>
 *
 * Using code from:
 *  - corgi_ts.c
 *	Copyright (C) 2004-2005 Richard Purdie
 *  - omap_ts.[hc], ads7846.h, ts_osk.c
 *	Copyright (C) 2002 MontaVista Software
 *	Copyright (C) 2004 Texas Instruments
 *	Copyright (C) 2005 Dirk Behme
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */
#include <asm/arch/gpio.h>
#include <spi.h>
#include <soft_spi.h>
#include <asm/types.h>
#include <ads7846.h>
#include <stddef.h>

struct ads7846_event {
	unsigned short	x;
	unsigned short	y;
	unsigned short	z1;
	unsigned short	z2;
};

static struct spi_slave *ads7846_slave = NULL;

static int ads7846_xfer(struct spi_slave *slave, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	int ret;
	ret = soft_spi_xfer(slave, bitlen, dout, din, flags);
	return ret;
}

static void ads7846_read_values(struct ads7846 *ts, struct ads7846_event *tsc)
{
	unsigned short dout,*din;
	u8 buf[2];

	dout = 0xd0;
	ads7846_xfer(ads7846_slave,16,&dout,buf,SPI_XFER_BEGIN | SPI_XFER_END);
	tsc->x = (buf[0] << 4) | (buf[1] >> 4);

	dout = 0x90;
	ads7846_xfer(ads7846_slave,16,&dout,buf,SPI_XFER_BEGIN | SPI_XFER_END);
	tsc->y = (buf[0] << 4) | (buf[1] >> 4);
	
	dout = 0xb0;
	ads7846_xfer(ads7846_slave,16,&dout,buf,SPI_XFER_BEGIN | SPI_XFER_END);
	tsc->z1 = (buf[0] << 4) | (buf[1] >> 4);

	dout = 0xc0;
	ads7846_xfer(ads7846_slave,16,&dout,buf,SPI_XFER_BEGIN | SPI_XFER_END);
	tsc->z1 = (buf[0] << 4) | (buf[1] >> 4);

}

int ads7846_work(struct ads7846 *ts, struct ts_event *tc)
{
    struct ads7846_event tsc;
	
	if((ts == NULL) || (tc == NULL)) {
		//printf("ads7846_work: ts is null or tc is null\n");
		return -1;
	}
	memset(tc, 0, sizeof(struct ts_event));
	if (ts->get_pendown_state == NULL) {
		//printf("ads7846_work: ts get_pendown_state is null\n");
		return -1;
	}
	if(!ts->get_pendown_state()) {
		//printf("ads7846_work: ts get_pendown_state is high\n");
		return 0;
	}
	/*else
	{
		printf("ads7846_work: ts get_pendown_state pen down\n");
		return 1;
	}*/
	ads7846_read_values(ts, &tsc);
	tc->x = tsc.x;
	tc->y = tsc.y;
	if ((tc->x < 0) || (tc->y < 0)) {
		printf("ads7846_work: tsc.x < 0 or tsc.y < 0\n");
		return 0;
	}
	if ((tc->x == 0) || (tc->y == 0)) {
		printf("ads7846_work: tsc.x == 0 and tsc.y == 0\n");
		return 0;
	}
	if (ts->convert) {
        int xy = ts->convert(tc->x, tc->y);
        tc->x = xy >> 16;
        tc->y = xy & 0xffff;
		tc->touch = ONE_TOUCH;
	}
	return 1;
}


int ads7846_init(struct ads7846 *ts)
{
	int err;
	
	if(ts == NULL) {
		printf("ads7846_init: ts is null\n");
		return -1;
	}
	if(ts->init)
		ts->init();
	
	ads7846_slave = soft_spi_setup_slave(ts->bus_num,ts->cs,ts->max_speed_hz,ts->mode);
	if(ads7846_slave == NULL)
	{
		printf("ads7846_init: ads7846_slave is null\n");
		return -1;
	}
	return 0;
}


