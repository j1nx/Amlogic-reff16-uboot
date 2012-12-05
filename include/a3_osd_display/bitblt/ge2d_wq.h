#ifndef BITBLT_H_
#define BITBLT_H_
//#include <ioapi.h>
//#include <Drivers/bitblt/ge2d.h>
#include "ge2d.h"

#define MAX_BITBLT_WORK_CONFIG 8

#define GE2D_HIGHEST_PRIORITY   0
#define GE2D_LOWEST_PRIORITY    255

#define FLAG_CONFIG_UPDATE_ONLY 0
#define FLAG_CONFIG_ALL         1

#define UPDATE_SRC_DATA     0x01
#define UPDATE_SRC_GEN      0x02
#define UPDATE_DST_DATA     0x04
#define UPDATE_DST_GEN      0x08
#define UPDATE_DP_GEN       0x10
#define UPDATE_SCALE_COEF   0x20
#define UPDATE_ALL          0x3f

//typedef struct ge2d_wq_s ge2d_wq_t;
typedef struct ge2d_wq_s {
    ge2d_config_t         config;
    ge2d_cmd_t            cmd;
    unsigned              cmd_cnt;
    unsigned              cmd_idx;  
    ge2d_cmd_t            cmd_queue[MAX_GE2D_CMD];
    ge2d_config_t         cmd_config[MAX_GE2D_CMD];
    unsigned              priority;
    unsigned              state;
//    OS_EVENT             *cmd_queue_mutex;
} ge2d_wq_t;
extern int ge2d_wq_init(void);
extern void ge2d_wq_deinit(void);

extern int ge2d_wq_add_work_config(ge2d_wq_t *ge2d);
extern int ge2d_wq_remove_config(int handle);

extern void ge2d_wq_set_src_data(int handle, ge2d_src1_data_t *src_data);
extern void ge2d_wq_set_src_gen(int handle, ge2d_src1_gen_t *src_gen);
extern void ge2d_wq_set_dst_data(int handle, ge2d_src2_dst_data_t *dst_data);
extern void ge2d_wq_set_dst_gen(int handle, ge2d_src2_dst_gen_t *src2_dst_gen);
extern void ge2d_wq_set_dp_gen(int handle, ge2d_dp_gen_t *dp_gen);
extern void ge2d_wq_set_scale_coef(int handle, unsigned v_scale_coef, unsigned h_scale_coef);

extern ge2d_src1_data_t * ge2d_wq_get_src_data(int handle);
extern ge2d_src1_gen_t * ge2d_wq_get_src_gen(int handle);
extern ge2d_src2_dst_data_t * ge2d_wq_get_dst_data(int handle);
extern ge2d_src2_dst_gen_t * ge2d_wq_get_dst_gen(int handle);
extern ge2d_dp_gen_t * ge2d_wq_get_dp_gen(int handle);
extern ge2d_cmd_t * ge2d_wq_get_cmd(int handle);

extern int ge2d_wq_add_work(int handle);
extern void ge2d_wq_wait_done(int handle);
typedef unsigned short avfs_device_major_number;
typedef unsigned short avfs_device_minor_number;
extern int bitblt_device_init(avfs_device_major_number major, avfs_device_minor_number minor, void *arg);
extern int bitblt_device_ioctl(avfs_device_major_number major, avfs_device_minor_number minor, void *arg);

#endif // BITBLT_H
