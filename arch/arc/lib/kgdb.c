#include <common.h>
#include <command.h>

#if defined(CONFIG_CMD_KGDB)

#include <kgdb.h>
#include <asm/signal.h>
#include <asm/processor.h>
#include <asm/arcregs.h>
//#include "kgdb.h"
typedef struct __soft_break_table{
    unsigned addr;
    unsigned short orig;
    unsigned short used;
}SoftBreakTable;
SoftBreakTable brk_table[256];
#define PC_REGNUM ARC_PC_REGNUM
#define SP_REGNUM ARC_SP_REGNUM
#define SPACE_REQUIRED	(ARC_NR_REGS*4)
void breakinst(void);
    

int
kgdb_setjmp(long *buf)
{
	 __asm__ __volatile__ ( "add_s sp,sp,8");
    __asm__ __volatile__ ( "j handle_setjmp");
	/* XXX should save fp regs as well */
	return 0;
}

void
kgdb_longjmp(long *buf, int val)
{
	 handle_longjmp(buf,val?val:1);
}



/* Convert the SPARC hardware trap type code to a unix signal number. */
/*
 * This table contains the mapping between PowerPC hardware trap types, and
 * signals, which are primarily what GDB understands.
 */
static struct hard_trap_info
{
	unsigned int tt;		/* Trap type code for powerpc */
	unsigned int mask;
	unsigned char signo;		/* Signal that we map this trap into */
} hard_trap_info[] = {
	{ 0x200000,0xff0000, SIGSEGV },			/* machine check */
//	{ 0x300, SIGSEGV },			/* address error (store) */
	{ 0x020000,0xff0000, SIGBUS },			/* instruction bus error */
//	{ 0x500, SIGINT },			/* interrupt */
	{ 0x230400,0xffff00, SIGBUS },			/* alingment */
	{ 0x250000,0xffffff, SIGTRAP },			/* breakpoint trap */
	{ 0x240200,0xffff00, SIGTRAP },			/* breakpoint trap */
//	{ 0x800, SIGFPE },			/* fpu unavail */
//	{ 0x900, SIGALRM },			/* decrementer */
//	{ 0xa00, SIGILL },			/* reserved */
//	{ 0xb00, SIGILL },			/* reserved */
//	{ 0xc00, SIGCHLD },			/* syscall */
//	{ 0xd00, SIGTRAP },			/* single-step/watch */
//	{ 0xe00, SIGFPE },			/* fp assist */
	{ 0, 0}				/* Must be last */
};

static int
computeSignal(unsigned int tt)
{
	struct hard_trap_info *ht;

	for (ht = hard_trap_info; ht->tt && ht->signo; ht++)
		if (ht->tt == (tt&ht->mask))
			return ht->signo;
	return SIGHUP;         /* default for things we don't know about */
}

void
kgdb_enter(struct pt_regs *regs, kgdb_data *kdp)
{
	unsigned long trap_addr;
    int ret=0;
//	kdp->private[0] = msr = get_msr();
    
	disable_interrupts();
    int vec_num=0;
    int i;
    int status=read_new_aux_reg(ARC_REG_STATUS32);
    if((status&(STATUS_A1_MASK|STATUS_A2_MASK|STATUS_AE_MASK))==0)
        while(1); // wrong status 
    if((status&STATUS_AE_MASK)==0)
    {
        write_new_aux_reg(ARC_REG_TIMER1_CTRL ,0xb);
        if(kgdb_serial_tstc())
        {
            if(getDebugChar()==0x3)
            {
                write_new_aux_reg(ARC_REG_AP_AMV0,regs->ret);
                write_new_aux_reg(ARC_REG_AP_AMM0,0);
                write_new_aux_reg(ARC_REG_AP_AC0,ARC_REG_AP_AC_AAM|AP_Inst_Addr|(2<<ARC_REG_AP_AC_TTOff));
            }
    	}
        return ;
    }
    write_new_aux_reg(ARC_REG_TIMER1_CTRL ,0xA);
        
	/* reply to host that an exception has occurred */
	kdp->private[0]=kgdb_trap(regs);
	kdp->sigval = computeSignal(kdp->private[0]);
	trap_addr=read_new_aux_reg(ARC_REG_AP_AMV0);
	
	for(i=0;i<sizeof(brk_table)/sizeof(SoftBreakTable);i++)
    {
        if(brk_table[i].used)
        {
            volatile unsigned short * d=(volatile unsigned short *)brk_table[i].addr;
            if(*d==brk_table[i].orig)
            {
                *d=0x781e;
            }
            if(brk_table[i].addr==trap_addr&&kdp->private[0]==0x240201)
                ret=1;
            if(kdp->private[0]==0x250000&&brk_table[i].addr==regs->ret-2)
            {
                regs->ret-=2;
            }
            
        }
    } 
    kgdb_flush_cache_all();

	kdp->nregs = 2;

	kdp->regs[0].num = PC_REGNUM;
	kdp->regs[0].val = regs->ret;

	kdp->regs[1].num = SP_REGNUM;
	kdp->regs[1].val = regs->sp;
	write_new_aux_reg(ARC_REG_AP_AC0,ARC_REG_AP_AC_AAM|AP_Inst_Addr|(1<<ARC_REG_AP_AC_MOff)|(0<<ARC_REG_AP_AC_TTOff));
	return ;
}

