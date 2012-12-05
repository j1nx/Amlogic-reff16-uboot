#include <common.h>
#include <asm/arch/am_reg_addr.h>
static ulong timer_base;

#ifdef TEST_UBOOT_BOOT_SPEND_TIME
extern int start_arm_mid1,start_arm_mid2;
#endif
int timer_init(void)
{
    #ifdef TEST_UBOOT_BOOT_SPEND_TIME
    start_arm_mid1 = TIMERE_GET();
    #endif
    /*
        Select Timer D us Base 
        Select Timer E 100 us base
    */

#if CONFIG_SYS_HZ==1000000 //1Mhz
	clrsetbits_le32(P_ISA_TIMER_MUX,0x7<<8,0x1<<8);//timer e is 1us base
#elif CONFIG_SYS_HZ==1000 //1khz
		//: use 1uS time base to control 1 ms. fix problem for "loady" command
//    clrsetbits_le32(P_ISA_TIMER_MUX,0x7<<8,0x4<<8);//timer e is 1ms base
	  clrsetbits_le32(P_ISA_TIMER_MUX,0x7<<8,0x1<<8);//timer e is 1us base
    setbits_le32(P_ISA_TIMER_MUX,0x1<<19);         //enable timer D , 1us base 
#else
    #error CONFIG_SYS_HZ must be 1000 or 1000000
#endif

    reset_timer();
    
    #ifdef TEST_UBOOT_BOOT_SPEND_TIME
    start_arm_mid2 = TIMERE_GET();
    #endif

    return 0;
}
/*
 * timer without interrupts
 */

void reset_timer(void)
{
    set_timer(0);
}
#if 0 //solution 0
ulong get_timer(ulong base)
{
    return (readl(P_ISA_TIMERE))+timer_base-base;
}

void set_timer(ulong t)
{
    timer_base=t;
    writel(0,P_ISA_TIMERE);
#if   CONFIG_SYS_HZ==1000 
    writel(1000,P_ISA_TIMERD);
#endif    
}
#if   CONFIG_SYS_HZ==1000 
ulong get_utimer(ulong ubase)
{
   return (get_timer(0)+1)*1000-(readl(P_ISA_TIMERD)>>16)-ubase;
}
#else

#define get_utimer get_timer
#endif
void __udelay(unsigned long usec)
{
    ulong base;
    base=get_utimer(0);
    while(get_utimer(base)<usec);
    
}
#else //solution 1
//: use 1uS time base to control 1 ms. fix problem for "loady" command
ulong get_utimer(ulong ubase)
{
    return (readl(P_ISA_TIMERE))+timer_base-ubase;
}
void set_timer(ulong t)
{
    timer_base=t;
    writel(0,P_ISA_TIMERE);
#if   CONFIG_SYS_HZ==1000 
    writel(1000,P_ISA_TIMERD);
#endif    
}

void __udelay(unsigned long usec)
{
    ulong base;
    base=get_utimer(0);
    while(get_utimer(base)<usec);
    
}
#if CONFIG_SYS_HZ == 1000
ulong get_timer(ulong base)
{
	 return((get_utimer(0)+1) / 1000 - base);
}
#else
#define get_timer get_utimer
#endif
#endif