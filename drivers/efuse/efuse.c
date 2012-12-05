/*
 * E-FUSE char device driver.
 *
 * Author: Bo Yang <bo.yang@amlogic.com>
 *
 * Copyright (c) 2010 Amlogic Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the smems of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 */
 
#include <config.h>
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <asm/byteorder.h>
#include <asm/arch/am_regs.h>
#include <linux/types.h>

#include "efuse.h"
#include "efuse_regs.h"

#define EFUSE_MODULE_NAME   "efuse"
#define EFUSE_DRIVER_NAME   "efuse"
#define EFUSE_DEVICE_NAME   "efuse"
#define EFUSE_CLASS_NAME    "efuse"

#define EFUSE_BITS             3072
#define EFUSE_BYTES            384  //(EFUSE_BITS/8)
#define EFUSE_DWORDS            96  //(EFUSE_BITS/32)

#define DOUBLE_WORD_BYTES        4
#define EFUSE_IS_OPEN           (0x01)


unsigned char re_licence[EFUSE_LICENSE_BYTES] = {0};	//4
unsigned char re_mac[EFUSE_MACADDR_BYTES+1] = {0};		//8
unsigned char re_hdcp[EFUSE_HDMHDCP_BYTES+2] = {0};		//312
unsigned char re_usid[EFUSE_USERIDF_BYTES+2] = {0};		//64
#ifdef CONFIG_EFUSE_DUMP
unsigned char efuse_buf[EFUSE_BYTES] = {0};
#endif

static void __efuse_write_byte( unsigned long addr, unsigned long data );
static void __efuse_read_dword( unsigned long addr, unsigned long *data);


static void __efuse_write_byte( unsigned long addr, unsigned long data )
{
    unsigned long auto_wr_is_enabled = 0;

    if ( READ_CBUS_REG( EFUSE_CNTL1) & ( 1 << CNTL1_AUTO_WR_ENABLE_BIT ) )
    {
        auto_wr_is_enabled = 1;
    }
    else
    {
        /* temporarily enable Write mode */
        WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_AUTO_WR_ENABLE_ON,
            CNTL1_AUTO_WR_ENABLE_BIT, CNTL1_AUTO_WR_ENABLE_SIZE );
    }

    /* write the address */
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, addr,
        CNTL1_BYTE_ADDR_BIT, CNTL1_BYTE_ADDR_SIZE );
    /* set starting byte address */
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_BYTE_ADDR_SET_ON,
        CNTL1_BYTE_ADDR_SET_BIT, CNTL1_BYTE_ADDR_SET_SIZE );
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_BYTE_ADDR_SET_OFF,
        CNTL1_BYTE_ADDR_SET_BIT, CNTL1_BYTE_ADDR_SET_SIZE );

    /* write the byte */
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, data,
        CNTL1_BYTE_WR_DATA_BIT, CNTL1_BYTE_WR_DATA_SIZE );
    /* start the write process */
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_AUTO_WR_START_ON,
        CNTL1_AUTO_WR_START_BIT, CNTL1_AUTO_WR_START_SIZE );
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_AUTO_WR_START_OFF,
        CNTL1_AUTO_WR_START_BIT, CNTL1_AUTO_WR_START_SIZE );
    /* dummy read */
    READ_CBUS_REG( EFUSE_CNTL1 );

    while ( READ_CBUS_REG(EFUSE_CNTL1) & ( 1 << CNTL1_AUTO_WR_BUSY_BIT ) )
    {
        udelay(1);
    }

    /* if auto write wasn't enabled and we enabled it, then disable it upon exit */
    if (auto_wr_is_enabled == 0 )
    {
        WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_AUTO_WR_ENABLE_OFF,
            CNTL1_AUTO_WR_ENABLE_BIT, CNTL1_AUTO_WR_ENABLE_SIZE );
    }
}

