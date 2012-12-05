/*
 * Common LCD routines for supported CPUs
 *
 * (C) Copyright 2001-2002
 * Wolfgang Denk, DENX Software Engineering -- wd@denx.de
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

/************************************************************************/
/* ** HEADER FILES							*/
/************************************************************************/

//#define DEBUG

#include <config.h>
#include <common.h>
#include <command.h>
#include <stdarg.h>
#include <linux/types.h>
#include <stdio_dev.h>
#include <lcd_aml.h>

/************************************************************************/
/* ** FONT DATA								*/
/************************************************************************/
#include <video_font.h>		/* Get font data, width and height	*/

extern vidinfo_t panel_info;

static unsigned long lcd_line_length = 0;

DECLARE_GLOBAL_DATA_PTR;

int lcd_init (void);
static int lcd_getbgcolor (void);
static void lcd_setfgcolor (int color);
static void lcd_setbgcolor (int color);

char lcd_is_enabled = 0;

/************************************************************************/
/* ** LOGO DATA								*/
/************************************************************************/
#ifdef CONFIG_LCD_LOGO
# include <bmp_logo.h>		/* Get logo data, width and height	*/
# if (CONSOLE_COLOR_WHITE >= BMP_LOGO_OFFSET) && (LCD_BPP != LCD_COLOR16)
#  error Default Color Map overlaps with Logo Color Map
# endif
#endif

ulong lcd_setmem (ulong addr);

static inline void lcd_puts_xy (ushort x, ushort y, uchar *s);
static inline void lcd_putc_xy (ushort x, ushort y, uchar  c);


static void *lcd_logo (void);




/************************************************************************/
#if 0
int lcd_getfontwidth()
{
    return VIDEO_FONT_WIDTH;
}

int lcd_getfontheight()
{
    return VIDEO_FONT_HEIGHT;
}
#endif
/*----------------------------------------------------------------------*/

static void console_scrollup (void)
{
	/* Copy up rows ignoring the first one */
	memcpy (CONSOLE_ROW_FIRST, CONSOLE_ROW_SECOND, CONSOLE_SCROLL_SIZE);

	/* Clear the last one */
	memset (CONSOLE_ROW_LAST, COLOR_MASK(panel_info.lcd_color_bg), CONSOLE_ROW_SIZE);
}

/*----------------------------------------------------------------------*/

static inline void console_back (void)
{
	if (--panel_info.console_col < 0) {
		panel_info.console_col = CONSOLE_COLS-1 ;
		if (--panel_info.console_row < 0) {
			panel_info.console_row = 0;
		}
	}

	lcd_putc_xy (panel_info.console_col * VIDEO_FONT_WIDTH,
		     panel_info.console_row * VIDEO_FONT_HEIGHT,
		     ' ');
}

/*----------------------------------------------------------------------*/

static inline void console_newline (void)
{
	++panel_info.console_row;
	panel_info.console_col = 0;

	/* Check if we need to scroll the terminal */
	if (panel_info.console_row >= CONSOLE_ROWS) {
		/* Scroll everything up */
		console_scrollup () ;
		--panel_info.console_row;
	}
}

/*----------------------------------------------------------------------*/

void lcd_putc (const char c)
{
	if (!lcd_is_enabled) {
		serial_putc(c);
		return;
	}

	switch (c) {
	case '\r':	panel_info.console_col = 0;
			return;

	case '\n':	console_newline();
			return;

	case '\t':	/* Tab (8 chars alignment) */
			panel_info.console_col +=  8;
			panel_info.console_col &= ~7;

			if (panel_info.console_col >= CONSOLE_COLS) {
				console_newline();
			}
			return;

	case '\b':	console_back();
			return;

	default:	lcd_putc_xy (panel_info.console_col * VIDEO_FONT_WIDTH,
				     panel_info.console_row * VIDEO_FONT_HEIGHT,
				     c);
			if (++panel_info.console_col >= CONSOLE_COLS) {
				console_newline();
			}
			return;
	}
	/* NOTREACHED */
}

/*----------------------------------------------------------------------*/

void lcd_puts (const char *s)
{
	if (!lcd_is_enabled) {
		serial_puts (s);
		return;
	}
	while (*s) {
		lcd_putc (*s++);
	}
}

