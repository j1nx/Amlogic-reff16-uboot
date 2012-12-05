#ifndef _EETI_H
#define _EETI_H
#include <ts.h>

#define MAX_I2C_LEN		10
#define REPORTID_VENDOR		0x03
#define REPORTID_MTOUCH		0x04

struct eeti {
	u32 addr;
	int (*get_pendown_state)(void);
	int (*convert)(int x, int y);
	int (*init)(void);
};

extern int eeti_init(struct eeti *ts);
extern int eeti_work(struct eeti *ts, struct ts_event *tc);

#endif
