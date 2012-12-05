/*
 * (C) Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <watchdog.h>

#ifndef CONFIG_WD_PERIOD
# define CONFIG_WD_PERIOD	(10 * 1000 * 1000)	/* 10 seconds default*/
#endif

/* ------------------------------------------------------------------------- */

void udelay(unsigned long usec)
{
	ulong kv;

	do {
		WATCHDOG_RESET();
		kv = usec > CONFIG_WD_PERIOD ? CONFIG_WD_PERIOD : usec;
		__udelay (kv);
		usec -= kv;
	} while(usec);
}

void mdelay(unsigned long msec)
{
    while (msec--) udelay(1000);
}

int sdelay(unsigned long sec)
{
    while (sec > 0) {
		int i;

		--sec;
		/* delay 100 * 10ms */
		for (i=0; i<100; ++i) {
			if (tstc()) {	/* we got a key press	*/
				sec = 0;	/* no more delay	*/
# ifdef CONFIG_MENUKEY
				menukey = getc();
# else
				(void) getc();  /* consume input	*/
# endif
				return	1;
			}
			udelay(10000);
		}

		//printf("\b\b\b%2d ", sec);
	}
	return	0;
}

int tick_delay(unsigned long msec)
{
	unsigned long ticks = msec / 10;
	
	while (ticks > 0) {
		int i;

		--ticks;
		/* delay ticks*10 ms */
		for (i=0; i<ticks; ++i) {
			if (tstc()) {	/* we got a key press	*/
				ticks = 0;	/* no more delay	*/
# ifdef CONFIG_MENUKEY
				menukey = getc();
# else
				(void) getc();  /* consume input	*/
# endif
				return	1;
			}
			udelay(10000);
		}

		//printf("\b\b\b%2d ", sec);
	}
	return	0;
}