/*----------------------------------------------------------------------*/

void lcd_printf(const char *fmt, ...)
{
	va_list args;
	char buf[CONFIG_SYS_PBSIZE];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	lcd_puts(buf);
}

/************************************************************************/
/* ** Low-Level Graphics Routines					*/
/************************************************************************/

#if 0
int lcd_draw_point (ushort x, ushort y, int color)
{
    uchar *dest;
	
	if((x > panel_info.vl_col) || (y > panel_info.vl_row)) {
		return	-1;
	}

	dest = (uchar *)(panel_info.lcd_base + y * lcd_line_length + x * (LCD_BPP / 8));
#if(LCD_BPP == LCD_COLOR24)
	*dest++ = color & 0xff;
	*dest++ = (color >> 8) & 0xff;
	*dest = (color >> 16) & 0xff;
#endif
	flush_cache(panel_info.lcd_base, panel_info.vl_col*panel_info.vl_row*panel_info.vl_bpix/8);
	return	0;
}
int lcd_draw_rect (ushort x, ushort y, ushort w, ushort h, int color)
{
    uchar *dest;
	ushort row, col;
	if((x > panel_info.vl_col) || (y > panel_info.vl_row)) {
		return	-1;
	}

	if((w < 0) || (h < 0)) {
		return -1;
	}

	dest = (uchar *)(panel_info.lcd_base + y * lcd_line_length + x * (LCD_BPP / 8));
	for(row=0; row<h; row++) {
		for(col=0; col<w; col++) {
#if(LCD_BPP == LCD_COLOR24)
			*dest++ = color & 0xff;
			*dest++ = (color >> 8) & 0xff;
			*dest++ = (color >> 16) & 0xff;
#endif
		}
		dest = (uchar *)(panel_info.lcd_base + (y + row) * lcd_line_length + x * (LCD_BPP / 8));
	}
	flush_cache(panel_info.lcd_base, panel_info.vl_col*panel_info.vl_row*panel_info.vl_bpix/8);
	return	0;
}
#endif

int lcd_drawchars (ushort x, ushort y, uchar *str, int count)
{
	uchar *dest;
	ushort off, row;

	if((x > panel_info.vl_col) || (y > panel_info.vl_row))
	{
		return	-1;
	}

	dest = (uchar *)(panel_info.lcd_base + y * lcd_line_length + x * (LCD_BPP / 8));
	off  = x * LCD_BPP % 8;
	//printf("panel_info.lcd_base:0x%x; lcd_line_length:%d; LCD_BPP:%d; dest: 0x%x\n", panel_info.lcd_base, lcd_line_length, LCD_BPP, dest);
	for (row=0;  row < VIDEO_FONT_HEIGHT;  ++row, dest += lcd_line_length)  {
		uchar *s = str;
		int i;

#if(LCD_BPP == LCD_COLOR16)
		ushort *d = (ushort *)dest;
#else
		uchar *d = dest;
#endif

#if(LCD_BPP == LCD_MONOCHROME)
		uchar rest = *d & -(1 << (8-off));
		uchar sym;
#endif
		for (i=0; i<count; ++i) {
			uchar c, bits;

			c = *s++;
			bits = video_fontdata[c * VIDEO_FONT_HEIGHT + row];
			//printf("c: %d;	row: %d;	bits: 0x%x\n", c, row, bits);

#if(LCD_BPP == LCD_MONOCHROME)
			sym  = (COLOR_MASK(panel_info.lcd_color_fg) & bits) |
			       (COLOR_MASK(panel_info.lcd_color_bg) & ~bits);

			*d++ = rest | (sym >> off);
			rest = sym << (8-off);
#elif(LCD_BPP == LCD_COLOR8)
			for (c=0; c<8; ++c) {
				*d++ = (bits & 0x80) ?
						panel_info.lcd_color_fg : panel_info.lcd_color_bg;
				bits <<= 1;
			}
#elif(LCD_BPP == LCD_COLOR16)
			for (c=0; c<8; ++c) {
				*d++ = (bits & 0x80) ?
						panel_info.lcd_color_fg : panel_info.lcd_color_bg;
				bits <<= 1;
			}
#elif(LCD_BPP == LCD_COLOR24)
			for (c=0; c<8; ++c) {
				if(bits & 0x80)
				{
					*d++ = (panel_info.lcd_color_fg >> 16) & 0xff;
					*d++ = (panel_info.lcd_color_fg >> 8) & 0xff;
					*d++ = panel_info.lcd_color_fg & 0xff;
				}
				else
				{
					*d++ = (panel_info.lcd_color_bg >> 16) & 0xff;
					*d++ = (panel_info.lcd_color_bg >> 8) & 0xff;
					*d++ = panel_info.lcd_color_bg & 0xff;
				}
				bits <<= 1;
			}
#endif	

		}
		
#if LCD_BPP == LCD_MONOCHROME
		*d  = rest | (*d & ((1 << (8-off)) - 1));
#endif
	flush_cache(panel_info.lcd_base, panel_info.vl_col*panel_info.vl_row*panel_info.vl_bpix/8);
	}
	return 0;
}

