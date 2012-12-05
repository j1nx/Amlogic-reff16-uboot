#include "saradc_reg.h"

#define CHAN_XP	CHAN_0
#define CHAN_YP	CHAN_1
#define CHAN_XN	CHAN_2
#define CHAN_YN	CHAN_3

static u8 chan_mux[SARADC_CHAN_NUM] = {0,1,2,3,4,5,6,7};


void saradc_enable(void)
{
	int i;

	//set adc clock as 1.28Mhz
	set_clock_divider(20);
	enable_clock();
	enable_adc();

	set_sample_mode(DIFF_MODE);
	set_tempsen(0);
	disable_fifo_irq();
	disable_continuous_sample();
	disable_chan0_delta();
	disable_chan1_delta();

	set_input_delay(10, INPUT_DELAY_TB_1US);
	set_sample_delay(10, SAMPLE_DELAY_TB_1US);
	set_block_delay(10, BLOCK_DELAY_TB_1US);
	
	// channels sampling mode setting
	for(i=0; i<SARADC_CHAN_NUM; i++) {
		set_sample_sw(i, IDLE_SW);
		set_sample_mux(i, chan_mux[i]);
	}
	
	// idle mode setting
	set_idle_sw(IDLE_SW);
	set_idle_mux(chan_mux[CHAN_0]);
	
	// detect mode setting
	set_detect_sw(DETECT_SW);
	set_detect_mux(chan_mux[CHAN_0]);
	disable_detect_sw();
	disable_detect_pullup();
	set_detect_irq_pol(0);
	disable_detect_irq();

	set_sc_phase();
	
	enable_sample_engine();

//	printk("ADCREG reg0 =%x\n", get_reg(SAR_ADC_REG0));
//	printk("ADCREG ch list =%x\n", get_reg(SAR_ADC_CHAN_LIST));
//	printk("ADCREG avg =%x\n", get_reg(SAR_ADC_AVG_CNTL));
//	printk("ADCREG reg3=%x\n", get_reg(SAR_ADC_REG3));
//	printk("ADCREG ch72 sw=%x\n", get_reg(SAR_ADC_AUX_SW));
//	printk("ADCREG ch10 sw=%x\n", get_reg(SAR_ADC_CHAN_10_SW));
//	printk("ADCREG detect&idle=%x\n", get_reg(SAR_ADC_DETECT_IDLE_SW));
}

int get_adc_sample(int chan)
{
	int count;
	int value;
	int sum;
	
	set_chan_list(chan, 1);
	set_avg_mode(chan, NO_AVG_MODE, SAMPLE_NUM_8);
	set_sample_mux(chan, chan_mux[chan]);
	set_detect_mux(chan_mux[chan]);
	set_idle_mux(chan_mux[chan]); // for revb
	enable_sample_engine();
	start_sample();

	// Read any CBUS register to delay one clock cycle after starting the sampling engine
	// The bus is really fast and we may miss that it started
	{ count = get_reg(ISA_TIMERE); }

	count = 0;
	while (delta_busy() || sample_busy() || avg_busy()) {
		if (++count > 10000) {
    			printf("ADC busy error.\n");
			goto end;
		}
	}
    stop_sample();
    
    sum = 0;
    count = 0;
    value = get_fifo_sample();
	while (get_fifo_cnt()) {
        value = get_fifo_sample() & 0x3ff;
        if ((value != 0x1fe) && (value != 0x1ff)) {
			sum += value & 0x3ff;
            count++;
        }
	}
	value = (count) ? (sum / count) : (-1);

end:
	//printf("ch%d = %d, count=%d\n", chan, value, count);
	disable_sample_engine();
	set_sc_phase();
	return value;
}

int saradc_disable(void)
{
	disable_adc();
	disable_sample_engine();
	return 0;
}
