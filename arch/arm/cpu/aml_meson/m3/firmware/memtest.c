/**********************************************************************
 *
 * Filename:    memtest.c
 * 
 * Description: General-purpose memory testing functions.
 *
 * Notes:       This software can be easily ported to systems with
 *              different data bus widths by redefining 'datum'.
 *
 * 
 * Copyright (c) 1998 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/


#include "memtest.h"
#define ENABLE_OUTPUT
#define RETURN_PRINT(a) {if(a){serial_put_dword(a);}return a;}
#ifndef FIRMWARE_IN_ONE_FILE
#define STATIC_PREFIX
#else
#define STATIC_PREFIX static
#endif
//#if (defined CONFIG_MEM_TEST)||(defined AML_DEBUG_ROM)


/**********************************************************************
 *
 * Function:    memTestDataBus()
 *
 * Description: Test the data bus wiring in a memory region by
 *              performing a walking 1's test at a fixed address
 *              within that region.  The address (and hence the
 *              memory region) is selected by the caller.
 *
 * Notes:       
 *
 * Returns:     0 if the test succeeds.  
 *              A non-zero result is the first pattern that failed.
 *
 **********************************************************************/
STATIC_PREFIX
datum
memTestDataBus(datum * address)
{
    datum pattern;


    /*
     * Perform a walking 1's test at the given address.
     */
    for (pattern = 1; pattern != 0; pattern <<= 1)
    {
        /*
         * Write the test pattern.
         */
        *address = pattern;

        /*
         * Read it back (immediately is okay for this test).
         */
        if (*address != pattern) 
        {
            return (pattern);
        }
    }

    return (0);

}   /* memTestDataBus() */


/**********************************************************************
 *
 * Function:    memTestAddressBus()
 *
 * Description: Test the address bus wiring in a memory region by
 *              performing a walking 1's test on the relevant bits
 *              of the address and checking for aliasing. This test
 *              will find single-bit address failures such as stuck
 *              -high, stuck-low, and shorted pins.  The base address
 *              and size of the region are selected by the caller.
 *
 * Notes:       For best results, the selected base address should
 *              have enough LSB 0's to guarantee single address bit
 *              changes.  For example, to test a 64-Kbyte region, 
 *              select a base address on a 64-Kbyte boundary.  Also, 
 *              select the region size as a power-of-two--if at all 
 *              possible.
 *
 * Returns:     NULL if the test succeeds.  
 *              A non-zero result is the first address at which an
 *              aliasing problem was uncovered.  By examining the
 *              contents of memory, it may be possible to gather
 *              additional information about the problem.
 *
 **********************************************************************/
STATIC_PREFIX
datum * 
memTestAddressBus(datum * baseAddress, unsigned long nBytes)
{
    unsigned long addressMask = (nBytes/sizeof(datum) - 1);
    unsigned long offset;
    unsigned long testOffset;

    datum pattern     = (datum) 0xAAAAAAAA;
    datum antipattern = (datum) 0x55555555;


    /*
     * Write the default pattern at each of the power-of-two offsets.
     */
    for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
    {
        baseAddress[offset] = pattern;
    }

    /* 
     * Check for address bits stuck high.
     */
    testOffset = 0;
    baseAddress[testOffset] = antipattern;

    for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
    {
        if (baseAddress[offset] != pattern)
        {
            return ((datum *) &baseAddress[offset]);
        }
    }

    baseAddress[testOffset] = pattern;

    /*
     * Check for address bits stuck low or shorted.
     */
    for (testOffset = 1; (testOffset & addressMask) != 0; testOffset <<= 1)
    {
        baseAddress[testOffset] = antipattern;

		if (baseAddress[0] != pattern)
		{
			return ((datum *) &baseAddress[testOffset]);
		}

        for (offset = 1; (offset & addressMask) != 0; offset <<= 1)
        {
            if ((baseAddress[offset] != pattern) && (offset != testOffset))
            {
                return ((datum *) &baseAddress[testOffset]);
            }
        }

        baseAddress[testOffset] = pattern;
    }

    return (NULL);

}   /* memTestAddressBus() */


