#include <common.h>
#include <command.h>
#include <aml_i2c.h>
#include <aml_rtc.h>
#include <rtc.h>
#include <asm/arch/gpio.h>
#include <lcd_aml.h>
#include <ts.h>
#include <tsc2007.h>
#include "sys_test.h"
#include <nand.h>
#include <mmc.h>
#include <asm/arch/nand.h>
#include <adc_test.h>

extern vidinfo_t panel_info;
static char *fail_buff[MAX_TEST_FAIL_NUM];
static unsigned int fail_pos = 0;

int fail_buff_free(void)
{
	int i = 0;
	
	for(i=0; i<MAX_TEST_RESULT_NUM; i++) {
		if(fail_buff[i]) {
			free((fail_buff[i]));
		}
		fail_buff[i] = NULL;
		
	}
	return 0;	
}

int fail_buff_malloc(int pos)
{
	fail_buff[pos] = (char *)malloc(MAX_TEST_RESULT_CHARS);
	if(fail_buff[pos] == NULL) {
		printf("%s: malloc is fail!\n", __FUNCTION__);
		return -1;
	}
	return 0;
}

int systest_free(char **buf)
{
	int i = 0;

	if(buf == NULL) {
		printf("%s: buf is null!\n", __FUNCTION__);
		return -1;
	}
	
	for(i=0; i<MAX_TEST_RESULT_NUM; i++) {
		if(buf[i]) {
			free((buf[i]));
		}
		buf[i] = NULL;
		
	}
	return 0;
}

int systest_malloc(char **buf)
{
	if(buf == NULL) {
		printf("%s: buf is null!\n", __FUNCTION__);
		return -1;
	}
	
	*buf = (char *)malloc(MAX_TEST_RESULT_CHARS);
	if(*buf == NULL) {
		printf("%s: malloc is fail!\n", __FUNCTION__);
		return -1;
	}
	return 0;
}

/* Nand Flash*/
static int nand_test(int item, unsigned int number, char **buf)
{
    nand_info_t *nand;
	int i = 0, count = 0;
	
	if(buf == NULL) {
		printf("%s: buf is null!\n", __FUNCTION__);
		return 0;
	}
	
	if(number <= 0) {
		printf("%s: buf number is overloop!\n", __FUNCTION__);
		return 0;
	}

	for (i = 0; i < CONFIG_SYS_MAX_NAND_DEVICE+1; i++) {
		nand = nand_info[i];
		if (!nand) {
			nand_init();
			if (!nand) {
				if(systest_malloc(buf+count) < 0) {
					printf("%s: malloc is fail!", __FUNCTION__);
					return 0;
				}
				count++;
				sprintf(buf[count], "No NAND device available!");
				printf("%s: nand is null!\n", __FUNCTION__);
				goto fail;
			}
		}
		if (nand->name) {
			nand = nand_info[i];
			struct nand_chip *chip = nand->priv;
				
			if(systest_malloc(buf+count) < 0) {
				printf("%s: malloc is fail!\n", __FUNCTION__);
				return 0;
			}
				
			//printf("Device %d:\n", i);
			sprintf(buf[count], "Device %d:", i);
			count++;
				
			if(systest_malloc(buf+count) < 0) {
				printf("%s: malloc is fail!\n", __FUNCTION__);
				return 0;
			}
#ifdef CONFIG_MTD_DEVICE
			sprintf(buf[count], "%s, %s sector size %u KiB",
					  nand->name, nand->info, nand->erasesize >> 10);
#else 
			sprintf(buf[count], "%s, sector size %u KiB",
					  nand->name, nand->erasesize >> 10);
#endif
            count++;
		}
	}
	return count;
fail:
	fail_buff_malloc(fail_pos);
	sprintf(fail_buff[fail_pos], "1 failed");
	fail_pos++;
	return -1;
}

