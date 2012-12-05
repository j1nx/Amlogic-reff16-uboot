#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <config.h>
#include <asm/arch/firm/io.h>
#include <asm/arch/firm/config.h>
#include <memtest.h>


#define DDR_RSLR_LEN 3
#define DDR_RDGR_LEN 4
#define DDR_MAX_WIN_LEN (DDR_RSLR_LEN*DDR_RDGR_LEN)

#if PHYS_MEMORY_SIZE==0x8000000
#define DDR_CTRL 0xff22d6	// 16bit 128M (gadmei)
#elif PHYS_MEMORY_SIZE==0x10000000
#define DDR_CTRL 0xff22da	// 16bit 256M (a3ref)
#endif
//#define ENABLE_POWER_SAVING 

#define NOP_CMD  0
#define PREA_CMD  1
#define REF_CMD  2
#define MRS_CMD  3
#define ZQ_SHORT_CMD 4
#define ZQ_LONG_CMD  5
#define SFT_RESET_CMD 6

STATIC_PREFIX datum   memTestDataBus(volatile datum * address);
STATIC_PREFIX datum * memTestAddressBus(volatile datum * baseAddress, unsigned long nBytes);
STATIC_PREFIX datum * memTestDevice(volatile datum * baseAddress, unsigned long nBytes);

#ifndef CONFIG_AML_UBOOT_MAGIC
#define CONFIG_AML_UBOOT_MAGIC 0x12345678
#endif
static unsigned ddr_start_again=1;

#define APB_Wr(addr, data) WRITE_APB_REG(addr,data)
#define APB_Rd(addr) READ_APB_REG(addr)

void set_ddr_clock(struct ddr_set * ddr_setting)
{
 int i;
#ifdef ENABLE_POWER_SAVING
    APB_Wr(PCTL_DLLCR_ADDR, APB_Rd(PCTL_DLLCR_ADDR)|(7<<2));
#endif
    writel(ddr_setting->ddr_pll_cntl|0x8000, P_HHI_DDR_PLL_CNTL);
    writel(0x65e31ff, P_HHI_DDR_PLL_CNTL2);
    writel(0x1649a941, P_HHI_DDR_PLL_CNTL3);
    writel(ddr_setting->ddr_pll_cntl, P_HHI_DDR_PLL_CNTL);
    for (i=0;i<1000;i++){
		readl(P_HHI_DDR_PLL_CNTL3);
    }
}

void load_nop(void)
{
  APB_Wr(PCTL_MCMD_ADDR, (1 << 31) |
                         (1 << 20) |   //rank select
                          NOP_CMD );
  while ( APB_Rd(PCTL_MCMD_ADDR) & 0x80000000 ) {}
}

void load_prea(void)
{
  APB_Wr(PCTL_MCMD_ADDR, (1 << 31) |
                         (1 << 20) |   //rank select
                         PREA_CMD );
  while ( APB_Rd(PCTL_MCMD_ADDR) & 0x80000000 ) {}
}

void load_mrs(int mrs_num, int mrs_value)
{
  APB_Wr(PCTL_MCMD_ADDR, (1 << 31) | 
                         (1 << 20) |   //rank select
                   (mrs_num << 17) |
                  (mrs_value << 4) |
                           MRS_CMD );
  while ( APB_Rd(PCTL_MCMD_ADDR) & 0x80000000 ) {};
}

void load_ref(void )
{
  APB_Wr(PCTL_MCMD_ADDR, (1 << 31) | 
                         (1 << 20) |   //rank select
                          REF_CMD );
  while ( APB_Rd(PCTL_MCMD_ADDR) & 0x80000000 ) {}
}

void load_zqcl(int zqcl_value )
{
  APB_Wr(PCTL_MCMD_ADDR, (1 << 31) | 
                         (1 << 20) |   //rank select
                  (zqcl_value << 4 ) |
                          ZQ_LONG_CMD );
  while ( APB_Rd(PCTL_MCMD_ADDR) & 0x80000000 ) {}
}

#if 0
void zq_adjust()
{
    int odt_pull_up, odt_pull_down, drv_pull_up, drv_pull_down;
#if 1
	unsigned char zqdrv[32] = {
    	0x00, 0x01, 0x02, 0x03, 0x06, 0x07, 0x04, 0x05, 
    	0x0c, 0x0d, 0x0e, 0x0f, 0x0a, 0x0b, 0x08, 0x09, 
    	0x18, 0x19, 0x1a, 0x1b, 0x1e, 0x1f, 0x1c, 0x1d, 
    	0x14, 0x15, 0x16, 0x17, 0x12, 0x13, 0x10, 0x11
	};
    int zq, i;
    zq = APB_Rd(PCTL_ZQSR_ADDR);
    odt_pull_up = (zq>>15)&0x1f; 
    odt_pull_down = (zq>>10)&0x1f; 
    drv_pull_up = (zq>>5)&0x1f; 
    drv_pull_down = (zq>>0)&0x1f; 
    for (i=1;i<31;i++){
        if (odt_pull_up == zqdrv[i]){
            odt_pull_up = zqdrv[i];
            break;
        }
    }
    for (i=1;i<31;i++){
        if (odt_pull_down == zqdrv[i]){
            odt_pull_down = zqdrv[i+1];
            break;
        }
    }
#else
    odt_pull_up = 0x05; 
    odt_pull_down = 0x05; 
    drv_pull_up = 0x0e; 
    drv_pull_down = 0x0e; 
#endif
    APB_Wr(PCTL_ZQCR_ADDR, ((odt_pull_up>>1)<<12)|((odt_pull_down>>1)<<8)|((drv_pull_up>>1)<<4)|((drv_pull_down>>1)<<0)|(1<<24));
    APB_Wr(MMC_PHY_CTRL, (APB_Rd(MMC_PHY_CTRL)&(~0x00f0))|((odt_pull_up&1)<<7)|((odt_pull_down&1)<<6)|((drv_pull_up&1)<<5)|((drv_pull_down&1)<<4));
}
#endif

