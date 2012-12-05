
include $(TOPDIR)/config.mk
ifeq ($(CONFIG_ENABLE_ARMCC),y)
targetComplier=arm
else
targetComplier=gcc
endif

all:	  
	$(MAKE) -f Makefile.$(targetComplier)

ifeq ($(CONFIG_ENABLE_ARC),y)
	$(MAKE) -f Makefile.rbt
endif

#ifeq ($(CONFIG_AML_SUSPEND),y)
#	$(MAKE) -C $(SRCTREE)/$(CPUDIR)/$(SOC)/firmware/appf
#endif