/**********************************************************************
 *
 * Function:    memTestDevice()
 *
 * Description: Test the integrity of a physical memory device by
 *              performing an increment/decrement test over the
 *              entire region.  In the process every storage bit 
 *              in the device is tested as a zero and a one.  The
 *              base address and the size of the region are
 *              selected by the caller.
 *
 * Notes:       
 *
 * Returns:     NULL if the test succeeds.
 *
 *              A non-zero result is the first address at which an
 *              incorrect value was read back.  By examining the
 *              contents of memory, it may be possible to gather
 *              additional information about the problem.
 *
 **********************************************************************/
#define Memtest_print_step(a,b) if((a&0x3ffff)==0x3ffff){serial_putc('\r');serial_put_hex(a<<b,32);}
#define AML_DEBUG_ROM
#define CONFIG_SYS_ALT_MEMTEST
#define START_PATTERN 0xffff0000
STATIC_PREFIX
datum * 
memTest_alt(datum * baseAddress, unsigned long nBytes)	
{
    volatile unsigned long	*addr, *start, *end;
	unsigned long	val;
	unsigned long	readback;
	unsigned long	errs = 0;
	int iterations = 0;
	int iteration_limit = 1;

	unsigned long	len;
	unsigned long	offset;
	unsigned long	test_offset;
	unsigned long	pattern;
	unsigned long	temp;
	unsigned long	anti_pattern;
	unsigned long	num_words;
	unsigned long   dummy_temp;
	volatile unsigned long *dummy = (volatile unsigned long*)&dummy_temp;
	int	j;

	static const unsigned long bitpattern[] = {
		0x00000001,	/* single bit */
		0x00000003,	/* two adjacent bits */
		0x00000007,	/* three adjacent bits */
		0x0000000F,	/* four adjacent bits */
		0x00000005,	/* two non-adjacent bits */
		0x00000015,	/* three non-adjacent bits */
		0x00000055,	/* four non-adjacent bits */
		0xaaaaaaaa,	/* alternating 1/0 */
	};

	start = (unsigned long *)((unsigned long)baseAddress);
	end = (unsigned long *)((unsigned long)baseAddress+nBytes-1);
	pattern = 0;

	serial_puts ("\nMemory test from ");
	serial_put_hex(start,32);
	serial_puts (" to ");
	serial_put_hex(end,32);
	serial_puts ("\n");

	for (;;) {
		if (iteration_limit && iterations >= iteration_limit) {
			serial_puts("Tested ");serial_put_hex(iterations-1,32);serial_puts(" iteration(s) with ");serial_put_hex(errs,32);serial_puts(" errors.\n");
			return errs != 0;
		}

		serial_puts("Iteration: ");serial_put_hex(iterations,32);serial_puts("\n");
		iterations++;

		/*
		 * Data line test: write a pattern to the first
		 * location, write the 1's complement to a 'parking'
		 * address (changes the state of the data bus so a
		 * floating bus doen't give a false OK), and then
		 * read the value back. Note that we read it back
		 * into a variable because the next time we read it,
		 * it might be right (been there, tough to explain to
		 * the quality guys why it prints a failure when the
		 * "is" and "should be" are obviously the same in the
		 * error message).
		 *
		 * Rather than exhaustively testing, we test some
		 * patterns by shifting '1' bits through a field of
		 * '0's and '0' bits through a field of '1's (i.e.
		 * pattern and ~pattern).
		 */
		addr = start;
		for (j = 0; j < sizeof(bitpattern)/sizeof(bitpattern[0]); j++) {
		    Memtest_print_step(j,2);
		    val = bitpattern[j];
		    for(; val != 0; val <<= 1) {
    			*addr  = val;
    			*dummy  = ~val; /* clear the test data off of the bus */
    			readback = *addr;
    			if(readback != val) {
    			    serial_puts("FAILURE (data line): ");
    			    serial_puts("expected ");
    			    serial_put_hex(val,32);
    			    serial_puts(", actual ");
    			    serial_put_hex(readback,32);
    			    serial_puts("\n");
    			    errs++;
    			}
    			*addr  = ~val;
    			*dummy  = val;
    			readback = *addr;
    			if(readback != ~val) {
    			    serial_puts("FAILURE (data line): ");
    			    serial_puts("expected ");
    			    serial_put_hex(~val,32);
    			    serial_puts(", actual ");
    			    serial_put_hex(readback,32);
    			    serial_puts("\n");
    			    errs++;
    			}
		    }
		}

		/*
		 * Based on code whose Original Author and Copyright
		 * information follows: Copyright (c) 1998 by Michael
		 * Barr. This software is placed into the public
		 * domain and may be used for any purpose. However,
		 * this notice must not be changed or removed and no
		 * warranty is either expressed or implied by its
		 * publication or distribution.
		 */

		/*
		 * Address line test
		 *
		 * Description: Test the address bus wiring in a
		 *              memory region by performing a walking
		 *              1's test on the relevant bits of the
		 *              address and checking for aliasing.
		 *              This test will find single-bit
		 *              address failures such as stuck -high,
		 *              stuck-low, and shorted pins. The base
		 *              address and size of the region are
		 *              selected by the caller.
		 *
		 * Notes:	For best results, the selected base
		 *              address should have enough LSB 0's to
		 *              guarantee single address bit changes.
		 *              For example, to test a 64-Kbyte
		 *              region, select a base address on a
		 *              64-Kbyte boundary. Also, select the
		 *              region size as a power-of-two if at
		 *              all possible.
		 *
		 * Returns:     0 if the test succeeds, 1 if the test fails.
		 */
		len = ((unsigned long)end - (unsigned long)start)/sizeof(volatile unsigned long);
		pattern = (volatile unsigned long) 0xaaaaaaaa;
		anti_pattern = (volatile unsigned long) 0x55555555;

		/*
		 * Write the default pattern at each of the
		 * power-of-two offsets.
		 */
		for (offset = 1; offset < len; offset <<= 1) {
		    
			start[offset] = pattern;
		}

		/*
		 * Check for address bits stuck high.
		 */
		test_offset = 0;
		start[test_offset] = anti_pattern;

		for (offset = 1; offset < len; offset <<= 1) {
		    
		    temp = start[offset];
		    if (temp != pattern) {
			serial_puts("\nFAILURE: Address bit stuck high @ 0x");
			serial_put_hex((unsigned long)&start[offset],32);
			serial_puts(": expected 0x");
			serial_put_hex(pattern,32);
			serial_puts(", actual 0x");
			serial_put_hex(temp,32);
			errs++;
		    }
		}
		start[test_offset] = pattern;

		/*
		 * Check for addr bits stuck low or shorted.
		 */
		for (test_offset = 1; test_offset < len; test_offset <<= 1) {
		    start[test_offset] = anti_pattern;
            
		    for (offset = 1; offset < len; offset <<= 1) {
			temp = start[offset];
			if ((temp != pattern) && (offset != test_offset)) {
			    serial_puts("\nFAILURE: Address bit stuck low or shorted @ 0x");
			    serial_put_hex((unsigned long)&start[offset],32);
			    serial_puts(": expected 0x");
			    serial_put_hex(pattern,32);
			    serial_puts(", actual 0x");
			    serial_put_hex(temp,32);
			    errs++;
			}
		    }
		    start[test_offset] = pattern;
		}

		/*
		 * Description: Test the integrity of a physical
		 *		memory device by performing an
		 *		increment/decrement test over the
		 *		entire region. In the process every
		 *		storage bit in the device is tested
		 *		as a zero and a one. The base address
		 *		and the size of the region are
		 *		selected by the caller.
		 *
		 * Returns:     0 if the test succeeds, 1 if the test fails.
		 */
		num_words = ((unsigned long)end - (unsigned long)start)/sizeof(volatile unsigned long) + 1;

		/*
		 * Fill memory with a known pattern.
		 */
		for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) {
			start[offset] = pattern;
		}

		/*
		 * Check each location and invert it for the second pass.
		 */
		for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) {
		    temp = start[offset];
		    Memtest_print_step(offset,2);
    		    if (temp != pattern) {
    			serial_puts("\nFAILURE (read/write) @ 0x");
    			serial_put_hex((unsigned long)&start[offset],32);
    			serial_puts(": expected 0x");
    			serial_put_hex(pattern,32);
    			serial_puts(", actual 0x");
    			serial_put_hex(temp,32);
    			errs++;
		    }

		    anti_pattern = ~pattern;
		    start[offset] = anti_pattern;
		}

		/*
		 * Check each location for the inverted pattern and zero it.
		 */
		for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) {
		    anti_pattern = ~pattern;
		    Memtest_print_step(offset,2);
		    temp = start[offset];
		    if (temp != anti_pattern) {
    			serial_puts("\nFAILURE (read/write) @ 0x");
    			serial_put_hex((unsigned long)&start[offset],32);
    			serial_puts(": expected 0x");
    			serial_put_hex(anti_pattern,32);
    			serial_puts(", actual 0x");
    			serial_put_hex(temp,32);
    			errs++;
		    }
		    start[offset] = 0;
		}
	}
    return (NULL);
}

