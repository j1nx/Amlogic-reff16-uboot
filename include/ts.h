#ifndef __TS_H
#define __TS_H

#define ZERO_TOUCH	0	
#define ONE_TOUCH	1
#define TWO_TOUCH	2

struct ts_event {
	u16	x;
	u16	y;
	u16	x1;
	u16	y1;
	u16 touch;
};

#endif
