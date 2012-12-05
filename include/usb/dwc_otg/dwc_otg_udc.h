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
#ifndef __DWC_OTG_UDC_H__
#define __DWC_OTG_UDC_H__

#include <usbdevice.h>

/* UDC level routines */
void udc_irq(void);
void udc_set_nak(int ep_num);
void udc_unset_nak(int ep_num);
int udc_endpoint_write(struct usb_endpoint_instance *endpoint);
void udc_setup_ep(struct usb_device_instance *device, unsigned int id,
		  struct usb_endpoint_instance *endpoint);
void udc_connect(void);
void udc_disconnect(void);
void udc_enable(struct usb_device_instance *device);
void udc_disable(void);
void udc_startup_events(struct usb_device_instance *device);
int udc_init(void);

#define EP0_MAX_PACKET_SIZE	64 /* DWC_OTG_EP0_FIFOSIZE */
#define UDC_IN_PACKET_SIZE	64
#define UDC_OUT_PACKET_SIZE	64
#define UDC_INT_PACKET_SIZE	64
#define UDC_BULK_PACKET_SIZE	64
/* usbtty */
#ifdef CONFIG_USB_TTY
#define UDC_IN_ENDPOINT		1
#define UDC_OUT_ENDPOINT	1
#define UDC_INT_ENDPOINT	2
#endif
/* android fastboot */
//#ifdef CONFIG_FASTBOOT
#define UDC_IN_EP_FASTBOOT	3
#define UDC_OUT_EP_FASTBOOT	3
//#endif

/* 
 * Add some new functions for callback drivers 
 * usage example: android_fastboot.
 */
struct udc_driver {
	char			*driver_name;
	int			(*bind)(struct usb_device_instance *);	/* require resource for driver */
	void			(*unbind)(struct usb_device_instance *);/* release resource for driver */
	int			(*setup)(struct urb *);
	void			(*complete)(struct urb *,int,int);
	struct usb_device_instance *usb_dev;
	void 			*private_data;
};
/* 
 * just for bulk-out transfer ,
 * it will recv no more than ep_maxsize_packet bytes from host 
 * please use it while you want to manage your OUT-transfer.
 * if ep_num = 0, udc will set all out-ep enable.
 * if len = 0, udc will set the ep_maxsize_packet for the assigned ep
 */
int ep_out_start(int len, int ep_num);
/*
 * enable endpoints for your usb-interface 
 * when device is in setup-setconfig phase
 */
void udc_ep_enable(struct usb_endpoint_instance *endpoint);
/* register a new drv for udc */
int udc_register_driver(struct udc_driver* udc_driver);
int udc_unregister_driver(struct udc_driver* udc_driver);

#endif /* __DWC_OTG_UDC_H__ */