STATIC_PREFIX
datum * 
memTestDevice(volatile datum * baseAddress, unsigned long nBytes)	
{
    unsigned long offset;
    unsigned long nWords = nBytes / sizeof(datum);
    unsigned long mask=nWords-1;
    unsigned off;
    int i;
    datum pattern;
    datum antipattern;
    serial_puts("Total Size");serial_put_dword(nBytes);
    srand(0xff);
    serial_put_dword(rand());
    serial_put_dword(rand());
//    serial_put_dword(random());
    serial_put_dword(mask);
    
    
    
    /*
     * Fill memory with a known pattern.
     */
    for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
    {
        baseAddress[offset] = pattern;
#ifdef ENABLE_OUTPUT
        if((offset&0x3ffff)==0x3ffff)
        {
        	serial_putc('\r');serial_put_hex((offset+1)<<2,32);
        }
#endif

    }
    serial_putc('\n');

    /*
     * Check each location and invert it for the second pass.
     */
    for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
    {
        if (baseAddress[offset] != pattern)
        {
            RETURN_PRINT ((datum *) &baseAddress[offset]);
        }

        antipattern = ~pattern;
        baseAddress[offset] = antipattern;
#ifdef ENABLE_OUTPUT
        if((offset&0x3ffff)==0x3ffff)
        {
        	serial_putc('\r');serial_put_hex((offset+1)<<2,32);
        }
#endif

    }
    serial_putc('\n');
    
    /*
     * Check each location for the inverted pattern and zero it.
     */
    for (pattern = 1, offset = 0; offset < nWords; pattern++, offset++)
    {
        antipattern = ~pattern;
        if (baseAddress[offset] != antipattern)
        {
            RETURN_PRINT ((datum *) &baseAddress[offset]);
        }
#ifdef ENABLE_OUTPUT
        if((offset&0x3ffff)==0x3ffff)
        {
        	serial_putc('\r');serial_put_hex((offset+1)<<2,32);
        }
#endif

    }
    serial_putc('\n');
        
    /*
     * Check each location for the inverted pattern and zero it.
     */
    for (offset = 0; offset < nWords;  offset++)
    {
        off=rand()&mask;
//        serial_put_dword(off);
#ifdef ENABLE_OUTPUT
        if((offset&0x3ffff)==0x3ffff)
        {
        	serial_putc('\r');serial_put_hex((offset+1)<<2,32);
        }
#endif
        antipattern = ~(off+1);
        if(offset&1)
        {
            baseAddress[off]=antipattern;
            continue;
        }
        if (baseAddress[off] != antipattern)
        {
            RETURN_PRINT ((datum *) &baseAddress[off]);
        }


    }


    serial_putc('\n');
    RETURN_PRINT (NULL);

}   /* memTestDevice() */

