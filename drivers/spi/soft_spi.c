/*
 * (C) Copyright 2002
 * Gerald Van Baren, Custom IDEAS, vanbaren@cideas.com.
 *
 * Influenced by code from:
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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

#include <common.h>
#include <spi.h>

#include <malloc.h>
#include <soft_spi.h>

/*-----------------------------------------------------------------------
 * Definitions
 */

#define SOFT_SPI_SCL  soft_spi_scl
#define SOFT_SPI_SDA  soft_spi_sda
#define SOFT_SPI_READ soft_spi_read()
#define SOFT_SPI_DELAY udelay(2)

#ifdef DEBUG_SPI
#define PRINTD(fmt,args...)	printf (fmt ,##args)
#else
#define PRINTD(fmt,args...)
#endif

struct soft_spi_slave {
	struct spi_slave slave;
	unsigned int mode;
};

static inline struct soft_spi_slave *to_soft_spi(struct spi_slave *slave)
{
	return container_of(slave, struct soft_spi_slave, slave);
}

/*=====================================================================*/
/*                         Public Functions                            */
/*=====================================================================*/
extern const struct soft_spi_gpio_data aml_spi_gpio_pdata;
void soft_spi_scl(int bit)
{
	gpio_direction_output(aml_spi_gpio_pdata.sck, bit);
}

void soft_spi_sda(int bit)
{
	gpio_direction_output(aml_spi_gpio_pdata.mosi, bit);
}

unsigned char soft_spi_read(void)
{
	gpio_direction_input(aml_spi_gpio_pdata.miso);
	return (unsigned char)gpio_get_value(aml_spi_gpio_pdata.miso);
}

int soft_spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return 1;
}

void soft_spi_cs_activate(struct spi_slave *slave)
{
	gpio_direction_output(slave->cs, 0);
}

void soft_spi_cs_deactivate(struct spi_slave *slave)
{
	gpio_direction_output(slave->cs, 1);
}

/*-----------------------------------------------------------------------
 * Initialization
 */
void soft_spi_init (void)
{
#ifdef	SPI_INIT
	volatile immap_t *immr = (immap_t *)CONFIG_SYS_IMMR;

	SPI_INIT;
#endif
}

struct spi_slave *soft_spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode)
{
	struct soft_spi_slave *ss;

	if (!soft_spi_cs_is_valid(bus, cs))
		return NULL;

	ss = malloc(sizeof(struct soft_spi_slave));
	if (!ss)
		return NULL;

	ss->slave.bus = bus;
	ss->slave.cs = cs;
	ss->mode = mode;

	/* TODO: Use max_hz to limit the SCK rate */

	return &ss->slave;
}

void soft_spi_free_slave(struct spi_slave *slave)
{
	struct soft_spi_slave *ss = to_soft_spi(slave);

	free(ss);
}

int soft_spi_claim_bus(struct spi_slave *slave)
{
#ifdef CONFIG_SYS_IMMR
	volatile immap_t *immr = (immap_t *)CONFIG_SYS_IMMR;
#endif
	struct soft_spi_slave *ss = to_soft_spi(slave);

	/*
	 * Make sure the SPI clock is in idle state as defined for
	 * this slave.
	 */
	if (ss->mode & SPI_CPOL)
		SOFT_SPI_SCL(1);
	else
		SOFT_SPI_SCL(0);

	return 0;
}

void soft_spi_release_bus(struct spi_slave *slave)
{
	/* Nothing to do */
}

/*-----------------------------------------------------------------------
 * SPI transfer
 *
 * This writes "bitlen" bits out the SPI MOSI port and simultaneously clocks
 * "bitlen" bits in the SPI MISO port.  That's just the way SPI works.
 *
 * The source of the outgoing bits is the "dout" parameter and the
 * destination of the input bits is the "din" parameter.  Note that "dout"
 * and "din" can point to the same memory location, in which case the
 * input data overwrites the output data (since both are buffered by
 * temporary variables, this is OK).
 */
int  soft_spi_xfer(struct spi_slave *slave, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
#ifdef CONFIG_SYS_IMMR
	volatile immap_t *immr = (immap_t *)CONFIG_SYS_IMMR;
#endif
	struct soft_spi_slave *ss = to_soft_spi(slave);
	uchar		tmpdin  = 0;
	uchar		tmpdout = 0;
	const u8	*txd = dout;
	u8		*rxd = din;
	int		cpol = ss->mode & SPI_CPOL;
	int		cpha = ss->mode & SPI_CPHA;
	unsigned int	j;

	PRINTD("spi_xfer: slave %u:%u dout %08X din %08X bitlen %u\n",
		slave->bus, slave->cs, *(uint *)txd, *(uint *)rxd, bitlen);

	if (flags & SPI_XFER_BEGIN)
		soft_spi_cs_activate(slave);

	for(j = 0; j < bitlen; j++) {
		/*
		 * Check if it is time to work on a new byte.
		 */
		if((j % 8) == 0) {
			tmpdout = *txd++;
			if(j != 0) {
				*rxd++ = tmpdin;
			}
			tmpdin  = 0;
		}

		if (!cpha)
			SOFT_SPI_SCL(!cpol);
		SOFT_SPI_SDA(tmpdout & 0x80);
		SOFT_SPI_DELAY;
		if (cpha)
			SOFT_SPI_SCL(!cpol);
		else
			SOFT_SPI_SCL(cpol);
		tmpdin	<<= 1;
		tmpdin	|= SOFT_SPI_READ;
		tmpdout	<<= 1;
		SOFT_SPI_DELAY;
		if (cpha)
			SOFT_SPI_SCL(cpol);
	}
	/*
	 * If the number of bits isn't a multiple of 8, shift the last
	 * bits over to left-justify them.  Then store the last byte
	 * read in.
	 */
	if((bitlen % 8) != 0)
		tmpdin <<= 8 - (bitlen % 8);
	*rxd++ = tmpdin;

	if (flags & SPI_XFER_END)
		soft_spi_cs_deactivate(slave);

	return(0);
}