int init_pctl_ddr3(struct ddr_set * ddr_setting)
{
    int mrs0_value;
    int mrs1_value;
    int mrs2_value;
    int mrs3_value;
    writel(RESET_DDR,P_RESET1_REGISTER);

    mrs0_value =          (1 << 12 ) |   // 1 fast, 0 slow.
                            (4 <<9 )   |   //wr recovery   4 means write recovery = 8 cycles..
                            (0  << 8 ) |   //DLL reset.
                            (0  << 7 ) |   //0= Normal 1=Test.
                            (0  << 3 ) |   //burst type,  0:sequential 1 Interleave.
                            (4  << 4 ) |   //cas latency high bits.  cas latency = 7.         
                            (0  << 2 ) |   //cas latency bit 0.
                                   0 ;     //burst length  :  2'b00 fixed BL8 

      mrs1_value =       (0 << 12 )  | // qoff 0=op buf enabled : 1=op buf disabled  (A12)
                         (0 << 11 )  | // rdqs enable 0=disabled : 1=enabled                (A11)
                         (0 << 10 )  | //  reserved for future use                 (A10)
                         (0 << 8 )   |      //reserved for future use.                 (A8)
                         (0 << 7  )  |  // Write leveling enable;      // 0=disable : 1=enable
                         (ddr_setting->t_al << 3) | //additive_latency; // 0 - 4                                 (A5-A3)
                         ( 0 << 9)   |
                         ( 0 << 6)   |
                         ( 1 << 2)   | //ODT;    //(A9, A6, A2) 0=disable : 1=Rzq/4 : 2=Rzq/2 : 3=Rzq/6 : 4=Rzq/12 : 5=Rzq/8 
                         ( 0 << 5)   |
                         ( 1 << 1 )  | //ocd_impedence;    // (A5, A1) 0=Rzq/6 : 1=Rzq/7
                         ( 0 << 0 ) ;  // dll_enable;       // 0=enable : 1=disable

       mrs2_value =  (0 << 9 )|    // Dynamic ODT. 
                     (0 << 7 )|   // self refresh temperature. 0 =Normal. 1 = extended(0~90).                  (A7)
                     (0 << 6) |   // Auto self refresh. 0 = disable. 1 = enable.                               (A6)
                     ((ddr_setting->t_cwl-5) << 3) |   // CWL   000: 5(ck>=2.5ns) 001: 6(ck>1.875ns), 010:7(ck>1.5ns) 011:8(ck>1.25ns)      (A5, A4, A3)
                     0;        // full Araay self -Refresh.   (A2, A1, A0)
    mrs3_value = 0;
    
    APB_Wr(MMC_DDR_CTRL, ddr_setting->ddr_ctrl);//reset pin ???
    //configure basic DDR PHY parameter.
      APB_Wr(PCTL_TOGCNT1U_ADDR, ddr_setting->t_1us_pck);
      APB_Wr(PCTL_TOGCNT100N_ADDR, ddr_setting->t_100ns_pck);
      APB_Wr(PCTL_TINIT_ADDR, ddr_setting->t_init_us);
      APB_Wr(PCTL_TRSTH_ADDR, 10);         //500 us for reset time. 
      APB_Wr(PCTL_TRSTL_ADDR, 10);        //  100 clock cycles for reset low 

      // to disalbe cmd lande receiver current. 
      // we don't need receive data from command lane.
      APB_Wr(MMC_PHY_CTRL,   1 );  

      //configure DDR PHY power down/self refresh power saving mode.
//    APB_Wr(PCTL_IOCR_ADDR, (APB_Rd(PCTL_IOCR_ADDR) & 0xffff00ff) | ((ddr_setting->ddr_ctrl&(1<<7))?0x300:0xf00));

#ifdef ENABLE_POWER_SAVING    
      APB_Wr(PCTL_IOCR_ADDR, 0xfe00030d);
#else
      APB_Wr(PCTL_IOCR_ADDR, 0xcc00030d);
#endif
      
      APB_Wr(PCTL_PHYCR_ADDR, 0x070); // bit 0 0 = active windowing mode.
                                     // bit 3 0 = drift compensation disabled.
                                     // bit 8 0 = automatic data training enable.

    while (!(APB_Rd(PCTL_POWSTAT_ADDR) & 2)) {} // wait for dll lock

    APB_Wr(PCTL_POWCTL_ADDR, 1);            // start memory power up sequence
    while (!(APB_Rd(PCTL_POWSTAT_ADDR) & 1)) {} // wait for memory power up


     
       //configure the PCTL for DDR2 SDRAM burst length = 4
      APB_Wr(PCTL_MCFG_ADDR,     1 |            // burst length 0 = 4; 1 = 8
                                (0 << 2) |     // bl8int_en.   enable bl8 interrupt function.
                                (1 << 5) |     // 1: ddr3 protocal; 0 : ddr2 protocal
                                (1 <<18) |      // tFAW.
                               (1 << 17) |      // power down exit which fast exit.
                                (0xf << 8)      // 0xf cycle empty will entry power down mode.
                                        );

    //configure DDR3 SDRAM parameter.
    APB_Wr(PCTL_TREFI_ADDR, ddr_setting->t_refi_100ns);
    APB_Wr(PCTL_TMRD_ADDR, 	ddr_setting->t_mrd);
    APB_Wr(PCTL_TRFC_ADDR, 	ddr_setting->t_rfc);
    APB_Wr(PCTL_TRP_ADDR, 	ddr_setting->t_rp);
    APB_Wr(PCTL_TAL_ADDR,  	ddr_setting->t_al); 
    APB_Wr(PCTL_TCWL_ADDR,  ddr_setting->t_cwl);
    APB_Wr(PCTL_TCL_ADDR, 	ddr_setting->cl);
    
    APB_Wr(PCTL_TRAS_ADDR, 	ddr_setting->t_ras);
    APB_Wr(PCTL_TRC_ADDR, 	ddr_setting->t_rc);
    APB_Wr(PCTL_TRCD_ADDR, 	ddr_setting->t_rcd);
    APB_Wr(PCTL_TRRD_ADDR, 	ddr_setting->t_rrd);
    APB_Wr(PCTL_TRTP_ADDR, 	ddr_setting->t_rtp);
    APB_Wr(PCTL_TWR_ADDR, 	ddr_setting->t_wr);
    APB_Wr(PCTL_TWTR_ADDR, 	ddr_setting->t_wtr);
    APB_Wr(PCTL_TEXSR_ADDR, ddr_setting->t_exsr);
    APB_Wr(PCTL_TXP_ADDR, 	ddr_setting->t_xp);
    APB_Wr(PCTL_TDQS_ADDR, 	ddr_setting->t_dqs);
    APB_Wr(PCTL_TMOD_ADDR,  ddr_setting->t_mod);
    APB_Wr(PCTL_TZQCL_ADDR, ddr_setting->t_zqcl);
    APB_Wr(PCTL_TCKSRX_ADDR,ddr_setting->t_cksrx);
    APB_Wr(PCTL_TCKSRE_ADDR,ddr_setting->t_cksre);
    APB_Wr(PCTL_TCKE_ADDR,  ddr_setting->t_cke);
    APB_Wr(PCTL_ODTCFG_ADDR, 8);         //configure ODT
    //ZQ calibration 
    APB_Wr(PCTL_ZQCR_ADDR, ( 1 << 31) | (0x3b <<16) );
    //wait for ZQ calibration.
    while ( APB_Rd(PCTL_ZQCR_ADDR ) & ( 1<<31) ) {}
        if ( APB_Rd(PCTL_ZQSR_ADDR) & (1 << 30) ) {
        return -1;
    } 
    //zq_adjust();
      // initialize DDR3 SDRAM
        load_nop();
        load_mrs(2, mrs2_value);
        load_mrs(3, mrs3_value);
        mrs1_value = mrs1_value & 0xfffffffe; //dll enable 
        load_mrs(1, mrs1_value);
        mrs0_value = mrs0_value | (1 << 8);    // dll reset.
        load_mrs(0, mrs0_value);
        load_zqcl(0);     // send ZQ calibration long command.
        return 0;
}