#if 0
datum * 
memTestDevice(datum * baseAddress, unsigned long nBytes)	
{
#if defined(CONFIG_SYS_ALT_MEMTEST)
	
#else
#if 1
	volatile unsigned long	*addr, *start, *end;
	unsigned long	val;
	unsigned long	readback;
	unsigned long	errs = 0;
	int iterations = 1;
	int iteration_limit;
	unsigned long	incr;
	unsigned long	pattern;

	start = (unsigned long *)0x82000000; //((unsigned long)baseAddress);
	end = (unsigned long *)0x82100000; //((unsigned long)baseAddress+nBytes);
	pattern = 0;
	iteration_limit = 256;

	serial_puts ("\nMemory test from ");
	serial_put_hex(start,32);
	serial_puts (" to ");
	serial_put_hex(end,32);
	serial_puts ("\n");

	incr = 1;
	for (;;) {
		if (iteration_limit && iterations > iteration_limit) {
			serial_puts("Tested ");serial_put_hex(iterations-1,32);serial_puts(" iteration(s) with ");serial_put_hex(errs,32);serial_puts(" errors.\n");
			return errs != 0;
		}
		++iterations;

		serial_puts ("\rPattern ");
		serial_put_hex(pattern,32);
		serial_puts (" Writing...");

		for (addr=start,val=pattern; addr<end; addr++) {
			*addr = val;
			val  += incr;
		}
        serial_puts ("\b\b\b\b\b\b\b\b\b\b");
		serial_puts ("Reading...");

		for (addr=start,val=pattern; addr<end; addr++) {
			readback = *addr;
			if (readback != val) {
				serial_puts ("\nMem error @ 0x");serial_put_hex((unsigned int)addr,32);serial_puts (": found ");serial_put_hex(readback,32);serial_puts(", expected ");serial_put_hex(readback,val);serial_puts ("\n");
				errs++;
			}
			val += incr;
		}

		/*
		 * Flip the pattern each time to make lots of zeros and
		 * then, the next time, lots of ones.  We decrement
		 * the "negative" patterns and increment the "positive"
		 * patterns to preserve this feature.
		 */
		if(pattern & 0x80000000) {
			pattern = -pattern;	/* complement & increment */
		}
		else {
			pattern = ~pattern;
		}
		incr = -incr;
	}
#else
    unsigned long offset;
    unsigned long nWords = nBytes / sizeof(datum);
    int i, j;
    datum pattern, result;
    datum antipattern;
    
    serial_puts("\nMemory Test Total Size ");serial_put_dword(nBytes);

    for (j=0;j<16;j++)
    {
        serial_puts(" test ");serial_put_hex(j,8);serial_puts(":\n");
        
    /*
     * Fill memory with a known pattern.
     */
    for (pattern = START_PATTERN, offset = 0; offset < nWords; pattern++, offset++)
    {
        baseAddress[offset] = pattern;
    }
    serial_puts("  Stage 1 finish\n");

    /*
     * Check each location and invert it for the second pass.
     */
    for (pattern = START_PATTERN, offset = 0; offset < nWords; pattern++, offset++)
    {
        result = baseAddress[offset];
        if (result != pattern)
        {
            serial_puts("\n****************************************");
            serial_puts("\n* ");serial_put_hex(pattern,32);serial_puts(" != ");serial_put_hex(result,32);serial_puts(" addr= ");serial_put_hex(baseAddress+(offset<<2),32);
            serial_puts("\n****************************************\n");
            //printf("\n****************************************");
            //printf("\n* %08x!=%08x; address=%08x *", pattern, result, baseAddress+(offset<<2));
            //printf("\n****************************************\n");
            while(1){;}
            return ((datum *) (baseAddress+(offset<<2)));
        }

        antipattern = ~pattern;
        baseAddress[offset] = antipattern;
    }
    serial_puts("  Stage 2 finish\n");
    
    /*
     * Check each location for the inverted pattern and zero it.
     */
    for (pattern = START_PATTERN, offset = 0; offset < nWords; pattern++, offset++)
    {
        antipattern = ~pattern;
        result = baseAddress[offset];
        if (result != antipattern)
        {
            serial_puts("\n****************************************");
            serial_puts("\n* ");serial_put_hex(antipattern,32);serial_puts(" != ");serial_put_hex(result,32);serial_puts(" addr= ");serial_put_hex(baseAddress+(offset<<2),32);
            serial_puts("\n****************************************\n");
            //printf("\n****************************************");
            //printf("\n* %08x!=%08x; address=%08x *", antipattern, result, baseAddress+(offset<<2));
            //printf("\n****************************************\n");
            while(1){;}
            return ((datum *) (baseAddress+(offset<<2)));
        }
    }
    serial_puts("  Stage 3 finish\n");
    }
#endif
#endif
    return (NULL);
}   /* memTestDevice() */
#endif
//#endif