static void __efuse_read_dword( unsigned long addr, unsigned long *data )
{
    unsigned long auto_rd_is_enabled = 0;

    if( READ_CBUS_REG(EFUSE_CNTL1) & ( 1 << CNTL1_AUTO_RD_ENABLE_BIT ) )
    {
        auto_rd_is_enabled = 1;
    }
    else
    {
        /* temporarily enable Read mode */
        WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_AUTO_RD_ENABLE_ON,
            CNTL1_AUTO_RD_ENABLE_BIT, CNTL1_AUTO_RD_ENABLE_SIZE );
    }

    /* write the address */
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, addr,
        CNTL1_BYTE_ADDR_BIT,  CNTL1_BYTE_ADDR_SIZE );
    /* set starting byte address */
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_BYTE_ADDR_SET_ON,
        CNTL1_BYTE_ADDR_SET_BIT, CNTL1_BYTE_ADDR_SET_SIZE );
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_BYTE_ADDR_SET_OFF,
        CNTL1_BYTE_ADDR_SET_BIT, CNTL1_BYTE_ADDR_SET_SIZE );
    /* start the read process */
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_AUTO_WR_START_ON,
        CNTL1_AUTO_RD_START_BIT, CNTL1_AUTO_RD_START_SIZE );
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_AUTO_WR_START_OFF,
        CNTL1_AUTO_RD_START_BIT, CNTL1_AUTO_RD_START_SIZE );
    /* dummy read */
    READ_CBUS_REG( EFUSE_CNTL1 );
    while ( READ_CBUS_REG(EFUSE_CNTL1) & ( 1 << CNTL1_AUTO_RD_BUSY_BIT ) )
    {
        udelay(1);
    }
    /* read the 32-bits value */
    ( *data ) = READ_CBUS_REG( EFUSE_CNTL2 );
    /* if auto read wasn't enabled and we enabled it, then disable it upon exit */
    if ( auto_rd_is_enabled == 0 )
    {
        WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_AUTO_RD_ENABLE_OFF,
            CNTL1_AUTO_RD_ENABLE_BIT, CNTL1_AUTO_RD_ENABLE_SIZE );
    }

//    printf("__efuse_read_dword: addr=%ld, data=0x%lx\n", addr, *data);
}

ssize_t efuse_read(char *buf, size_t count, loff_t *ppos )
{
    unsigned long contents[EFUSE_DWORDS];
	unsigned pos = *ppos;
    unsigned long *pdw;
    unsigned int dwsize = (count + 3)/4;

	if (pos >= EFUSE_BYTES)
		return 0;

	if (count > EFUSE_BYTES - pos)
		count = EFUSE_BYTES - pos;
	if (count > EFUSE_BYTES)
		return -1;

    memset(contents, 0, sizeof(contents));

#ifdef CONFIG_AML_MESION_3
	// Enabel auto-read mode
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_AUTO_RD_ENABLE_ON,
            CNTL1_AUTO_RD_ENABLE_BIT, CNTL1_AUTO_RD_ENABLE_SIZE );
#endif

	for (pdw = contents; dwsize-- > 0 && pos < EFUSE_BYTES; pos += 4, ++pdw)
		__efuse_read_dword(pos, pdw);
	
#ifdef CONFIG_AML_MESION_3
    // Disable auto-read mode    
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_AUTO_RD_ENABLE_OFF,
            CNTL1_AUTO_RD_ENABLE_BIT, CNTL1_AUTO_RD_ENABLE_SIZE );
#endif
	
    memcpy(buf, contents, count);

    *ppos += count;
    return count;
}

ssize_t efuse_write(const char *buf, size_t count, loff_t *ppos )
{
 	unsigned char contents[EFUSE_BYTES];
	unsigned pos = *ppos;
	unsigned char *pc;

	if (pos >= EFUSE_BYTES)
		return 0;	/* Past EOF */

	if (count > EFUSE_BYTES - pos)
		count = EFUSE_BYTES - pos;
	if (count > EFUSE_BYTES)
		return -1;

	memcpy(contents, buf, count);

	for (pc = contents; count--; ++pos, ++pc)
		__efuse_write_byte(pos, *pc);

	*ppos = pos;

	return pc - contents;
}

int efuse_chk_written(int usr_type)
{
	int ret = 0;
	unsigned char buf[EFUSE_BYTES];
	loff_t ppos;
	size_t count;
	int dec_len,i;
	char *op;
	
	switch(usr_type){
		case USR_LICENCE: ppos =0; count=4;op=re_licence;break;
		case USR_MACADDR: ppos =4; count=8;op=re_mac;break;
		case USR_HDMIHDCP: ppos =12; count=312;op=re_hdcp;break;
		case USR_USERIDF: ppos =320; count=64;op=re_usid;break;
	}	
	
	efuse_init();
	memset(op,0,count);
	memset(buf,0,sizeof(buf));
	efuse_read(buf, count, &ppos);	
	
	unsigned char ckbit=0x0;
	for(i=0;i<count;i++){
		if(buf[i]|ckbit){
			ret = -1;
			break ;
		}
	}
	
	return ret ;
}

