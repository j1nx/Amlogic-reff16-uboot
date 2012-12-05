/*
 * SPI flash interface
 *
 * Copyright (C) 2008 Atmel Corporation
 */
#define DEBUG
#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <spi_flash.h>

#include "spi_flash_aml_internal.h"
#include <asm/arch/am_regs.h>

int spi_flash_cmd(struct spi_slave *spi, u32 cmd, void *response, size_t len)		
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;
	u32 var=cmd;	

	if (len == 0)
		flags |= SPI_XFER_END;
	
	flags|=SPI_XFER_CMD;
	ret = spi_xfer(spi, 4*8, (u8*)&var, NULL, flags);				
	if (ret) {
		debug("SF: Failed to send command %02x: %d\n", cmd, ret);
		return ret;
	}

	if (len) {
		if((1<<SPI_FLASH_RDID)==cmd) flags=SPI_XFER_ID;
		else {
				if((1<<SPI_FLASH_RDSR)==cmd) flags=SPI_XFER_STATUS;
		}
		ret = spi_xfer(spi, len * 8, NULL, response, flags|SPI_XFER_END);
		if (ret)
			debug("SF: Failed to read response (%zu bytes): %d\n",
					len, ret);
	}

	return ret;
}



int spi_flash_adr_write(struct spi_slave *spi,  u32 adr){

	unsigned flags=SPI_XFER_END;
	int ret;

	ret=adr;	
	flags|=SPI_XFER_ADR;
	
	ret=spi_xfer(spi,4*8,&ret,NULL,flags);
	
	if(ret)	{
	
		debug("SF: Faild to send adr(%zu bytes): %d\n",4,ret);
	}

	return ret;

}






int spi_flash_write_aml(struct spi_flash *flash,u32 offset, size_t len, const void *buf){
	
	struct spi_slave *spi = flash->spi;
	int ret;
	unsigned temp_addr;
    int temp_length;
    temp_addr = offset;
    temp_length = len;	
	unsigned flags;
 	
	while(temp_length>0){
		
		flags=(temp_addr & 0xffffff)|( (temp_length>=32?32:temp_length) << SPI_FLASH_BYTES_LEN);
		
		spi_flash_adr_write(spi, flags);
		
		flags=SPI_XFER_WRITECACHE;

		spi_xfer(spi,(temp_length>=32?32:temp_length)*8,buf,NULL,flags);	

		
		flags=(1<<SPI_FLASH_WREN);
		spi_flash_cmd(spi,flags,NULL,0);
		
		flags=(1<<SPI_FLASH_PP);
		spi_flash_cmd(spi,flags,NULL,0);
		
		
		 ret=1;
   		 while ( (ret&1) == 1 ){
      
			flags=1<<SPI_FLASH_RDSR;
		
			spi_flash_cmd(spi,flags,&ret,2);		//2 byte status	

		
    	}
	 	
		
        temp_addr   += (temp_length>=32?32:temp_length);
		buf			+= (temp_length>=32?32:temp_length);
		temp_length -= (temp_length>=32?32:temp_length);
		 
	}	

#ifdef SPI_WRITE_PROTECT
        spi_enable_write_protect();
#endif

	return 0;


}

int spi_flash_read_aml(struct spi_flash *flash,u32 offset, size_t len, void *buf){

	struct spi_slave *spi = flash->spi;
	//int ret;
	u32 temp_addr;
    int temp_length;
    temp_addr = offset;
    temp_length = len;	
	unsigned flags;	
	

    /* 0x400000 ~ 0x7fffff */
    if(temp_addr + len > 0x400000 && temp_addr < 0x400000){

      flags=SPI_XFER_END|SPI_XFER_COPY;
      spi_xfer(spi,(0x400000-temp_addr)*8,&temp_addr,buf,flags);	  
      buf += (0x400000-temp_addr);
      temp_length = len - (0x400000-temp_addr);
      temp_addr = 0x400000;			
    }
    /* 0x000000 ~ 0x3fffff */
	else if(temp_addr < 0x400000){
	   flags=SPI_XFER_END|SPI_XFER_COPY;
       spi_xfer(spi,temp_length*8,&temp_addr,buf,flags);	  
	   return 0;
    }
		
	spi_claim_bus(spi);															/*FIXME for spi_xfer release bus*/	
	while(temp_length>0){
		
		flags=(temp_addr & 0xffffff)|( (temp_length>=32?32:temp_length) << SPI_FLASH_BYTES_LEN);
	
		spi_flash_adr_write(spi, flags);	
		
		flags=(1<<SPI_FLASH_READ);
		
		spi_flash_cmd(spi,flags,NULL,0);
		
		flags=SPI_XFER_READCACHE;

		spi_xfer(spi,(temp_length>=32?32:temp_length)*8,NULL,buf,flags);	

        temp_addr   += (temp_length>=32?32:temp_length);
		buf			+= (temp_length>=32?32:temp_length);	
		temp_length -= (temp_length>=32?32:temp_length);
			
	}	
		
	
	return 0;	

}


struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode)
{
	struct spi_slave *spi;
	struct spi_flash *flash;
	int ret;
	u8 idcode[3];

	spi = spi_setup_slave(bus, cs, max_hz, spi_mode);
	if (!spi) {
		debug("SF: Failed to set up slave\n");
		return NULL;
	}

	ret = spi_claim_bus(spi);
	if (ret) {
		debug("SF: Failed to claim SPI bus: %d\n", ret);
		goto err_claim_bus;
	}

																				/* Read the ID codes */
	ret=spi_flash_cmd(spi,1<<SPI_FLASH_RDID,&idcode, sizeof(idcode));
	if (ret)
		goto err_read_id;

	debug("SF: Got idcode %02x %02x %02x\n", idcode[0],
			idcode[1], idcode[2]);

	switch (idcode[0]) {

#ifdef CONFIG_SPI_FLASH_MACRONIX
	case 0xc2:
		flash=spi_flash_probe_mxic(spi,idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_SST
	case 0xbf:
		flash=spi_flash_probe_sst(spi,idcode);
		break;
#endif

#ifdef CONFIG_SPI_FLASH_SPANSION
	case 0x01:
		flash = spi_flash_probe_spansion(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_ATMEL
	case 0x1F:
		flash = spi_flash_probe_atmel(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_WINBOND
	case 0xef:
		flash = spi_flash_probe_winbond(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_GIGABYTE
	case 0xc8:
		flash = spi_flash_probe_gigabyte(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_STMICRO
	case 0x20:
		flash = spi_flash_probe_stmicro(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_EON:
    case 0x1c:
        flash = spi_flash_probe_eon(spi, idcode);
        break;
#endif
	default:
		debug("SF: Unsupported manufacturer %02X\n", idcode[0]);
		flash = NULL;
		break;
	}

	if (!flash)
		goto err_manufacturer_probe;
#ifdef SPI_WRITE_PROTECT
        if(spi_check_write_protect())
             printf("\nSPI NOR Flash have write protect!!!\n");
        else{
             printf("\nSPI NOR Flash NO write protect!!!, So I will enable it...\n");
             spi_enable_write_protect();
            }
#endif
	spi_release_bus(spi);

	return flash;

err_manufacturer_probe:
err_read_id:
	spi_release_bus(spi);
err_claim_bus:
	spi_free_slave(spi);
	return NULL;
}


void spi_flash_free(struct spi_flash *flash)
{
	spi_free_slave(flash->spi);
	free(flash);
}