int init_pctl_ddr2(struct ddr_set * ddr_setting)
{
    int mrs0_value;
    int mrs1_value;
    int mrs2_value;
    int mrs3_value;
    writel(RESET_DDR,P_RESET1_REGISTER);

    mrs0_value =  (0 << 12 ) |              //0 fast, 1 slow.
                  (ddr_setting->t_wr <<9 ) |  //wr cycles.
                  (0  << 8 )   |            //DLL reset.
                  (0  << 7)    |            //0= Normal 1=Test.
                  (ddr_setting->cl <<4) |     //cas latency.
                  (0 << 3) |                //burst type,  0:sequential 1 Interleave.
                  2 ;                       //burst length  : 010 : 4,  011: 8.

    mrs1_value =  (0 << 12 )  |             // qoff 0=op buf enabled : 1=op buf disabled  (A12)
                  (0 << 11 )  |             // rdqs enable 0=disabled : 1=enabled         (A11)
                  (0 << 10 )  |             // DQSN enable 1=disabled : 0=enabled         (A10)
                  (0 << 7  )  |             // ocd_program;      // 0, 1, 2, 3, or 7      (A9-A7)
                  (ddr_setting->t_al << 3) |  //additive_latency; // 0 - 4                  (A5-A3)
				  ( 0 << 6)   |
                  ( 1 << 2)   |             //rtt_nominal;      // 50, 75, or 150 ohms    (A6:A2)
                  ( 1 << 1 )  | //( 0 << 1 )  |             //ocd_impedence;    // 0=full : 1=reduced strength (A1)
                  ( 0 << 0 ) ;              // dll_enable;       // 0=enable : 1=disable       (A0)
    mrs2_value = 0;
    mrs3_value = 0;
    
    APB_Wr(PCTL_IOCR_ADDR, (APB_Rd(PCTL_IOCR_ADDR) & 0xffff00ff) | ((ddr_setting->ddr_ctrl&(1<<7))?0x300:0xf00));
    //write memory timing registers
    APB_Wr(PCTL_TOGCNT1U_ADDR, 		ddr_setting->t_1us_pck);
    APB_Wr(PCTL_TINIT_ADDR, 		ddr_setting->t_init_us);
    APB_Wr(PCTL_TOGCNT100N_ADDR, 	ddr_setting->t_100ns_pck);
    APB_Wr(PCTL_TREFI_ADDR, 		ddr_setting->t_refi_100ns);

    APB_Wr(PCTL_TRSTH_ADDR, 0);       // 0 for ddr2
    while (!(APB_Rd(PCTL_POWSTAT_ADDR) & 2)) {} // wait for dll lock

    APB_Wr(PCTL_POWCTL_ADDR, 1);            // start memory power up sequence
    while (!(APB_Rd(PCTL_POWSTAT_ADDR) & 1)) {} // wait for memory power up

    APB_Wr(PCTL_ODTCFG_ADDR, 8);         //configure ODT

      // we don't need receive data from command lane.
      APB_Wr(MMC_PHY_CTRL,   1 );  

      //configure DDR PHY power down/self refresh power saving mode.
      APB_Wr(PCTL_IOCR_ADDR, 0xfe0c030d);
    APB_Wr(PCTL_TMRD_ADDR, 	ddr_setting->t_mrd);
    APB_Wr(PCTL_TRFC_ADDR, 	ddr_setting->t_rfc);
    APB_Wr(PCTL_TRP_ADDR, 	ddr_setting->t_rp);
    APB_Wr(PCTL_TAL_ADDR,  	ddr_setting->t_al);
    APB_Wr(PCTL_TCWL_ADDR,  ddr_setting->cl-1 + ddr_setting->t_al);
    APB_Wr(PCTL_TCL_ADDR, 	ddr_setting->cl);
   	APB_Wr(PCTL_TRAS_ADDR, 	ddr_setting->t_ras);
    APB_Wr(PCTL_TRC_ADDR, 	ddr_setting->t_rc);
    APB_Wr(PCTL_TRCD_ADDR, 	ddr_setting->t_rcd);
    APB_Wr(PCTL_TRRD_ADDR, 	ddr_setting->t_rrd);
    APB_Wr(PCTL_TRTP_ADDR, 	ddr_setting->t_rtp);
    APB_Wr(PCTL_TWR_ADDR, 	ddr_setting->t_wr);
    APB_Wr(PCTL_TWTR_ADDR, 	ddr_setting->t_wtr);
    APB_Wr(PCTL_TEXSR_ADDR, ddr_setting->t_exsr);
    APB_Wr(PCTL_TXP_ADDR, 	ddr_setting->t_xp);
    APB_Wr(PCTL_TDQS_ADDR, 	ddr_setting->t_dqs);
    //configure the PCTL for DDR2 SDRAM burst length = 4
   APB_Wr(PCTL_MCFG_ADDR, 0 |     // burst length = 4
                    (0 << 2) |     // bl8int_en.   enable bl8 interrupt function.
                      (0 << 5) |      // 0: ddr2 protocal 1: ddr3 protocal
                      (1 <<18) |       // tFAW.
                      (1 << 17) |      // power down exit with fast exit.
                      (0xf << 8 )       // enable auto power mode with 0xf clock cycles NIF IDLE.
                                );


   // Don't do it for gates simulation.
	APB_Wr(PCTL_ZQCR_ADDR,   (1 << 24) | 0x33dd);
//	APB_Wr(PCTL_ZQCR_ADDR,   (1 << 24) | 0x1166);

    // initialize SDRAM
    
    load_nop();
    load_prea();
    load_mrs(2, mrs2_value);
    load_mrs(3, mrs3_value);
    mrs1_value = mrs1_value & 0xfffffffe; //dll enable
    load_mrs(1, mrs1_value);
    mrs0_value = mrs0_value | (1 << 8);    // dll reset.
    load_mrs(0, mrs0_value);
    load_prea();
    load_ref();
    load_ref();
    mrs0_value = mrs0_value & (~(1 << 8));    // dll reset.
    load_mrs(0, mrs0_value);
    mrs1_value = (mrs1_value & (~(7 << 7))) | (7 << 7 );  //OCD default.
    load_mrs(1, mrs1_value);
    mrs1_value = (mrs1_value & (~(7 << 7))) | (0 << 7 );  // ocd_exit;
    load_mrs(1, mrs1_value);
    return 0;
}

