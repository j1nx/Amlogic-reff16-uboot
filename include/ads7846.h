#ifndef _ADS7846_H
#define _ADS7846_H
#include <ts.h>

struct ads7846 {
	unsigned int cs;
	unsigned int max_speed_hz;
	unsigned int bus_num;
	unsigned int mode;
	int (*get_pendown_state)(void);
	int (*convert)(int x, int y);
	int (*init)(void);
};

extern int ads7846_init(struct ads7846 *ts);
extern int ads7846_work(struct ads7846 *ts, struct ts_event *tc);

#endif