unsigned char *efuse_read_usr(int usr_type)
{
	unsigned char buf[EFUSE_BYTES];
	unsigned char buf_dec[EFUSE_BYTES];
	loff_t ppos;
	size_t count;
	int dec_len,i;
	char *op;
	
	switch(usr_type){
		case USR_LICENCE: ppos =0; count=4;dec_len=4;op=re_licence;break;
		case USR_MACADDR: ppos =4; count=8;dec_len=7;op=re_mac;break;
		case USR_HDMIHDCP: ppos =12; count=312;dec_len=310;op=re_hdcp;break;
		case USR_USERIDF: ppos =320; count=64;dec_len=62;op=re_usid;break;
	}
	
	efuse_init();
	memset(op,0,count);
	memset(buf,0,sizeof(buf));
	memset(buf_dec,0,sizeof(buf_dec));
	efuse_read(buf, count, &ppos);
	
	if(usr_type==USR_USERIDF)
		memcpy(buf_dec,buf+2,dec_len);
	else
		memcpy(buf_dec,buf,dec_len);
	
	if(dec_len>30)
		for(i=0;i*31<dec_len;i++)
			efuse_bch_dec(buf_dec+i*31, 31, op+i*30);
	else 
		efuse_bch_dec(buf_dec, dec_len, op);
	
	return op;
	
}
unsigned char *efuse_read_usr_workaround(int usr_type)
{
	
	unsigned char buf[EFUSE_BYTES];
	unsigned char buf_dec[EFUSE_BYTES];
	loff_t ppos;
	size_t count;
	int dec_len,i;
	char *op;
	
	switch(usr_type){
		case USR_LICENCE: ppos =0; count=4;dec_len=4;op=re_licence;break;
		case USR_MACADDR: ppos =4; count=8;dec_len=7;op=re_mac;break;
		case USR_HDMIHDCP: ppos =12; count=312;dec_len=310;op=re_hdcp;break;
		case USR_USERIDF: ppos =324; count=24;dec_len=21;op=re_usid;break;
	}
	
	efuse_init();
	memset(op,0,count);
	memset(buf,0,sizeof(buf));
	memset(buf_dec,0,sizeof(buf_dec));
	efuse_read(buf, count, &ppos);
	
	/*
	if(usr_type==USR_USERIDF)
		memcpy(buf_dec,buf+2,dec_len);
	else*/
		memcpy(buf_dec,buf,dec_len);
	
	if(dec_len>30)
		for(i=0;i*31<dec_len;i++)
			efuse_bch_dec(buf_dec+i*31, 31, op+i*30);
	else 
		efuse_bch_dec(buf_dec, dec_len, op);
	
	return op;
	
	
}


int efuse_write_usr(int usr_type, unsigned char *data)
{
	int ret = 0;
	unsigned char buf[EFUSE_BYTES];
	int count,enc_len,data_len,i;
	loff_t ppos;
	char *ip;
	
	switch(usr_type){
		case USR_LICENCE: ppos =0; 		count=4;	enc_len=4;	data_len=3;		ip=re_licence;	break;
		case USR_MACADDR: ppos =4; 		count=8;	enc_len=7;	data_len=6;		ip=re_mac;break;
		case USR_HDMIHDCP: ppos =12; 	count=312;	enc_len=310;data_len=300;	ip=re_hdcp;break;
		case USR_USERIDF: ppos =322; 	count=64;	enc_len=62;	data_len=60;	ip=re_usid;break;
	}
	
	efuse_init();

	if(efuse_chk_written(usr_type))
		return -1;
	
	memset(ip,0,count);
	memset(buf,0,sizeof(buf));
	memcpy(buf,data,data_len);
	
	if(enc_len>=30)
		for(i=0;i*30<data_len;i++)
			efuse_bch_enc(buf+i*30, 30, ip+i*31);
	else 
		efuse_bch_enc(buf, data_len, ip);
	
	efuse_write(ip, enc_len, &ppos);
	
	return 0 ;
}

int efuse_init(void)
{
    /* disable efuse encryption */
    WRITE_CBUS_REG_BITS( EFUSE_CNTL4, CNTL1_AUTO_WR_ENABLE_OFF,
        CNTL4_ENCRYPT_ENABLE_BIT, CNTL4_ENCRYPT_ENABLE_SIZE );

    return 0;
}


#ifdef CONFIG_EFUSE_DUMP
unsigned char* efuse_dump()
{
	  int i=0;
    unsigned pos;
    memset(efuse_buf, 0, sizeof(efuse_buf));

#ifdef CONFIG_AML_MESION_3		
	// Enabel auto-read mode
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_AUTO_RD_ENABLE_ON,
            CNTL1_AUTO_RD_ENABLE_BIT, CNTL1_AUTO_RD_ENABLE_SIZE );
#endif            

	for(i=0; i<384; i+=4)
		__efuse_read_dword(i,  &(efuse_buf[i]));	    
		
#ifdef CONFIG_AML_MESION_3		
    // Disable auto-read mode    
    WRITE_CBUS_REG_BITS( EFUSE_CNTL1, CNTL1_AUTO_RD_ENABLE_OFF,
            CNTL1_AUTO_RD_ENABLE_BIT, CNTL1_AUTO_RD_ENABLE_SIZE );
#endif
     
     return (char*)efuse_buf;
}
#endif
