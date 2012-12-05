#include <common.h>
#include <upgrade.h>
#include <asm/arch/am_regs.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/gpio.h>

inline int is_ac_connected(void)
{
	return axp_charger_is_ac_online();
}


static void set_charging_mode(int flags)
{
	return;	//Elvis Fool
}

static int get_charging_status()
{
	//axp_charger_state_dump();
	return axp_charger_get_charging_status();
}

static int get_charging_percentage(void)
{
	return axp_charger_get_charging_percent();
}

inline int any_key_pressed()
{
    int adc_val = get_adc_sample(4);
    //printf("get_adc_sample(4): 0x%x\n", adc_val);
    // no key pressed 0x3fd
    //return((((adc_val >= 0x0) && (adc_val < 0x3c0)) | powerkey_scan()) ? 1 : 0);
    return(((adc_val >= 0x0) && (adc_val < 0x3c0)) ? 1 : 0);
}

#define bat_pic_x   480
#define bat_pic_y    226
#define bat_pic_num   4
#define bat_pic_start   0x4000000
#define bat_pic_size    0x100000

int battery_charging(void)
{
    char	str[128];
    int i = 0, j = 5, first_up = 1;
    int sleep_val = 0;
    int bmp_cnt = 0;
    int tmp=-1, charging_percentage=-2;
#ifdef	CONFIG_UBI_SUPPORT    
    run_command ("ubi part system", 0);    
    run_command ("ubifsmount system", 0);
    //run_command ("ubifsls", 0);
#endif
    set_charging_mode(1);
    while(1)
    {
        if(!is_ac_connected())
        {
            printf("Ac adapter removed!!!\n");
            power_down();
            printf("Power Down!\n");
            hang();
        }
        if(powerkey_scan() || any_key_pressed())
        {
			if(sleep_val >= 7)
            {
                axp_charger_process(1) ;
                run_command("lcd bl on", 0);
            }
            sleep_val = 0;
        }
        if(powerkey_hold(1000))
        {
            return  0;
        } 

		if(j >= 5)
		{
			if(j >= 5) j = 0;
	        if(sleep_val++ < 7)
		    {
		        if(get_charging_status()&&(get_charging_percentage()==100))
		        {
#ifdef	CONFIG_UBI_SUPPORT
		            sprintf(str, "ubifsload ${loadaddr} resource/battery_pic/G33.bmp");
#else
		            sprintf(str, "nand read ${loadaddr} %x %x",
		                bat_pic_start+bat_pic_size*(bat_pic_num-1), bat_pic_size);
#endif
		            run_command(str, 0);
		            sprintf(str, "bmp display ${loadaddr} %d %d", bat_pic_x, bat_pic_y);
		            if(run_command(str, 0))
		        	{
		//						AsciiPrintf("Battery Charging...", 260, 200, 0x00ff00);
		//						AsciiPrintf("Press POWER will plug into the ANDROID world!", 130, 240, 0x00ff00);
					}
		            if(first_up)
		            {
		                axp_charger_process(1) ;
		                run_command ("lcd bl on", 0);
		                first_up = 0;
		            }
		            if(tmp != 100)
		            {
		                tmp = charging_percentage = 100;
			            sprintf(str,"%3d%",charging_percentage);
			            AsciiPrintf(str, 640, 322, 0xffffff);
		            }
		        }
		        else
			    {
			        charging_percentage = get_charging_percentage();
			        if(charging_percentage<15) {
			            bmp_cnt = 0;
			        }
			        else if(charging_percentage<60) {
			            bmp_cnt = 1;
			        }
			        else if(charging_percentage<90) {
			            bmp_cnt = 2;
			        }
			        else {
			            bmp_cnt = 3;
			        }
			        
#ifdef	CONFIG_UBI_SUPPORT
			        sprintf(str, "ubifsload ${loadaddr} resource/battery_pic/G%d%d.bmp", bmp_cnt,i);
#else
			        sprintf(str, "nand read ${loadaddr} %x %x",
			        bat_pic_start+bat_pic_size*bmp_cnt, bat_pic_size);
#endif
			        run_command(str, 0);
			        sprintf(str, "bmp display ${loadaddr} %d %d", bat_pic_x, bat_pic_y);
			        if(run_command(str, 0))
			    	{
			//						AsciiPrintf("Battery Charging...", 260, 200, 0x00ff00);
			//						AsciiPrintf("Press POWER will plug into the ANDROID world!", 130, 240, 0x00ff00);
					}
			        if(first_up)
			        {
			            axp_charger_process(1) ;
			            run_command ("lcd bl on", 0);
			            first_up = 0;
			        }
			        
			        i = (i+1)%bat_pic_num;
					
			        charging_percentage = get_charging_percentage();
			        //if(tmp != charging_percentage)
			        {
			            tmp = charging_percentage;
			            sprintf(str,"%3d%",charging_percentage);
			            AsciiPrintf(str, 640, 322, 0xffffff);
			        }
			    }
		        if(sleep_val >= 7)
		        {
		            tmp = -1;
		            run_command("lcd bl off", 0);
		            axp_charger_process(0) ;
		            if(enter_boot_power_down()) 
		            {
                        axp_charger_process(1) ;
                        run_command("lcd bl on", 0);
                        sleep_val = 0;
		            }
		        }
		    }
		}
		if(tick_delay(50))
        {
            return  1;
        }
		j++;
    }
}

void power_low_display(void)
{
    char	str[128];
	printf("power_low_display\n");
#ifdef	CONFIG_UBI_SUPPORT
    run_command ("ubi part system", 0);    
    run_command ("ubifsmount system", 0);
    //run_command ("ubifsls", 0);
    sprintf(str, "ubifsload ${loadaddr} resource/battery_pic/Glow.bmp");
#else
    sprintf(str, "nand read ${loadaddr} %x %x",
        bat_pic_start+bat_pic_size*bat_pic_num, bat_pic_size);
#endif
    run_command(str, 0);
    sprintf(str, "bmp display ${loadaddr} %d %d", bat_pic_x, bat_pic_y);
    run_command(str, 0);
    axp_charger_process(1) ;
    run_command ("lcd bl on", 0);
}
