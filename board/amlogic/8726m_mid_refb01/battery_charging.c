#include <common.h>
#include <asm/arch/am_regs.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/gpio.h>


#define is_ac_connected ((READ_CBUS_REG(ASSIST_HW_REV)&(1<<9))? 1:0)
static int bat_value_table[36]={
0,  //0    
700,//0
726,//4
732,//10
737,//15
740,//18
743,//20
745,//23
747,//26
748,//29
750,//32
752,//35
753,//37
754,//40
755,//43
757,//46
758,//49
760,//51
762,//54
765,//57
768,//60
771,//63
773,//66
776,//68
779,//71
782,//74
785,//77
788,//80
791,//83
794,//85
796,//88
799,//91
803,//95
806,//97
814,//100
814 //100
};

static int bat_charge_value_table[36]={
0,  //0    
732,//0
749,//4
755,//10
759,//15
762,//18
765,//20
767,//23
769,//26
771,//29
773,//32
775,//35
777,//37
780,//40
781,//43
782,//46
783,//49
784,//51
785,//54
788,//57
791,//60
793,//63
795,//66
800,//68
804,//71
806,//74
809,//77
813,//80
815,//83
818,//85
820,//88
823,//91
826,//95
829,//97
830,//100
830 //100
};

static int bat_level_table[36]={
0,
0,
4,
10,
15,
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

static void set_charging_mode(int flags)
{
    //GPIOD_22 low: fast charge high: slow charge
    CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7, (1<<18));
    if(flags == 1)
    {
        set_gpio_val(GPIOD_bank_bit2_24(22), GPIOD_bit_bit2_24(22), 0); //fast charge
    }
    else
    {
        set_gpio_val(GPIOD_bank_bit2_24(22), GPIOD_bit_bit2_24(22), 1);	//slow charge
    }
    set_gpio_mode(GPIOD_bank_bit2_24(22), GPIOD_bit_bit2_24(22), GPIO_OUTPUT_MODE);
}

static inline int get_bat_vol(void)
{
    return get_adc_sample(5);
}

static inline int get_charging_status()
{
    return (READ_CBUS_REG(ASSIST_HW_REV)&(1<<8))? 1:0;
}

int get_charging_percentage(void)
{
    int adc_val, i;
    adc_val = get_bat_vol();
    //printf("adc_val: %d\n", adc_val);
    for(i=0; i<36; i++)
    {
        if(adc_val <= bat_charge_value_table[i])
        {
            return(bat_level_table[i]);
        }
    }
    return  100;
}

int get_battery_percentage(void)
{
    int adc_val, i;
    adc_val = get_bat_vol();
    printf("adc_val: %d\n", adc_val);
    for(i=0; i<36; i++)
    {
        if(adc_val <= bat_value_table[i])
        {
            return(bat_level_table[i]);
        }
    }
    return  100;
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

    for(i=0; i<26; i++)
    {
        lcd_printf("\n");
    }
    for(i=0; i<52; i++)
    {
        lcd_printf(" ");
    }
    i = 0;
    while(1)
    {
        if(!is_ac_connected)
        {
            printf("Ac adapter removed!!!\n");
            power_unhold();
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
//									AsciiPrintf("Battery Charging...", 260, 200, 0x00ff00);
//									AsciiPrintf("Press POWER will plug into the ANDROID world!", 130, 240, 0x00ff00);
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
//										AsciiPrintf("Battery Charging...", 260, 200, 0x00ff00);
//										AsciiPrintf("Press POWER will plug into the ANDROID world!", 130, 240, 0x00ff00);
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

