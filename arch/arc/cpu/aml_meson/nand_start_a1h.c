#define C_ASSIST_HW_REV                            (volatile unsigned long *)0xc1107d4c
#define C_ASSIST_POR_CONFIG                        (volatile unsigned long *)0xc1107d54

#define PERIPHS_PIN_MUX_0                        (volatile unsigned long *)0xc11080b0

#define PNAND_CLE                 					(volatile unsigned long *)0xc1200300
#define PNAND_ALE                 					(volatile unsigned long *)0xc1200304
#define PNAND_WRITE               					(volatile unsigned long *)0xc1200308
#define PNAND_READ                					(volatile unsigned long *)0xc1200308

#define PNAND_GEN_CNTL                           (volatile unsigned long *)0xc1108600
#define PNAND_GEN_TIMING                         (volatile unsigned long *)0xc1108604
#define PNAND_DMA_START_ADDR                     (volatile unsigned long *)0xc1108608
#define PNAND_DMA_XFER_COUNT                     (volatile unsigned long *)0xc110860c
#define PNAND_DMA_CNTL                           (volatile unsigned long *)0xc1108610
#define PNAND_DMA_COL_PAGE                       (volatile unsigned long *)0xc1108614
#define PNAND_DMA_BURST                          (volatile unsigned long *)0xc1108618
#define PNAND_USER_3_0                           (volatile unsigned long *)0xc110861c
#define PNAND_USER_7_4                           (volatile unsigned long *)0xc1108620
#define PNAND_USER_11_8                          (volatile unsigned long *)0xc1108624
#define PNAND_USER_15_12                         (volatile unsigned long *)0xc1108628
#define PNAND_ERR_LOC                            (volatile unsigned long *)0xc110862c
#define PNAND_ERR_CNT0                           (volatile unsigned long *)0xc1108630
#define PNAND_ERR_CNT1                           (volatile unsigned long *)0xc1108634
#define PNAND_ERR_UNC                            (volatile unsigned long *)0xc1108638
#define PNAND_USER_B_3_0                         (volatile unsigned long *)0xc110863c
#define PNAND_USER_B_7_4                         (volatile unsigned long *)0xc1108640
#define PNAND_USER_B_11_8                        (volatile unsigned long *)0xc1108648
#define PNAND_USER_B_15_12                       (volatile unsigned long *)0xc110864c

#define C_TIMERE                               	 (volatile unsigned long *)0xc1109954

#define MEMORY_LOC      0xc1001000 

#define C_ROM_BOOT_DEBUG_0                         (volatile unsigned long *)(MEMORY_LOC + 0xc00)
#define C_ROM_BOOT_DEBUG_1                         (volatile unsigned long *)(MEMORY_LOC + 0xc04)
#define C_ROM_BOOT_DEBUG_2                         (volatile unsigned long *)(MEMORY_LOC + 0xc08)
#define C_ROM_BOOT_DEBUG_3                         (volatile unsigned long *)(MEMORY_LOC + 0xc0c)

