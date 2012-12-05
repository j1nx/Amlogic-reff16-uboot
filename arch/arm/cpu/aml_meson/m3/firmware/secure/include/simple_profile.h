/*
 * simple_profile.h
 *
 *  Created on: 2011-10-11
 *      Author: jerry.yu
 */

#ifndef SIMPLE_PROFILE_H_
#define SIMPLE_PROFILE_H_
#include <time.h>
#ifndef PROFILE_DISABLE
#define PROFILE_DECLARE() clock_t profile_clk,profile_total=0;static unsigned profile_count=0;static clock_t profile_time=0;profile_count++;profile_clk=clock()

#define PROFILE_POINT()	profile_clk=clock()-profile_clk;  \
					printf("\n[%s %d]:%u",__func__,__LINE__,(unsigned)profile_clk); \
  	  	  	  	  	profile_total+=profile_clk;           \
  	  	  	  	  	profile_clk=clock()
#define PROFILE_END() PROFILE_POINT();profile_time+=profile_total; \
					printf(",total=%u,count=%d,times=%d\n", \
							(unsigned)profile_total,profile_count,(unsigned)profile_time)
#else
#define PROFILE_DECLARE()
#define PROFILE_POINT()
#define PROFILE_END()

#endif

#endif /* SIMPLE_PROFILE_H_ */
