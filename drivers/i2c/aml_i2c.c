/*
 * Author:  Elvis Yu <elvis.yu@amlogic.com>
 */

#include <asm-generic/errno.h>
#include <linux/types.h>
#include <asm/arch/clock.h>
#include <asm/arch/i2c.h>
#include <aml_i2c.h>

extern struct aml_i2c_platform aml_i2c_plat;

struct aml_i2c aml_i2c_ddata = {
	.i2c_debug = 0,
};

static void aml_i2c_set_clk(struct aml_i2c *i2c) 
{	
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	unsigned int i2c_clock_set;
	unsigned int sys_clk;
	struct aml_i2c_reg_ctrl* ctrl;
	//have not thought about sleep mode, sleep mode is low system clock
	sys_clk = get_clk81();
	AML_I2C_DBG(1, "clk81 is 0x%x\n", sys_clk);
	//sys_clk_rate = get_mpeg_clk();

	i2c_clock_set = sys_clk / i2c->master_i2c_speed;
	i2c_clock_set >>= 2;

	ctrl = (struct aml_i2c_reg_ctrl*)&(i2c->master_regs->i2c_ctrl);
	ctrl->clk_delay = i2c_clock_set & AML_I2C_CTRL_CLK_DELAY_MASK;
} 

static void aml_i2c_set_platform_data(struct aml_i2c *i2c, 
										struct aml_i2c_platform *plat)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	i2c->master_i2c_speed = plat->master_i2c_speed;
	i2c->wait_count = plat->wait_count;
	i2c->wait_ack_interval = plat->wait_ack_interval;
	i2c->wait_read_interval = plat->wait_read_interval;
	i2c->wait_xfer_interval = plat->wait_xfer_interval;

	if(i2c->master_no == MASTER_A){
		i2c->master_pinmux.scl_reg = plat->master_a_pinmux.scl_reg;
		i2c->master_pinmux.scl_bit = plat->master_a_pinmux.scl_bit;
		i2c->master_pinmux.sda_reg = plat->master_a_pinmux.sda_reg;
		i2c->master_pinmux.sda_bit = plat->master_a_pinmux.sda_bit;
	}
	else if(i2c->master_no == MASTER_B){
		i2c->master_pinmux.scl_reg = plat->master_b_pinmux.scl_reg;
		i2c->master_pinmux.scl_bit = plat->master_b_pinmux.scl_bit;
		i2c->master_pinmux.sda_reg = plat->master_b_pinmux.sda_reg;
		i2c->master_pinmux.sda_bit = plat->master_b_pinmux.sda_bit;
	}
#ifdef HAS_AO_MODULE
	else if(i2c->master_no == MASTER_AO){
		i2c->master_pinmux.scl_reg = plat->master_ao_pinmux.scl_reg;
		i2c->master_pinmux.scl_bit = plat->master_ao_pinmux.scl_bit;
		i2c->master_pinmux.sda_reg = plat->master_ao_pinmux.sda_reg;
		i2c->master_pinmux.sda_bit = plat->master_ao_pinmux.sda_bit;
	}
#endif
}

static void aml_i2c_pinmux_master(struct aml_i2c *i2c)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	unsigned int scl_pinmux;
	unsigned int sda_pinmux;
	
	scl_pinmux = readl(i2c->master_pinmux.scl_reg);
	scl_pinmux |= i2c->master_pinmux.scl_bit;
	writel(scl_pinmux, i2c->master_pinmux.scl_reg);
	
	sda_pinmux = readl(i2c->master_pinmux.sda_reg);
	sda_pinmux |= i2c->master_pinmux.sda_bit;
	writel(sda_pinmux, i2c->master_pinmux.sda_reg);
}