#if 0
int lcd_drawstring(uchar *text, unsigned len, unsigned short x, unsigned short y, int fg_color, int bg_color)
{
    lcd_setfgcolor(fg_color);
	lcd_setbgcolor(bg_color);
    return lcd_drawchars(x, y, text, len);
}
#endif
/*----------------------------------------------------------------------*/

static inline void lcd_puts_xy (ushort x, ushort y, uchar *s)
{
#if defined(CONFIG_LCD_LOGO) && !defined(CONFIG_LCD_INFO_BELOW_LOGO)
	lcd_drawchars (x, y+BMP_LOGO_HEIGHT, s, strlen ((char *)s));
#else
	lcd_drawchars (x, y, s, strlen ((char *)s));
#endif
}

/*----------------------------------------------------------------------*/

static inline void lcd_putc_xy (ushort x, ushort y, uchar c)
{
#if defined(CONFIG_LCD_LOGO) && !defined(CONFIG_LCD_INFO_BELOW_LOGO)
	lcd_drawchars (x, y+BMP_LOGO_HEIGHT, &c, 1);
#else
	lcd_drawchars (x, y, &c, 1);
#endif
}

/************************************************************************/
/**  Small utility to check that you got the colours right		*/
/************************************************************************/
#ifdef LCD_TEST_PATTERN

#define	N_BLK_VERT	2
#define	N_BLK_HOR	3

static int test_colors[N_BLK_HOR*N_BLK_VERT] = {
	CONSOLE_COLOR_RED,	CONSOLE_COLOR_GREEN,	CONSOLE_COLOR_YELLOW,
	CONSOLE_COLOR_BLUE,	CONSOLE_COLOR_MAGENTA,	CONSOLE_COLOR_CYAN,
};

static void test_pattern (void)
{
	ushort v_max  = panel_info.vl_row;
	ushort h_max  = panel_info.vl_col;
	ushort v_step = (v_max + N_BLK_VERT - 1) / N_BLK_VERT;
	ushort h_step = (h_max + N_BLK_HOR  - 1) / N_BLK_HOR;
	ushort v, h;
	uchar *pix = (uchar *)panel_info.lcd_base;

	printf ("[LCD] Test Pattern: %d x %d [%d x %d]\n",
		h_max, v_max, h_step, v_step);

	/* WARNING: Code silently assumes 8bit/pixel */
	for (v=0; v<v_max; ++v) {
		uchar iy = v / v_step;
		for (h=0; h<h_max; ++h) {
			uchar ix = N_BLK_HOR * iy + (h/h_step);
			*pix++ = test_colors[ix];
		}
	}
}
#endif /* LCD_TEST_PATTERN */

/*
 * Subroutine:  lcd_info
 *
 * Description: Show information about lcd
 *
 * Inputs:	None
 *
 * Return:      None
 *
 */
