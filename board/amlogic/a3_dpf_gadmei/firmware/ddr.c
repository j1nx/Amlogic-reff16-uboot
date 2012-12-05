#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <config.h>
#include <asm/arch/firm/io.h>
#include <asm/arch/firm/config.h>
#include <memtest.h>
static void __udelay(unsigned long usec);


STATIC_PREFIX datum   memTestDataBus(volatile datum * address);
STATIC_PREFIX datum * memTestAddressBus(volatile datum * baseAddress, unsigned long nBytes);
STATIC_PREFIX datum * memTestDevice(volatile datum * baseAddress, unsigned long nBytes);

#ifndef CONFIG_AML_UBOOT_MAGIC
#define CONFIG_AML_UBOOT_MAGIC 0x12345678
#endif

static unsigned ddr_start_again=0;
#define APB_Wr(addr, data) WRITE_APB_REG(addr,data)
#define APB_Rd(addr) READ_APB_REG(addr)
#include "ddr/util.c"
#include "ddr/level_1.c"
#include "ddr/ddr3.c"
#include "ddr/ddr2.c"
#define CONFIG_ENABLE_LEVEL2 1
#if CONFIG_ENABLE_LEVEL2
#include "ddr/level_2.c"
#endif









STATIC_PREFIX unsigned ddr_init (struct ddr_set * ddr_setting)
{
	
	
	int i,j,k;
	unsigned sl;
    unsigned temp;
    unsigned ret=0;
    unsigned base=TIMERE_GET();
    static ddr_adjust_t g_reg={
        .rslr=9,
        .rdgr=0x5,
        .dqstr=0x1b,
        .dqsntr=0x1b,
        .dqtr={-1,-1,-1,-1}
    };
    if(ddr_setting->init_pctl(ddr_setting))
	{
	   return 1;
	}
	unsigned iocr=readl(P_PCTL_IOCR_ADDR)>>8;
    /** find out system latency **/
    for (k = 0; k < 8; k++) 
    {
        if((iocr&(1<<k))){
            sl=dwc_find_system_latency(ddr_setting,&g_reg,k);
            if(sl>=DDR_MAX_WIN_LEN)
            {
                printf("CAN NOT find out window for lane %d\n",k);
                BUG();
            }
            else{
                dwc_set_sl(&g_reg,k,sl,0);
            }
        }
	}
	debug_print("\nTR\n");
    /** findout tr value **/
    for (k = 0; k < 8; k++) 
    {
        if((iocr&(1<<k))){
            sl=dwc_find_system_latency(ddr_setting,&g_reg,k);
            if(sl>=DDR_MAX_WIN_LEN)
            {
                printf("CAN NOT find out window for lane %d\n",k);
                BUG();
            }
            else{
                dwc_set_sl(&g_reg,k,sl,0);
            }
        }
	}
	debug_print("\nPhase\n");
	/** findout phase value **/
    for (k = 0; k < 8; k++) 
    {
        if((iocr&(1<<k))){
            if(ddr_phase_sel(ddr_setting,&g_reg,k)<0){
             
                BUG();
                return -1;
            }
        }
	}
	debug_print("\nDQSDLY\n");
	if(ddr_dqsdly_sel(ddr_setting,&g_reg)<0)
	{
	    BUG();
	    return -1;
	}
	debug_print("\nSystem latency\n");
    /** find out system latency **/
    for (k = 0; k < 8; k++) 
    {
        if((iocr&(1<<k))){
            sl=dwc_find(ddr_setting,&g_reg,k,0,-1);
            if(sl>=DDR_MAX_WIN_LEN)
            {
                printf("CAN NOT find out window for lane %d\n",k);
                BUG();
            }
            else{
                dwc_set_sl(&g_reg,k,sl,0);
            }
        }
	}
    debug_print("\nAfter Trainning\n");
    write_regs(&g_reg);
    print_registers();
    
    write_regs(&g_reg);
    if(ddr_setting->init_pctl(ddr_setting))
	{
	   return 1;
	}
	write_regs(&g_reg);
    if(dwc_pctl_stat(PCTL_STAT_Access)<0)//timeout
        return -1;
    /** init dmc **/
    APB_Wr(MMC_DDR_CTRL, ddr_setting->ddr_ctrl);
    APB_Wr(MMC_REQ_CTRL,0xff);
    printf("\nDDR init time %d\n",TIMERE_GET()-base);
    
    return 0;

}
STATIC_PREFIX void ddr_pll_init(struct ddr_set * ddr_setting) ;
static volatile unsigned * sw_train=(volatile unsigned *)0x4901fff0;
void set_ddr_clock(struct ddr_set * ddr_setting)
{
 int i;
    APB_Wr(PCTL_DLLCR_ADDR, APB_Rd(PCTL_DLLCR_ADDR)|(7<<2));
    writel(ddr_setting->ddr_pll_cntl|0x8000, P_HHI_DDR_PLL_CNTL);
    writel(0x65e31ff, P_HHI_DDR_PLL_CNTL2);
    writel(0x1649a941, P_HHI_DDR_PLL_CNTL3);
    writel(ddr_setting->ddr_pll_cntl, P_HHI_DDR_PLL_CNTL);
    for (i=0;i<1000;i++){
		readl(P_HHI_DDR_PLL_CNTL3);
    }
}