static void aml_i2c_dbg(struct aml_i2c *i2c)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	int i;
	struct aml_i2c_reg_ctrl* ctrl;

	if(i2c->i2c_debug == 0)
		return ;
	
	printf( "i2c_slave_addr:  0x%x\n", 
								i2c->master_regs->i2c_slave_addr);
	printf( "i2c_token_list_0:  0x%x\n", 
								i2c->master_regs->i2c_token_list_0);
	printf( "i2c_token_list_1:  0x%x\n", 
								i2c->master_regs->i2c_token_list_1);
	printf( "i2c_token_wdata_0:  0x%x\n", 
								i2c->master_regs->i2c_token_wdata_0);
	printf( "i2c_token_wdata_1:  0x%x\n", 
								i2c->master_regs->i2c_token_wdata_1);
	printf( "i2c_token_rdata_0:  0x%x\n", 
								i2c->master_regs->i2c_token_rdata_0);
	printf( "i2c_token_rdata_1:  0x%x\n", 
								i2c->master_regs->i2c_token_rdata_1);
	for(i=0; i<AML_I2C_MAX_TOKENS; i++)
		printf("token_tag[%d]  %d\n", i, i2c->token_tag[i]);
	
	ctrl = ((struct aml_i2c_reg_ctrl*)&(i2c->master_regs->i2c_ctrl));
	printf( "i2c_ctrl:  0x%x\n", i2c->master_regs->i2c_ctrl);
	printf( "ctrl.rdsda  0x%x\n", ctrl->rdsda);
	printf( "ctrl.rdscl  0x%x\n", ctrl->rdscl);
	printf( "ctrl.wrsda  0x%x\n", ctrl->wrsda);
	printf( "ctrl.wrscl  0x%x\n", ctrl->wrscl);
	printf( "ctrl.manual_en  0x%x\n", ctrl->manual_en);
	printf( "ctrl.clk_delay  0x%x\n", ctrl->clk_delay);
	printf( "ctrl.rd_data_cnt  0x%x\n", ctrl->rd_data_cnt);
	printf( "ctrl.cur_token  0x%x\n", ctrl->cur_token);
	printf( "ctrl.error  0x%x\n", ctrl->error);
	printf( "ctrl.status  0x%x\n", ctrl->status);
	printf( "ctrl.ack_ignore  0x%x\n", ctrl->ack_ignore);
	printf( "ctrl.start  0x%x\n", ctrl->start);
								
}

static void aml_i2c_clear_token_list(struct aml_i2c *i2c)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	i2c->master_regs->i2c_token_list_0 = 0;
	i2c->master_regs->i2c_token_list_1 = 0;
	memset(i2c->token_tag, TOKEN_END, AML_I2C_MAX_TOKENS);
}

static void aml_i2c_set_token_list(struct aml_i2c *i2c)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	int i;
	unsigned int token_reg=0;
	
	for(i=0; i<AML_I2C_MAX_TOKENS; i++)
		token_reg |= i2c->token_tag[i]<<(i*4);

	i2c->master_regs->i2c_token_list_0=token_reg;
}

static void aml_i2c_hw_init(struct aml_i2c *i2c, unsigned int use_pio)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	struct aml_i2c_reg_ctrl* ctrl;

	aml_i2c_set_clk(i2c);

	/*manual mode*/
	if(use_pio){
		ctrl = (struct aml_i2c_reg_ctrl*)&(i2c->master_regs->i2c_ctrl);
		ctrl->manual_en = 1;
	}
}

static int aml_i2c_check_error(struct aml_i2c *i2c)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	struct aml_i2c_reg_ctrl* ctrl;
	ctrl = (struct aml_i2c_reg_ctrl*)&(i2c->master_regs->i2c_ctrl);
	
	if(ctrl->error)
	{
		//printf( "ctrl.cur_token  0x%x\n", ctrl->cur_token);
		return -EIO;
	}
	else
		return 0;
}

/*poll status*/
static int aml_i2c_wait_ack(struct aml_i2c *i2c)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	int i;
	struct aml_i2c_reg_ctrl* ctrl;
	
	for(i=0; i<i2c->wait_count; i++) {
		udelay(i2c->wait_ack_interval);
		ctrl = (struct aml_i2c_reg_ctrl*)&(i2c->master_regs->i2c_ctrl);
		if(ctrl->status == IDLE)
			return aml_i2c_check_error(i2c);
		
		//cond_resched();
	}

	return -ETIMEDOUT;			
}

