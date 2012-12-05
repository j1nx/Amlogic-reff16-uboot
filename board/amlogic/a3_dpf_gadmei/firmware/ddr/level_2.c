static char * phase_name[]={
    "90 ",
    "72 ",
    "54 ",
    "36 ",
    "108",
    "90 ",
    "72 ",
    "54 ",
    "126",
    "108",
    "90 ",
    "72 ",
    "144",
    "126",
    "108",
    "90 "
};
static void dwc_set_dqtr(ddr_adjust_t * reg,unsigned lane,unsigned val)
{
    lane_info_t  lane_info;
    regs2lane(reg,&lane_info,lane);
    lane_info.dqtr=val;
    lane2regs(reg,&lane_info,lane);
}
static int dwc_dq_alignment(struct ddr_set * ddr_setting,
    ddr_adjust_t * reg,
    unsigned lane)
{
    int i,j;
    unsigned result;
    int first_good = -1; 
    int last_good = -1;
    unsigned dq_result,dqtr;
    ddr_adjust_t ireg=*reg;
    ddr_setting->init_pctl(ddr_setting);
    for(i=0;i<7;i++)
    {
        dwc_set_phase(&ireg,lane,i);
        result=dtu_test(ddr_setting,&ireg,lane,-1);
        if(result)
        {
        }else {
            if (first_good<0){
                first_good = i;
                last_good = i;
            }
            else{
                last_good = i;            
            }
        }
    }
    if (last_good<0){
//      serial_puts(" could not find phase.\n");
      return -1;
    }
    dq_result=0;
    
    for(j=3;j>=0;j--)
    {
        for(i=0;i<8;i++)
        {
            if(dq_result&(1<<i))
                continue;
            dqtr=(dqtr&(0xf<<i))|(((j<<2)|j)<<i);
        }
        dwc_set_dqtr(&ireg,lane,dqtr);
        result=dtu_test(ddr_setting,&ireg,lane,-1);
        if(j==3&&result)
        {
            BUG();
            return -1;
        }
        for(i=0;i<8;i++)
        {
            if((dq_result^result)&(1<<i))
                continue;
            dqtr=(dqtr&(0xf<<i))|(((j<<2)|j)<<i);
        }
        dq_result|=result;
    }
    dwc_set_dqtr(reg,lane,dqtr);
    return 0;
}

static int ddr_phase_sel(struct ddr_set *ddr_setting, ddr_adjust_t * reg, unsigned lane)
{
    int i;
    ddr_adjust_t ireg=*reg;
    int first_good;
    int last_good;
    int result;
    ddr_setting->init_pctl(ddr_setting);
    first_good = last_good = -1;
    for (i = 0; i < 7; i++)
    {
        dwc_set_phase(&ireg,lane,i);
        result = dtu_test(ddr_setting,&ireg,lane,-1);
        
        if (!result)
        {
            if (first_good == -1)
                first_good = i;
            last_good = i;
        
        }
        
    }
    
    if (first_good == -1){
        BUG();
        return -1;
    }
    else{
        dwc_set_phase(reg,lane,(first_good + last_good) >> 1);
        return (first_good + last_good) >> 1;
    }
    BUG("What's up");
    return -1;
}
static void dwc_set_dqsdly(ddr_adjust_t * reg,unsigned lane,unsigned dqsdly)
{
    lane_info_t  lane_info;
    regs2lane(reg,&lane_info,lane);
    lane_info.dqstr=dqsdly;
    lane2regs(reg,&lane_info,lane);
}
static void dwc_set_dqsndly(ddr_adjust_t * reg,unsigned lane,unsigned dqsndly)
{
    lane_info_t  lane_info;
    regs2lane(reg,&lane_info,lane);
    lane_info.dqsntr=dqsndly;
    lane2regs(reg,&lane_info,lane);
}
static int ddr_dqsdly_selex(struct ddr_set *ddr_setting, ddr_adjust_t * reg, unsigned lane,unsigned edge)
{
    int i;
    ddr_adjust_t ireg=*reg;
    int first_good;
    int last_good;
    int result;
    ddr_setting->init_pctl(ddr_setting);
    first_good = last_good = -1;
    dwc_set_dqsndly(&ireg,lane,3);
    dwc_set_dqsdly(&ireg,lane,3);
    run_test_print=0;
    for (i = 0; i < 8; i++)
    {
        if(edge)
            dwc_set_dqsndly(&ireg,lane,i);
        else
            dwc_set_dqsdly(&ireg,lane,i);
//        printf("lane %d edge %d %d\n",lane,edge,TIMERE_GET());
        
        result = dtu_test(ddr_setting,&ireg,lane,-1);
//        printf("A lane %d edge %d %d\n",lane,edge,TIMERE_GET());
        
        if (!result)
        {
            if (first_good == -1)
                first_good = i;
            last_good = i;
        
        }
        
    }
    run_test_print=0;
    if (first_good == -1){
        BUG();
        return -1;
    }
    else{
        if(edge)
            dwc_set_dqsndly(&ireg,lane,(first_good + last_good) >> 1);
        else
            dwc_set_dqsdly(&ireg,lane,(first_good + last_good) >> 1);
//        dwc_set_phase(reg,lane,(first_good + last_good) >> 1);
        return (first_good + last_good) >> 1;
    }
    BUG("What's up");
    return -1;
    
}
static int ddr_dqsdly_sel(struct ddr_set *ddr_setting, ddr_adjust_t * reg)
{
    int j,i;
    int result;
    unsigned iocr=readl(P_PCTL_IOCR_ADDR)>>8;
    for(j=0;j<2;j++)
    {
        for(i=0;i<8;i++){
            if((iocr&(1<<i))){
//                printf("lane %d edge %d %d\n",i,j,TIMERE_GET());
                result=ddr_dqsdly_selex(ddr_setting,reg,i,j);
                if(result<0)
                    return result;
            }
        }
    }
    return 0;
}
//0x4901fde4