/* SD Card*/
static int sdcard_test(int item, unsigned int number, char **buf)
{
	struct mmc *mmc;
	int dev_num;
	int count = 0;
	
	if(buf == NULL) {
		printf("%s: buf is null!\n", __FUNCTION__);
		return 0;
	}
	
	if(number <= 0) {
		printf("%s: buf number is overloop!\n", __FUNCTION__);
		return 0;
	}

	dev_num = simple_strtoul("mmcinfo", NULL, 0);

	mmc = find_mmc_device(dev_num);

	if (mmc) {
		mmc_init(mmc);

		if(systest_malloc(buf+count) < 0) {
			printf("%s: malloc is fail!\n", __FUNCTION__);
			return 0;
		}
	    if((mmc->tran_speed == 0) || (mmc->read_bl_len == 0) || (mmc->capacity == 0)) {
			sprintf(buf[count], "No MMC device available!");
			printf("%s: mmc is null!\n", __FUNCTION__);
			goto fail;
	    }

		sprintf(buf[count], "Device: %s", mmc->name);
		count++;

		if(systest_malloc(buf+count) < 0) {
			printf("%s: malloc is fail!\n", __FUNCTION__);
			return 0;
		}
		sprintf(buf[count], "Manufacturer ID: %x", mmc->cid[0] >> 24);
		count++;
		
		if(systest_malloc(buf+count) < 0) {
			printf("%s: malloc is fail!\n", __FUNCTION__);
			return 0;
		}
		sprintf(buf[count], "OEM: %x", (mmc->cid[0] >> 8) & 0xffff);
		count++;

		if(systest_malloc(buf+count) < 0) {
			printf("%s: malloc is fail!", __FUNCTION__);
			return 0;
		}
		sprintf(buf[count], "Name: %c%c%c%c%c", mmc->cid[0] & 0xff,
				(mmc->cid[1] >> 24), (mmc->cid[1] >> 16) & 0xff,
				(mmc->cid[1] >> 8) & 0xff, mmc->cid[1] & 0xff);
		count++;
		
		if(systest_malloc(buf+count) < 0) {
			printf("%s: malloc is fail!\n", __FUNCTION__);
			return 0;
		}
		sprintf(buf[count], "Tran Speed: %d", mmc->tran_speed);
		count++;

		if(systest_malloc(buf+count) < 0) {
			printf("%s: malloc is fail!\n", __FUNCTION__);
			return 0;
		}
		sprintf(buf[count], "Rd Block Len: %d", mmc->read_bl_len);
		count++;

		if(systest_malloc(buf+count) < 0) {
			printf("%s: malloc is fail!\n", __FUNCTION__);
			return 0;
		}
		sprintf(buf[count], "%s version %d.%d", IS_SD(mmc) ? "SD" : "MMC",
				(mmc->version >> 4) & 0xf, mmc->version & 0xf);
		count++;

		if(systest_malloc(buf+count) < 0) {
			printf("%s: malloc is fail!\n", __FUNCTION__);
			return 0;
		}
		sprintf(buf[count], "High Capacity: %s", mmc->high_capacity ? "Yes" : "No");
		count++;

		if(systest_malloc(buf+count) < 0) {
			printf("%s: malloc is fail!\n", __FUNCTION__);
			return 0;
		}
		sprintf(buf[count], "Capacity: %lld", mmc->capacity);
		count++;

		if(systest_malloc(buf+count) < 0) {
			printf("%s: malloc is fail!\n", __FUNCTION__);
			return 0;
		}
		sprintf(buf[count], "Bus Width: %d-bit", mmc->bus_width);
	}
	else {
		if(systest_malloc(buf+count) < 0) {
			printf("%s: malloc is fail!", __FUNCTION__);
			return 0;
		}
		sprintf(buf[count], "No MMC device available!");
		printf("%s: mmc is null!\n", __FUNCTION__);
		goto fail;
	}
	return count;
fail:
	fail_buff_malloc(fail_pos);
	sprintf(fail_buff[fail_pos], "2 failed");
	fail_pos++;
	return -1;
}

/* USB Storage*/
static int usb_test(int item, unsigned int number, char **buf)
{
#if 0
    usb_power_on();
	run_command ("usb start", 0);
	run_command ("usb info", 0);
#endif
	return 1;
}

/* I2C */
static unsigned int i2c_dev_num = 0;

#ifndef CONFIG_AML_I2C
struct i2c_board_info {
	char type[64];
	int i2c_id;
	unsigned short	addr;
	void (*device_init)(void);
	void (*device_uninit)(void);
};
#endif

struct i2c_board_info *aml_i2c_info = NULL;

void sys_test_i2c_init(struct i2c_board_info *i2c_info, unsigned int len)
{
    if(i2c_info != NULL)
		aml_i2c_info = i2c_info;
	i2c_dev_num = len;
}

int i2c_xfer(int id, unsigned int addr)
{
    if(id == SOFT_I2C) {
		printf("%s: software i2c!\n", __FUNCTION__);
		
		#ifdef CONFIG_SOFT_I2C
		if (i2c_probe(addr) != 0) {
			printf("%s: i2c transfer failed!\n", __FUNCTION__);
			return -1;
		}
		#endif
    }
	else {
		printf("%s: hardware i2c!\n", __FUNCTION__);
		
		#ifdef CONFIG_AML_I2C
		unsigned char cmd = 0x0;
		
		struct i2c_msg msgs[] = {
			{
				.addr = addr,
				.flags = 0,
				.len = 1,
				.buf = &cmd,
			}
		};
		
		if(aml_i2c_xfer(msgs, 1) < 0) {
			printf("%s: i2c transfer failed!\n", __FUNCTION__);
			return -1;
		}
		#endif
	}

	return 0;
}

static int i2c_test(int item, unsigned int number, char **buf)
{
	if(buf == NULL) {
		printf("%s: buf is null!\n", __FUNCTION__);
		return 0;
	}
	
	if(number <= 0) {
		printf("%s: buf number is overloop!\n", __FUNCTION__);
		return 0;
	}
	
	if(systest_malloc(buf) < 0) {
		printf("%s: malloc is fail!\n", __FUNCTION__);
		return 0;
	}

	if(aml_i2c_info[item].device_init) 
		aml_i2c_info[item].device_init();

	if(i2c_xfer(aml_i2c_info[item].i2c_id, aml_i2c_info[item].addr) < 0) {
		sprintf(*buf, "%s i2c transfer failed!", aml_i2c_info[item].type);
		goto fail;
	}
	sprintf(*buf, "%s, addr is 0x%x", aml_i2c_info[item].type, aml_i2c_info[item].addr);

	if(aml_i2c_info[item].device_uninit) 
		aml_i2c_info[item].device_uninit();
	return 1;	
fail:
	fail_buff_malloc(fail_pos);
	sprintf(fail_buff[fail_pos], "%d failed", item+3);
	fail_pos++;
	return -1;
}

