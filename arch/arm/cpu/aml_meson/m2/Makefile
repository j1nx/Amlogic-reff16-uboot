

include $(TOPDIR)/config.mk

LIB	=  $(obj)lib$(SOC).a

SOBJS	:= mmutable.o
COBJS	+= timer.o soc.o board.o sdio.o
COBJS   += serial.o clock.o pinmux.o clk_set.o gpio.o


ifdef CONFIG_AMLOSD
COBJS += canvas.o osd_hw.o
endif

ifdef CONFIG_USB_DWC_OTG_HCD
COBJS += usb.o
endif

COBJS += cpu.o

ifdef CONFIG_AML_RTC
COBJS += aml_rtc_setting.o
endif

SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS) $(SOBJS))

all:	 $(obj).depend $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