static void aml_i2c_get_read_data(struct aml_i2c *i2c, unsigned char *buf, 
														size_t len)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	int i;
	unsigned long rdata0 = i2c->master_regs->i2c_token_rdata_0;
	unsigned long rdata1 = i2c->master_regs->i2c_token_rdata_1;

	for(i=0; i< min_t(size_t, len, AML_I2C_MAX_TOKENS>>1); i++)
		*buf++ = (rdata0 >> (i*8)) & 0xff;

	for(; i< min_t(size_t, len, AML_I2C_MAX_TOKENS); i++) 
		*buf++ = (rdata1 >> ((i - (AML_I2C_MAX_TOKENS>>1))*8)) & 0xff;
}

static void aml_i2c_fill_data(struct aml_i2c *i2c, unsigned char *buf, 
							size_t len)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	int i;
	unsigned int wdata0 = 0;
	unsigned int wdata1 = 0;

	for(i=0; i< min_t(size_t, len, AML_I2C_MAX_TOKENS>>1); i++)
		wdata0 |= (*buf++) << (i*8);

	for(; i< min_t(size_t, len, AML_I2C_MAX_TOKENS); i++)
		wdata1 |= (*buf++) << ((i - (AML_I2C_MAX_TOKENS>>1))*8); 

	i2c->master_regs->i2c_token_wdata_0 = wdata0;
	i2c->master_regs->i2c_token_wdata_1 = wdata1;
}

static void aml_i2c_xfer_prepare(struct aml_i2c *i2c)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	aml_i2c_pinmux_master(i2c);
	aml_i2c_set_clk(i2c);
} 

static void aml_i2c_start_token_xfer(struct aml_i2c *i2c)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	//((struct aml_i2c_reg_ctrl*)&(i2c->master_regs->i2c_ctrl))->start = 0;	/*clear*/
	//((struct aml_i2c_reg_ctrl*)&(i2c->master_regs->i2c_ctrl))->start = 1;	/*set*/
	i2c->master_regs->i2c_ctrl &= ~1;	/*clear*/
	i2c->master_regs->i2c_ctrl |= 1;	/*set*/
	
	udelay(i2c->wait_xfer_interval);
}

/*our controller should send write data with slave addr in a token list,
	so we can't do normal address, just set addr into addr reg*/
static int aml_i2c_do_address(struct aml_i2c *i2c, unsigned int addr)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	int ret;
	
	i2c->cur_slave_addr = addr&0x7f;
	//((struct aml_i2c_reg_slave_addr*)&(i2c->master_regs->i2c_slave_addr))
	//								->slave_addr = i2c->cur_slave_addr<<1;
	i2c->master_regs->i2c_slave_addr = i2c->cur_slave_addr<<1;
	
	return 0;
}

static void aml_i2c_stop(struct aml_i2c *i2c)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	aml_i2c_clear_token_list(i2c);
	i2c->token_tag[0]=TOKEN_STOP;
	aml_i2c_set_token_list(i2c);
	aml_i2c_start_token_xfer(i2c);
}

