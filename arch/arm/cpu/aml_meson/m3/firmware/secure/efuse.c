#include "registers.h"

#define CNTL1_PD_ENABLE_BIT					27
#define CNTL1_PD_ENABLE_SIZE					1
#define CNTL1_PD_ENABLE_ON	1
#define CNTL1_PD_ENABLE_OFF   0

#define CNTL1_AUTO_RD_BUSY_BIT              26
#define CNTL1_AUTO_RD_BUSY_SIZE             1

#define CNTL1_AUTO_RD_START_BIT             25
#define CNTL1_AUTO_RD_START_SIZE            1

#define CNTL1_AUTO_RD_ENABLE_BIT            24
#define CNTL1_AUTO_RD_ENABLE_SIZE           1
#define CNTL1_AUTO_RD_ENABLE_ON             1
#define CNTL1_AUTO_RD_ENABLE_OFF            0

#define CNTL1_BYTE_WR_DATA_BIT              16
#define CNTL1_BYTE_WR_DATA_SIZE             8

#define CNTL1_AUTO_WR_BUSY_BIT              14
#define CNTL1_AUTO_WR_BUSY_SIZE             1

#define CNTL1_AUTO_WR_START_BIT             13
#define CNTL1_AUTO_WR_START_SIZE            1
#define CNTL1_AUTO_WR_START_ON              1
#define CNTL1_AUTO_WR_START_OFF             0

#define CNTL1_AUTO_WR_ENABLE_BIT            12
#define CNTL1_AUTO_WR_ENABLE_SIZE           1
#define CNTL1_AUTO_WR_ENABLE_ON             1
#define CNTL1_AUTO_WR_ENABLE_OFF            0

#define CNTL1_BYTE_ADDR_SET_BIT             11
#define CNTL1_BYTE_ADDR_SET_SIZE            1
#define CNTL1_BYTE_ADDR_SET_ON              1
#define CNTL1_BYTE_ADDR_SET_OFF             0

#define CNTL1_BYTE_ADDR_BIT                 0
#define CNTL1_BYTE_ADDR_SIZE                10

/* EFUSE_CNTL2 */

/* EFUSE_CNTL3 */

/**
EFUSE_CNTL4

bit[31-24]
bit[23-16]  RD_CLOCK_HIGH
bit[15-11]
bit[10]     Encrypt enable
bit[9]      Encrypt reset
bit[8]      XOR_ROTATE
bit[7-0]    XOR
**/
#define CNTL4_ENCRYPT_ENABLE_BIT            10
#define CNTL4_ENCRYPT_ENABLE_SIZE           1
#define CNTL4_ENCRYPT_ENABLE_ON             1
#define CNTL4_ENCRYPT_ENABLE_OFF            0

#define CNTL4_ENCRYPT_RESET_BIT             9
#define CNTL4_ENCRYPT_RESET_SIZE            1
#define CNTL4_ENCRYPT_RESET_ON              1
#define CNTL4_ENCRYPT_RESET_OFF             0


#define CNTL4_XOR_ROTATE_BIT                8
#define CNTL4_XOR_ROTATE_SIZE               1

#define CNTL4_XOR_BIT                       0
#define CNTL4_XOR_SIZE                      8


#define EFUSE_BYTES            384  //(EFUSE_BITS/8)
#define EFUSE_DWORDS            96  //(EFUSE_BITS/32)

#define WRITE_EFUSE_REG(reg, val)  writel(val, reg)
#define READ_EFUSE_REG(reg)  readl(reg)
#define WRITE_EFUSE_REG_BITS(reg, val, start, len) \
	WRITE_EFUSE_REG(reg,	(READ_EFUSE_REG(reg) & ~(((1L<<(len))-1)<<(start)) )| ((unsigned)((val)&((1L<<(len))-1)) << (start)))
#define READ_EFUSE_REG_BITS(reg, start, len) \
	((READ_EFUSE_REG(reg) >> (start)) & ((1L<<(len))-1))


static int which;

#ifdef SECURE_EFUSE_DEBUG
static char efusedebug[EFUSE_BYTES];

