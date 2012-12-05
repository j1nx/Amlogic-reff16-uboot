
#include <config.h>
#include <version.h>
#include <common.h>
#include <command.h>

#include <asm/io.h>

#include <asm/cache-l2x0.h>

//if it use this test function, you need define CONFIG_l2_CACHE_TEST  1 at config file
#ifdef CONFIG_l2_CACHE_TEST
/*L2-cache operate for test function*/


static void cache_cache_wait(unsigned reg, unsigned long mask)
{
	/* wait for the operation to complete */
	while (readl(reg) & mask)
		;
}

void cache_cache_sync(void)
{
	writel(0, L2X0_CACHE_SYNC);
	cache_cache_wait(L2X0_CACHE_SYNC, 1);
}

void cache_l2x0_inv_all(void)
{
    
    /* invalidate all ways */
	writel(0xff, L2X0_INV_WAY);
	cache_cache_wait(L2X0_INV_WAY, 0xff);
	cache_cache_sync();
}


void cache_l2x0_clean_all (void)
{
   
	/* invalidate all ways */
	writel(0xff, L2X0_CLEAN_WAY);
	cache_cache_wait(L2X0_CLEAN_WAY, 0xff);
	cache_cache_sync();
}

int cache_l2x0_status(void)
{
    return readl( L2X0_CTRL) & 1;
}
void cache_l2x0_enable(void)
{
	__u32 aux;

	/*
	 * Check if l2x0 controller is already enabled.
	 * If you are booting from non-secure mode
	 * accessing the below registers will fault.
	 */
	if (!(readl( L2X0_CTRL) & 1)) {

		/* l2x0 controller is disabled */

		aux = readl(L2X0_AUX_CTRL);
		aux &= 0xff800fff;
		aux |= 0x00020000;
		writel(aux,L2X0_AUX_CTRL);

		cache_l2x0_inv_all();

		/* enable L2X0 */
		writel(1,  L2X0_CTRL);
	}

}
void cache_l2x0_disable(void)
{
    writel(0,  L2X0_CTRL);
}



extern int _get_cache_type(void);
extern int _get_cache_level(void);
extern int _get_cache_size_id(int);
extern void _l2_cache_lockdown(void);

//extern int DataUnifiedCache_test(void);
int DataUnifiedCache_test(int fill_value,int modify_value);



/*m1 cache type: 0x83338003*/
int m1_cache_type;

/*m1 cache level: 0x9200003: LoUU,LoC,LoUIS, Ctype1*/
int m1_cache_level;

/*
m1_cache_size_ID:
m1 cache size identification: 0x701fe019:
WT: not support
WB: support
RA: support
WA: support
NumSets::252
Associativity: 516
LineSize: 3 == Number of words is 8 == 32 byte
*/
int m1_cache_size_ID;

int m1_cache_size_select;

int result_test_cache;
int L2_cache_pattern[]={
    0xaaaaaaaa,
    0xdd22ee11, 0x7788bb44, 0x337755aa, 0xff00aa55,
    0xff000000, 0x000000ff, 0x00ffffff, 0xffffff00,
    0x01000200, 0x04000800, 0x10002000, 0x40008000,
    0xfefffbff, 0xf7fffeff, 0xefffbfff, 0x7fffdfff,
    0x00100020, 0x00400080, 0x00100020, 0x00400800,
    0xfffefffb, 0xfff7fffe, 0xffefffbf, 0xff7fffdf,
    0x55aa00ff, 0xaa0055ff, 0x55ffaa00, 0xff00aa55,
    0xdd22ee11, 0x7788bb44, 0xdd22ee11, 0x7788bb44,

    0x01fe00ff, 0x01fe00ff, 0x01fe00ff, 0x01fe00ff,
    0x02fc00ff, 0x02fc00ff, 0x02fc00ff, 0x02fc00ff,
    0x04fb00ff, 0x04fb00ff, 0x04fb00ff, 0x04fb00ff,
    0x08f700ff, 0x08f700ff, 0x08f700ff, 0x08f700ff,
    0x10ef00ff, 0x10ef00ff, 0x10ef00ff, 0x10ef00ff,
    0x20df00ff, 0x20df00ff, 0x20df00ff, 0x20df00ff,
    0x40bf00ff, 0x40bf00ff, 0x40bf00ff, 0x40bf00ff,
    0x807f00ff, 0x807f00ff, 0x807f00ff, 0x807f00ff,

    0xfe01ff00, 0xfe01ff00, 0xfe01ff00, 0xfe01ff00,
    0xfd02ff00, 0xfd02ff00, 0xfd02ff00, 0xfd02ff00,
    0xfb04ff00, 0xfb04ff00, 0xfb04ff00, 0xfb04ff00,
    0xf708ff00, 0xf708ff00, 0xf708ff00, 0xf708ff00,
    0xef10ff00, 0xef10ff00, 0xef10ff00, 0xef10ff00,
    0xdf20ff00, 0xdf20ff00, 0xdf20ff00, 0xdf20ff00,
    0xbf40ff00, 0xbf40ff00, 0xbf40ff00, 0xbf40ff00,
    0x7f80ff00, 0x7f80ff00, 0x7f80ff00, 0x7f80ff00
    };