static int aml_i2c_read(struct aml_i2c *i2c, unsigned char *buf, 
							size_t len) 
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	int i;
	int ret;
	size_t rd_len;
	int tagnum=0;

	aml_i2c_clear_token_list(i2c);
	
	if(! (i2c->msg_flags & I2C_M_NOSTART)){
		i2c->token_tag[tagnum++]=TOKEN_START;
		i2c->token_tag[tagnum++]=TOKEN_SLAVE_ADDR_READ;

		aml_i2c_set_token_list(i2c);
		aml_i2c_dbg(i2c);
		aml_i2c_start_token_xfer(i2c);

		udelay(i2c->wait_ack_interval);
		
		ret = aml_i2c_wait_ack(i2c);
		if(ret<0)
			return ret;	
		aml_i2c_clear_token_list(i2c);
	}
	
	while(len){
		tagnum = 0;
		rd_len = min_t(size_t, len, AML_I2C_MAX_TOKENS);
		if(rd_len == 1)
			i2c->token_tag[tagnum++]=TOKEN_DATA_LAST;
		else{
			for(i=0; i<rd_len-1; i++)
				i2c->token_tag[tagnum++]=TOKEN_DATA;
			if(len > rd_len)
				i2c->token_tag[tagnum++]=TOKEN_DATA;
			else
				i2c->token_tag[tagnum++]=TOKEN_DATA_LAST;
		}
		aml_i2c_set_token_list(i2c);
		aml_i2c_dbg(i2c);
		aml_i2c_start_token_xfer(i2c);

		udelay(i2c->wait_ack_interval);
		
		ret = aml_i2c_wait_ack(i2c);
		if(ret<0)
			return ret;	
		
		aml_i2c_get_read_data(i2c, buf, rd_len);
		len -= rd_len;
		buf += rd_len;

		aml_i2c_dbg(i2c);
		udelay(i2c->wait_read_interval);
		aml_i2c_clear_token_list(i2c);
	}
	return 0;
}

static int aml_i2c_write(struct aml_i2c *i2c, unsigned char *buf, 
							size_t len) 
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	int i;
	int ret;
	size_t wr_len;
	int tagnum=0;

	aml_i2c_clear_token_list(i2c);
	if(! (i2c->msg_flags & I2C_M_NOSTART)){
		i2c->token_tag[tagnum++]=TOKEN_START;
		i2c->token_tag[tagnum++]=TOKEN_SLAVE_ADDR_WRITE;
	}
	while(len){
		wr_len = min_t(size_t, len, AML_I2C_MAX_TOKENS-tagnum);
		for(i=0; i<wr_len; i++)
			i2c->token_tag[tagnum++]=TOKEN_DATA;
		
		aml_i2c_set_token_list(i2c);
		
		aml_i2c_fill_data(i2c, buf, wr_len);
		
		aml_i2c_dbg(i2c);
		aml_i2c_start_token_xfer(i2c);

		len -= wr_len;
		buf += wr_len;
		tagnum = 0;

		ret = aml_i2c_wait_ack(i2c);
		if(ret<0)
			return ret;
		
		aml_i2c_clear_token_list(i2c);
    	}
	return 0;
}

static struct aml_i2c_ops aml_i2c_m1_ops = {
	.xfer_prepare 	= aml_i2c_xfer_prepare,
	.read 		= aml_i2c_read,
	.write 		= aml_i2c_write,
	.do_address	= aml_i2c_do_address,
	.stop		= aml_i2c_stop,
};

/*General i2c master transfer*/
int aml_i2c_xfer(struct i2c_msg *msgs, 
							int num)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	struct aml_i2c *i2c = &aml_i2c_ddata;
	struct i2c_msg * p;
	unsigned int i;
	unsigned int ret=0;	
	
	i2c->ops->xfer_prepare(i2c);

	for (i = 0; !ret && i < num; i++) {
		p = &msgs[i];
		i2c->msg_flags = p->flags;
		ret = i2c->ops->do_address(i2c, p->addr, p->buf, p->flags & I2C_M_RD, p->len);
		if (ret || !p->len)
			continue;
		if (p->flags & I2C_M_RD)
			ret = i2c->ops->read(i2c, p->buf, p->len);
		else
			ret = i2c->ops->write(i2c, p->buf, p->len);
	}
	
	i2c->ops->stop(i2c);

	
	if (p->flags & I2C_M_RD){
		AML_I2C_DBG(0, "read ");
	}
	else {
		AML_I2C_DBG(0, "write ");
	}
	for(i=0;i<p->len;i++)
		AML_I2C_DBG(0, "%x-",*(p->buf)++);
	AML_I2C_DBG(0, "\n");
	
	/* Return the number of messages processed, or the error code*/
	if (ret == 0)
		return num;
	else {
		printf("[aml_i2c_xfer] error ret = %d \t", ret);
		printf("i2c master %s current slave addr is 0x%x\n", 
						i2c->master_no?"a":"b", i2c->cur_slave_addr);
		return ret;
	}
}