/* ADC */
// get saradc value
struct adc_info *aml_adc_info = NULL;
static unsigned int adc_num = 0;

void adc_init(struct adc_info *adc_info, unsigned int len)
{
    if(adc_info != NULL)
		aml_adc_info = adc_info;
	adc_num = len;
}

#define IS_KEY(adc_val, value, tolerance) ((adc_val >= (value-tolerance))&&(adc_val <= (value+tolerance)))?1:0

static char *adc_return_tint(int item)
{
    if(ADC_KEY == aml_adc_info[item].adc_type) {
		return aml_adc_info[item].tint;
	}
	else {
		return NULL;
	}
}

static int adc_test(int item, unsigned int number, char **buf)
{
	int adc_val;
	int countdown = 10*1000*10;//10 second
	adckey_info_t *aml_adckey_info = NULL;
	
	if(buf == NULL) {
		printf("%s: buf is null!\n", __FUNCTION__);
		return 0;
	}
	
	if(number <= 0) {
		printf("%s: buf number is overloop!\n", __FUNCTION__);
		return 0;
	}
	
	if(systest_malloc(buf) < 0) {
		printf("%s: malloc is fail!\n", __FUNCTION__);
		return 0;
	}
	saradc_enable();

	if(aml_adc_info[item].adc_data != NULL) {
		if(ADC_KEY == aml_adc_info[item].adc_type) {
			aml_adckey_info = (adckey_info_t *)aml_adc_info[item].adc_data;
			while(countdown > 0) {
				udelay(100);
				adc_val = get_adc_sample(aml_adc_info[item].chan);
				//if(get_key())
				//	printf("adc key value: %d\n", adc_val);
				if(IS_KEY(adc_val, aml_adckey_info->value, aml_adckey_info->tolerance)) {
					sprintf(*buf, "%s is OK", aml_adc_info[item].tint);
					return 1;
				}
				countdown -= 1; 
			}
			
			sprintf(*buf, "%s, value is 0x%03x",aml_adc_info[item].tint, adc_val);
			goto fail;
		}
	}
	else {
		adc_val = get_adc_sample(aml_adc_info[item].chan);
		sprintf(*buf, "SARADC[%d]: 0x%03x", aml_adc_info[item].chan, adc_val);
		return 1;
	}
fail:
	fail_buff_malloc(fail_pos);
	sprintf(fail_buff[fail_pos], "%d failed", item+i2c_dev_num+3);
	fail_pos++;
	return -1;	
}

/* PWM */
static int bl_set_level(unsigned level)
{
	panel_info.set_bl_level(level);
	return 0;
}

// adjust backlight
static int pwm_test(int item, unsigned int number, char **buf)
{
	if(buf == NULL) {
		printf("%s: buf is null!\n", __FUNCTION__);
		return 0;
	}

	if(number <= 0) {
		printf("%s: buf number is overloop!\n", __FUNCTION__);
		return 0;
	}
	
	if(systest_malloc(buf) < 0) {
		printf("%s: malloc is fail!\n", __FUNCTION__);
		return 0;
	}
	
	if(item == 0){
		bl_set_level(128/2);
		sprintf(*buf, "set backlight low!");
	} 
	else if(item == 1) {
		bl_set_level(128*2);
		sprintf(*buf, "set backlight high!");
	} 
	else {
		bl_set_level(128);
		sprintf(*buf, "set backlight normal!");
	}
	return 1;
}

/* UART */
static int uart_test(int item, unsigned int number, char **buf)
{
	return 1;
}

/* RTC */
static int rtc_read_time(struct rtc_time *tm)
{
	aml_rtc_read_time(tm);
	return 0;
}
static int rtc_write_time(struct rtc_time *tm)
{
	aml_rtc_write_time(tm);
	return 0;
}
// read/write RTC 
static int RTC_test = 0;

