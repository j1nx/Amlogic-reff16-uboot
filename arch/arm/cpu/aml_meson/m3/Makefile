

include $(TOPDIR)/config.mk

LIB	=  $(obj)lib$(SOC).a

SOBJS	:= mmutable.o
COBJS	+= timer.o soc.o board.o sdio.o
COBJS += serial.o clock.o pinmux.o clk_set.o gpio.o 

ifdef CONFIG_AMLOSD
COBJS += canvas.o osd_hw.o
endif

ifdef CONFIG_LCD_AML
COBJS += canvas.o osd_hw.o
endif
ifdef CONFIG_LCD_AML_TTL
COBJS += tcon.o
endif
ifdef CONFIG_LCD_AML_LVDS
COBJS += lvds.o
endif
ifdef CONFIG_LCD_AML_MINI_LVDS
COBJS += mlvds.o
endif

ifdef CONFIG_AMLVIDEO
COBJS += tv_out.o
endif
 
ifdef CONFIG_AMLHDMI
COBJS += hdmi_tx_m1b.o hdmi_tx_reg.o hdmi_tx.o hdmi_tx_edid.o hdmi_tx_video.o dsp.o
endif

ifdef CONFIG_USB_DWC_OTG_HCD
COBJS += usb.o
endif

COBJS += cpu.o

ifdef CONFIG_AML_RTC
COBJS += aml_rtc_setting.o
endif

ifdef CONFIG_AML_SUSPEND
COBJS += firmwareld.o
endif

ifdef CONFIG_AML_SECURE
COBJS += secure_firmwareld.o
endif

SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS) $(SOBJS))

all:	power_firmware secure_firmware  $(obj).depend $(LIB)


$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

ifdef CONFIG_AML_SUSPEND
FIRMWARE_AUTO_UPDATE=y
export FIRMWARE_AUTO_UPDATE
power_firmware:
		make -C $(SRCTREE)/$(CPUDIR)/$(SOC)/firmware/arc_power
		make -C $(SRCTREE)/$(CPUDIR)/$(SOC)/firmware/appf
ifdef CONFIG_ENABLE_ARC
		make -C $(SRCTREE)/$(CPUDIR)/$(SOC)/firmware/arc_testpd
endif
else
power_firmware:
ifdef CONFIG_ENABLE_ARC
		make -C $(SRCTREE)/$(CPUDIR)/$(SOC)/firmware/arc_testpd
endif
endif 

.PHONY :       secure_firmware
secure_firmware:
ifdef CONFIG_AML_SECURE
	make -C $(SRCTREE)/$(CPUDIR)/$(SOC)/firmware/secure
endif

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
