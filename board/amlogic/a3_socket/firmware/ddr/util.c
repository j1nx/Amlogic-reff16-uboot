#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <config.h>
#include <asm/arch/firm/io.h>
#include <asm/arch/firm/config.h>
#include <memtest.h>
void printf(const char *fmt, ...);

static void __udelay(unsigned long usec);
#undef debug_print
#define debug_print(fmt,args...) printf (fmt ,##args)

#define BUG(args...)            

//3¡¯b0000 = Init_mem
//3¡¯b0001 = Config
//3¡¯b0010 = Config_req
//3¡¯b0011 = Access
//3¡¯b0100 = Access_req
//3¡¯b0101 = Low_power
//3¡¯b0110 = Low_power_entry_req
//3¡¯b0111 = Low_power_exit_req
//Others   = Reserved
#define PCTL_STAT_Init_mem           	0
#define PCTL_STAT_Config              1
#define PCTL_STAT_Config_req          2
#define PCTL_STAT_Access              3
#define PCTL_STAT_Access_req          4
#define PCTL_STAT_Low_power           5
#define PCTL_STAT_Low_power_entry_req 6
#define PCTL_STAT_Low_power_exit_req  7

//3'b000 = INIT (move to Init_mem from Config)
//3'b001 = CFG (move to Config from Init_mem or Access)
//3'b010 = GO (move to Access from Config)
//3'b011 = SLEEP (move to Low_power from Access)
//3'b100 = WAKEUP (move to Access from Low_power)

#define PCTL_SCTL_INIT                  0
#define PCTL_SCTL_CFG                   1
#define PCTL_SCTL_GO                    2
#define PCTL_SCTL_SLEEP                 3
#define PCTL_SCTL_WAKEUP 			    4
//-1 , error . Others success
static int dwc_pctl_stat(unsigned stat)
{
    unsigned cur_stat=readl(P_PCTL_STAT_ADDR);
    int     i=10000;
    switch(stat)
    {
    case PCTL_STAT_Config_req         :
	case PCTL_STAT_Access_req         :
	case PCTL_STAT_Low_power_entry_req:
	case PCTL_STAT_Low_power_exit_req :
	    return -1;//error
	    //do nothing , these should not be target stat
	}

	while(cur_stat!=stat&&i--)
	{
		switch(cur_stat)
		{
			case PCTL_STAT_Init_mem           :
			    writel(PCTL_SCTL_CFG,P_PCTL_SCTL_ADDR);
			    break;
			case PCTL_STAT_Config             :
			    if(cur_stat>stat)
			    {
			        writel(PCTL_SCTL_INIT,P_PCTL_SCTL_ADDR);
			    }
			    else
			        writel(PCTL_SCTL_GO,P_PCTL_SCTL_ADDR);
			    break;
			case PCTL_STAT_Access             :
			    if(cur_stat>stat)
			        writel(PCTL_SCTL_CFG,P_PCTL_SCTL_ADDR);
			    else
			        writel(PCTL_SCTL_SLEEP,P_PCTL_SCTL_ADDR);
			    break;
			case PCTL_STAT_Low_power          :
			    writel(PCTL_SCTL_WAKEUP,P_PCTL_SCTL_ADDR);
			    break;

		}
		cur_stat=readl(P_PCTL_STAT_ADDR);

	}
	return i;
}
typedef struct {
    unsigned rslr;
    unsigned rdgr;
    unsigned dqstr;
    unsigned dqsntr;
    unsigned dllcr[8];
    unsigned dqtr[8];
}ddr_adjust_t;
static ddr_adjust_t default_ddr={
    .rslr=9,
    .rdgr=0x5,
    .dqstr=0x1b,
    .dqsntr=0x1b,
    .dqtr={-1,-1,-1,-1}
};
static ddr_adjust_t ddr_cal={
    .rslr=9,
    .rdgr=0x5,
    .dqstr=0x1b,
    .dqsntr=0x1b,
    .dqtr={-1,-1,-1,-1}
};
static char * para_name[]={
    "rslr",
    "rdgr",
    "dqstr",
    "dqsntr",
    "dllcr"
};
typedef struct lane_info_s{
    unsigned char rslr;
    unsigned char rdgr;
    unsigned char dqstr;
    unsigned char dqsntr;
    unsigned char dllcr;
    unsigned      dqtr;
}lane_info_t;
static void read_regs(ddr_adjust_t * regs)
{
    int i;
    unsigned max=readl(P_PCTL_IOCR_ADDR)>>8;

    regs->rslr=APB_Rd(PCTL_RSLR0_ADDR);
    regs->rdgr=APB_Rd(PCTL_RDGR0_ADDR);
    regs->dqstr=APB_Rd(PCTL_DQSTR_ADDR);
    regs->dqsntr=APB_Rd(PCTL_DQSNTR_ADDR);
    for(i=0;i<8;i++)
    {
        if(((1<<i)&max)==0)
            continue;

        regs->dllcr[i]=APB_Rd(PCTL_DLLCR0_ADDR+(i<<2));
        regs->dqtr[i]=APB_Rd(PCTL_DQTR0_ADDR+(i<<2));
    }
}
static int write_regs(ddr_adjust_t * regs)
{
    unsigned max=readl(P_PCTL_IOCR_ADDR)>>8;
    int i;
    if(dwc_pctl_stat(PCTL_STAT_Config)<0)
        return -1;
    writel(regs->rslr,P_PCTL_RSLR0_ADDR);
    writel(regs->rdgr,P_PCTL_RDGR0_ADDR);
    writel(regs->dqstr,P_PCTL_DQSTR_ADDR);
    writel(regs->dqsntr,P_PCTL_DQSNTR_ADDR);
    for(i=0;i<8;i++)
    {
        if(((1<<i)&max)==0)
            continue;
        writel(regs->dllcr[i],P_PCTL_DLLCR0_ADDR+(i<<2));
        writel(regs->dqtr[i],P_PCTL_DQTR0_ADDR+(i<<2));
    }
    return 0;
}

