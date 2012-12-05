/*
 * MPC823 and PXA LCD Controller
 *
 * Modeled after video interface by Paolo Scaffardi
 *
 *
 * (C) Copyright 2001
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Author:  Elvis Yu <elvis.yu@amlogic.com>
 *
 */

#ifndef _LCD_AML_H_
#define _LCD_AML_H_
#include <asm/arch/osd.h>

extern char lcd_is_enabled;

//extern vidinfo_t panel_info;

extern void lcd_ctrl_init (void *lcdbase);


/* gunzip_bmp used if CONFIG_VIDEO_BMP_GZIP */
extern struct bmp_image *gunzip_bmp(unsigned long addr, unsigned long *lenp);

typedef struct vidinfo {
	ushort	vl_col;		/* Number of columns (i.e. 160) */
	ushort	vl_row;		/* Number of rows (i.e. 100) */

	u_char	vl_bpix;		/* Bits per pixel, 0 = 1 */

	void		*lcd_base;	/* Start of framebuffer memory	*/

	void		*lcd_console_address;	/* Start of console buffer	*/
	short 	console_col;
	short 	console_row;
	
	int 		lcd_color_fg;
	int 		lcd_color_bg;
	
	int		max_bl_level;

	ushort	*cmap;		/* Pointer to the colormap */

	void	*priv;			/* Pointer to driver-specific data */

	void  (*lcd_enable)(void);
	void  (*lcd_disable)(void);
	void  (*lcd_bl_on)(void);
	void  (*lcd_bl_off)(void);
	void  (*set_bl_level)(unsigned level);
} vidinfo_t;


/* int	lcd_init	(void *lcdbase); */
void	lcd_putc	(const char c);
void	lcd_puts	(const char *s);
void	lcd_printf	(const char *fmt, ...);

//extern int lcd_getfontwidth();
//extern int lcd_getfontheight();

//extern int lcd_draw_point (ushort x, ushort y, int color);
//extern int lcd_draw_rect (ushort x, ushort y, ushort x1, ushort y1, int color);
extern int lcd_drawchars (ushort x, ushort y, uchar *str, int count);
//extern int lcd_drawstring(uchar *text, unsigned len, unsigned short x, unsigned short y, int fg_color, int bg_color);

/* Allow boards to customize the information displayed */
void lcd_show_board_info(void);

/************************************************************************/
/* ** BITMAP DISPLAY SUPPORT						*/
/************************************************************************/
#if defined(CONFIG_CMD_BMP) || defined(CONFIG_SPLASH_SCREEN)
# include <bmp_layout.h>
# include <asm/byteorder.h>
#endif

/*
 *  Information about displays we are using. This is for configuring
 *  the LCD controller and memory allocation. Someone has to know what
 *  is connected, as we can't autodetect anything.
 */
#define CONFIG_SYS_HIGH	0	/* Pins are active high			*/
#define CONFIG_SYS_LOW		1	/* Pins are active low			*/

#define LCD_COLOR2	2
#define LCD_COLOR4	4
#define LCD_COLOR8	8
#define LCD_COLOR16	16
#define LCD_COLOR24	24

/*----------------------------------------------------------------------*/
#if defined(CONFIG_LCD_INFO_BELOW_LOGO)
# define LCD_INFO_X		0
# define LCD_INFO_Y		(BMP_LOGO_HEIGHT + VIDEO_FONT_HEIGHT)
#elif defined(CONFIG_LCD_LOGO)
# define LCD_INFO_X		(BMP_LOGO_WIDTH + 4 * VIDEO_FONT_WIDTH)
# define LCD_INFO_Y		(VIDEO_FONT_HEIGHT)
#else
# define LCD_INFO_X		(VIDEO_FONT_WIDTH)
# define LCD_INFO_Y		(VIDEO_FONT_HEIGHT)
#endif

/* Calculate nr. of bits per pixel  and nr. of colors */
#define NBITS(bit_code)		(bit_code)
#define NCOLORS(bit_code)	(1 << NBITS(bit_code))

/************************************************************************/
/* ** CONSOLE CONSTANTS							*/
/************************************************************************/
#if LCD_BPP == LCD_MONOCHROME

/*
 * Simple black/white definitions
 */
# define CONSOLE_COLOR_BLACK	0
# define CONSOLE_COLOR_WHITE	1	/* Must remain last / highest	*/

#elif LCD_BPP == LCD_COLOR8

/*
 * 8bpp color definitions
 */
# define CONSOLE_COLOR_BLACK	0
# define CONSOLE_COLOR_RED	1
# define CONSOLE_COLOR_GREEN	2
# define CONSOLE_COLOR_YELLOW	3
# define CONSOLE_COLOR_BLUE	4
# define CONSOLE_COLOR_MAGENTA	5
# define CONSOLE_COLOR_CYAN	6
# define CONSOLE_COLOR_GREY	14
# define CONSOLE_COLOR_WHITE	15	/* Must remain last / highest	*/

#else

/*
 * 16bpp color definitions
 */
# define CONSOLE_COLOR_BLACK	0x0000
# define CONSOLE_COLOR_WHITE	0xffff	/* Must remain last / highest	*/

#endif /* color definitions */

/************************************************************************/
#ifndef PAGE_SIZE
# define PAGE_SIZE	4096
#endif

/************************************************************************/
/* ** CONSOLE DEFINITIONS & FUNCTIONS					*/
/************************************************************************/
#if defined(CONFIG_LCD_LOGO) && !defined(CONFIG_LCD_INFO_BELOW_LOGO)
# define CONSOLE_ROWS		((panel_info.vl_row-BMP_LOGO_HEIGHT) \
					/ VIDEO_FONT_HEIGHT)
#else
# define CONSOLE_ROWS		(panel_info.vl_row / VIDEO_FONT_HEIGHT)
#endif

#define CONSOLE_COLS		(panel_info.vl_col / VIDEO_FONT_WIDTH)
#define CONSOLE_ROW_SIZE	(VIDEO_FONT_HEIGHT * lcd_line_length)
#define CONSOLE_ROW_FIRST	(panel_info.lcd_console_address)
#define CONSOLE_ROW_SECOND	(panel_info.lcd_console_address + CONSOLE_ROW_SIZE)
#define CONSOLE_ROW_LAST	(panel_info.lcd_console_address + CONSOLE_SIZE \
					- CONSOLE_ROW_SIZE)
#define CONSOLE_SIZE		(CONSOLE_ROW_SIZE * CONSOLE_ROWS)
#define CONSOLE_SCROLL_SIZE	(CONSOLE_SIZE - CONSOLE_ROW_SIZE)

# define COLOR_MASK(c)		(c)

/************************************************************************/

#endif	/* _LCD_H_ */
