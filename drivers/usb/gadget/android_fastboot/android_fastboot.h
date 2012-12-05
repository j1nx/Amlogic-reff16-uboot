/*
 * Copyright (c) 2011 Amlogic (Shanghai), Inc.
 * Tianhui.Wang <tianhui.wang@amlogic.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#ifndef __ANDROID_FASTBOOT_H__
#define __ANDROID_FASTBOOT_H__

#include <usb/dwc_otg/dwc_otg_udc.h>
#include <usbdevice.h>
#include <usbdescriptors.h>

#define NUM_CONFIGS	1
#define NUM_ENDPOINTS 	2
#define NUM_INTERFACES	1
#define MAX_INTERFACES	1

#define CONFIG_USBD_DATA_INTERFACE_STR "Bulk Data Interface"
#define CONFIG_USBD_CTRL_INTERFACE_STR "Control Interface"
#define BOOT_ARGS_SIZE 	512
#define SIGNATURE_SIZE 	256
#define BOOT_MAGIC_SIZE 8
#define BOOT_NAME_SIZE 16
#define BOOT_MAGIC "ANDROID!"
#define STR_COUNT	4

/* dev_state */
#define FSTB_INIT_PHASE	0
#define FSTB_CMD_PHASE	1
#define FSTB_DATA_PHASE	2
#define FSTB_DIS_PHASE	3

struct fastboot_dev {
	char *name;
	struct usb_device_instance *device;
	struct udc_driver *udc_driver;
	char dev_state;
};
struct fastboot_config_desc {
	struct usb_configuration_descriptor configuration_desc;
	struct usb_interface_descriptor interface_desc;
	struct usb_endpoint_descriptor ep[NUM_ENDPOINTS];
} __attribute__((packed));

struct usb_device_descriptor device_descriptor = {
	sizeof(struct usb_device_descriptor),
	USB_DT_DEVICE,
	cpu_to_le16(USB_BCD_VERSION),
	0x00,            // class
	0x00,            // subclass
	0x00,            // protocol
	0x40,            // max packet size
	0x1B8E,      // vendor id  -- Amlogic
	0xd00d,      // product id
	0x0100,      // version 1.0
	0x01,            // manufacturer str idx
	0x02,            // product str idx
	0x03,            // serial number index
	0x01,            // number of configs,
};

struct fastboot_config_desc config_desc_fs = {
	.configuration_desc = {
		0x09,            // length
		USB_DT_CONFIG,
		0x0020,      // total length
		0x01,            // # interfaces
		0x01,            // config value
		0x00,            // config string
		0x80,            // attributes
		0x80,            // XXX max power (250ma)
	},
	.interface_desc = {
		0x09,            // length
		USB_DT_INTERFACE,
		0x00,            // interface number
		0x00,            // alt number
		0x02,            // # endpoints
		0xFF,
		0x42,
		0x03,
		0x00,            // interface string
	},
	.ep = {
		{	0x07,            // length
			USB_DT_ENDPOINT,
			UDC_IN_EP_FASTBOOT | USB_DIR_IN, 
			0x02,            // bulk
			UDC_IN_PACKET_SIZE,
			0x00,            // interval
    		},
		{	0x07,            // length
			USB_DT_ENDPOINT,
			UDC_OUT_EP_FASTBOOT | USB_DIR_OUT,
			0x02,            // bulk
			UDC_OUT_PACKET_SIZE,
			0x00,            // interval
		},
	},
};
    
static char fastboot_string_vid[16]=	{16,USB_DT_STRING,'A',0,'m',0,'l',0,'o',0,'g',0,'i',0,'c',0};
static char fastboot_string_pid[18]=	{18,USB_DT_STRING,'f',0,'a',0,'s',0,'t',0,'b',0,'o',0,'o',0,'t',0};
static char fastboot_string_serial[18]=	{18,USB_DT_STRING,'u',0,'b',0,'o',0,'o',0,'t',0,'-',0,'0',0,'1',0};

/* the struct include the information of target-system partitions */
typedef struct mtd_partition {
	char *name;			/* identifier string */
	unsigned long offset;		/* offset within the master MTD space */
	unsigned long size;		/* partition size */
	unsigned int  flags;
}ptentry;

/* example based on board-8726m-refb06.c. you can modify it for your project */
unsigned kernel_addr = 0x82000000;

static struct mtd_partition inand_partition_info[] = 
{
	{
		.name = "bootloader",
		.offset = 0,
		.size=4*1024*1024,
	},
	{
		.name = "environment",
		.offset = 4*1024*1024,
		.size=4*1024*1024,
	},
	{
		.name = "splash",
		.offset = 8*1024*1024,
		.size=4*1024*1024,
	},
	{
		.name = "recovery",
		.offset = 12*1024*1024,
		.size = 16 * 1024*1024,
	},
	{
		.name = "boot",
		.offset = 28*1024*1024,
		.size = 16 * 1024*1024,
	},
	{
		.name = "system",
		.offset = 44*4*1024*1024,
		.size = 240 * 1024*1024,
	},
	{
		.name = "cache",
		.offset = 416*1024*1024,
		.size = 16 * 1024*1024,
	},
	{
		.name = "userdata",
		.offset= 432*1024*1024,
		.size= 256 * 1024*1024,
	},
#if 0
	{
		.name = "media",
		.offset = MTDPART_OFS_APPEND,
		.size = (0x200000000-(432+256)*1024*1024),
	},
#endif
};
#endif