static void regs2lane(ddr_adjust_t * regs,lane_info_t * lane_info,unsigned lane)
{
    lane_info->rslr=((regs->rslr)>>(lane*3))&7;
    lane_info->rdgr=(regs->rdgr>>(lane*2))&3;
    lane_info->dqstr=(regs->dqstr>>(lane*3))&7;
    lane_info->dqsntr=(regs->dqsntr>>(lane*3))&7;
    lane_info->dllcr=(regs->dllcr[lane]>>14)&0xf;
    lane_info->dqtr=regs->dqtr[lane];

}
static void lane2regs_sl(ddr_adjust_t * regs,lane_info_t * lane_info,unsigned lane)
{
    regs->rslr=(regs->rslr&(~(7<<(lane*3))))|((lane_info->rslr&7)<<(lane*3));
    regs->rdgr=(regs->rdgr&(~(3<<(lane*2))))|((lane_info->rdgr&3)<<(lane*2));
}
static void lane2regs(ddr_adjust_t * regs,lane_info_t * lane_info,unsigned lane)
{
    lane2regs_sl(regs,lane_info,lane);
    regs->dqstr=(regs->dqstr&(~(7<<(lane*3))))|((lane_info->dqstr&7)<<(lane*3));
    regs->dqsntr=(regs->dqsntr&(~(7<<(lane*3))))|((lane_info->dqsntr&7)<<(lane*3));
    regs->dllcr[lane]=(regs->dllcr[lane]&(~(0xf<<14)))|((lane_info->dllcr&0xf)<<14);
    regs->dqtr[lane]=lane_info->dqtr;
}
static void print_lane_info(lane_info_t * lane_i,unsigned lane)
{
    int i;
    lane_info_t *p=lane_i;
    char * lane_char;
    lane_info_t lane_info;
    if(lane_i==NULL)
    {
        ddr_adjust_t reg;
        read_regs(&reg);
        regs2lane(&reg,&lane_info,lane);
        p=&lane_info;
    }
    lane_char=(char*)&(p->rslr);
    debug_print("lane=%d,",lane);
    for(i=0;i<sizeof(para_name)/sizeof(para_name[0]);i++)
    {
        debug_print("%s=0x%x,",para_name[i],lane_char[i]);
    }
    debug_print("dqtr=0x%08x",p->dqtr);
}




