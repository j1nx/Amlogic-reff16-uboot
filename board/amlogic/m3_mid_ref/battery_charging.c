#include <common.h>
#include <asm/arch/am_regs.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/gpio.h>

static int bat_value_table[37]={
0,  //0    
737,//0
742,//4
750,//10
752,//15
753,//16
754,//18
755,//20
756,//23
757,//26
758,//29
760,//32
761,//35
762,//37
763,//40
766,//43
768,//46
770,//49
772,//51
775,//54
778,//57
781,//60
786,//63
788,//66
791,//68
795,//71
798,//74
800,//77
806,//80
810,//83
812,//85
817,//88
823,//91
828,//95
832,//97
835,//100
835 //100
};

static int bat_charge_value_table[37]={
0,  //0    
766,//0
773,//4
779,//10
780,//15
781,//16
782,//18
783,//20
784,//23
785,//26
786,//29
787,//32
788,//35
789,//37
790,//40
791,//43
792,//46
794,//49
796,//51
798,//54
802,//57
804,//60
807,//63
809,//66
810,//68
813,//71
815,//74
818,//77
820,//80
823,//83
825,//85
828,//88
831,//91
836,//95
839,//97
842,//100
842 //100
};

static int bat_level_table[37]={
0,
0,
4,
10,
15,
16,
18,
20,
23,
26,
29,
32,
35,
37,
40,
43,
46,
49,
51,
54,
57,
60,
63,
66,
68,
71,
74,
77,
80,
83,
85,
88,
91,
95,
97,
100,
100   
};

static int is_ac_connected(void)
{
	return pmu_is_ac_online();
}

static void set_charging_mode(int flags)
{
	pmu_set_charge_current(flags);
}

static int get_bat_vol(void)
{
    return pmu_measure_voltage();
}

static int get_charging_status()
{
    return pmu_get_charge_status();
}

static inline int get_bat_percentage(int adc_vaule, int *adc_table, 
										int *per_table, int table_size)
{
	int i;
	
	for(i=0; i<(table_size - 1); i++) {
		if ((adc_vaule > adc_table[i]) && (adc_vaule <= adc_table[i+1]))
			break;
	}
//	printf("%s: adc_vaule=%d, i=%d, per_table[i]=%d \n", 
//				__FUNCTION__, adc_vaule, i, per_table[i]);

	return per_table[i];
}

static inline int get_bat_adc_value()
{
    return get_adc_sample(5);
}

static int get_charging_percentage(void)
{
	int adc = get_bat_adc_value();
	int table_size = sizeof(bat_charge_value_table)/sizeof(bat_charge_value_table[0]);
	
	return get_bat_percentage(adc, bat_charge_value_table, bat_level_table, table_size);
}

int get_battery_percentage(void)
{
	int adc = get_bat_adc_value();
	int table_size = sizeof(bat_value_table)/sizeof(bat_value_table[0]);
	
	return get_bat_percentage(adc, bat_value_table, bat_level_table, table_size);
}

inline int any_key_pressed()
{
    int adc_val = get_adc_sample(4);
    //printf("get_adc_sample(4): 0x%x\n", adc_val);
    // no key pressed 0x3fd
    //return((((adc_val >= 0x0) && (adc_val < 0x3c0)) | powerkey_scan()) ? 1 : 0);
    return(((adc_val >= 0x0) && (adc_val < 0x3c0)) ? 1 : 0);
}

#define bat_pic_x   230
#define bat_pic_y    100
#define bat_pic_num   5
#define bat_pic_start   0x4000000
#define bat_pic_size    0x100000

int battery_charging(void)
{
    char	str[128];
    int i = 0, j = 5, first_up = 1;
    int sleep_val = 0;
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
		        if(get_charging_status())
		        {
#ifdef	CONFIG_UBI_SUPPORT
		            sprintf(str, "ubifsload ${loadaddr} resource/battery_pic/%d.bmp", (bat_pic_num-1));
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
		                run_command ("lcd bl on", 0);
		                first_up = 0;
		            }
		            if(tmp != 100)
		            {
		                tmp = charging_percentage = 100;
		                //lcd_printf("\b\b\b\b%3d% ", charging_percentage);
		            }
		        }
		        else
			    {
#ifdef	CONFIG_UBI_SUPPORT
			        sprintf(str, "ubifsload ${loadaddr} resource/battery_pic/%d.bmp", i);
#else
			        sprintf(str, "nand read ${loadaddr} %x %x",
			        bat_pic_start+bat_pic_size*i, bat_pic_size);
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
			            run_command ("lcd bl on", 0);
			            first_up = 0;
			        }
			        
			        i = (i+1)%bat_pic_num;

			        charging_percentage = get_charging_percentage();
			        if(tmp != charging_percentage)
			        {
			            tmp = charging_percentage;
			            //lcd_printf("\b\b\b\b%3d% ", charging_percentage);
			        }
			    }
		        if(sleep_val >= 7)
		        {
		            tmp = -1;
		            run_command("lcd bl off", 0);
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
    sprintf(str, "ubifsload ${loadaddr} resource/battery_pic/power_low.bmp");
#else
    sprintf(str, "nand read ${loadaddr} %x %x",
        bat_pic_start+bat_pic_size*bat_pic_num, bat_pic_size);
#endif
    run_command(str, 0);
    sprintf(str, "bmp display ${loadaddr} %d %d", bat_pic_x, bat_pic_y);
    run_command(str, 0);
    run_command ("lcd bl on", 0);
}