void efuse_debug_init()
{
	memset((char*)efusedebug, 0, EFUSE_BYTES);
	const unsigned char auth_bch[]={
		0x54, 0x55, 0x79, 0x69, 0xBE, 0x43, 0x65, 0x41, 0xD2, 0xE6, 0xCA, 0x4B, 0x66, 0x11, 0x7F, 0x82, 0x83, 
		0x2B, 0x25, 0x0D, 0x5B, 0xDF, 0x30, 0xFC, 0xAA, 0x4F, 0xD6, 0x18, 0x72, 0x8A, 0x58, 0x64, 0xA9, 0xB2, 
	};
	unsigned char sn_bch[] = {
		0x4D, 0x52, 0x56, 0x4C, 0x43, 0x54, 0x43, 0x43, 0x32, 0x30, 0x31, 0x32, 0x14, 
		0x30, 0x37, 0x31, 0x32, 0x53, 0x4E, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x8D, 
	};
	unsigned char in_bch[] = {
		0x4D, 0x52, 0x56, 0x4C, 0x32, 0x30, 0x31, 0x32, 0x30, 0x37, 0x31, 0x32, 0xEC, 
		0x43, 0x48, 0x49, 0x50, 0x53, 0x4E, 0x30, 0x30, 0x30, 0x30, 0x30, 0x31, 0x4A, 
	};
	unsigned char ci_bch[] = {
		0x4D, 0x52, 0x56, 0x4C, 0xC4,
	};
	unsigned char version_bch[] = {
		0x03, 0x00, 0x00, 0x8C,
	};
	
	unsigned char *pdest;
	pdest = (unsigned char*)(&(efusedebug[112]));
	memcpy(pdest, auth_bch, 34);
	
	pdest = (unsigned char*)(&(efusedebug[146]));
	memcpy(pdest, ci_bch, 5);
	
	/*pdest = (unsigned char*)(&(efusedebug[151]));
	memcpy(pdest, sn_bch, 26);
	
	pdest = (unsigned char*)(&(efusedebug[177]));
	memcpy(pdest, in_bch, 26);
	*/
	
	pdest = (unsigned char*)(&(efusedebug[380]));
	memcpy(pdest, version_bch, 7);
	
}

void efuse_debug_get(unsigned pos, int size, char *pOut)
{	
	//memcpy(pOut, (char*)(&(efusedebug[pos])), size);
	char* pdest=(char*)((unsigned)efusedebug + pos);
	memcpy(pOut, pdest, size);
}
#endif



static unsigned efuse_reg_addr(unsigned offset)
{
	if(which==FROM_UBOOT)
		return (IO_EFUSE_PHY_BASE+offset);	
	else
		return (IO_EFUSE_BASE+offset);	
}

static unsigned timerE_get()
{
	unsigned addr;
	if(which==FROM_UBOOT)
		addr=IO_CBUS_PHY_BASE+(ISA_TIMERE<<2);
	else
		addr=IO_CBUS_BASE+(ISA_TIMERE<<2);
	
	return (readl(addr)&0xffffff);
}

static void  udelay(unsigned long usec)
{
    unsigned long base=timerE_get();
    while(TIMERE_SUB(timerE_get(),base)<usec);    
}


static void efuse_init(void)
{
	 WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL4), CNTL4_ENCRYPT_ENABLE_OFF,
        												CNTL4_ENCRYPT_ENABLE_BIT, CNTL4_ENCRYPT_ENABLE_SIZE );
      WRITE_EFUSE_REG_BITS(efuse_reg_addr(EFUSE_CNTL1), CNTL1_PD_ENABLE_OFF, 
													CNTL1_PD_ENABLE_BIT, CNTL1_PD_ENABLE_SIZE);      										
}


static void __efuse_read_dword( unsigned long addr, unsigned long *data)
{	
#ifdef SECURE_EFUSE_DEBUG
	unsigned *p =efusedebug;
	*data = p[addr>>2];
	return;
#endif	
	    /* write the address */    	
    WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), addr,
        					CNTL1_BYTE_ADDR_BIT,  CNTL1_BYTE_ADDR_SIZE );
    	
    /* set starting byte address */
    WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), CNTL1_BYTE_ADDR_SET_ON,
        								CNTL1_BYTE_ADDR_SET_BIT, CNTL1_BYTE_ADDR_SET_SIZE );	
    WRITE_EFUSE_REG_BITS(efuse_reg_addr(EFUSE_CNTL1), CNTL1_BYTE_ADDR_SET_OFF,
    								    CNTL1_BYTE_ADDR_SET_BIT, CNTL1_BYTE_ADDR_SET_SIZE );
   /* start the read process */
    WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), CNTL1_AUTO_WR_START_ON,
        								CNTL1_AUTO_RD_START_BIT, CNTL1_AUTO_RD_START_SIZE );      
    WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), CNTL1_AUTO_WR_START_OFF,
        								CNTL1_AUTO_RD_START_BIT, CNTL1_AUTO_RD_START_SIZE );
      
    /* dummy read */
    READ_EFUSE_REG( efuse_reg_addr(EFUSE_CNTL1) );
    while ( READ_EFUSE_REG(efuse_reg_addr(EFUSE_CNTL1)) & ( 1 << CNTL1_AUTO_RD_BUSY_BIT ) )
    {
        udelay(1);
    }
    /* read the 32-bits value */
    ( *data ) = READ_EFUSE_REG( efuse_reg_addr(EFUSE_CNTL2) );          

}