static char * zqname[]={
    " output-pulldown",
    " output-pullup",
    " on-die-pulldown",
    " on-die-pullup"
};
static void print_ZQ(void)
{
    int i;
    //debug_print_hex(APB_Rd(PCTL_ZQSR_ADDR),32);
    debug_print(" ");
    for(i=0;i<4;i++)
    {
        debug_print(",%s=%d",zqname[i],(((APB_Rd(PCTL_ZQSR_ADDR)>>(i*5))&0x1f)));
    }
}
static void print_reg(ddr_adjust_t * reg)
{
    unsigned max=readl(P_PCTL_IOCR_ADDR)>>8;
    int i;
    char c;
//    printf("\nRSLR=0x%08x,RDGR=0x%08x,",reg->rslr,reg->rdgr);
    debug_print("\nRSLR=0x%08x,RDGR=0x%08x,",reg->rslr,reg->rdgr);
    debug_print("DQSTR=0x%08x,DQSNTR=0x%08x",reg->dqstr,reg->dqsntr);

    for(i=0,c='\n';i<8;i++)
    {
        if(((1<<i)&max)==0)
            continue;
        debug_print("%cDLLCR[%d]=%08x",c,i,reg->dllcr[i]);
        c=',';
//        debug_print(" DQTR");
//        debug_print_hex(i,4);
//        debug_print("=");
//        debug_print_hex(APB_Rd(PCTL_DQTR0_ADDR+(i<<2)),32);

    }
    for(i=0,c='\n';i<8;i++)
    {
        if(((1<<i)&max)==0)
            continue;
        debug_print("%cDQTR[%d]=%08x",c,i,reg->dqtr[i]);
        c=',';
    }
}
static void print_registers(void)
{
    unsigned i;
    unsigned max=readl(P_PCTL_IOCR_ADDR)>>8;
    ddr_adjust_t reg;
    lane_info_t lane_info;
    read_regs(&reg);
    print_reg(&reg);
    for(i=0;i<8;i++)
    {
        if(((1<<i)&max)==0)
            continue;
        regs2lane(&reg,&lane_info,i);
        debug_print("\n");
        print_lane_info(&lane_info,i);
    }
    debug_print("\n");
}

#define NOP_CMD   0
#define PREA_CMD   1
#define REF_CMD   2
#define MRS_CMD   3
#define ZQ_SHORT_CMD  4
#define ZQ_LONG_CMD   5
#define SFT_RESET_CMD  6

static inline void load_mcmd(unsigned val)
{
	APB_Wr(PCTL_MCMD_ADDR, val);
	
	while ( APB_Rd(PCTL_MCMD_ADDR) & 0x80000000 );
}
#define load_nop()  load_mcmd((1<<31)|(1<<20)|NOP_CMD)
#define load_prea()  	load_mcmd((1<<31)|(1<<20)|PREA_CMD)
#define load_ref()  	load_mcmd((1<<31)|(1<<20)|REF_CMD)

#define load_mrs(a,b)	load_mcmd((1 << 31) | \
								  (8 << 24) | \
								  (1 << 20) | \
							      (a << 17) | \
						          (b << 4)  | \
								   MRS_CMD )
#define load_zqcl(zq) load_mcmd(  (1 << 31) | \
								  (1 << 20) | \
							      (zq << 4) | \
								  ZQ_LONG_CMD )



static int pdm_write_pattern(unsigned  * pattern,unsigned addr)
{

    unsigned row=addr&0x1fff;
    int i;
    unsigned * wd=pattern;
    unsigned t_addr= addr;
    if(dwc_pctl_stat(PCTL_STAT_Config)<0)
        return -1;
    for(i=0;i<4;i++)
        writel(wd[i],P_PCTL_DTUWD0_ADDR+(i<<2));
    APB_Wr(PCTL_DTUWACTL_ADDR,	t_addr);
    APB_Wr(PCTL_DTURACTL_ADDR,	t_addr);
    return 0;
}
static int pdm_run()
{
    int ret=10000;
    if(dwc_pctl_stat(PCTL_STAT_Config)<0)//timeout
        return -1;
    writel(1,P_PCTL_DTUECTL_ADDR);     // start wr/rd
    if(dwc_pctl_stat(PCTL_STAT_Access)<0)//timeout
        return -1;
    while((readl(P_PCTL_DTUECTL_ADDR)&1)&&ret--);
    if(ret<0)//timeout
        return ret;
    ret=readl(P_PCTL_DTUPDES_ADDR);
    if((ret>>8))//rdmissing
        return -1;
    return ret&0xff;//time out
}
#define TEST_COUNT 64
static unsigned run_test_print=0;
static int dtu_run_test(struct ddr_set * ddr_setting,
    ddr_adjust_t * reg,
    unsigned  * pattern,
    unsigned    lane
    )
{
    int i;
    int result=0;
    int temp;
    unsigned base=TIMERE_GET();
    unsigned repeat=TEST_COUNT;
    for(i=0;i<repeat;i++)
    {
        if(run_test_print)
        {            
            printf("%d:%d\n",__LINE__,TIMERE_GET()-base);
            base=TIMERE_GET();
        }
        if(write_regs(reg)<0)
            return 0xff;
        if(run_test_print)
        {            
            printf("%d:%d\n",__LINE__,TIMERE_GET()-base);
            base=TIMERE_GET();
        }
        if(pdm_write_pattern(pattern,i*0x20)<0)
            return 0xff;
        if(run_test_print)
        {            
            printf("%d:%d\n",__LINE__,TIMERE_GET()-base);
            base=TIMERE_GET();
        }
        if(dwc_pctl_stat(PCTL_STAT_Config)<0)
            return 0xff;
        if(run_test_print)
        {            
            printf("%d:%d\n",__LINE__,TIMERE_GET()-base);
            base=TIMERE_GET();
        }
        writel( ((lane << 10) | 0x01),P_PCTL_DTUCFG_ADDR);
        temp=pdm_run();
        if(run_test_print)
        {            
            printf("%d:%d\n",__LINE__,TIMERE_GET()-base);
            base=TIMERE_GET();
        }
        if(temp<0)
        {
            
            ddr_setting->init_pctl(ddr_setting);
            return 0xff;
        }
        result|=temp&0xff;
        if(run_test_print)
        {            
            printf("%d:%d\n",__LINE__,TIMERE_GET()-base);
            base=TIMERE_GET();
        }
        
    }
    return result;
}