/*General i2c master transfer 100k*/
static int aml_i2c_xfer_slow(struct i2c_msg *msgs, 
							int num)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	struct aml_i2c *i2c = &aml_i2c_ddata;
	struct i2c_msg * p;
	unsigned int i;
	unsigned int ret=0;
	unsigned int last_speed = i2c->master_i2c_speed;
	

	i2c->master_i2c_speed = AML_I2C_SPPED_100K;/* change speed in i2c->lock*/
	i2c->ops->xfer_prepare(i2c);

	for (i = 0; !ret && i < num; i++) {
		p = &msgs[i];
		i2c->msg_flags = p->flags;
		ret = i2c->ops->do_address(i2c, p->addr, p->buf, p->flags & I2C_M_RD, p->len);
		if (ret || !p->len)
			continue;
		if (p->flags & I2C_M_RD)
			ret = i2c->ops->read(i2c, p->buf, p->len);
		else
			ret = i2c->ops->write(i2c, p->buf, p->len);
	}
	
	i2c->ops->stop(i2c);


	AML_I2C_DBG(0, "aml_i2c_xfer_slow");
	if (p->flags & I2C_M_RD){
		AML_I2C_DBG(0, "read ");
	}
	else {
		AML_I2C_DBG(0, "write ");
	}
	for(i=0;i<p->len;i++)
		AML_I2C_DBG(0, "%x-",*(p->buf)++);
	AML_I2C_DBG(0, "\n");
	
	i2c->master_i2c_speed = last_speed;
	/* Return the number of messages processed, or the error code*/
	if (ret == 0)
		return num;
	else {
		struct aml_i2c_reg_ctrl* ctrl;
		
		//printf("i2c master %s current slave addr is 0x%x \t", 
						//i2c->master_no?"a":"b", i2c->cur_slave_addr);

		ctrl = ((struct aml_i2c_reg_ctrl*)&(i2c->master_regs->i2c_ctrl));
		//if(ctrl->cur_token == TOKEN_START)
			//printf("error addr\n");
		//else
			//printf("error data\n");
		return ret;
	}
}


/***************i2c class****************/

static ssize_t show_i2c_debug(void)
{
	struct aml_i2c *i2c = &aml_i2c_ddata;
	printf("i2c debug is 0x%x\n", i2c->i2c_debug);
}