inline static void lcd_info(void)
{
	printf("LCD Information:\n");
	printf("vl_col:		%d\n", panel_info.vl_col);
	printf("vl_row:		%d\n", panel_info.vl_row);
	printf("vl_bpix:	%d\n", panel_info.vl_bpix);
	printf("lcd_base:	0x%08x\n", panel_info.lcd_base);
	printf("lcd_console_address:	0x%08x\n", panel_info.lcd_console_address);
	printf("console_col:	%d\n", panel_info.console_col);
	printf("console_row:	%d\n", panel_info.console_row);
	printf("lcd_color_fg:	0x%x\n", panel_info.lcd_color_fg);
	printf("lcd_color_bg:	0x%x\n", panel_info.lcd_color_bg);
	printf("max_bl_level:	%d\n", panel_info.max_bl_level);
	printf("cmap:		0x%08x\n", panel_info.cmap);
}

/*
 * Subroutine:  lcd_bl_opt
 *
 * Description: operate backlight
 *
 * Inputs:	status
 *
 * Return:      None
 *
 */
static void lcd_bl_opt(unsigned long status)
{
	if(status == 0)
	{
		panel_info.lcd_bl_off();
	}
	else if(status == 0xffffffff)
	{
		panel_info.lcd_bl_on();
	}
	else	if(status <= panel_info.max_bl_level)
	{
		panel_info.set_bl_level(status);
	}
	else
	{
		printf("lcd_bl_opt failed!\n");
	}
		
}

/*
 * Subroutine:  lcd_info
 *
 * Description: Clear lcd screen
 *
 * Inputs:	None
 *
 * Return:      None
 *
 */
inline static void lcd_clear (void)
{
	printf("LCD screen clear!\n");
	memset ((char *)panel_info.lcd_base,
		COLOR_MASK(lcd_getbgcolor()),
		lcd_line_length*panel_info.vl_row);
}

/************************************************************************/
/* ** GENERIC Initialization Routines					*/
/************************************************************************/

int drv_lcd_init (void)
{
	struct stdio_dev lcddev;
	int rc;

	lcd_init ();		/* LCD initialization */

	/* Device initialization */
	memset (&lcddev, 0, sizeof (lcddev));

	strcpy (lcddev.name, "lcd");
	lcddev.ext   = 0;			/* No extensions */
	lcddev.flags = DEV_FLAGS_OUTPUT;	/* Output only */
	lcddev.putc  = lcd_putc;		/* 'putc' function */
	lcddev.puts  = lcd_puts;		/* 'puts' function */

	rc = stdio_register (&lcddev);

	//panel_info.lcd_bl_on();

	return (rc == 0) ? 1 : rc;
}

/*----------------------------------------------------------------------*/

int lcd_init (void)
{
	/* Initialize the lcd controller */
	gd->fb_base = panel_info.lcd_base;

	lcd_line_length = (panel_info.vl_col * NBITS (panel_info.vl_bpix)) / 8;

	debug ("[LCD] Initializing LCD frambuffer at %p\n", panel_info.lcd_base);

	lcd_is_enabled = 1;
	//lcd_clear (NULL, 1, 1, NULL);	/* dummy args */
	panel_info.lcd_enable();
	lcd_clear();

	/* Initialize the console */
	panel_info.console_col = 0;
#ifdef CONFIG_LCD_INFO_BELOW_LOGO
	panel_info.console_row = 7 + BMP_LOGO_HEIGHT / VIDEO_FONT_HEIGHT;
#else
	panel_info.console_row = 1;	/* leave 1 blank line below logo */
#endif
	printf("LCD:	%dx%d %dbbp\n", panel_info.vl_col, panel_info.vl_row, panel_info.vl_bpix);
	return 0;
}


/************************************************************************/
/* ** ROM capable initialization part - needed to reserve FB memory	*/
/************************************************************************/
/*
 * This is called early in the system initialization to grab memory
 * for the LCD controller.
 * Returns new address for monitor, after reserving LCD buffer memory
 *
 * Note that this is running from ROM, so no write access to global data.
 */
ulong lcd_setmem (ulong addr)
{
	ulong size;
	int line_length = (panel_info.vl_col * NBITS (panel_info.vl_bpix)) / 8;

	debug ("LCD panel info: %d x %d, %d bit/pix\n",
		panel_info.vl_col, panel_info.vl_row, NBITS (panel_info.vl_bpix) );

	size = line_length * panel_info.vl_row;

	/* Round up to nearest full page */
	size = (size + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);

	/* Allocate pages for the frame buffer. */
	addr -= size;

	debug ("Reserving %ldk for LCD Framebuffer at: %08lx\n", size>>10, addr);

	return (addr);
}