static  int start_ddr_config(void)
{
    unsigned timeout = -1;
    APB_Wr(PCTL_SCTL_ADDR, 0x1);
    while((APB_Rd(PCTL_STAT_ADDR) != 0x1) && timeout)
        --timeout;

    return timeout;
}

static  int end_ddr_config(void)
{
    unsigned timeout = 10000;
    APB_Wr(PCTL_SCTL_ADDR, 0x2);
    while((APB_Rd(PCTL_STAT_ADDR) != 0x3) && timeout)
        --timeout;

    return timeout;
}

static  int dtu_enable(void)
{
    unsigned timeout = 10000;
    APB_Wr(PCTL_DTUECTL_ADDR, 0x1);  // start wr/rd
    while((APB_Rd(PCTL_DTUECTL_ADDR)&1) && timeout)
        --timeout;
    return timeout;
}

static unsigned char get_best_dtu(unsigned char* p, unsigned char* best)
{
    unsigned char i;

    for(i=0;i<=DDR_MAX_WIN_LEN -DDR_RDGR_LEN +1;i++)
    {
        if(*(p+i) + *(p+i+1) + *(p+i+2) == 48)
            goto next;
    }
#if 0
    for(i=0;i<DDR_MAX_WIN_LEN-1 ;i++)
    {
        if(*(p+i) + *(p+i+1) == 32)
        {
            *best=i;
            return 0;
        }
    }
#endif

    return 1;

next:

    for(i=0;i<=DDR_MAX_WIN_LEN -DDR_RDGR_LEN;i++)
    {
        if(*(p+i) + *(p+i+1) + *(p+i+2) + *(p+i+3) == 64)
        {
            if(!i)
                *best = 2;
            else
            {
                if(*(p+i-1)>*(p+i+4))
                    *best = i + 1;
                else
                    *best = i + 2;
            }

            return 0;
        }
    }

    for(i=0;i<=DDR_MAX_WIN_LEN -DDR_RDGR_LEN +1;i++)
    {
        if(*(p+i) + *(p+i+1) + *(p+i+2) == 48)
        {
            *best = i + 1;
            return 0;
        }
    }

    return 2;
}

