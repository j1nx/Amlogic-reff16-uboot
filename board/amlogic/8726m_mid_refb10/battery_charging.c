#include <common.h>
#include <asm/arch/am_regs.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/gpio.h>


#define is_ac_connected ((READ_CBUS_REG(ASSIST_HW_REV)&(1<<9))? 1:0)
static int bat_value_table[37]={
0,  //0    
710,//0
715,//5
725,//10
727,//13
731,//16
733,//18
734,//20
736,//23
737,//26
739,//29
741,//32
742,//35
744,//37
745,//40
747,//43
749,//46
751,//49
753,//51
755,//54
758,//57
761,//60
765,//63
769,//66
772,//68
776,//71
780,//74
784,//77
789,//80
795,//83
802,//85
805,//88
810,//91
817,//95
823,//97
834,//100
836 //100
};

static int bat_charge_value_table[37]={
0,  //0    
724,//0
729,//5
739,//10
741,//13
745,//16
747,//18
748,//20
750,//23
753,//29
755,//32
756,//35
758,//37
759,//40
761,//43
763,//46
765,//49
767,//51
769,//54
772,//57
775,//60
779,//63
783,//66
786,//68
790,//71
794,//74
798,//77
803,//80
809,//83
816,//85
819,//88
824,//91
831,//95
837,//97
848,//100
848 //100
};

static int bat_level_table[37]={
0,
0,
5,
10,
13,//15,
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
99,
99  
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
    printf("get_adc_sample(4): 0x%x\n", adc_val);
    return((((adc_val >= 0x40) && (adc_val < 0x3c0)) | powerkey_scan()) ? 1 : 0);
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
    int need_show_bat = 1;
    int ret;
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
        if(powerkey_scan())
        {
            need_show_bat = 1;         
            if(sleep_val >= 15)
            {
                run_command("lcd bl on", 0);
            }
            sleep_val = 0;
        }
        ret = powerkey_hold(1000);
        if(ret == 1)
        {
            return  0;
        }
        if(ret == 2)
        {
        	  need_show_bat = 1;
            if(sleep_val >= 15)
            {
                run_command("lcd bl on", 0);
            }
            sleep_val = 0;        	
        }
        
		if(need_show_bat == 1)
		{
			if(j >= 5) j = 0;
        if(sleep_val++ < 15)
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
                run_command(str, 0);
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
                run_command(str, 0);
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
            if(sleep_val >= 15)
            {
                tmp = -1;
                run_command("lcd bl off", 0);
                need_show_bat = 0;
            }
        }
		}
		if(tick_delay(100))
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