/*----------------------------------------------------------------------*/

static void lcd_setfgcolor (int color)
{
	panel_info.lcd_color_fg = color;
}

/*----------------------------------------------------------------------*/

static void lcd_setbgcolor (int color)
{
	panel_info.lcd_color_bg = color;
}

/*----------------------------------------------------------------------*/

static int lcd_getfgcolor (void)
{
	return panel_info.lcd_color_fg;
}

/*----------------------------------------------------------------------*/

static int lcd_getbgcolor (void)
{
	return panel_info.lcd_color_bg;
}

/*----------------------------------------------------------------------*/

#if	0
/************************************************************************/
/* ** Chipset depending Bitmap / Logo stuff...                          */
/************************************************************************/
#ifdef CONFIG_LCD_LOGO
void bitmap_plot (int x, int y)
{
#ifdef CONFIG_ATMEL_LCD
	uint *cmap;
#else
	ushort *cmap;
#endif
	ushort i, j;
	uchar *bmap;
	uchar *fb;
	ushort *fb16;
#if defined(CONFIG_PXA250)
	struct pxafb_info *fbi = &panel_info.pxa;
#elif defined(CONFIG_MPC823)
	volatile immap_t *immr = (immap_t *) CONFIG_SYS_IMMR;
	volatile cpm8xx_t *cp = &(immr->im_cpm);
#endif

	debug ("Logo: width %d  height %d  colors %d  cmap %d\n",
		BMP_LOGO_WIDTH, BMP_LOGO_HEIGHT, BMP_LOGO_COLORS,
		(int)(sizeof(bmp_logo_palette)/(sizeof(ushort))));

	bmap = &bmp_logo_bitmap[0];
	fb   = (uchar *)(lcd_base + y * lcd_line_length + x);

	if (NBITS(panel_info.vl_bpix) < 12) {
		/* Leave room for default color map */
#if defined(CONFIG_PXA250)
		cmap = (ushort *)fbi->palette;
#elif defined(CONFIG_MPC823)
		cmap = (ushort *)&(cp->lcd_cmap[BMP_LOGO_OFFSET*sizeof(ushort)]);
#elif defined(CONFIG_ATMEL_LCD)
		cmap = (uint *) (panel_info.mmio + ATMEL_LCDC_LUT(0));
#else
		/*
		 * default case: generic system with no cmap (most likely 16bpp)
		 * We set cmap to the source palette, so no change is done.
		 * This avoids even more ifdef in the next stanza
		 */
		cmap = bmp_logo_palette;
#endif

		WATCHDOG_RESET();

		/* Set color map */
		for (i=0; i<(sizeof(bmp_logo_palette)/(sizeof(ushort))); ++i) {
			ushort colreg = bmp_logo_palette[i];
#ifdef CONFIG_ATMEL_LCD
			uint lut_entry;
#ifdef CONFIG_ATMEL_LCD_BGR555
			lut_entry = ((colreg & 0x000F) << 11) |
				    ((colreg & 0x00F0) <<  2) |
				    ((colreg & 0x0F00) >>  7);
#else /* CONFIG_ATMEL_LCD_RGB565 */
			lut_entry = ((colreg & 0x000F) << 1) |
				    ((colreg & 0x00F0) << 3) |
				    ((colreg & 0x0F00) << 4);
#endif
			*(cmap + BMP_LOGO_OFFSET) = lut_entry;
			cmap++;
#else /* !CONFIG_ATMEL_LCD */
#ifdef  CONFIG_SYS_INVERT_COLORS
			*cmap++ = 0xffff - colreg;
#else
			*cmap++ = colreg;
#endif
#endif /* CONFIG_ATMEL_LCD */
		}

		WATCHDOG_RESET();

		for (i=0; i<BMP_LOGO_HEIGHT; ++i) {
			memcpy (fb, bmap, BMP_LOGO_WIDTH);
			bmap += BMP_LOGO_WIDTH;
			fb   += panel_info.vl_col;
		}
	}
	else { /* true color mode */
		u16 col16;
		fb16 = (ushort *)(lcd_base + y * lcd_line_length + x);
		for (i=0; i<BMP_LOGO_HEIGHT; ++i) {
			for (j=0; j<BMP_LOGO_WIDTH; j++) {
				col16 = bmp_logo_palette[(bmap[j]-16)];
				fb16[j] =
					((col16 & 0x000F) << 1) |
					((col16 & 0x00F0) << 3) |
					((col16 & 0x0F00) << 4);
				}
			bmap += BMP_LOGO_WIDTH;
			fb16 += panel_info.vl_col;
		}
	}

	WATCHDOG_RESET();
}
#endif /* CONFIG_LCD_LOGO */
#endif