static void set_result(unsigned char* res)
{
    unsigned rslr0=((res[0]>>2)&7) | (((res[1]>>2)&7)<<3) | (((res[2]>>2)&7)<<6) | (((res[3]>>2)&7)<<9);
    unsigned rdgr0=(res[0]&3)      | ((res[1]&3)     <<2) | ((res[2]&3)     <<4) | ((res[3]&3)<<6);
    APB_Wr(PCTL_RSLR0_ADDR,rslr0);
    APB_Wr(PCTL_RDGR0_ADDR,rdgr0);
#if 0
    serial_puts("\nRSLR0=");
    serial_put_hex(rslr0,32);
    serial_puts(" RDGR0=");
    serial_put_hex(rdgr0,32);
    serial_puts("\n");
#endif
    
}

void init_dmc(struct ddr_set * ddr_setting)
{
	APB_Wr(MMC_DDR_CTRL, ddr_setting->ddr_ctrl);
	APB_Wr(MMC_REQ_CTRL,0xff);
}




static unsigned char dllcr[7]={3,2,1,0,0xe,0xd,0xc};
static unsigned char max_lane = 2;
#define TEST_COUNT 16
#define MAX_PATERN 18
static unsigned phd[MAX_PATERN][4]={
                            {0xdd22ee11, 0x7788bb44, 0x337755aa, 0xff00aa55},
                            {0xff000000, 0x000000ff, 0x00ffffff, 0xffffff00},

                            {0x01fe00ff, 0x01fe00ff, 0x01fe00ff, 0x01fe00ff},
                            {0x02fc00ff, 0x02fc00ff, 0x02fc00ff, 0x02fc00ff},
                            {0x04fb00ff, 0x04fb00ff, 0x04fb00ff, 0x04fb00ff},
                            {0x08f700ff, 0x08f700ff, 0x08f700ff, 0x08f700ff},
                            {0x10ef00ff, 0x10ef00ff, 0x10ef00ff, 0x10ef00ff},
                            {0x20df00ff, 0x20df00ff, 0x20df00ff, 0x20df00ff},
                            {0x40bf00ff, 0x40bf00ff, 0x40bf00ff, 0x40bf00ff},
                            {0x807f00ff, 0x807f00ff, 0x807f00ff, 0x807f00ff},

                            {0xfe01ff00, 0xfe01ff00, 0xfe01ff00, 0xfe01ff00},
                            {0xfd02ff00, 0xfd02ff00, 0xfd02ff00, 0xfd02ff00},
                            {0xfb04ff00, 0xfb04ff00, 0xfb04ff00, 0xfb04ff00},
                            {0xf708ff00, 0xf708ff00, 0xf708ff00, 0xf708ff00},
                            {0xef10ff00, 0xef10ff00, 0xef10ff00, 0xef10ff00},
                            {0xdf20ff00, 0xdf20ff00, 0xdf20ff00, 0xdf20ff00},
                            {0xbf40ff00, 0xbf40ff00, 0xbf40ff00, 0xbf40ff00},
                            {0x7f80ff00, 0x7f80ff00, 0x7f80ff00, 0x7f80ff00},
                          };

static int dtu_test(struct ddr_set *ddr_setting, unsigned lane, unsigned char *tra, unsigned char phase, unsigned dq, unsigned char *dqs, unsigned char *dqsn, unsigned pattern_index, unsigned repeat_count)
{
    int i=0;
    int result = -1;
    int dtu_result;
    for (i=0;i<repeat_count;i++){
        APB_Wr(PCTL_PHYCR_ADDR, 0x60);
        APB_Wr(PCTL_PHYCR_ADDR, 0x70);
        if (!start_ddr_config())
            return 0xff;
            set_result(tra);
        APB_Wr(PCTL_DQSTR_ADDR, (APB_Rd(PCTL_DQSTR_ADDR)&~(0x7<<(lane*3)))|(dqs[lane]<<(lane*3)));
        APB_Wr(PCTL_DQSNTR_ADDR, (APB_Rd(PCTL_DQSNTR_ADDR)&~(0x7<<(lane*3)))|(dqsn[lane]<<(lane*3)));
        APB_Wr(PCTL_DTUCFG_ADDR, (lane << 10) | 0x01);
        APB_Wr(PCTL_DLLCR0_ADDR+(lane<<2), (APB_Rd(PCTL_DLLCR0_ADDR+(lane<<2)) & ~(0xf<<14)) | (phase<<14));
        APB_Wr(PCTL_DQTR0_ADDR+(lane<<2), dq);
        APB_Wr(PCTL_DTUWD0_ADDR, phd[pattern_index][0]);
        APB_Wr(PCTL_DTUWD1_ADDR, phd[pattern_index][1]);
        APB_Wr(PCTL_DTUWD2_ADDR, phd[pattern_index][2]);
        APB_Wr(PCTL_DTUWD3_ADDR, phd[pattern_index][3]);
        APB_Wr(PCTL_DTUWACTL_ADDR, repeat_count*0x20);
        APB_Wr(PCTL_DTURACTL_ADDR, repeat_count*0x20);
        if (!end_ddr_config())
            return 0xff;
        if (dtu_enable()){
        if (APB_Rd(PCTL_DTUPDES_ADDR)>>8){
            //printf("\n%d ****** dtu test error: %x ********\n", i, APB_Rd(PCTL_DTUPDES_ADDR));
                return 0xff;
        }
        else{
            if (result==-1)
              result = 0;
                result |= APB_Rd(PCTL_DTUPDES_ADDR)&0xff;
        }    
        }
        else{
            //printf("\n%d ****** dtu enable error ********\n", i);
            return 0xff;
        }
        APB_Wr(PCTL_DTUECTL_ADDR, 0x0);
    }
    return result&0xff;
}

