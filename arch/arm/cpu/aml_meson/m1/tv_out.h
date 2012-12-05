#ifndef TVOUT_H
#define TVOUT_H

/* same to tv encoder */
enum
{
    TVOUT_480I  = 0,
    TVOUT_480CVBS,
    TVOUT_480P  ,
    TVOUT_576I  ,
    TVOUT_576CVBS,
    TVOUT_576P  ,
    TVOUT_720P  ,
    TVOUT_1080I ,
    TVOUT_1080P ,
    TVOUT_MAX   
};

#define TVOUT_VALID(m) (m < TVOUT_MAX)

int tv_out_open(int mode);
int tv_out_close(void);
int tv_out_cur_mode(void);
int tv_out_get_info(int mode, unsigned *width, unsigned *height);

#endif