/*----------------------------------------------------------------------*/
#if defined(CONFIG_CMD_BMP) || defined(CONFIG_SPLASH_SCREEN)
/*
 * Display the BMP file located at address bmp_image.
 * Only uncompressed.
 */

#ifdef CONFIG_SPLASH_SCREEN_ALIGN
#define BMP_ALIGN_CENTER	0x7FFF
#endif

int lcd_display_bitmap(ulong bmp_image, int x, int y)
{
	ushort *cmap_base = NULL;
	ushort i, j;
	uchar *fb;
	bmp_image_t *bmp=(bmp_image_t *)bmp_image;
	uchar *bmap;
	ushort padded_line;
	unsigned long width, height, byte_width;
	unsigned long pwidth = panel_info.vl_col;
	unsigned colors, bpix, bmp_bpix;
	unsigned long compression;

	if (!((bmp->header.signature[0]=='B') &&
		(bmp->header.signature[1]=='M'))) {
		printf ("Error: no valid bmp image at %lx\n", bmp_image);
		return 1;
	}

	width = le32_to_cpu (bmp->header.width);
	height = le32_to_cpu (bmp->header.height);
	bmp_bpix = le16_to_cpu(bmp->header.bit_count);
	colors = 1 << bmp_bpix;
	compression = le32_to_cpu (bmp->header.compression);

	bpix = NBITS(panel_info.vl_bpix);

	if((x == -1) &&(y == -1))
	{
		if((width > panel_info.vl_col) || (height > panel_info.vl_row))
		{
			x = 0;
			y = 0;
		}
		else
		{
			x = (panel_info.vl_col - width) / 2;
			y = (panel_info.vl_row - height) / 2;
		}
	}

	if ((bpix != 1) && (bpix != 8) && (bpix != 16) && (bpix != 24)) {
		printf ("Error: %d bit/pixel mode, but BMP has %d bit/pixel\n",
			bpix, bmp_bpix);
		return 1;
	}

	/* We support displaying 8bpp BMPs on 16bpp LCDs */
	if (bpix != bmp_bpix && (bmp_bpix != 8 || bpix != 16)) {
		printf ("Error: %d bit/pixel mode, but BMP has %d bit/pixel\n",
			bpix,
			le16_to_cpu(bmp->header.bit_count));
		return 1;
	}

	debug ("Display-bmp: %d x %d  with %d colors\n",
		(int)width, (int)height, (int)colors);


	/*
	 *  BMP format for Monochrome assumes that the state of a
	 * pixel is described on a per Bit basis, not per Byte.
	 *  So, in case of Monochrome BMP we should align widths
	 * on a byte boundary and convert them from Bit to Byte
	 * units.
	 *  Probably, PXA250 and MPC823 process 1bpp BMP images in
	 * their own ways, so make the converting to be MCC200
	 * specific.
	 */
	padded_line = (width&0x3) ? ((width&~0x3)+4) : (width);

#ifdef CONFIG_SPLASH_SCREEN_ALIGN
	if (x == BMP_ALIGN_CENTER)
		x = max(0, (pwidth - width) / 2);
	else if (x < 0)
		x = max(0, pwidth - width + x + 1);

	if (y == BMP_ALIGN_CENTER)
		y = max(0, (panel_info.vl_row - height) / 2);
	else if (y < 0)
		y = max(0, panel_info.vl_row - height + y + 1);
#endif /* CONFIG_SPLASH_SCREEN_ALIGN */

	if ((x + width)>pwidth)
		width = pwidth - x;
	if ((y + height)>panel_info.vl_row)
		height = panel_info.vl_row - y;

	bmap = (uchar *)bmp + le32_to_cpu (bmp->header.data_offset);
	fb   = (uchar *) (panel_info.lcd_base +
		(y + height - 1) * lcd_line_length + x*(LCD_BPP/8));

	debug("fb=0x%08x; bmap=0x%08x, width=%d, height= %d, lcd_line_length=%d, padded_line=%d\n",
			fb, bmap, width, height, lcd_line_length, padded_line);
	switch (bmp_bpix) {
#if(LCD_BPP ==LCD_COLOR8)
	case 8:
		if (bpix != 16)
			byte_width = width;
		else
			byte_width = width * 2;

		for (i = 0; i < height; ++i) {
			for (j = 0; j < width; j++) {
				if (bpix != 16) {
					*(fb++) = *(bmap++);
				} else {
					*(uint16_t *)fb = cmap_base[*(bmap++)];
					fb += sizeof(uint16_t) / sizeof(*fb);
				}
			}
			bmap += (width - padded_line);
			fb   -= (byte_width + lcd_line_length);
		}
		break;
#endif /* LCD_BPP ==LCD_COLOR8 */

#if(LCD_BPP ==LCD_COLOR16)
	case 16:
		for (i = 0; i < height; ++i) {
			for (j = 0; j < width; j++) {

				*(fb++) = *(bmap++);
				*(fb++) = *(bmap++);
			}
			bmap += (padded_line - width) * 2;
			fb   -= (width * 2 + lcd_line_length);
		}
		break;
#endif /* LCD_BPP ==LCD_COLOR16 */

#if(LCD_BPP ==LCD_COLOR24)
	case 24:
		for (i = 0; i < height; ++i) {
			for (j = 0; j < width; j++) {

				*(fb++) = *(bmap++);
				*(fb++) = *(bmap++);
				*(fb++) = *(bmap++);
			}
			bmap += (padded_line - width);
			fb   -= (width * 3 + lcd_line_length);
		}
		break;
#endif /* LCD_BPP ==LCD_COLOR24 */

	default:
		printf("ERROR LCD_BPP is %d\n", LCD_BPP);
		return (-1);
	};
	flush_cache(panel_info.lcd_base, panel_info.vl_col*panel_info.vl_row*panel_info.vl_bpix/8);
	return (0);
}
#endif