static int rtc_test(int item, unsigned int number, char **buf)
{
	struct rtc_time tm;
	int tm_year = 111;
	int tm_mon = 2;
	int tm_mday = 23;
	int tm_hour = 13;
	int tm_min = 15;
	int tm_sec = 3;
	
	if(buf == NULL) {
		printf("%s: buf is null!\n", __FUNCTION__);
		return 0;
	}

	if(number <= 0) {
		printf("%s: buf number is overloop!\n", __FUNCTION__);
		return 0;
	}
	
	if(systest_malloc(buf) < 0) {
		printf("%s: malloc is fail!\n", __FUNCTION__);
		return 0;
	}
	
	if(item == 0) {
		tm.tm_year = tm_year;
		tm.tm_mon = tm_mon;
		tm.tm_mday = tm_mday;						
		tm.tm_hour = tm_hour;						
		tm.tm_min = tm_min;
		tm.tm_sec = tm_sec;	 
		rtc_write_time(&tm); 
		sprintf(*buf, "set time: %04d-%02d-%02d %02d:%02d:%02d",
			        tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
					tm.tm_hour, tm.tm_min, tm.tm_sec);
	}
	else if(item == 1) {
		rtc_read_time(&tm);
		if((tm.tm_year != tm_year) || (tm.tm_mon != tm_mon) || (tm.tm_mday != tm_mday)
			|| (tm.tm_hour != tm_hour) || (tm.tm_min != tm_min) || (tm.tm_sec < 0) || (tm.tm_sec > 60)) {
			sprintf(*buf, "RTC read time fail!");
			goto fail;
		}
		else
			sprintf(*buf, "get time: %04d-%02d-%02d %02d:%02d:%02d", 
					tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
					tm.tm_hour, tm.tm_min, tm.tm_sec);
	}
	else if(item == 2) {
		rtc_read_time(&tm);
		if((tm.tm_year != tm_year) || (tm.tm_mon != tm_mon) || (tm.tm_mday != tm_mday)
			|| (tm.tm_hour != tm_hour) || (tm.tm_min != tm_min) || (tm.tm_sec < 0) || (tm.tm_sec > 60)) {
			sprintf(*buf, "RTC read time fail!");
			goto fail;
		}
		else
			sprintf(*buf, "get time: %04d-%02d-%02d %02d:%02d:%02d", 
					tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
					tm.tm_hour, tm.tm_min, tm.tm_sec);
	}
	else if(item == 3) {
		rtc_read_time(&tm);
		if((tm.tm_year != tm_year) || (tm.tm_mon != tm_mon) || (tm.tm_mday != tm_mday)
			|| (tm.tm_hour != tm_hour) || (tm.tm_min != tm_min) || (tm.tm_sec < 0) || (tm.tm_sec > 60)) {
			sprintf(*buf, "RTC read time fail!");
			goto fail;
		}
		else
		sprintf(*buf, "get time: %04d-%02d-%02d %02d:%02d:%02d", 
					tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
					tm.tm_hour, tm.tm_min, tm.tm_sec);
	}
	else if(item == 4) {
		rtc_read_time(&tm);
		if((tm.tm_year != tm_year) || (tm.tm_mon != tm_mon) || (tm.tm_mday != tm_mday)
			|| (tm.tm_hour != tm_hour) || (tm.tm_min != tm_min) || (tm.tm_sec < 0) || (tm.tm_sec > 60)) {
			sprintf(*buf, "RTC read time fail!");
			goto fail;
		}
		else
			sprintf(*buf, "get time: %04d-%02d-%02d %02d:%02d:%02d", 
					tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
					tm.tm_hour, tm.tm_min, tm.tm_sec);
	}
	return 1;
fail:
	if(RTC_test == 0) {
	    fail_buff_malloc(fail_pos);
	    sprintf(fail_buff[fail_pos], "%d failed", adc_num+i2c_dev_num+2);
	    fail_pos++;
	    RTC_test = 1;
	}
	return -1;
}

/* GPIO */
// set GPIOA_2
static int gpio_test(int item, unsigned int number, char **buf)
{
	return 1;
}

/*TouchScreen*/
static int TOUCH_test = 0;

const char touch_tint[5][128] = {
    "Press first point",
	"Press second point",
	"Press third point",
	"Press fourth point",
	"Press fifth point",
};

static char *touch_return_tint(int item)
{
	return touch_tint[item];
}

static int touch_test(int item, unsigned int number, char **buf)
{
	int ret = 0;
	struct ts_event tc;
	int countdown = 10*1000*10;//10 second

	if(buf == NULL) {
		printf("%s: buf is null!\n", __FUNCTION__);
		return 0;
	}

	if(number <= 0) {
		printf("%s: buf number is overloop!\n", __FUNCTION__);
		return 0;
	}
	
	if(systest_malloc(buf) < 0) {
		printf("%s: malloc is fail!\n", __FUNCTION__);
		return 0;
	}

	ret = touch_init();
	if(ret < 0) {
		printf("%s: touch init fail!\n", __FUNCTION__);
		goto fail;
	}
	
	while(countdown > 0) {
		udelay(100);
		ret = touch_work(&tc);
		if(ret < 0) {
			printf("%s: touch work fail!\n", __FUNCTION__);
			goto fail;
		}
		else if(ret > 0) {
			sprintf(*buf, "%s: (x,y)=(%d,%d),(x1,y1)=(%d,%d),touch=%d", touch_tint[item], tc.x, tc.y, tc.x1, tc.y1,tc.touch);
			return 1;
		}
		countdown -= 1;	
	}
	sprintf(*buf, "%s is fail!",touch_tint[item]);
fail:
	if(TOUCH_test == 0) {
		fail_buff_malloc(fail_pos);
		sprintf(fail_buff[fail_pos], "%d failed", adc_num+i2c_dev_num+3);
		fail_pos++;
		TOUCH_test = 1;
	}
	return -1;
}

static void hdmi_open(void) {
	run_command ("tv open 720P", 0);
}

static void hdmi_close(void) {
	run_command ("tv close", 0);
}

static int hdmi_test(int item, unsigned int number, char **buf)
{
    hdmi_open();
    return 1;
}


static titeminfo_t nand_test_item = {
	.open_close  = ITEM_CLOSE,
	.test_type   = TEST_START,
	.pos         = 0,
	.title_width = 0,
	.item_nums   = 1,
	.title       = "Nand Test",
	.return_tint = NULL,
	.do_action   = nand_test,
};

