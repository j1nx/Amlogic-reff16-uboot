#ifndef _UOR6X5X_H_
#define _UOR6X5X_H_

struct uor6x5x {
	u32 addr;
	int (*get_pendown_state)(void);
	int (*convert)(int x, int y);
};

extern int uor6x5x_init(struct uor6x5x *ts);
extern int uor6x5x_work(struct uor6x5x *ts, struct ts_event *tc);

#endif
