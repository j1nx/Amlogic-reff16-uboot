
include $(TOPDIR)/config.mk
ifeq ($(CONFIG_ENABLE_ARMCC),y)
targetComplier=arm
else
targetComplier=gcc
endif

all:	  
	$(MAKE) -f Makefile.$(targetComplier)