static titeminfo_t sdcard_test_item = {
	.open_close  = ITEM_CLOSE,
	.test_type   = TEST_START,
	.pos         = 0,
	.title_width = 0,
	.item_nums   = 1,
	.title       = "SD Card Test",
	.return_tint = NULL,
	.do_action   = sdcard_test,
};

static titeminfo_t usb_test_item = {
	.open_close  = ITEM_CLOSE,
	.test_type   = TEST_START,
	.pos         = 0,
	.title_width = 0,
	.item_nums   = 1,
	.title       = "USB Test",
	.return_tint = NULL,
	.do_action   = usb_test,
};

static titeminfo_t i2c_test_item = {
	.open_close  = ITEM_CLOSE,
	.test_type   = TEST_START,
	.pos         = 0,
	.title_width = 0,
	.item_nums   = 0,
	.title       = "I2C Test",
	.return_tint = NULL,
	.do_action   = i2c_test,
};

static titeminfo_t adc_test_item = {
	.open_close  = ITEM_CLOSE,
	.test_type   = TEST_START,
	.pos         = 0,
	.title_width = 0,
	.item_nums   = 0,
	.title       = "ADC Test",
	.return_tint = adc_return_tint,
	.do_action   = adc_test,
};

static titeminfo_t pwm_test_item = {
	.open_close  = ITEM_CLOSE,
	.test_type   = TEST_START,
	.pos         = 0,
	.title_width = 0,
	.item_nums   = 3,
	.title       = "PWM Test",
	.return_tint = NULL,
	.do_action   = pwm_test,
};

static titeminfo_t uart_test_item = {
	.open_close  = ITEM_CLOSE,
	.test_type   = TEST_START,
	.pos         = 0,
	.title_width = 0,
	.item_nums   = 0,
	.title       = "UART Test",
	.return_tint = NULL,
	.do_action   = uart_test,
};

static titeminfo_t rtc_test_item = {
	.open_close  = ITEM_CLOSE,
	.test_type   = TEST_START,
	.pos         = 0,
	.title_width = 0,
	.item_nums   = 5,
	.title       = "RTC Test",
	.return_tint = NULL,
	.do_action   = rtc_test,
};

static titeminfo_t gpio_test_item = {
	.open_close  = ITEM_CLOSE,
	.test_type   = TEST_START,
	.pos         = 0,
	.title_width = 0,
	.item_nums   = 0,
	.title       = "GPIO Test",
	.return_tint = NULL,
	.do_action   = gpio_test,
};

static titeminfo_t touch_test_item = {
	.open_close  = ITEM_CLOSE,
	.test_type   = TEST_START,
	.pos         = 0,
	.title_width = 0,
	.item_nums   = 5,
	.title       = "Touch Test",
	.return_tint = touch_return_tint,
	.do_action   = touch_test,
};

static titeminfo_t hdmi_test_item = {
	.open_close  = ITEM_CLOSE,
	.test_type   = TEST_START,
	.pos         = 0,
	.title_width = 0,
	.item_nums   = 1,
	.title       = "HDMI Test",
	.return_tint = NULL,
	.do_action   = hdmi_test,
};

typedef struct st {
	int number;
	titeminfo_t test_items[MAX_TEST_ITEMS_NUM];
	char *buff[MAX_TEST_RESULT_NUM];
}st_t;
st_t st;

