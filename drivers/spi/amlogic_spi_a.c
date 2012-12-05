/*
 * Copyright C 2009 by Amlogic, Inc. All Rights Reserved.
 * Description:Amlogic SPI Controller  
 * Author:	pfs
 */



#include <common.h>
#include <asm/arch/am_regs.h>
#include <asm/arch/am_reg_addr.h>
#include <malloc.h>
#include <spi.h>


struct aml_spi_slave {
	struct spi_slave slave;
	void * adr_base;
	u32  mode;
	u32   ctl;
};

static inline struct aml_spi_slave *to_aml_spi(struct spi_slave *slave){
	return container_of(slave, struct aml_spi_slave, slave);
}
__attribute__((weak))
int spi_cs_is_valid(unsigned int bus, unsigned int cs){
	return 0;
}

__attribute__((weak))
void spi_cs_activate(struct spi_slave *slave){
}

__attribute__((weak))
void spi_cs_deactivate(struct spi_slave *slave){
}


void spi_init(){


    writel(0xea949,P_SPI_FLASH_CTRL);
}

#ifdef SPI_WRITE_PROTECT
void spi_enable_write_protect(void)
{
	unsigned char statusValue;
	int ret, var;

	statusValue = 0x7<<2;//all protect;
		
	/*write enable*/
	 writel(1<<SPI_FLASH_WREN, P_SPI_FLASH_CMD);
        while(readl(P_SPI_FLASH_CMD)!=0){
		do{
			writel(1<<SPI_FLASH_RDSR, P_SPI_FLASH_CMD);
			while(readl(P_SPI_FLASH_CMD)!=0);
			ret = readl(P_SPI_FLASH_STATUS);
  		}while (ret&1);
        }
		
	/*write status register*/
        writel(statusValue,P_SPI_FLASH_STATUS);
        writel(1<<SPI_FLASH_WRSR, P_SPI_FLASH_CMD);
        while(readl(P_SPI_FLASH_CMD)!=0){
		do{
			writel(1<<SPI_FLASH_RDSR, P_SPI_FLASH_CMD);
			while(readl(P_SPI_FLASH_CMD)!=0);
			ret = readl(P_SPI_FLASH_STATUS);
  		}while (ret&1);
        }
}

void spi_disable_write_protect(void)
{
    unsigned char statusValue;
    int ret, var;

    statusValue = 0;

    /*write enable*/ 
    var = 1 << SPI_FLASH_WREN;
    writel(var, P_SPI_FLASH_CMD);
    while(readl(P_SPI_FLASH_CMD)!=0);
    ret=1;
    while ( (ret&1)==1 ) {   	
        var=1<<SPI_FLASH_RDSR;
        writel(var, P_SPI_FLASH_CMD);
        while(readl(P_SPI_FLASH_CMD)!=0);
        ret = readl(P_SPI_FLASH_STATUS)&0xff;
        }

    /*write status register*/
    writel(statusValue,P_SPI_FLASH_STATUS);
    var = 1<<SPI_FLASH_WRSR;
    writel(var, P_SPI_FLASH_CMD);
    while(readl(P_SPI_FLASH_CMD)!=0);
    ret=1;
    while ( (ret&1)==1 ) {   	
        var=1<<SPI_FLASH_RDSR;
        writel(var, P_SPI_FLASH_CMD);
        while(readl(P_SPI_FLASH_CMD)!=0);
        ret = readl(P_SPI_FLASH_STATUS)&0xff;
        }
}

int spi_check_write_protect(void)
{
    writel(1<<SPI_FLASH_RDSR, P_SPI_FLASH_CMD);
    while(readl(P_SPI_FLASH_CMD)!=0);
    return (readl(P_SPI_FLASH_STATUS)&(0x7<<2)) == (0x7<<2)?1:0;
}
#endif //SPI_WRITE_PROTECT

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
			unsigned int max_hz, unsigned int mode)
{
	struct aml_spi_slave * amls;
	amls = ( struct aml_spi_slave *)malloc(sizeof(struct aml_spi_slave));
	if (!amls)
		return NULL;

	spi_init();

	amls->slave.bus = bus;
	amls->slave.cs 	= cs;
#ifdef CONFIG_AML_A1H
	amls->adr_base 	=(void*)0xC1800000;
#elif defined CONFIG_MESON
    amls->adr_base 	=(void*)0x40000000;
#else
	amls->adr_base 	=(void*)0x400000;
#endif
	amls->mode=mode;					//mode 0 or mode3 ? FIXME	
	return &amls->slave;
}

void spi_free_slave(struct spi_slave *slave)
{
	struct aml_spi_slave *amls = to_aml_spi(slave);

	free(amls);
}

