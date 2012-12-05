#
#  Copyright Codito Technologies (www.codito.com)
#
#  cpu/arc/Makefile
#
#  Copyright (C) 
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
# 
# Authors : Sandeep Patil (sandeep.patil@codito.com) 
# 			Pradeep Sawlani (pradeep.sawlani@codito.com) 
#


include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(CPU).a

START	= start.o 
#boot_start.o nand_start_a1h.o sd_io.o jtag-console.o
OBJS-y	= cpu.o entry.o 
OBJS-$(CONFIG_JTAG_CONSOLE) += jtag-console.o
#interrupts.o entry.o irq.o traps.o

OBJS	:= $(addprefix $(obj),$(OBJS-y))
START	:= $(addprefix $(obj),$(START))

all:	$(obj).depend $(START) $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
