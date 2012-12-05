#include <common.h>
#include <asm/arch/am_regs.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/gpio.h>
#include <bq27x00_battery.h>
#include <sn7325.h>
#ifdef CONFIG_BQ27x00_BATTERY
#include <bq27x00_battery.h>
static  struct bq27x00_device_info *di = NULL;
static int is_ac_connected(void)
{
	return (READ_CBUS_REG(ASSIST_HW_REV)&(1<<9))? 1:0;//GP_INPUT1
}


static void set_charging_mode(int flags)
{

    if(flags == 1)//fast charge
        {
        sn7325_set_io_dir(sn7325_get_io_dir() & ~OD7);
        sn7325_set_output_level(sn7325_get_output_level() | OD7);
	    }
    else        //slow charge
        {
        sn7325_set_io_dir(sn7325_get_io_dir() & ~OD7);
        sn7325_set_output_level(sn7325_get_output_level() & ~OD7);
        } 
}

static void set_bat_off(void)
{
    if(is_ac_connected()){ //AC in after power off press
        //kernel_restart("reboot");
    }
    set_gpio_val(GPIOA_bank_bit(8), GPIOA_bit_bit0_14(8), 0);
    set_gpio_mode(GPIOA_bank_bit(8), GPIOA_bit_bit0_14(8), GPIO_OUTPUT_MODE);

}

static struct bq27x00_battery_pdata bq27x00_pdata = {
	.is_ac_online	= is_ac_connected,
	.is_usb_online	= NULL,
	.set_charge = set_charging_mode,
	.set_bat_off = set_bat_off,
	.addr = 0x55,
    .chip = BQ27500,
};

static int get_charging_status()
{
     union power_supply_propval val;
     if(di == NULL){
        di = bq27x00_battery_probe(&bq27x00_pdata);
     }
     di->bat.get_property(&(di->bat), POWER_SUPPLY_PROP_STATUS, &val);
     return val.intval;    
     
}
int get_charging_percentage(void)
{   
     if(di == NULL){
        di = bq27x00_battery_probe(&bq27x00_pdata);
     }
     union power_supply_propval val;
     di->bat.get_property(&(di->bat), POWER_SUPPLY_PROP_CAPACITY, &val);
     printf("Battery Vol is %d%.\n", val.intval);
     return val.intval;    
}

int get_battery_percentage(void)
{
     if(di == NULL){
        di = bq27x00_battery_probe(&bq27x00_pdata);
     }
     union power_supply_propval val;
     di->bat.get_property(&(di->bat), POWER_SUPPLY_PROP_CAPACITY, &val);
     printf("Battery Vol is %d%.\n", val.intval);
     return val.intval;  
}
#endif

inline int any_key_pressed()
{
    int adc_val = get_adc_sample(4);
    //printf("get_adc_sample(4): 0x%x\n", adc_val);
    // no key pressed 0x3fd
    //return((((adc_val >= 0x0) && (adc_val < 0x3c0)) | powerkey_scan()) ? 1 : 0);
    return(((adc_val >= 0x0) && (adc_val < 0x3c0)) ? 1 : 0);
}

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
    printf("battery_charging!!!\n");
    set_charging_mode(1);
    di = bq27x00_battery_probe(&bq27x00_pdata);
    
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
        if(!(is_ac_connected()))
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
            if(get_charging_status()==POWER_SUPPLY_STATUS_FULL)
            {
                printf("Change over :100% !\n");
#ifdef	CONFIG_UBI_SUPPORT
                sprintf(str, "ubifsload ${loadaddr} resource/battery_pic/%d.bmp", (bat_pic_num-1));
#else
                sprintf(str, "nand read ${loadaddr} %x %x",
                    bat_pic_start+bat_pic_size*(bat_pic_num-1), bat_pic_size);
#endif
                run_command(str, 0);
                run_command("bmp display ${loadaddr}", 0);
                if(first_up)
                {
                    run_command ("lcd bl on", 0);
                    first_up = 0;
                }
                if(tmp != 100)
                {
                    tmp = charging_percentage = 100;
                    lcd_printf("\b\b\b\b%3d% ", charging_percentage);
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
                run_command("bmp display ${loadaddr}", 0);
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
                    lcd_printf("\b\b\b\b%3d% ", charging_percentage);
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
    run_command("bmp display ${loadaddr}", 0);
    run_command ("lcd bl on", 0);
}