static ssize_t show_i2c_info(void)
{
	struct aml_i2c *i2c = &aml_i2c_ddata;
	struct aml_i2c_reg_ctrl* ctrl;

	printf( "i2c master %s current slave addr is 0x%x\n", 
						i2c->master_no?"a":"b", i2c->cur_slave_addr);
	printf( "wait ack timeout is 0x%x\n", 
							i2c->wait_count * i2c->wait_ack_interval);
	printf( "master regs base is 0x%x \n", 
								(unsigned int)(i2c->master_regs));

	ctrl = ((struct aml_i2c_reg_ctrl*)&(i2c->master_regs->i2c_ctrl));
	printf( "i2c_ctrl:  0x%x\n", i2c->master_regs->i2c_ctrl);
	printf( "ctrl.rdsda  0x%x\n", ctrl->rdsda);
	printf( "ctrl.rdscl  0x%x\n", ctrl->rdscl);
	printf( "ctrl.wrsda  0x%x\n", ctrl->wrsda);
	printf( "ctrl.wrscl  0x%x\n", ctrl->wrscl);
	printf( "ctrl.manual_en  0x%x\n", ctrl->manual_en);
	printf( "ctrl.clk_delay  0x%x\n", ctrl->clk_delay);
	printf( "ctrl.rd_data_cnt  0x%x\n", ctrl->rd_data_cnt);
	printf( "ctrl.cur_token  0x%x\n", ctrl->cur_token);
	printf( "ctrl.error  0x%x\n", ctrl->error);
	printf( "ctrl.status  0x%x\n", ctrl->status);
	printf( "ctrl.ack_ignore  0x%x\n", ctrl->ack_ignore);
	printf( "ctrl.start  0x%x\n", ctrl->start);
	
	printf( "i2c_slave_addr:  0x%x\n", 
								i2c->master_regs->i2c_slave_addr);
	printf( "i2c_token_list_0:  0x%x\n", 
								i2c->master_regs->i2c_token_list_0);
	printf( "i2c_token_list_1:  0x%x\n", 
								i2c->master_regs->i2c_token_list_1);
	printf( "i2c_token_wdata_0:  0x%x\n", 
								i2c->master_regs->i2c_token_wdata_0);
	printf( "i2c_token_wdata_1:  0x%x\n", 
								i2c->master_regs->i2c_token_wdata_1);
	printf( "i2c_token_rdata_0:  0x%x\n", 
								i2c->master_regs->i2c_token_rdata_0);
	printf( "i2c_token_rdata_1:  0x%x\n", 
								i2c->master_regs->i2c_token_rdata_1);
								
	printf( "master pinmux\n");
	printf( "scl_reg:  0x%x\n", i2c->master_pinmux.scl_reg);
	printf( "scl_bit:  0x%x\n", i2c->master_pinmux.scl_bit);
	printf( "sda_reg:  0x%x\n", i2c->master_pinmux.sda_reg);
	printf( "sda_bit:  0x%x\n", i2c->master_pinmux.sda_bit);

	return 0;
}

static unsigned int clock81_reading(void)
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	int val;
	
	val = READ_CBUS_REG(0x1070);
	printf( "1070=%x\n", val);
	val = READ_CBUS_REG(0x105d);
	printf( "105d=%x\n", val);
	return 148;
}


const unsigned long aml_i2c_reg_start[] = {
    [0] = MESON_I2C_MASTER_A_START,/*master a*/
    [1] = MESON_I2C_MASTER_B_START,/*master b*/
    [2] = MESON_I2C_SLAVE_START,/*slave*/
#ifdef HAS_AO_MODULE
    [3] = MESON_I2C_MASTER_AO_START,/*master ao*/
#endif
};

int aml_i2c_init(void) 
{
	AML_I2C_DBG(1, "FILE:%s:%d, FUNC:%s\n", __FILE__,__LINE__,__func__);
	int ret;
	struct aml_i2c_platform *plat = &aml_i2c_plat;
	struct aml_i2c *i2c = &aml_i2c_ddata;

	printf("aml_i2c_init %p %p \n",plat,i2c);

	if(plat == NULL)
	{
		printf("\nERROR!	struct aml_i2c_platform *plat is a NULL pointer!\n");
		return	-1;
	}

	i2c->ops = &aml_i2c_m1_ops;
	i2c->master_no = plat->master_no;
	i2c->use_pio = plat->use_pio;
	AML_I2C_ASSERT((i2c->master_no >= 0) && (i2c->master_no <= 3));

	/*master a or master b*/
 	if(i2c->master_no >= ARRAY_SIZE(aml_i2c_reg_start))
	{
		printf("\nERROR!	overflow: i2c->master_no = %d\n", i2c->master_no);
		return	-1;
	}
		
	i2c->master_regs = (struct aml_i2c_reg_master __iomem*)(aml_i2c_reg_start[i2c->master_no]);

	AML_I2C_ASSERT(i2c->master_regs);
	AML_I2C_ASSERT(plat);
	aml_i2c_set_platform_data(i2c, plat);

	aml_i2c_hw_init(i2c , i2c->use_pio);
	
	return 0;
}
