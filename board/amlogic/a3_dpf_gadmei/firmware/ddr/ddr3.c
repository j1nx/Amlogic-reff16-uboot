static int init_pctl_ddr3(struct ddr_set * ddr_setting)
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
    
      APB_Wr(PCTL_IOCR_ADDR, 0xfe00030d);
      
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
    APB_Wr(PCTL_TMRD_ADDR,  ddr_setting->t_mrd);
    APB_Wr(PCTL_TRFC_ADDR,  ddr_setting->t_rfc);
    APB_Wr(PCTL_TRP_ADDR,   ddr_setting->t_rp);
    APB_Wr(PCTL_TAL_ADDR,   ddr_setting->t_al); 
    APB_Wr(PCTL_TCWL_ADDR,  ddr_setting->t_cwl);
    APB_Wr(PCTL_TCL_ADDR,   ddr_setting->cl);
    
    APB_Wr(PCTL_TRAS_ADDR,  ddr_setting->t_ras);
    APB_Wr(PCTL_TRC_ADDR,   ddr_setting->t_rc);
    APB_Wr(PCTL_TRCD_ADDR,  ddr_setting->t_rcd);
    APB_Wr(PCTL_TRRD_ADDR,  ddr_setting->t_rrd);
    APB_Wr(PCTL_TRTP_ADDR,  ddr_setting->t_rtp);
    APB_Wr(PCTL_TWR_ADDR,   ddr_setting->t_wr);
    APB_Wr(PCTL_TWTR_ADDR,  ddr_setting->t_wtr);
    APB_Wr(PCTL_TEXSR_ADDR, ddr_setting->t_exsr);
    APB_Wr(PCTL_TXP_ADDR,   ddr_setting->t_xp);
    APB_Wr(PCTL_TDQS_ADDR,  ddr_setting->t_dqs);
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