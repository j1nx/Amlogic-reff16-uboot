#include <common.h>
#include <asm/arch/am_regs.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/gpio.h>

static int bat_value_table[37]={
0,  //0    
620,//0  
623,//4  
650,//10 
675,//15 
680,//16 
685,//18 
690,//20 
693,//23 
696,//26 
699,//29 
709,//32 
716,//35 
725,//37 
735,//40 
745,//43 
751,//46 
753,//49 
755,//51 
757,//54 
758,//57 
759,//60 
762,//63 
767,//66 
772,//68 
778,//71 
784,//74 
791,//77 
797,//80 
803,//83 
811,//85 
820,//88 
827,//91 
838,//95 
845,//97 
850,//100
852 //100
};

static int bat_charge_value_table[37]={
0,  //0    
620,//0
675,//4
689,//10
700,//15
709,//16
713,//18
718,//20
723,//23
729,//26
731,//29
732,//32
734,//35
735,//37
737,//40
739,//43
741,//46
745,//49 -- need test
750,//51
754,//54
761,//57
768,//60
775,//63
780,//66
783,//68
788,//71
793,//74
812,//77
819,//80
828,//83
838,//85
840,//88
847,//91
852,//95 -- need test
855,//97
865,//100
868 //100
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

static int new_bat_value_table[37]={
0,  //0    
3500000,//0  
3518899,//4  
3549407,//10 
3573985,//15 
3591305,//16 
3632629,//18 
3665652,//20 
3698675,//23 
3727997,//26 
3743844,//29 
3752432,//32 
3759835,//35   
3764722,//37 
3768423,//40 
3782940,//43 
3789157,//46 
3803818,//49 
3817293,//51 
3830768,//54 
3851502,//57 
3869864,//60 
3888226,//63 
3911475,//66 
3938282,//68 
3954272,//71 
3966705,//74 
4003142,//77 
4036165,//80 
4060600,//83 
4083849,//85 
4108389,//88 
4124236,//91 
4137711,//95 
4141257,//97     4145257   
4145257,//100    4170734
4200000 //100
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

/*
 *	Charging Status Indication
 *
 *	CSTATE[1]	CSTATE[0]	STATE MACHINE STATUS
 *
 *		1			1		PRECONDITION State
 *		1			0		FAST-CHARGE / TOP-OFF State
 *		0			1		END-OF-CHARGE State
 *		0			0		SUSPEND/DISABLED / FAULT State
 *
 */
static int get_charging_status()
{
	int status;
	status = pmu_get_charge_status();
	//printf("charging status %d \n", status);
	return status;
}

static inline int get_bat_percentage(int adc_vaule, int *adc_table, 
										int *per_table, int table_size)
{
	int i;
	
	for(i=0; i<(table_size - 1); i++) {
		if ((adc_vaule > adc_table[i]) && (adc_vaule <= adc_table[i+1]))
			break;
	}
	//printf("per_table[%d] %d\n", i, per_table[i]);
	return per_table[i];
}

static inline int get_bat_adc_value()
{
    return get_adc_sample(5);
}

static inline int _measure_current(void)
{
	int val, Vgnd, Vbatn, vdiff;
	set_gpio_mode(GPIOA_bank_bit0_27(22), GPIOA_bit_bit0_27(22), GPIO_OUTPUT_MODE);
	set_gpio_val(GPIOA_bank_bit0_27(22), GPIOA_bit_bit0_27(22), 1);
	
	udelay(100); 
	Vbatn = get_adc_sample(7) * (2500000 / 1023);
	
	set_gpio_mode(GPIOA_bank_bit0_27(22), GPIOA_bit_bit0_27(22), GPIO_OUTPUT_MODE);
	set_gpio_val(GPIOA_bank_bit0_27(22), GPIOA_bit_bit0_27(22), 0);
	udelay(5000);
	Vgnd = get_adc_sample(7) * (2500000 / 1023);
	
	vdiff = Vbatn - Vgnd;
	val = (vdiff*1047)/(110*2);
	
	//printf("%s Vbatn:%duV Vgnd:%duV vdiff:%duV I:%duA. ", (vdiff>0)?"charging...":"uncharging...", Vbatn, Vgnd, vdiff, val);
	return val;
	
}

static inline int _measure_voltage(void)
{
	int val, Vbat, Icur;

	val = get_bat_adc_value();
	val =  val * (2 * 2500000 / 1023);

	Icur = _measure_current();
	Vbat = val - Icur*102/1000;  // 0.102
	//printf("v:%d vbat:%d\n", val, Vbat);
	return Vbat;
}

static int get_charging_percentage(void)
{
	int adc = _measure_voltage();
	int table_size = sizeof(new_bat_value_table)/sizeof(new_bat_value_table[0]);
	
	return get_bat_percentage(adc, new_bat_value_table, bat_level_table, table_size);
}

int get_battery_percentage(void)
{
	int adc = _measure_voltage();
	int table_size = sizeof(new_bat_value_table)/sizeof(new_bat_value_table[0]);
	
	return get_bat_percentage(adc, new_bat_value_table, bat_level_table, table_size);
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
		        if(get_charging_status() || (100 == get_charging_percentage()))
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
						//AsciiPrintf("Battery Charging...", 260, 200, 0x00ff00);
						//AsciiPrintf("Press POWER will plug into the ANDROID world!", 130, 240, 0x00ff00);
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
						//AsciiPrintf("Battery Charging...", 260, 200, 0x00ff00);
						//AsciiPrintf("Press POWER will plug into the ANDROID world!", 130, 240, 0x00ff00);
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