static inline unsigned lowlevel_ddr(void)
{
    struct ddr_set * ddr=&__ddr2_nanya_1Gbit;;
    int ret;
    if(POR_GET_DDR(romboot_info->por_cfg)==POR_DDR3)
    {
        
        serial_puts("Init DDR3\n");
        ddr=&__ddr3_nanya_1Gbit;
    }else{
        debug_print("Init DDR2\n");
    }
//    __builtin_arc_sleep(0);
    set_ddr_clock(ddr);
    if(1)
    {        
            while(ddr_init(ddr))ddr_start_again=0;
        
        
    }
    print_registers();
    
    return 0;
}
static inline unsigned lowlevel_mem_test_device(void)
{
    return (unsigned)memTestDevice((volatile datum *)PHYS_MEMORY_START,PHYS_MEMORY_SIZE);
}
static inline unsigned lowlevel_mem_test_data(void)
{
    return (unsigned)memTestDataBus((volatile datum *) PHYS_MEMORY_START);
}
static inline unsigned lowlevel_mem_test_addr(void)
{
    return (unsigned)memTestAddressBus((volatile datum *)PHYS_MEMORY_START,
				    PHYS_MEMORY_SIZE);
}
static unsigned ( * mem_test[])(void)={
    lowlevel_ddr,
    lowlevel_mem_test_addr,
    lowlevel_mem_test_data,
#ifdef CONFIG_ENABLE_MEM_DEVICE_TEST
    lowlevel_mem_test_device
#endif
};
#define MEM_DEVICE_TEST_ITEMS (sizeof(mem_test)/sizeof(mem_test[0]))
#ifdef CONFIG_ENABLE_MEM_DEVICE_TEST
#define MEM_DEVICE_TEST_ITEMS_BASE (MEM_DEVICE_TEST_ITEMS -1)
#else
#define MEM_DEVICE_TEST_ITEMS_BASE (MEM_DEVICE_TEST_ITEMS -0)
#endif
STATIC_PREFIX unsigned ddr_init_test(void)
{
    int i,j;
    unsigned por_cfg=1;
    writel((0<<22)+0x186a0,P_WATCHDOG_TC);
    setbits_le32(0xc1100000+0x1111*4,1<<6);
    por_cfg=0;
#ifdef CONFIG_ENABLE_MEM_DEVICE_TEST    
    for(i=0;i<MEM_DEVICE_TEST_ITEMS_BASE+1&&por_cfg==0;i++)
#else    
    for(i=0;i<MEM_DEVICE_TEST_ITEMS_BASE&&por_cfg==0;i++)
#endif    
	{
	        writel(0,P_WATCHDOG_RESET);
	        por_cfg=mem_test[i]();
	        debug_print("\nStage %d",i);
	        debug_print(" Result %08x",por_cfg);
	}
	i=0;
	#if 0
	por_cfg=0;
    while(por_cfg==0)
    {
        debug_print("%02x\n",i++);
        por_cfg=lowlevel_mem_test_device();
    }
    #endif
	writel(0,P_WATCHDOG_TC);
//	ddr_start_again=por_cfg?1:ddr_start_again;
	return por_cfg;
}