static unsigned char ddr_window(struct ddr_set *ddr_setting, int lane, unsigned char *chk, unsigned char *tra, unsigned char *dll_phase, unsigned *dq_dly, unsigned char *dqs, unsigned char *dqsn, int pattern_mask, int test_count, int tra_mask)
{
    int i,j,l;
  unsigned char Tra_tmp[2];

  unsigned result;
    init_pctl_ddr3(ddr_setting);
  serial_puts("\nwindow select:\n");
    serial_puts("lane ");
    serial_put_hex(lane,8);
    serial_puts(", phase ");
    serial_put_hex(dll_phase[lane],8);
    serial_puts(":");
    Tra_tmp[0] = tra[0];
    Tra_tmp[1] = tra[1];

		for (i = 0; i < DDR_RSLR_LEN; i++) {
			for (j = 0; j < DDR_RDGR_LEN; j++) {
        if (chk[lane*DDR_RSLR_LEN*DDR_RDGR_LEN + i * DDR_RDGR_LEN + j]==16){
          if (tra_mask==0)
              Tra_tmp[1] = Tra_tmp[0] = i*DDR_RDGR_LEN+j;
          else
              Tra_tmp[lane] = i*DDR_RDGR_LEN+j;
        result = 0;
        if (i==DDR_RSLR_LEN-1)
          i = DDR_RSLR_LEN-1;
        for (l=0;l<32;l++){
          if ((1<<l)&pattern_mask){
            //printf("%d %d %d: %d %d %x %x %d\n", k, i, j, Tra_tmp[0], Tra_tmp[1], dllcr[dll_phase[k]], dq_dly[k], l);
              result |= dtu_test(ddr_setting, lane, Tra_tmp, dllcr[dll_phase[lane]], dq_dly[lane], dqs, dqsn, l, test_count);
          }
        }
          if (!result){
            chk[lane*DDR_RSLR_LEN*DDR_RDGR_LEN + i * DDR_RDGR_LEN + j] = 16;
          }
          else{
            chk[lane*DDR_RSLR_LEN*DDR_RDGR_LEN + i * DDR_RDGR_LEN + j] = 0;
          }
        }
			}
		}
    
    for (i = 0; i < DDR_RSLR_LEN; i++) {
			for (j = 0; j < DDR_RDGR_LEN; j++) {
            if (chk[lane*DDR_RSLR_LEN*DDR_RDGR_LEN + i*DDR_RDGR_LEN + j]==16)
                serial_puts("o");
            else
                serial_puts("x");
      }
    }
    if (get_best_dtu(&chk[lane*DDR_RSLR_LEN*DDR_RDGR_LEN], &Tra_tmp[lane])) {
        serial_puts(" -- ");
        serial_puts("not found\n");
        Tra_tmp[lane] = 0xff;
		}else{
		    serial_puts(" -- ");
        serial_put_hex(Tra_tmp[lane],8);
        serial_puts("\n");
	}
    return Tra_tmp[lane];
}

static int dwc_dq_alignment(struct ddr_set *ddr_setting, unsigned lane, unsigned char *tra, unsigned char *dqs, unsigned char *dqsn)
{
    int i,j,k,dqcr,result;
    int first_good = -1; 
    int last_good = -1;
    unsigned dq_result; 
    
    init_pctl_ddr3(ddr_setting);
    serial_puts("\nLane ");
    serial_put_hex(lane,8);
    serial_puts(" go phase from 0 to 6: ");
    dqcr = 0xffffffff;
    for(i=0;i<7;i++)
    {
        result = 0;
        for (k=0;k<MAX_PATERN;k++)
            result |= dtu_test(ddr_setting, lane, tra, dllcr[i], dqcr, dqs, dqsn, k, TEST_COUNT);
        if (result){
                serial_puts("x");
        }
        else{
            if (first_good<0){
                first_good = i;
                last_good = i;
            }
            else{
                last_good = i;            
            }
            serial_puts("o");
        }
    }
    if (last_good<0){
      serial_puts(" could not find phase.\n");
      return -1;
    }
    serial_puts("\n");
    if (dqcr==0xffffffff){
        dq_result = 0;
        for (j=3;j>=0;j--){
            for (i=0;i<8;i++){
                if (!(dq_result & (1<<i))){
                    dqcr = (dqcr&~(0xf<<(i*4))) | (((j<<2)|j)<<(i*4));
                }       
            }
            result = 0;
            for (k=0;k<MAX_PATERN;k++)
              result |= dtu_test(ddr_setting, lane, tra, dllcr[last_good], dqcr, dqs, dqsn, k, TEST_COUNT);
            serial_put_hex(j,8);
            serial_puts(": ");
            serial_put_hex(dllcr[last_good],8);
            serial_puts(" ");
            serial_put_hex(dqcr,32);
            serial_puts(" ");
            serial_put_hex(result&0xff,8);
            serial_puts(" ");
            for (i=0;i<8;i++){
                if ((!(dq_result & (1<<i)))&&(result & (1<<i))){
                    if (j<3)
                        dqcr = (dqcr&~(0xf<<(i*4))) | ((((j+1)<<2)|(j+1))<<(i*4));
                    //else
                    //    printf("err -==================- \n");
                }       
            }
            dq_result |= result;
            serial_put_hex(dq_result&0xff,8);
            serial_puts(" ");
            serial_put_hex(dqcr,32);
            serial_puts("\n");
        }
    }
    return dqcr;
}

