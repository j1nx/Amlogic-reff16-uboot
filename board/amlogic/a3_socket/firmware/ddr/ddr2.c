STATIC_PREFIX int init_pctl_ddr2(struct ddr_set * ddr_setting)
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