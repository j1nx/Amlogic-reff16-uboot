#include <asm/arch/i2c.h>
#include <aml_i2c.h>
#include <ts.h>
#include <tsc2007.h>

#define TSC2007_MEASURE_X		(0xc << 4)
#define TSC2007_MEASURE_Y		(0xd << 4)
#define TSC2007_MEASURE_Z1		(0xe << 4)
#define TSC2007_MEASURE_Z2		(0xf << 4)

#define TSC2007_POWER_OFF_IRQ_EN	(0x0 << 2)
#define TSC2007_ADC_ON_IRQ_DIS0		(0x1 << 2)

#define TSC2007_12BIT			(0x0 << 1)

#define ADC_ON_12BIT	(TSC2007_12BIT | TSC2007_ADC_ON_IRQ_DIS0)

#define READ_Y		(ADC_ON_12BIT | TSC2007_MEASURE_Y)
#define READ_Z1		(ADC_ON_12BIT | TSC2007_MEASURE_Z1)
#define READ_Z2		(ADC_ON_12BIT | TSC2007_MEASURE_Z2)
#define READ_X		(ADC_ON_12BIT | TSC2007_MEASURE_X)
#define PWRDOWN		(TSC2007_12BIT | TSC2007_POWER_OFF_IRQ_EN)


struct tsc2007_event {
	u16	x;
	u16	y;
	u16	z1;
	u16	z2;
};

static int i2c_master_send(u32 addr, u8 *buff, u32 count)
{
	int ret;
	struct i2c_msg msg[] = {
		{
		.addr = addr,
		.flags = 0,
		.len = count,
		.buf = buff,
		}
	};

	ret = aml_i2c_xfer(msg, 1);
	return (ret == 1) ? count : ret;
}

static int i2c_master_recv(u32 addr, u8 *buff, u32 count)
{
	int ret;
	struct i2c_msg msg[] = {
		{
		.addr = addr,
		.flags = I2C_M_RD,
		.len = count,
		.buf = buff,
		}
	};

	ret = aml_i2c_xfer(msg, 1);
	return (ret == 1) ? count : ret;
}

static int tsc2007_xfer(u32 addr, u8 cmd)
{
	int ret;
	u8 buf[2];
	
	buf[0] = cmd;
	ret = i2c_master_send(addr, buf, 1);
	if (ret >= 0 ) {
		ret = i2c_master_recv(addr, buf, 2);
		if (ret >= 0) {
		    ret = (buf[0] << 4) | (buf[1] >> 4);
		}
	}
	return ret;
}

static void tsc2007_read_values(u32 addr, struct tsc2007_event *tsc)
{
	/* y- still on; turn on only y+ (and ADC) */
	tsc->y = tsc2007_xfer(addr, READ_Y);

	/* turn y- off, x+ on, then leave in lowpower */
	tsc->x = tsc2007_xfer(addr, READ_X);

	/* turn y+ off, x- on; we'll use formula #1 */
	tsc->z1 = tsc2007_xfer(addr, READ_Z1);
	tsc->z2 = tsc2007_xfer(addr, READ_Z2);

	/* Prepare for next touch reading - power down ADC, enable PENIRQ */
	tsc2007_xfer(addr, PWRDOWN);
}

int tsc2007_work(struct tsc2007 *ts, struct ts_event *tc)
{
    struct tsc2007_event tsc;
	
	if((ts == NULL) || (tc == NULL)) {
		//printf("tsc2007_work: ts is null or tc is null\n");
		return -1;
	}
	memset(tc, 0, sizeof(struct ts_event));
	if (ts->get_pendown_state == NULL) {
		//printf("tsc2007_work: ts get_pendown_state is null\n");
		return -1;
	}
	
	if(!ts->get_pendown_state()) {
		//printf("tsc2007_work: ts get_pendown_state is high\n");
		return 0;
	}
		
	tsc2007_read_values(ts->addr, &tsc);
	tc->x = tsc.x;
	tc->y = tsc.y;
	if ((tc->x < 0) || (tc->y < 0)) {
		//printf("tsc2007_work: tsc.x < 0 or tsc.y < 0\n");
		return 0;
	}
	if ((tc->x == 0) || (tc->y == 0)) {
		//printf("tsc2007_work: tsc.x == 0 and tsc.y == 0\n");
		return 0;
	}

	if (ts->convert) {
        int xy = ts->convert(tc->x, tc->y);
        tc->x = xy >> 16;
        tc->y = xy & 0xffff;
		tc->touch = ONE_TOUCH;
	}
	return 1;
}

int tsc2007_init(struct tsc2007 *ts)
{
	int err;
	
	if(ts == NULL) {
		printf("tsc2007_init: ts is null\n");
		return -1;
	}
	

	err = tsc2007_xfer(ts->addr, PWRDOWN);
	if (err < 0) {
		printf("tsc2007_init: i2c power down command failed\n");
		return -1;
	}
	return 0;
}