#define PATTERN_NUMBER  sizeof(L2_cache_pattern)/sizeof(L2_cache_pattern[0])

void test_cache_main(int cacheable_addr, int nocacheable_addr)
{
    int i,j;
    printf("start test L2-cache.....\n");
    m1_cache_type = _get_cache_type();
    m1_cache_level = _get_cache_level();
    m1_cache_size_ID = _get_cache_size_id(0);
    for(i=0;i<PATTERN_NUMBER;i++)
    {
        result_test_cache = DataUnifiedCache_test(0x55555555,L2_cache_pattern[i]);
        if(result_test_cache != 0)
        {
            j=1;
            break;
            //while(j);
        }
        result_test_cache = DataUnifiedCache_test(0x55555555,~L2_cache_pattern[i]);
        if(result_test_cache != 0)
        {
            break;
        }
    }
    if(i<PATTERN_NUMBER)
    {
        printf("L2-cache is error\n");
        printf("error pattern:0x%x\n",L2_cache_pattern[i]);
    }
    else
    {
        printf("L2-cache is ok\n");
        printf("test count:%d\n",i);
    }
    printf("L2-cache test end!\n");
    //_l2_cache_lockdown();
}


int do_L2_cache_test(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
    int cacheable_addr,no_cacheable_addr;
    
    if (argc < 2) {
		cmd_usage(cmdtp);
		return 1;
	}
	cacheable_addr = 0;
	no_cacheable_addr = 0;
	if(argc > 2)
	{
	    if(argc<4)
	    {   
	        printf("parameter error!");
	        return 1;
	    }
	    cacheable_addr = simple_strtoul(argv[2], NULL, 16);
	    no_cacheable_addr = simple_strtoul(argv[3], NULL, 16);
	}
	test_cache_main(cacheable_addr, no_cacheable_addr);
}

//below icache pattern is bist test pattern, the sequence can not be changed.
static int L1_icache_pattern_serial[]={
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1        
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0        
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1        
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0        
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1        
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0        
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1        
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0        
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1        
0x6,//ce ((unsigned long *)S:0xC1109910)[0]=0x6        
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7        
0x6,//ce ((unsigned long *)S:0xC1109910)[0]=0x6        
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7        
0x6,//ce ((unsigned long *)S:0xC1109910)[0]=0x6        
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7        
0x6,//ce ((unsigned long *)S:0xC1109910)[0]=0x6        
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7        
0x2,//ce ((unsigned long *)S:0xC1109910)[0]=0x2        
0x3,//ce ((unsigned long *)S:0xC1109910)[0]=0x3        
0x2,//ce ((unsigned long *)S:0xC1109910)[0]=0x2        
0x3,//ce ((unsigned long *)S:0xC1109910)[0]=0x3        
0x6,//ce ((unsigned long *)S:0xC1109910)[0]=0x6        
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7        
0x6,//ce ((unsigned long *)S:0xC1109910)[0]=0x6        
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7        
0x6,//ce ((unsigned long *)S:0xC1109910)[0]=0x6        
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7        
0x4,//ce ((unsigned long *)S:0xC1109910)[0]=0x4        
0x5,//ce ((unsigned long *)S:0xC1109910)[0]=0x5        
0x4,//ce ((unsigned long *)S:0xC1109910)[0]=0x4        
0x5,//ce ((unsigned long *)S:0xC1109910)[0]=0x5        
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0        
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1        
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0        
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1        
0x4,//ce ((unsigned long *)S:0xC1109910)[0]=0x4        
0x5,//ce ((unsigned long *)S:0xC1109910)[0]=0x5        
0x2,//ce ((unsigned long *)S:0xC1109910)[0]=0x2        
0x3,//ce ((unsigned long *)S:0xC1109910)[0]=0x3        
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0        
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1        
};