static void set_test_items(st_t *st)
{
	int i = 0;
	int width = 0;
	int height = 0;
	int world_len = 0;
	
	st->number  = 0;
	if(ITEM_NAND_TEST == ITEM_OPEN) {
		st->test_items[i] = nand_test_item;
		st->test_items[i].open_close = ITEM_OPEN;
		width = 0;
		GetTextSize(st->test_items[i].title, strlen(st->test_items[i].title), &width, &height, &world_len);
		st->test_items[i].title_width = width;
		st->test_items[i].pos = i;
		i++;
		st->number = i;
		if(st->number > MAX_TEST_ITEMS_NUM) {
			return;
		}
	}
	if(ITEM_SDCARD_TEST == ITEM_OPEN) {
		st->test_items[i] = sdcard_test_item;
		st->test_items[i].open_close = ITEM_OPEN;
		width = 0;
		GetTextSize(st->test_items[i].title, strlen(st->test_items[i].title), &width, &height, &world_len);
		st->test_items[i].title_width = width;
		st->test_items[i].pos = i;
		i++;
		st->number = i;
		if(st->number > MAX_TEST_ITEMS_NUM) {
			return;
		}
	}
	if(ITEM_USB_TEST == ITEM_OPEN) {
		st->test_items[i] = usb_test_item;
		st->test_items[i].open_close = ITEM_OPEN;
		width = 0;
		GetTextSize(st->test_items[i].title, strlen(st->test_items[i].title), &width, &height, &world_len);
		st->test_items[i].title_width = width;
		st->test_items[i].pos = i;
		i++;
		st->number = i;
		if(st->number > MAX_TEST_ITEMS_NUM) {
			return;
		}
	}
	if(ITEM_I2C_TEST == ITEM_OPEN) {
		st->test_items[i] = i2c_test_item;
		st->test_items[i].open_close = ITEM_OPEN;
		width = 0;
		GetTextSize(st->test_items[i].title, strlen(st->test_items[i].title), &width, &height, &world_len);
		st->test_items[i].title_width = width;
		st->test_items[i].pos = i;
		#ifdef CONFIG_AML_I2C
		if(aml_i2c_info != NULL) {
			st->test_items[i].item_nums = i2c_dev_num;
			//printf("%s: item_nums is %d\n", __FUNCTION__, st->test_items[i].item_nums);
		}
		#endif
		i++;
		st->number = i;
		if(st->number > MAX_TEST_ITEMS_NUM) {
			return;
		}
	}
	if(ITEM_ADC_TEST == ITEM_OPEN) {
		st->test_items[i] = adc_test_item;
		st->test_items[i].open_close = ITEM_OPEN;
		width = 0;
		GetTextSize(st->test_items[i].title, strlen(st->test_items[i].title), &width, &height, &world_len);
		st->test_items[i].title_width = width;
		st->test_items[i].pos = i;
		if(aml_adc_info != NULL) {
			st->test_items[i].item_nums = adc_num;
			//printf("%s: item_nums is %d\n", __FUNCTION__, st->test_items[i].item_nums);
		}
		i++;
		st->number = i;
		if(st->number > MAX_TEST_ITEMS_NUM) {
			return;
		}
	}
	if(ITEM_PWM_TEST == ITEM_OPEN) {
		st->test_items[i] = pwm_test_item;
		st->test_items[i].open_close = ITEM_OPEN;
		width = 0;
		GetTextSize(st->test_items[i].title, strlen(st->test_items[i].title), &width, &height, &world_len);
		st->test_items[i].title_width = width;
		st->test_items[i].pos = i;
		i++;
		st->number = i;
		if(st->number > MAX_TEST_ITEMS_NUM) {
			return;
		}
	}
	if(ITEM_UART_TEST == ITEM_OPEN) {
		st->test_items[i] = uart_test_item;
		st->test_items[i].open_close = ITEM_OPEN;
		width = 0;
		GetTextSize(st->test_items[i].title, strlen(st->test_items[i].title), &width, &height, &world_len);
		st->test_items[i].title_width = width;
		st->test_items[i].pos = i;
		i++;
		st->number = i;
		if(st->number > MAX_TEST_ITEMS_NUM) {
			return;
		}
	}
	if(ITEM_RTC_TEST == ITEM_OPEN) {
		st->test_items[i] = rtc_test_item;
		st->test_items[i].open_close = ITEM_OPEN;
		width = 0;
		GetTextSize(st->test_items[i].title, strlen(st->test_items[i].title), &width, &height, &world_len);
		st->test_items[i].title_width = width;
		st->test_items[i].pos = i;
		i++;
		st->number = i;
		if(st->number > MAX_TEST_ITEMS_NUM) {
			return;
		}
	}
	if(ITEM_GPIO_TEST == ITEM_OPEN) {
		st->test_items[i] = gpio_test_item;
		st->test_items[i].open_close = ITEM_OPEN;
		width = 0;
		GetTextSize(st->test_items[i].title, strlen(st->test_items[i].title), &width, &height, &world_len);
		st->test_items[i].title_width = width;
		st->test_items[i].pos = i;
		i++;
		st->number = i;
		if(st->number > MAX_TEST_ITEMS_NUM) {
			return;
		}
	}
	if(ITEM_TOUCH_TEST == ITEM_OPEN) {
		st->test_items[i] = touch_test_item;
		st->test_items[i].open_close = ITEM_OPEN;
		width = 0;
		GetTextSize(st->test_items[i].title, strlen(st->test_items[i].title), &width, &height, &world_len);
		st->test_items[i].title_width = width;
		st->test_items[i].pos = i;
		i++;
		st->number = i;
		if(st->number > MAX_TEST_ITEMS_NUM) {
			return;
		}
	}
	if(ITEM_HDMI_TEST == ITEM_OPEN) {
		st->test_items[i] = hdmi_test_item;
		st->test_items[i].open_close = ITEM_OPEN;
		width = 0;
		GetTextSize(st->test_items[i].title, strlen(st->test_items[i].title), &width, &height, &world_len);
		st->test_items[i].title_width = width;
		st->test_items[i].pos = i;
		i++;
		st->number = i;
		if(st->number > MAX_TEST_ITEMS_NUM) {
			return;
		}
	}

	printf("%s: st_t number is %d\n", __FUNCTION__, st->number);
}

typedef struct sd {
	int first_x;
	int first_y;
	int test_items_y;
	int result_title_y;
	int result_y;
	int max_item_width;
}sd_t;
sd_t sd = {
	.first_x        = 40,
	.first_y        = 20,
	.test_items_y   = 0,
	.result_title_y = 0,
	.result_y       = 0,
	.max_item_width   = 0,
};

static void display_test_title(sd_t *sd)
{
	uchar context[13] = "System test:";
	AsciiPrintf(context, sd->first_x, sd->first_y, DISPLAY_WHITE_COLOR);
	sd->test_items_y = sd->first_y + GetCharHeight();
}

static void displsy_all_test_items(st_t *st, sd_t *sd)
{
	int font_height = GetCharHeight();
	int i = 0;
	
	sd->test_items_y += 10;
	for(i=0; i<st->number; i++) {
		if(st->test_items[i].open_close == ITEM_OPEN) {
			if(sd->max_item_width < st->test_items[i].title_width) {
				sd->max_item_width = st->test_items[i].title_width;
			}
			AsciiPrintf((uchar *)st->test_items[i].title, sd->first_x, sd->test_items_y+font_height*i, DISPLAY_WHITE_COLOR);
		}
		else {
			break;
		}
	}
	sd->result_title_y = sd->test_items_y+font_height*i;
}

