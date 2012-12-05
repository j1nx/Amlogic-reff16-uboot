
CROSS_COMPILE=arm-none-eabi-
ARM_CPU=cortex-a9
PLATFORM_CPPFLAGS += -mcpu=$(ARM_CPU) -mthumb-interwork -O -mno-long-calls  -Wall
#end of SRAM on 7266M
#in 6236M , it is same with 0x87800000
TEXT_BASE=0x8F800000
UCL_TEXT_BASE=8F000000
LDSCRIPT := $(SRCTREE)/$(CPUDIR)/m1/u-boot.lds