void
kgdb_exit(struct pt_regs *regs, kgdb_data *kdp)
{
	unsigned long reason = kdp->private[0];
	unsigned long trap_addr=regs->ret;
	int i;
    write_new_aux_reg(ARC_REG_TIMER1_CTRL ,0xb);
	if (kdp->extype & KGDBEXIT_WITHADDR)
		regs->ret = kdp->exaddr;
    if(reason==0x250000)
    {
        for(i=0;i<sizeof(brk_table)/sizeof(SoftBreakTable);i++)
        {
            if(brk_table[i].used&&trap_addr==brk_table[i].addr)
            {
                volatile unsigned short * d=(volatile unsigned short *)trap_addr;
                *d=brk_table[i].orig;
                break;
            }
        }
    }
	switch (kdp->extype & KGDBEXIT_TYPEMASK) {
	    case KGDBEXIT_KILL:
	        break;
	    case KGDBEXIT_CONTINUE:
	        if(reason!=0x250000)
		        break;
		    if(i>=sizeof(brk_table)/sizeof(SoftBreakTable))
		        break;
	    case KGDBEXIT_SINGLE:
            write_new_aux_reg(ARC_REG_AP_AMV0,regs->ret);
            write_new_aux_reg(ARC_REG_AP_AMM0,0);
            write_new_aux_reg(ARC_REG_AP_AC0,ARC_REG_AP_AC_AAM|AP_Inst_Addr|(1<<ARC_REG_AP_AC_MOff)|(2<<ARC_REG_AP_AC_TTOff));
		    break;
	}
}

int
kgdb_trap(struct pt_regs *regs)
{
    int status=read_new_aux_reg(ARC_REG_STATUS32);
    if(status&STATUS_AE_MASK)
        return  read_new_aux_reg(ARC_REG_ECR);
    return ((read_new_aux_reg(AUX_ICAUSE2))&0x1f)<<16;
	
}

/* return the value of the CPU registers.
 * some of them are non-PowerPC names :(
 * they are stored in gdb like:
 * struct {
 *     u32 gpr[32];
 *     f64 fpr[32];
 *     u32 pc, ps, cnd, lr; (ps=msr)
 *     u32 cnt, xer, mq;
 * }
 */

static int limit_temp=0x4e;
int kgdb_arc_getreg(struct pt_regs *regs, int gdbno,unsigned long * val)
{
    int i;
    if(gdbno>limit_temp)
        i=3;
    /** register mapping for arc gdb**/
    switch(gdbno){
#define RW 1
#define RO 1
#define WO 0
#define UU 1        
#define RAUX(name, hwregno, desc, gdbregno, mask, mode, version) case gdbregno:if(mode)*val=read_new_aux_reg(hwregno);else *val=0;break;
#define RBCR(name, hwregno, desc, gdbregno, mask, mode, version) case gdbregno:if(mode)*val=read_new_aux_reg(hwregno);else *val=0;break;
#define RPTRACE_REG(name,pt_reg,desc,gdbregno,mode)              case gdbregno: *val=regs->pt_reg    ;break;
#define RAW_REG(name,hwregno,desc,gdbregno,mode)              case gdbregno: *val=__builtin_arc_core_read(hwregno);break;
    #include "kgdb_map.h"
#undef RPTRACE_REG
#undef RAW_REG    
#undef RAUX
#undef RBCR    
#undef RW
#undef RO
#undef WO
#undef UU
    default:
        return -1;
    }
    return 0;
}
int kgdb_arc_putreg(struct pt_regs *regs, int gdbno, unsigned long val)
{
        switch(gdbno){
#define RW 1  
#define RO 0  
#define WO 1  
#define UU 0  
            
#define RAUX(name, hwregno, desc, gdbregno, mask, mode, version) case gdbregno:if(mode)write_new_aux_reg(hwregno,val);else return -1;break;
#define RBCR(name, hwregno, desc, gdbregno, mask, mode, version) case gdbregno:if(mode)write_new_aux_reg(hwregno,val);else return -1;break;
#define RPTRACE_REG(name,pt_reg,desc,gdbregno,mode)              case gdbregno: regs->pt_reg=val    ;break;
#define RAW_REG(name,hwregno,desc,gdbregno,mode)              case gdbregno: __builtin_arc_core_write(val,hwregno);break;

    #include "kgdb_map.h"
    
#undef RPTRACE_REG
#undef RAW_REG    
#undef RAUX
#undef RBCR
#undef RW
#undef RO
#undef WO
#undef UU    
        default:
            return -1;
        }
        return 0;
}
int
kgdb_getregs(struct pt_regs *regs, char *buf, int max)
{
	int i;
	unsigned long *ptr = (unsigned long *)buf;
   if (max < SPACE_REQUIRED)
		kgdb_error(KGDBERR_NOSPACE);

	if ((unsigned long)ptr & 3)
		kgdb_error(KGDBERR_ALIGNFAULT);
    for(i=0;i<ARC_NR_REGS;i++,ptr++)
    {
        if(kgdb_arc_getreg(regs,i,ptr))
            *ptr=0;
    }
	return (SPACE_REQUIRED);
}

