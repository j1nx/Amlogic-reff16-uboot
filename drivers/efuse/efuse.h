#ifndef __EFUSE_H
#define __EFUSE_H

#define EFUSE_LICENSE_BYTES		4
#define EFUSE_MACADDR_BYTES		7
#define EFUSE_HDMHDCP_BYTES		310
#define EFUSE_USERIDF_BYTES		62
//#define EFUSE_BYTES         	384

#define USR_LICENCE		1
#define USR_MACADDR		2
#define USR_HDMIHDCP		3
#define USR_USERIDF		4

unsigned char *efuse_read_usr(int usr_type);
int efuse_write_usr(int usr_type, unsigned char *data);

int efuse_init(void);

#endif

