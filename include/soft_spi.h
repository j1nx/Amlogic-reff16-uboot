/*
 * (C) Copyright 2001
 * Gerald Van Baren, Custom IDEAS, vanbaren@cideas.com.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _SOFT_SPI_H_
#define _SOFT_SPI_H_
struct soft_spi_gpio_data {
	unsigned	sck;
	unsigned	mosi;
	unsigned	miso;
};

extern int  soft_spi_xfer(struct spi_slave *slave, unsigned int bitlen,const void *dout, void *din, unsigned long flags);
extern struct spi_slave *soft_spi_setup_slave(unsigned int bus, unsigned int cs,unsigned int max_hz, unsigned int mode);


#endif	/* _SOFT_SPI_H_ */

