#ifndef _BMP_H
#define _BMP_H
#include <common.h>
#include <command.h>
#include <asm/byteorder.h>
#include <malloc.h>
#include <a3_osd_display/lcd_config.h>

//typedef struct bmp_color_table_entry {
//	__u8	blue;
//	__u8	green;
//	__u8	red;
//	__u8	reserved;
//} __attribute__ ((packed)) bmp_color_table_entry_t;

/* When accessing these fields, remember that they are stored in little
   endian format, so use linux macros, e.g. le32_to_cpu(width)          */

typedef struct bmp_header_t {
	/* Header */
	char signature[2];
	__u32	file_size;
	__u32	reserved;
	__u32	data_offset;
	/* InfoHeader */
	__u32	size;
	__u32	width;
	__u32	height;
	__u16	planes;
	__u16	bit_count;
	__u32	compression;
	__u32	image_size;
	__u32	x_pixels_per_m;
	__u32	y_pixels_per_m;
	__u32	colors_used;
	__u32	colors_important;
	/* ColorTable */

}  __attribute__ ((packed)) s_bmp_header_t;

//typedef struct bmp_image {
//	s_bmp_header_t header;
//	/* We use a zero sized array just as a placeholder for variable
//	   sized array */
//	bmp_color_table_entry_t color_table[0];
//} s_bmp_image_t;

extern int convertbmptoRGB655(unsigned long addr,unsigned convertaddr, int x, int y);


//extern int bmp_logo_display(unsigned long addr, int x, int y);
#endif