static void *lcd_logo (void)
{
#ifdef CONFIG_SPLASH_SCREEN
	char *s;
	ulong addr;
	static int do_splash = 1;

	if (do_splash && (s = getenv("splashimage")) != NULL) {
		int x = 0, y = 0;
		do_splash = 0;

		addr = simple_strtoul (s, NULL, 16);
#ifdef CONFIG_SPLASH_SCREEN_ALIGN
		if ((s = getenv ("splashpos")) != NULL) {
			if (s[0] == 'm')
				x = BMP_ALIGN_CENTER;
			else
				x = simple_strtol (s, NULL, 0);

			if ((s = strchr (s + 1, ',')) != NULL) {
				if (s[1] == 'm')
					y = BMP_ALIGN_CENTER;
				else
					y = simple_strtol (s + 1, NULL, 0);
			}
		}
#endif /* CONFIG_SPLASH_SCREEN_ALIGN */

#ifdef CONFIG_VIDEO_BMP_GZIP
		bmp_image_t *bmp = (bmp_image_t *)addr;
		unsigned long len;

		if (!((bmp->header.signature[0]=='B') &&
		      (bmp->header.signature[1]=='M'))) {
			addr = (ulong)gunzip_bmp(addr, &len);
		}
#endif

		if (lcd_display_bitmap (addr, x, y) == 0) {
			return ((void *)panel_info.lcd_base);
		}
	}
#endif /* CONFIG_SPLASH_SCREEN */

#ifdef CONFIG_LCD_LOGO
	bitmap_plot (0, 0);
#endif /* CONFIG_LCD_LOGO */

#ifdef CONFIG_LCD_INFO
	panel_info.console_col = LCD_INFO_X / VIDEO_FONT_WIDTH;
	panel_info.console_row = LCD_INFO_Y / VIDEO_FONT_HEIGHT;
	lcd_show_board_info();
#endif /* CONFIG_LCD_INFO */

#if defined(CONFIG_LCD_LOGO) && !defined(CONFIG_LCD_INFO_BELOW_LOGO)
	return ((void *)((ulong)panel_info.lcd_base + BMP_LOGO_HEIGHT * lcd_line_length));
#else
	return ((void *)panel_info.lcd_base);
#endif /* CONFIG_LCD_LOGO && !CONFIG_LCD_INFO_BELOW_LOGO */
}

