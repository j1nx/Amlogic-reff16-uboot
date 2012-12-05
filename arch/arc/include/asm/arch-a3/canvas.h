#ifndef CANVAS_H
#define CANVAS_H

#include <linux/types.h>

typedef struct {
    ulong addr;
    u32 width;
    u32 height;
    u32 wrap;
    u32 blkmode;
} canvas_t;

#define OSD1_CANVAS_INDEX 0x40
#define OSD2_CANVAS_INDEX 0x43
#define ALLOC_CANVAS_INDEX  0x46

#define GE2D_MAX_CANVAS_INDEX   0x5f

#define DISPLAY_CANVAS_BASE_INDEX   0x60
#define DISPLAY_CANVAS_MAX_INDEX    0x65 

#define DEINTERLACE_CANVAS_BASE_INDEX	0x78
#define DEINTERLACE_CANVAS_MAX_INDEX	0x7f

extern void canvas_config(u32 index, ulong addr, u32 width,
                          u32 height, u32 wrap, u32 blkmode);

extern void canvas_read(u32 index, canvas_t *p);

extern void canvas_copy(unsigned src, unsigned dst);

extern void canvas_update_addr(u32 index, u32 addr);

extern unsigned int canvas_get_addr(u32 index);

#endif /* CANVAS_H */

