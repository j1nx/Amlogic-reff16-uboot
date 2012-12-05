
#include "platform.h"
#include "usb_pcd.h"

#include "usb_pcd.c"
#include "platform.c"
#include "dwc_pcd.c"
#include "dwc_pcd_irq.c"
#include "burn_func.c"

#include <asm/arch/sram.h>

#define reboot_mode *((volatile unsigned long*)(0xC9000000 + REBOOT_MODE_OFFSET))

/*
 * Commands accepted by the arm_machine_restart() system call.
 *
 * AMLOGIC_NORMAL_BOOT     			Restart system normally.
 * AMLOGIC_FACTORY_RESET_REBOOT      Restart system into recovery factory reset.
 * AMLOGIC_UPDATE_REBOOT			Restart system into recovery update.
 * AMLOGIC_CHARGING_REBOOT     		Restart system into charging.
 * AMLOGIC_CRASH_REBOOT   			Restart system with system crach.
 * AMLOGIC_FACTORY_TEST_REBOOT    	Restart system into factory test.
 * AMLOGIC_SYSTEM_SWITCH_REBOOT  	Restart system for switch other OS.
 * AMLOGIC_SAFE_REBOOT       			Restart system into safe mode.
 * AMLOGIC_LOCK_REBOOT  			Restart system into lock mode.
 * elvis.yu---elvis.yu@amlogic.com
 */
#define	AMLOGIC_NORMAL_BOOT					0x0
#define	AMLOGIC_FACTORY_RESET_REBOOT		0x01010101
#define	AMLOGIC_UPDATE_REBOOT				0x02020202
#define	AMLOGIC_CHARGING_REBOOT				0x03030303
#define	AMLOGIC_CRASH_REBOOT				0x04040404
#define	AMLOGIC_FACTORY_TEST_REBOOT		0x05050505
#define	AMLOGIC_SYSTEM_SWITCH_REBOOT		0x06060606
#define	AMLOGIC_SAFE_REBOOT					0x07070707
#define	AMLOGIC_LOCK_REBOOT					0x08080808
#define	MESON_USB_BURNER_REBOOT			0x09090909
#define	AMLOGIC_REBOOT_CLEAR					0xdeaddead


int usb_boot(int clk_cfg)
{
	int cfg = INT_CLOCK;
	if(clk_cfg)
		cfg = EXT_CLOCK;
	set_usb_phy_config(cfg);

	usb_parameter_init();
		
	if(usb_pcd_init())
		return 0;

	while(1)
	{
		//watchdog_clear();		//Elvis Fool
		if(usb_pcd_irq())
			break;
	}
	return 0;
}

void relocate_init(unsigned __TEXT_BASE,unsigned __TEXT_SIZE)
{
	writel(0,P_WATCHDOG_TC);//disable Watchdog
	reboot_mode = MESON_USB_BURNER_REBOOT;
	while(1)
	{
		//usb_boot(0);	//Elvis
		usb_boot(1);
	}
}


