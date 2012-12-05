/*
 *  Copyright (C) 2010 AMLOGIC, INC.
 *
 *  Y:\arch\arm\include\asm\arch-m1\serial.h
 * 
 *
 * License terms: GNU General Public License (GPL) version 2
 * Basic register address definitions in physical memory and
 * some block defintions for core devices like the timer.
 * 03/06/10
 *
 * author : jerry.yu
 */
#ifndef __MESON_FIRM_CONFIG_H_
#define __MESON_FIRM_CONFIG_H_
#ifndef FIRMWARE_IN_ONE_FILE
#define STATIC_PREFIX
#else
#define STATIC_PREFIX static
#endif


//UART Section
#if CONFIG_AML_DEF_UART==UART_B

#define BOARD_UART_PORT UART_PORT_1
#endif
#if CONFIG_AML_DEF_UART==UART_A
#define BOARD_UART_PORT UART_PORT_0
#endif

#endif