static int ddr_phase_sel(struct ddr_set *ddr_setting, int lane, unsigned char *tra, unsigned *tr, unsigned char *dqs, unsigned char *dqsn)
{
    int i, k;
    int first_good;
    int last_good;
    int result;

    init_pctl_ddr3(ddr_setting);
    serial_puts("\nphase select:\n");
    {
        serial_puts("lane ");
        serial_put_hex(lane,8);
        serial_puts(": ");
        first_good = last_good = -1;
        for (i = 0; i < 7; i++)
        {
            result = 0;
            for(k = 0; k < MAX_PATERN; k++)
                result |= dtu_test(ddr_setting, lane, tra, dllcr[i], tr[lane], dqs, dqsn, k, TEST_COUNT);
            if (!result){
                if (first_good == -1)
                    first_good = i;
                last_good = i;
                serial_puts("o");
                    }
            else{
              serial_puts("x");
            }
        }

        if (first_good == -1){
		    serial_puts(" could not find phase\n");
            return 3;
        }
        else{
            serial_puts(" -- ");
			serial_put_hex((first_good + last_good) >> 1, 8);
            serial_puts("\n");
            return (first_good + last_good) >> 1;
        }
    }
}

static int dqs_dly_select(struct ddr_set *ddr_setting, unsigned char *phase, unsigned char *tra, unsigned *tr, unsigned char *dqs, unsigned char *dqsn)
{
  int i,j,k;
  unsigned char result;
  int first_good, last_good;
  unsigned char dqs_temp[2];
  unsigned char dqsn_temp[2];

  init_pctl_ddr3(ddr_setting);
  serial_puts("\ndqs_dly:\n");
  dqsn_temp[0] = dqsn_temp[1] = 3;
  for (i=0;i<2;i++){
    serial_puts("lane ");
    serial_put_hex(i,8);
    serial_puts(": ");
    dqs_temp[0] = dqs_temp[1] = 3;
    first_good = last_good = -1;
    for (j=0;j<8;j++){
      dqs_temp[i] = j; 
      result = 0;
      for (k=0;k<MAX_PATERN;k++)
        result |= dtu_test(ddr_setting, i, tra, dllcr[phase[i]], tr[i], dqs_temp, dqsn_temp, k, TEST_COUNT);
      if (!result){
        if (first_good<0)
          first_good = last_good = j;
        else
          last_good = j;
        serial_puts("o");
      }
      else{
        serial_puts("x");
      }
    }
    if (first_good>=0){
      dqs[i] = (first_good+last_good)>>1;
	  serial_puts(" -- ");
      serial_put_hex(dqs[i],8);
      serial_puts("\n");
    }
    else{
      serial_puts(" failed\n"); 
      dqs[i] = 3;
    }
  }

  dqs_temp[0] = dqs[0];
  dqs_temp[1] = dqs[1];
  serial_puts("\ndqsn_dly:\n");
  for (i=0;i<2;i++){
    serial_puts("lane ");
    serial_put_hex(i,8);
    serial_puts(": ");
    dqsn_temp[0] = dqsn_temp[1] = 3;
    first_good = last_good = -1;
    for (j=0;j<8;j++){
      dqsn_temp[i] = j; 
      result = 0;
      for (k=0;k<MAX_PATERN;k++)
        result |= dtu_test(ddr_setting, i, tra, dllcr[phase[i]], tr[i], dqs_temp, dqsn_temp, k, TEST_COUNT);
      if (!result){
        if (first_good<0)
          first_good = last_good = j;
        else
          last_good = j;
        serial_puts("o");
      }
      else{
        serial_puts("x");
      }
    }
    if (first_good>=0){
      dqsn[i] = (first_good+last_good)>>1;
	  serial_puts(" -- ");
      serial_put_hex(dqsn[i],8);
      serial_puts("\n");
    }
    else{
      serial_puts(" failed\n");
      dqsn[i] = 3; 
    }
  }
  return 0;
}