static void display_test_items(st_t *st, sd_t *sd, int pos, int bg_color) 
{
	int font_height = GetCharHeight();
	
	if(pos >= st->number) {
		return;
	}
	DrawRect(sd->first_x, sd->test_items_y+font_height*pos, st->test_items[pos].title_width, font_height, bg_color);
	AsciiPrintf((uchar *)st->test_items[pos].title, sd->first_x, sd->test_items_y+font_height*pos, DISPLAY_WHITE_COLOR);
}

static void display_result_title(sd_t *sd) 
{
	uchar context[20] = "System test result:";
	
	DrawRect(sd->first_x, sd->result_title_y+10, panel_info.vl_col-sd->first_x*2, 1, DISPLAY_WHITE_COLOR);
	AsciiPrintf(context, sd->first_x, sd->result_title_y+20, DISPLAY_WHITE_COLOR);
		
	sd->result_y = sd->result_title_y+GetCharHeight()+20;
}

static void clear_all_result(sd_t *sd) {
	DrawRect(sd->first_x, sd->result_y, panel_info.vl_col-sd->first_x, panel_info.vl_row-sd->result_y, DISPLAY_BLACK_COLOR);
}

static void display_all_result(sd_t *sd, char **buf) 
{
	int i = 0, j = 0;
	int font_height = GetCharHeight();

	for(i=0; i<MAX_TEST_RESULT_NUM; i++) {
		if(buf[i]) {
			AsciiPrintf((uchar *)buf[i], sd->first_x, sd->result_y+font_height*j, DISPLAY_WHITE_COLOR);
			j++;
		}
	}
}

static void display_result(sd_t *sd, int pos, char **buf) 
{
	int font_height = GetCharHeight();
	
	DrawRect(sd->first_x, sd->result_y+font_height*pos, panel_info.vl_col-sd->first_x, font_height, DISPLAY_BLACK_COLOR);
	if(buf[pos]) {
		AsciiPrintf((uchar *)buf[pos], sd->first_x, sd->result_y+font_height*pos, DISPLAY_WHITE_COLOR);
	}
}

static void display_result_tint(sd_t *sd, int pos, char *buf)
{
	int font_height = GetCharHeight();
	
	DrawRect(sd->first_x, sd->result_y+font_height*pos, panel_info.vl_col-sd->first_x, font_height, DISPLAY_BLACK_COLOR);
	if(buf) {
		AsciiPrintf((uchar *)buf, sd->first_x, sd->result_y+font_height*pos, DISPLAY_WHITE_COLOR);
	}
}

static void display_pass_or_fail(sd_t *sd, int pos, int flag) 
{
	int font_height = GetCharHeight();

	if(flag == TEST_SUCESS) {
		uchar context[5] = "PASS";
		
		DrawRect(sd->first_x*2+sd->max_item_width, sd->test_items_y+font_height*pos, panel_info.vl_col-sd->first_x*2-sd->max_item_width, font_height, DISPLAY_BLACK_COLOR);
		AsciiPrintf(context, sd->first_x*2+sd->max_item_width, sd->test_items_y+font_height*pos, DISPLAY_BLUE_COLOR);
	}
	else if(flag == TEST_FAIL) {
		uchar context[5] = "FAIL";
		
		DrawRect(sd->first_x*2+sd->max_item_width, sd->test_items_y+font_height*pos, panel_info.vl_col-sd->first_x*2-sd->max_item_width, font_height, DISPLAY_BLACK_COLOR);
		AsciiPrintf(context, sd->first_x*2+sd->max_item_width, sd->test_items_y+font_height*pos, DISPLAY_RED_COLOR);
	}		
}

static void display_all_fail_test(sd_t *sd)
{
	int i = 0, j = 0;
	int font_height = GetCharHeight();
	int half = panel_info.vl_row/2;
	char temp[MAX_TEST_RESULT_CHARS];
	
	DrawRect(0, 0, 1280, 720, DISPLAY_BLACK_COLOR);
	for(i = 0; i < fail_pos; i++) {
		if(fail_buff[i]) {
			if((sd->first_y+font_height*i) < panel_info.vl_col) {
				AsciiPrintf((uchar *)fail_buff[i], sd->first_x, sd->first_y+font_height*i, DISPLAY_WHITE_COLOR);
			}
			else {
				AsciiPrintf((uchar *)fail_buff[i], sd->first_x+half, sd->first_y+font_height*j, DISPLAY_WHITE_COLOR);
				j++;
			}
		}
	}
}

