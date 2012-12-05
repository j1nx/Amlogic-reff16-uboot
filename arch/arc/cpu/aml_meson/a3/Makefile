

include $(TOPDIR)/config.mk

LIB	=  $(obj)lib$(SOC).a

#SOBJS	:= mmutable.o
COBJS	+= timer.o soc.o  sdio.o
COBJS +=  clock.o pinmux.o clk_set.o gpio.o
COBJS-$(CONFIG_SERIAL_HWUART) +=serial_hw.o
COBJS-$(CONFIG_SERIAL_SWUART) +=serial_sw.o
COBJS-$(CONFIG_SERIAL_NONE) +=serial_none.o

COBJS-$(CONFIG_AML_RTC) +=aml_rtc_setting.o

SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS) $(SOBJS) $(COBJS-y))

all:	 $(obj).depend $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