int kgdb_getreg(struct pt_regs *regs, int regno, char *buf, int length)
{
    unsigned long *ptr = (unsigned long *)buf;
    if(kgdb_arc_getreg(regs,regno,ptr))kgdb_error(KGDBERR_BADPARAMS);
    return 4;
}

void
kgdb_putreg(struct pt_regs *regs, int regno, char *buf, int length)
{
	unsigned long *ptr = (unsigned long *)buf;

	if ((unsigned long)ptr & 3)
		kgdb_error(KGDBERR_ALIGNFAULT);
/** register mapping for arc gdb**/
    
    if(kgdb_arc_putreg(regs,regno,*ptr))kgdb_error(KGDBERR_BADPARAMS);
}

void
kgdb_putregs(struct pt_regs *regs, char *buf, int length)
{
	int i;
	unsigned long *ptr = (unsigned long *)buf;

	if (length < SPACE_REQUIRED)
		kgdb_error(KGDBERR_NOSPACE);

	if ((unsigned long)ptr & 3)
		kgdb_error(KGDBERR_ALIGNFAULT);

    for(i=0;i<ARC_NR_REGS;i++,ptr++)
    {
        kgdb_arc_putreg(regs,i,*ptr);
    }
}
void arch_kgdb_set_sw_break(struct kgdb_bkpt *bkpt)
{
    BUG();
}
void arch_kgdb_remove_sw_break(struct kgdb_bkpt *bkpt)
{
    BUG();
}
int 
kgdb_set_breakpoint(struct pt_regs * regs,int type,unsigned addr, unsigned length)
{
    int i;
    switch(type)
    {
        case 0:
           for(i=0;i<sizeof(brk_table)/sizeof(SoftBreakTable);i++)
           {
                if(brk_table[i].used==0)
                {
                    brk_table[i].addr=addr;
                    volatile unsigned short * d=(volatile unsigned short*)addr;
                    brk_table[i].orig=*d;
                    *d=0x781e;
                    brk_table[i].used=1;
                    flush_cache(0,0);
                    return 0;
                }
           }
           break;
    }
    return 1;
} 
int 
kgdb_remove_breakpoint(struct pt_regs * regs,int type,unsigned addr, unsigned length)
{
    int i;
    switch(type)
    {
        case 0:
           for(i=0;i<sizeof(brk_table)/sizeof(SoftBreakTable);i++)
            {
                if(brk_table[i].used&&brk_table[i].addr==addr)
                {
                    volatile unsigned short * d=(volatile unsigned short *)addr;
                    *d=brk_table[i].orig;
                    brk_table[i].used=0;
                    flush_cache(0,0);
                    return 0;
                }
            }
           break;
    }
    return 1;
}

/* This function will generate a breakpoint exception.  It is used at the
   beginning of a program to sync up with a debugger and can be used
   otherwise as a quick means to stop program execution and "break" into
   the debugger. */

void
kgdb_breakpoint(int argc, char *argv[])
{
	__asm__ __volatile__ ( "trap_s 0");
}
void kgdb_flush_cache_range(void * a, void * b){ flush_cache(0,0);}
void kgdb_flush_cache_all(void){ flush_cache(0,0);}

#endif







unsigned ap_list[8];
int test_func(int i)
{
    return ap_list[i&7];
}
void kgdb_test()
{
//    int i=0;
//    kgdb_breakpoint(0,NULL);
    int i=0;
    while(1){
        test_func(i++);
    }
}