void main_test(void)
{
	int i = 0, j = 0;
	int tag = 0;
	int ret = 0;
	
	DrawRect(0, 0, panel_info.vl_row, panel_info.vl_col, DISPLAY_BLACK_COLOR);
	set_test_items(&st);
	display_test_title(&sd);
	displsy_all_test_items(&st, &sd);
	display_result_title(&sd);
	display_test_items(&st, &sd, i, DISPLAY_BLUE_COLOR);
	
	if(st.number <= 0) {
		printf("%s: st_t number <= 0\n", __FUNCTION__);
		return;
	}
	
	#ifdef CONFIG_SOFT_I2C
	i2c_init(0, 0);
	#endif
	
	for(j=0; j<MAX_TEST_RESULT_NUM; j++) {
		st.buff[j] = NULL;
	}
	
	sdelay(1);
	if(TESTACTION_TYPE == AUTO_TEST) {
		for(i=0; i<st.number; i++) {
			if((st.test_items[i].open_close == ITEM_OPEN) && (st.test_items[i].do_action != NULL)) {
				tag = 0;
				if(st.test_items[i].test_type != TEST_START) {
					printf("%s: item%d test_type != TEST_START\n", __FUNCTION__, i);
					continue;
				}
				if(i > 0)
					display_test_items(&st, &sd, i-1, DISPLAY_BLACK_COLOR);
				display_test_items(&st, &sd, i, DISPLAY_BLUE_COLOR);
				systest_free(st.buff);
				clear_all_result(&sd);
				for(j=0; j<st.test_items[i].item_nums; j++) {
					if(st.test_items[i].return_tint && (st.test_items[i].return_tint(j) != NULL)) {
						//printf("%s", st.test_items[i].return_tint(j));
						display_result_tint(&sd, j, st.test_items[i].return_tint(j));
					}
					ret = st.test_items[i].do_action(j, MAX_TEST_RESULT_NUM-j, st.buff+j);
					if(ret < 0) {
						tag = 1;
					}
					if(ret > 1) {
						display_all_result(&sd, st.buff);
					}
					else {
						display_result(&sd, j, st.buff);
					}
					sdelay(1);
				}
				if(tag == 0) {
					//sdelay(5);
					st.test_items[i].test_type = TEST_SUCESS;
					display_pass_or_fail(&sd, i, TEST_SUCESS);
				}
				else {
					//sdelay(5);
					st.test_items[i].test_type = TEST_FAIL;
					display_pass_or_fail(&sd, i, TEST_FAIL);
				}
			}
			else {
				printf("%s: item%d open_close != ITEM_OPEN or do_action is null\n", __FUNCTION__, i);
				break;
			}
		}
		if(fail_pos > 0) {
			//sdelay(1);
			display_all_fail_test(&sd);
			while(1);
		}
		sdelay(5);
		AsciiPrintf("Please press vol+ or vol- to upgrade the code.", sd.first_x, 340, DISPLAY_WHITE_COLOR);
		int tag = 1;
		saradc_enable();
		
		while(tag == 1) {
			mdelay(200);
			int tmp = get_key_value();
			if(get_key())
				printf("%s: get_key = %d\n", __FUNCTION__, tmp);
			if(KEY_UP(tmp) || KEY_DOWN(tmp)) {
				tag = 0;
				printf("the key is vol+ or vol-\n");
			}
		}
	}
	else {
		while(1){
			mdelay(200);
			if(i >= st.number) {
				printf("i >= systest number break\n", __FUNCTION__);
				break;
			}
			int tmp = get_key_value();
			if(get_key())
				printf("%s: get_key = %d\n", __FUNCTION__, tmp);
			if(KEY_EXIT(tmp)) {
				printf("KEY_EXIT break\n");
				break;
			}
			else if(KEY_UP(tmp)) {
				display_test_items(&st, &sd, i, DISPLAY_BLACK_COLOR);
				if(i <= 0){
					i = st.number-1;
				}
				else {
					i--;
				}
				display_test_items(&st, &sd, i, DISPLAY_BLUE_COLOR);
			}
			else if(KEY_DOWN(tmp)) {
				display_test_items(&st, &sd, i, DISPLAY_BLACK_COLOR);
				if(i >= st.number-1) {
					i = 0;
				}
				else {
					i++;
				}
				display_test_items(&st, &sd, i, DISPLAY_BLUE_COLOR);
			}
			else if(KEY_ENTER(tmp)) {
				tag = 0;
				if((st.test_items[i].open_close != ITEM_OPEN) || (st.test_items[i].do_action == NULL)) {
					printf("KEY_ENTER parameter error\n");
					break;
				}
				if(st.test_items[i].test_type != TEST_START) {
					continue;
				}
				systest_free(st.buff);
				clear_all_result(&sd);
				for(j=0; j<st.test_items[i].item_nums; j++) {
					if(st.test_items[i].return_tint && (st.test_items[i].return_tint(j) != NULL)) {
						//printf("%s", st.test_items[i].return_tint(j));
						display_result_tint(&sd, j, st.test_items[i].return_tint(j));
					}
					ret = st.test_items[i].do_action(j, MAX_TEST_RESULT_NUM-j, st.buff+j);
					if(ret < 0) {
						tag = 1;
					}
					if(ret > 1) {
						display_all_result(&sd, st.buff);
					}
					else {
						display_result(&sd, j, st.buff);
					}
					sdelay(1);
				}
				if(tag == 0) {
					sdelay(5);
					st.test_items[i].test_type = TEST_SUCESS;
					display_pass_or_fail(&sd, i, TEST_SUCESS);
				}
				else {
					sdelay(5);
					st.test_items[i].test_type = TEST_FAIL;
					display_pass_or_fail(&sd, i, TEST_FAIL);
				}
			}
		}
	}
	systest_free(st.buff);
	fail_buff_free();
}