static int do_panel_enable(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	lcd_is_enabled = 1;
	panel_info.lcd_enable();
	return (0);
}

static int do_panel_disable(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	lcd_is_enabled = 0;
	panel_info.lcd_disable();
	return (0);
}

static int do_lcd_info(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	lcd_info();
	return (0);
}

/*----------------------------------------------------------------------*/
static int do_lcd_clear (cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	lcd_clear();
	return (0);
}

static int do_lcd_bl(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	if(argc < 2)
	{
		cmd_usage(cmdtp);
		return 1;
	}

	if(!strcmp(argv[1], "on"))
	{
		printf("LCD backlight on!\n");
		lcd_bl_opt(0xffffffff);
	}
	else if(!strcmp(argv[1], "off"))
	{
		printf("LCD backlight off!\n");
		lcd_bl_opt(0);
	}
	else if(!strcmp(argv[1], "set"))
	{
		if(argc < 3)
		{
		cmd_usage(cmdtp);
		return 1;
		}
		else
		{
			printf("Backlight level is %d now!\n", simple_strtoul(argv[2], NULL, 10));
			lcd_bl_opt(simple_strtoul(argv[2], NULL, 10));
		}
	}
	else
	{
		cmd_usage(cmdtp);
		return 1;
	}
	
	 return (0);
}

static int do_lcd_test (cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
    printf("LCD Test!\n");
    //lcd_drawchars(16, 64, "0123456789", 10);
	lcd_printf("\n");
	lcd_printf("   D   \n");
	lcd_printf("lcd_test: FILE:%s:%d, FUNC:%s\n",\
                                                     __FILE__,__LINE__,__func__);
	
	return (0);
}

static cmd_tbl_t cmd_lcd_sub[] = {
	U_BOOT_CMD_MKENT(enable, 2, 0, do_panel_enable, "", ""),
	U_BOOT_CMD_MKENT(disable, 2, 0, do_panel_disable, "", ""),
	U_BOOT_CMD_MKENT(info, 2, 0, do_lcd_info, "", ""),
	U_BOOT_CMD_MKENT(clear, 2, 0, do_lcd_clear, "", ""),
	U_BOOT_CMD_MKENT(bl, 4, 0, do_lcd_bl, "", ""),
	U_BOOT_CMD_MKENT(test, 2, 0, do_lcd_test, "", ""),
};

/*
 * Subroutine:  do_lcd
 *
 * Description: Handler for 'lcd' command..
 *
 * Inputs:	argv[1] contains the subcommand
 *
 * Return:      None
 *
 */
static int do_lcd(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	cmd_tbl_t *c;

	/* Strip off leading 'bmp' command argument */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], &cmd_lcd_sub[0], ARRAY_SIZE(cmd_lcd_sub));

	if (c) {
		return  c->cmd(cmdtp, flag, argc, argv);
	} else {
		cmd_usage(cmdtp);
		return 1;
	}
}

U_BOOT_CMD(
	lcd,	5,	1,	do_lcd,
	"manipulate LCD function",
	"\nlcd info	- display lcd info\n"
	"lcd enable		- Panel enable\n"
	"lcd disable	- Panel disable\n"
	"lcd clear		- clear screen to black\n"
	"lcd bl on		- power on backlight\n"
	"lcd bl off		- power off backlight\n"
	"lcd bl set <backlight level>	- set backlight\n"
	"lcd test		- test lcd func\n"
);

/************************************************************************/
/************************************************************************/

