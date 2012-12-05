/*
 * SPI flash internal definitions
 *
 * Copyright (C) 2009 Amlogic Corporation
 */


/* Common parameters */
#define SPI_FLASH_PROG_TIMEOUT		((10 * CONFIG_SYS_HZ) / 1000)
#define SPI_FLASH_PAGE_ERASE_TIMEOUT	((50 * CONFIG_SYS_HZ) / 1000)
#define SPI_FLASH_SECTOR_ERASE_TIMEOUT	(10 * CONFIG_SYS_HZ)

/* Common commands */
#define CMD_READ_ID			0x9f
#define CMD_READ_ARRAY_SLOW		0x03
#define CMD_READ_ARRAY_FAST		0x0b
#define CMD_READ_ARRAY_LEGACY		0xe8

/* Send a single-byte command to the device and read the response */
int spi_flash_cmd(struct spi_slave *spi, u32 cmd, void *response, size_t len);
int spi_flash_adr_write(struct spi_slave *spi, u32 adr);
//int spi_flash_adr_write(struct spi_slave *spi, const u8 * adr,size_t adr_len);
int spi_flash_write_aml(struct spi_flash *flash,u32 offset, size_t len, const void *buf);
int spi_flash_read_aml(struct spi_flash *flash,u32 offset, size_t len, void *buf);



/* Manufacturer-specific probe functions */

struct spi_flash *spi_flash_probe_mxic(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_spansion(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_atmel(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_stmicro(struct spi_slave *spi, u8 *idcode);
struct spi_flash *spi_flash_probe_winbond(struct spi_slave *spi, u8 *idcode);


