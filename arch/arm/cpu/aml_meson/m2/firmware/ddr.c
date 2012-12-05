
#include <asm/arch/firm/io.h>
#include <asm/arch/firm/uart.h>
#include <asm/arch/firm/timing.h>
#include <config.h>
#include <memtest.h>
#define DDR_RSLR_LEN 6
#define DDR_RDGR_LEN 4
#define DDR_MAX_WIN_LEN (DDR_RSLR_LEN*DDR_RDGR_LEN)
#define PHYS_MEMORY_START 0x80000000

#define DDR_RANK  1   // 2'b11 means 2 rank.
#define DDR_LANE  2

//#define ENABLE_POWER_SAVING 
#define RESET_MMC_FOR_DTU_TEST

#define NOP_CMD  0
#define PREA_CMD  1
#define REF_CMD  2
#define MRS_CMD  3
#define ZQ_SHORT_CMD 4
#define ZQ_LONG_CMD  5
#define SFT_RESET_CMD 6
 

//static unsigned ddr_start_again=1;

#define APB_Wr(addr, data) WRITE_APB_REG(addr,data)
#define APB_Rd(addr) READ_APB_REG(addr)
#define Wr           WRITE_CBUS_REG
#define Rd           READ_CBUS_REG

#include <ddr_init_sw.c>
#include <ddr_init_hw.c>
#include <ddr_init_pctl.c>

static void wait_pll(unsigned clk,unsigned dest);
static void set_ddr_clock(struct ddr_set * timing_reg)
{
 int i;
#ifdef ENABLE_POWER_SAVING
    APB_Wr(PCTL_DLLCR_ADDR, APB_Rd(PCTL_DLLCR_ADDR)|(7<<2));
#endif


#ifdef CONFIG_AML_DDR_PLL_FINE_TUNE
	if(readl(P_PREG_STICKY_REG0) == 0xa2011){
		writel(readl(P_PREG_STICKY_REG1)|0x8000,P_HHI_DDR_PLL_CNTL);			
	}
	else
#endif
	Wr(HHI_DDR_PLL_CNTL, timing_reg->ddr_pll_cntl |0x8000);
    Wr(HHI_DDR_PLL_CNTL2,0x65e31ff);
    Wr(HHI_DDR_PLL_CNTL3,0x1649a941);

#ifdef CONFIG_AML_DDR_PLL_FINE_TUNE
	if(readl(P_PREG_STICKY_REG0) == 0xa2011){
		writel(readl(P_PREG_STICKY_REG1),P_HHI_DDR_PLL_CNTL);
		writel(0,P_PREG_STICKY_REG0);
		writel(0,P_PREG_STICKY_REG1);
	}
	else
#endif
	Wr(HHI_DDR_PLL_CNTL, timing_reg->ddr_pll_cntl);
	while(!(readl(P_HHI_DDR_PLL_CNTL3)&(1<<31)));
#if 0    
    wait_pll(3,timing_reg->ddr_clk);
    wait_pll(30,timing_reg->ddr_clk);
#endif    
    Wr(RESET1_REGISTER,1<<3);
    __udelay(1000);
//    for ( i = 0; i <= 1000; i ++) {          
//       APB_Wr(MMC_DDR_CTRL, timing_reg->ddr_ctrl );   // 
//   }
}




static inline unsigned lowlevel_ddr(unsigned tag,struct ddr_set * timing_reg)
{
    set_ddr_clock(timing_reg);
#if 0    
    if(tag)
        return ddr_init_sw(timing_reg);
#endif        
    return ddr_init_hw(timing_reg);
}
static inline unsigned lowlevel_mem_test_device(unsigned tag,struct ddr_set * timing_reg)
{
    return tag&&(unsigned)memTestDevice((volatile datum *)PHYS_MEMORY_START,PHYS_MEMORY_SIZE);
}

static inline unsigned lowlevel_mem_test_data(unsigned tag,struct ddr_set * timing_reg)
{
    return tag&&(unsigned)memTestDataBus((volatile datum *) PHYS_MEMORY_START);
}

static inline unsigned lowlevel_mem_test_addr(unsigned tag,struct ddr_set * timing_reg)
{
    return tag&&(unsigned)memTestAddressBus((volatile datum *)PHYS_MEMORY_START, PHYS_MEMORY_SIZE);
}

static unsigned ( * mem_test[])(unsigned tag,struct ddr_set * timing_reg)={
    lowlevel_ddr,
    lowlevel_mem_test_addr,
    lowlevel_mem_test_data,
#ifdef CONFIG_ENABLE_MEM_DEVICE_TEST
    lowlevel_mem_test_device
#endif
};

#define MEM_DEVICE_TEST_ITEMS_BASE (sizeof(mem_test)/sizeof(mem_test[0]))

STATIC_PREFIX unsigned long    clk_util_clk_msr(unsigned long   clk_mux);

static unsigned ddr_init_test(int arg)
{
    int i,j;
    unsigned por_cfg=1;
    serial_putc('\n');
    por_cfg=0;
    for(i=0;i<MEM_DEVICE_TEST_ITEMS_BASE&&por_cfg==0;i++)
	{
        writel(0,P_WATCHDOG_RESET);
        por_cfg=mem_test[i](arg&(1<<i),&__ddr_setting);
        serial_puts("\nStage ");
        serial_put_hex(i,8);
        serial_puts(" Result ");
        serial_put_hex(por_cfg,32);
	}
	writel(0,P_WATCHDOG_TC);
	ddr_start_again=por_cfg?1:ddr_start_again;
	return por_cfg;
}
#if 0
void do_exception(unsigned reason,unsigned lr)
{
    serial_puts("Enter Exception:");
    serial_put_dword(reason);
    serial_put_dword(lr);
//    serial_put_dword(testmem(0));
    while(1);
//    writel((1<<22)|1000000,P_WATCHDOG_TC);//enable Watchdog
}
#endif