_nand_read(unsigned dummy,unsigned target_addr,unsigned size)
{
    unsigned i,j;
    unsigned page,is_4k_bugfix = 0;
    unsigned err_count;
    unsigned por_config     = (*C_ASSIST_POR_CONFIG);
//    register spi_a_b        = (por_config & (1 << 8)) ? 1 : 0;
//    register bch15          = (por_config & (1 << 2)) ? 1 : 0;
//    register sdram          = (por_config >> 9) & 0x3;
//    register NAND_boot      = (por_config & (1 << 4)) ? 1 : 0;
//    register SD_boot_type   = (por_config >> 11) & 0x3;  // bits [6:5]
//    register int clock_27   = ((*C_ASSIST_HW_REV) & (1 << 12)) ? 0 : 1;
    unsigned bch15 = 0;
	unsigned nand_id;
    (*C_ROM_BOOT_DEBUG_3) = por_config;
	unsigned base;

        // Pin_mux....enable PNAND pins and disable all other configurations
        // wire            pm_NAND_pins_en             = pin_mux_reg0[0];  // DON'T CHANGE....Part of ROM Code
//	(*PERIPHS_PIN_MUX_0)    |= (1 << 0);
	(*PNAND_GEN_CNTL)	|= (1<<6);
	
	(*PNAND_CLE)=0x90;
	(*PNAND_ALE)=0x00;
	
	nand_id=(*PNAND_READ);
		
	if(((nand_id>>24)&0x3)==2)
		is_4k_bugfix=2;
	else
		is_4k_bugfix=1;

//	unsigned end_pages=(size*4+1535)/1536;
	unsigned end_pages = 0;
	end_pages = (size*4+0x800*is_4k_bugfix-1)&(~(0x800*is_4k_bugfix-1));
	end_pages = end_pages>>(11+is_4k_bugfix-1);
	 base=((((*C_ROM_BOOT_DEBUG_2)))<<8)+2;
		for(i=0;i<end_pages;i++)
		{    
        		// Set CLE and ALE for Normal DMA
            (*PNAND_CLE)    = 0x00000000;
            (*PNAND_ALE)    = 0x00000000;           // Column [7:0]
            (*PNAND_ALE)    = 0x00000000;           // Column [11:8]
            (*PNAND_ALE)    = (i+base)&0xff;           // page
            (*PNAND_ALE)    = ((i+base)>>8)&0xff;       // page
            (*PNAND_ALE)    = 0x00000000;           // page
            (*PNAND_CLE)    = 0x00000030;
            (*C_TIMERE) = 0;    // reset the counter
            while( (*C_TIMERE) < 1 ) {}
			if(i>0)
				(*PNAND_DMA_START_ADDR) = target_addr+((i*is_4k_bugfix)*0x800)-(is_4k_bugfix*0x800-0x400);
			else
    			(*PNAND_DMA_START_ADDR) = target_addr+((i*is_4k_bugfix)*0x800);
    		(*PNAND_DMA_XFER_COUNT) = 0x800*is_4k_bugfix;                     // Move 2k bytes 
    		(*PNAND_DMA_CNTL)       =   (bch15 << 26)   |       // 1 =  BCH-15, 0 = BCH-9 
                                (1 << 24)       |       // Enable BCH
                                (0 << 21)       |       // endian = 00 
                                (1 << 20)       |       // Read the NAND
                                (0 << 19)       |       // ECC Endian
                                (0 << 17)       |       // boot DMA
                                (1 << 16)       |       // Normal DMA
                                (10 << 0);
        // Wait for DMA to complete
        while( (*PNAND_DMA_CNTL) & (1 << 31) ) {}
    
        for( page = 0; page < (4*is_4k_bugfix); page++ ) {
            // If page transferred and has correctable errors
            if( !((*PNAND_ERR_UNC) & (1 << page)) ) {
                unsigned sdram_base;
                // Fix errors for 512 bytes (page)
                err_count = ((*PNAND_ERR_CNT0) >> ((page)*4)) & 0xf;
                if(i>0)
                	sdram_base = target_addr+((i*is_4k_bugfix)*0x800)-(is_4k_bugfix*0x800-0x400)+page*512;
                else
                	sdram_base = target_addr+((i*is_4k_bugfix)*0x800)+ page*512;
                // Point to the error list in the FIFO/memory
                if( bch15 ) 
                	{ 
                		(*PNAND_ERR_LOC) = page*15; 
                	}
                else        
                	{ 
                		(*PNAND_ERR_LOC) = page*9;  
                	}
                for(j = 0; j < err_count; j++ ) {
                    unsigned byte; 
                    unsigned bit;
                    byte    = (*PNAND_ERR_LOC); // Read byte/bit location
                    bit     = byte & 0x7;
                    
                    // If the error is in the payload section
                    if( (byte >> 4) <= 511 ) {
                        (*(volatile unsigned char *)(sdram_base + (byte >> 4))) ^= (1 << bit);
                    }
                    if( (byte >> 4) == 512){
						(*(volatile unsigned char *)(PNAND_USER_3_0 + page)) ^= (1 << bit);
					}
                }
            }
        }
 	  }
    
}

