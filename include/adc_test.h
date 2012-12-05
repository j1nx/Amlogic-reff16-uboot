#ifndef __ADC_TEST_H
#define __ADC_TEST_H

enum {
	ADC_OTHER = 0,
	ADC_KEY,
}ADC_TYPE;

typedef struct adckey_info{
	const char *key;
	int value;	/* voltage/3.3v * 1023 */
	int tolerance;
}adckey_info_t;

typedef struct adc_info{
	char *tint;
	int chan;
	int adc_type;
	void *adc_data;
}adc_info_t;

#endif
