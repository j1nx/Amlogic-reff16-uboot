#include <asm/arch/i2c.h>
#include <aml_i2c.h>
#include <ts.h>
#include <eeti.h>

struct eeti_event {
	u16	x1;
	u16	y1;
	u16 status1;
	u16	x2;
	u16	y2;
	u16 status2;
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
	return ret;
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
	return ret;
}

static int eeti_read_values(u32 addr, struct eeti_event *tsc)
{
	u8 x_buf[MAX_I2C_LEN];
	int ret;
	short X=0, Y=0, ContactID=0, Status=0;
	ret = i2c_master_recv(addr, x_buf, MAX_I2C_LEN);
	if( ret<0 )
		return -1;
	if(x_buf[0]!=REPORTID_VENDOR && x_buf[0]!=REPORTID_MTOUCH)
	{
		printf(" I2C read error data with Len=%d hedaer=%d\n", ret, x_buf[0]);
		return 0;
	}
	Status = x_buf[1]&0x01;
	ContactID = (x_buf[1]&0x7C)>>2;
	X = ((x_buf[3]<<8) + x_buf[2])>>4;
	Y = ((x_buf[5]<<8) + x_buf[4])>>4;
	memset(tsc,0,sizeof(struct eeti_event));
	if(ContactID == 0)
	{
		tsc->x1 = X;
		tsc->y1 = Y;
		if(tsc->status1 != Status)
		{
			tsc->status1 = Status;
		}
	}
	else if(ContactID == 1)
	{
		tsc->x2 = X;
		tsc->y2 = Y;
		if(tsc->status2 != Status)
		{
			tsc->status2 = Status;
		}
	}
	return 1;
}

int eeti_work(struct eeti *ts, struct ts_event *tc)
{
    struct eeti_event tsc;
	int ret;
	if((ts == NULL) || (tc == NULL)) {
		printf("eeti_work: ts is null or tc is null\n");
		return -1;
	}
	memset(tc, 0, sizeof(struct ts_event));
	if (ts->get_pendown_state == NULL) {
		printf("eeti_work: ts get_pendown_state is null\n");
		return -1;
	}
	
	if(ts->get_pendown_state()) {
		//printf("eeti_work: ts get_pendown_state is high\n");
		return 0;
	}
	else
		printf("eeti_work: pendown!!!\n");
		
	ret = eeti_read_values(ts->addr, &tsc);
	if(ret != 1){
		printf("eeti_work: eeti_read_values error\n");
		return ret;
	}
		
	tc->x = tsc.x1;
	tc->y = tsc.y1;
	tc->x1 = tsc.x2;
	tc->y1 = tsc.y2;
	if ((tc->x < 0) || (tc->y < 0)||(tc->x1 < 0) || (tc->y1 < 0)) {
		//printf("eeti_work: tsc.x1 < 0 or tsc.y1 < 0 or tsc.x2 < 0 or tsc.y2 < 0\n");
		return 0;
	}

	if (ts->convert) {
        int xy = ts->convert(tc->x, tc->y);
        tc->x = xy >> 16;
        tc->y = xy & 0xffff;
        xy = ts->convert(tc->x1, tc->y1);
        tc->x1 = xy >> 16;
        tc->y1 = xy & 0xffff;
	}
	if(tsc.status1>0 && tsc.status2>0)
		tc->touch = TWO_TOUCH;
	else if(tsc.status1==0 && tsc.status2==0)
		tc->touch = ZERO_TOUCH;
	else
		tc->touch = ONE_TOUCH;
	return 1;
}

int eeti_init(struct eeti *ts)
{
	int err;
	
	if(ts == NULL) {
		printf("eeti_init: ts is null\n");
		return -1;
	}
	
	if(ts->init)
		ts->init();
	return 0;
}