static void __efuse_write_byte( unsigned long addr, unsigned long data )
{
#ifdef SECURE_EFUSE_DEBUG
	char *p = (char*)efusedebug;
	p[addr] = data;
	return;
#endif
	
    unsigned long auto_wr_is_enabled = 0;

    if ( READ_EFUSE_REG( efuse_reg_addr(EFUSE_CNTL1)) & ( 1 << CNTL1_AUTO_WR_ENABLE_BIT ) )
    {
        auto_wr_is_enabled = 1;
    }
    else
    {
        /* temporarily enable Write mode */
        WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), CNTL1_AUTO_WR_ENABLE_ON,
            CNTL1_AUTO_WR_ENABLE_BIT, CNTL1_AUTO_WR_ENABLE_SIZE );
    }

    /* write the address */
    WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), addr,
        CNTL1_BYTE_ADDR_BIT, CNTL1_BYTE_ADDR_SIZE );
    /* set starting byte address */
    WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), CNTL1_BYTE_ADDR_SET_ON,
        CNTL1_BYTE_ADDR_SET_BIT, CNTL1_BYTE_ADDR_SET_SIZE );
    WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), CNTL1_BYTE_ADDR_SET_OFF,
        CNTL1_BYTE_ADDR_SET_BIT, CNTL1_BYTE_ADDR_SET_SIZE );

    /* write the byte */
    WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), data,
        CNTL1_BYTE_WR_DATA_BIT, CNTL1_BYTE_WR_DATA_SIZE );
    /* start the write process */
    WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), CNTL1_AUTO_WR_START_ON,
        CNTL1_AUTO_WR_START_BIT, CNTL1_AUTO_WR_START_SIZE );
    WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), CNTL1_AUTO_WR_START_OFF,
        CNTL1_AUTO_WR_START_BIT, CNTL1_AUTO_WR_START_SIZE );
    /* dummy read */
    READ_EFUSE_REG( efuse_reg_addr(EFUSE_CNTL1) );

    while ( READ_EFUSE_REG(efuse_reg_addr(EFUSE_CNTL1)) & ( 1 << CNTL1_AUTO_WR_BUSY_BIT ) )
    {
        udelay(1);
    }

    /* if auto write wasn't enabled and we enabled it, then disable it upon exit */
    if (auto_wr_is_enabled == 0 )
    {
        WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), CNTL1_AUTO_WR_ENABLE_OFF,
            CNTL1_AUTO_WR_ENABLE_BIT, CNTL1_AUTO_WR_ENABLE_SIZE );
    }	

}

/**********************************************************************************/
/* efuse interface APIs */
/**********************************************************************************/
int efuse_read(char *buf, unsigned count, unsigned *ppos, int from)
{
    unsigned long contents[EFUSE_DWORDS];
	unsigned pos = *ppos;
    unsigned long *pdw;
    unsigned residunt = pos%4;
    unsigned int dwsize = (count+residunt+3)>>2;
    
	if (pos >= EFUSE_BYTES)
		return 0;
	if (count > EFUSE_BYTES - pos)
		count = EFUSE_BYTES - pos;
	if (count > EFUSE_BYTES)
		return -1;
	
	which = from;	
	efuse_init();

    memset(contents, 0, sizeof(contents));

 	// Enabel auto-read mode    
    WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), CNTL1_AUTO_RD_ENABLE_ON,
             							CNTL1_AUTO_RD_ENABLE_BIT, CNTL1_AUTO_RD_ENABLE_SIZE ); 
    pos = (pos/4)*4;
    for (pdw = contents; dwsize-- > 0 && pos < EFUSE_BYTES; pos += 4, ++pdw)
		__efuse_read_dword(pos, pdw);	    

     // Disable auto-read mode    
    WRITE_EFUSE_REG_BITS( efuse_reg_addr(EFUSE_CNTL1), CNTL1_AUTO_RD_ENABLE_OFF,
             CNTL1_AUTO_RD_ENABLE_BIT, CNTL1_AUTO_RD_ENABLE_SIZE );
            
	memcpy(buf, (char*)contents+residunt, count);	
	
    *ppos += count;
    return 0;
}


int efuse_write(char *buf, unsigned count, unsigned *ppos, int from)
{
	unsigned pos = *ppos;
	const char *pc;
	unsigned long contents[EFUSE_DWORDS];
	
	if (pos >= EFUSE_BYTES)
		return 0;	/* Past EOF */
	if (count > EFUSE_BYTES - pos)
		count = EFUSE_BYTES - pos;
	if (count > EFUSE_BYTES)
		return -1;

	which=from;
	efuse_init();
	
	int i;	
	// checkout if have been written
	memset(contents, 0, EFUSE_DWORDS);
	pc = (char*)contents;
	efuse_read(pc, count, &pos, from);
	for(i=0; i<count; i++){
		if(pc[i] != 0)
			return -1;
	}
	
	pos = *ppos;
	for (pc = buf; count--; ++pos, ++pc)
		__efuse_write_byte(pos, *pc);
		
	*ppos = pos;
	
	return 0;	
}