static unsigned ddr_init (struct ddr_set * ddr_setting)
{
    int i,j;
    unsigned char phase_retry[5]={3,4,2,5,1};
    unsigned char Tra[2]={5,5};
    unsigned char Phase[2]={3,3};
	unsigned int Tr[2]={0xffffffff,0xffffffff};
    unsigned char chk[2*DDR_RSLR_LEN*DDR_RDGR_LEN];
	unsigned char Dqs[2]={3,3};
	unsigned char Dqsn[2]={3,3};
	
    writel(RESET_DDR,P_RESET1_REGISTER);
	serial_puts("\nTraining start:\n");
    for (i=0;i<2*DDR_RSLR_LEN*DDR_RDGR_LEN;i++)
      chk[i] = 16;
    for (i=0;i<5;i++){
        Phase[0] = phase_retry[i];
        for (j=0;j<DDR_RSLR_LEN*DDR_RDGR_LEN;j++)
          chk[j] = 16;
        Tra[0] = ddr_window(ddr_setting, 0, chk, Tra, Phase, Tr, Dqs, Dqsn, (1<<MAX_PATERN)-1, TEST_COUNT, 0);
        if (Tra[0]!=0xff)
            break;
    }
    if (Tra[0]==0xff){
        for (i=0;i<5;i++){
            Phase[0] = phase_retry[i];
            for (j=0;j<DDR_RSLR_LEN*DDR_RDGR_LEN;j++)
              chk[j] = 16;
            Tra[0] = ddr_window(ddr_setting, 0, chk, Tra, Phase, Tr, Dqs, Dqsn, 1, TEST_COUNT, 0);
            if (Tra[0]!=0xff)
                break;
        }
        if (Tra[0]=0xff) Tra[0]=5;
    }
    for (i=0;i<5;i++){
        Phase[1] = phase_retry[i];
        for (j=DDR_RSLR_LEN*DDR_RDGR_LEN;j<2*DDR_RSLR_LEN*DDR_RDGR_LEN;j++)
          chk[j] = 16;
        Tra[1] = ddr_window(ddr_setting, 1, chk, Tra, Phase, Tr, Dqs, Dqsn, (1<<MAX_PATERN)-1, TEST_COUNT, 1);
        if (Tra[1]!=0xff)
            break;
    }
    if (Tra[1]==0xff){
        for (i=0;i<5;i++){
            Phase[1] = phase_retry[i];
            for (j=DDR_RSLR_LEN*DDR_RDGR_LEN;j<2*DDR_RSLR_LEN*DDR_RDGR_LEN;j++)
              chk[j] = 16;
            Tra[1] = ddr_window(ddr_setting, 1, chk, Tra, Phase, Tr, Dqs, Dqsn, 1, TEST_COUNT, 1);
            if (Tra[1]!=0xff)
                break;
        }
        if (Tra[1]=0xff) Tra[1]=5;
    }
    Tr[0] = dwc_dq_alignment(ddr_setting, 0, Tra, Dqs, Dqsn);
    Tr[1] = dwc_dq_alignment(ddr_setting, 1, Tra, Dqs, Dqsn);
    Phase[0] = ddr_phase_sel(ddr_setting, 0, Tra, Tr, Dqs, Dqsn);
    Phase[1] = ddr_phase_sel(ddr_setting, 1, Tra, Tr, Dqs, Dqsn);
    dqs_dly_select(ddr_setting, Phase, Tra, Tr, Dqs, Dqsn);
    Tra[0] = ddr_window(ddr_setting, 0, chk, Tra, Phase, Tr, Dqs, Dqsn, (1<<MAX_PATERN)-1, TEST_COUNT, 1);
    Tra[1] = ddr_window(ddr_setting, 1, chk, Tra, Phase, Tr, Dqs, Dqsn, (1<<MAX_PATERN)-1, TEST_COUNT, 1);
  
    init_pctl_ddr3(ddr_setting);
    if (!start_ddr_config())
      return 1;
    set_result(Tra);
    APB_Wr(PCTL_DLLCR0_ADDR, (APB_Rd(PCTL_DLLCR0_ADDR) & 0xfffc3fff) | (dllcr[Phase[0]]<<14));
    APB_Wr(PCTL_DLLCR1_ADDR, (APB_Rd(PCTL_DLLCR1_ADDR) & 0xfffc3fff) | (dllcr[Phase[1]]<<14));
    APB_Wr(PCTL_DQSTR_ADDR, (APB_Rd(PCTL_DQSTR_ADDR)&~(0x3f))|(Dqs[1]<<3)|Dqs[0]);
    APB_Wr(PCTL_DQSNTR_ADDR, (APB_Rd(PCTL_DQSNTR_ADDR)&~(0x3f))|(Dqsn[1]<<3)|Dqsn[0]);
    APB_Wr(PCTL_DQTR0_ADDR, Tr[0]);
    APB_Wr(PCTL_DQTR1_ADDR, Tr[1]);
    APB_Wr(PCTL_PHYCR_ADDR, 0x78);
    if (!end_ddr_config())
      return 1;
    	
    init_dmc(ddr_setting);

    serial_puts("\nTraining result:\n");
    serial_puts("\tRSLR0=");
	serial_put_hex(APB_Rd(PCTL_RSLR0_ADDR), 32);
	serial_puts("\n");
    serial_puts("\tRDGR0=");
	serial_put_hex(APB_Rd(PCTL_RDGR0_ADDR), 32);
	serial_puts("\n");
    serial_puts("\tDLLCR0="); 
	serial_put_hex(APB_Rd(PCTL_DLLCR0_ADDR), 32);
	serial_puts("\n");
    serial_puts("\tDLLCR1="); 
	serial_put_hex(APB_Rd(PCTL_DLLCR1_ADDR), 32);
	serial_puts("\n");
    serial_puts("\tDQTR0="); 
	serial_put_hex(APB_Rd(PCTL_DQTR0_ADDR), 32);
	serial_puts("\n");
    serial_puts("\tDQTR1=");
    serial_put_hex(APB_Rd(PCTL_DQTR1_ADDR), 32);
	serial_puts("\n");
    serial_puts("\tDQSTR=");
    serial_put_hex(APB_Rd(PCTL_DQSTR_ADDR), 32);
	serial_puts("\n");
    serial_puts("\tDQSNTR=");
    serial_put_hex(APB_Rd(PCTL_DQSNTR_ADDR), 32);
	serial_puts("\n");

  return 0;
}

static inline unsigned lowlevel_ddr(void)
{
	struct ddr_set * ddr=&__ddr2_nanya_1Gbit;;
    
    if(POR_GET_DDR(romboot_info->por_cfg)==POR_DDR3)
    {
        serial_puts("Init DDR3\n");
        ddr=&__ddr3_nanya_1Gbit;
    }else{
        serial_puts("Init DDR2\n");
    }
    set_ddr_clock(ddr);
    return ddr_init(ddr);
    
    
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
    return (unsigned)memTestAddressBus((volatile datum *)PHYS_MEMORY_START, PHYS_MEMORY_SIZE);
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

unsigned ddr_init_test(void)
{
    int i,j;
    unsigned por_cfg=1;
    serial_putc('\n');
    writel((1<<23)-1,P_WATCHDOG_TC);
    for(j=0;j<13&&por_cfg;j++)
    {
        serial_put_hex(j, 8);
        por_cfg=0;
#ifdef CONFIG_ENABLE_MEM_DEVICE_TEST    
        for(i=0;i<MEM_DEVICE_TEST_ITEMS_BASE+1&&por_cfg==0;i++)
#else    
        for(i=0;i<MEM_DEVICE_TEST_ITEMS_BASE&&por_cfg==0;i++)
#endif    
    	{
    	        writel(0,P_WATCHDOG_RESET);
    	        por_cfg=mem_test[i]();
    	        serial_puts("\nStage ");
    	        serial_put_hex(i,8);
    	        serial_puts(" Result ");
    	        serial_put_hex(por_cfg,32);
    	}
    }
	writel(0,P_WATCHDOG_TC);
	ddr_start_again=por_cfg?1:ddr_start_again;
	return por_cfg;
}
