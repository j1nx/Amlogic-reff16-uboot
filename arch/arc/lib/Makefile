#
# Copyright Codito Technologies (www.codito.com)
#
#  lib_arc/Makefile
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

LIB	= $(obj)lib$(ARCH).a
COBJS-$(CONFIG_CMD_KGDB)+= kgdb.o
AOBJS	=

COBJS	= board.o arclinux.o cache.o
#chipsupport.o serial.o ethernet.o sd_mmc.o
COBJS	+= arcavos.o interrupts.o
#minilzo.o
OBJS	= $(AOBJS) $(COBJS) $(sort $(COBJS-y))
OBJS	:= $(addprefix $(obj),$(OBJS))

$(LIB):	$(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