static int L1_dcache_pattern_serial[]={
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1
0x6,//ce ((unsigned long *)S:0xC1109910)[0]=0x6
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7
0x6,//ce ((unsigned long *)S:0xC1109910)[0]=0x6
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7
0x6,//ce ((unsigned long *)S:0xC1109910)[0]=0x6
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7
0x6,//ce ((unsigned long *)S:0xC1109910)[0]=0x6
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7
0x2,//ce ((unsigned long *)S:0xC1109910)[0]=0x2
0x3,//ce ((unsigned long *)S:0xC1109910)[0]=0x3
0x2,//ce ((unsigned long *)S:0xC1109910)[0]=0x2
0x3,//ce ((unsigned long *)S:0xC1109910)[0]=0x3
0x6,//ce ((unsigned long *)S:0xC1109910)[0]=0x6
0x7,//ce ((unsigned long *)S:0xC1109910)[0]=0x7
0x2,//ce ((unsigned long *)S:0xC1109910)[0]=0x2
0x3,//ce ((unsigned long *)S:0xC1109910)[0]=0x3
0x2,//ce ((unsigned long *)S:0xC1109910)[0]=0x2
0x3,//ce ((unsigned long *)S:0xC1109910)[0]=0x3
0x3,//ce ((unsigned long *)S:0xC1109910)[0]=0x3
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1
0x4,//ce ((unsigned long *)S:0xC1109910)[0]=0x4
0x5,//ce ((unsigned long *)S:0xC1109910)[0]=0x5
0x2,//ce ((unsigned long *)S:0xC1109910)[0]=0x2
0x3,//ce ((unsigned long *)S:0xC1109910)[0]=0x3
0x3,//ce ((unsigned long *)S:0xC1109910)[0]=0x3
0x0,//ce ((unsigned long *)S:0xC1109910)[0]=0x0
0x1,//ce ((unsigned long *)S:0xC1109910)[0]=0x1
};


#define ICACHE_PATTERN_NUMBER   sizeof(L1_icache_pattern_serial)/sizeof(L1_icache_pattern_serial[0])
#define DCACHE_PATTERN_NUMBER   sizeof(L1_dcache_pattern_serial)/sizeof(L1_dcache_pattern_serial[0])


extern int L1_icache_bist_test(int *pattern_addr,int pattern_count);
extern int L1_dcache_bist_test(int *pattern_addr,int pattern_count);

//disable icache and dcache to test L1-icache 
void L1_icache_test(void)
{
    int i;
    static int L1_icache_status=0;
    int ret=0;
    if(L1_icache_status)
    {
        printf("icached tested error, don't test again\n");
        return ;
    }
    printf("start test L1 icache......\n");
    for(i=0;i<100;i++)
    {
        ret |= L1_icache_bist_test(&L1_icache_pattern_serial[0],ICACHE_PATTERN_NUMBER);
        if(ret)
        {
            break;
        }
    }
    L1_icache_status = ret;
    if(ret)
    {
        printf("icache error\n");
    }
    else
    {
        printf("icache ok\n");
    }
    printf("test count:%d\n",i);
    printf("L1 icache test end\n");
}

void L1_dcache_test(void)
{
    int i;
    static int L1_dcache_status=0;
    int ret=0;
    if(L1_dcache_status)
    {
        printf("dcached tested error, don't test again\n");
        return ;
    }    
    printf("start test L1 dcache......\n");
    for(i=0;i<100;i++)
    {
        ret |= L1_dcache_bist_test(&L1_dcache_pattern_serial[0],DCACHE_PATTERN_NUMBER);
        if(ret)
        {
            break;
        }
    }
    L1_dcache_status = ret;
    if(ret)
    {
        printf("dcache error\n");
    }
    else
    {
        printf("dcache ok\n");
    }
    printf("test count:%d\n",i);
    printf("L1 dcache test end\n");
}
int do_L1_icache_test(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{

    if (argc < 2) {
		cmd_usage(cmdtp);
		return 1;
	}
	L1_icache_test();
}



int do_L1_dcache_test(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{

    if (argc < 2) {
		cmd_usage(cmdtp);
		return 1;
	}
	L1_dcache_test();
}


U_BOOT_CMD(L2_cache, CONFIG_SYS_MAXARGS, 1,	do_L2_cache_test,
    "L2-cache test subsystem",
    "L2_cache test cacheable_addr no_cacheable_addr\n"
);




U_BOOT_CMD(L1_icache, CONFIG_SYS_MAXARGS, 1,	do_L1_icache_test,
    "L1-icache test subsystem",
    "L1_icache test cacheable_addr no_cacheable_addr\n"
);

U_BOOT_CMD(L1_dcache, CONFIG_SYS_MAXARGS, 1,	do_L1_dcache_test,
    "L1-dcache test subsystem",
    "L1_dcache test cacheable_addr no_cacheable_addr\n"
);



#if 0
void single_gcc_asm()
{
    asm(
        "mov r0, r0\n\t"
        "mov r0, r0\n\t"
        "mov r0, r0\n\t"
        "mov r0, r0"
    );
}
#endif

#endif  //CONFIG_l2_CACHE_TEST
