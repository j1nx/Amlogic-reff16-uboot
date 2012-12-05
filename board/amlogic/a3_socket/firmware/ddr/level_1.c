
#define DDR_RSLR_LEN 6
#define DDR_RDGR_LEN 4
#define DDR_MAX_WIN_LEN (DDR_RSLR_LEN*DDR_RDGR_LEN)
#define dwc_find_window  dwc_find_window_sw



static int get_best_dtu(unsigned short * p,unsigned * winsize)
{
    unsigned char i;
    int best;

#if 1
    int win_start=-1;
    for(i=0;i<DDR_MAX_WIN_LEN ;i++)
    {


        if(win_start==-1&&(*(p+i)==0))
        {
            win_start=i;
        }
        if(win_start!=-1&&(*(p+i)!=0))
        {
            break;
        }
    }
    if(win_start==-1)
        return -1;//NO windows found
    best=(win_start+i)>>1;
    *winsize=i-win_start;
    return best;

#else
    for(i=0;i<=DDR_MAX_WIN_LEN -DDR_RDGR_LEN +1;i++)
    {
        if(*(p+i) + *(p+i+1) + *(p+i+2) == 48)
            goto next;
    }
    return 1;

next:

    for(i=0;i<=DDR_MAX_WIN_LEN -DDR_RDGR_LEN;i++)
    {
        if(*(p+i) + *(p+i+1) + *(p+i+2) + *(p+i+3) == 64)
        {
            if(!i)
                *best = 2;
            else if(i == 8)
                *best = 9;
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
#endif
    return 2;
}
#define DDR_RSLR_LEN 6
#define DDR_RDGR_LEN 4
#define DDR_MAX_WIN_LEN (DDR_RSLR_LEN*DDR_RDGR_LEN)
#define CONFIG_L1_NO_DETAILS 1
static void dwc_set_sl(ddr_adjust_t * reg,unsigned lane,unsigned sl,unsigned mode)
{
    int i;
    lane_info_t lane_info;
    regs2lane(reg,&lane_info,lane);
    lane_info.rslr=(sl&31)>>2;
    lane_info.rdgr=sl&3;
    lane2regs(reg,&lane_info,lane);
    mode&=(~(1<<lane))&(readl(P_PCTL_IOCR_ADDR)>>8);
    for(i=0;i<8;i++)
    {
        if(mode&(1<<i))
        {
            lane2regs_sl(reg,&lane_info,i);   
        }
    }
}
static unsigned phd[][4]=
{
    {0xdd22ee11, 0x7788bb44, 0x337755aa, 0xff00aa55},
    {0xff000000, 0x000000ff, 0x00ffffff, 0xffffff00},
    {0x01000200, 0x04000800, 0x10002000, 0x40008000},
    {0xfefffbff, 0xf7fffeff, 0xefffbfff, 0x7fffdfff},
    {0x00100020, 0x00400080, 0x00100020, 0x00400800},
    {0xfffefffb, 0xfff7fffe, 0xffefffbf, 0xff7fffdf},
    {0x55aa00ff, 0xaa0055ff, 0x55ffaa00, 0xff00aa55},
    {0xdd22ee11, 0x7788bb44, 0xdd22ee11, 0x7788bb44},
    
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
static unsigned dtu_test(struct ddr_set * ddr_setting,
    ddr_adjust_t * reg,
    unsigned lane,
    unsigned pattern_mask)
{
    int j;
    int result=0;
    for(j=0;j<sizeof(phd)/sizeof(phd[0]);j++)
    {
        if((1<<j)&pattern_mask)
        {
            result|=dtu_run_test(ddr_setting,reg,
                phd[j],lane);
            if(result)
                return result;
        }
    }
    return result;
}
STATIC_PREFIX int dwc_find(struct ddr_set * ddr_setting,
    ddr_adjust_t * reg,
    unsigned lane,
    unsigned mode,
    unsigned pattern_mask)
{
	int  result;
	unsigned i=0;
	unsigned j=0;
	unsigned start=DDR_MAX_WIN_LEN+1;
	unsigned end=DDR_MAX_WIN_LEN+1;
    debug_print("\n");
    for(i=0;i<DDR_MAX_WIN_LEN;i++)
    {
        result=0;
        dwc_set_sl(reg,lane,i,mode);
        result=dtu_test(ddr_setting,reg,lane,pattern_mask);
        if(result)
        {
            debug_print("X");
            if(start>DDR_MAX_WIN_LEN)            
                continue;
            else
                break;
        }
        debug_print("O");
        if(start>DDR_MAX_WIN_LEN)
        {
            
            start=i;
        }
        end=i;
    }
//    __builtin_arc_sleep(0);
    if(start>DDR_MAX_WIN_LEN)
        return 0xff;
    end++;
    if(end-start<3)
        return 0xff;
    if(end>DDR_MAX_WIN_LEN)
        end=DDR_MAX_WIN_LEN;
    unsigned middle;
    if(end-start>3 )
        middle=start+2;       
    else
        middle=start+1;
    
    dwc_set_sl(reg,lane,middle,0);
    debug_print(" result %d",middle);
    return middle;
}

static void dwc_set_phase(ddr_adjust_t * reg,unsigned lane,unsigned val)
{
    lane_info_t  lane_info;
    regs2lane(reg,&lane_info,lane);
    static unsigned char dllcr[7]={3,2,1,0,0xe,0xd,0xc};
    lane_info.dllcr=dllcr[val];
    lane2regs(reg,&lane_info,lane);
}
static unsigned dwc_find_system_latency(struct ddr_set * ddr_setting,
    ddr_adjust_t * reg,
    unsigned lane)
{
    int i;
    unsigned tra;
    lane_info_t lane_info;
    ddr_adjust_t ireg;
    ireg=*reg;
    
    for(i=0;i<3;i++)
    {
        dwc_set_phase(&ireg,lane,3+i);
        tra=dwc_find(ddr_setting,&ireg,lane,0xff,-1);
        if(tra!=0xff)
            return tra;
        dwc_set_phase(&ireg,lane,3-i);
        tra=dwc_find(ddr_setting,&ireg,lane,0xff,-1);
        if(tra!=0xff)
            return tra;
    }
    tra=dwc_find(ddr_setting,&ireg,lane,0xff,1);
    if(tra!=0xff)
        return tra;
    return 0xff;
}