static void print_result_title(void)
{
    int i;
    unsigned wdm=readl(P_PCTL_DTUWDM_ADDR);
    debug_print("\t");
    for(i=0;i<16;i++)
    {
        if(wdm&(1<<i))
            continue;
        debug_print("wdrd[%x]\t",i);
    }
}

static void print_result(void)
{
    int i;
    unsigned rd[4];
    unsigned wr[4];
    unsigned char *w,*r;

    for(i=0;i<4;i++)
    {
        rd[i]=APB_Rd(PCTL_DTURD0_ADDR+(i<<2));
        wr[i]=APB_Rd(PCTL_DTUWD0_ADDR+(i<<2));
    }
    unsigned wdm=readl(P_PCTL_DTUWDM_ADDR);
    debug_print("\t");
    w=(unsigned char *)&wr[0];
    r=(unsigned char *)&rd[0];
    for(i=0;i<16;i++)
    {
        if(wdm&(1<<i))
            continue;
        unsigned a,b;
        a=w[i];
        b=r[i];
        debug_print("%02x %02x\t",a,b);//,(a&0xf)>(b&0xf)?(a&0xf)-(b&0xf):(b&0xf)-(a&0xf));
    }

}
static unsigned default_patern[]={0xdd22ee11
, 0x7788bb44
, 0xdd22ee11
, 0x7788bb44
};
static unsigned mem_patern[]={0xd322e110,
0x7786b544,0xdb2ae918,0x7f8ebd4c
};
#if 0
static int pdm_memtest()
{
    int i,j,k;
    unsigned max=readl(P_PCTL_IOCR_ADDR)>>8;
    pdm_set_wdm(0);
    int ret=0,r=0,x;
    for(i=0;i<1;i++)
    {
        if((~max)&(1<<i))
            continue;
        ret=0;
        for(j=0;j<1000;j++)
        {
            pdm_write(mem_patern,j);
            pdm_dtu_enable(i,1);
            pdm_run(1);
            for(k=0;k<3;k++){
                x=pdm_run(0);
                if(check())
                {
                    if(ret==0){
                        debug_print("\nlane%d",i);
                        print_result_title();
                    }
                    debug_print("\n%03d %03d",j,k);
                    print_result();
                    debug_print("\t%d",x);
                    if(ret>10)
                        break;
                    r+=ret++;

                }
            }
            if(ret>10)
               break;


        }
    }
    return r;
}
#endif
void * memcpy(void *dest, const void *src, size_t count)
{
        unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
        char *d8, *s8;

        /* while all data is aligned (common case), copy a word at a time */
        if ( (((ulong)dest | (ulong)src) & (sizeof(*dl) - 1)) == 0) {
                while (count >= sizeof(*dl)) {
                        *dl++ = *sl++;
                        count -= sizeof(*dl);
                }
        }
        /* copy the reset one byte at a time */
        d8 = (char *)dl;
        s8 = (char *)sl;
        while (count--)
                *d8++ = *s8++;

        return dest;
}
