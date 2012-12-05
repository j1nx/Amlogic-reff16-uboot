#ifndef _TSC2007_H
#define _TSC2007_H

struct tsc2007 {
	u32 addr;
	int (*get_pendown_state)(void);
	int (*convert)(int x, int y);
};

extern int tsc2007_init(struct tsc2007 *ts);
extern int tsc2007_work(struct tsc2007 *ts, struct ts_event *tc);

#endif