int spi_claim_bus(struct spi_slave *slave)
{

	debug("%s: bus:%i cs:%i\n", __func__, slave->bus, slave->cs);
#ifdef CONFIG_AML_MESION_1
	///MESON 1 
	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_1,( (1<<29) | (1<<27) | (1<<25) | (1<<23)));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,0x7fff);
	
//	*(P_SPI_FLASH_CTRL) = 0xea949;    // system clock / 10.
	
#endif	

#ifdef CONFIG_AML_MESION2
	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_2,( (1<<7) | (1<<8) | (1<<9) | (1<<10)|(1<<11)|(1<<12)));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_5,0x7fff);
#endif
#ifdef CONFIG_AML_MESION_3
    clrbits_le32(P_PERIPHS_PIN_MUX_2,7<<19);
    clrsetbits_le32(P_PERIPHS_PIN_MUX_5,(0xf<<6),(0xf));
#endif

	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
	debug("%s: bus:%i cs:%i\n", __func__, slave->bus, slave->cs);
#ifdef CONFIG_AML_MESION_1
	///MESON 1 
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1,( (1<<29) | (1<<27) | (1<<25) | (1<<23)));
#endif	
 
#ifdef CONFIG_AML_MESION2
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_2,( (1<<7) | (1<<8) | (1<<9) | (1<<10)|(1<<11)|(1<<12)));
#endif


}

//For Our Spi controller......
//note flag for  cmd adr status data trans
//bitlen is byte*8  
int spi_xfer(struct spi_slave *slave, unsigned int bitlen,
				const void *dout, void *din, unsigned long flags){


	struct aml_spi_slave *as = to_aml_spi(slave);
	u32 len;
	u32 temp_len;
	u8 temp_data[4];
	u8 *buf = temp_data;
	int		ret;
	const u8	*txp = (u8 *)dout;
	u8		*rxp = (u8 *)din;
	u32		value;
	int i,j;
	
	ret = 0;
	if (bitlen == 0)
		/* Finish any previously submitted transfers */
		goto out;
    

	if (bitlen % 8) {
		/* Errors always terminate an ongoing transfer */
		flags |= SPI_XFER_END;
		goto out;
	}

	len = bitlen / 8;

	
	if (flags & SPI_XFER_BEGIN)
		spi_cs_activate(slave);

	
	if(flags&SPI_XFER_ADR){
		writel(*((u32*)txp), P_SPI_FLASH_ADDR);
		goto out;	
	}
	
	if(flags&SPI_XFER_CMD){

		if(*((u32*)dout)==(1<<SPI_FLASH_RDID))	{
			writel(0, P_SPI_FLASH_C0);			/*only for rdid*/
		}
    	writel(*((u32*)txp), P_SPI_FLASH_CMD);
		while(readl(P_SPI_FLASH_CMD)!=0);

		goto out;
	}
	
		
	if((rxp)&&(flags&SPI_XFER_STATUS)){																/*FIXME WRSTAUS	*/	
		
		value=readl(P_SPI_FLASH_STATUS)&0xffff;
		*rxp=value&0xff;
		*(rxp+1)=(value>>8)&0xff;
		goto out;
	}

	if((rxp)&&(flags&SPI_XFER_ID)){																	/*FIXME EON*/
	
		value=readl(P_SPI_FLASH_C0)&0xffffff;
		*rxp=value&0xff;
		*(rxp+1)=(value>>8)&0xff;
		*(rxp+2)=(value>>16)&0xff;
		goto out;
	}	

	
	if((rxp)&&(flags&SPI_XFER_COPY)){
		writel(readl(P_SPI_FLASH_CTRL) | (SPI_ENABLE_AHB), P_SPI_FLASH_CTRL);							/*for AHB request*/
	  	memcpy((unsigned char *)(rxp),(unsigned char *)((as->adr_base)+(*(u32*)txp)),bitlen/8);
		goto out;
	}	

	
	if((rxp)&&(flags&SPI_XFER_READCACHE)){
		temp_len = len/4;
		for( i=0;i<temp_len*4;i+=4){		
			*(u32 *)(rxp+i)=readl(P_SPI_FLASH_C0+i);
		}	
		if(len%4){
			temp_len = len%4;
			*(u32 *)buf = readl(P_SPI_FLASH_C0+i);
			for(j=0;j<temp_len;j++){
				*(u8 *)(rxp+i+j)=*(buf+j);
			}
		}
		goto out;
	}	


	if((txp)&&(flags&SPI_XFER_WRITECACHE)){
		
		for( j=0;j<len;j+=4){
			writel(*(u32*)(txp+j), P_SPI_FLASH_C0+j);
		}	
	
		goto out;	
	}	
	


out:
	if (flags & SPI_XFER_END) {
		
		spi_cs_deactivate(slave);
	}

	return 0;

}
